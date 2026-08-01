/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    mode.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_mode
 */
#pragma once
#ifndef dansi_mode__included
#define dansi_mode__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "csi.h"

/*========== Macros and Declarations ========================================*/

#define dansi_mode_enable_final __str__dansi_mode_enable_final
#define dansi_mode_enable_final_byte __uint__dansi_mode_enable_final_byte
#define dansi_mode_disable_final __str__dansi_mode_disable_final
#define dansi_mode_disable_final_byte __uint__dansi_mode_disable_final_byte

typedef enum_((dansi_mode_Code $fits($packed))(
    dansi_mode_Code_insert = 4,
    dansi_mode_Code_line_feed_new_line = 20
)) dansi_mode_Code;
claim_assert_static(eqlType$(dansi_mode_Code, u8));
T_use_prl$(dansi_mode_Code);

#define dansi_mode_setRaw_static(_mode_tok, _pp_enabled) \
    ____dansi_mode_setRaw_static(_mode_tok, _pp_enabled)
typedef A$$(4 + uint_log10Ceil_static(u16_limit_max), u8) dansi_mode_SetRawBuf;
$attr($must_check)
$extern fn_((dansi_mode_setRaw(u16 mode, bool enabled, dansi_mode_SetRawBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_setRawWrite(u16 mode, bool enabled, io_Writer out))(io_PrintE$void));

#define dansi_mode_enableRaw_static(_mode_tok) \
    ____dansi_mode_enableRaw_static(_mode_tok)
typedef A$$(4 + uint_log10Ceil_static(u16_limit_max), u8) dansi_mode_EnableRawBuf;
$attr($must_check)
$extern fn_((dansi_mode_enableRaw(u16 mode, dansi_mode_EnableRawBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_enableRawWrite(u16 mode, io_Writer out))(io_PrintE$void));

#define dansi_mode_disableRaw_static(_mode_tok) \
    ____dansi_mode_disableRaw_static(_mode_tok)
typedef A$$(4 + uint_log10Ceil_static(u16_limit_max), u8) dansi_mode_DisableRawBuf;
$attr($must_check)
$extern fn_((dansi_mode_disableRaw(u16 mode, dansi_mode_DisableRawBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_disableRawWrite(u16 mode, io_Writer out))(io_PrintE$void));

$attr($must_check)
$extern fn_((dansi_mode_setMany(S_const$u16 modes, bool enabled, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_mode_setManyWrite(S_const$u16 modes, bool enabled, io_Writer out))(io_PrintE$void));

#define dansi_mode_Code_staticParse(_mode_tok) \
    ____dansi_mode_Code_staticParse(_mode_tok)
#define dansi_mode_set_static(_mode_tok, _pp_enabled) \
    dansi_mode_setRaw_static(dansi_mode_Code_staticParse(_mode_tok), _pp_enabled)
typedef dansi_mode_SetRawBuf dansi_mode_SetBuf;
$attr($must_check)
$extern fn_((dansi_mode_set(dansi_mode_Code mode, bool enabled, dansi_mode_SetBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_setWrite(dansi_mode_Code mode, bool enabled, io_Writer out))(io_PrintE$void));

#define dansi_mode_enable_static(_mode_tok) \
    dansi_mode_enableRaw_static(dansi_mode_Code_staticParse(_mode_tok))
typedef dansi_mode_EnableRawBuf dansi_mode_EnableBuf;
$attr($must_check)
$extern fn_((dansi_mode_enable(dansi_mode_Code mode, dansi_mode_EnableBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_enableWrite(dansi_mode_Code mode, io_Writer out))(io_PrintE$void));

#define dansi_mode_disable_static(_mode_tok) \
    dansi_mode_disableRaw_static(dansi_mode_Code_staticParse(_mode_tok))
typedef dansi_mode_DisableRawBuf dansi_mode_DisableBuf;
$attr($must_check)
$extern fn_((dansi_mode_disable(dansi_mode_Code mode, dansi_mode_DisableBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_mode_disableWrite(dansi_mode_Code mode, io_Writer out))(io_PrintE$void));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_mode_enable_final "h"
#define __uint__dansi_mode_enable_final_byte u8_c('h')
#define __str__dansi_mode_disable_final "l"
#define __uint__dansi_mode_disable_final_byte u8_c('l')
#define ____dansi_mode_enableRaw_static(_mode_tok) \
    dansi_csi_make1_static(_mode_tok, dansi_mode_enable_final)
#define ____dansi_mode_disableRaw_static(_mode_tok) \
    dansi_csi_make1_static(_mode_tok, dansi_mode_disable_final)
#define ____dansi_mode_setRaw_static(_mode_tok, _pp_enabled) \
    pp_if_(_pp_enabled)( \
        pp_then_(dansi_mode_enableRaw_static(_mode_tok)), \
        pp_else_(dansi_mode_disableRaw_static(_mode_tok)) \
    )
#define ____dansi_mode_Code_staticParse(_mode_tok) \
    pp_join($, ____dansi_mode_Code_str, _mode_tok)
#define ____dansi_mode_Code_str$dansi_mode_Code_insert "4"
#define ____dansi_mode_Code_str$dansi_mode_Code_line_feed_new_line "20"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_mode__included */
