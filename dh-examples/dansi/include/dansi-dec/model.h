/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    model.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_dec_model
 */
#pragma once
#ifndef dansi_dec_model__included
#define dansi_dec_model__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/prl.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_dec_model_Term $fits($packed))(
    dansi_dec_model_Term_vt52 = 52,
    dansi_dec_model_Term_vt100 = 100,
    dansi_dec_model_Term_vt220 = 220,
    dansi_dec_model_Term_vt320 = 320,
    dansi_dec_model_Term_vt420 = 420,
    dansi_dec_model_Term_vt510 = 510,
    dansi_dec_model_Term_vt520 = 520
)) dansi_dec_model_Term;
claim_assert_static(eqlType$(dansi_dec_model_Term, u16));
T_use_prl$(dansi_dec_model_Term);

typedef struct dansi_dec_model_Feats {
    var_(has_csi_8bit, bool);
    var_(has_s8c1t, bool);
    var_(has_left_right_margin, bool);
    var_(has_rectangular_ops, bool);
} dansi_dec_model_Feats;
T_use_prl$(dansi_dec_model_Feats);

$extern fn_((dansi_dec_model_feats(dansi_dec_model_Term term))(dansi_dec_model_Feats));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_dec_model__included */
