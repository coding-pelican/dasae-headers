#include "engine/platform.h"
#include "dh/log.h"
#include "engine/platform_backend.h"
#include "engine/window.h"
#include "engine/canvas.h"
#include "engine/input.h"

#include <windows.h>

// Win32 console backend
typedef struct engine_Win32ConsoleBackend {
    engine_RenderBackend base;
    HANDLE               output_handle; // Handle to console screen buffer
    HANDLE               input_handle;  // Handle to console screen buffer
    HWND                 window_handle; // Handle to console window
    char*                buffer;
    usize                buffer_size;
    usize                buffer_capacity;
    bool                 cursor_visible;
    engine_WindowMetrics last_metrics; // Cache of last known metrics
} engine_Win32ConsoleBackend;


#define Win32ConsoleBackend_calculateBufferSize(width, height) ((width) * (height) * 40)
static void                     Win32ConsoleBackend_destroy(engine_Platform* platform);
static void                     Win32ConsoleBackend_processEvents(engine_Platform* platform);
static void                     Win32ConsoleBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height);
static Opt$engine_WindowMetrics Win32ConsoleBackend_getWindowMetrics(engine_Platform* platform);

static Vec2i Win32ConsoleBackend_getPixelPosition(const MOUSE_EVENT_RECORD* mer, HANDLE output_handle);
static void  Win32ConsoleBackend_enableMouseInput(engine_Win32ConsoleBackend* backend);
static void  Win32ConsoleBackend_disableMouseInput(engine_Win32ConsoleBackend* backend);
static void  Win32ConsoleBackend_processMouseEvent(engine_Win32ConsoleBackend* backend, const MOUSE_EVENT_RECORD* mer);

const var test = Win32ConsoleBackend_calculateBufferSize(80, 50);

Err$Ptr$engine_Platform engine_Platform_create(const engine_PlatformParams* params) {
    reserveReturn(Err$Ptr$engine_Platform);

    engine_Platform* const platform = (engine_Platform*)malloc(sizeof(engine_Platform));
    if (!platform) {
        return_err(mem_AllocErr_OutOfMemory());
    }

    switch (params->backend_type) {
    case engine_RenderBackendType_vt100: {
        engine_Win32ConsoleBackend* const backend = (engine_Win32ConsoleBackend*)malloc(sizeof(engine_Win32ConsoleBackend));
        if (!backend) {
            free(platform);
            return_err(mem_AllocErr_OutOfMemory());
        }
        // Initialize console backend
        // HANDLE hConsole = CreateConsoleScreenBuffer(
        //     GENERIC_READ | GENERIC_WRITE,
        //     FILE_SHARE_READ | FILE_SHARE_WRITE,
        //     null,
        //     CONSOLE_TEXTMODE_BUFFER,
        //     null
        // );
        // if (hConsole == INVALID_HANDLE_VALUE) {
        //     free(backend);
        //     free(platform);
        //     return_err(engine_PlatformErr_err(engine_PlatformErrCode_AccessDenied));
        // }

        // Initialize console backend
        HANDLE     hOutput           = GetStdHandle(STD_OUTPUT_HANDLE);
        SMALL_RECT windowSizeInitial = (SMALL_RECT){ 0, 0, 1, 1 };
        SetConsoleWindowInfo(hOutput, true, &windowSizeInitial);

        // Get console window handle
        backend->window_handle = GetConsoleWindow();

        // Configure buffer size
        COORD dwSize = (COORD){ (SHORT)params->width, (SHORT)params->height };
        SetConsoleScreenBufferSize(hOutput, dwSize);

        SMALL_RECT windowSize = (SMALL_RECT){ 0, 0, (SHORT)(params->width), (SHORT)(params->height) };
        SetConsoleWindowInfo(hOutput, true, &windowSize);

        // Initialize last known metrics
        backend->last_metrics = (engine_WindowMetrics){
            .width         = params->width,
            .height        = params->height,
            .min_width     = params->width,
            .min_height    = params->height,
            .client_width  = params->width,
            .client_height = params->height,
            .x             = 0,
            .y             = 0,
            .is_focused    = true,
            .is_minimized  = false,
            .is_maximized  = false
        };

        // Configure console
        SetConsoleOutputCP(CP_UTF8);

        // Configure console output mode
        DWORD out_mode = 0;
        GetConsoleMode(hOutput, &out_mode);
        out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
        SetConsoleMode(hOutput, out_mode);

        // Get and configure input handle
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        if (hInput != INVALID_HANDLE_VALUE) {
            DWORD in_mode = 0;
            GetConsoleMode(hInput, &in_mode);
            in_mode |= ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
            SetConsoleMode(hInput, in_mode);

            // Store input handle in backend
            backend->input_handle = hInput;
        }

        // Hide cursor
        CONSOLE_CURSOR_INFO cursor_info = { 1, false };
        SetConsoleCursorInfo(hOutput, &cursor_info);

        // Allocate string buffer for ANSI sequences
        backend->buffer_capacity = (usize)Win32ConsoleBackend_calculateBufferSize((usize)params->width, (usize)params->height);
        backend->buffer          = malloc(backend->buffer_capacity);
        if (!backend->buffer) {
            free(backend);
            free(platform);
            return_err(mem_AllocErr_OutOfMemory());
        }

        backend->output_handle  = hOutput;
        backend->buffer_size    = 0;
        backend->cursor_visible = false;

        // Setup backend interface
        backend->base = eval({
            var vt              = engine_RenderBackend_createNoOp();
            vt.type             = engine_RenderBackendType_vt100;
            vt.destroy          = Win32ConsoleBackend_destroy;
            vt.processEvents    = Win32ConsoleBackend_processEvents;
            vt.presentBuffer    = Win32ConsoleBackend_presentBuffer;
            vt.getWindowMetrics = Win32ConsoleBackend_getWindowMetrics;
            eval_return vt;
        });
        Win32ConsoleBackend_enableMouseInput(backend);

        platform->backend = &backend->base;
        return_ok(platform);
    }

    case engine_RenderBackendType_win32_gdi:
    case engine_RenderBackendType_directx:
        free(platform);
        return_err(Err_NotImplemented());

    case engine_RenderBackendType_custom:
        if (!params->custom_data) {
            free(platform);
            return_err(Err_InvalidArgument());
        }
        platform->backend = params->custom_data;
        return_ok(platform);

    default:
        free(platform);
        return_err(Err_InvalidArgument());
    }
}

void engine_Platform_destroy(engine_Platform* platform) {
    if (!platform) { return; }
    if (platform->backend && platform->backend->destroy) {
        platform->backend->destroy(platform);
    }
}



static void Win32ConsoleBackend_destroy(engine_Platform* platform) {
    engine_Win32ConsoleBackend* backend = (engine_Win32ConsoleBackend*)platform->backend;
    if (backend) {
        // Disable mouse input
        Win32ConsoleBackend_disableMouseInput(backend);
        // Free buffer
        if (backend->buffer) {
            free(backend->buffer);
        }
        // Restore console state
        if (!backend->cursor_visible) {
            CONSOLE_CURSOR_INFO cursor_info = { 1, true };
            SetConsoleCursorInfo(backend->output_handle, &cursor_info);
        }
        // Free backend
        free(backend);
    }
    // Free platform
    free(platform);

    /* Reset cursor position */ {
        static const char* const reset_cursor = "\033[H";
        printf("%s", reset_cursor);
        // memcpy(backend->buffer + backend->buffer_size, reset_cursor, strlen(reset_cursor));
        // backend->buffer_size += strlen(reset_cursor);
    }
}

static i32  prev_mouse_x = 0;
static i32  prev_mouse_y = 0;
static void Win32ConsoleBackend_processEvents(engine_Platform* platform) {
    let backend = (engine_Win32ConsoleBackend*)platform->backend;

    // Process window metrics
    if_some (Win32ConsoleBackend_getWindowMetrics(platform), current_metrics) {
        if (!engine_WindowMetrics_eq(&backend->last_metrics, &current_metrics)) {
            if (current_metrics.client_width != backend->last_metrics.client_width
                || current_metrics.client_height != backend->last_metrics.client_height) {
                COORD new_size = {
                    (SHORT)current_metrics.client_width,
                    (SHORT)current_metrics.client_height
                };
                SetConsoleScreenBufferSize(backend->input_handle, new_size);
            }
            backend->last_metrics = current_metrics;
        }
    }

    // Process mouse and keyboard events
    INPUT_RECORD input_record[32] = cleared();
    DWORD        events_read      = 0;

    bool occurs_mouse_event = false;
    while (PeekConsoleInput(backend->input_handle, input_record, 32, &events_read) && events_read > 0) {
        ReadConsoleInput(backend->input_handle, input_record, events_read, &events_read);

        for (DWORD i = 0; i < events_read; ++i) {
            switch (input_record[i].EventType) {
            case MOUSE_EVENT:
                Win32ConsoleBackend_processMouseEvent(backend, &input_record[i].Event.MouseEvent);
                occurs_mouse_event = true;
                break;

            case WINDOW_BUFFER_SIZE_EVENT:
                // Handle through metrics update
                break;

            case FOCUS_EVENT:
                // Handle focus event
                backend->last_metrics.is_focused = input_record[i].Event.FocusEvent.bSetFocus;
                break;
            default:
                break;
            }
        }
    }

    /* static int prev_cell_y  = 0;
    static int prev_subcell = 0;
    if (!occurs_mouse_event) {
        // 1) Get the actual console font size
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo = { 0 };
        CONSOLE_FONT_INFOEX        fontInfo   = { .cbSize = sizeof(CONSOLE_FONT_INFOEX) };

        GetConsoleScreenBufferInfo(backend->output_handle, &bufferInfo);
        GetCurrentConsoleFontEx(backend->output_handle, false, &fontInfo);

        // 2) Get the absolute mouse position on screen
        POINT mousePos = { 0 };
        GetCursorPos(&mousePos);

        // 3) Convert client-area origin to screen coordinates
        HWND  consoleWnd   = GetConsoleWindow();
        POINT clientOrigin = { 0, 0 };
        ClientToScreen(consoleWnd, &clientOrigin); // now clientOrigin = screen coords of the text-area (0,0)

        // 4) Compute relativeY = mouse’s Y coordinate minus client-top
        int relativeY = mousePos.y - clientOrigin.y;

        // 5) The total height of one cell in pixels
        int cellHeight = fontInfo.dwFontSize.Y;

        // 6) Which sub-cell are we in? (0 = upper, 1 = lower)
        //    If cellHeight is e.g. 16, then row 0..7 is “upper half,” 8..15 “lower half.”
        int subCell = (relativeY % cellHeight) >= (cellHeight / 2) ? 1 : 0;

        // 7) Decide if we “bump” the console cell Y up or down.
        //    Example: If we detect a transition from subCell=0 to subCell=1, and the mouse is moving downward, we do +1.
        //    If from 1 to 0, and the mouse is moving upward, we do -1.
        //    In other words:
        int dy_screen = mousePos.y - prev_mouse_y; // how far did the mouse move in actual screen coords?

        // Clone your logic:
        //  if previous_subcell == 0 (upper pixel)
        //      and dy_screen > 0 (moving down) -> y++ (transition upper->lower)
        //  else if previous_subcell == 1 (lower pixel)
        //      and dy_screen < 0 (moving up) -> y-- (transition lower->upper)

        int new_cell_y = prev_cell_y;
        if (prev_subcell == 0 && subCell == 1 && dy_screen > 0) {
            new_cell_y = prev_cell_y + 1;
        } else if (prev_subcell == 1 && subCell == 0 && dy_screen < 0) {
            new_cell_y = prev_cell_y - 1;
        }

        // 8) Update our ‘input->mouse.y’ (in “double-height space,” if that’s how your engine wants it).
        //    i.e. we store “row*2 + subCell” if you prefer a “pixel-like” coordinate,
        //    or just store (row) plus a half offset.
        scope_with(let input = engine_Input_instance()) {
            input->mouse.prev_y = input->mouse.y;
            input->mouse.y      = (new_cell_y * 2) + subCell;
            // The X coordinate could be done similarly if you want 2-wide cells, etc.
        }

        // 9) Remember these for next time
        prev_cell_y  = new_cell_y;
        prev_subcell = subCell;
        prev_mouse_y = mousePos.y;
    }
 */
    if (!occurs_mouse_event) {
        // 1) Get the actual console font size
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo = { 0 };
        CONSOLE_FONT_INFOEX        fontInfo   = { .cbSize = sizeof(CONSOLE_FONT_INFOEX) };

        GetConsoleScreenBufferInfo(backend->output_handle, &bufferInfo);
        GetCurrentConsoleFontEx(backend->output_handle, false, &fontInfo);

        // 2) Get the absolute mouse position on screen
        POINT mousePos = { 0 };
        GetCursorPos(&mousePos);

        // 3) Get console window rect
        HWND consoleWnd  = GetConsoleWindow();
        RECT consoleRect = { 0 };
        GetWindowRect(consoleWnd, &consoleRect);

        // 1. 커서 0,0이 위치하는 스크린 좌표와 Window rect 기준 0,0 사이의 오프셋을 구한다.
        // 2. 콘솔 마우스 이벤트가 발생하지 않았으나, 마우스의 y좌표가 이전 마우스 y좌표와 다르다면 다음 과정을 수행한다.
        //   if 이전 마우스 콘솔 커서 == upper pixel
        //      이전 마우스 y와 현재 마우스 y 사이의 delta 값이 양수인 경우에만 +1 (upper->lower)
        //   else
        //      이전 마우스 y와 현재 마우스 y 사이의 delta 값이 음수인 경우에만 -1 (lower->upper)

        // 4) Convert that to a 0-based “cell coordinate”
        int cellHeight = fontInfo.dwFontSize.Y;
        // Relative mouse coordinates inside the console window
        int relativeY  = mousePos.y - consoleRect.top;
        // Which console column/row is that? (integer division)
        int consoleY   = prev_mouse_y;

        // 5) Figure out top/bottom half
        if (consoleY & 1) { // is odd

        } else {
        }
        int yRemainder    = relativeY % cellHeight;
        int subCellOffset = (yRemainder >= cellHeight / 2) ? 1 : 0;
        int doubleHeightY = (consoleY * 2) + subCellOffset;

        // 6) Update the input system
        scope_with(let input = engine_Input_instance()) {
            input->mouse.prev_y = input->mouse.y;
            input->mouse.y      = doubleHeightY; // or clamp as needed
        }
    }
}

static void Win32ConsoleBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height) {
    debug_assert_nonnull(platform);
    debug_assert_nonnull(data);
    let backend = (engine_Win32ConsoleBackend*)platform->backend;
    let pixels  = data;

    /* Reset buffer */ {
        backend->buffer_size = 0;
    }

    for (u32 y = 0; (y + 1) < height; y += 2) {
        for (u32 x = 0; x < width; ++x) {
            // Get upper and lower pixels
            const Color upper = pixels[y * width + x];
            const Color lower = pixels[(y + 1) * width + x];

            // clang-format off
            // Find run length of identical color pairs
            usize run_length = 1;
            while ((x + run_length) < width) {
                const Color next_upper = pixels[y * width + x + run_length];
                const Color next_lower = pixels[(y + 1) * width + x + run_length];

                if (memcmp(&upper.channels, &next_upper.channels, sizeof(Color)) != 0
                 || memcmp(&lower.channels, &next_lower.channels, sizeof(Color)) != 0) {
                    break;
                }
                run_length++;
            }

            // Write color sequence
            backend->buffer_size += sprintf(
                backend->buffer + backend->buffer_size,
                "\033[38;2;%d;%d;%d;48;2;%d;%d;%dm",
                upper.r, upper.g, upper.b,
                lower.r, lower.g, lower.b
            );
            // clang-format on

            // Write half-blocks for the run
            for (usize i = 0; i < run_length; ++i) {
                memcpy(backend->buffer + backend->buffer_size, "▀", strlen("▀"));
                backend->buffer_size += strlen("▀");
            }

            // NOTE: 으아 ㅠㅠㅠ 드디어 고쳐냈다 ㅠㅠ 길이를 1개 감소시키지 않았다고 화면이 와장창 깨지다니...
            x += run_length - 1; // -1 because loop will increment
        }
        backend->buffer[backend->buffer_size++] = '\n';
    }
    backend->buffer[--backend->buffer_size] = '\0';

    // /* Reset colors */ {
    //     const char* reset_colors = "\033[0m";
    //     memcpy(backend->buffer + backend->buffer_size, reset_colors, strlen(reset_colors));
    //     backend->buffer_size += strlen(reset_colors);
    // }

    // /* Clear console */ {
    //     HANDLE                     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //     COORD                      topLeft  = { 0, 0 };
    //     CONSOLE_SCREEN_BUFFER_INFO screen   = { 0 };
    //     DWORD                      written  = { 0 };

    //     GetConsoleScreenBufferInfo(hConsole, &screen);
    //     FillConsoleOutputCharacter(hConsole, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    //     FillConsoleOutputAttribute(hConsole, 0, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    //     // FillConsoleOutputAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    //     SetConsoleCursorPosition(hConsole, topLeft);
    // }

    /* Reset cursor position */ {
        static const char* const reset_cursor = "\033[H";
        printf("%s", reset_cursor);
        // memcpy(backend->buffer + backend->buffer_size, reset_cursor, strlen(reset_cursor));
        // backend->buffer_size += strlen(reset_cursor);
    }

    /* Write to console */ {
        DWORD written = 0;
        WriteConsoleA(
            backend->output_handle,
            backend->buffer,
            (DWORD)backend->buffer_size,
            &written,
            null
        );
    }
}

static Opt$engine_WindowMetrics Win32ConsoleBackend_getWindowMetrics(engine_Platform* platform) {
    reserveReturn(Opt$engine_WindowMetrics);
    debug_assert_nonnull(platform);

    let backend = (engine_Win32ConsoleBackend*)platform->backend;
    if (!backend->window_handle) {
        return_none();
    }

    // Get window placement info
    WINDOWPLACEMENT placement = { .length = sizeof(WINDOWPLACEMENT), .flags = 0 };
    if (!GetWindowPlacement(backend->window_handle, &placement)) {
        return_none();
    }

    // Get window rect for position and overall size
    RECT windowRect = placement.rcNormalPosition;

    // Get client rect for client area size
    RECT clientRect = cleared();
    if (!GetClientRect(backend->window_handle, &clientRect)) {
        return_none();
    }

    // Update metrics
    let metrics = (engine_WindowMetrics){
        .x             = windowRect.left,
        .y             = windowRect.top,
        .width         = windowRect.right - windowRect.left,
        .height        = windowRect.bottom - windowRect.top,
        .client_width  = clientRect.right - clientRect.left,
        .client_height = clientRect.bottom - clientRect.top,
        // .is_focused    = (GetForegroundWindow() == backend->window_handle),
        .is_focused    = backend->last_metrics.is_focused,
        .is_minimized  = (placement.showCmd == SW_SHOWMINIMIZED),
        .is_maximized  = (placement.showCmd == SW_SHOWMAXIMIZED)
    };
    // log_debug("Window metrics x:%d y:%d w:%d h:%d", metrics.x, metrics.y, metrics.width, metrics.height);
    // log_debug("Window metrics client w:%d h:%d", metrics.client_width, metrics.client_height);

    // Cache the metrics
    backend->last_metrics = metrics;
    /* if (metrics.client_width == 0 || metrics.client_height == 0) {
        HANDLE                     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (GetConsoleScreenBufferInfo(hConsole, &bufferInfo)) {
            // backend->last_metrics.client_width  = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
            // backend->last_metrics.client_height = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;
            // log_debug("Client width and height cached from console buffer info: w:%d h:%d", backend->last_metrics.client_width, backend->last_metrics.client_height);
            backend->last_metrics.client_width  = bufferInfo.dwSize.X;
            backend->last_metrics.client_height = bufferInfo.dwSize.Y * 2;
            // log_debug("Client width and height restored from console buffer info: w:%d h:%d", backend->last_metrics.client_width, backend->last_metrics.client_height);
        }
    } */

    return_some(metrics);
}

static void Win32ConsoleBackend_enableMouseInput(engine_Win32ConsoleBackend* backend) {
    DWORD prevMode = 0;
    GetConsoleMode(backend->input_handle, &prevMode);

    // Enable mouse input and disable QuickEdit
    DWORD newMode = ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    newMode &= ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(backend->input_handle, newMode);
}

static void Win32ConsoleBackend_disableMouseInput(engine_Win32ConsoleBackend* backend) {
    DWORD prevMode = 0;
    GetConsoleMode(backend->input_handle, &prevMode);

    // Restore previous mode without mouse input
    DWORD newMode = prevMode & ~ENABLE_MOUSE_INPUT;
    newMode |= ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(backend->input_handle, newMode);
}

static void Win32ConsoleBackend_processMouseEvent(engine_Win32ConsoleBackend* backend, const MOUSE_EVENT_RECORD* mer) {
    // Get pixel position in double-height space
    let pos = Win32ConsoleBackend_getPixelPosition(mer, backend->output_handle);

    // Get console buffer info for bounds checking
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo = cleared();
    GetConsoleScreenBufferInfo(backend->output_handle, &bufferInfo);

    // Ensure coordinates are within bounds
    if (pos.x >= 0 && pos.x < bufferInfo.dwSize.X && pos.y >= 0 && pos.y < bufferInfo.dwSize.Y * 2) {
        engine_MouseEvent event = { 0 };
        event.timestamp         = (f64)GetTickCount64() / 1000.0;

        switch (mer->dwEventFlags) {
        case 0: { // Mouse clicks
            event.type = engine_MouseEventType_button;

            // Left button
            if (mer->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
                event.button.key   = engine_MouseButton_left;
                event.button.state = engine_KeyStates_pressed;
                engine_InputEventBuffer_push(*(engine_InputEvent*)&event);
            }
            // Right button
            if (mer->dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
                event.button.key   = engine_MouseButton_right;
                event.button.state = engine_KeyStates_pressed;
                engine_InputEventBuffer_push(*(engine_InputEvent*)&event);
            }
        } break;

        case MOUSE_MOVED: { // Mouse movement
            event.type   = engine_MouseEventType_motion;
            event.move.x = pos.x;
            event.move.y = pos.y;
            engine_InputEventBuffer_push(*(engine_InputEvent*)&event);
        } break;

        case MOUSE_WHEELED: { // Mouse wheel scrolling
            event.type            = engine_MouseEventType_scroll;
            // Proper scroll delta extraction for Windows
            const i16 wheel_delta = HIWORD(mer->dwButtonState);
            event.scroll.delta    = wheel_delta > 0 ? 1 : -1; // Use actual wheel delta

            // Update cached scroll delta and calculate speed
            scope_with(let input = engine_Input_instance()) {
                input->mouse.scroll_delta = event.scroll.delta;

                // Calculate time difference
                f64 current_time = event.timestamp;
                f64 delta_time   = current_time - input->mouse.last_scroll_timestamp;

                // Accumulate scroll delta
                input->mouse.accumulated_scroll_delta += input->mouse.scroll_delta;

                // Calculate speed (only if delta_time is significant to avoid division by near-zero)
                if (delta_time > 0.01) { // Adjust threshold as needed
                    input->mouse.scroll_speed = (f64)input->mouse.accumulated_scroll_delta / delta_time;

                    // Reset accumulated delta and update last scroll timestamp
                    input->mouse.accumulated_scroll_delta = 0;
                    input->mouse.last_scroll_timestamp    = current_time;
                }
            }
        } break;

        default:
            break;
        }

        // Update cached mouse position
        scope_with(let input = engine_Input_instance()) {
            input->mouse.prev_x = input->mouse.x;
            input->mouse.prev_y = input->mouse.y;
            input->mouse.x      = pos.x;
            input->mouse.y      = pos.y;
        }
    }
}

/* // Function to get the console window size in characters
static void win32_get_console_size(i32* width, i32* height) {
    HANDLE                     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (GetConsoleScreenBufferInfo(hConsole, &bufferInfo)) {
        *width  = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
        *height = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;
    } else {
        *width  = 80; // Default values
        *height = 25;
    }
}

// Function to get the console font size
static void win32_get_font_size(i32* width, i32* height) {
    HANDLE              hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo = { .cbSize = sizeof(CONSOLE_FONT_INFOEX) };
    if (GetCurrentConsoleFontEx(hConsole, false, &fontInfo)) {
        *width  = fontInfo.dwFontSize.X;
        *height = fontInfo.dwFontSize.Y;
    } else {
        *width  = 8; // Default values
        *height = 16;
    }
}

void platform_get_client_size(i32* width, i32* height) {
    // Get the size of the console window in characters
    i32 console_width  = 0;
    i32 console_height = 0;
    win32_get_console_size(&console_width, &console_height);

    // Get the size of the console font
    i32 font_width  = 0;
    i32 font_height = 0;
    win32_get_font_size(&font_width, &font_height);

    // Calculate client area size based on characters and font size
    *width  = console_width * font_width;
    *height = console_height * font_height / 2; // Adjust for double-height pixels
} */

static Vec2i Win32ConsoleBackend_getPixelPosition(const MOUSE_EVENT_RECORD* mer, HANDLE output_handle) {
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo = cleared();
    CONSOLE_FONT_INFOEX        fontInfo   = { .cbSize = sizeof(CONSOLE_FONT_INFOEX) };

    GetConsoleScreenBufferInfo(output_handle, &bufferInfo);
    GetCurrentConsoleFontEx(output_handle, false, &fontInfo);

    // Get raw mouse position
    POINT mousePos = cleared();
    GetCursorPos(&mousePos);

    // Get console window handle and position
    HWND consoleWnd  = GetConsoleWindow();
    RECT consoleRect = cleared();
    GetWindowRect(consoleWnd, &consoleRect);

    // Calculate relative position for double-height characters
    i32 cellHeight    = fontInfo.dwFontSize.Y;
    i32 relativeY     = mousePos.y - consoleRect.top;
    i32 consoleY      = mer->dwMousePosition.Y;
    i32 subCellOffset = (relativeY % cellHeight) >= (cellHeight / 2) ? 1 : 0;

    prev_mouse_x = mer->dwMousePosition.X;
    prev_mouse_y = mer->dwMousePosition.Y;

    return (Vec2i){
        .s = {
            mer->dwMousePosition.X,
            (consoleY * 2) + subCellOffset // Convert to double-height space
        }
    };
}



engine_RenderBackend engine_RenderBackend_createNoOp(void) {
    return (engine_RenderBackend){
        .type             = engine_RenderBackendType_null,
        .destroy          = engine_RenderBackend_noDestroy,
        .processEvents    = engine_RenderBackend_noProcessEvents,
        .presentBuffer    = engine_RenderBackend_noPresentBuffer,
        .getWindowMetrics = engine_RenderBackend_noGetWindowMetrics
    };
}

void engine_RenderBackend_noDestroy(struct engine_Platform* platform) {
    unused(platform);
}

void engine_RenderBackend_noProcessEvents(struct engine_Platform* platform) {
    unused(platform);
}

void engine_RenderBackend_noPresentBuffer(struct engine_Platform* platform, const Color* buffer, u32 width, u32 height) {
    unused(platform);
    unused(buffer);
    unused(width);
    unused(height);
}

Opt$engine_WindowMetrics engine_RenderBackend_noGetWindowMetrics(struct engine_Platform* platform) {
    unused(platform);
    reserveReturn(Opt$engine_WindowMetrics);
    return_none();
}
