/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cmp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-24 (date of creation)
 * @updated 2024-11-29 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  (none)
 *
 * @brief   Comparator utilities
 * @details Provides comparator utilities and functions for:
 *          - Comparison operations
 *          - Equality checks
 *          - Inequality checks
 *          - Ordering operations
 *          - Comparison utilities
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
    cmp_Ord_lt = -1,
    cmp_Ord_eq = 0,
    cmp_Ord_gt = 1,
} cmp_Ord;

#define cmp_Ord_inv(_ord...) (-(_ord))

/* Comparators */
#define cmp_cmp(TSelf) pp_join(_, TSelf, cmp)
#define cmp_cmpApx(TSelf) pp_join(_, TSelf, cmpApx)

/* Relational operators */
#define cmp_eq(TSelf) pp_join(_, TSelf, eq)
#define cmp_ne(TSelf) pp_join(_, TSelf, ne)
#define cmp_lt(TSelf) pp_join(_, TSelf, lt)
#define cmp_gt(TSelf) pp_join(_, TSelf, gt)
#define cmp_le(TSelf) pp_join(_, TSelf, le)
#define cmp_ge(TSelf) pp_join(_, TSelf, ge)
#define cmp_eqApx(TSelf) pp_join(_, TSelf, eqApx)
#define cmp_neApx(TSelf) pp_join(_, TSelf, neApx)
#define cmp_ltApx(TSelf) pp_join(_, TSelf, ltApx)
#define cmp_gtApx(TSelf) pp_join(_, TSelf, gtApx)
#define cmp_leApx(TSelf) pp_join(_, TSelf, leApx)
#define cmp_geApx(TSelf) pp_join(_, TSelf, geApx)

/* Function-like macros */
#define cmp_fnCmp(TSelf) cmp_Ord pp_join(_, cmp, cmp)(TSelf)(const TSelf self, const TSelf other)
#define cmp_fnOrd(fnCmp, TSelf) bool pp_join(_, cmp, fnCmp)(TSelf)(const TSelf self, const TSelf other)
#define cmp_fnEq(TSelf) cmp_fnOrd(eq, TSelf)
#define cmp_fnNe(TSelf) cmp_fnOrd(ne, TSelf)
#define cmp_fnLt(TSelf) cmp_fnOrd(lt, TSelf)
#define cmp_fnGt(TSelf) cmp_fnOrd(gt, TSelf)
#define cmp_fnLe(TSelf) cmp_fnOrd(le, TSelf)
#define cmp_fnGe(TSelf) cmp_fnOrd(ge, TSelf)

#define cmp_fnCmpApx(TSelf) cmp_Ord pp_join(_, cmp, cmpApx)(TSelf)(const TSelf self, const TSelf other, const TSelf threshold)
#define cmp_fnOrdApx(fnCmpApx, TSelf) bool pp_join(_, cmp, fnCmpApx)(TSelf)(const TSelf self, const TSelf other, const TSelf threshold)
#define cmp_fnEqApx(TSelf) cmp_fnOrdApx(eqApx, TSelf)
#define cmp_fnNeApx(TSelf) cmp_fnOrdApx(neApx, TSelf)
#define cmp_fnLtApx(TSelf) cmp_fnOrdApx(ltApx, TSelf)
#define cmp_fnGtApx(TSelf) cmp_fnOrdApx(gtApx, TSelf)
#define cmp_fnLeApx(TSelf) cmp_fnOrdApx(leApx, TSelf)
#define cmp_fnGeApx(TSelf) cmp_fnOrdApx(geApx, TSelf)

/* Binary operators default implementation */
// clang-format off
#define cmp_fnEq_default(TSelf) $inline_always cmp_fnOrd(eq, TSelf) { return pp_join(_, TSelf, cmp)(self, other) == cmp_Ord_eq; }
#define cmp_fnNe_default(TSelf) $inline_always cmp_fnOrd(ne, TSelf) { return !pp_join(_, TSelf, eq)(self, other); }
#define cmp_fnLt_default(TSelf) $inline_always cmp_fnOrd(lt, TSelf) { return pp_join(_, TSelf, cmp)(self, other) == cmp_Ord_lt; }
#define cmp_fnGt_default(TSelf) $inline_always cmp_fnOrd(gt, TSelf) { return pp_join(_, TSelf, cmp)(self, other) == cmp_Ord_gt; }
#define cmp_fnLe_default(TSelf) $inline_always cmp_fnOrd(le, TSelf) { return !pp_join(_, TSelf, gt)(self, other); }
#define cmp_fnGe_default(TSelf) $inline_always cmp_fnOrd(ge, TSelf) { return !pp_join(_, TSelf, lt)(self, other); }
// clang-format on

// clang-format off
#define cmp_fnEqApx_default(TSelf) $inline_always cmp_fnOrdApx(eqApx, TSelf) { return pp_join(_, TSelf, cmpApx)(self, other, threshold) == cmp_Ord_eq; }
#define cmp_fnNeApx_default(TSelf) $inline_always cmp_fnOrdApx(neApx, TSelf) { return !pp_join(_, TSelf, eqApx)(self, other, threshold); }
#define cmp_fnLtApx_default(TSelf) $inline_always cmp_fnOrdApx(ltApx, TSelf) { return pp_join(_, TSelf, cmpApx)(self, other, threshold) == cmp_Ord_lt; }
#define cmp_fnGtApx_default(TSelf) $inline_always cmp_fnOrdApx(gtApx, TSelf) { return pp_join(_, TSelf, cmpApx)(self, other, threshold) == cmp_Ord_gt; }
#define cmp_fnLeApx_default(TSelf) $inline_always cmp_fnOrdApx(leApx, TSelf) { return !pp_join(_, TSelf, gtApx)(self, other, threshold); }
#define cmp_fnGeApx_default(TSelf) $inline_always cmp_fnOrdApx(geApx, TSelf) { return !pp_join(_, TSelf, ltApx)(self, other, threshold); }
// clang-format on

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
cmp_fnCmp(int) {
    if (self < other) { return cmp_Ord_lt; }
    if (other < self) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
}
// cmp_impl(int)
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_CMP_INCLUDED */
