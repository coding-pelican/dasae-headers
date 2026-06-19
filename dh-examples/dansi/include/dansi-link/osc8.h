/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    osc8.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_link_osc8
 */
#pragma once
#ifndef dansi_link_osc8__included
#define dansi_link_osc8__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../dansi-core/osc.h"

/*========== Macros and Declarations ========================================*/

typedef variant_((dansi_link_osc8_Params $fits($packed))(
    (dansi_link_osc8_Params_none, Void),
    (dansi_link_osc8_Params_id, S_const$u8),
    (dansi_link_osc8_Params_raw, S_const$u8)
)) dansi_link_osc8_Params;
claim_assert_static(eqlType$(FieldType$(dansi_link_osc8_Params, tag), u8));
T_use_prl$(dansi_link_osc8_Params);

$attr($must_check)
$extern fn_((dansi_link_osc8_open(S_const$u8 uri, dansi_link_osc8_Params params, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_link_osc8_openWrite(S_const$u8 uri, dansi_link_osc8_Params params, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_link_osc8_openPlain(S_const$u8 uri, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_link_osc8_openPlainWrite(S_const$u8 uri, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_link_osc8_openWithId(S_const$u8 uri, S_const$u8 id, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_link_osc8_openWithIdWrite(S_const$u8 uri, S_const$u8 id, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_link_osc8_openRaw(S_const$u8 uri, S_const$u8 params, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_link_osc8_openRawWrite(S_const$u8 uri, S_const$u8 params, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_link_osc8_close(S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_link_osc8_closeWrite(io_Writer out))(E$void));

typedef struct dansi_link_osc8_ParsedParams {
    var_(raw, S_const$u8);
    var_(id, O$S_const$u8);
} dansi_link_osc8_ParsedParams;
T_use_prl$(dansi_link_osc8_ParsedParams);

typedef struct dansi_link_osc8_Link {
    var_(params, dansi_link_osc8_ParsedParams);
    var_(uri, S_const$u8);
} dansi_link_osc8_Link;
T_use_prl$(dansi_link_osc8_Link);

typedef variant_((dansi_link_osc8_Frame $fits($packed))(
    (dansi_link_osc8_Frame_open, dansi_link_osc8_Link),
    (dansi_link_osc8_Frame_close, Void)
)) dansi_link_osc8_Frame;
claim_assert_static(eqlType$(FieldType$(dansi_link_osc8_Frame, tag), u8));
T_use_prl$(dansi_link_osc8_Frame);

errset_((dansi_link_osc8_E)(dansi_link_osc8_Invalid));
T_use_E$($set(dansi_link_osc8_E)(dansi_link_osc8_Frame));

$attr($must_check)
$extern fn_((dansi_link_osc8_parse(dansi_osc_Frame frame))(dansi_link_osc8_E$dansi_link_osc8_Frame));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_link_osc8__included */
