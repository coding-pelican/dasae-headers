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

#define prim_eq(LHS, RHS...) IMPL_prim_eq(LHS, RHS)
#define prim_ne(LHS, RHS...) IMPL_prim_ne(LHS, RHS)
#define prim_lt(LHS, RHS...) IMPL_prim_lt(LHS, RHS)
#define prim_le(LHS, RHS...) IMPL_prim_le(LHS, RHS)
#define prim_gt(LHS, RHS...) IMPL_prim_gt(LHS, RHS)
#define prim_ge(LHS, RHS...) IMPL_prim_ge(LHS, RHS)

#define prim_divisible(LHS, RHS...) IMPL_prim_divisible(LHS, RHS...)

/*========== Macros Implementation ==========================================*/

#define IMPL_prim_eq(LHS, RHS...) ((LHS) == (RHS))
#define IMPL_prim_ne(LHS, RHS...) ((LHS) != (RHS))
#define IMPL_prim_lt(LHS, RHS...) ((LHS) < (RHS))
#define IMPL_prim_le(LHS, RHS...) ((LHS) <= (RHS))
#define IMPL_prim_gt(LHS, RHS...) ((LHS) > (RHS))
#define IMPL_prim_ge(LHS, RHS...) ((LHS) >= (RHS))

#define IMPL_prim_divisible(LHS, RHS...) (((RHS) == 0) ? 0 : ((LHS) % (RHS) == 0))


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_INCLUDED */
