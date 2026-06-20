/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    ctrl.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_ctrl
 */
#pragma once
#ifndef dansi_ctrl__included
#define dansi_ctrl__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "c0.h"
#include "c1.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_ctrl_Code $fits($packed))(
    dansi_ctrl_Code_del = 0x7f
)) dansi_ctrl_Code;
claim_assert_static(eqlType$(dansi_ctrl_Code, u8));
T_use_prl$(dansi_ctrl_Code);

$attr($inline_always)
$static fn_((dansi_ctrl_isCtrl(u8 byte))(bool)) {
    return dansi_c0_isCtrl(byte) || byte == u8_(dansi_ctrl_Code_del) || dansi_c1_isCtrl(byte);
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_ctrl__included */
