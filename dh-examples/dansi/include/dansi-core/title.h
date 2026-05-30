/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    title.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-05-23 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_title
 */
#ifndef dansi_title__included
#define dansi_title__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_title_StackTarget $fits($packed))(
    dansi_title_StackTarget_icon = 1,
    dansi_title_StackTarget_window = 2,
    dansi_title_StackTarget_both = 3,
)) dansi_title_StackTarget;
#define dansi_title_StackTarget_staticParse(_target_tok) \
    ____dansi_title_StackTarget_staticParse(_target_tok)

#define dansi_title_setIcon_static(_title_tok) \
    ____dansi_title_setIcon_static(_title_tok)
$extern fn_((dansi_title_setIcon(S_const$u8 title, S$u8 buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_title_setIconWrite(S_const$u8 title, io_Writer writer))(E$void));

#define dansi_title_setWindow_static(_title_tok) \
    ____dansi_title_setWindow_static(_title_tok)
$extern fn_((dansi_title_setWindow(S_const$u8 title, S$u8 buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_title_setWindowWrite(S_const$u8 title, io_Writer writer))(E$void));

#define dansi_title_setBoth_static(_title_tok) \
    ____dansi_title_setBoth_static(_title_tok)
$extern fn_((dansi_title_setBoth(S_const$u8 title, S$u8 buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_title_setBothWrite(S_const$u8 title, io_Writer writer))(E$void));

typedef A$$(7 + uint_log10Ceil_static(u16_limit_max), u8) dansi_title_StackBuf;

#define dansi_title_push_static(_target_tok) \
    ____dansi_title_push_static(_target_tok)
$extern fn_((dansi_title_push(dansi_title_StackTarget target, dansi_title_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_title_pushWrite(dansi_title_StackTarget target, io_Writer writer))(E$void));

#define dansi_title_pop_static(_target_tok) \
    ____dansi_title_pop_static(_target_tok)
$extern fn_((dansi_title_pop(dansi_title_StackTarget target, dansi_title_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_title_popWrite(dansi_title_StackTarget target, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

#define ____dansi_title_StackTarget_staticParse(_target_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_target_tok))( \
        pp_Tok_then_(#_target_tok), \
        pp_Tok_else_(pp_join($, ____dansi_title__pp_StackTarget_str, _target_tok)) \
    )
#define ____dansi_title_setIcon_static(_title_tok) \
    dansi_utils_formatOSC_static(dansi_utils_osc_title_icon, _title_tok)
#define ____dansi_title_setWindow_static(_title_tok) \
    dansi_utils_formatOSC_static(dansi_utils_osc_title_window, _title_tok)
#define ____dansi_title_setBoth_static(_title_tok) \
    dansi_utils_formatOSC_static(dansi_utils_osc_title_both, _title_tok)
#define ____dansi_title_push_static(_target_tok) \
    dansi_utils_csi dansi_utils_title_stack_push dansi_utils_sep _target_tok dansi_utils_screen_response_size
#define ____dansi_title_pop_static(_target_tok) \
    dansi_utils_csi dansi_utils_title_stack_pop dansi_utils_sep _target_tok dansi_utils_screen_response_size

#pragma region "pp title stack target str"
#define ____dansi_title__pp_StackTarget_str$dansi_title_StackTarget_icon "1"
#define ____dansi_title__pp_StackTarget_str$dansi_title_StackTarget_window "2"
#define ____dansi_title__pp_StackTarget_str$dansi_title_StackTarget_both "3"
#pragma endregion "pp title stack target str"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_title__included */
