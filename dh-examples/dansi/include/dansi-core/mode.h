/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    mode.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-05-23 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_mode
 */
#ifndef dansi_mode__included
#define dansi_mode__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_mode_Private $fits($packed))(
    dansi_mode_Private_cursor_keys = 1,
    dansi_mode_Private_origin = 6,
    dansi_mode_Private_auto_wrap = 7,
    dansi_mode_Private_alt_screen = 1049,
    dansi_mode_Private_bracketed_paste = 2004,
    dansi_mode_Private_synchronized_output = 2026,
)) dansi_mode_Private;
#define dansi_mode_Private_staticParse(_mode_tok) \
    ____dansi_mode_Private_staticParse(_mode_tok)

typedef A$$(4 + uint_log10Ceil_static(u16_limit_max), u8) dansi_mode_PrivateBuf;

#define dansi_mode_enablePrivate_static(_mode_tok) \
    ____dansi_mode_enablePrivate_static(_mode_tok)
$extern fn_((dansi_mode_enablePrivate(u16 mode, dansi_mode_PrivateBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_enablePrivateWrite(u16 mode, io_Writer writer))(E$void));

#define dansi_mode_disablePrivate_static(_mode_tok) \
    ____dansi_mode_disablePrivate_static(_mode_tok)
$extern fn_((dansi_mode_disablePrivate(u16 mode, dansi_mode_PrivateBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_disablePrivateWrite(u16 mode, io_Writer writer))(E$void));

#define dansi_mode_setPrivate_static(_mode_tok, _pp_enabled) \
    ____dansi_mode_setPrivate_static(_mode_tok, _pp_enabled)
$extern fn_((dansi_mode_setPrivate(u16 mode, bool enabled, dansi_mode_PrivateBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_setPrivateWrite(u16 mode, bool enabled, io_Writer writer))(E$void));

typedef enum_((dansi_mode_ANSI $fits($packed))(
    dansi_mode_ANSI_insert = 4,
    dansi_mode_ANSI_line_feed_new_line = 20,
)) dansi_mode_ANSI;
#define dansi_mode_ANSI_staticParse(_mode_tok) \
    ____dansi_mode_ANSI_staticParse(_mode_tok)

typedef A$$(3 + uint_log10Ceil_static(u16_limit_max), u8) dansi_mode_ANSIBuf;

#define dansi_mode_enableANSI_static(_mode_tok) \
    ____dansi_mode_enableANSI_static(_mode_tok)
$extern fn_((dansi_mode_enableANSI(u16 mode, dansi_mode_ANSIBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_enableANSIWrite(u16 mode, io_Writer writer))(E$void));

#define dansi_mode_disableANSI_static(_mode_tok) \
    ____dansi_mode_disableANSI_static(_mode_tok)
$extern fn_((dansi_mode_disableANSI(u16 mode, dansi_mode_ANSIBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_disableANSIWrite(u16 mode, io_Writer writer))(E$void));

#define dansi_mode_setANSI_static(_mode_tok, _pp_enabled) \
    ____dansi_mode_setANSI_static(_mode_tok, _pp_enabled)
$extern fn_((dansi_mode_setANSI(u16 mode, bool enabled, dansi_mode_ANSIBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_setANSIWrite(u16 mode, bool enabled, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

#define ____dansi_mode_Private_staticParse(_mode_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_mode_tok))( \
        pp_Tok_then_(#_mode_tok), \
        pp_Tok_else_(pp_join($, ____dansi_mode__pp_Private_str, _mode_tok)) \
    )
#define ____dansi_mode_enablePrivate_static(_mode_tok) \
    dansi_utils_csi dansi_utils_mode_private_prefix _mode_tok dansi_utils_mode_set
#define ____dansi_mode_disablePrivate_static(_mode_tok) \
    dansi_utils_csi dansi_utils_mode_private_prefix _mode_tok dansi_utils_mode_reset
#define ____dansi_mode_setPrivate_static(_mode_tok, _pp_enabled) \
    pp_if_(_pp_enabled)( \
        pp_then_(dansi_mode_enablePrivate_static(_mode_tok)), \
        pp_else_(dansi_mode_disablePrivate_static(_mode_tok)) \
    )
#define ____dansi_mode_ANSI_staticParse(_mode_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_mode_tok))( \
        pp_Tok_then_(#_mode_tok), \
        pp_Tok_else_(pp_join($, ____dansi_mode__pp_ANSI_str, _mode_tok)) \
    )
#define ____dansi_mode_enableANSI_static(_mode_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_mode_set, _mode_tok)
#define ____dansi_mode_disableANSI_static(_mode_tok) \
    dansi_utils_formatCSI_u16_static(dansi_utils_mode_reset, _mode_tok)
#define ____dansi_mode_setANSI_static(_mode_tok, _pp_enabled) \
    pp_if_(_pp_enabled)( \
        pp_then_(dansi_mode_enableANSI_static(_mode_tok)), \
        pp_else_(dansi_mode_disableANSI_static(_mode_tok)) \
    )

#pragma region "pp private mode str"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_cursor_keys "1"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_origin "6"
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_auto_wrap "7"
#define pp_Tok_cmp$1049(_x) _x
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_alt_screen "1049"
#define pp_Tok_cmp$2004(_x) _x
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_bracketed_paste "2004"
#define pp_Tok_cmp$2026(_x) _x
#define ____dansi_mode__pp_Private_str$dansi_mode_Private_synchronized_output "2026"
#pragma endregion "pp private mode str"

#pragma region "pp ansi mode str"
#define ____dansi_mode__pp_ANSI_str$dansi_mode_ANSI_insert "4"
#define ____dansi_mode__pp_ANSI_str$dansi_mode_ANSI_line_feed_new_line "20"
#pragma endregion "pp ansi mode str"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_mode__included */
