/**
 * @file    Runtime.h
 * @brief   Engine runtime - central controller for dage
 * @details Manages Backend, Windows, and the main loop.
 *
 * ## Ownership Model
 * - Runtime owns Backend (injected at init)
 * - Runtime owns Windows (created via createWindow)
 * - Runtime coordinates event dispatch
 *
 * ## Main Loop Flow
 * ```
 * while (!shouldQuit) {
 *     processEvents();    // Pump Backend → dispatch to Windows
 *     user_update();      // Game logic
 *     presentAll();       // Composite and present all Windows
 * }
 * ```
 *
 * ## Error Boundaries
 * - init: May fail (Backend injection, allocation)
 * - createWindow: May fail (target creation, allocation)
 * - processEvents: Never fails (0 events is valid)
 * - presentAll: Never fails
 */
#ifndef dage_Runtime__included
#define dage_Runtime__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dage/common.h"
#include "dage/Backend.h"
#include "dage/Window.h"

/*========== Macros and Declarations ========================================*/

/*========== Runtime Constants ==========*/

#define dage_Runtime_window_limit (8)
#define dage_Runtime_event_buffer_size (256)

/*========== Runtime Structure ==========*/

/// @brief Engine runtime context
typedef struct dage_Runtime {
    /*=== Core Systems ===*/
    dage_Backend backend; /* Injected backend (non-owning, caller manages lifetime) */
    ArrList$P$dage_Window windows; /* Managed windows (owns) */

    /*=== State ===*/
    struct {
        bool should_quit;
        bool is_running;
    } state;

    /*=== Internal ===*/
    dage_WindowId next_window_id;
    mem_Allocator gpa;
} dage_Runtime;
T_use_E$(dage_Runtime);

/*========== Runtime Configuration ==========*/

/// @brief Configuration for creating runtime
typedef struct dage_Runtime_Cfg {
    mem_Allocator gpa;
    dage_Backend backend; /* Backend instance (caller retains ownership of underlying impl) */
} dage_Runtime_Cfg;

/*========== Lifecycle ==========*/

/// @brief Initialize runtime with backend
/// @param cfg Configuration with injected backend
/// @return    Runtime on success, error on failure
/// @note      Error boundary - allocation may fail
///
/// @example
/// ```c
/// // Create backend first (user chooses implementation)
/// var wsi = try_(dage_core_WSI_init(...));
/// defer_(dage_core_WSI_fini(&wsi));
/// let backend = dage_core_WSI_backend(wsi);
///
/// // Inject into runtime
/// let runtime = try_(dage_Runtime_init((dage_Runtime_Cfg){
///     .gpa = gpa,
///     .backend = backend,
/// }));
/// defer_(dage_Runtime_fini(&runtime));
/// ```
$attr($must_check)
$extern fn_((dage_Runtime_init(dage_Runtime_Cfg cfg))(E$dage_Runtime));
/// @brief Shutdown runtime and free resources
/// @note  Destroys all windows but NOT the backend (caller owns that)
$extern fn_((dage_Runtime_fini(dage_Runtime* self))(void));

/*========== Window Management ==========*/

/// @brief Window creation configuration (extends dage_Window_Cfg)
typedef struct dage_Runtime_WindowCfg {
    /* Window configuration */
    m_V2u32 size;
    O$color_RGBA clear_color;

    /* Target configuration (for Backend) */
    O$S_const$u8 title;
    f32 scale;
    bool resizable;
    bool borderless;
    bool fullscreen;
} dage_Runtime_WindowCfg;

/// @brief Create a new window with backend target
/// @return Window ID on success, error on failure
/// @note   Error boundary - target creation or allocation may fail
$attr($must_check)
$extern fn_((dage_Runtime_createWindow(dage_Runtime* self, dage_Runtime_WindowCfg cfg))(E$dage_WindowId));
/// @brief Destroy a window and its backend target
$extern fn_((dage_Runtime_destroyWindow(dage_Runtime* self, dage_WindowId id))(void));
/// @brief Get window by ID
/// @return Window pointer, or None if not found
$extern fn_((dage_Runtime_getWindow(dage_Runtime* self, dage_WindowId id))(O$P$dage_Window));
/// @brief Get window by target ID (used during event dispatch)
/// @return Window pointer, or None if not found
$extern fn_((dage_Runtime_findWindowByTarget(dage_Runtime* self, dage_TargetId target_id))(O$P$dage_Window));
/// @brief Get number of windows
$attr($inline_always)
$static fn_((dage_Runtime_getWindowCount(const dage_Runtime* self))(usize));
/// @brief Iterate over all windows
$attr($inline_always)
$static fn_((dage_Runtime_getWindows(dage_Runtime* self))(S$P$dage_Window));

/*========== Main Loop ==========*/

/// @brief Process all pending events
/// @details
/// 1. Pumps events from Backend
/// 2. Dispatches events to appropriate Windows
/// 3. Updates Window InputState
///
/// @note Never fails - 0 events is valid
$extern fn_((dage_Runtime_processEvents(dage_Runtime* self))(void));
/// @brief Present all windows
/// @details
/// 1. Calls composite() on each window
/// 2. Presents composite buffer to Backend
///
/// @note Never fails
$extern fn_((dage_Runtime_presentAll(dage_Runtime* self))(void));
/// @brief End frame for all windows
/// @details Clears transient input state (pressed/released flags)
$extern fn_((dage_Runtime_endFrame(dage_Runtime* self))(void));
/// @brief Request quit
$attr($inline_always)
$static fn_((dage_Runtime_requestQuit(dage_Runtime* self))(void));
/// @brief Check if quit was requested
$attr($inline_always)
$static fn_((dage_Runtime_shouldQuit(const dage_Runtime* self))(bool));

/*========== Convenience: Single-Window Helpers ==========*/

/// @brief Get the first (primary) window
/// @note  Convenience for single-window applications
$attr($inline_always)
$static fn_((dage_Runtime_getPrimaryWindow(dage_Runtime* self))(O$P$dage_Window));

/*========== Macros and Definitions =========================================*/

fn_((dage_Runtime_getWindowCount(const dage_Runtime* self))(usize)) {
    claim_assert_nonnull(self);
    return self->windows.items.len;
};

fn_((dage_Runtime_getWindows(dage_Runtime* self))(S$P$dage_Window)) {
    claim_assert_nonnull(self);
    return self->windows.items;
};

fn_((dage_Runtime_requestQuit(dage_Runtime* self))(void)) {
    claim_assert_nonnull(self);
    self->state.should_quit = true;
};

fn_((dage_Runtime_shouldQuit(const dage_Runtime* self))(bool)) {
    claim_assert_nonnull(self);
    return self->state.should_quit;
};

fn_((dage_Runtime_getPrimaryWindow(dage_Runtime* self))(O$P$dage_Window) $scope) {
    claim_assert_nonnull(self);
    if (self->windows.items.len == 0) {
        return_none();
    }
    return_some(*S_at((self->windows.items)[0]));
} $unscoped_(fn);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_Runtime__included */
