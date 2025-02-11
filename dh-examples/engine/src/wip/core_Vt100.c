#include "engine-wip/core/Vt100.h"
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
    struct {
        struct {
            HWND   window; // Handle to console window
            HANDLE output; // Handle to console screen buffer
            HANDLE input;  // Handle to console screen buffer
        } handle;
        struct {
            Vec2u top_left;
        } pos_in_monitor;
        struct {
            Vec2u min_size;
            Vec2u max_size;
            Vec2u current_size;
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

static Vec2u getWindowSize(anyptr ctx);

static Vec2u    getWindowMinSize(anyptr ctx);
static Vec2u    getWindowMaxSize(anyptr ctx);
static Err$void setWindowMinSize(anyptr ctx, Vec2u size) must_check;
static Err$void setWindowMaxSize(anyptr ctx, Vec2u size) must_check;

static bool isWindowFocused(anyptr ctx);
static bool isWindowMinimized(anyptr ctx);
static bool isWindowMaximized(anyptr ctx);

engine_Backend engine_core_Vt100_backend(void) {
    static engine_core_Vt100 ctx[1] = cleared();

    static const engine_BackendVT vt[1] = { {
        .processEvents = processEvents,
        .presentBuffer = presentBuffer,
    } };

    static const engine_BackendVT_Internal vt_internal[1] = { {
        .getWindowSize = getWindowSize,

        .getWindowMinSize = getWindowMinSize,
        .getWindowMaxSize = getWindowMaxSize,
        .setWindowMinSize = setWindowMinSize,
        .setWindowMaxSize = setWindowMaxSize,

        .isWindowFocused   = isWindowFocused,
        .isWindowMinimized = isWindowMinimized,
        .isWindowMaximized = isWindowMaximized,
    } };

    return (engine_Backend){
        .ptr         = ctx,
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

force_inline bool needsResizeAbstractWindow(engine_core_Vt100* self); /* TODO: validate */
static Err$void   resizeAbstractWindow(engine_core_Vt100* self);      /* TODO: validate */

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
        in_mode |= ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
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
        in_mode |= ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
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
Err$void engine_core_Vt100_init(engine_Backend self, const engine_core_Vt100_Config* config) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(config->window);
    debug_assert_nonnull(config->allocator.ptr);
    debug_assert_nonnull(config->allocator.vt);

    scope_reserveReturn(Err$void) {
        let core              = as$(engine_core_Vt100*, self.ptr);
        core->allocator       = config->allocator;
        core->abstract.window = eval({
            let window = config->window;
            assignSome(window->backend, self);
            eval_return window;
        });
        core->abstract.buffer = eval({
            let         allocator = core->allocator;
            let         window    = core->abstract.window;
            let         size      = calcAbstractBufferSize(window->composite_buffer->width, window->composite_buffer->height);
            eval_return type$(ArrList$u8, try(ArrList_initCap(typeInfo$(u8), allocator, size)));
        });
        errdefer(ArrList_fini(&core->abstract.buffer.base));
        core->client.handle.window = eval({
            let handle = GetConsoleWindow();
            if (!handle || handle == INVALID_HANDLE_VALUE) {
                return_err(engine_core_Vt100_InitErr_FailedGetConsoleWindowHandle());
            }
            eval_return handle;
        });
        core->client.handle.output = eval({
            let handle = GetStdHandle(STD_OUTPUT_HANDLE);
            if (!handle || handle == INVALID_HANDLE_VALUE) {
                return_err(engine_core_Vt100_InitErr_FailedGetOutputHandle());
            }
            eval_return handle;
        });
        core->client.handle.input  = eval({
            let handle = GetStdHandle(STD_INPUT_HANDLE);
            if (!handle || handle == INVALID_HANDLE_VALUE) {
                return_err(engine_core_Vt100_InitErr_FailedGetInputHandle());
            }
            eval_return handle;
        });

        try(configureConsoleOutput(core));
        try(configureConsoleInput(core));

        // Hide cursor by default
        try(hideConsoleCursor(core));

        core->client.is_focused   = true;
        core->client.is_minimized = false;
        core->client.is_maximized = false;

        return_void();
    }
    scope_returnReserved;
};
void engine_Core_Vt100_fini(engine_Backend self) {
    unused(self); /* TODO: Implement this function */ /* TODO: Remove unused self warning */
}

/*========== Interface Implementation =======================================*/

/// Poll or process events each frame/tick:
/// - Mouse events
/// - Possibly check for console window resizing/focus changes
/// - Update your engine’s internal state
static void processEvents(anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(engine_core_Vt100*, ctx);

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

use_Sli$(Color);
use_Mat$(Color);
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
        eval_return Mat_fromSli$(Mat$Color, Sli_asNamed$(Sli$Color, buffer->buffer), buffer->width, buffer->height);
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

static Vec2u getWindowSize(anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(engine_core_Vt100*, ctx);
    return clientWindowPixelRect(self);
}

static Vec2u getWindowMinSize(anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(engine_core_Vt100*, ctx);
    return self->client.metrics.min_size;
}

static Vec2u getWindowMaxSize(anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(engine_core_Vt100*, ctx);
    return self->client.metrics.max_size;
}

static Err$void setWindowMinSize(anyptr ctx, Vec2u size) {
    reserveReturn(Err$void);
    debug_assert_nonnull(ctx);
    unused(ctx), unused(size);
    return_err(Err_NotImplemented()); /* TODO: Implement this function */
}

static Err$void setWindowMaxSize(anyptr ctx, Vec2u size) {
    reserveReturn(Err$void);
    debug_assert_nonnull(ctx);
    unused(ctx), unused(size);
    return_err(Err_NotImplemented()); /* TODO: Implement this function */
}

static bool isWindowFocused(anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(engine_core_Vt100*, ctx);
    return self->client.is_focused;
}

static bool isWindowMinimized(anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(engine_core_Vt100*, ctx);
    return self->client.is_minimized;
}

static bool isWindowMaximized(anyptr ctx) {
    debug_assert_nonnull(ctx);
    let self = as$(engine_core_Vt100*, ctx);
    return self->client.is_maximized;
}

#else

// Provide stubs or alternative implementations for non-Windows platforms
// or simply leave them empty if engine only supports Windows for Vt100.

#endif
