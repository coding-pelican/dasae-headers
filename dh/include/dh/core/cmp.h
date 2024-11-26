/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    cmp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-24 (date of creation)
 * @updated 2024-11-24 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  cmp
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef CORE_CMP_INCLUDED
#define CORE_CMP_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin.h"
#include "dh/core/prim/bool.h"
#include "dh/core/pp.h"

/*========== Macros and Definitions =========================================*/

typedef enum cmp_Ord {
    cmp_Ord_less    = -1,
    cmp_Ord_equal   = 0,
    cmp_Ord_greater = 1
} cmp_Ord;

#define cmp(TSelf) pp_join(_, TSelf, cmp)

#define eq(TSelf) pp_join(_, TSelf, eq)
#define ne(TSelf) pp_join(_, TSelf, ne)
#define lt(TSelf) pp_join(_, TSelf, lt)
#define gt(TSelf) pp_join(_, TSelf, gt)
#define le(TSelf) pp_join(_, TSelf, le)
#define ge(TSelf) pp_join(_, TSelf, ge)

/* Function-like macros */
#define cmp_fn(TRet, fnOps...)                  static_inline TRet fnOps
#define cmp_fnOrd(/* TRet: cmp_Ord */ fnOps...) cmp_fn(cmp_Ord, fnOps)
#define cmp_fnBool(/* TRet: bool */ fnOps...)   cmp_fn(bool, fnOps)

/* Comparators */
#define cmp_cmp(TSelf) pp_join(_, TSelf, cmp)(TSelf self, TSelf other)

// clang-format off
/* Binary operators */
#define cmp_eq(TSelf) cmp_fnBool(pp_join(_, TSelf, eq)(TSelf self, TSelf other))
#define cmp_ne(TSelf) cmp_fnBool(pp_join(_, TSelf, ne)(TSelf self, TSelf other))
#define cmp_lt(TSelf) cmp_fnBool(pp_join(_, TSelf, lt)(TSelf self, TSelf other))
#define cmp_gt(TSelf) cmp_fnBool(pp_join(_, TSelf, gt)(TSelf self, TSelf other))
#define cmp_le(TSelf) cmp_fnBool(pp_join(_, TSelf, le)(TSelf self, TSelf other))
#define cmp_ge(TSelf) cmp_fnBool(pp_join(_, TSelf, ge)(TSelf self, TSelf other))

/* Binary operators default implementation */
#define cmp_eq_impl(TSelf) cmp_eq(TSelf) { return pp_join(_, TSelf, cmp)(self, other) == cmp_Ord_equal; }
#define cmp_ne_impl(TSelf) cmp_ne(TSelf) { return !pp_join(_, TSelf, eq)(self, other); }
#define cmp_lt_impl(TSelf) cmp_lt(TSelf) { return pp_join(_, TSelf, cmp)(self, other) == cmp_Ord_less; }
#define cmp_gt_impl(TSelf) cmp_gt(TSelf) { return pp_join(_, TSelf, cmp)(self, other) == cmp_Ord_greater; }
#define cmp_le_impl(TSelf) cmp_le(TSelf) { return !pp_join(_, TSelf, gt)(self, other); }
#define cmp_ge_impl(TSelf) cmp_ge(TSelf) { return !pp_join(_, TSelf, lt)(self, other); }
// clang-format on

// #define cmp_impl(TSelf) \
//     cmp_eq_impl(TSelf) \
//     cmp_ne_impl(TSelf) \
//     cmp_lt_impl(TSelf) \
//     cmp_gt_impl(TSelf) \
//     cmp_le_impl(TSelf) \
//     cmp_ge_impl(TSelf)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
cmp_fnOrd(cmp_cmp(int)) {
    if (self < other) { return cmp_Ord_less; }
    if (other < self) { return cmp_Ord_greater; }
    return cmp_Ord_equal;
}
// cmp_impl(int)
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_CMP_INCLUDED */
