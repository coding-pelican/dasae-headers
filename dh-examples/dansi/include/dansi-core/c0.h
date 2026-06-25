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

#include <dh/prl.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_c0_Code $fits($packed))(
    dansi_c0_Code_nul = 0x00,
    dansi_c0_Code_soh = 0x01,
    dansi_c0_Code_stx = 0x02,
    dansi_c0_Code_etx = 0x03,
    dansi_c0_Code_eot = 0x04,
    dansi_c0_Code_enq = 0x05,
    dansi_c0_Code_ack = 0x06,
    dansi_c0_Code_bel = 0x07,
    dansi_c0_Code_bs = 0x08,
    dansi_c0_Code_ht = 0x09,
    dansi_c0_Code_lf = 0x0a,
    dansi_c0_Code_vt = 0x0b,
    dansi_c0_Code_ff = 0x0c,
    dansi_c0_Code_cr = 0x0d,
    dansi_c0_Code_so = 0x0e,
    dansi_c0_Code_si = 0x0f,
    dansi_c0_Code_dle = 0x10,
    dansi_c0_Code_dc1 = 0x11,
    dansi_c0_Code_dc2 = 0x12,
    dansi_c0_Code_dc3 = 0x13,
    dansi_c0_Code_dc4 = 0x14,
    dansi_c0_Code_nak = 0x15,
    dansi_c0_Code_syn = 0x16,
    dansi_c0_Code_etb = 0x17,
    dansi_c0_Code_can = 0x18,
    dansi_c0_Code_em = 0x19,
    dansi_c0_Code_sub = 0x1a,
    dansi_c0_Code_esc = 0x1b,
    dansi_c0_Code_fs = 0x1c,
    dansi_c0_Code_gs = 0x1d,
    dansi_c0_Code_rs = 0x1e,
    dansi_c0_Code_us = 0x1f
)) dansi_c0_Code;
claim_assert_static(eqlType$(dansi_c0_Code, u8));
T_use_prl$(dansi_c0_Code);

$attr($inline_always)
$static fn_((dansi_c0_isCtrl(u8 byte))(bool));

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((dansi_c0_isCtrl(u8 byte))(bool)) { return byte <= dansi_c0_Code_us; };
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_c0__included */
