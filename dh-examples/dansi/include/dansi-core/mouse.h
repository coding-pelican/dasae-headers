/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    mouse.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-06-17 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_mouse
 *
 * @brief   Mouse input domain types and xterm tracking mode controls.
 */
#pragma once
#ifndef dansi_mouse__included
#define dansi_mouse__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_mouse_Btn $fits($packed))(
    dansi_mouse_Btn_left,
    dansi_mouse_Btn_middle,
    dansi_mouse_Btn_right,
    dansi_mouse_Btn_backward,
    dansi_mouse_Btn_forward,
    dansi_mouse_Btn_aux1,
    dansi_mouse_Btn_aux2
)) dansi_mouse_Btn;
T_use_prl$(dansi_mouse_Btn);

typedef union dansi_mouse_Btns {
    struct {
        u8 left      : 1;
        u8 middle    : 1;
        u8 right     : 1;
        u8 backward  : 1;
        u8 forward   : 1;
        u8 aux1      : 1;
        u8 aux2      : 1;
        u8 reserved_ : 1;
    };
    u8 packed;
} dansi_mouse_Btns;
T_use_prl$(dansi_mouse_Btns);

typedef enum_((dansi_mouse_Wheel $fits($packed))(
    dansi_mouse_Wheel_up,
    dansi_mouse_Wheel_down,
    dansi_mouse_Wheel_left,
    dansi_mouse_Wheel_right
)) dansi_mouse_Wheel;
T_use_prl$(dansi_mouse_Wheel);

typedef enum_((dansi_mouse_TrackingMode $fits($packed))(
    dansi_mouse_TrackingMode_x10 = 9,
    dansi_mouse_TrackingMode_normal = 1000,
    dansi_mouse_TrackingMode_hilite = 1001,
    dansi_mouse_TrackingMode_button_event = 1002,
    dansi_mouse_TrackingMode_any_event = 1003,
    dansi_mouse_TrackingMode_utf8 = 1005,
    dansi_mouse_TrackingMode_sgr = 1006,
    dansi_mouse_TrackingMode_urxvt = 1015,
    dansi_mouse_TrackingMode_sgr_pixels = 1016
)) dansi_mouse_TrackingMode;
#define dansi_mouse_TrackingMode_staticParse(_mode_tok) \
    ____dansi_mouse_TrackingMode_staticParse(_mode_tok)

typedef A$$(4 + uint_log10Ceil_static(u16_limit_max), u8) dansi_mouse_TrackingModeBuf;

#define dansi_mouse_enableTracking_static(_mode_tok) \
    ____dansi_mouse_enableTracking_static(_mode_tok)
$extern fn_((dansi_mouse_enableTracking(dansi_mouse_TrackingMode mode, dansi_mouse_TrackingModeBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mouse_enableTrackingWrite(dansi_mouse_TrackingMode mode, io_Writer writer))(E$void));

#define dansi_mouse_disableTracking_static(_mode_tok) \
    ____dansi_mouse_disableTracking_static(_mode_tok)
$extern fn_((dansi_mouse_disableTracking(dansi_mouse_TrackingMode mode, dansi_mouse_TrackingModeBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mouse_disableTrackingWrite(dansi_mouse_TrackingMode mode, io_Writer writer))(E$void));

#define dansi_mouse_setTracking_static(_mode_tok, _pp_enabled) \
    ____dansi_mouse_setTracking_static(_mode_tok, _pp_enabled)
$extern fn_((dansi_mouse_setTracking(dansi_mouse_TrackingMode mode, bool enabled, dansi_mouse_TrackingModeBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mouse_setTrackingWrite(dansi_mouse_TrackingMode mode, bool enabled, io_Writer writer))(E$void));

#define dansi_mouse_enableAny_static() \
    ____dansi_mouse_enableAny_static()
$extern fn_((dansi_mouse_enableAny(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_mouse_enableAnyWrite(io_Writer writer))(E$void));
#define dansi_mouse_disableAny_static() \
    ____dansi_mouse_disableAny_static()
$extern fn_((dansi_mouse_disableAny(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_mouse_disableAnyWrite(io_Writer writer))(E$void));
#define dansi_mouse_enableSGR_static() \
    ____dansi_mouse_enableSGR_static()
$extern fn_((dansi_mouse_enableSGR(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_mouse_enableSGRWrite(io_Writer writer))(E$void));
#define dansi_mouse_disableSGR_static() \
    ____dansi_mouse_disableSGR_static()
$extern fn_((dansi_mouse_disableSGR(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_mouse_disableSGRWrite(io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "mode.h"
#include "utils.h"

$attr($inline_always)
$static fn_((dansi_mouse_btnsNone(void))(dansi_mouse_Btns)) {
    return (dansi_mouse_Btns){};
};

$attr($inline_always)
$static fn_((dansi_mouse_btnsFromBtn(dansi_mouse_Btn btn))(dansi_mouse_Btns)) {
    switch (btn) {
    case dansi_mouse_Btn_left: return (dansi_mouse_Btns){ .left = true };
    case dansi_mouse_Btn_middle: return (dansi_mouse_Btns){ .middle = true };
    case dansi_mouse_Btn_right: return (dansi_mouse_Btns){ .right = true };
    case dansi_mouse_Btn_backward: return (dansi_mouse_Btns){ .backward = true };
    case dansi_mouse_Btn_forward: return (dansi_mouse_Btns){ .forward = true };
    case dansi_mouse_Btn_aux1: return (dansi_mouse_Btns){ .aux1 = true };
    case dansi_mouse_Btn_aux2: return (dansi_mouse_Btns){ .aux2 = true };
    };
    claim_unreachable;
};

#define ____dansi_mouse_TrackingMode_staticParse(_mode_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_mode_tok))( \
        pp_Tok_then_(#_mode_tok), \
        pp_Tok_else_(pp_join($, ____dansi_mouse__pp_TrackingMode_str, _mode_tok)) \
    )

#define ____dansi_mouse_enableTracking_static(_mode_tok) \
    dansi_mode_enablePrivate_static(_mode_tok)
#define ____dansi_mouse_disableTracking_static(_mode_tok) \
    dansi_mode_disablePrivate_static(_mode_tok)
#define ____dansi_mouse_setTracking_static(_mode_tok, _pp_enabled) \
    dansi_mode_setPrivate_static(_mode_tok, _pp_enabled)

#define ____dansi_mouse_enableAny_static() \
    dansi_mouse_enableTracking_static(dansi_mouse_TrackingMode_staticParse(dansi_mouse_TrackingMode_any_event))
#define ____dansi_mouse_disableAny_static() \
    dansi_mouse_disableTracking_static(dansi_mouse_TrackingMode_staticParse(dansi_mouse_TrackingMode_any_event))
#define ____dansi_mouse_enableSGR_static() \
    dansi_mouse_enableTracking_static(dansi_mouse_TrackingMode_staticParse(dansi_mouse_TrackingMode_sgr))
#define ____dansi_mouse_disableSGR_static() \
    dansi_mouse_disableTracking_static(dansi_mouse_TrackingMode_staticParse(dansi_mouse_TrackingMode_sgr))

#pragma region "pp mouse mode str"
#define ____dansi_mouse__pp_TrackingMode_str$dansi_mouse_TrackingMode_x10 dansi_utils_mouse_mode_x10
#define ____dansi_mouse__pp_TrackingMode_str$dansi_mouse_TrackingMode_normal dansi_utils_mouse_mode_normal
#define ____dansi_mouse__pp_TrackingMode_str$dansi_mouse_TrackingMode_hilite dansi_utils_mouse_mode_hilite
#define ____dansi_mouse__pp_TrackingMode_str$dansi_mouse_TrackingMode_button_event dansi_utils_mouse_mode_button_event
#define ____dansi_mouse__pp_TrackingMode_str$dansi_mouse_TrackingMode_any_event dansi_utils_mouse_mode_any_event
#define ____dansi_mouse__pp_TrackingMode_str$dansi_mouse_TrackingMode_utf8 dansi_utils_mouse_mode_utf8
#define ____dansi_mouse__pp_TrackingMode_str$dansi_mouse_TrackingMode_sgr dansi_utils_mouse_mode_sgr
#define ____dansi_mouse__pp_TrackingMode_str$dansi_mouse_TrackingMode_urxvt dansi_utils_mouse_mode_urxvt
#define ____dansi_mouse__pp_TrackingMode_str$dansi_mouse_TrackingMode_sgr_pixels dansi_utils_mouse_mode_sgr_pixels
#pragma endregion "pp mouse mode str"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_mouse__included */
