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

#include "prim/bitwise.h"
#include "prim/bool.h"
#include "prim/int.h"
#include "prim/flt.h"
#include "prim/ptr.h"
#include "prim/null.h"
#include "prim/wchar.h"

#include "prim/enum.h"
#include "prim/struct.h"
#include "prim/union.h"
#include "prim/switch.h"

/*========== Macros and Definitions =========================================*/

#define prim_cmp(val_lhs, val_rhs) IMPL_prim_cmp(val_lhs, val_rhs)
#define prim_eq(val_lhs, val_rhs)  IMPL_prim_eq(val_lhs, val_rhs)
#define prim_ne(val_lhs, val_rhs)  IMPL_prim_ne(val_lhs, val_rhs)
#define prim_lt(val_lhs, val_rhs)  IMPL_prim_lt(val_lhs, val_rhs)
#define prim_gt(val_lhs, val_rhs)  IMPL_prim_gt(val_lhs, val_rhs)
#define prim_le(val_lhs, val_rhs)  IMPL_prim_le(val_lhs, val_rhs)
#define prim_ge(val_lhs, val_rhs)  IMPL_prim_ge(val_lhs, val_rhs)

#define prim_divisible(val_lhs, val_rhs) IMPL_prim_divisible(val_lhs, val_rhs)

/*========== Macros Implementation ==========================================*/

#define IMPL_prim_cmp(val_lhs, val_rhs) ((val_lhs) < (val_rhs) ? -1 : ((val_lhs) > (val_rhs) ? 1 : 0))
#define IMPL_prim_eq(val_lhs, val_rhs)  ((val_lhs) == (val_rhs))
#define IMPL_prim_ne(val_lhs, val_rhs)  ((val_lhs) != (val_rhs))
#define IMPL_prim_lt(val_lhs, val_rhs)  ((val_lhs) < (val_rhs))
#define IMPL_prim_le(val_lhs, val_rhs)  ((val_lhs) <= (val_rhs))
#define IMPL_prim_gt(val_lhs, val_rhs)  ((val_lhs) > (val_rhs))
#define IMPL_prim_ge(val_lhs, val_rhs)  ((val_lhs) >= (val_rhs))

#define IMPL_prim_divisible(val_lhs, val_rhs) (((val_rhs) == 0) ? 0 : ((val_lhs) % (val_rhs) == 0))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_INCLUDED */
