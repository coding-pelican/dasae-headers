/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    cmp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-24 (date of creation)
 * @updated 2024-11-29 (date of last update)
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

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

typedef enum cmp_Ord {
    cmp_Ord_less    = -1,
    cmp_Ord_equal   = 0,
    cmp_Ord_greater = 1
} cmp_Ord;

/* Comparators */
#define cmp_cmp(TSelf) pp_join(_, TSelf, cmp)

/* Relational operators */
#define cmp_eq(TSelf) pp_join(_, TSelf, eq)
#define cmp_ne(TSelf) pp_join(_, TSelf, ne)
#define cmp_lt(TSelf) pp_join(_, TSelf, lt)
#define cmp_gt(TSelf) pp_join(_, TSelf, gt)
#define cmp_le(TSelf) pp_join(_, TSelf, le)
#define cmp_ge(TSelf) pp_join(_, TSelf, ge)

/* Function-like macros */
#define cmp_fnCmp(TSelf)        cmp_Ord pp_join(_, cmp, cmp)(TSelf)(TSelf self, TSelf other)
#define cmp_fnOrd(fnCmp, TSelf) bool pp_join(_, cmp, fnCmp)(TSelf)(TSelf self, TSelf other)
#define cmp_fnEq(TSelf)         cmp_fnOrd(eq, TSelf)
#define cmp_fnNe(TSelf)         cmp_fnOrd(ne, TSelf)
#define cmp_fnLt(TSelf)         cmp_fnOrd(lt, TSelf)
#define cmp_fnGt(TSelf)         cmp_fnOrd(gt, TSelf)
#define cmp_fnLe(TSelf)         cmp_fnOrd(le, TSelf)
#define cmp_fnGe(TSelf)         cmp_fnOrd(ge, TSelf)

/* Binary operators default implementation */
// clang-format off
#define cmp_fnEq_default(TSelf) force_inline cmp_fnOrd(eq, TSelf) { return pp_join(_, TSelf, cmp)(self, other) == cmp_Ord_equal; }
#define cmp_fnNe_default(TSelf) force_inline cmp_fnOrd(ne, TSelf) { return pp_join(_, TSelf, eq)(self, other) !=  cmp_Ord_equal; }
#define cmp_fnLt_default(TSelf) force_inline cmp_fnOrd(lt, TSelf) { return pp_join(_, TSelf, cmp)(self, other) == cmp_Ord_less; }
#define cmp_fnGt_default(TSelf) force_inline cmp_fnOrd(gt, TSelf) { return pp_join(_, TSelf, cmp)(self, other) == cmp_Ord_greater; }
#define cmp_fnLe_default(TSelf) force_inline cmp_fnOrd(le, TSelf) { return !pp_join(_, TSelf, gt)(self, other); }
#define cmp_fnGe_default(TSelf) force_inline cmp_fnOrd(ge, TSelf) { return !pp_join(_, TSelf, lt)(self, other); }
// clang-format on

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
cmp_fnCmp(int) {
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
