/**
 * @file    cfg.h
 * @ingroup dage/core/WSI
 * @prefix  dage_core_WSI
 *
 * @brief   Configuration settings for WSI backend
 * @details Defines compile-time options and platform-specific types
 *          for Window System Integration.
 */
#ifndef dage_core_WSI_cfg__included
#define dage_core_WSI_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/core.h>
#include <dh/prl.h>

/*========== Platform Configuration =========================================*/

#define dage_core_WSI__use_x11 \
    /** \
     * @brief Determine which WSI backend to use per platform \
     */ \
    __comp_bool__dage_core_WSI__use_x11
#define __comp_bool__dage_core_WSI__use_x11 pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(pp_false)), \
        pp_case_((plat_type_linux)(pp_true)), \
        pp_case_((plat_type_darwin)(pp_false)), \
        pp_default_(pp_false) \
    ) pp_end \
)

/*========== Platform Includes ==============================================*/

#if plat_is_windows
#include <dh-ext/os/windows/wnd.h>
#endif /* plat_is_windows */
#if dage_core_WSI__use_x11
#include <dh-ext/os/linux/x11.h>
#endif /* dage_core_WSI__use_x11 */

/*========== Constants ======================================================*/

#define dage_core_WSI_max_targets ((usize)8)
#define dage_core_WSI_max_pending_events ((usize)64)

/*========== Platform-Specific Types ========================================*/

/// @brief Platform-specific window handle type
typedef pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(HWND)),
    pp_case_((plat_type_linux)(u64)), /* X11 Window (XID) */
    pp_case_((plat_type_darwin)(P$raw)), /* NSWindow* */
    pp_default_(P$raw)
)) dage_core_WSI_Handle;
T_use_O$(dage_core_WSI_Handle);

/// @brief Platform-specific device context type
typedef pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(HDC)),
    pp_case_((plat_type_linux)(u64)), /* GC (XID) */
    pp_case_((plat_type_darwin)(P$raw)),
    pp_default_(P$raw)
)) dage_core_WSI_Device;

/// @brief Platform-specific image/bitmap structure for pixel blitting
typedef struct dage_core_WSI_Image dage_core_WSI_Image;
struct dage_core_WSI_Image pp_switch_((plat_type)(
    pp_case_((plat_type_windows)({
        var_(bmi, BITMAPINFO);
    })),
    pp_case_((plat_type_linux)({
        var_(ximage, P$raw); /* XImage* */
        var_(pixels, S$u32); /* Pixel buffer (owned, allocated via gpa) */
    })),
    pp_default_({
        var_(unused_, Void);
    })
));

/// @brief Platform-specific display/connection type
typedef pp_switch_((plat_type)(
    pp_case_((plat_type_windows)(HINSTANCE)),
    pp_case_((plat_type_linux)(P$raw)), /* Display* */
    pp_case_((plat_type_darwin)(P$raw)),
    pp_default_(P$raw)
)) dage_core_WSI_Display;
T_use_O$(dage_core_WSI_Display);

/// @brief Platform-specific implementation type
typedef struct dage_core_WSI_Impl dage_core_WSI_Impl;
struct dage_core_WSI_Impl pp_switch_((plat_type)(
    pp_case_((plat_type_windows)({
        var_(window_class_registered, bool);
    })),
    pp_case_((plat_type_linux)({
        var_(screen, i32);
        var_(wm_delete_window, u64); /* Atom */
    })),
    pp_default_({ var_(unused_, Void); })
));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_core_WSI_cfg__included */
