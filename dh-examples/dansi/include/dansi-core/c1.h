/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    c1.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_c1
 */
#pragma once
#ifndef dansi_c1__included
#define dansi_c1__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/prl.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_c1_Code $fits($packed))(
    dansi_c1_Code_pad = 0x80,
    dansi_c1_Code_hop = 0x81,
    dansi_c1_Code_bph = 0x82,
    dansi_c1_Code_nbh = 0x83,
    dansi_c1_Code_ind = 0x84,
    dansi_c1_Code_nel = 0x85,
    dansi_c1_Code_ssa = 0x86,
    dansi_c1_Code_esa = 0x87,
    dansi_c1_Code_hts = 0x88,
    dansi_c1_Code_htj = 0x89,
    dansi_c1_Code_vts = 0x8a,
    dansi_c1_Code_pld = 0x8b,
    dansi_c1_Code_plu = 0x8c,
    dansi_c1_Code_ri = 0x8d,
    dansi_c1_Code_ss2 = 0x8e,
    dansi_c1_Code_ss3 = 0x8f,
    dansi_c1_Code_dcs = 0x90,
    dansi_c1_Code_pu1 = 0x91,
    dansi_c1_Code_pu2 = 0x92,
    dansi_c1_Code_sts = 0x93,
    dansi_c1_Code_cch = 0x94,
    dansi_c1_Code_mw = 0x95,
    dansi_c1_Code_spa = 0x96,
    dansi_c1_Code_epa = 0x97,
    dansi_c1_Code_sos = 0x98,
    dansi_c1_Code_sgci = 0x99,
    dansi_c1_Code_sci = 0x9a,
    dansi_c1_Code_csi = 0x9b,
    dansi_c1_Code_st = 0x9c,
    dansi_c1_Code_osc = 0x9d,
    dansi_c1_Code_pm = 0x9e,
    dansi_c1_Code_apc = 0x9f
)) dansi_c1_Code;
claim_assert_static(eqlType$(dansi_c1_Code, u8));
T_use_prl$(dansi_c1_Code);

$attr($inline_always)
$static fn_((dansi_c1_isCtrl(u8 byte))(bool));

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((dansi_c1_isCtrl(u8 byte))(bool)) { return dansi_c1_Code_pad <= byte && byte <= dansi_c1_Code_apc; };
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_c1__included */
