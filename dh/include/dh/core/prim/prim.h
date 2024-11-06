/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    prim.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2024-10-28 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  prim
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef PRIM_INCLUDED
#define PRIM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "prim_cfg.h"

#include "prim_bool.h"
#include "prim_flt.h"
#include "prim_int.h"
#include "prim_null.h"
#include "prim_ptr.h"
#include "prim_ref.h"

#include "prim_enum.h"
#include "prim_struct.h"
#include "prim_union.h"

/*========== Macros and Definitions =========================================*/

#define prim_as(TYPE, PRIM_VAL...)      RETURN_prim_as(TYPE, PRIM_VAL)
#define prim_value(TYPE, PRIM_VAL...)   RETURN_prim_value(TYPE, PRIM_VAL)
#define prim_literal(TYPE, PRIM_VAL...) RETURN_prim_literal(TYPE, PRIM_VAL)

#define prim_eq(PRIM_LHS, PRIM_RHS...) RETURN_prim_eq(PRIM_LHS, PRIM_RHS)
#define prim_ne(PRIM_LHS, PRIM_RHS...) RETURN_prim_ne(PRIM_LHS, PRIM_RHS)
#define prim_lt(PRIM_LHS, PRIM_RHS...) RETURN_prim_lt(PRIM_LHS, PRIM_RHS)
#define prim_le(PRIM_LHS, PRIM_RHS...) RETURN_prim_le(PRIM_LHS, PRIM_RHS)
#define prim_gt(PRIM_LHS, PRIM_RHS...) RETURN_prim_gt(PRIM_LHS, PRIM_RHS)
#define prim_ge(PRIM_LHS, PRIM_RHS...) RETURN_prim_ge(PRIM_LHS, PRIM_RHS)

#define prim_divisible(PRIM_N, PRIM_M...) RETURN_prim_divisible(PRIM_N, PRIM_M...)

/*========== Macros Implementation ==========================================*/

#define RETURN_prim_as(TYPE, PRIM_VAL...)      ((TYPE)PRIM_VAL)
#define RETURN_prim_value(TYPE, PRIM_VAL...)   ((TYPE){ PRIM_VAL })
#define RETURN_prim_literal(TYPE, PRIM_VAL...) ((TYPE[1]){ PRIM_VAL })

#define RETURN_prim_eq(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) == (PRIM_RHS))
#define RETURN_prim_ne(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) != (PRIM_RHS))
#define RETURN_prim_lt(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) < (PRIM_RHS))
#define RETURN_prim_le(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) <= (PRIM_RHS))
#define RETURN_prim_gt(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) > (PRIM_RHS))
#define RETURN_prim_ge(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) >= (PRIM_RHS))

#define RETURN_prim_divisible(PRIM_N, PRIM_M...) ((PRIM_M == 0) ? 0 : ((PRIM_N) % (PRIM_M) == 0))


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PRIM_INCLUDED */
