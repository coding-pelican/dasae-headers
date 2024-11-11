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


#ifndef CORE_PRIM_INCLUDED
#define CORE_PRIM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "prim/cfg.h"

#include "prim/bool.h"
#include "prim/int.h"
#include "prim/flt.h"
#include "prim/ptr.h"
#include "prim/ref.h"
#include "prim/null.h"

#include "prim/enum.h"
#include "prim/struct.h"
#include "prim/union.h"

/*========== Macros and Definitions =========================================*/

#define prim_as(TYPE, PRIM_VAL...)      IMPL_prim_as(TYPE, PRIM_VAL)
#define prim_value(TYPE, PRIM_VAL...)   IMPL_prim_value(TYPE, PRIM_VAL)
#define prim_literal(TYPE, PRIM_VAL...) IMPL_prim_literal(TYPE, PRIM_VAL)

#define prim_eq(PRIM_LHS, PRIM_RHS...) IMPL_prim_eq(PRIM_LHS, PRIM_RHS)
#define prim_ne(PRIM_LHS, PRIM_RHS...) IMPL_prim_ne(PRIM_LHS, PRIM_RHS)
#define prim_lt(PRIM_LHS, PRIM_RHS...) IMPL_prim_lt(PRIM_LHS, PRIM_RHS)
#define prim_le(PRIM_LHS, PRIM_RHS...) IMPL_prim_le(PRIM_LHS, PRIM_RHS)
#define prim_gt(PRIM_LHS, PRIM_RHS...) IMPL_prim_gt(PRIM_LHS, PRIM_RHS)
#define prim_ge(PRIM_LHS, PRIM_RHS...) IMPL_prim_ge(PRIM_LHS, PRIM_RHS)

#define prim_divisible(PRIM_N, PRIM_M...) IMPL_prim_divisible(PRIM_N, PRIM_M...)

/*========== Macros Implementation ==========================================*/

#define IMPL_prim_as(TYPE, PRIM_VAL...)      ((TYPE)PRIM_VAL)
#define IMPL_prim_value(TYPE, PRIM_VAL...)   ((TYPE){ PRIM_VAL })
#define IMPL_prim_literal(TYPE, PRIM_VAL...) ((TYPE[1]){ PRIM_VAL })

#define IMPL_prim_eq(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) == (PRIM_RHS))
#define IMPL_prim_ne(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) != (PRIM_RHS))
#define IMPL_prim_lt(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) < (PRIM_RHS))
#define IMPL_prim_le(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) <= (PRIM_RHS))
#define IMPL_prim_gt(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) > (PRIM_RHS))
#define IMPL_prim_ge(PRIM_LHS, PRIM_RHS...) ((PRIM_LHS) >= (PRIM_RHS))

#define IMPL_prim_divisible(PRIM_N, PRIM_M...) ((PRIM_M == 0) ? 0 : ((PRIM_N) % (PRIM_M) == 0))


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_INCLUDED */
