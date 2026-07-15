/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    c0.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_c0
 */
#pragma once
#ifndef dansi_c0__included
#define dansi_c0__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/ascii.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_c0_Code $fits($packed))(
    dansi_c0_Code_nul = ascii_ctrl_Code_nul,
    dansi_c0_Code_soh = ascii_ctrl_Code_soh,
    dansi_c0_Code_stx = ascii_ctrl_Code_stx,
    dansi_c0_Code_etx = ascii_ctrl_Code_etx,
    dansi_c0_Code_eot = ascii_ctrl_Code_eot,
    dansi_c0_Code_enq = ascii_ctrl_Code_enq,
    dansi_c0_Code_ack = ascii_ctrl_Code_ack,
    dansi_c0_Code_bel = ascii_ctrl_Code_bel,
    dansi_c0_Code_bs = ascii_ctrl_Code_bs,
    dansi_c0_Code_ht = ascii_ctrl_Code_ht,
    dansi_c0_Code_lf = ascii_ctrl_Code_lf,
    dansi_c0_Code_vt = ascii_ctrl_Code_vt,
    dansi_c0_Code_ff = ascii_ctrl_Code_ff,
    dansi_c0_Code_cr = ascii_ctrl_Code_cr,
    dansi_c0_Code_so = ascii_ctrl_Code_so,
    dansi_c0_Code_si = ascii_ctrl_Code_si,
    dansi_c0_Code_dle = ascii_ctrl_Code_dle,
    dansi_c0_Code_dc1 = ascii_ctrl_Code_dc1,
    dansi_c0_Code_dc2 = ascii_ctrl_Code_dc2,
    dansi_c0_Code_dc3 = ascii_ctrl_Code_dc3,
    dansi_c0_Code_dc4 = ascii_ctrl_Code_dc4,
    dansi_c0_Code_nak = ascii_ctrl_Code_nak,
    dansi_c0_Code_syn = ascii_ctrl_Code_syn,
    dansi_c0_Code_etb = ascii_ctrl_Code_etb,
    dansi_c0_Code_can = ascii_ctrl_Code_can,
    dansi_c0_Code_em = ascii_ctrl_Code_em,
    dansi_c0_Code_sub = ascii_ctrl_Code_sub,
    dansi_c0_Code_esc = ascii_ctrl_Code_esc,
    dansi_c0_Code_fs = ascii_ctrl_Code_fs,
    dansi_c0_Code_gs = ascii_ctrl_Code_gs,
    dansi_c0_Code_rs = ascii_ctrl_Code_rs,
    dansi_c0_Code_us = ascii_ctrl_Code_us
)) dansi_c0_Code;
claim_assert_static(TypeInfoPacked_eql(
    packTypeInfo$(dansi_c0_Code),
    packTypeInfo$(ascii_ctrl_Code)
));
T_use_prl$(dansi_c0_Code);

$attr($inline_always)
$static fn_((dansi_c0_isCtrl(u8 byte))(bool));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((dansi_c0_isCtrl(u8 byte))(bool)) { return byte <= dansi_c0_Code_us; };
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_c0__included */
