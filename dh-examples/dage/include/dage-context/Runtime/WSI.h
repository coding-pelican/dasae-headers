/**
 * @file    WSI.h
 * @brief   Window System Integration backend for dage
 * @details Native window backend supporting Win32 (Windows) and X11 (Linux).
 *          Implements the dage_Backend interface.
 *
 * ## Usage
 * ```c
 * // 1. Create WSI instance
 * var wsi = try_(dage_Runtime_WSI_init((dage_Runtime_WSI_Cfg){
 *     .gpa = gpa,
 * }));
 * defer_(dage_Runtime_WSI_fini(&wsi));
 *
 * // 2. Get backend interface
 * let backend = dage_Runtime_WSI_backend(wsi);
 *
 * // 3. Inject into Runtime
 * var runtime = try_(dage_Runtime_init((dage_Runtime_Cfg){
 *     .gpa = gpa,
 *     .backend = backend,
 * }));
 * defer_(dage_Runtime_fini(&runtime));
 * // ... use wsi, backend, runtime ...
 * ```
 *
 * ## Platform Support
 * - Windows: Win32 API (HWND, WndProc, DIB)
 * - Linux: X11 (XOpenDisplay, XCreateWindow, XImage)
 * - macOS: Not yet supported (planned: Cocoa)
 */
#ifndef dage_Runtime_WSI__included
#define dage_Runtime_WSI__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "WSI/cfg.h"
#include "dage-runtime.h"

/*========== Target Structure ===============================================*/

/// @brief Per-target state
typedef struct dage_Runtime_WSI_Target {
    var_(id, dage_TargetId);
    var_(active, bool);

    var_(metrics, dage_TargetMetrics);

    var_(handle, dage_Runtime_WSI_Handle);
    var_(device, dage_Runtime_WSI_Device);
    var_(image, dage_Runtime_WSI_Image);

    var_(pending_events, A$$(dage_Runtime_WSI_max_pending_events, dage_Event));
    var_(pending_event_count, usize);
} dage_Runtime_WSI_Target;
T_use_prl$(dage_Runtime_WSI_Target);

/*========== WSI Instance ===================================================*/

/// @brief WSI backend instance
typedef struct dage_Runtime_WSI {
    var_(gpa, mem_Alctr);

    /* Target management */
    var_(targets, A$$(dage_Runtime_WSI_max_targets, dage_Runtime_WSI_Target));
    var_(target_count, dage_TargetId); /* using as next target id counter */

    /* Platform-specific state */
    var_(display, dage_Runtime_WSI_Display);
    var_(impl, dage_Runtime_WSI_Impl);

    var_(initialized, bool);
} dage_Runtime_WSI;
T_use_P$(dage_Runtime_WSI);
T_use_E$(P$dage_Runtime_WSI);

/*========== Configuration ==================================================*/

/// @brief WSI initialization configuration
typedef struct dage_Runtime_WSI_Cfg {
    var_(gpa, mem_Alctr);
} dage_Runtime_WSI_Cfg;

/*========== Lifecycle ======================================================*/

/// @brief Initialize WSI backend
/// @param cfg Configuration
/// @return WSI instance on success, error on failure
/// @note Error boundary - platform initialization may fail
$attr($must_check)
$extern fn_((dage_Runtime_WSI_init(dage_Runtime_WSI_Cfg cfg))(E$P$dage_Runtime_WSI));
/// @brief Shutdown WSI backend
/// @param self WSI instance
/// @note Destroys all remaining targets
$extern fn_((dage_Runtime_WSI_fini(dage_Runtime_WSI** self))(void));

/*========== Backend Interface ==============================================*/

/// @brief Get backend interface from WSI instance
/// @param self WSI instance
/// @return Backend fat pointer
$extern fn_((dage_Runtime_WSI_backend(dage_Runtime_WSI* self))(dage_Backend));

/*========== Native Handle Queries (Unified API) ============================*/

/// @brief Get native window handle for a target
/// @param self  WSI instance
/// @param id    Target ID
/// @return      Platform-specific handle:
///              - Windows: HWND
///              - X11: Window (XID)
///              - Returns target handle or panic
///
/// @note This is the "escape hatch" for platform-specific operations.
///       The return type automatically matches the platform at compile time.
///
/// @example
/// ```c
/// let handle = dage_Runtime_WSI_getNativeHandle(wsi, target_id);
/// #if plat_is_windows
///     SetWindowTextA(handle, "New Title");
/// #elif dage_Runtime_WSI__use_x11
///     XStoreName(display, handle, "New Title");
/// #endif
/// ```
$extern fn_((dage_Runtime_WSI_getNativeHandle(dage_Runtime_WSI* self, dage_TargetId id))(dage_Runtime_WSI_Handle));
/// @brief Get native display/instance connection
/// @param self  WSI instance
/// @return      Platform-specific display:
///              - Windows: HINSTANCE
///              - X11: Display*
///              - Returns display
$extern fn_((dage_Runtime_WSI_getNativeDisplay(dage_Runtime_WSI* self))(dage_Runtime_WSI_Display));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_Runtime_WSI__included */
