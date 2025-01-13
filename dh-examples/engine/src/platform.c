#include "engine/platform.h"
#include "engine/platform_backend.h"
#include "engine/window.h"
#include "engine/canvas.h"
#include "engine/input.h"

#define Win32ConsoleBackend_calculateBufferSize(width, height) ((width) * (height) * 32)
static void                     Win32ConsoleBackend_destroy(engine_Platform* platform);
static void                     Win32ConsoleBackend_processEvents(engine_Platform* platform);
static void                     Win32ConsoleBackend_presentBuffer(engine_Platform* platform, const Color* data, u32 width, u32 height);
static Opt$engine_WindowMetrics Win32ConsoleBackend_getWindowMetrics(engine_Platform* platform);

static Vec2i Win32ConsoleBackend_getPixelPosition(const MOUSE_EVENT_RECORD* mer, HANDLE output_handle);
static void  Win32ConsoleBackend_enableMouseInput(engine_Win32ConsoleBackend* backend);
static void  Win32ConsoleBackend_disableMouseInput(engine_Win32ConsoleBackend* backend);
static void  Win32ConsoleBackend_processMouseEvent(engine_Win32ConsoleBackend* backend, const MOUSE_EVENT_RECORD* mer);



Err$Ptr$engine_Platform engine_Platform_create(const engine_PlatformParams* params) {
    reserveReturn(Err$Ptr$engine_Platform);
    engine_Platform* const platform = (engine_Platform*)malloc(sizeof(engine_Platform));
    if (!platform) {
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }

    switch (params->backend_type) {
    case engine_RenderBackendType_vt100: {
        engine_Win32ConsoleBackend* const backend = (engine_Win32ConsoleBackend*)malloc(sizeof(engine_Win32ConsoleBackend));
        if (!backend) {
            free(platform);
            return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
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
        //     return_err(engine_PlatformErr_err(engine_PlatformErrType_AccessDenied));
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
            return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }

        backend->output_handle  = hOutput;
        backend->buffer_size    = 0;
        backend->cursor_visible = false;

        // Setup backend interface
        backend->base = eval(
            var vt              = engine_RenderBackend_createNoOp();
            vt.type             = engine_RenderBackendType_vt100;
            vt.destroy          = Win32ConsoleBackend_destroy;
            vt.processEvents    = Win32ConsoleBackend_processEvents;
            vt.presentBuffer    = Win32ConsoleBackend_presentBuffer;
            vt.getWindowMetrics = Win32ConsoleBackend_getWindowMetrics;
            eval_return vt;
        );
        Win32ConsoleBackend_enableMouseInput(backend);

        platform->backend = &backend->base;
        return_ok(platform);
    }

    case engine_RenderBackendType_win32_gdi:
    case engine_RenderBackendType_directx:
        free(platform);
        return_err(engine_PlatformErr_err(engine_PlatformErrType_NotImplemented));

    case engine_RenderBackendType_custom:
        if (!params->custom_data) {
            free(platform);
            return_err(engine_PlatformErr_err(engine_PlatformErrType_InvalidArgument));
        }
        platform->backend = params->custom_data;
        return_ok(platform);

    default:
        free(platform);
        return_err(engine_PlatformErr_err(engine_PlatformErrType_InvalidArgument));
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

static void Win32ConsoleBackend_processEvents(engine_Platform* platform) {
    let backend = (engine_Win32ConsoleBackend*)platform->backend;

    // Process window metrics
    if_some(Win32ConsoleBackend_getWindowMetrics(platform), current_metrics) {
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

    while (PeekConsoleInput(backend->input_handle, input_record, 32, &events_read) && events_read > 0) {
        ReadConsoleInput(backend->input_handle, input_record, events_read, &events_read);

        for (DWORD i = 0; i < events_read; ++i) {
            switch (input_record[i].EventType) {
            case MOUSE_EVENT:
                Win32ConsoleBackend_processMouseEvent(backend, &input_record[i].Event.MouseEvent);
                break;

            case WINDOW_BUFFER_SIZE_EVENT:
                // Handle through metrics update
                break;
            }
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
        .is_focused    = (GetForegroundWindow() == backend->window_handle),
        .is_minimized  = (placement.showCmd == SW_SHOWMINIMIZED),
        .is_maximized  = (placement.showCmd == SW_SHOWMAXIMIZED)
    };

    // Cache the metrics
    backend->last_metrics = metrics;
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
        case 0: // Mouse clicks
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
            break;

        case MOUSE_MOVED:
            event.type   = engine_MouseEventType_move;
            event.move.x = pos.x;
            event.move.y = pos.y;
            engine_InputEventBuffer_push(*(engine_InputEvent*)&event);
            break;

        case MOUSE_WHEELED:
            event.type         = engine_MouseEventType_scroll;
            event.scroll.delta = ((i32)mer->dwButtonState > 0) ? 1 : -1;
            engine_InputEventBuffer_push(*(engine_InputEvent*)&event);
            break;
        }

        // Update cached mouse position
        if (engine_InputState_global) {
            engine_InputState_global->mouse.prev_x = engine_InputState_global->mouse.x;
            engine_InputState_global->mouse.prev_y = engine_InputState_global->mouse.y;
            engine_InputState_global->mouse.x      = pos.x;
            engine_InputState_global->mouse.y      = pos.y;
        }
    }
}

static Vec2i Win32ConsoleBackend_getPixelPosition(const MOUSE_EVENT_RECORD* mer, HANDLE output_handle) {
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo = cleared();
    CONSOLE_FONT_INFOEX        fontInfo   = { .cbSize = sizeof(CONSOLE_FONT_INFOEX) };

    GetConsoleScreenBufferInfo(output_handle, &bufferInfo);
    GetCurrentConsoleFontEx(output_handle, FALSE, &fontInfo);

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
