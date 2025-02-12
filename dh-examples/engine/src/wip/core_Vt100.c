#include "dh/Err.h"
#include "engine-wip/core/Vt100.h"
#include "engine-wip/Input.h"
#include "Backend_Internal.h"
#include "dh/ArrList.h"

#if bti_plat_windows
#include <windows.h>

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
            HWND   window; // Handle to console window
            HANDLE output; // Handle to console screen buffer
            HANDLE input;  // Handle to console screen buffer
        } handle;
        struct {
            Vec2u top_left;
        } pos_on_display;
        struct {
            Vec2u dim;
            struct {
                Vec2u min;
                Vec2u max;
                Vec2u curr;
            } res;
        } metrics;
        bool is_focused;
        bool is_minimized;
        bool is_maximized;
    } client;
    mem_Allocator allocator;
};
config_ErrSet(engine_core_Vt100_Err);

static void processEvents(anyptr ctx); /* TODO: validate */ // processing events and updating properties
static void presentBuffer(anyptr ctx); /* TODO: validate */ // only present

static Vec2u getWindowPos(const anyptr ctx);
static Vec2u getWindowDim(const anyptr ctx);
static Vec2u getWindowRes(const anyptr ctx);

static Vec2u    getWindowMinRes(const anyptr ctx);
static Vec2u    getWindowMaxRes(const anyptr ctx);
static Err$void setWindowMinRes(anyptr ctx, Vec2u size) must_check;
static Err$void setWindowMaxREs(anyptr ctx, Vec2u size) must_check;

static bool isWindowFocused(const anyptr ctx);
static bool isWindowMinimized(const anyptr ctx);
static bool isWindowMaximized(const anyptr ctx);

static u8   getKeyboardState(const anyptr ctx, engine_KeyCode key);
static bool isKeyboardState(const anyptr ctx, engine_KeyCode key, engine_KeyButtonStates state);

static bool pressedKeyboard(const anyptr ctx, engine_KeyCode key);
static bool heldKeyboard(const anyptr ctx, engine_KeyCode key);
static bool releasedKeyboard(const anyptr ctx, engine_KeyCode key);

static u8   getMouseState(const anyptr ctx, engine_MouseButton button);
static bool isMouseState(const anyptr ctx, engine_MouseButton button, engine_KeyButtonStates state);

static bool pressedMouse(const anyptr ctx, engine_MouseButton button);
static bool heldMouse(const anyptr ctx, engine_MouseButton button);
static bool releasedMouse(const anyptr ctx, engine_MouseButton button);

static Vec2i getMousePos(const anyptr ctx);
static Vec2i getMouseDelta(const anyptr ctx);
static Vec2f getMouseWheelScrollDelta(const anyptr ctx);

engine_Backend engine_core_Vt100_backend(engine_core_Vt100* self) {
    static const engine_BackendVT vt[1] = { {
        .processEvents = processEvents,
        .presentBuffer = presentBuffer,
    } };

    static const engine_BackendVT_Internal vt_internal[1] = { {
        .getWindowPos = getWindowPos,
        .getWindowDim = getWindowDim,
        .getWindowRes = getWindowRes,

        .getWindowMinRes = getWindowMinRes,
        .getWindowMaxRes = getWindowMaxRes,
        .setWindowMinRes = setWindowMinRes,
        .setWindowMaxRes = setWindowMaxREs,

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
        .getMousePosDelta         = getMouseDelta,
        .getMouseWheelScrollDelta = getMouseWheelScrollDelta,
    } };

    return (engine_Backend){
        .ptr         = self,
        .vt          = vt,
        .vt_internal = vt_internal
    };
}

/*========== Forward declarations ===========================================*/

force_inline usize calcAbstractBufferSize(u32 width, u32 height) { return as$(usize, width) * height * 40; }
force_inline Vec2u clientWindowPixelRect(engine_core_Vt100* self);   /* TODO: validate */
force_inline Vec2u clientOutputConsoleRect(engine_core_Vt100* self); /* TODO: validate */

force_inline Vec2u abstractWindowRect(engine_core_Vt100* self);
force_inline usize abstractBufferCapSize(engine_core_Vt100* self);

force_inline bool needsResizeAbstractWindow(engine_core_Vt100* self);       /* TODO: validate */
static Err$void   resizeAbstractWindow(engine_core_Vt100* self) must_check; /* TODO: validate */
static Err$void   syncWindowMetrics(engine_core_Vt100* self) must_check;

static Err$void configureConsoleOutput(engine_core_Vt100* self) must_check;
static Err$void configureConsoleInput(engine_core_Vt100* self) must_check;

force_inline Err$void hideConsoleCursor(engine_core_Vt100* self) must_check;
force_inline Err$void showConsoleCursor(engine_core_Vt100* self) must_check;
force_inline Err$void resetConsoleCursorPos(engine_core_Vt100* self) must_check;

force_inline Err$void enableConsoleMouse(engine_core_Vt100* self) must_check;
force_inline Err$void disableConsoleMouse(engine_core_Vt100* self) must_check;
static Err$void       processConsoleMouseEvents(engine_core_Vt100* self) must_check; /* TODO: validate */

/*========== Implementations ================================================*/

/// Returns the size (width, height) in pixels of the client area
/// for the console window.
force_inline Vec2u clientWindowPixelRect(engine_core_Vt100* self) {
    if_(RECT rect = cleared(), GetClientRect(self->client.handle.window, &rect)) {
        // Update client metrics in sync
        self->client.metrics.current_size = (Vec2u){
            .x = as$(u32, rect.right - rect.left),
            .y = as$(u32, rect.bottom - rect.top)
        };
        return self->client.metrics.current_size;
    }
    return self->client.metrics.current_size; // Return cached size as fallback
}

/// Returns the size (width, height) of the console screen buffer
/// in character cells (columns and rows).
force_inline Vec2u clientOutputConsoleRect(engine_core_Vt100* self) {
    if_(CONSOLE_SCREEN_BUFFER_INFO info = cleared(), GetConsoleScreenBufferInfo(self->client.handle.output, &info)) {
        // Keep buffer size in sync with console
        self->client.metrics.current_size = (Vec2u){
            .x = as$(u32, info.dwSize.X),
            .y = as$(u32, info.dwSize.Y)
        };
        return self->client.metrics.current_size;
    }
    return self->client.metrics.current_size;
}

/// Returns the current abstract window size from engine_WindowMetrics,
/// typically used for high-level engine logic.
force_inline Vec2u abstractWindowRect(engine_core_Vt100* self) {
    return (Vec2u){
        .x = self->abstract.window->composite_buffer->width,
        .y = self->abstract.window->composite_buffer->height
    };
}

/// Returns the bytes-length of the internal "abstract" buffer,
/// which you might use for storing render data before presenting.
/// If you’re using ArrList$u8 as a text buffer only, adapt as needed.
force_inline usize abstractBufferCapSize(engine_core_Vt100* self) {
    // If you store rows and columns separately in abstract.buffer, adapt accordingly.
    // For example, if you always keep a buffer big enough for (width * height * cellSize),
    // you could do something like the logic below or just re-return the window rect.
    // For demonstration, we’ll match the window’s dimension:
    return self->abstract.buffer.cap;
}

/// Checks if the abstract buffer needs to be resized based on the
/// latest window metrics or any other condition your engine considers.
force_inline bool needsResizeAbstractWindow(engine_core_Vt100* self) {
    let current_size = abstractBufferCapSize(self);
    // Compare with the expected size from your function
    let needed_size  = eval({
        let         rect = abstractWindowRect(self);
        eval_return calcAbstractBufferSize(rect.x, rect.y);
    });
    return current_size != needed_size;
}

/// Perform an actual resize of the abstract window/buffer, and also
/// reconfigure the console screen buffer size if needed.
static Err$void resizeAbstractWindow(engine_core_Vt100* self) {
    reserveReturn(Err$void);

    // 1. Re-size the console’s screen buffer
    let rect = abstractWindowRect(self);
    if (!SetConsoleScreenBufferSize(self->client.handle.output, (COORD){ as$(SHORT, rect.x), as$(SHORT, rect.y) })) {
        // If you have specific error sets for failing to resize, return them here
        return_err(Err_Unspecified());
    }

    // 2. Re-size your ArrList if needed
    let needed = calcAbstractBufferSize(rect.x, rect.y);
    try(ArrList_resize(&self->abstract.buffer.base, needed));

    return_void();
}

static Err$void syncWindowMetrics(engine_core_Vt100* self) {
    reserveReturn(Err$void);
    let                        handle      = self->client.handle.output;
    CONSOLE_SCREEN_BUFFER_INFO buffer_info = cleared();
    CONSOLE_FONT_INFOEX        fontInfo    = { .cbSize = sizeof(CONSOLE_FONT_INFOEX) };

    if (!GetCurrentConsoleFontEx(handle, false, &fontInfo)) {
        return_err(Err_Unspecified());
    }
    if (!GetConsoleScreenBufferInfo(handle, &buffer_info)) {
        return_err(Err_Unspecified());
    }

    /* TODO: Process sync logic */
    return_void();
}

config_ErrSet(
    ConfigConsoleOutputErr,
    FailedSetWindowInfo,
    FailedSetScreenBufferSize,
    FailedSetCodePage,
    FailedGetMode,
    FailedSetMode
);
/// Function to initialize console-specific configurations like screen buffer size
static Err$void configureConsoleOutput(engine_core_Vt100* self) {
    reserveReturn(Err$void);
    let handle = self->client.handle.output;

    // Initialize console window size
    if (!SetConsoleWindowInfo(handle, true, &(SMALL_RECT){ 0, 0, 1, 1 })) {
        return_err(ConfigConsoleOutputErr_FailedSetWindowInfo());
    }

    // Configure buffer size for the console
    let rect = abstractWindowRect(self);
    if (!SetConsoleScreenBufferSize(handle, (COORD){ as$(SHORT, rect.x), as$(SHORT, rect.y) })) {
        return_err(ConfigConsoleOutputErr_FailedSetScreenBufferSize());
    }
    if (!SetConsoleWindowInfo(handle, true, &(SMALL_RECT){ 0, 0, as$(SHORT, rect.x), as$(SHORT, rect.y) })) {
        return_err(ConfigConsoleOutputErr_FailedSetWindowInfo());
    }

    // Configure console output code page to UTF-8
    if (!SetConsoleOutputCP(CP_UTF8)) {
        return_err(ConfigConsoleOutputErr_FailedSetCodePage());
    }

    // Set console output mode for processing terminal sequences
    if_(DWORD out_mode = 0, !GetConsoleMode(handle, &out_mode)) {
        return_err(ConfigConsoleOutputErr_FailedGetMode());
    }
    else {
        out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
        if (!SetConsoleMode(handle, out_mode)) {
            return_err(ConfigConsoleOutputErr_FailedSetMode());
        }
    }
    return_void();
}

config_ErrSet(
    ConfigConsoleInputErr,
    FailedGetMode,
    FailedSetMode
);
/// Function to configure console input mode
static Err$void configureConsoleInput(engine_core_Vt100* self) {
    reserveReturn(Err$void);
    let handle = self->client.handle.input;
    if_(DWORD in_mode = 0, !GetConsoleMode(handle, &in_mode)) {
        return_err(ConfigConsoleInputErr_FailedGetMode());
    }
    else {
        in_mode |= ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT;
        if (!SetConsoleMode(handle, in_mode)) {
            return_err(ConfigConsoleInputErr_FailedSetMode());
        }
    }
    return_void();
}

config_ErrSet(
    ConsoleCursorErr,
    FailedHide,
    FailedShow,
    FailedResetPos
);
force_inline Err$void hideConsoleCursor(engine_core_Vt100* self) {
    reserveReturn(Err$void);
    let handle = self->client.handle.output;
    if (!SetConsoleCursorInfo(handle, &(CONSOLE_CURSOR_INFO){ 1, false })) {
        return_err(ConsoleCursorErr_FailedHide());
    }
    return_void();
}
force_inline Err$void showConsoleCursor(engine_core_Vt100* self) {
    reserveReturn(Err$void);
    let handle = self->client.handle.output;
    if (!SetConsoleCursorInfo(handle, &(CONSOLE_CURSOR_INFO){ 1, true })) {
        return_err(ConsoleCursorErr_FailedShow());
    }
    return_void();
}
force_inline Err$void resetConsoleCursorPos(engine_core_Vt100* self) {
    reserveReturn(Err$void);
    let handle = self->client.handle.output;
    if (!WriteConsoleA(handle, "\033[H", 3, null, null)) {
        return_err(ConsoleCursorErr_FailedResetPos());
    };
    return_void();
}

/// Enable mouse input through the console.
force_inline Err$void enableConsoleMouse(engine_core_Vt100* self) {
    reserveReturn(Err$void);
    let handle = self->client.handle.input;
    if_(DWORD in_mode = 0, !GetConsoleMode(handle, &in_mode)) {
        return_err(Err_Unspecified());
    }
    else {
        in_mode |= ENABLE_MOUSE_INPUT;
        in_mode &= ~ENABLE_QUICK_EDIT_MODE;
        if (!SetConsoleMode(handle, in_mode)) {
            return_err(Err_Unspecified());
        }
    }
    return_void();
}

/// Disable mouse input in the console.
force_inline Err$void disableConsoleMouse(engine_core_Vt100* self) {
    reserveReturn(Err$void);
    let handle = self->client.handle.input;
    if_(DWORD in_mode = 0, !GetConsoleMode(handle, &in_mode)) {
        return_err(Err_Unspecified());
    }
    else {
        in_mode &= ~ENABLE_MOUSE_INPUT;
        in_mode |= ENABLE_QUICK_EDIT_MODE;
        if (!SetConsoleMode(handle, in_mode)) {
            return_err(Err_Unspecified());
        }
    }
    return_void();
}

/// Read and process mouse-related events from the console input.
/// In a real engine, you might dispatch these events into an input system
/// or event queue. For simplicity, we’ll do minimal handling here.
static Err$void processConsoleMouseEvents(engine_core_Vt100* self) {
    reserveReturn(Err$void);

    // Example read
    INPUT_RECORD records[32] = cleared();
    DWORD        readCount   = 0;
    HANDLE       handle      = self->client.handle.input;

    while (PeekConsoleInput(handle, records, 32, &readCount) && readCount > 0) {
        // Actually read them out
        ReadConsoleInput(handle, records, readCount, &readCount);
        for (DWORD i = 0; i < readCount; ++i) {
            if (records[i].EventType == MOUSE_EVENT) {
                // Here you can interpret records[i].Event.MouseEvent.
                // e.g. handle clicks, movements, wheel, etc.
                // ...
            }
        }
    }
    return_void();
}

/*========== Lifecycle Implementation =======================================*/

config_ErrSet(
    engine_core_Vt100_InitErr,
    FailedGetConsoleWindowHandle,
    FailedGetOutputHandle,
    FailedGetInputHandle
);
Err$Ptr$engine_core_Vt100 engine_core_Vt100_init(const engine_core_Vt100_Config* config) {
    debug_assert_nonnull(config->allocator.ptr);
    debug_assert_nonnull(config->allocator.vt);
    debug_assert_nonnull(config->window);

    scope_reserveReturn(Err$Ptr$engine_core_Vt100) {
        let allocator = config->allocator;
        let self      = meta_cast$(engine_core_Vt100*, try(mem_Allocator_create(allocator, typeInfo$(engine_core_Vt100))));
        errdefer(mem_Allocator_destroy(allocator, anyPtr(self)));

        self->allocator       = allocator;
        self->abstract.window = eval({
            let window = config->window;
            someAsg(window->backend, engine_core_Vt100_backend(self));
            eval_return window;
        });
        self->abstract.buffer = eval({
            let         allocator = self->allocator;
            let         window    = self->abstract.window;
            let         size      = calcAbstractBufferSize(window->composite_buffer->width, window->composite_buffer->height);
            eval_return type$(ArrList$u8, try(ArrList_initCap(typeInfo$(u8), allocator, size)));
        });
        errdefer(ArrList_fini(&self->abstract.buffer.base));
        self->input = eval({
            let input = config->input;
            someAsg(input->backend, engine_core_Vt100_backend(self));
            eval_return input;
        });

        self->client.handle.window = eval({
            let handle = GetConsoleWindow();
            if (!handle || handle == INVALID_HANDLE_VALUE) {
                return_err(engine_core_Vt100_InitErr_FailedGetConsoleWindowHandle());
            }
            eval_return handle;
        });
        self->client.handle.output = eval({
            let handle = GetStdHandle(STD_OUTPUT_HANDLE);
            if (!handle || handle == INVALID_HANDLE_VALUE) {
                return_err(engine_core_Vt100_InitErr_FailedGetOutputHandle());
            }
            eval_return handle;
        });
        self->client.handle.input  = eval({
            let handle = GetStdHandle(STD_INPUT_HANDLE);
            if (!handle || handle == INVALID_HANDLE_VALUE) {
                return_err(engine_core_Vt100_InitErr_FailedGetInputHandle());
            }
            eval_return handle;
        });

        try(configureConsoleOutput(self));
        try(configureConsoleInput(self));

        try(hideConsoleCursor(self));
        try(enableConsoleMouse(self));

        self->client.is_focused   = true;
        self->client.is_minimized = false;
        self->client.is_maximized = false;

        return_ok(self);
    }
    scope_returnReserved;
};
void engine_core_Vt100_fini(engine_core_Vt100* self) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self);
    debug_assert_nonnull(self);

    catch_default(disableConsoleMouse(self), claim_unreachable);
    catch_default(showConsoleCursor(self), claim_unreachable);

    ArrList_fini(&self->abstract.buffer.base);
    mem_Allocator_destroy(self->allocator, anyPtr(self));
}

/*========== Interface Implementation =======================================*/

/// Poll or process events each frame/tick:
/// - Mouse events
/// - Possibly check for console window resizing/focus changes
/// - Update your engine’s internal state
static void processEvents(anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(engine_core_Vt100*, ctx);

    catch_default(engine_Input_update(self->input), claim_unreachable);

    // Update window state
    WINDOWPLACEMENT placement = { .length = sizeof(WINDOWPLACEMENT) };
    if (GetWindowPlacement(self->client.handle.window, &placement)) {
        self->client.is_minimized = (placement.showCmd == SW_SHOWMINIMIZED);
        self->client.is_maximized = (placement.showCmd == SW_SHOWMAXIMIZED);
        self->client.is_focused   = (GetForegroundWindow() == self->client.handle.window);
    }

    // Update current size and check for resize
    clientWindowPixelRect(self); // This updates metrics.current_size

    if (needsResizeAbstractWindow(self)) {
        catch_default(resizeAbstractWindow(self), claim_unreachable);
    }
    catch_default(processConsoleMouseEvents(self), claim_unreachable);
}

/// Presents the current buffer content to the console.
/// For a text-based approach, you could simply write the stored data to the console.
/// If you’re storing color/ANSI sequences, handle them accordingly.
static void presentBuffer(anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(engine_core_Vt100*, ctx);
    if (self->client.is_minimized) { return; }

    // Ensure buffer size matches current window size
    if (needsResizeAbstractWindow(self)) {
        catch_default(resizeAbstractWindow(self), claim_unreachable);
    }
    ArrList_clearRetainingCap(&self->abstract.buffer.base);

    let rect   = abstractWindowRect(self);
    let height = rect.y;
    let width  = rect.x;
    let pixels = eval({
        let         buffer = self->abstract.window->composite_buffer;
        eval_return Mat_fromSli$(Mat$Color, buffer->buffer, buffer->width, buffer->height);
    });
    for (u32 y = 0; (y + 1) < height; y += 2) {
        for (u32 x = 0; x < width; ++x) {
            // Get upper and lower pixels
            const Color upper = *Mat_at(pixels, x, y);
            const Color lower = *Mat_at(pixels, x, y + 1);

            // clang-format off
            // Find run length of identical color pairs
            usize run_length = 1;
            while ((x + run_length) < width) {
                const Color next_upper = *Mat_at(pixels, x + run_length, y);
                const Color next_lower = *Mat_at(pixels, x + run_length, y + 1);

                if (memcmp(&upper.channels, &next_upper.channels, sizeof(Color)) != 0
                 || memcmp(&lower.channels, &next_lower.channels, sizeof(Color)) != 0) {
                    break;
                }
                run_length++;
            }

            // Write color sequence
            self->abstract.buffer.items.len += sprintf(
                as$(char*, self->abstract.buffer.items.ptr + self->abstract.buffer.items.len),
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

    catch_default(resetConsoleCursorPos(self), claim_unreachable);
    /* Write buffer content */ {
        let handle = self->client.handle.output;
        let ptr    = self->abstract.buffer.items.ptr;
        let len    = self->abstract.buffer.items.len;
        if_(DWORD written = 0,
            !WriteConsoleA(handle, ptr, len, &written, null)
                || written != len
        ) {
            claim_unreachable;
        }
    }
    catch_default(resetConsoleCursorPos(self), claim_unreachable);
}

static Vec2u getWindowDim(const anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(engine_core_Vt100*, ctx);
    return self->client.metrics.dim;
}

static Vec2u getWindowRes(const anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(engine_core_Vt100*, ctx);
    return self->client.metrics.res.curr;
}

static Vec2u getWindowMinRes(const anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(const engine_core_Vt100*, ctx);
    return self->client.metrics.res.min;
}

static Vec2u getWindowMaxRes(const anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(const engine_core_Vt100*, ctx);
    return self->client.metrics.res.max;
}

static Err$void setWindowMinRes(anyptr ctx, Vec2u size) {
    reserveReturn(Err$void);
    debug_assert_nonnull(ctx);
    unused(ctx), unused(size);
    return_err(Err_NotImplemented()); /* TODO: Implement this function */
}

static Err$void setWindowMaxREs(anyptr ctx, Vec2u size) {
    reserveReturn(Err$void);
    debug_assert_nonnull(ctx);
    unused(ctx), unused(size);
    return_err(Err_NotImplemented()); /* TODO: Implement this function */
}

static bool isWindowFocused(const anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(const engine_core_Vt100*, ctx);
    return self->client.is_focused;
}

static bool isWindowMinimized(const anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(const engine_core_Vt100*, ctx);
    return self->client.is_minimized;
}

static bool isWindowMaximized(const anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(const engine_core_Vt100*, ctx);
    return self->client.is_maximized;
}

#else

// Provide stubs or alternative implementations for non-Windows platforms
// or simply leave them empty if engine only supports Windows for Vt100.

#endif
