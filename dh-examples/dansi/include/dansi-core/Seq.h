/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Seq.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_Seq
 */
#pragma once
#ifndef dansi_Seq__included
#define dansi_Seq__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/Buf.h>
#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_Seq_Kind $fits($packed))(
    dansi_Seq_Kind_text = 0,
    dansi_Seq_Kind_c0,
    dansi_Seq_Kind_esc,
    dansi_Seq_Kind_c1,
    dansi_Seq_Kind_ss2,
    dansi_Seq_Kind_ss3,
    dansi_Seq_Kind_dcs,
    dansi_Seq_Kind_sos,
    dansi_Seq_Kind_csi,
    dansi_Seq_Kind_osc,
    dansi_Seq_Kind_pm,
    dansi_Seq_Kind_apc
)) dansi_Seq_Kind;
claim_assert_static(eqlType$(dansi_Seq_Kind, u8));
T_use_prl$(dansi_Seq_Kind);

typedef enum_((dansi_Seq_EOS $fits($packed))(
    dansi_Seq_EOS_none = 0,
    dansi_Seq_EOS_bel,
    dansi_Seq_EOS_st_7bit,
    dansi_Seq_EOS_st_8bit
)) dansi_Seq_EOS;
claim_assert_static(eqlType$(dansi_Seq_EOS, u8));
T_use_prl$(dansi_Seq_EOS);

typedef struct dansi_Seq {
    var_(kind, dansi_Seq_Kind);
    var_(bytes, S_const$u8);
} dansi_Seq;
T_use_prl$(dansi_Seq);

errset_((dansi_Seq_E)(
    dansi_Seq_Incomplete,
    dansi_Seq_TooLong,
    dansi_Seq_Invalid
));
T_use_E$($set(dansi_Seq_E)(dansi_Seq));

$attr($inline_always)
$static fn_((dansi_Seq_from(dansi_Seq_Kind kind, S_const$u8 bytes))(dansi_Seq)) {
    return (dansi_Seq){ .kind = kind, .bytes = bytes };
};

$attr($must_check)
$extern fn_((dansi_Seq_extract(io_Buf_Reader* reader))(dansi_Seq_E$dansi_Seq));
$attr($must_check)
$extern fn_((dansi_Seq_receive(io_Reader in, S$u8 buf))(dansi_Seq_E$dansi_Seq));
$attr($must_check)
$extern fn_((dansi_Seq_receiveC0(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_Seq_receiveESC(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_Seq_receiveC1(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_Seq_receiveSS2(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_Seq_receiveSS3(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_Seq_receiveDCS(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_Seq_receiveSOS(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_Seq_receiveCSI(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_Seq_receiveOSC(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_Seq_receivePM(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_Seq_receiveAPC(io_Reader in, S$u8 buf))(E$S$u8));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_Seq__included */
