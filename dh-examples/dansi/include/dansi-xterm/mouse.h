/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    mouse.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-05-23 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_mouse
 *
 * @brief   xterm mouse tracking controls and event parser.
 */
#ifndef dansi_xterm_mouse__included
#define dansi_xterm_mouse__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/Event.h"
#include "dansi-core/mode.h"
#include "utils.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_mouse_Mode $fits($packed))(
    dansi_mouse_Mode_x10 = 9,
    dansi_mouse_Mode_normal = 1000,
    dansi_mouse_Mode_button_event = 1002,
    dansi_mouse_Mode_any_event = 1003,
    dansi_mouse_Mode_sgr = 1006,
)) dansi_mouse_Mode;
#define dansi_mouse_Mode_staticParse(_mode_tok) \
    ____dansi_mouse_Mode_staticParse(_mode_tok)

typedef A$$(4 + uint_log10Ceil_static(u16_limit_max), u8) dansi_mouse_ModeBuf;

typedef enum_((dansi_mouse_Button $fits($packed))(
    dansi_mouse_Button_none,
    dansi_mouse_Button_left,
    dansi_mouse_Button_middle,
    dansi_mouse_Button_right,
)) dansi_mouse_Button;

typedef enum_((dansi_mouse_Action $fits($packed))(
    dansi_mouse_Action_press,
    dansi_mouse_Action_release,
    dansi_mouse_Action_drag,
    dansi_mouse_Action_motion,
)) dansi_mouse_Action;

typedef enum_((dansi_mouse_Wheel $fits($packed))(
    dansi_mouse_Wheel_none,
    dansi_mouse_Wheel_up,
    dansi_mouse_Wheel_down,
)) dansi_mouse_Wheel;

typedef struct dansi_mouse_Event {
    var_(x, u16);
    var_(y, u16);
    var_(button, dansi_mouse_Button);
    var_(action, dansi_mouse_Action);
    var_(wheel, dansi_mouse_Wheel);
    var_(mods, dansi_Event_KeyMods);
} dansi_mouse_Event;
T_use_prl$(dansi_mouse_Event);

errset_((dansi_mouse_E)(
    dansi_mouse_NotSGRSeq,
    dansi_mouse_InvalidSGRSeq
));
T_use_E$($set(dansi_mouse_E)(dansi_mouse_Event));

#define dansi_mouse_enable_static(_mode_tok) \
    ____dansi_mouse_enable_static(_mode_tok)
$extern fn_((dansi_mouse_enable(dansi_mouse_Mode mode, dansi_mouse_ModeBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mouse_enableWrite(dansi_mouse_Mode mode, io_Writer writer))(E$void));

#define dansi_mouse_disable_static(_mode_tok) \
    ____dansi_mouse_disable_static(_mode_tok)
$extern fn_((dansi_mouse_disable(dansi_mouse_Mode mode, dansi_mouse_ModeBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mouse_disableWrite(dansi_mouse_Mode mode, io_Writer writer))(E$void));

#define dansi_mouse_set_static(_mode_tok, _pp_enabled) \
    ____dansi_mouse_set_static(_mode_tok, _pp_enabled)
$extern fn_((dansi_mouse_set(dansi_mouse_Mode mode, bool enabled, dansi_mouse_ModeBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mouse_setWrite(dansi_mouse_Mode mode, bool enabled, io_Writer writer))(E$void));

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

$attr($must_check)
$extern fn_((dansi_mouse_parseSGR(dansi_Seq seq))(dansi_mouse_E$dansi_mouse_Event));

/*========== Macros and Definitions =========================================*/

#define ____dansi_mouse_Mode_staticParse(_mode_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_mode_tok))( \
        pp_Tok_then_(#_mode_tok), \
        pp_Tok_else_(pp_join($, ____dansi_mouse__pp_Mode_str, _mode_tok)) \
    )

#define ____dansi_mouse_enable_static(_mode_tok) \
    dansi_mode_enablePrivate_static(_mode_tok)
#define ____dansi_mouse_disable_static(_mode_tok) \
    dansi_mode_disablePrivate_static(_mode_tok)
#define ____dansi_mouse_set_static(_mode_tok, _pp_enabled) \
    dansi_mode_setPrivate_static(_mode_tok, _pp_enabled)

#define ____dansi_mouse_enableAny_static() \
    dansi_mouse_enable_static(dansi_mouse_Mode_staticParse(dansi_mouse_Mode_any_event))
#define ____dansi_mouse_disableAny_static() \
    dansi_mouse_disable_static(dansi_mouse_Mode_staticParse(dansi_mouse_Mode_any_event))
#define ____dansi_mouse_enableSGR_static() \
    dansi_mouse_enable_static(dansi_mouse_Mode_staticParse(dansi_mouse_Mode_sgr))
#define ____dansi_mouse_disableSGR_static() \
    dansi_mouse_disable_static(dansi_mouse_Mode_staticParse(dansi_mouse_Mode_sgr))

#pragma region "pp xterm mouse mode str"
#define ____dansi_mouse__pp_Mode_str$dansi_mouse_Mode_x10 dansi_utils_mouse_mode_x10
#define ____dansi_mouse__pp_Mode_str$dansi_mouse_Mode_normal dansi_utils_mouse_mode_normal
#define ____dansi_mouse__pp_Mode_str$dansi_mouse_Mode_button_event dansi_utils_mouse_mode_button_event
#define ____dansi_mouse__pp_Mode_str$dansi_mouse_Mode_any_event dansi_utils_mouse_mode_any_event
#define ____dansi_mouse__pp_Mode_str$dansi_mouse_Mode_sgr dansi_utils_mouse_mode_sgr
#pragma endregion "pp xterm mouse mode str"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_mouse__included */
