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
#ifndef core_cmp__included
#define core_cmp__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim.h"

/*========== Macros and Declarations ========================================*/

/* --- Equality Operators --- */

#define cmp_eql$(_T /*)(_lhs: _T, _rhs: _T) -> (bool)(*/) pp_join(_, _T, eql)
#define cmp_neq$(_T /*)(_lhs: _T, _rhs: _T) -> (bool)(*/) pp_join(_, _T, neq)

#define cmp_eqlCtx$(_T /*)(_lhs: _T, _rhs: _T, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, eqlCtx)
#define cmp_neqCtx$(_T /*)(_lhs: _T, _rhs: _T, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, neqCtx)

#define cmp_u_eql$(_T /*)(_lhs: _T, _rhs: _T) -> (bool)(*/) pp_join(_, _T, u_eql)
#define cmp_u_neq$(_T /*)(_lhs: _T, _rhs: _T) -> (bool)(*/) pp_join(_, _T, u_neq)

#define cmp_u_eqlCtx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_eqlCtx)
#define cmp_u_neqCtx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_neqCtx)

/* --- Ordering Operators --- */

typedef enum_(cmp_Ord $bits(8)) {
    cmp_Ord_lt = -1,
    cmp_Ord_eq = 0,
    cmp_Ord_gt = 1,
} cmp_Ord;
#define cmp_Ord_inv(_ord /*: cmp_Ord*/... /*(cmp_Ord)*/) (-(_ord))
#define cmp_Ord_isEq(_ord /*: cmp_Ord*/... /*(bool)*/) (as$(bool)((_ord) == cmp_Ord_eq))
#define cmp_Ord_isNe(_ord /*: cmp_Ord*/... /*(bool)*/) (as$(bool)((_ord) != cmp_Ord_eq))
#define cmp_Ord_isLt(_ord /*: cmp_Ord*/... /*(bool)*/) (as$(bool)((_ord) < cmp_Ord_eq))
#define cmp_Ord_isGt(_ord /*: cmp_Ord*/... /*(bool)*/) (as$(bool)((_ord) > cmp_Ord_eq))
#define cmp_Ord_isLe(_ord /*: cmp_Ord*/... /*(bool)*/) (as$(bool)((_ord) <= cmp_Ord_eq))
#define cmp_Ord_isGe(_ord /*: cmp_Ord*/... /*(bool)*/) (as$(bool)((_ord) >= cmp_Ord_eq))

/* --- Comparison Operators --- */

#define cmp_ord$(_T /*)(_lhs: _T, _rhs: _T) -> (cmp_Ord)(*/) pp_join(_, _T, ord)
#define cmp_eq$(_T /*)(_lhs: _T, _rhs: _T) -> (bool)(*/) pp_join(_, _T, eq)
#define cmp_ne$(_T /*)(_lhs: _T, _rhs: _T) -> (bool)(*/) pp_join(_, _T, ne)
#define cmp_lt$(_T /*)(_lhs: _T, _rhs: _T) -> (bool)(*/) pp_join(_, _T, lt)
#define cmp_gt$(_T /*)(_lhs: _T, _rhs: _T) -> (bool)(*/) pp_join(_, _T, gt)
#define cmp_le$(_T /*)(_lhs: _T, _rhs: _T) -> (bool)(*/) pp_join(_, _T, le)
#define cmp_ge$(_T /*)(_lhs: _T, _rhs: _T) -> (bool)(*/) pp_join(_, _T, ge)

#define cmp_ordCtx$(_T /*)(_lhs: _T, _rhs: _T, _ctx: u_V$raw) -> (cmp_Ord)(*/) pp_join(_, _T, ordCtx)
#define cmp_eqCtx$(_T /*)(_lhs: _T, _rhs: _T, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, eqCtx)
#define cmp_neCtx$(_T /*)(_lhs: _T, _rhs: _T, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, neCtx)
#define cmp_ltCtx$(_T /*)(_lhs: _T, _rhs: _T, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, ltCtx)
#define cmp_gtCtx$(_T /*)(_lhs: _T, _rhs: _T, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, gtCtx)
#define cmp_leCtx$(_T /*)(_lhs: _T, _rhs: _T, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, leCtx)
#define cmp_geCtx$(_T /*)(_lhs: _T, _rhs: _T, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, geCtx)

#define cmp_ordApx$(_T /*)(_lhs: _T, _rhs: _T, _threshold: _T) -> (cmp_Ord)(*/) pp_join(_, _T, ordApx)
#define cmp_eqApx$(_T /*)(_lhs: _T, _rhs: _T, _threshold: _T) -> (bool)(*/) pp_join(_, _T, eqApx)
#define cmp_neApx$(_T /*)(_lhs: _T, _rhs: _T, _threshold: _T) -> (bool)(*/) pp_join(_, _T, neApx)
#define cmp_ltApx$(_T /*)(_lhs: _T, _rhs: _T, _threshold: _T) -> (bool)(*/) pp_join(_, _T, ltApx)
#define cmp_gtApx$(_T /*)(_lhs: _T, _rhs: _T, _threshold: _T) -> (bool)(*/) pp_join(_, _T, gtApx)
#define cmp_leApx$(_T /*)(_lhs: _T, _rhs: _T, _threshold: _T) -> (bool)(*/) pp_join(_, _T, leApx)
#define cmp_geApx$(_T /*)(_lhs: _T, _rhs: _T, _threshold: _T) -> (bool)(*/) pp_join(_, _T, geApx)

#define cmp_u_ord$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw) -> (cmp_Ord)(*/) pp_join(_, _T, u_ord)
#define cmp_u_eq$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_eq)
#define cmp_u_ne$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_ne)
#define cmp_u_lt$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_lt)
#define cmp_u_gt$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_gt)
#define cmp_u_le$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_le)
#define cmp_u_ge$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_ge)

#define cmp_u_ordCtx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _ctx: u_V$raw) -> (cmp_Ord)(*/) pp_join(_, _T, u_ordCtx)
#define cmp_u_eqCtx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_eqCtx)
#define cmp_u_neCtx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_neCtx)
#define cmp_u_ltCtx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_ltCtx)
#define cmp_u_gtCtx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_gtCtx)
#define cmp_u_leCtx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_leCtx)
#define cmp_u_geCtx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _ctx: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_geCtx)

#define cmp_u_ordApx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _threshold: u_V$raw) -> (cmp_Ord)(*/) pp_join(_, _T, u_ordApx)
#define cmp_u_eqApx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _threshold: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_eqApx)
#define cmp_u_neApx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _threshold: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_neApx)
#define cmp_u_ltApx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _threshold: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_ltApx)
#define cmp_u_gtApx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _threshold: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_gtApx)
#define cmp_u_leApx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _threshold: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_leApx)
#define cmp_u_geApx$(_T /*)(_lhs: u_V$raw, _rhs: u_V$raw, _threshold: u_V$raw) -> (bool)(*/) pp_join(_, _T, u_geApx)

/* --- Function-like Macros --- */

#define cmp_fn_(_op) pp_cat(pp_join(_, cmp_fn, _op), $)

#define cmp_fn_eql$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_eql$(__step__cmp_fn_eql$__parse __VA_ARGS__)
#define cmp_fn_neq$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_neq$(__step__cmp_fn_neq$__parse __VA_ARGS__)

#define cmp_fn_eqlCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_eqlCtx$(__step__cmp_fn_eqlCtx$__parse __VA_ARGS__)
#define cmp_fn_neqCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_neqCtx$(__step__cmp_fn_neqCtx$__parse __VA_ARGS__)

#define cmp_fn_ord$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_ord$(__step__cmp_fn_ord$__parse __VA_ARGS__)
#define cmp_fn_eq$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_eq$(__step__cmp_fn_eq$__parse __VA_ARGS__)
#define cmp_fn_ne$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_ne$(__step__cmp_fn_ne$__parse __VA_ARGS__)
#define cmp_fn_lt$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_lt$(__step__cmp_fn_lt$__parse __VA_ARGS__)
#define cmp_fn_gt$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_gt$(__step__cmp_fn_gt$__parse __VA_ARGS__)
#define cmp_fn_le$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_le$(__step__cmp_fn_le$__parse __VA_ARGS__)
#define cmp_fn_ge$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_ge$(__step__cmp_fn_ge$__parse __VA_ARGS__)

#define cmp_fn_ordCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_ordCtx$(__step__cmp_fn_ordCtx$__parse __VA_ARGS__)
#define cmp_fn_eqCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_eqCtx$(__step__cmp_fn_eqCtx$__parse __VA_ARGS__)
#define cmp_fn_neCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_neCtx$(__step__cmp_fn_neCtx$__parse __VA_ARGS__)
#define cmp_fn_ltCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_ltCtx$(__step__cmp_fn_ltCtx$__parse __VA_ARGS__)
#define cmp_fn_gtCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_gtCtx$(__step__cmp_fn_gtCtx$__parse __VA_ARGS__)
#define cmp_fn_leCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_leCtx$(__step__cmp_fn_leCtx$__parse __VA_ARGS__)
#define cmp_fn_geCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_geCtx$(__step__cmp_fn_geCtx$__parse __VA_ARGS__)

#define cmp_fn_ordApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_ordApx$(__step__cmp_fn_ordApx$__parse __VA_ARGS__)
#define cmp_fn_eqApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_eqApx$(__step__cmp_fn_eqApx$__parse __VA_ARGS__)
#define cmp_fn_neApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_neApx$(__step__cmp_fn_neApx$__parse __VA_ARGS__)
#define cmp_fn_ltApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_ltApx$(__step__cmp_fn_ltApx$__parse __VA_ARGS__)
#define cmp_fn_gtApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_gtApx$(__step__cmp_fn_gtApx$__parse __VA_ARGS__)
#define cmp_fn_leApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_leApx$(__step__cmp_fn_leApx$__parse __VA_ARGS__)
#define cmp_fn_geApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_geApx$(__step__cmp_fn_geApx$__parse __VA_ARGS__)

#define cmp_fn_u_eql$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_u_eql$(__step__cmp_fn_u_eql$__parse __VA_ARGS__)
#define cmp_fn_u_neq$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_u_neq$(__step__cmp_fn_u_neq$__parse __VA_ARGS__)

#define cmp_fn_u_eqlCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_u_eqlCtx$(__step__cmp_fn_u_eqlCtx$__parse __VA_ARGS__)
#define cmp_fn_u_neqCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_u_neqCtx$(__step__cmp_fn_u_neqCtx$__parse __VA_ARGS__)

#define cmp_fn_u_ord$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_u_ord$(__step__cmp_fn_u_ord$__parse __VA_ARGS__)
#define cmp_fn_u_eq$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_u_eq$(__step__cmp_fn_u_eq$__parse __VA_ARGS__)
#define cmp_fn_u_ne$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_u_ne$(__step__cmp_fn_u_ne$__parse __VA_ARGS__)
#define cmp_fn_u_lt$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_u_lt$(__step__cmp_fn_u_lt$__parse __VA_ARGS__)
#define cmp_fn_u_gt$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_u_gt$(__step__cmp_fn_u_gt$__parse __VA_ARGS__)
#define cmp_fn_u_le$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_u_le$(__step__cmp_fn_u_le$__parse __VA_ARGS__)
#define cmp_fn_u_ge$(/*(_T)(_id_lhs, _id_rhs)*/...) __step__cmp_fn_u_ge$(__step__cmp_fn_u_ge$__parse __VA_ARGS__)

#define cmp_fn_u_ordCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_u_ordCtx$(__step__cmp_fn_u_ordCtx$__parse __VA_ARGS__)
#define cmp_fn_u_eqCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_u_eqCtx$(__step__cmp_fn_u_eqCtx$__parse __VA_ARGS__)
#define cmp_fn_u_neCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_u_neCtx$(__step__cmp_fn_u_neCtx$__parse __VA_ARGS__)
#define cmp_fn_u_ltCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_u_ltCtx$(__step__cmp_fn_u_ltCtx$__parse __VA_ARGS__)
#define cmp_fn_u_gtCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_u_gtCtx$(__step__cmp_fn_u_gtCtx$__parse __VA_ARGS__)
#define cmp_fn_u_leCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_u_leCtx$(__step__cmp_fn_u_leCtx$__parse __VA_ARGS__)
#define cmp_fn_u_geCtx$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) __step__cmp_fn_u_geCtx$(__step__cmp_fn_u_geCtx$__parse __VA_ARGS__)

#define cmp_fn_u_ordApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_u_ordApx$(__step__cmp_fn_u_ordApx$__parse __VA_ARGS__)
#define cmp_fn_u_eqApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_u_eqApx$(__step__cmp_fn_u_eqApx$__parse __VA_ARGS__)
#define cmp_fn_u_neApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_u_neApx$(__step__cmp_fn_u_neApx$__parse __VA_ARGS__)
#define cmp_fn_u_ltApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_u_ltApx$(__step__cmp_fn_u_ltApx$__parse __VA_ARGS__)
#define cmp_fn_u_gtApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_u_gtApx$(__step__cmp_fn_u_gtApx$__parse __VA_ARGS__)
#define cmp_fn_u_leApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_u_leApx$(__step__cmp_fn_u_leApx$__parse __VA_ARGS__)
#define cmp_fn_u_geApx$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) __step__cmp_fn_u_geApx$(__step__cmp_fn_u_geApx$__parse __VA_ARGS__)

/* --- Function-like Macros Default Implementations --- */

#define cmp_fn_eql_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_neq` is implemented */ \
    __step__cmp_fn_eql_default$(__step__cmp_fn_eql_default$__parse __VA_ARGS__)
#define cmp_fn_neq_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_eql` is implemented */ \
    __step__cmp_fn_neq_default$(__step__cmp_fn_neq_default$__parse __VA_ARGS__)

#define cmp_fn_eqlCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_neqCtx` is implemented */ \
    __step__cmp_fn_eqlCtx_default$(__step__cmp_fn_eqlCtx_default$__parse __VA_ARGS__)
#define cmp_fn_neqCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_eqlCtx` is implemented */ \
    __step__cmp_fn_neqCtx_default$(__step__cmp_fn_neqCtx_default$__parse __VA_ARGS__)

#define cmp_fn_ord_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_lt` is implemented */ \
    __step__cmp_fn_ord_default$(__step__cmp_fn_ord_default$__parse __VA_ARGS__)
#define cmp_fn_eq_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_eq_default$(__step__cmp_fn_eq_default$__parse __VA_ARGS__)
#define cmp_fn_ne_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_ne_default$(__step__cmp_fn_ne_default$__parse __VA_ARGS__)
#define cmp_fn_lt_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_lt_default$(__step__cmp_fn_lt_default$__parse __VA_ARGS__)
#define cmp_fn_gt_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_gt_default$(__step__cmp_fn_gt_default$__parse __VA_ARGS__)
#define cmp_fn_le_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_le_default$(__step__cmp_fn_le_default$__parse __VA_ARGS__)
#define cmp_fn_ge_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_ge_default$(__step__cmp_fn_ge_default$__parse __VA_ARGS__)

#define cmp_fn_ordCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_ltCtx` is implemented */ \
    __step__cmp_fn_ordCtx_default$(__step__cmp_fn_ordCtx_default$__parse __VA_ARGS__)
#define cmp_fn_eqCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_eqCtx_default$(__step__cmp_fn_eqCtx_default$__parse __VA_ARGS__)
#define cmp_fn_neCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_neCtx_default$(__step__cmp_fn_neCtx_default$__parse __VA_ARGS__)
#define cmp_fn_ltCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_ltCtx_default$(__step__cmp_fn_ltCtx_default$__parse __VA_ARGS__)
#define cmp_fn_gtCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_gtCtx_default$(__step__cmp_fn_gtCtx_default$__parse __VA_ARGS__)
#define cmp_fn_leCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_leCtx_default$(__step__cmp_fn_leCtx_default$__parse __VA_ARGS__)
#define cmp_fn_geCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_geCtx_default$(__step__cmp_fn_geCtx_default$__parse __VA_ARGS__)

#define cmp_fn_ordApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_ltApx` is implemented */ \
    __step__cmp_fn_ordApx_default$(__step__cmp_fn_ordApx_default$__parse __VA_ARGS__)
#define cmp_fn_eqApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_eqApx_default$(__step__cmp_fn_eqApx_default$__parse __VA_ARGS__)
#define cmp_fn_neApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_neApx_default$(__step__cmp_fn_neApx_default$__parse __VA_ARGS__)
#define cmp_fn_ltApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_ltApx_default$(__step__cmp_fn_ltApx_default$__parse __VA_ARGS__)
#define cmp_fn_gtApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_gtApx_default$(__step__cmp_fn_gtApx_default$__parse __VA_ARGS__)
#define cmp_fn_leApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_leApx_default$(__step__cmp_fn_leApx_default$__parse __VA_ARGS__)
#define cmp_fn_geApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_geApx_default$(__step__cmp_fn_geApx_default$__parse __VA_ARGS__)

#define cmp_fn_u_eql_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_u_neq` is implemented */ \
    __step__cmp_fn_u_eql_default$(__step__cmp_fn_u_eql_default$__parse __VA_ARGS__)
#define cmp_fn_u_neq_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_u_eql` is implemented */ \
    __step__cmp_fn_u_neq_default$(__step__cmp_fn_u_neq_default$__parse __VA_ARGS__)

#define cmp_fn_u_eqlCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_u_neqCtx` is implemented */ \
    __step__cmp_fn_u_eqlCtx_default$(__step__cmp_fn_u_eqlCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_neqCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_u_eqlCtx` is implemented */ \
    __step__cmp_fn_u_neqCtx_default$(__step__cmp_fn_u_neqCtx_default$__parse __VA_ARGS__)

#define cmp_fn_u_ord_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_u_lt` is implemented */ \
    __step__cmp_fn_u_ord_default$(__step__cmp_fn_u_ord_default$__parse __VA_ARGS__)
#define cmp_fn_u_eq_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_eq_default$(__step__cmp_fn_u_eq_default$__parse __VA_ARGS__)
#define cmp_fn_u_ne_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_ne_default$(__step__cmp_fn_u_ne_default$__parse __VA_ARGS__)
#define cmp_fn_u_lt_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_lt_default$(__step__cmp_fn_u_lt_default$__parse __VA_ARGS__)
#define cmp_fn_u_gt_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_gt_default$(__step__cmp_fn_u_gt_default$__parse __VA_ARGS__)
#define cmp_fn_u_le_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_le_default$(__step__cmp_fn_u_le_default$__parse __VA_ARGS__)
#define cmp_fn_u_ge_default$(/*(_T)(_id_lhs, _id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_ge_default$(__step__cmp_fn_u_ge_default$__parse __VA_ARGS__)

#define cmp_fn_u_ordCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_u_ltCtx` is implemented */ \
    __step__cmp_fn_u_ordCtx_default$(__step__cmp_fn_u_ordCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_eqCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_eqCtx_default$(__step__cmp_fn_u_eqCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_neCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_neCtx_default$(__step__cmp_fn_u_neCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_ltCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_ltCtx_default$(__step__cmp_fn_u_ltCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_gtCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_gtCtx_default$(__step__cmp_fn_u_gtCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_leCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_leCtx_default$(__step__cmp_fn_u_leCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_geCtx_default$(/*(_T)(_id_lhs, _id_rhs, _id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_geCtx_default$(__step__cmp_fn_u_geCtx_default$__parse __VA_ARGS__)

#define cmp_fn_u_ordApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_u_ltApx` is implemented */ \
    __step__cmp_fn_u_ordApx_default$(__step__cmp_fn_u_ordApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_eqApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_eqApx_default$(__step__cmp_fn_u_eqApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_neApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_neApx_default$(__step__cmp_fn_u_neApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_ltApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_ltApx_default$(__step__cmp_fn_u_ltApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_gtApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_gtApx_default$(__step__cmp_fn_u_gtApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_leApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_leApx_default$(__step__cmp_fn_u_leApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_geApx_default$(/*(_T)(_id_lhs, _id_rhs, _threshold)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_geApx_default$(__step__cmp_fn_u_geApx_default$__parse __VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __step__cmp_fn_eql$(...) __cmp_fn_eql$(__VA_ARGS__)
#define __step__cmp_fn_eql$__parse(_T...) _T, __step__cmp_fn_eql$__parseNext
#define __step__cmp_fn_eql$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_eql$(_T, _lhs, _rhs...) \
    fn_((cmp_eql$(_T)(_T _lhs, _T _rhs))(bool))
#define __step__cmp_fn_neq$(...) __cmp_fn_neq$(__VA_ARGS__)
#define __step__cmp_fn_neq$__parse(_T...) _T, __step__cmp_fn_neq$__parseNext
#define __step__cmp_fn_neq$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_neq$(_T, _lhs, _rhs...) \
    fn_((cmp_neq$(_T)(_T _lhs, _T _rhs))(bool))

#define __step__cmp_fn_eqlCtx$(...) __cmp_fn_eqlCtx$(__VA_ARGS__)
#define __step__cmp_fn_eqlCtx$__parse(_T...) _T, __step__cmp_fn_eqlCtx$__parseNext
#define __step__cmp_fn_eqlCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_eqlCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_eqlCtx$(_T)(_T _lhs, _T _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_neqCtx$(...) __cmp_fn_neqCtx$(__VA_ARGS__)
#define __step__cmp_fn_neqCtx$__parse(_T...) _T, __step__cmp_fn_neqCtx$__parseNext
#define __step__cmp_fn_neqCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_neqCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_neqCtx$(_T)(_T _lhs, _T _rhs, u_V$raw _ctx))(bool))

#define __step__cmp_fn_ord$(...) __cmp_fn_ord$(__VA_ARGS__)
#define __step__cmp_fn_ord$__parse(_T...) _T, __step__cmp_fn_ord$__parseNext
#define __step__cmp_fn_ord$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_ord$(_T, _lhs, _rhs...) \
    fn_((cmp_ord$(_T)(_T _lhs, _T _rhs))(cmp_Ord))
#define __step__cmp_fn_eq$(...) __cmp_fn_eq$(__VA_ARGS__)
#define __step__cmp_fn_eq$__parse(_T...) _T, __step__cmp_fn_eq$__parseNext
#define __step__cmp_fn_eq$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_eq$(_T, _lhs, _rhs...) \
    fn_((cmp_eq$(_T)(_T _lhs, _T _rhs))(bool))
#define __step__cmp_fn_ne$(...) __cmp_fn_ne$(__VA_ARGS__)
#define __step__cmp_fn_ne$__parse(_T...) _T, __step__cmp_fn_ne$__parseNext
#define __step__cmp_fn_ne$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_ne$(_T, _lhs, _rhs...) \
    fn_((cmp_ne$(_T)(_T _lhs, _T _rhs))(bool))
#define __step__cmp_fn_lt$(...) __cmp_fn_lt$(__VA_ARGS__)
#define __step__cmp_fn_lt$__parse(_T...) _T, __step__cmp_fn_lt$__parseNext
#define __step__cmp_fn_lt$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_lt$(_T, _lhs, _rhs...) \
    fn_((cmp_lt$(_T)(_T _lhs, _T _rhs))(bool))
#define __step__cmp_fn_gt$(...) __cmp_fn_gt$(__VA_ARGS__)
#define __step__cmp_fn_gt$__parse(_T...) _T, __step__cmp_fn_gt$__parseNext
#define __step__cmp_fn_gt$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_gt$(_T, _lhs, _rhs...) \
    fn_((cmp_gt$(_T)(_T _lhs, _T _rhs))(bool))
#define __step__cmp_fn_le$(...) __cmp_fn_le$(__VA_ARGS__)
#define __step__cmp_fn_le$__parse(_T...) _T, __step__cmp_fn_le$__parseNext
#define __step__cmp_fn_le$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_le$(_T, _lhs, _rhs...) \
    fn_((cmp_le$(_T)(_T _lhs, _T _rhs))(bool))
#define __step__cmp_fn_ge$(...) __cmp_fn_ge$(__VA_ARGS__)
#define __step__cmp_fn_ge$__parse(_T...) _T, __step__cmp_fn_ge$__parseNext
#define __step__cmp_fn_ge$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_ge$(_T, _lhs, _rhs...) \
    fn_((cmp_ge$(_T)(_T _lhs, _T _rhs))(bool))

#define __step__cmp_fn_ordCtx$(...) __cmp_fn_ordCtx$(__VA_ARGS__)
#define __step__cmp_fn_ordCtx$__parse(_T...) _T, __step__cmp_fn_ordCtx$__parseNext
#define __step__cmp_fn_ordCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_ordCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_ordCtx$(_T)(_T _lhs, _T _rhs, u_V$raw _ctx))(cmp_Ord))
#define __step__cmp_fn_eqCtx$(...) __cmp_fn_eqCtx$(__VA_ARGS__)
#define __step__cmp_fn_eqCtx$__parse(_T...) _T, __step__cmp_fn_eqCtx$__parseNext
#define __step__cmp_fn_eqCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_eqCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_eqCtx$(_T)(_T _lhs, _T _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_neCtx$(...) __cmp_fn_neCtx$(__VA_ARGS__)
#define __step__cmp_fn_neCtx$__parse(_T...) _T, __step__cmp_fn_neCtx$__parseNext
#define __step__cmp_fn_neCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_neCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_neCtx$(_T)(_T _lhs, _T _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_ltCtx$(...) __cmp_fn_ltCtx$(__VA_ARGS__)
#define __step__cmp_fn_ltCtx$__parse(_T...) _T, __step__cmp_fn_ltCtx$__parseNext
#define __step__cmp_fn_ltCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_ltCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_ltCtx$(_T)(_T _lhs, _T _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_gtCtx$(...) __cmp_fn_gtCtx$(__VA_ARGS__)
#define __step__cmp_fn_gtCtx$__parse(_T...) _T, __step__cmp_fn_gtCtx$__parseNext
#define __step__cmp_fn_gtCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_gtCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_gtCtx$(_T)(_T _lhs, _T _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_leCtx$(...) __cmp_fn_leCtx$(__VA_ARGS__)
#define __step__cmp_fn_leCtx$__parse(_T...) _T, __step__cmp_fn_leCtx$__parseNext
#define __step__cmp_fn_leCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_leCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_leCtx$(_T)(_T _lhs, _T _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_geCtx$(...) __cmp_fn_geCtx$(__VA_ARGS__)
#define __step__cmp_fn_geCtx$__parse(_T...) _T, __step__cmp_fn_geCtx$__parseNext
#define __step__cmp_fn_geCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_geCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_geCtx$(_T)(_T _lhs, _T _rhs, u_V$raw _ctx))(bool))

#define __step__cmp_fn_ordApx$(...) __cmp_fn_ordApx$(__VA_ARGS__)
#define __step__cmp_fn_ordApx$__parse(_T...) _T, __step__cmp_fn_ordApx$__parseNext
#define __step__cmp_fn_ordApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_ordApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_ordApx$(_T)(_T _lhs, _T _rhs, _T _threshold))(cmp_Ord))
#define __step__cmp_fn_eqApx$(...) __cmp_fn_eqApx$(__VA_ARGS__)
#define __step__cmp_fn_eqApx$__parse(_T...) _T, __step__cmp_fn_eqApx$__parseNext
#define __step__cmp_fn_eqApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_eqApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_eqApx$(_T)(_T _lhs, _T _rhs, _T _threshold))(bool))
#define __step__cmp_fn_neApx$(...) __cmp_fn_neApx$(__VA_ARGS__)
#define __step__cmp_fn_neApx$__parse(_T...) _T, __step__cmp_fn_neApx$__parseNext
#define __step__cmp_fn_neApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_neApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_neApx$(_T)(_T _lhs, _T _rhs, _T _threshold))(bool))
#define __step__cmp_fn_ltApx$(...) __cmp_fn_ltApx$(__VA_ARGS__)
#define __step__cmp_fn_ltApx$__parse(_T...) _T, __step__cmp_fn_ltApx$__parseNext
#define __step__cmp_fn_ltApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_ltApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_ltApx$(_T)(_T _lhs, _T _rhs, _T _threshold))(bool))
#define __step__cmp_fn_gtApx$(...) __cmp_fn_gtApx$(__VA_ARGS__)
#define __step__cmp_fn_gtApx$__parse(_T...) _T, __step__cmp_fn_gtApx$__parseNext
#define __step__cmp_fn_gtApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_gtApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_gtApx$(_T)(_T _lhs, _T _rhs, _T _threshold))(bool))
#define __step__cmp_fn_leApx$(...) __cmp_fn_leApx$(__VA_ARGS__)
#define __step__cmp_fn_leApx$__parse(_T...) _T, __step__cmp_fn_leApx$__parseNext
#define __step__cmp_fn_leApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_leApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_leApx$(_T)(_T _lhs, _T _rhs, _T _threshold))(bool))
#define __step__cmp_fn_geApx$(...) __cmp_fn_geApx$(__VA_ARGS__)
#define __step__cmp_fn_geApx$__parse(_T...) _T, __step__cmp_fn_geApx$__parseNext
#define __step__cmp_fn_geApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_geApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_geApx$(_T)(_T _lhs, _T _rhs, _T _threshold))(bool))

#define __step__cmp_fn_u_eql$(...) __cmp_fn_u_eql$(__VA_ARGS__)
#define __step__cmp_fn_u_eql$__parse(_T...) _T, __step__cmp_fn_u_eql$__parseNext
#define __step__cmp_fn_u_eql$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_eql$(_T, _lhs, _rhs...) \
    fn_((cmp_u_eql$(_T)(u_V$raw _lhs, u_V$raw _rhs))(bool))
#define __step__cmp_fn_u_neq$(...) __cmp_fn_u_neq$(__VA_ARGS__)
#define __step__cmp_fn_u_neq$__parse(_T...) _T, __step__cmp_fn_u_neq$__parseNext
#define __step__cmp_fn_u_neq$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_neq$(_T, _lhs, _rhs...) \
    fn_((cmp_u_neq$(_T)(u_V$raw _lhs, u_V$raw _rhs))(bool))

#define __step__cmp_fn_u_eqlCtx$(...) __cmp_fn_u_eqlCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_eqlCtx$__parse(_T...) _T, __step__cmp_fn_u_eqlCtx$__parseNext
#define __step__cmp_fn_u_eqlCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_eqlCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_u_eqlCtx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_u_neqCtx$(...) __cmp_fn_u_neqCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_neqCtx$__parse(_T...) _T, __step__cmp_fn_u_neqCtx$__parseNext
#define __step__cmp_fn_u_neqCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_neqCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_u_neqCtx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _ctx))(bool))

#define __step__cmp_fn_u_ord$(...) __cmp_fn_u_ord$(__VA_ARGS__)
#define __step__cmp_fn_u_ord$__parse(_T...) _T, __step__cmp_fn_u_ord$__parseNext
#define __step__cmp_fn_u_ord$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_ord$(_T, _lhs, _rhs...) \
    fn_((cmp_u_ord$(_T)(u_V$raw _lhs, u_V$raw _rhs))(cmp_Ord))
#define __step__cmp_fn_u_eq$(...) __cmp_fn_u_eq$(__VA_ARGS__)
#define __step__cmp_fn_u_eq$__parse(_T...) _T, __step__cmp_fn_u_eq$__parseNext
#define __step__cmp_fn_u_eq$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_eq$(_T, _lhs, _rhs...) \
    fn_((cmp_u_eq$(_T)(u_V$raw _lhs, u_V$raw _rhs))(bool))
#define __step__cmp_fn_u_ne$(...) __cmp_fn_u_ne$(__VA_ARGS__)
#define __step__cmp_fn_u_ne$__parse(_T...) _T, __step__cmp_fn_u_ne$__parseNext
#define __step__cmp_fn_u_ne$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_ne$(_T, _lhs, _rhs...) \
    fn_((cmp_u_ne$(_T)(u_V$raw _lhs, u_V$raw _rhs))(bool))
#define __step__cmp_fn_u_lt$(...) __cmp_fn_u_lt$(__VA_ARGS__)
#define __step__cmp_fn_u_lt$__parse(_T...) _T, __step__cmp_fn_u_lt$__parseNext
#define __step__cmp_fn_u_lt$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_lt$(_T, _lhs, _rhs...) \
    fn_((cmp_u_lt$(_T)(u_V$raw _lhs, u_V$raw _rhs))(bool))
#define __step__cmp_fn_u_gt$(...) __cmp_fn_u_gt$(__VA_ARGS__)
#define __step__cmp_fn_u_gt$__parse(_T...) _T, __step__cmp_fn_u_gt$__parseNext
#define __step__cmp_fn_u_gt$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_gt$(_T, _lhs, _rhs...) \
    fn_((cmp_u_gt$(_T)(u_V$raw _lhs, u_V$raw _rhs))(bool))
#define __step__cmp_fn_u_le$(...) __cmp_fn_u_le$(__VA_ARGS__)
#define __step__cmp_fn_u_le$__parse(_T...) _T, __step__cmp_fn_u_le$__parseNext
#define __step__cmp_fn_u_le$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_le$(_T, _lhs, _rhs...) \
    fn_((cmp_u_le$(_T)(u_V$raw _lhs, u_V$raw _rhs))(bool))
#define __step__cmp_fn_u_ge$(...) __cmp_fn_u_ge$(__VA_ARGS__)
#define __step__cmp_fn_u_ge$__parse(_T...) _T, __step__cmp_fn_u_ge$__parseNext
#define __step__cmp_fn_u_ge$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_ge$(_T, _lhs, _rhs...) \
    fn_((cmp_u_ge$(_T)(u_V$raw _lhs, u_V$raw _rhs))(bool))

#define __step__cmp_fn_u_ordCtx$(...) __cmp_fn_u_ordCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_ordCtx$__parse(_T...) _T, __step__cmp_fn_u_ordCtx$__parseNext
#define __step__cmp_fn_u_ordCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_ordCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_u_ordCtx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _ctx))(cmp_Ord))
#define __step__cmp_fn_u_eqCtx$(...) __cmp_fn_u_eqCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_eqCtx$__parse(_T...) _T, __step__cmp_fn_u_eqCtx$__parseNext
#define __step__cmp_fn_u_eqCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_eqCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_u_eqCtx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_u_neCtx$(...) __cmp_fn_u_neCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_neCtx$__parse(_T...) _T, __step__cmp_fn_u_neCtx$__parseNext
#define __step__cmp_fn_u_neCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_neCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_u_neCtx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_u_ltCtx$(...) __cmp_fn_u_ltCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_ltCtx$__parse(_T...) _T, __step__cmp_fn_u_ltCtx$__parseNext
#define __step__cmp_fn_u_ltCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_ltCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_u_ltCtx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_u_gtCtx$(...) __cmp_fn_u_gtCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_gtCtx$__parse(_T...) _T, __step__cmp_fn_u_gtCtx$__parseNext
#define __step__cmp_fn_u_gtCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_gtCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_u_gtCtx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_u_leCtx$(...) __cmp_fn_u_leCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_leCtx$__parse(_T...) _T, __step__cmp_fn_u_leCtx$__parseNext
#define __step__cmp_fn_u_leCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_leCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_u_leCtx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _ctx))(bool))
#define __step__cmp_fn_u_geCtx$(...) __cmp_fn_u_geCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_geCtx$__parse(_T...) _T, __step__cmp_fn_u_geCtx$__parseNext
#define __step__cmp_fn_u_geCtx$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_geCtx$(_T, _lhs, _rhs, _ctx...) \
    fn_((cmp_u_geCtx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _ctx))(bool))

#define __step__cmp_fn_u_ordApx$(...) __cmp_fn_u_ordApx$(__VA_ARGS__)
#define __step__cmp_fn_u_ordApx$__parse(_T...) _T, __step__cmp_fn_u_ordApx$__parseNext
#define __step__cmp_fn_u_ordApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_ordApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_u_ordApx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _threshold))(cmp_Ord))
#define __step__cmp_fn_u_eqApx$(...) __cmp_fn_u_eqApx$(__VA_ARGS__)
#define __step__cmp_fn_u_eqApx$__parse(_T...) _T, __step__cmp_fn_u_eqApx$__parseNext
#define __step__cmp_fn_u_eqApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_eqApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_u_eqApx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _threshold))(bool))
#define __step__cmp_fn_u_neApx$(...) __cmp_fn_u_neApx$(__VA_ARGS__)
#define __step__cmp_fn_u_neApx$__parse(_T...) _T, __step__cmp_fn_u_neApx$__parseNext
#define __step__cmp_fn_u_neApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_neApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_u_neApx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _threshold))(bool))
#define __step__cmp_fn_u_ltApx$(...) __cmp_fn_u_ltApx$(__VA_ARGS__)
#define __step__cmp_fn_u_ltApx$__parse(_T...) _T, __step__cmp_fn_u_ltApx$__parseNext
#define __step__cmp_fn_u_ltApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_ltApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_u_ltApx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _threshold))(bool))
#define __step__cmp_fn_u_gtApx$(...) __cmp_fn_u_gtApx$(__VA_ARGS__)
#define __step__cmp_fn_u_gtApx$__parse(_T...) _T, __step__cmp_fn_u_gtApx$__parseNext
#define __step__cmp_fn_u_gtApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_gtApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_u_gtApx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _threshold))(bool))
#define __step__cmp_fn_u_leApx$(...) __cmp_fn_u_leApx$(__VA_ARGS__)
#define __step__cmp_fn_u_leApx$__parse(_T...) _T, __step__cmp_fn_u_leApx$__parseNext
#define __step__cmp_fn_u_leApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_leApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_u_leApx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _threshold))(bool))
#define __step__cmp_fn_u_geApx$(...) __cmp_fn_u_geApx$(__VA_ARGS__)
#define __step__cmp_fn_u_geApx$__parse(_T...) _T, __step__cmp_fn_u_geApx$__parseNext
#define __step__cmp_fn_u_geApx$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_geApx$(_T, _lhs, _rhs, _threshold...) \
    fn_((cmp_u_geApx$(_T)(u_V$raw _lhs, u_V$raw _rhs, u_V$raw _threshold))(bool))

#define __step__cmp_fn_eql_default$(...) __cmp_fn_eql_default$(__VA_ARGS__)
#define __step__cmp_fn_eql_default$__parse(_T...) _T, __step__cmp_fn_eql_default$__parseNext
#define __step__cmp_fn_eql_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_eql_default$(_T, _lhs, _rhs...) \
    cmp_fn_(eql)((_T)(_lhs, _rhs)) { \
        return !cmp_neq$(_T)(_lhs, _rhs); \
    }
#define __step__cmp_fn_neq_default$(...) __cmp_fn_neq_default$(__VA_ARGS__)
#define __step__cmp_fn_neq_default$__parse(_T...) _T, __step__cmp_fn_neq_default$__parseNext
#define __step__cmp_fn_neq_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_neq_default$(_T, _lhs, _rhs...) \
    cmp_fn_(neq)((_T)(_lhs, _rhs)) { \
        return !cmp_eql$(_T)(_lhs, _rhs); \
    }

#define __step__cmp_fn_eqlCtx_default$(...) __cmp_fn_eqlCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_eqlCtx_default$__parse(_T...) _T, __step__cmp_fn_eqlCtx_default$__parseNext
#define __step__cmp_fn_eqlCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_eqlCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(eqlCtx)((_T)(_lhs, _rhs, _ctx)) { \
        return !cmp_neqCtx$(_T)(_lhs, _rhs, _ctx); \
    }
#define __step__cmp_fn_neqCtx_default$(...) __cmp_fn_neqCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_neqCtx_default$__parse(_T...) _T, __step__cmp_fn_neqCtx_default$__parseNext
#define __step__cmp_fn_neqCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_neqCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(neqCtx)((_T)(_lhs, _rhs, _ctx)) { \
        return !cmp_eqlCtx$(_T)(_lhs, _rhs, _ctx); \
    }

#define __step__cmp_fn_ord_default$(...) __cmp_fn_ord_default$(__VA_ARGS__)
#define __step__cmp_fn_ord_default$__parse(_T...) _T, __step__cmp_fn_ord_default$__parseNext
#define __step__cmp_fn_ord_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_ord_default$(_T, _lhs, _rhs...) \
    cmp_fn_(ord)((_T)(_lhs, _rhs)) { \
        return cmp_lt$(_T)(_lhs, _rhs) ? cmp_Ord_lt \
             : cmp_lt$(_T)(_rhs, _lhs) ? cmp_Ord_gt \
                                       : cmp_Ord_eq; \
    }
#define __step__cmp_fn_eq_default$(...) __cmp_fn_eq_default$(__VA_ARGS__)
#define __step__cmp_fn_eq_default$__parse(_T...) _T, __step__cmp_fn_eq_default$__parseNext
#define __step__cmp_fn_eq_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_eq_default$(_T, _lhs, _rhs...) \
    cmp_fn_(eq)((_T)(_lhs, _rhs)) { \
        return cmp_ord$(_T)(_lhs, _rhs) == cmp_Ord_eq; \
    }
#define __step__cmp_fn_ne_default$(...) __cmp_fn_ne_default$(__VA_ARGS__)
#define __step__cmp_fn_ne_default$__parse(_T...) _T, __step__cmp_fn_ne_default$__parseNext
#define __step__cmp_fn_ne_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_ne_default$(_T, _lhs, _rhs...) \
    cmp_fn_(ne)((_T)(_lhs, _rhs)) { \
        return cmp_ord$(_T)(_lhs, _rhs) != cmp_Ord_eq; \
    }
#define __step__cmp_fn_lt_default$(...) __cmp_fn_lt_default$(__VA_ARGS__)
#define __step__cmp_fn_lt_default$__parse(_T...) _T, __step__cmp_fn_lt_default$__parseNext
#define __step__cmp_fn_lt_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_lt_default$(_T, _lhs, _rhs...) \
    cmp_fn_(lt)((_T)(_lhs, _rhs)) { \
        return cmp_ord$(_T)(_lhs, _rhs) < cmp_Ord_eq; \
    }
#define __step__cmp_fn_gt_default$(...) __cmp_fn_gt_default$(__VA_ARGS__)
#define __step__cmp_fn_gt_default$__parse(_T...) _T, __step__cmp_fn_gt_default$__parseNext
#define __step__cmp_fn_gt_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_gt_default$(_T, _lhs, _rhs...) \
    cmp_fn_(gt)((_T)(_lhs, _rhs)) { \
        return cmp_ord$(_T)(_lhs, _rhs) > cmp_Ord_eq; \
    }
#define __step__cmp_fn_le_default$(...) __cmp_fn_le_default$(__VA_ARGS__)
#define __step__cmp_fn_le_default$__parse(_T...) _T, __step__cmp_fn_le_default$__parseNext
#define __step__cmp_fn_le_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_le_default$(_T, _lhs, _rhs...) \
    cmp_fn_(le)((_T)(_lhs, _rhs)) { \
        return cmp_ord$(_T)(_lhs, _rhs) <= cmp_Ord_eq; \
    }
#define __step__cmp_fn_ge_default$(...) __cmp_fn_ge_default$(__VA_ARGS__)
#define __step__cmp_fn_ge_default$__parse(_T...) _T, __step__cmp_fn_ge_default$__parseNext
#define __step__cmp_fn_ge_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_ge_default$(_T, _lhs, _rhs...) \
    cmp_fn_(ge)((_T)(_lhs, _rhs)) { \
        return cmp_ord$(_T)(_lhs, _rhs) >= cmp_Ord_eq; \
    }

#define __step__cmp_fn_ordCtx_default$(...) __cmp_fn_ordCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_ordCtx_default$__parse(_T...) _T, __step__cmp_fn_ordCtx_default$__parseNext
#define __step__cmp_fn_ordCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_ordCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(ordCtx)((_T)(_lhs, _rhs, _ctx)) { \
        return cmp_ltCtx$(_T)(_lhs, _rhs, _ctx) ? cmp_Ord_lt \
             : cmp_ltCtx$(_T)(_rhs, _lhs, _ctx) ? cmp_Ord_gt \
                                                : cmp_Ord_eq; \
    }
#define __step__cmp_fn_eqCtx_default$(...) __cmp_fn_eqCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_eqCtx_default$__parse(_T...) _T, __step__cmp_fn_eqCtx_default$__parseNext
#define __step__cmp_fn_eqCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_eqCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(eqCtx)((_T)(_lhs, _rhs, _ctx)) { \
        return cmp_ordCtx$(_T)(_lhs, _rhs, _ctx) == cmp_Ord_eq; \
    }
#define __step__cmp_fn_neCtx_default$(...) __cmp_fn_neCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_neCtx_default$__parse(_T...) _T, __step__cmp_fn_neCtx_default$__parseNext
#define __step__cmp_fn_neCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_neCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(neCtx)((_T)(_lhs, _rhs, _ctx)) { \
        return cmp_ordCtx$(_T)(_lhs, _rhs, _ctx) != cmp_Ord_eq; \
    }
#define __step__cmp_fn_ltCtx_default$(...) __cmp_fn_ltCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_ltCtx_default$__parse(_T...) _T, __step__cmp_fn_ltCtx_default$__parseNext
#define __step__cmp_fn_ltCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_ltCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(ltCtx)((_T)(_lhs, _rhs, _ctx)) { \
        return cmp_ordCtx$(_T)(_lhs, _rhs, _ctx) < cmp_Ord_eq; \
    }
#define __step__cmp_fn_gtCtx_default$(...) __cmp_fn_gtCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_gtCtx_default$__parse(_T...) _T, __step__cmp_fn_gtCtx_default$__parseNext
#define __step__cmp_fn_gtCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_gtCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(gtCtx)((_T)(_lhs, _rhs, _ctx)) { \
        return cmp_ordCtx$(_T)(_lhs, _rhs, _ctx) > cmp_Ord_eq; \
    }
#define __step__cmp_fn_leCtx_default$(...) __cmp_fn_leCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_leCtx_default$__parse(_T...) _T, __step__cmp_fn_leCtx_default$__parseNext
#define __step__cmp_fn_leCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_leCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(leCtx)((_T)(_lhs, _rhs, _ctx)) { \
        return cmp_ordCtx$(_T)(_lhs, _rhs, _ctx) <= cmp_Ord_eq; \
    }
#define __step__cmp_fn_geCtx_default$(...) __cmp_fn_geCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_geCtx_default$__parse(_T...) _T, __step__cmp_fn_geCtx_default$__parseNext
#define __step__cmp_fn_geCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_geCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(geCtx)((_T)(_lhs, _rhs, _ctx)) { \
        return cmp_ordCtx$(_T)(_lhs, _rhs, _ctx) >= cmp_Ord_eq; \
    }

#define __step__cmp_fn_ordApx_default$(...) __cmp_fn_ordApx_default$(__VA_ARGS__)
#define __step__cmp_fn_ordApx_default$__parse(_T...) _T, __step__cmp_fn_ordApx_default$__parseNext
#define __step__cmp_fn_ordApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_ordApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(ordApx)((_T)(_lhs, _rhs, _threshold)) { \
        return cmp_ltApx$(_T)(_lhs, _rhs, _threshold) ? cmp_Ord_lt \
             : cmp_ltApx$(_T)(_rhs, _lhs, _threshold) ? cmp_Ord_gt \
                                                      : cmp_Ord_eq; \
    }
#define __step__cmp_fn_eqApx_default$(...) __cmp_fn_eqApx_default$(__VA_ARGS__)
#define __step__cmp_fn_eqApx_default$__parse(_T...) _T, __step__cmp_fn_eqApx_default$__parseNext
#define __step__cmp_fn_eqApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_eqApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(eqApx)((_T)(_lhs, _rhs, _threshold)) { \
        return cmp_ordApx$(_T)(_lhs, _rhs, _threshold) == cmp_Ord_eq; \
    }
#define __step__cmp_fn_neApx_default$(...) __cmp_fn_neApx_default$(__VA_ARGS__)
#define __step__cmp_fn_neApx_default$__parse(_T...) _T, __step__cmp_fn_neApx_default$__parseNext
#define __step__cmp_fn_neApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_neApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(neApx)((_T)(_lhs, _rhs, _threshold)) { \
        return cmp_ordApx$(_T)(_lhs, _rhs, _threshold) != cmp_Ord_eq; \
    }
#define __step__cmp_fn_ltApx_default$(...) __cmp_fn_ltApx_default$(__VA_ARGS__)
#define __step__cmp_fn_ltApx_default$__parse(_T...) _T, __step__cmp_fn_ltApx_default$__parseNext
#define __step__cmp_fn_ltApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_ltApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(ltApx)((_T)(_lhs, _rhs, _threshold)) { \
        return cmp_ordApx$(_T)(_lhs, _rhs, _threshold) < cmp_Ord_eq; \
    }
#define __step__cmp_fn_gtApx_default$(...) __cmp_fn_gtApx_default$(__VA_ARGS__)
#define __step__cmp_fn_gtApx_default$__parse(_T...) _T, __step__cmp_fn_gtApx_default$__parseNext
#define __step__cmp_fn_gtApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_gtApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(gtApx)((_T)(_lhs, _rhs, _threshold)) { \
        return cmp_ordApx$(_T)(_lhs, _rhs, _threshold) > cmp_Ord_eq; \
    }
#define __step__cmp_fn_leApx_default$(...) __cmp_fn_leApx_default$(__VA_ARGS__)
#define __step__cmp_fn_leApx_default$__parse(_T...) _T, __step__cmp_fn_leApx_default$__parseNext
#define __step__cmp_fn_leApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_leApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(leApx)((_T)(_lhs, _rhs, _threshold)) { \
        return cmp_ordApx$(_T)(_lhs, _rhs, _threshold) <= cmp_Ord_eq; \
    }
#define __step__cmp_fn_geApx_default$(...) __cmp_fn_geApx_default$(__VA_ARGS__)
#define __step__cmp_fn_geApx_default$__parse(_T...) _T, __step__cmp_fn_geApx_default$__parseNext
#define __step__cmp_fn_geApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_geApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(geApx)((_T)(_lhs, _rhs, _threshold)) { \
        return cmp_ordApx$(_T)(_lhs, _rhs, _threshold) >= cmp_Ord_eq; \
    }

#define __step__cmp_fn_u_eql_default$(...) __cmp_fn_u_eql_default$(__VA_ARGS__)
#define __step__cmp_fn_u_eql_default$__parse(_T...) _T, __step__cmp_fn_u_eql_default$__parseNext
#define __step__cmp_fn_u_eql_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_eql_default$(_T, _lhs, _rhs...) \
    cmp_fn_(u_eql)((_T)(_lhs, _rhs)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_eql$(_T)(l, r); \
    }
#define __step__cmp_fn_u_neq_default$(...) __cmp_fn_u_neq_default$(__VA_ARGS__)
#define __step__cmp_fn_u_neq_default$__parse(_T...) _T, __step__cmp_fn_u_neq_default$__parseNext
#define __step__cmp_fn_u_neq_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_neq_default$(_T, _lhs, _rhs...) \
    cmp_fn_(u_neq)((_T)(_lhs, _rhs)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_neq$(_T)(l, r); \
    }

#define __step__cmp_fn_u_eqlCtx_default$(...) __cmp_fn_u_eqlCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_eqlCtx_default$__parse(_T...) _T, __step__cmp_fn_u_eqlCtx_default$__parseNext
#define __step__cmp_fn_u_eqlCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_eqlCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(u_eqlCtx)((_T)(_lhs, _rhs, _ctx)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_eqlCtx$(_T)(l, r, _ctx); \
    }
#define __step__cmp_fn_u_neqCtx_default$(...) __cmp_fn_u_neqCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_neqCtx_default$__parse(_T...) _T, __step__cmp_fn_u_neqCtx_default$__parseNext
#define __step__cmp_fn_u_neqCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_neqCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(u_neqCtx)((_T)(_lhs, _rhs, _ctx)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_neqCtx$(_T)(l, r, _ctx); \
    }

#define __step__cmp_fn_u_ord_default$(...) __cmp_fn_u_ord_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ord_default$__parse(_T...) _T, __step__cmp_fn_u_ord_default$__parseNext
#define __step__cmp_fn_u_ord_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_ord_default$(_T, _lhs, _rhs...) \
    cmp_fn_(u_ord)((_T)(_lhs, _rhs)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_ord$(_T)(l, r); \
    }
#define __step__cmp_fn_u_eq_default$(...) __cmp_fn_u_eq_default$(__VA_ARGS__)
#define __step__cmp_fn_u_eq_default$__parse(_T...) _T, __step__cmp_fn_u_eq_default$__parseNext
#define __step__cmp_fn_u_eq_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_eq_default$(_T, _lhs, _rhs...) \
    cmp_fn_(u_eq)((_T)(_lhs, _rhs)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_eq$(_T)(l, r); \
    }
#define __step__cmp_fn_u_ne_default$(...) __cmp_fn_u_ne_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ne_default$__parse(_T...) _T, __step__cmp_fn_u_ne_default$__parseNext
#define __step__cmp_fn_u_ne_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_ne_default$(_T, _lhs, _rhs...) \
    cmp_fn_(u_ne)((_T)(_lhs, _rhs)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_ne$(_T)(l, r); \
    }
#define __step__cmp_fn_u_lt_default$(...) __cmp_fn_u_lt_default$(__VA_ARGS__)
#define __step__cmp_fn_u_lt_default$__parse(_T...) _T, __step__cmp_fn_u_lt_default$__parseNext
#define __step__cmp_fn_u_lt_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_lt_default$(_T, _lhs, _rhs...) \
    cmp_fn_(u_lt)((_T)(_lhs, _rhs)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_lt$(_T)(l, r); \
    }
#define __step__cmp_fn_u_gt_default$(...) __cmp_fn_u_gt_default$(__VA_ARGS__)
#define __step__cmp_fn_u_gt_default$__parse(_T...) _T, __step__cmp_fn_u_gt_default$__parseNext
#define __step__cmp_fn_u_gt_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_gt_default$(_T, _lhs, _rhs...) \
    cmp_fn_(u_gt)((_T)(_lhs, _rhs)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_gt$(_T)(l, r); \
    }
#define __step__cmp_fn_u_le_default$(...) __cmp_fn_u_le_default$(__VA_ARGS__)
#define __step__cmp_fn_u_le_default$__parse(_T...) _T, __step__cmp_fn_u_le_default$__parseNext
#define __step__cmp_fn_u_le_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_le_default$(_T, _lhs, _rhs...) \
    cmp_fn_(u_le)((_T)(_lhs, _rhs)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_le$(_T)(l, r); \
    }
#define __step__cmp_fn_u_ge_default$(...) __cmp_fn_u_ge_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ge_default$__parse(_T...) _T, __step__cmp_fn_u_ge_default$__parseNext
#define __step__cmp_fn_u_ge_default$__parseNext(_lhs, _rhs...) _lhs, _rhs
#define __cmp_fn_u_ge_default$(_T, _lhs, _rhs...) \
    cmp_fn_(u_ge)((_T)(_lhs, _rhs)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_ge$(_T)(l, r); \
    }

#define __step__cmp_fn_u_ordCtx_default$(...) __cmp_fn_u_ordCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ordCtx_default$__parse(_T...) _T, __step__cmp_fn_u_ordCtx_default$__parseNext
#define __step__cmp_fn_u_ordCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_ordCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(u_ordCtx)((_T)(_lhs, _rhs, _ctx)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_ordCtx$(_T)(l, r, _ctx); \
    }
#define __step__cmp_fn_u_eqCtx_default$(...) __cmp_fn_u_eqCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_eqCtx_default$__parse(_T...) _T, __step__cmp_fn_u_eqCtx_default$__parseNext
#define __step__cmp_fn_u_eqCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_eqCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(u_eqCtx)((_T)(_lhs, _rhs, _ctx)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_eqCtx$(_T)(l, r, _ctx); \
    }
#define __step__cmp_fn_u_neCtx_default$(...) __cmp_fn_u_neCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_neCtx_default$__parse(_T...) _T, __step__cmp_fn_u_neCtx_default$__parseNext
#define __step__cmp_fn_u_neCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_neCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(u_neqCtx)((_T)(_lhs, _rhs, _ctx)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_neqCtx$(_T)(l, r, _ctx); \
    }
#define __step__cmp_fn_u_ltCtx_default$(...) __cmp_fn_u_ltCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ltCtx_default$__parse(_T...) _T, __step__cmp_fn_u_ltCtx_default$__parseNext
#define __step__cmp_fn_u_ltCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_ltCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(u_ltCtx)((_T)(_lhs, _rhs, _ctx)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_ltCtx$(_T)(l, r, _ctx); \
    }
#define __step__cmp_fn_u_gtCtx_default$(...) __cmp_fn_u_gtCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_gtCtx_default$__parse(_T...) _T, __step__cmp_fn_u_gtCtx_default$__parseNext
#define __step__cmp_fn_u_gtCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_gtCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(u_gtCtx)((_T)(_lhs, _rhs, _ctx)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_gtCtx$(_T)(l, r, _ctx); \
    }
#define __step__cmp_fn_u_leCtx_default$(...) __cmp_fn_u_leCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_leCtx_default$__parse(_T...) _T, __step__cmp_fn_u_leCtx_default$__parseNext
#define __step__cmp_fn_u_leCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_leCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(u_leCtx)((_T)(_lhs, _rhs, _ctx)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_leCtx$(_T)(l, r, _ctx); \
    }
#define __step__cmp_fn_u_geCtx_default$(...) __cmp_fn_u_geCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_geCtx_default$__parse(_T...) _T, __step__cmp_fn_u_geCtx_default$__parseNext
#define __step__cmp_fn_u_geCtx_default$__parseNext(_lhs, _rhs, _ctx...) _lhs, _rhs, _ctx
#define __cmp_fn_u_geCtx_default$(_T, _lhs, _rhs, _ctx...) \
    cmp_fn_(u_geCtx)((_T)(_lhs, _rhs, _ctx)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        return cmp_geCtx$(_T)(l, r, _ctx); \
    }

#define __step__cmp_fn_u_ordApx_default$(...) __cmp_fn_u_ordApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ordApx_default$__parse(_T...) _T, __step__cmp_fn_u_ordApx_default$__parseNext
#define __step__cmp_fn_u_ordApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_ordApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(u_ordApx)((_T)(_lhs, _rhs, _threshold)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        let t = u_castV$((_T)(_threshold)); \
        return cmp_ordApx$(_T)(l, r, t); \
    }
#define __step__cmp_fn_u_eqApx_default$(...) __cmp_fn_u_eqApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_eqApx_default$__parse(_T...) _T, __step__cmp_fn_u_eqApx_default$__parseNext
#define __step__cmp_fn_u_eqApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_eqApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(u_eqApx)((_T)(_lhs, _rhs, _threshold)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        let t = u_castV$((_T)(_threshold)); \
        return cmp_eqApx$(_T)(l, r, t); \
    }
#define __step__cmp_fn_u_neApx_default$(...) __cmp_fn_u_neApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_neApx_default$__parse(_T...) _T, __step__cmp_fn_u_neApx_default$__parseNext
#define __step__cmp_fn_u_neApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_neApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(u_neApx)((_T)(_lhs, _rhs, _threshold)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        let t = u_castV$((_T)(_threshold)); \
        return cmp_neApx$(_T)(l, r, t); \
    }
#define __step__cmp_fn_u_ltApx_default$(...) __cmp_fn_u_ltApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ltApx_default$__parse(_T...) _T, __step__cmp_fn_u_ltApx_default$__parseNext
#define __step__cmp_fn_u_ltApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_ltApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(u_ltApx)((_T)(_lhs, _rhs, _threshold)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        let t = u_castV$((_T)(_threshold)); \
        return cmp_ltApx$(_T)(l, r, t); \
    }
#define __step__cmp_fn_u_gtApx_default$(...) __cmp_fn_u_gtApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_gtApx_default$__parse(_T...) _T, __step__cmp_fn_u_gtApx_default$__parseNext
#define __step__cmp_fn_u_gtApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_gtApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(u_gtApx)((_T)(_lhs, _rhs, _threshold)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        let t = u_castV$((_T)(_threshold)); \
        return cmp_gtApx$(_T)(l, r, t); \
    }
#define __step__cmp_fn_u_leApx_default$(...) __cmp_fn_u_leApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_leApx_default$__parse(_T...) _T, __step__cmp_fn_u_leApx_default$__parseNext
#define __step__cmp_fn_u_leApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_leApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(u_leApx)((_T)(_lhs, _rhs, _threshold)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        let t = u_castV$((_T)(_threshold)); \
        return cmp_leApx$(_T)(l, r, t); \
    }
#define __step__cmp_fn_u_geApx_default$(...) __cmp_fn_u_geApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_geApx_default$__parse(_T...) _T, __step__cmp_fn_u_geApx_default$__parseNext
#define __step__cmp_fn_u_geApx_default$__parseNext(_lhs, _rhs, _threshold...) _lhs, _rhs, _threshold
#define __cmp_fn_u_geApx_default$(_T, _lhs, _rhs, _threshold...) \
    cmp_fn_(u_geApx)((_T)(_lhs, _rhs, _threshold)) { \
        let l = u_castV$((_T)(_lhs)); \
        let r = u_castV$((_T)(_rhs)); \
        let t = u_castV$((_T)(_threshold)); \
        return cmp_geApx$(_T)(l, r, t); \
    }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_cmp__included */
