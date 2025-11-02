#include "engine/core/Vt100.h"
#include "engine/Input.h"
#include "../Backend_Internal.h"

#include "dh/mem/common.h"
#include "dh/ArrList.h"

#if plat_windows
#include "dh/os/windows.h"

/* structure */
use_ArrList$(u8);
struct engine_core_Vt100 {
    struct {
        engine_Window* window;
        ArrList$u8     buffer;
    } abstract;
    engine_Input* input;
    struct {
        struct {
            HWND   window; // Handle to console(or console wrapper) window
            HANDLE output; // Handle to console output
            HANDLE input;  // Handle to console input
        } handle;
        struct {
            Vec2i top_left_including_title_bar;
            union {
                Vec2i top_left_excluding_title_bar;
                Vec2i top_left; // Default excluding title bar
            };
        } pos_on_display;
        struct {
            Vec2u dim;
            struct {
                Vec2u min; /* TODO: Add handling logic */
                Vec2u max; /* TODO: Add handling logic */
                Vec2u curr;
            } res;
        } metrics;       // Based pos on display top left (default excluding title bar)
        bool is_focused; // Change when gain focus or lose focus
        bool is_minimized;
        bool is_maximized;
    } client;
    mem_Allocator allocator;
};
config_ErrSet(engine_core_Vt100_Err);

static fn_((processEvents(P$raw ctx))(void)); /* TODO: validate - processing events and updating properties */
static fn_((presentBuffer(P$raw ctx))(void));

static fn_((getWindowPos(const P$raw ctx))(Vec2i));
static fn_((getWindowDim(const P$raw ctx))(Vec2u));
static fn_((getWindowRes(const P$raw ctx))(Vec2u));

static fn_((getWindowMinRes(const P$raw ctx))(Vec2u));
static fn_((getWindowMaxRes(const P$raw ctx))(Vec2u));
static fn_((setWindowMinRes(P$raw ctx, Vec2u size))(E$void)) $must_check;
static fn_((setWindowMaxRes(P$raw ctx, Vec2u size))(E$void)) $must_check;

static fn_((isWindowFocused(const P$raw ctx))(bool));
static fn_((isWindowMinimized(const P$raw ctx))(bool));
static fn_((isWindowMaximized(const P$raw ctx))(bool));

static fn_((getKeyboardState(const P$raw ctx, engine_KeyCode key))(u8));
static fn_((isKeyboardState(const P$raw ctx, engine_KeyCode key, engine_KeyButtonStates state))(bool));

static fn_((pressedKeyboard(const P$raw ctx, engine_KeyCode key))(bool));
static fn_((heldKeyboard(const P$raw ctx, engine_KeyCode key))(bool));
static fn_((releasedKeyboard(const P$raw ctx, engine_KeyCode key))(bool));

static fn_((getMouseState(const P$raw ctx, engine_MouseButton button))(u8));
static fn_((isMouseState(const P$raw ctx, engine_MouseButton button, engine_KeyButtonStates state))(bool));

static fn_((pressedMouse(const P$raw ctx, engine_MouseButton button))(bool));
static fn_((heldMouse(const P$raw ctx, engine_MouseButton button))(bool));
static fn_((releasedMouse(const P$raw ctx, engine_MouseButton button))(bool));

static fn_((getMousePos(const P$raw ctx))(Vec2i));
static fn_((getMousePosDelta(const P$raw ctx))(Vec2i));
static fn_((getMouseWheelScrollDelta(const P$raw ctx))(Vec2f));

fn_((engine_core_Vt100_backend(engine_core_Vt100* self))(engine_Backend)) {
    static const engine_Backend_VT vt[1] = { {
        .processEvents = processEvents,
        .presentBuffer = presentBuffer,
    } };

    static const engine_Backend_VT_Internal vt_internal[1] = { {
        .getWindowPos = getWindowPos,
        .getWindowDim = getWindowDim,
        .getWindowRes = getWindowRes,

        .getWindowMinRes = getWindowMinRes,
        .getWindowMaxRes = getWindowMaxRes,
        .setWindowMinRes = setWindowMinRes,
        .setWindowMaxRes = setWindowMaxRes,

        .isWindowFocused   = isWindowFocused,
        .isWindowMinimized = isWindowMinimized,
        .isWindowMaximized = isWindowMaximized,

        .getKeyboardState = getKeyboardState,
        .isKeyboardState  = isKeyboardState,

        .pressedKeyboard  = pressedKeyboard,
        .heldKeyboard     = heldKeyboard,
        .releasedKeyboard = releasedKeyboard,

        .getMouseState = getMouseState,
        .isMouseState  = isMouseState,

        .pressedMouse  = pressedMouse,
        .heldMouse     = heldMouse,
        .releasedMouse = releasedMouse,

        .getMousePos              = getMousePos,
        .getMousePosDelta         = getMousePosDelta,
        .getMouseWheelScrollDelta = getMouseWheelScrollDelta,
    } };

    return (engine_Backend){
        .ptr         = self,
        .vt          = vt,
        .vt_internal = vt_internal
    };
}

/*========== Forward declarations ===========================================*/

static $inline_always fn_((calcAbstractBufferSize(u32 width, u32 height))(usize));
$deprecated static $inline_always fn_((clientWindowPixelRect(engine_core_Vt100* self))(Vec2u));
$deprecated static $inline_always fn_((clientOutputConsoleRect(engine_core_Vt100* self))(Vec2u));

static $inline_always fn_((abstractWindowRect(engine_core_Vt100* self))(Vec2u));
static $inline_always fn_((abstractBufferCapSize(engine_core_Vt100* self))(usize));

$deprecated static $inline_always fn_((needsResizeAbstractWindow(engine_core_Vt100* self))(bool));
$deprecated static fn_((resizeAbstractWindow(engine_core_Vt100* self))(E$void)) $must_check;

static fn_((syncWindowMetrics(engine_core_Vt100* self))(E$void)) $must_check;

static fn_((configureConsoleOutput(engine_core_Vt100* self))(E$void)) $must_check;
static fn_((configureConsoleInput(engine_core_Vt100* self))(E$void)) $must_check;

static fn_((hideConsoleCursor(engine_core_Vt100* self))(E$void)) $must_check;
static fn_((showConsoleCursor(engine_core_Vt100* self))(E$void)) $must_check;
static fn_((resetConsoleCursorPos(engine_core_Vt100* self))(E$void)) $must_check;

static fn_((processConsoleKeyboardKey(engine_core_Vt100* self, engine_KeyCode key, bool is_down))(void));
static fn_((processConsoleKeyboardEvents(engine_core_Vt100* self))(void));

static fn_((enableConsoleMouse(engine_core_Vt100* self))(E$void)) $must_check;
static fn_((disableConsoleMouse(engine_core_Vt100* self))(E$void)) $must_check;
static fn_((processConsoleMouseButton(engine_core_Vt100* self, engine_MouseButton button, bool is_down))(void));
static fn_((processConsoleMouseEvents(engine_core_Vt100* self))(void)); /* TODO: validate */

/*========== Implementations ================================================*/

static $inline_always fn_((calcAbstractBufferSize(u32 width, u32 height))(usize)) {
    return (as$((usize)(width)) + 1)
         * (as$((usize)(height)) + 1)
         * (sizeof("\033[38;2;255;255;255;48;2;255;255;255m▀") - 1);
}

/// Returns the size (width, height) in pixels of the client area
/// for the console window.
$maybe_unused static $inline_always fn_((clientWindowPixelRect(engine_core_Vt100* self))(Vec2u)) {
    // if_(RECT rect = cleared(), GetClientRect(self->client.handle.window, &rect)) {
    //     // Update client metrics in sync
    //     self->client.metrics.current_size = (Vec2u){
    //         .x = as$(u32, rect.right - rect.left),
    //         .y = as$(u32, rect.bottom - rect.top)
    //     };
    //     return self->client.metrics.res.curr;
    // }
    // return self->client.metrics.current_size; // Return cached size as fallback
    $unused(self);
    return (Vec2u){ 0 };
}

/// Returns the size (width, height) of the console screen buffer
/// in character cells (columns and rows).
$maybe_unused static $inline_always fn_((clientOutputConsoleRect(engine_core_Vt100* self))(Vec2u)) {
    // if_(CONSOLE_SCREEN_BUFFER_INFO info = cleared(), GetConsoleScreenBufferInfo(self->client.handle.output, &info)) {
    //     // Keep buffer size in sync with console
    //     self->client.metrics.current_size = (Vec2u){
    //         .x = as$(u32, info.dwSize.X),
    //         .y = as$(u32, info.dwSize.Y)
    //     };
    //     return self->client.metrics.res.curr;
    // }
    // return self->client.metrics.current_size;
    $unused(self);
    return (Vec2u){ 0 };
}

/// Returns the current abstract window size from engine_WindowMetrics,
/// typically used for high-level engine logic.
static $inline_always fn_((abstractWindowRect(engine_core_Vt100* self))(Vec2u)) {
    return (Vec2u){
        .x = Grid_width(self->abstract.window->composite_buffer->buffer),
        .y = Grid_height(self->abstract.window->composite_buffer->buffer)
    };
}

/// Returns the bytes-length of the internal "abstract" buffer,
/// which you might use for storing render data before presenting.
/// If you’re using ArrList$u8 as a text buffer only, adapt as needed.
static $inline_always fn_((abstractBufferCapSize(engine_core_Vt100* self))(usize)) {
    // If you store rows and columns separately in abstract.buffer, adapt accordingly.
    // For example, if you always keep a buffer big enough for (width * height * cellSize),
    // you could do something like the logic below or just re-return the window rect.
    // For demonstration, we’ll match the window’s dimension:
    return self->abstract.buffer.cap;
}

/// Checks if the abstract buffer needs to be resized based on the
/// latest window metrics or any other condition your engine considers.
$maybe_unused static $inline_always fn_((needsResizeAbstractWindow(engine_core_Vt100* self))(bool)) {
    let current_size = abstractBufferCapSize(self);
    // Compare with the expected size from your function
    let needed_size  = blk({
        let rect = abstractWindowRect(self);
        blk_return calcAbstractBufferSize(rect.x, rect.y);
     });
    return current_size != needed_size;
}

/// Perform an actual resize of the abstract window/buffer, and also
/// reconfigure the console screen buffer size if needed.
$maybe_unused
fn_((resizeAbstractWindow(engine_core_Vt100* self))(E$void) $scope) {
    // 1. Re-size the console’s screen buffer
    let rect = abstractWindowRect(self);
    if (!SetConsoleScreenBufferSize(self->client.handle.output, (COORD){ as$((SHORT)(rect.x)), as$((SHORT)(rect.y)) })) {
        // If you have specific error sets for failing to resize, return them here
        return_err(E_Unspecified());
    }

    // 2. Re-size your ArrList if needed
    let needed = calcAbstractBufferSize(rect.x, rect.y);
    try_(
        ArrList_resize(self->abstract.buffer.base, needed)
    );

    return_ok({});
} $unscoped_(fn);

fn_((syncWindowMetrics(engine_core_Vt100* self))(E$void) $scope) {
    let handle_window = self->client.handle.window;
    let window_rect   = blk({
        var rect = make$(RECT);
        GetWindowRect(handle_window, &rect);
        blk_return rect;
      });
    // log_debug("Window rect: %d,%d,%d,%d", window_rect.left, window_rect.top, window_rect.right, window_rect.bottom);
    let client_rect   = blk({
        var rect = make$(RECT);
        GetClientRect(handle_window, &rect);
        rect.top += 40;
        rect.top += 12;
        rect.left += 12;
        rect.right -= 12;
        rect.bottom += 12;
        blk_return rect;
      });
    // log_debug("Client rect: %d,%d,%d,%d", client_rect.left, client_rect.top, client_rect.right, client_rect.bottom);
    let corner_point  = blk({
        // Convert the client area (0,0) to screen coords
        var point = make$(POINT);
        ClientToScreen(handle_window, &point);
        blk_return point;
    });
    // log_debug("Corner point: %d,%d", corner_point.x, corner_point.y);

    // Now corner_point.x, corner_point.y is the client’s top-left in screen coords
    // Border offset:
    // i32 border_x = corner_point.x - window_rect.left;
    // i32 border_y = corner_point.y - window_rect.top;
    // log_debug("Border offset: %d,%d", border_x, border_y);

    // Store them
    self->client.pos_on_display.top_left_including_title_bar.x = window_rect.left;
    self->client.pos_on_display.top_left_including_title_bar.y = window_rect.top;

    self->client.pos_on_display.top_left.x = corner_point.x + 12;      // i.e. window_rect.left + borderX
    self->client.pos_on_display.top_left.y = corner_point.y + 40 + 12; // i.e. window_rect.top  + borderY

    let handle_output    = self->client.handle.output;
    let buffer_info      = blk({
        var info = make$(CONSOLE_SCREEN_BUFFER_INFO);
        if (!GetConsoleScreenBufferInfo(handle_output, &info)) {
            return_err(E_Unspecified());
        }
        blk_return info;
         });
    let font_info        = blk({
        var info = make$(CONSOLE_FONT_INFOEX, .cbSize = sizeof(CONSOLE_FONT_INFOEX));
        if (!GetCurrentConsoleFontEx(handle_output, false, &info)) {
            return_err(E_Unspecified());
        }
        /* if (info.dwFontSize.X == 0) {
            info.dwFontSize.X = as$(SHORT, info.dwFontSize.Y / 2);
        } */
        blk_return info;
           });
    let usable_font_info = make$(bool, font_info.dwFontSize.X != 0 && font_info.dwFontSize.Y != 0);
    if (usable_font_info) {
        self->client.metrics.dim.x = buffer_info.dwSize.X * font_info.dwFontSize.X;
        self->client.metrics.dim.y = buffer_info.dwSize.Y * font_info.dwFontSize.Y;

        // 2 "rows" per console cell
        self->client.metrics.res.curr.x = buffer_info.dwSize.X;
        self->client.metrics.res.curr.y = buffer_info.dwSize.Y * 2;
    } else {
        // fallback for no font info
        self->client.metrics.dim.x = client_rect.right;
        self->client.metrics.dim.y = client_rect.bottom;

        self->client.metrics.res.curr.x = buffer_info.dwSize.X;
        self->client.metrics.res.curr.y = buffer_info.dwSize.Y * 2;
    }

    // Resize abstract buffer
    let needed = calcAbstractBufferSize(
        self->client.metrics.res.curr.x,
        self->client.metrics.res.curr.y
    );
    try_(
        ArrList_resize(self->abstract.buffer.base, needed)
    );
    try_(
        engine_Canvas_resize(
            self->abstract.window->composite_buffer,
            self->client.metrics.res.curr.x,
            self->client.metrics.res.curr.y
        )
    );

    return_ok({});
} $unscoped_(fn);

// static E$void syncWindowMetrics(engine_core_Vt100* self) {
//     reserveReturn(E$void);

//     let handle_window = self->client.handle.window;
//     let window_rect   = blk({
//         var rect = make$(RECT);
//         GetWindowRect(handle_window, &rect);
//         blk_return rect;
//     });
//     let client_rect   = blk({
//         var rect = make$(RECT);
//         GetClientRect(handle_window, &rect);
//         blk_return rect;
//     });

//     let handle_output = self->client.handle.output;
//     let buffer_info   = blk({
//         var info = make$(CONSOLE_SCREEN_BUFFER_INFO);
//         if (!GetConsoleScreenBufferInfo(handle_output, &info)) {
//             return_err(E_Unspecified());
//         }
//         blk_return info;
//     });
//     let font_info     = blk({
//         var info = make$(CONSOLE_FONT_INFOEX, .cbSize = sizeof(CONSOLE_FONT_INFOEX));
//         if (!GetCurrentConsoleFontEx(handle_output, false, &info)) {
//             return_err(E_Unspecified());
//         }
//         /* if (info.dwFontSize.X == 0) {
//             info.dwFontSize.X = as$(SHORT, info.dwFontSize.Y / 2);
//         } */
//         blk_return info;
//     });

//     let usable_font_info = font_info.dwFontSize.X != 0 && font_info.dwFontSize.Y != 0;
//     if (usable_font_info) {
//         // Use actual font metrics to compute total client dimension
//         self->client.metrics.dim.x = buffer_info.dwSize.X * font_info.dwFontSize.X;
//         self->client.metrics.dim.y = buffer_info.dwSize.Y * font_info.dwFontSize.Y;

//         // Logical resolution: 2 "pixels" per character cell vertically
//         self->client.metrics.res.curr.x = buffer_info.dwSize.X;
//         self->client.metrics.res.curr.y = buffer_info.dwSize.Y * 2;

//         // -- Update offsets for correct mouse transform
//         self->client.pos_on_display.top_left_including_title_bar.x = window_rect.left;
//         self->client.pos_on_display.top_left_including_title_bar.y = window_rect.top;

//         self->client.pos_on_display.top_left.x = window_rect.right - client_rect.right;
//         self->client.pos_on_display.top_left.y = window_rect.bottom - client_rect.bottom;
//     } else {
//         // Fallback if cannot get font metrics
//         self->client.metrics.dim.x = client_rect.right;
//         self->client.metrics.dim.y = client_rect.bottom;

//         self->client.pos_on_display.top_left_including_title_bar.x = window_rect.left;
//         self->client.pos_on_display.top_left_including_title_bar.y = window_rect.top;

//         self->client.pos_on_display.top_left.x = window_rect.right - client_rect.right;
//         self->client.pos_on_display.top_left.y = window_rect.bottom - client_rect.bottom;

//         self->client.metrics.res.curr.x = buffer_info.dwSize.X;
//         self->client.metrics.res.curr.y = buffer_info.dwSize.Y * 2;
//     }

//     // Resize abstract buffer
//     let needed = calcAbstractBufferSize(
//         self->client.metrics.res.curr.x,
//         self->client.metrics.res.curr.y
//     );
//     try_(ArrList_resize(&self->abstract.buffer.base, needed));
//     try_(engine_Canvas_resize(
//         self->abstract.window->composite_buffer,
//         self->client.metrics.res.curr.x,
//         self->client.metrics.res.curr.y
//     ));

//     return_void();
// }

config_ErrSet(ConfigConsoleOutputErr,
    FailedSetWindowInfo,
    FailedSetScreenBufferSize,
    FailedAssignScreenBuffer,
    FailedSetCodePage,
    FailedGetMode,
    FailedSetMode
);
/// Function to initialize console-specific configurations like screen buffer size
fn_((configureConsoleOutput(engine_core_Vt100* self))(E$void) $scope) {
    let handle = self->client.handle.output;

    // Initialize console window size to minimum, so ScreenBuffer can shrink
    if (!SetConsoleWindowInfo(handle, true, &(SMALL_RECT){ 0, 0, 1, 1 })) {
        log_error("Failed to set console window info: %d", GetLastError());
        return_err(ConfigConsoleOutputE_FailedSetWindowInfo());
    }

    // Configure screen buffer size for the console
    let rect = abstractWindowRect(self);
    if (!SetConsoleScreenBufferSize(handle, (COORD){ as$((SHORT)(rect.x)), as$((SHORT)(rect.y)) })) {
        log_error("Failed to set console screen buffer size: %d", GetLastError());
        return_err(ConfigConsoleOutputE_FailedSetScreenBufferSize());
    }
    // Assign screen buffer to the console
    if (!SetConsoleActiveScreenBuffer(handle)) {
        log_error("Failed to assign screen buffer to the console: %d", GetLastError());
        return_err(ConfigConsoleOutputE_FailedAssignScreenBuffer());
    }
    // Set physical console window size
    if (!SetConsoleWindowInfo(handle, true, &(SMALL_RECT){ 0, 0, as$((SHORT)(rect.x - 1)), as$((SHORT)(rect.y - 1)) })) {
        log_error("Failed to set console window info: %d", GetLastError());
        return_err(ConfigConsoleOutputE_FailedSetWindowInfo());
    }

    // Configure console output code page to UTF-8
    if (!SetConsoleOutputCP(CP_UTF8)) {
        log_error("Failed to set console output code page to UTF-8: %d", GetLastError());
        return_err(ConfigConsoleOutputE_FailedSetCodePage());
    }

    // Set console output mode for processing terminal sequences
    // debug_only(if (IsDebuggerPresent()) { return_void(); }); // Skip logic for debugging via debugger
    if_(DWORD out_mode = 0, !GetConsoleMode(handle, &out_mode)) {
        log_error("Failed to get console output mode: %d", GetLastError());
        return_err(ConfigConsoleOutputE_FailedSetMode());
    } else {
        out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
        if (!SetConsoleMode(handle, out_mode)) {
            log_error("Failed to set console output mode: %d", GetLastError());
            return_err(ConfigConsoleOutputE_FailedSetMode());
        }
    }
    return_ok({});
} $unscoped_(fn);

config_ErrSet(ConfigConsoleInputErr,
    FailedGetMode,
    FailedSetMode
);
/// Function to configure console input mode
fn_((configureConsoleInput(engine_core_Vt100* self))(E$void) $scope) {
    let handle = self->client.handle.input;
    // debug_only(if (IsDebuggerPresent()) { return_void(); }); // Skip logic for debugging via debugger
    if_(DWORD in_mode = 0, !GetConsoleMode(handle, &in_mode)) {
        log_error("Failed to get console input mode: %d", GetLastError());
        return_err(ConfigConsoleInputE_FailedGetMode());
    } else {
        in_mode |= ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT;
        if (!SetConsoleMode(handle, in_mode)) {
            log_error("Failed to set console input mode: %d", GetLastError());
            return_err(ConfigConsoleInputE_FailedSetMode());
        }
    }
    return_ok({});
} $unscoped_(fn);

config_ErrSet(ConfigConsoleCursorErr,
    FailedHide,
    FailedShow,
    FailedResetPos
);
fn_((hideConsoleCursor(engine_core_Vt100* self))(E$void) $scope) {
    let handle = self->client.handle.output;
    if (!SetConsoleCursorInfo(handle, &(CONSOLE_CURSOR_INFO){ 1, false })) {
        log_error("Failed to hide console cursor: %d", GetLastError());
        return_err(ConfigConsoleCursorE_FailedHide());
    }
    return_ok({});
} $unscoped_(fn);

fn_((showConsoleCursor(engine_core_Vt100* self))(E$void) $scope) {
    let handle = self->client.handle.output;
    if (!SetConsoleCursorInfo(handle, &(CONSOLE_CURSOR_INFO){ 1, true })) {
        log_error("Failed to show console cursor: %d", GetLastError());
        return_err(ConfigConsoleCursorE_FailedShow());
    }
    return_ok({});
} $unscoped_(fn);

fn_((resetConsoleCursorPos(engine_core_Vt100* self))(E$void) $scope) {
    let handle  = self->client.handle.output;
    let command = u8_l("\033[H");
    if_(DWORD written = 0,
       !WriteConsoleA(
           handle,
           command.ptr,
           command.len,
           &written,
           null
       )
       || written != command.len) {
        log_error("Failed to reset console cursor position: %d", GetLastError());
        return_err(ConfigConsoleCursorE_FailedResetPos());
    };
    return_ok({});
} $unscoped_(fn);

fn_((processConsoleKeyboardKey(engine_core_Vt100* self, engine_KeyCode key, bool is_down))(void)) {
    let input    = self->input;
    let keyboard = input->keyboard;

    let curr_state = A_at(keyboard->keys.curr_states, key);
    let prev_state = *A_at(keyboard->keys.prev_states, key);

    // Clear previous state flags
    *curr_state = engine_KeyButtonStates_none;

    if (is_down) {
        if (!(prev_state & engine_KeyButtonStates_held)) {
            // Key was just pressed
            *curr_state |= engine_KeyButtonStates_pressed;

            // Create pressed event
            engine_InputEventBuffer_push(input, variant_of$(engine_InputEvent, engine_InputEvent_key_press, { .key = key }));
        }
        // Key is being held
        *curr_state |= engine_KeyButtonStates_held;

        // Create held event
        engine_InputEventBuffer_push(input, variant_of$(engine_InputEvent, engine_InputEvent_key_hold, { .key = key }));
    } else if (prev_state & (engine_KeyButtonStates_pressed | engine_KeyButtonStates_held)) {
        // Key was just released
        *curr_state |= engine_KeyButtonStates_released;

        // Create released event
        engine_InputEventBuffer_push(input, variant_of$(engine_InputEvent, engine_InputEvent_key_release, { .key = key }));
    }
}

fn_((processConsoleKeyboardEvents(engine_core_Vt100* self))(void)) {
    let keyboard = self->input->keyboard;

    // Save previous state
    memcpy(keyboard->keys.prev_states.buf, keyboard->keys.curr_states.buf, sizeOf(keyboard->keys.curr_states.buf));

    // Process each key
    for (engine_KeyCode key = engine_KeyCode_none + 1; key < as$((engine_KeyCode)(engine_KeyCode_count)); ++key) {
        let  key_state = GetAsyncKeyState(key);
        bool is_down   = (key_state & 0x8000) != 0;
        processConsoleKeyboardKey(self, key, is_down);
    }
}

config_ErrSet(ConfigConsoleMouseErr,
    FailedEnableGetMode,
    FailedEnableSetMode,
    FailedDisableGetMode,
    FailedDisableSetMode
);
/// Enable mouse input through the console.
fn_((enableConsoleMouse(engine_core_Vt100* self))(E$void) $scope) {
    let handle = self->client.handle.input;
    // debug_only(if (IsDebuggerPresent()) { return_void(); }); // Skip logic for debugging via debugger
    if_(DWORD in_mode = 0, !GetConsoleMode(handle, &in_mode)) {
        log_error("Failed to get console mode: %d", GetLastError());
        return_err(ConfigConsoleMouseE_FailedEnableGetMode());
    } else {
        in_mode |= ENABLE_MOUSE_INPUT;
        in_mode &= ~ENABLE_QUICK_EDIT_MODE;
        if (!SetConsoleMode(handle, in_mode)) {
            log_error("Failed to set console mode: %d", GetLastError());
            return_err(ConfigConsoleMouseE_FailedEnableSetMode());
        }
    }
    return_ok({});
} $unscoped_(fn);
/// Disable mouse input in the console.
fn_((disableConsoleMouse(engine_core_Vt100* self))(E$void) $scope) {
    let handle = self->client.handle.input;
    // debug_only(if (IsDebuggerPresent()) { return_void(); }); // Skip logic for debugging via debugger
    if_(DWORD in_mode = 0, !GetConsoleMode(handle, &in_mode)) {
        log_error("Failed to get console mode: %d", GetLastError());
        return_err(ConfigConsoleMouseE_FailedDisableGetMode());
    } else {
        in_mode &= ~ENABLE_MOUSE_INPUT;
        in_mode |= ENABLE_QUICK_EDIT_MODE;
        if (!SetConsoleMode(handle, in_mode)) {
            log_error("Failed to set console mode: %d", GetLastError());
            return_err(ConfigConsoleMouseE_FailedDisableSetMode());
        }
    }
    return_ok({});
} $unscoped_(fn);

fn_((processConsoleMouseButton(engine_core_Vt100* self, engine_MouseButton button, bool is_down))(void)) {
    let input = self->input;
    let mouse = input->mouse;

    let curr_state = A_at(mouse->buttons.curr_states, as$((usize)(button)));
    let prev_state = *A_at(mouse->buttons.prev_states, as$((usize)(button)));

    // Clear previous state flags
    *curr_state = engine_KeyButtonStates_none;

    if (is_down) {
        if (!(prev_state & engine_KeyButtonStates_held)) {
            // Key was just pressed
            *curr_state |= engine_KeyButtonStates_pressed;

            // Create pressed event
            engine_InputEventBuffer_push(input, variant_of$(engine_InputEvent, engine_InputEvent_mouse_press, { .button = button }));
        }
        // Key is being held
        *curr_state |= engine_KeyButtonStates_held;

        // Create held event
        engine_InputEventBuffer_push(input, variant_of$(engine_InputEvent, engine_InputEvent_mouse_hold, { .button = button }));
    } else if (prev_state & (engine_KeyButtonStates_pressed | engine_KeyButtonStates_held)) {
        // Key was just released
        *curr_state |= engine_KeyButtonStates_released;

        // Create released event
        engine_InputEventBuffer_push(input, variant_of$(engine_InputEvent, engine_InputEvent_mouse_release, { .button = button }));
    }
}
/// Read and process mouse-related events from the console input.
/// In a real engine, you might dispatch these events into an input system
/// or event queue. For simplicity, we’ll do minimal handling here.
fn_((processConsoleMouseEvents(engine_core_Vt100* self))(void)) {
    let mouse = self->input->mouse;

    // Save previous button states
    memcpy(mouse->buttons.prev_states.buf, mouse->buttons.curr_states.buf, sizeOf(mouse->buttons.curr_states.buf));

    // Example read
    INPUT_RECORD records[32] = cleared();
    DWORD        readCount   = 0;
    HANDLE       handle      = self->client.handle.input;

    while (PeekConsoleInput(handle, records, 32, &readCount) && readCount > 0) {
        // Actually read them out
        ReadConsoleInput(handle, records, readCount, &readCount);
        for (DWORD i = 0; i < readCount; ++i) {
            if (records[i].EventType == MOUSE_EVENT) {
                MOUSE_EVENT_RECORD mouseEvent = records[i].Event.MouseEvent;

                // For mouse button events, you might want to check these flags
                if (!(mouseEvent.dwEventFlags & (MOUSE_WHEELED | MOUSE_HWHEELED | MOUSE_MOVED))) {
                    // Process button state changes
                    if (mouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
                        processConsoleMouseButton(self, engine_MouseButton_left, true);
                    } else {
                        processConsoleMouseButton(self, engine_MouseButton_left, false);
                    }

                    if (mouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
                        processConsoleMouseButton(self, engine_MouseButton_right, true);
                    } else {
                        processConsoleMouseButton(self, engine_MouseButton_right, false);
                    }

                    if (mouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) {
                        processConsoleMouseButton(self, engine_MouseButton_middle, true);
                    } else {
                        processConsoleMouseButton(self, engine_MouseButton_middle, false);
                    }

                    if (mouseEvent.dwButtonState & FROM_LEFT_3RD_BUTTON_PRESSED) {
                        processConsoleMouseButton(self, engine_MouseButton_x1, true);
                    } else {
                        processConsoleMouseButton(self, engine_MouseButton_x1, false);
                    }

                    if (mouseEvent.dwButtonState & FROM_LEFT_4TH_BUTTON_PRESSED) {
                        processConsoleMouseButton(self, engine_MouseButton_x2, true);
                    } else {
                        processConsoleMouseButton(self, engine_MouseButton_x2, false);
                    }

                    // Similar handling for other buttons
                }

                /* // Update mouse cursor position
                self->input->mouse.cursor.prev_pos = self->input->mouse.cursor.curr_pos;
                self->input->mouse.cursor.curr_pos = (Vec2i){
                    .x = mouseEvent.dwMousePosition.X,
                    .y = mouseEvent.dwMousePosition.Y
                }; */

                // Handle wheel events
                if (mouseEvent.dwEventFlags & MOUSE_WHEELED) {
                    // Get the wheel delta (high word of dwButtonState)
                    SHORT wheel_delta = HIWORD(mouseEvent.dwButtonState);

                    // Update mouse wheel data
                    self->input->mouse->wheel.prev_scroll_amount = self->input->mouse->wheel.curr_scroll_amount;
                    self->input->mouse->wheel.curr_scroll_amount.y += as$((f32)(wheel_delta)) / as$((f32)(WHEEL_DELTA));

                    // Create wheel event if needed
                    engine_InputEventBuffer_push(
                        self->input,
                        variant_of$(
                            engine_InputEvent,
                            engine_InputEvent_mouse_scroll,
                            { .delta = m_V2f32_from(0, wheel_delta / as$((f32)(WHEEL_DELTA))) }
                        )
                    );
                }

                // Handle horizontal wheel if needed
                if (mouseEvent.dwEventFlags & MOUSE_HWHEELED) {
                    SHORT wheel_delta = HIWORD(mouseEvent.dwButtonState);

                    self->input->mouse->wheel.prev_scroll_amount = self->input->mouse->wheel.curr_scroll_amount;
                    self->input->mouse->wheel.curr_scroll_amount.x += as$((f32)(wheel_delta)) / as$((f32)(WHEEL_DELTA));

                    engine_InputEventBuffer_push(
                        self->input,
                        variant_of$(
                            engine_InputEvent,
                            engine_InputEvent_mouse_scroll,
                            { .delta = m_V2f32_from(wheel_delta / as$((f32)(WHEEL_DELTA)), 0) }
                        )
                    );
                }
            }
        }
    }

    {
        POINT mouseScreenPos;
        GetCursorPos(&mouseScreenPos);
        ScreenToClient(self->client.handle.window, &mouseScreenPos);
        mouseScreenPos.x += 0;
        mouseScreenPos.y += 0;

        // // Apply offsets we stored in syncWindowMetrics
        // mouseScreenPos.x -= as$(i32, self->client.pos_on_display.top_left.x);
        // mouseScreenPos.y -= as$(i32, self->client.pos_on_display.top_left.y);

        // Grab the current font metrics for the console
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        GetConsoleScreenBufferInfo(self->client.handle.output, &bufferInfo);

        CONSOLE_FONT_INFOEX fontInfo = { .cbSize = sizeof(CONSOLE_FONT_INFOEX) };
        GetCurrentConsoleFontEx(self->client.handle.output, false, &fontInfo);

        i32 fontWidth  = fontInfo.dwFontSize.X;
        i32 fontHeight = fontInfo.dwFontSize.Y;
        if (fontWidth <= 0 || fontHeight <= 0) {
            // Fallback if cannot get the font size
            fontWidth  = as$((i32)(self->client.metrics.dim.x)) / bufferInfo.dwSize.X;
            fontHeight = as$((i32)(self->client.metrics.dim.y)) / bufferInfo.dwSize.Y;
        }

        // Protect against negative positions (mouse outside the client area).
        i32 consoleX = prim_max(0, mouseScreenPos.x / fontWidth);
        i32 consoleY = prim_max(0, mouseScreenPos.y / fontHeight);

        // Check if the mouse is in the upper or lower half of the character cell.
        bool isLowerHalf = (mouseScreenPos.y % fontHeight) > (fontHeight / 2);

        // Because this engine uses two vertical “pixels” per cell,
        // multiply consoleY by 2 and add 1 if in the lower half.
        self->input->mouse->cursor.prev_pos = self->input->mouse->cursor.curr_pos;
        self->input->mouse->cursor.curr_pos = (Vec2i){
            .x = consoleX,
            .y = consoleY * 2 + (isLowerHalf ? 1 : 0)
        };

        // Clamp to valid resolution
        self->input->mouse->cursor.curr_pos.x = prim_min(
            self->input->mouse->cursor.curr_pos.x,
            as$((i32)(self->client.metrics.res.curr.x)) - 1
        );
        self->input->mouse->cursor.curr_pos.y = prim_min(
            self->input->mouse->cursor.curr_pos.y,
            as$((i32)(self->client.metrics.res.curr.y)) - 1
        );
    }
}

/*========== Lifecycle Implementation =======================================*/

config_ErrSet(InitErr,
    FailedGetConsoleWindowHandle,
    FailedGetConsoleOutputHandle,
    FailedGetConsoleInputHandle
);
fn_((engine_core_Vt100_init(engine_core_Vt100_Config config))(E$P$engine_core_Vt100) $guard) {
    debug_assert_nonnull(config.window);
    debug_assert_nonnull(config.input);

    let allocator = unwrap(config.allocator);
    let self      = meta_cast$(engine_core_Vt100*, try_(mem_Allocator_create(allocator, typeInfo$(engine_core_Vt100))));
    errdefer_($ignore, mem_Allocator_destroy(allocator, anyPtr(self)));

    self->allocator       = allocator;
    self->abstract.window = blk({
        let window = config.window;
        toSome(&window->backend, engine_core_Vt100_backend(self));
        blk_return window;
    });
    self->abstract.buffer = blk({
        let allocator = self->allocator;
        let window    = self->abstract.window;
        let size      = calcAbstractBufferSize(Grid_width(window->composite_buffer->buffer), Grid_height(window->composite_buffer->buffer));
        blk_return type$(ArrList$u8, try_(ArrList_initCap(typeInfo$(u8), allocator, size)));
    });
    errdefer_($ignore, ArrList_fini(self->abstract.buffer.base));
    self->input = blk({
        let input = config.input;
        toSome(&input->backend, engine_core_Vt100_backend(self));
        blk_return input;
    });

    self->client.handle.window = blk({
        let handle = GetForegroundWindow();
        if (!handle || handle == INVALID_HANDLE_VALUE) {
            log_error("Failed to get foreground window: %d", GetLastError());
            return_err(InitE_FailedGetConsoleWindowHandle());
        }
        blk_return handle;
    });
#define engine_core_Vt100_comp_using_screen_buffer 0
#if !engine_core_Vt100_comp_using_screen_buffer
    self->client.handle.output = blk({
        let handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!handle || handle == INVALID_HANDLE_VALUE) {
            log_error("Failed to get standard output handle: %d", GetLastError());
            return_err(InitE_FailedGetConsoleOutputHandle());
        }
        blk_return handle;
    });
#else
    self->client.handle.output = blk({
        // Create a new console screen buffer
        let handle = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,       // access
            FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode
            NULL,                               // security attributes
            CONSOLE_TEXTMODE_BUFFER,            // console mode
            NULL                                // used by CONSOLE_GRAPHICS_BUFFER
        );
        if (!handle || handle == INVALID_HANDLE_VALUE) {
            log_error("Failed to create console screen buffer: %d", GetLastError());
            return_err(InitE_FailedGetConsoleOutputHandle());
        }
        blk_return handle;
    });
#endif
    self->client.handle.input = blk({
        let handle = GetStdHandle(STD_INPUT_HANDLE);
        if (!handle || handle == INVALID_HANDLE_VALUE) {
            log_error("Failed to get standard input handle: %d", GetLastError());
            return_err(InitE_FailedGetConsoleInputHandle());
        }
        blk_return handle;
    });

    try_(configureConsoleOutput(self));
    try_(configureConsoleInput(self));

    try_(hideConsoleCursor(self));
    try_(enableConsoleMouse(self));

    self->client.is_focused   = true;
    self->client.is_minimized = false;
    self->client.is_maximized = false;

    return_ok(self);
} $unguarded_(fn);

fn_((engine_core_Vt100_fini(engine_core_Vt100* self))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self);
    debug_assert_nonnull(self);

    catch_((disableConsoleMouse(self))(err, {
        Err_print(err);
        ErrTrace_print();
        claim_unreachable;
    }));
    catch_((showConsoleCursor(self))(err, {
        Err_print(err);
        ErrTrace_print();
        claim_unreachable;
    }));

    ArrList_fini(self->abstract.buffer.base);
    mem_Allocator_destroy(self->allocator, anyPtr(self));
}

/*========== Interface Implementation =======================================*/

/// Poll or process events each frame/tick:
/// - Mouse events
/// - Possibly check for console window resizing/focus changes
/// - Update your engine’s internal state
fn_((processEvents(P$raw ctx))(void)) {
    debug_assert_nonnull(ctx);
    let self = as$((engine_core_Vt100*)(ctx));

    // Update window state
    if_(WINDOWPLACEMENT placement = { .length = sizeof(WINDOWPLACEMENT) },
       !GetWindowPlacement(self->client.handle.window, &placement)) {
        log_error("Failed to get window placement: %d", GetLastError());
        claim_unreachable;
    } else {
        const bool is_minimized = placement.showCmd == SW_SHOWMINIMIZED;
        const bool is_maximized = placement.showCmd == SW_SHOWMAXIMIZED;
        const bool is_focused   = blk({
            let handle = GetForegroundWindow();
            if (!handle || handle == INVALID_HANDLE_VALUE) {
                log_error("Failed to get foreground window: %d", GetLastError());
                claim_unreachable;
            }
            blk_return handle == self->client.handle.window;
          });

        self->client.is_minimized = is_minimized;
        self->client.is_maximized = is_maximized;
        self->client.is_focused   = is_focused;
    }

    catch_((syncWindowMetrics(self))(err, {
        Err_print(err);
        ErrTrace_print();
        claim_unreachable;
    }));
    processConsoleKeyboardEvents(self);
    processConsoleMouseEvents(self);
    // catch_default(engine_Input_update(self->input), claim_unreachable);
}

/// Presents the current buffer content to the console.
/// For a text-based approach, you could simply write the stored data to the console.
/// If you’re storing color/ANSI sequences, handle them accordingly.
fn_((presentBuffer(P$raw ctx))(void)) {
    debug_assert_nonnull(ctx);
    let self = as$((engine_core_Vt100*)(ctx));
    if (self->client.is_minimized) { return; }

    // Clear buffer for draw a new content. This is a simple way to reset the console.
    ArrList_clearRetainingCap(self->abstract.buffer.base);
    mem_setBytes(self->abstract.buffer.items.ptr, 0, self->abstract.buffer.cap);

    let rect   = abstractWindowRect(self);
    let width  = rect.x;
    let height = rect.y;
    let pixels = self->abstract.window->composite_buffer->buffer;
    for (u32 y = 0; (y + 1) < height; y += 2) {
        for (u32 x = 0; x < width; ++x) {
            // Get upper and lower pixels
            const Color upper = *Grid_at(pixels, x, y);
            const Color lower = *Grid_at(pixels, x, y + 1);

            // clang-format off
            // Find run length of identical color pairs
            usize run_length = 1;
            while ((x + run_length) < width) {
                const Color next_upper = *Grid_at(pixels, x + run_length, y);
                const Color next_lower = *Grid_at(pixels, x + run_length, y + 1);

                if (memcmp(&upper.channels, &next_upper.channels, sizeof(Color)) != 0
                 || memcmp(&lower.channels, &next_lower.channels, sizeof(Color)) != 0) {
                    break;
                }
                run_length++;
            }

            // Write color sequence
            self->abstract.buffer.items.len += sprintf(
                as$((char*)(self->abstract.buffer.items.ptr + self->abstract.buffer.items.len)),
                "\033[38;2;%d;%d;%d;48;2;%d;%d;%dm",
                upper.r, upper.g, upper.b,
                lower.r, lower.g, lower.b
            );
            // clang-format on

            // Write half-blocks for the run
            for (usize i = 0; i < run_length; ++i) {
                memcpy(self->abstract.buffer.items.ptr + self->abstract.buffer.items.len, "▀", strlen("▀"));
                self->abstract.buffer.items.len += strlen("▀");
            }
            x += run_length - 1; // -1 because loop will increment
        }
        self->abstract.buffer.items.ptr[self->abstract.buffer.items.len++] = '\n';
    }
    self->abstract.buffer.items.ptr[--self->abstract.buffer.items.len] = '\0';

    catch_((resetConsoleCursorPos(self))(err, {
        Err_print(err);
        ErrTrace_print();
        claim_unreachable;
    }));
    /* Write buffer content */ {
        let handle = self->client.handle.output;
        let ptr    = self->abstract.buffer.items.ptr;
        let len    = self->abstract.buffer.items.len;
        if_(DWORD written = 0, !WriteConsoleA(handle, ptr, len, &written, null) || written != len) {
            log_error("Failed to write console buffer content: %d", GetLastError());
            claim_unreachable;
        }
    }
    catch_((resetConsoleCursorPos(self))(err, {
        Err_print(err);
        ErrTrace_print();
        claim_unreachable;
    }));
}

fn_((getWindowPos(const P$raw ctx))(Vec2i)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return self->client.pos_on_display.top_left;
}

fn_((getWindowDim(const P$raw ctx))(Vec2u)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return self->client.metrics.dim;
}

fn_((getWindowRes(const P$raw ctx))(Vec2u)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return self->client.metrics.res.curr;
}

fn_((getWindowMinRes(const P$raw ctx))(Vec2u)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return self->client.metrics.res.min;
}

fn_((getWindowMaxRes(const P$raw ctx))(Vec2u)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return self->client.metrics.res.max;
}

fn_((setWindowMinRes(P$raw ctx, Vec2u size))(E$void) $scope) {
    debug_assert_nonnull(ctx);
    $unused(ctx), $unused(size);
    return_err(E_NotImplemented()); /* TODO: Implement this function */
} $unscoped_(fn);

fn_((setWindowMaxRes(P$raw ctx, Vec2u size))(E$void) $scope) {
    debug_assert_nonnull(ctx);
    $unused(ctx), $unused(size);
    return_err(E_NotImplemented()); /* TODO: Implement this function */
} $unscoped_(fn);

fn_((isWindowFocused(const P$raw ctx))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return self->client.is_focused;
}

fn_((isWindowMinimized(const P$raw ctx))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return self->client.is_minimized;
}

fn_((isWindowMaximized(const P$raw ctx))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return self->client.is_maximized;
}

fn_((getKeyboardState(const P$raw ctx, engine_KeyCode key))(u8)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Keyboard_getState(self->input->keyboard, key);
}

fn_((isKeyboardState(const P$raw ctx, engine_KeyCode key, engine_KeyButtonStates state))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Keyboard_isState(self->input->keyboard, key, state);
}

fn_((pressedKeyboard(const P$raw ctx, engine_KeyCode key))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Keyboard_pressed(self->input->keyboard, key);
}

fn_((heldKeyboard(const P$raw ctx, engine_KeyCode key))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Keyboard_held(self->input->keyboard, key);
}

fn_((releasedKeyboard(const P$raw ctx, engine_KeyCode key))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Keyboard_released(self->input->keyboard, key);
}

fn_((getMouseState(const P$raw ctx, engine_MouseButton button))(u8)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Mouse_getState(self->input->mouse, button);
}

fn_((isMouseState(const P$raw ctx, engine_MouseButton button, engine_KeyButtonStates state))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Mouse_isState(self->input->mouse, button, state);
}

fn_((pressedMouse(const P$raw ctx, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Mouse_pressed(self->input->mouse, button);
}

fn_((heldMouse(const P$raw ctx, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Mouse_held(self->input->mouse, button);
}

fn_((releasedMouse(const P$raw ctx, engine_MouseButton button))(bool)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Mouse_released(self->input->mouse, button);
}

fn_((getMousePos(const P$raw ctx))(Vec2i)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Mouse_getPos(self->input->mouse);
}

fn_((getMousePosDelta(const P$raw ctx))(Vec2i)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Mouse_getPosDelta(self->input->mouse);
}

fn_((getMouseWheelScrollDelta(const P$raw ctx))(Vec2f)) {
    debug_assert_nonnull(ctx);
    let self = as$((const engine_core_Vt100*)(ctx));
    return engine_Mouse_getWheelScrollDelta(self->input->mouse);
}

#else

// Provide stubs or alternative implementations for non-Windows platforms
// or simply leave them empty if engine only supports Windows for Vt100.

#endif
