/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cmp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-24 (date of creation)
 * @updated 2026-06-21 (date of last update)
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
#pragma once
#ifndef core_cmp__included
#define core_cmp__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pri.h"

/*========== Macros and Declarations ========================================*/

/* --- Equality Operators --- */

#define cmp_eql$(_$T /*)(_$lhs: _$T, _$rhs: _$T) -> (bool*/) tpl_(_$T, eql)
#define cmp_neq$(_$T /*)(_$lhs: _$T, _$rhs: _$T) -> (bool*/) tpl_(_$T, neq)

#define cmp_eqlCtx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, eqlCtx)
#define cmp_neqCtx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, neqCtx)

#define cmp_u_eql$(_$T /*)(_$lhs: _$T, _$rhs: _$T) -> (bool)(*/) tpl_(_$T, u_eql)
#define cmp_u_neq$(_$T /*)(_$lhs: _$T, _$rhs: _$T) -> (bool)(*/) tpl_(_$T, u_neq)

#define cmp_u_eqlCtx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, u_eqlCtx)
#define cmp_u_neqCtx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, u_neqCtx)

/* --- Ordering Operators --- */

typedef enum_((cmp_Sgn $fits($packed))(
    cmp_Sgn_neg = -1,
    cmp_Sgn_ngtv = cmp_Sgn_neg,
    cmp_Sgn_zero = 0,
    cmp_Sgn_pos = 1,
    cmp_Sgn_pstv = cmp_Sgn_pos
)) cmp_Sgn;
#define cmp_Sgn_inv(_$sgn /*: cmp_Sgn*/... /*(cmp_Sgn)*/) (as$(cmp_Sgn)(-(_$sgn)))
#define cmp_Sgn_isNeg(_$sgn /*: cmp_Sgn*/... /*(bool)*/) bool_((_$sgn) < cmp_Sgn_zero)
#define cmp_Sgn_isNgtv(_$sgn /*: cmp_Sgn*/... /*(bool)*/) cmp_Sgn_isNeg(_$sgn)
#define cmp_Sgn_isZero(_$sgn /*: cmp_Sgn*/... /*(bool)*/) bool_((_$sgn) == cmp_Sgn_zero)
#define cmp_Sgn_isNonzero(_$sgn /*: cmp_Sgn*/... /*(bool)*/) bool_((_$sgn) != cmp_Sgn_zero)
#define cmp_Sgn_isPos(_$sgn /*: cmp_Sgn*/... /*(bool)*/) bool_((_$sgn) > cmp_Sgn_zero)
#define cmp_Sgn_isPstv(_$sgn /*: cmp_Sgn*/... /*(bool)*/) cmp_Sgn_isPos(_$sgn)

typedef enum_((cmp_Ord $fits($packed))(
    cmp_Ord_lt = -1,
    cmp_Ord_eq = 0,
    cmp_Ord_gt = 1,
)) cmp_Ord;
#define cmp_Ord_inv(_$ord /*: cmp_Ord*/... /*(cmp_Ord)*/) (as$(cmp_Ord)(-(_$ord)))
#define cmp_Ord_isEq(_$ord /*: cmp_Ord*/... /*(bool)*/) bool_((_$ord) == cmp_Ord_eq)
#define cmp_Ord_isNe(_$ord /*: cmp_Ord*/... /*(bool)*/) bool_((_$ord) != cmp_Ord_eq)
#define cmp_Ord_isLt(_$ord /*: cmp_Ord*/... /*(bool)*/) bool_((_$ord) < cmp_Ord_eq)
#define cmp_Ord_isGt(_$ord /*: cmp_Ord*/... /*(bool)*/) bool_((_$ord) > cmp_Ord_eq)
#define cmp_Ord_isLe(_$ord /*: cmp_Ord*/... /*(bool)*/) bool_((_$ord) <= cmp_Ord_eq)
#define cmp_Ord_isGe(_$ord /*: cmp_Ord*/... /*(bool)*/) bool_((_$ord) >= cmp_Ord_eq)

typedef enum_((cmp_ApxMode $fits($packed))(
    cmp_ApxMode_abs,
    cmp_ApxMode_rel,
)) cmp_ApxMode;

/*
 * Defining ordering for a type _$T (cmp_ord$ / cmp_lt$):
 *
 * Case 1 — ord defined: Provide cmp_ord$(_$T)(lhs, rhs) -> cmp_Ord (three-way).
 *   Then cmp_eq$(_$T), cmp_ne$(_$T), cmp_lt$(_$T), cmp_gt$(_$T), cmp_le$(_$T), cmp_ge$(_$T)
 *   can be defaulted from ord via cmp_fn_eq_default$(_$T), cmp_fn_lt_default$(_$T), etc.
 *
 * Case 2 — lt defined only: Provide cmp_lt$(_$T)(lhs, rhs) -> bool (strict less-than).
 *   Use cmp_fn_ord_default$(_$T) to derive ord from lt:
 *   ord = lt(lhs,rhs) ? cmp_Ord_lt : (lt(rhs,lhs) ? cmp_Ord_gt : cmp_Ord_eq).
 *   Then eq, ne, lt, gt, le, ge are defaulted from that ord as in Case 1.
 *
 * Same pattern for Ctx and Apx variants: ordCtx/ltCtx, ordApx/ltApx.
 */

/* --- Comparison Operators --- */

#define cmp_ord$(_$T /*)(_$lhs: _$T, _$rhs: _$T) -> (cmp_Ord*/) tpl_(_$T, ord)
#define cmp_eq$(_$T /*)(_$lhs: _$T, _$rhs: _$T) -> (bool*/) tpl_(_$T, eq)
#define cmp_ne$(_$T /*)(_$lhs: _$T, _$rhs: _$T) -> (bool*/) tpl_(_$T, ne)
#define cmp_lt$(_$T /*)(_$lhs: _$T, _$rhs: _$T) -> (bool*/) tpl_(_$T, lt)
#define cmp_gt$(_$T /*)(_$lhs: _$T, _$rhs: _$T) -> (bool*/) tpl_(_$T, gt)
#define cmp_le$(_$T /*)(_$lhs: _$T, _$rhs: _$T) -> (bool*/) tpl_(_$T, le)
#define cmp_ge$(_$T /*)(_$lhs: _$T, _$rhs: _$T) -> (bool*/) tpl_(_$T, ge)

#define cmp_ordCtx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$ctx: u_V$raw) -> (cmp_Ord*/) tpl_(_$T, ordCtx)
#define cmp_eqCtx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, eqCtx)
#define cmp_neCtx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, neCtx)
#define cmp_ltCtx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, ltCtx)
#define cmp_gtCtx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, gtCtx)
#define cmp_leCtx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, leCtx)
#define cmp_geCtx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, geCtx)

#define cmp_ordApx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T, _$mode: cmp_ApxMode) -> (cmp_Ord*/) tpl_(_$T, ordApx)
#define cmp_eqApx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, eqApx)
#define cmp_neApx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, neApx)
#define cmp_ltApx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, ltApx)
#define cmp_gtApx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, gtApx)
#define cmp_leApx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, leApx)
#define cmp_geApx$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, geApx)

#define cmp_ordApxAbs$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (cmp_Ord*/) tpl_(_$T, ordApxAbs)
#define cmp_eqApxAbs$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, eqApxAbs)
#define cmp_neApxAbs$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, neApxAbs)
#define cmp_ltApxAbs$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, ltApxAbs)
#define cmp_gtApxAbs$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, gtApxAbs)
#define cmp_leApxAbs$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, leApxAbs)
#define cmp_geApxAbs$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, geApxAbs)

#define cmp_ordApxRel$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (cmp_Ord*/) tpl_(_$T, ordApxRel)
#define cmp_eqApxRel$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, eqApxRel)
#define cmp_neApxRel$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, neApxRel)
#define cmp_ltApxRel$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, ltApxRel)
#define cmp_gtApxRel$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, gtApxRel)
#define cmp_leApxRel$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, leApxRel)
#define cmp_geApxRel$(_$T /*)(_$lhs: _$T, _$rhs: _$T, _$threshold: _$T) -> (bool*/) tpl_(_$T, geApxRel)

#define cmp_u_ord$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw) -> (cmp_Ord*/) tpl_(_$T, u_ord)
#define cmp_u_eq$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw) -> (bool*/) tpl_(_$T, u_eq)
#define cmp_u_ne$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw) -> (bool*/) tpl_(_$T, u_ne)
#define cmp_u_lt$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw) -> (bool*/) tpl_(_$T, u_lt)
#define cmp_u_gt$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw) -> (bool*/) tpl_(_$T, u_gt)
#define cmp_u_le$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw) -> (bool*/) tpl_(_$T, u_le)
#define cmp_u_ge$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw) -> (bool*/) tpl_(_$T, u_ge)

#define cmp_u_ordCtx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$ctx: u_V$raw) -> (cmp_Ord*/) tpl_(_$T, u_ordCtx)
#define cmp_u_eqCtx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, u_eqCtx)
#define cmp_u_neCtx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, u_neCtx)
#define cmp_u_ltCtx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, u_ltCtx)
#define cmp_u_gtCtx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, u_gtCtx)
#define cmp_u_leCtx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, u_leCtx)
#define cmp_u_geCtx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$ctx: u_V$raw) -> (bool*/) tpl_(_$T, u_geCtx)

#define cmp_u_ordApx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw, _$mode: cmp_ApxMode) -> (cmp_Ord*/) tpl_(_$T, u_ordApx)
#define cmp_u_eqApx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, u_eqApx)
#define cmp_u_neApx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, u_neApx)
#define cmp_u_ltApx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, u_ltApx)
#define cmp_u_gtApx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, u_gtApx)
#define cmp_u_leApx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, u_leApx)
#define cmp_u_geApx$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw, _$mode: cmp_ApxMode) -> (bool*/) tpl_(_$T, u_geApx)

#define cmp_u_ordApxAbs$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (cmp_Ord*/) tpl_(_$T, u_ordApxAbs)
#define cmp_u_eqApxAbs$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_eqApxAbs)
#define cmp_u_neApxAbs$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_neApxAbs)
#define cmp_u_ltApxAbs$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_ltApxAbs)
#define cmp_u_gtApxAbs$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_gtApxAbs)
#define cmp_u_leApxAbs$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_leApxAbs)
#define cmp_u_geApxAbs$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_geApxAbs)

#define cmp_u_ordApxRel$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (cmp_Ord*/) tpl_(_$T, u_ordApxRel)
#define cmp_u_eqApxRel$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_eqApxRel)
#define cmp_u_neApxRel$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_neApxRel)
#define cmp_u_ltApxRel$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_ltApxRel)
#define cmp_u_gtApxRel$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_gtApxRel)
#define cmp_u_leApxRel$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_leApxRel)
#define cmp_u_geApxRel$(_$T /*)(_$lhs: u_V$raw, _$rhs: u_V$raw, _$threshold: u_V$raw) -> (bool*/) tpl_(_$T, u_geApxRel)

/* --- Function-like Macros --- */

#define cmp_fn_(_$op) pp_cat(tpl_(cmp_fn, _$op), $)

#define cmp_fn_eql$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_eql$(__step__cmp_fn_eql$__parse __VA_ARGS__)
#define cmp_fn_neq$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_neq$(__step__cmp_fn_neq$__parse __VA_ARGS__)

#define cmp_fn_eqlCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_eqlCtx$(__step__cmp_fn_eqlCtx$__parse __VA_ARGS__)
#define cmp_fn_neqCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_neqCtx$(__step__cmp_fn_neqCtx$__parse __VA_ARGS__)

#define cmp_fn_ord$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_ord$(__step__cmp_fn_ord$__parse __VA_ARGS__)
#define cmp_fn_eq$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_eq$(__step__cmp_fn_eq$__parse __VA_ARGS__)
#define cmp_fn_ne$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_ne$(__step__cmp_fn_ne$__parse __VA_ARGS__)
#define cmp_fn_lt$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_lt$(__step__cmp_fn_lt$__parse __VA_ARGS__)
#define cmp_fn_gt$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_gt$(__step__cmp_fn_gt$__parse __VA_ARGS__)
#define cmp_fn_le$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_le$(__step__cmp_fn_le$__parse __VA_ARGS__)
#define cmp_fn_ge$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_ge$(__step__cmp_fn_ge$__parse __VA_ARGS__)

#define cmp_fn_ordCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_ordCtx$(__step__cmp_fn_ordCtx$__parse __VA_ARGS__)
#define cmp_fn_eqCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_eqCtx$(__step__cmp_fn_eqCtx$__parse __VA_ARGS__)
#define cmp_fn_neCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_neCtx$(__step__cmp_fn_neCtx$__parse __VA_ARGS__)
#define cmp_fn_ltCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_ltCtx$(__step__cmp_fn_ltCtx$__parse __VA_ARGS__)
#define cmp_fn_gtCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_gtCtx$(__step__cmp_fn_gtCtx$__parse __VA_ARGS__)
#define cmp_fn_leCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_leCtx$(__step__cmp_fn_leCtx$__parse __VA_ARGS__)
#define cmp_fn_geCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_geCtx$(__step__cmp_fn_geCtx$__parse __VA_ARGS__)

#define cmp_fn_ordApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_ordApx$(__step__cmp_fn_ordApx$__parse __VA_ARGS__)
#define cmp_fn_eqApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_eqApx$(__step__cmp_fn_eqApx$__parse __VA_ARGS__)
#define cmp_fn_neApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_neApx$(__step__cmp_fn_neApx$__parse __VA_ARGS__)
#define cmp_fn_ltApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_ltApx$(__step__cmp_fn_ltApx$__parse __VA_ARGS__)
#define cmp_fn_gtApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_gtApx$(__step__cmp_fn_gtApx$__parse __VA_ARGS__)
#define cmp_fn_leApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_leApx$(__step__cmp_fn_leApx$__parse __VA_ARGS__)
#define cmp_fn_geApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_geApx$(__step__cmp_fn_geApx$__parse __VA_ARGS__)

#define cmp_fn_ordApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_ordApxAbs$(__step__cmp_fn_ordApxAbs$__parse __VA_ARGS__)
#define cmp_fn_eqApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_eqApxAbs$(__step__cmp_fn_eqApxAbs$__parse __VA_ARGS__)
#define cmp_fn_neApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_neApxAbs$(__step__cmp_fn_neApxAbs$__parse __VA_ARGS__)
#define cmp_fn_ltApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_ltApxAbs$(__step__cmp_fn_ltApxAbs$__parse __VA_ARGS__)
#define cmp_fn_gtApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_gtApxAbs$(__step__cmp_fn_gtApxAbs$__parse __VA_ARGS__)
#define cmp_fn_leApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_leApxAbs$(__step__cmp_fn_leApxAbs$__parse __VA_ARGS__)
#define cmp_fn_geApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_geApxAbs$(__step__cmp_fn_geApxAbs$__parse __VA_ARGS__)

#define cmp_fn_ordApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_ordApxRel$(__step__cmp_fn_ordApxRel$__parse __VA_ARGS__)
#define cmp_fn_eqApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_eqApxRel$(__step__cmp_fn_eqApxRel$__parse __VA_ARGS__)
#define cmp_fn_neApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_neApxRel$(__step__cmp_fn_neApxRel$__parse __VA_ARGS__)
#define cmp_fn_ltApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_ltApxRel$(__step__cmp_fn_ltApxRel$__parse __VA_ARGS__)
#define cmp_fn_gtApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_gtApxRel$(__step__cmp_fn_gtApxRel$__parse __VA_ARGS__)
#define cmp_fn_leApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_leApxRel$(__step__cmp_fn_leApxRel$__parse __VA_ARGS__)
#define cmp_fn_geApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_geApxRel$(__step__cmp_fn_geApxRel$__parse __VA_ARGS__)

#define cmp_fn_u_eql$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_u_eql$(__step__cmp_fn_u_eql$__parse __VA_ARGS__)
#define cmp_fn_u_neq$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_u_neq$(__step__cmp_fn_u_neq$__parse __VA_ARGS__)

#define cmp_fn_u_eqlCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_u_eqlCtx$(__step__cmp_fn_u_eqlCtx$__parse __VA_ARGS__)
#define cmp_fn_u_neqCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_u_neqCtx$(__step__cmp_fn_u_neqCtx$__parse __VA_ARGS__)

#define cmp_fn_u_ord$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_u_ord$(__step__cmp_fn_u_ord$__parse __VA_ARGS__)
#define cmp_fn_u_eq$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_u_eq$(__step__cmp_fn_u_eq$__parse __VA_ARGS__)
#define cmp_fn_u_ne$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_u_ne$(__step__cmp_fn_u_ne$__parse __VA_ARGS__)
#define cmp_fn_u_lt$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_u_lt$(__step__cmp_fn_u_lt$__parse __VA_ARGS__)
#define cmp_fn_u_gt$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_u_gt$(__step__cmp_fn_u_gt$__parse __VA_ARGS__)
#define cmp_fn_u_le$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_u_le$(__step__cmp_fn_u_le$__parse __VA_ARGS__)
#define cmp_fn_u_ge$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) __step__cmp_fn_u_ge$(__step__cmp_fn_u_ge$__parse __VA_ARGS__)

#define cmp_fn_u_ordCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_u_ordCtx$(__step__cmp_fn_u_ordCtx$__parse __VA_ARGS__)
#define cmp_fn_u_eqCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_u_eqCtx$(__step__cmp_fn_u_eqCtx$__parse __VA_ARGS__)
#define cmp_fn_u_neCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_u_neCtx$(__step__cmp_fn_u_neCtx$__parse __VA_ARGS__)
#define cmp_fn_u_ltCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_u_ltCtx$(__step__cmp_fn_u_ltCtx$__parse __VA_ARGS__)
#define cmp_fn_u_gtCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_u_gtCtx$(__step__cmp_fn_u_gtCtx$__parse __VA_ARGS__)
#define cmp_fn_u_leCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_u_leCtx$(__step__cmp_fn_u_leCtx$__parse __VA_ARGS__)
#define cmp_fn_u_geCtx$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) __step__cmp_fn_u_geCtx$(__step__cmp_fn_u_geCtx$__parse __VA_ARGS__)

#define cmp_fn_u_ordApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_u_ordApx$(__step__cmp_fn_u_ordApx$__parse __VA_ARGS__)
#define cmp_fn_u_eqApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_u_eqApx$(__step__cmp_fn_u_eqApx$__parse __VA_ARGS__)
#define cmp_fn_u_neApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_u_neApx$(__step__cmp_fn_u_neApx$__parse __VA_ARGS__)
#define cmp_fn_u_ltApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_u_ltApx$(__step__cmp_fn_u_ltApx$__parse __VA_ARGS__)
#define cmp_fn_u_gtApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_u_gtApx$(__step__cmp_fn_u_gtApx$__parse __VA_ARGS__)
#define cmp_fn_u_leApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_u_leApx$(__step__cmp_fn_u_leApx$__parse __VA_ARGS__)
#define cmp_fn_u_geApx$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) __step__cmp_fn_u_geApx$(__step__cmp_fn_u_geApx$__parse __VA_ARGS__)

#define cmp_fn_u_ordApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_ordApxAbs$(__step__cmp_fn_u_ordApxAbs$__parse __VA_ARGS__)
#define cmp_fn_u_eqApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_eqApxAbs$(__step__cmp_fn_u_eqApxAbs$__parse __VA_ARGS__)
#define cmp_fn_u_neApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_neApxAbs$(__step__cmp_fn_u_neApxAbs$__parse __VA_ARGS__)
#define cmp_fn_u_ltApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_ltApxAbs$(__step__cmp_fn_u_ltApxAbs$__parse __VA_ARGS__)
#define cmp_fn_u_gtApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_gtApxAbs$(__step__cmp_fn_u_gtApxAbs$__parse __VA_ARGS__)
#define cmp_fn_u_leApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_leApxAbs$(__step__cmp_fn_u_leApxAbs$__parse __VA_ARGS__)
#define cmp_fn_u_geApxAbs$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_geApxAbs$(__step__cmp_fn_u_geApxAbs$__parse __VA_ARGS__)

#define cmp_fn_u_ordApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_ordApxRel$(__step__cmp_fn_u_ordApxRel$__parse __VA_ARGS__)
#define cmp_fn_u_eqApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_eqApxRel$(__step__cmp_fn_u_eqApxRel$__parse __VA_ARGS__)
#define cmp_fn_u_neApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_neApxRel$(__step__cmp_fn_u_neApxRel$__parse __VA_ARGS__)
#define cmp_fn_u_ltApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_ltApxRel$(__step__cmp_fn_u_ltApxRel$__parse __VA_ARGS__)
#define cmp_fn_u_gtApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_gtApxRel$(__step__cmp_fn_u_gtApxRel$__parse __VA_ARGS__)
#define cmp_fn_u_leApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_leApxRel$(__step__cmp_fn_u_leApxRel$__parse __VA_ARGS__)
#define cmp_fn_u_geApxRel$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) __step__cmp_fn_u_geApxRel$(__step__cmp_fn_u_geApxRel$__parse __VA_ARGS__)

/* --- Function-like Macros Default Implementations --- */
/*
 * Ord-related defaults: use cmp_fn_ord_default$ when only cmp_lt$ is defined;
 * use cmp_fn_eq_default$, cmp_fn_lt_default$, etc. when cmp_ord$ is defined (or derived).
 */

#define cmp_fn_eql_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_neq` is implemented */ \
    __step__cmp_fn_eql_default$(__step__cmp_fn_eql_default$__parse __VA_ARGS__)
#define cmp_fn_neq_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_eql` is implemented */ \
    __step__cmp_fn_neq_default$(__step__cmp_fn_neq_default$__parse __VA_ARGS__)

#define cmp_fn_eqlCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_neqCtx` is implemented */ \
    __step__cmp_fn_eqlCtx_default$(__step__cmp_fn_eqlCtx_default$__parse __VA_ARGS__)
#define cmp_fn_neqCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_eqlCtx` is implemented */ \
    __step__cmp_fn_neqCtx_default$(__step__cmp_fn_neqCtx_default$__parse __VA_ARGS__)

#define cmp_fn_ord_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_lt` is implemented */ \
    __step__cmp_fn_ord_default$(__step__cmp_fn_ord_default$__parse __VA_ARGS__)
#define cmp_fn_eq_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_eq_default$(__step__cmp_fn_eq_default$__parse __VA_ARGS__)
#define cmp_fn_ne_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_ne_default$(__step__cmp_fn_ne_default$__parse __VA_ARGS__)
#define cmp_fn_lt_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_lt_default$(__step__cmp_fn_lt_default$__parse __VA_ARGS__)
#define cmp_fn_gt_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_gt_default$(__step__cmp_fn_gt_default$__parse __VA_ARGS__)
#define cmp_fn_le_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_le_default$(__step__cmp_fn_le_default$__parse __VA_ARGS__)
#define cmp_fn_ge_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_ord` is implemented */ \
    __step__cmp_fn_ge_default$(__step__cmp_fn_ge_default$__parse __VA_ARGS__)

#define cmp_fn_ordCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_ltCtx` is implemented */ \
    __step__cmp_fn_ordCtx_default$(__step__cmp_fn_ordCtx_default$__parse __VA_ARGS__)
#define cmp_fn_eqCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_eqCtx_default$(__step__cmp_fn_eqCtx_default$__parse __VA_ARGS__)
#define cmp_fn_neCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_neCtx_default$(__step__cmp_fn_neCtx_default$__parse __VA_ARGS__)
#define cmp_fn_ltCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_ltCtx_default$(__step__cmp_fn_ltCtx_default$__parse __VA_ARGS__)
#define cmp_fn_gtCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_gtCtx_default$(__step__cmp_fn_gtCtx_default$__parse __VA_ARGS__)
#define cmp_fn_leCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_leCtx_default$(__step__cmp_fn_leCtx_default$__parse __VA_ARGS__)
#define cmp_fn_geCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_ordCtx` is implemented */ \
    __step__cmp_fn_geCtx_default$(__step__cmp_fn_geCtx_default$__parse __VA_ARGS__)

#define cmp_fn_ordApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_ordApxAbs` and `cmp_ordApxRel` are implemented */ \
    __step__cmp_fn_ordApx_default$(__step__cmp_fn_ordApx_default$__parse __VA_ARGS__)
#define cmp_fn_eqApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_eqApx_default$(__step__cmp_fn_eqApx_default$__parse __VA_ARGS__)
#define cmp_fn_neApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_neApx_default$(__step__cmp_fn_neApx_default$__parse __VA_ARGS__)
#define cmp_fn_ltApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_ltApx_default$(__step__cmp_fn_ltApx_default$__parse __VA_ARGS__)
#define cmp_fn_gtApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_gtApx_default$(__step__cmp_fn_gtApx_default$__parse __VA_ARGS__)
#define cmp_fn_leApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_leApx_default$(__step__cmp_fn_leApx_default$__parse __VA_ARGS__)
#define cmp_fn_geApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_ordApx` is implemented */ \
    __step__cmp_fn_geApx_default$(__step__cmp_fn_geApx_default$__parse __VA_ARGS__)

#define cmp_fn_ordApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ltApxAbs` is implemented */ \
    __step__cmp_fn_ordApxAbs_default$(__step__cmp_fn_ordApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_eqApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxAbs` is implemented */ \
    __step__cmp_fn_eqApxAbs_default$(__step__cmp_fn_eqApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_neApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxAbs` is implemented */ \
    __step__cmp_fn_neApxAbs_default$(__step__cmp_fn_neApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_ltApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxAbs` is implemented */ \
    __step__cmp_fn_ltApxAbs_default$(__step__cmp_fn_ltApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_gtApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxAbs` is implemented */ \
    __step__cmp_fn_gtApxAbs_default$(__step__cmp_fn_gtApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_leApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxAbs` is implemented */ \
    __step__cmp_fn_leApxAbs_default$(__step__cmp_fn_leApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_geApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxAbs` is implemented */ \
    __step__cmp_fn_geApxAbs_default$(__step__cmp_fn_geApxAbs_default$__parse __VA_ARGS__)

#define cmp_fn_ordApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ltApxRel` is implemented */ \
    __step__cmp_fn_ordApxRel_default$(__step__cmp_fn_ordApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_eqApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxRel` is implemented */ \
    __step__cmp_fn_eqApxRel_default$(__step__cmp_fn_eqApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_neApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxRel` is implemented */ \
    __step__cmp_fn_neApxRel_default$(__step__cmp_fn_neApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_ltApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxRel` is implemented */ \
    __step__cmp_fn_ltApxRel_default$(__step__cmp_fn_ltApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_gtApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxRel` is implemented */ \
    __step__cmp_fn_gtApxRel_default$(__step__cmp_fn_gtApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_leApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxRel` is implemented */ \
    __step__cmp_fn_leApxRel_default$(__step__cmp_fn_leApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_geApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    /* if `cmp_ordApxRel` is implemented */ \
    __step__cmp_fn_geApxRel_default$(__step__cmp_fn_geApxRel_default$__parse __VA_ARGS__)

#define cmp_fn_u_eql_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_u_neq` is implemented */ \
    __step__cmp_fn_u_eql_default$(__step__cmp_fn_u_eql_default$__parse __VA_ARGS__)
#define cmp_fn_u_neq_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_u_eql` is implemented */ \
    __step__cmp_fn_u_neq_default$(__step__cmp_fn_u_neq_default$__parse __VA_ARGS__)

#define cmp_fn_u_eqlCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_u_neqCtx` is implemented */ \
    __step__cmp_fn_u_eqlCtx_default$(__step__cmp_fn_u_eqlCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_neqCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_u_eqlCtx` is implemented */ \
    __step__cmp_fn_u_neqCtx_default$(__step__cmp_fn_u_neqCtx_default$__parse __VA_ARGS__)

#define cmp_fn_u_ord_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_u_lt` is implemented */ \
    __step__cmp_fn_u_ord_default$(__step__cmp_fn_u_ord_default$__parse __VA_ARGS__)
#define cmp_fn_u_eq_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_eq_default$(__step__cmp_fn_u_eq_default$__parse __VA_ARGS__)
#define cmp_fn_u_ne_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_ne_default$(__step__cmp_fn_u_ne_default$__parse __VA_ARGS__)
#define cmp_fn_u_lt_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_lt_default$(__step__cmp_fn_u_lt_default$__parse __VA_ARGS__)
#define cmp_fn_u_gt_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_gt_default$(__step__cmp_fn_u_gt_default$__parse __VA_ARGS__)
#define cmp_fn_u_le_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_le_default$(__step__cmp_fn_u_le_default$__parse __VA_ARGS__)
#define cmp_fn_u_ge_default$(/*(_$T)(_$id_lhs, _$id_rhs)*/...) \
    /* if `cmp_u_ord` is implemented */ \
    __step__cmp_fn_u_ge_default$(__step__cmp_fn_u_ge_default$__parse __VA_ARGS__)

#define cmp_fn_u_ordCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_u_ltCtx` is implemented */ \
    __step__cmp_fn_u_ordCtx_default$(__step__cmp_fn_u_ordCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_eqCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_eqCtx_default$(__step__cmp_fn_u_eqCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_neCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_neCtx_default$(__step__cmp_fn_u_neCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_ltCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_ltCtx_default$(__step__cmp_fn_u_ltCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_gtCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_gtCtx_default$(__step__cmp_fn_u_gtCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_leCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_leCtx_default$(__step__cmp_fn_u_leCtx_default$__parse __VA_ARGS__)
#define cmp_fn_u_geCtx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$id_ctx)*/...) \
    /* if `cmp_u_ordCtx` is implemented */ \
    __step__cmp_fn_u_geCtx_default$(__step__cmp_fn_u_geCtx_default$__parse __VA_ARGS__)

#define cmp_fn_u_ordApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_u_ltApx` is implemented */ \
    __step__cmp_fn_u_ordApx_default$(__step__cmp_fn_u_ordApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_eqApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_eqApx_default$(__step__cmp_fn_u_eqApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_neApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_neApx_default$(__step__cmp_fn_u_neApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_ltApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_ltApx_default$(__step__cmp_fn_u_ltApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_gtApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_gtApx_default$(__step__cmp_fn_u_gtApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_leApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_leApx_default$(__step__cmp_fn_u_leApx_default$__parse __VA_ARGS__)
#define cmp_fn_u_geApx_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold, _$mode)*/...) \
    /* if `cmp_u_ordApx` is implemented */ \
    __step__cmp_fn_u_geApx_default$(__step__cmp_fn_u_geApx_default$__parse __VA_ARGS__)

#define cmp_fn_u_ordApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_ordApxAbs_default$(__step__cmp_fn_u_ordApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_u_eqApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_eqApxAbs_default$(__step__cmp_fn_u_eqApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_u_neApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_neApxAbs_default$(__step__cmp_fn_u_neApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_u_ltApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_ltApxAbs_default$(__step__cmp_fn_u_ltApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_u_gtApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_gtApxAbs_default$(__step__cmp_fn_u_gtApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_u_leApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_leApxAbs_default$(__step__cmp_fn_u_leApxAbs_default$__parse __VA_ARGS__)
#define cmp_fn_u_geApxAbs_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_geApxAbs_default$(__step__cmp_fn_u_geApxAbs_default$__parse __VA_ARGS__)

#define cmp_fn_u_ordApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_ordApxRel_default$(__step__cmp_fn_u_ordApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_u_eqApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_eqApxRel_default$(__step__cmp_fn_u_eqApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_u_neApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_neApxRel_default$(__step__cmp_fn_u_neApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_u_ltApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_ltApxRel_default$(__step__cmp_fn_u_ltApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_u_gtApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_gtApxRel_default$(__step__cmp_fn_u_gtApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_u_leApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_leApxRel_default$(__step__cmp_fn_u_leApxRel_default$__parse __VA_ARGS__)
#define cmp_fn_u_geApxRel_default$(/*(_$T)(_$id_lhs, _$id_rhs, _$threshold)*/...) \
    __step__cmp_fn_u_geApxRel_default$(__step__cmp_fn_u_geApxRel_default$__parse __VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __step__cmp_fn_eql$(...) __cmp_fn_eql$(__VA_ARGS__)
#define __step__cmp_fn_eql$__parse(_$T...) _$T, __step__cmp_fn_eql$__parseNext
#define __step__cmp_fn_eql$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_eql$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_eql$(_$T)(_$T _$lhs, _$T _$rhs))(bool))
#define __step__cmp_fn_neq$(...) __cmp_fn_neq$(__VA_ARGS__)
#define __step__cmp_fn_neq$__parse(_$T...) _$T, __step__cmp_fn_neq$__parseNext
#define __step__cmp_fn_neq$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_neq$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_neq$(_$T)(_$T _$lhs, _$T _$rhs))(bool))

#define __step__cmp_fn_eqlCtx$(...) __cmp_fn_eqlCtx$(__VA_ARGS__)
#define __step__cmp_fn_eqlCtx$__parse(_$T...) _$T, __step__cmp_fn_eqlCtx$__parseNext
#define __step__cmp_fn_eqlCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_eqlCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_eqlCtx$(_$T)(_$T _$lhs, _$T _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_neqCtx$(...) __cmp_fn_neqCtx$(__VA_ARGS__)
#define __step__cmp_fn_neqCtx$__parse(_$T...) _$T, __step__cmp_fn_neqCtx$__parseNext
#define __step__cmp_fn_neqCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_neqCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_neqCtx$(_$T)(_$T _$lhs, _$T _$rhs, u_V$raw _$ctx))(bool))

#define __step__cmp_fn_ord$(...) __cmp_fn_ord$(__VA_ARGS__)
#define __step__cmp_fn_ord$__parse(_$T...) _$T, __step__cmp_fn_ord$__parseNext
#define __step__cmp_fn_ord$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_ord$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_ord$(_$T)(_$T _$lhs, _$T _$rhs))(cmp_Ord))
#define __step__cmp_fn_eq$(...) __cmp_fn_eq$(__VA_ARGS__)
#define __step__cmp_fn_eq$__parse(_$T...) _$T, __step__cmp_fn_eq$__parseNext
#define __step__cmp_fn_eq$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_eq$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_eq$(_$T)(_$T _$lhs, _$T _$rhs))(bool))
#define __step__cmp_fn_ne$(...) __cmp_fn_ne$(__VA_ARGS__)
#define __step__cmp_fn_ne$__parse(_$T...) _$T, __step__cmp_fn_ne$__parseNext
#define __step__cmp_fn_ne$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_ne$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_ne$(_$T)(_$T _$lhs, _$T _$rhs))(bool))
#define __step__cmp_fn_lt$(...) __cmp_fn_lt$(__VA_ARGS__)
#define __step__cmp_fn_lt$__parse(_$T...) _$T, __step__cmp_fn_lt$__parseNext
#define __step__cmp_fn_lt$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_lt$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_lt$(_$T)(_$T _$lhs, _$T _$rhs))(bool))
#define __step__cmp_fn_gt$(...) __cmp_fn_gt$(__VA_ARGS__)
#define __step__cmp_fn_gt$__parse(_$T...) _$T, __step__cmp_fn_gt$__parseNext
#define __step__cmp_fn_gt$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_gt$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_gt$(_$T)(_$T _$lhs, _$T _$rhs))(bool))
#define __step__cmp_fn_le$(...) __cmp_fn_le$(__VA_ARGS__)
#define __step__cmp_fn_le$__parse(_$T...) _$T, __step__cmp_fn_le$__parseNext
#define __step__cmp_fn_le$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_le$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_le$(_$T)(_$T _$lhs, _$T _$rhs))(bool))
#define __step__cmp_fn_ge$(...) __cmp_fn_ge$(__VA_ARGS__)
#define __step__cmp_fn_ge$__parse(_$T...) _$T, __step__cmp_fn_ge$__parseNext
#define __step__cmp_fn_ge$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_ge$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_ge$(_$T)(_$T _$lhs, _$T _$rhs))(bool))

#define __step__cmp_fn_ordCtx$(...) __cmp_fn_ordCtx$(__VA_ARGS__)
#define __step__cmp_fn_ordCtx$__parse(_$T...) _$T, __step__cmp_fn_ordCtx$__parseNext
#define __step__cmp_fn_ordCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_ordCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_ordCtx$(_$T)(_$T _$lhs, _$T _$rhs, u_V$raw _$ctx))(cmp_Ord))
#define __step__cmp_fn_eqCtx$(...) __cmp_fn_eqCtx$(__VA_ARGS__)
#define __step__cmp_fn_eqCtx$__parse(_$T...) _$T, __step__cmp_fn_eqCtx$__parseNext
#define __step__cmp_fn_eqCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_eqCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_eqCtx$(_$T)(_$T _$lhs, _$T _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_neCtx$(...) __cmp_fn_neCtx$(__VA_ARGS__)
#define __step__cmp_fn_neCtx$__parse(_$T...) _$T, __step__cmp_fn_neCtx$__parseNext
#define __step__cmp_fn_neCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_neCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_neCtx$(_$T)(_$T _$lhs, _$T _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_ltCtx$(...) __cmp_fn_ltCtx$(__VA_ARGS__)
#define __step__cmp_fn_ltCtx$__parse(_$T...) _$T, __step__cmp_fn_ltCtx$__parseNext
#define __step__cmp_fn_ltCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_ltCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_ltCtx$(_$T)(_$T _$lhs, _$T _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_gtCtx$(...) __cmp_fn_gtCtx$(__VA_ARGS__)
#define __step__cmp_fn_gtCtx$__parse(_$T...) _$T, __step__cmp_fn_gtCtx$__parseNext
#define __step__cmp_fn_gtCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_gtCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_gtCtx$(_$T)(_$T _$lhs, _$T _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_leCtx$(...) __cmp_fn_leCtx$(__VA_ARGS__)
#define __step__cmp_fn_leCtx$__parse(_$T...) _$T, __step__cmp_fn_leCtx$__parseNext
#define __step__cmp_fn_leCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_leCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_leCtx$(_$T)(_$T _$lhs, _$T _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_geCtx$(...) __cmp_fn_geCtx$(__VA_ARGS__)
#define __step__cmp_fn_geCtx$__parse(_$T...) _$T, __step__cmp_fn_geCtx$__parseNext
#define __step__cmp_fn_geCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_geCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_geCtx$(_$T)(_$T _$lhs, _$T _$rhs, u_V$raw _$ctx))(bool))

#define __step__cmp_fn_ordApx$(...) __cmp_fn_ordApx$(__VA_ARGS__)
#define __step__cmp_fn_ordApx$__parse(_$T...) _$T, __step__cmp_fn_ordApx$__parseNext
#define __step__cmp_fn_ordApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_ordApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_ordApx$(_$T)(_$T _$lhs, _$T _$rhs, _$T _$threshold, cmp_ApxMode _$mode))(cmp_Ord))
#define __step__cmp_fn_eqApx$(...) __cmp_fn_eqApx$(__VA_ARGS__)
#define __step__cmp_fn_eqApx$__parse(_$T...) _$T, __step__cmp_fn_eqApx$__parseNext
#define __step__cmp_fn_eqApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_eqApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_eqApx$(_$T)(_$T _$lhs, _$T _$rhs, _$T _$threshold, cmp_ApxMode _$mode))(bool))
#define __step__cmp_fn_neApx$(...) __cmp_fn_neApx$(__VA_ARGS__)
#define __step__cmp_fn_neApx$__parse(_$T...) _$T, __step__cmp_fn_neApx$__parseNext
#define __step__cmp_fn_neApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_neApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_neApx$(_$T)(_$T _$lhs, _$T _$rhs, _$T _$threshold, cmp_ApxMode _$mode))(bool))
#define __step__cmp_fn_ltApx$(...) __cmp_fn_ltApx$(__VA_ARGS__)
#define __step__cmp_fn_ltApx$__parse(_$T...) _$T, __step__cmp_fn_ltApx$__parseNext
#define __step__cmp_fn_ltApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_ltApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_ltApx$(_$T)(_$T _$lhs, _$T _$rhs, _$T _$threshold, cmp_ApxMode _$mode))(bool))
#define __step__cmp_fn_gtApx$(...) __cmp_fn_gtApx$(__VA_ARGS__)
#define __step__cmp_fn_gtApx$__parse(_$T...) _$T, __step__cmp_fn_gtApx$__parseNext
#define __step__cmp_fn_gtApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_gtApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_gtApx$(_$T)(_$T _$lhs, _$T _$rhs, _$T _$threshold, cmp_ApxMode _$mode))(bool))
#define __step__cmp_fn_leApx$(...) __cmp_fn_leApx$(__VA_ARGS__)
#define __step__cmp_fn_leApx$__parse(_$T...) _$T, __step__cmp_fn_leApx$__parseNext
#define __step__cmp_fn_leApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_leApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_leApx$(_$T)(_$T _$lhs, _$T _$rhs, _$T _$threshold, cmp_ApxMode _$mode))(bool))
#define __step__cmp_fn_geApx$(...) __cmp_fn_geApx$(__VA_ARGS__)
#define __step__cmp_fn_geApx$__parse(_$T...) _$T, __step__cmp_fn_geApx$__parseNext
#define __step__cmp_fn_geApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_geApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_geApx$(_$T)(_$T _$lhs, _$T _$rhs, _$T _$threshold, cmp_ApxMode _$mode))(bool))

#define __step__cmp_fn_Apx3$__parse(_$T...) _$T, __step__cmp_fn_Apx3$__parseNext
#define __step__cmp_fn_Apx3$__parseNext(_$lhs, _$rhs, _$threshold...) _$lhs, _$rhs, _$threshold
#define __cmp_fn_Apx3$(_$op, _Ret, _$T, _$lhs, _$rhs, _$threshold...) \
    fn_((tpl_(_$T, _$op)(_$T _$lhs, _$T _$rhs, _$T _$threshold))(_Ret))

#define __step__cmp_fn_ordApxAbs$(...) __cmp_fn_Apx3$(ordApxAbs, cmp_Ord, __VA_ARGS__)
#define __step__cmp_fn_ordApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_eqApxAbs$(...) __cmp_fn_Apx3$(eqApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_eqApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_neApxAbs$(...) __cmp_fn_Apx3$(neApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_neApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_ltApxAbs$(...) __cmp_fn_Apx3$(ltApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_ltApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_gtApxAbs$(...) __cmp_fn_Apx3$(gtApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_gtApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_leApxAbs$(...) __cmp_fn_Apx3$(leApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_leApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_geApxAbs$(...) __cmp_fn_Apx3$(geApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_geApxAbs$__parse __step__cmp_fn_Apx3$__parse

#define __step__cmp_fn_ordApxRel$(...) __cmp_fn_Apx3$(ordApxRel, cmp_Ord, __VA_ARGS__)
#define __step__cmp_fn_ordApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_eqApxRel$(...) __cmp_fn_Apx3$(eqApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_eqApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_neApxRel$(...) __cmp_fn_Apx3$(neApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_neApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_ltApxRel$(...) __cmp_fn_Apx3$(ltApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_ltApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_gtApxRel$(...) __cmp_fn_Apx3$(gtApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_gtApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_leApxRel$(...) __cmp_fn_Apx3$(leApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_leApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_geApxRel$(...) __cmp_fn_Apx3$(geApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_geApxRel$__parse __step__cmp_fn_Apx3$__parse

#define __step__cmp_fn_u_eql$(...) __cmp_fn_u_eql$(__VA_ARGS__)
#define __step__cmp_fn_u_eql$__parse(_$T...) _$T, __step__cmp_fn_u_eql$__parseNext
#define __step__cmp_fn_u_eql$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_eql$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_u_eql$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs))(bool))
#define __step__cmp_fn_u_neq$(...) __cmp_fn_u_neq$(__VA_ARGS__)
#define __step__cmp_fn_u_neq$__parse(_$T...) _$T, __step__cmp_fn_u_neq$__parseNext
#define __step__cmp_fn_u_neq$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_neq$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_u_neq$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs))(bool))

#define __step__cmp_fn_u_eqlCtx$(...) __cmp_fn_u_eqlCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_eqlCtx$__parse(_$T...) _$T, __step__cmp_fn_u_eqlCtx$__parseNext
#define __step__cmp_fn_u_eqlCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_eqlCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_u_eqlCtx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_u_neqCtx$(...) __cmp_fn_u_neqCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_neqCtx$__parse(_$T...) _$T, __step__cmp_fn_u_neqCtx$__parseNext
#define __step__cmp_fn_u_neqCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_neqCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_u_neqCtx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$ctx))(bool))

#define __step__cmp_fn_u_ord$(...) __cmp_fn_u_ord$(__VA_ARGS__)
#define __step__cmp_fn_u_ord$__parse(_$T...) _$T, __step__cmp_fn_u_ord$__parseNext
#define __step__cmp_fn_u_ord$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_ord$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_u_ord$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs))(cmp_Ord))
#define __step__cmp_fn_u_eq$(...) __cmp_fn_u_eq$(__VA_ARGS__)
#define __step__cmp_fn_u_eq$__parse(_$T...) _$T, __step__cmp_fn_u_eq$__parseNext
#define __step__cmp_fn_u_eq$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_eq$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_u_eq$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs))(bool))
#define __step__cmp_fn_u_ne$(...) __cmp_fn_u_ne$(__VA_ARGS__)
#define __step__cmp_fn_u_ne$__parse(_$T...) _$T, __step__cmp_fn_u_ne$__parseNext
#define __step__cmp_fn_u_ne$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_ne$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_u_ne$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs))(bool))
#define __step__cmp_fn_u_lt$(...) __cmp_fn_u_lt$(__VA_ARGS__)
#define __step__cmp_fn_u_lt$__parse(_$T...) _$T, __step__cmp_fn_u_lt$__parseNext
#define __step__cmp_fn_u_lt$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_lt$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_u_lt$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs))(bool))
#define __step__cmp_fn_u_gt$(...) __cmp_fn_u_gt$(__VA_ARGS__)
#define __step__cmp_fn_u_gt$__parse(_$T...) _$T, __step__cmp_fn_u_gt$__parseNext
#define __step__cmp_fn_u_gt$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_gt$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_u_gt$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs))(bool))
#define __step__cmp_fn_u_le$(...) __cmp_fn_u_le$(__VA_ARGS__)
#define __step__cmp_fn_u_le$__parse(_$T...) _$T, __step__cmp_fn_u_le$__parseNext
#define __step__cmp_fn_u_le$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_le$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_u_le$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs))(bool))
#define __step__cmp_fn_u_ge$(...) __cmp_fn_u_ge$(__VA_ARGS__)
#define __step__cmp_fn_u_ge$__parse(_$T...) _$T, __step__cmp_fn_u_ge$__parseNext
#define __step__cmp_fn_u_ge$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_ge$(_$T, _$lhs, _$rhs...) \
    fn_((cmp_u_ge$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs))(bool))

#define __step__cmp_fn_u_ordCtx$(...) __cmp_fn_u_ordCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_ordCtx$__parse(_$T...) _$T, __step__cmp_fn_u_ordCtx$__parseNext
#define __step__cmp_fn_u_ordCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_ordCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_u_ordCtx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$ctx))(cmp_Ord))
#define __step__cmp_fn_u_eqCtx$(...) __cmp_fn_u_eqCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_eqCtx$__parse(_$T...) _$T, __step__cmp_fn_u_eqCtx$__parseNext
#define __step__cmp_fn_u_eqCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_eqCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_u_eqCtx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_u_neCtx$(...) __cmp_fn_u_neCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_neCtx$__parse(_$T...) _$T, __step__cmp_fn_u_neCtx$__parseNext
#define __step__cmp_fn_u_neCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_neCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_u_neCtx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_u_ltCtx$(...) __cmp_fn_u_ltCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_ltCtx$__parse(_$T...) _$T, __step__cmp_fn_u_ltCtx$__parseNext
#define __step__cmp_fn_u_ltCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_ltCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_u_ltCtx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_u_gtCtx$(...) __cmp_fn_u_gtCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_gtCtx$__parse(_$T...) _$T, __step__cmp_fn_u_gtCtx$__parseNext
#define __step__cmp_fn_u_gtCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_gtCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_u_gtCtx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_u_leCtx$(...) __cmp_fn_u_leCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_leCtx$__parse(_$T...) _$T, __step__cmp_fn_u_leCtx$__parseNext
#define __step__cmp_fn_u_leCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_leCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_u_leCtx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$ctx))(bool))
#define __step__cmp_fn_u_geCtx$(...) __cmp_fn_u_geCtx$(__VA_ARGS__)
#define __step__cmp_fn_u_geCtx$__parse(_$T...) _$T, __step__cmp_fn_u_geCtx$__parseNext
#define __step__cmp_fn_u_geCtx$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_geCtx$(_$T, _$lhs, _$rhs, _$ctx...) \
    fn_((cmp_u_geCtx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$ctx))(bool))

#define __step__cmp_fn_u_ordApx$(...) __cmp_fn_u_ordApx$(__VA_ARGS__)
#define __step__cmp_fn_u_ordApx$__parse(_$T...) _$T, __step__cmp_fn_u_ordApx$__parseNext
#define __step__cmp_fn_u_ordApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_ordApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_u_ordApx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$threshold, cmp_ApxMode _$mode))(cmp_Ord))
#define __step__cmp_fn_u_eqApx$(...) __cmp_fn_u_eqApx$(__VA_ARGS__)
#define __step__cmp_fn_u_eqApx$__parse(_$T...) _$T, __step__cmp_fn_u_eqApx$__parseNext
#define __step__cmp_fn_u_eqApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_eqApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_u_eqApx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$threshold, cmp_ApxMode _$mode))(bool))
#define __step__cmp_fn_u_neApx$(...) __cmp_fn_u_neApx$(__VA_ARGS__)
#define __step__cmp_fn_u_neApx$__parse(_$T...) _$T, __step__cmp_fn_u_neApx$__parseNext
#define __step__cmp_fn_u_neApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_neApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_u_neApx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$threshold, cmp_ApxMode _$mode))(bool))
#define __step__cmp_fn_u_ltApx$(...) __cmp_fn_u_ltApx$(__VA_ARGS__)
#define __step__cmp_fn_u_ltApx$__parse(_$T...) _$T, __step__cmp_fn_u_ltApx$__parseNext
#define __step__cmp_fn_u_ltApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_ltApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_u_ltApx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$threshold, cmp_ApxMode _$mode))(bool))
#define __step__cmp_fn_u_gtApx$(...) __cmp_fn_u_gtApx$(__VA_ARGS__)
#define __step__cmp_fn_u_gtApx$__parse(_$T...) _$T, __step__cmp_fn_u_gtApx$__parseNext
#define __step__cmp_fn_u_gtApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_gtApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_u_gtApx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$threshold, cmp_ApxMode _$mode))(bool))
#define __step__cmp_fn_u_leApx$(...) __cmp_fn_u_leApx$(__VA_ARGS__)
#define __step__cmp_fn_u_leApx$__parse(_$T...) _$T, __step__cmp_fn_u_leApx$__parseNext
#define __step__cmp_fn_u_leApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_leApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_u_leApx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$threshold, cmp_ApxMode _$mode))(bool))
#define __step__cmp_fn_u_geApx$(...) __cmp_fn_u_geApx$(__VA_ARGS__)
#define __step__cmp_fn_u_geApx$__parse(_$T...) _$T, __step__cmp_fn_u_geApx$__parseNext
#define __step__cmp_fn_u_geApx$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_geApx$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    fn_((cmp_u_geApx$(_$T)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$threshold, cmp_ApxMode _$mode))(bool))

#define __cmp_fn_u_Apx3$(_$op, _Ret, _$T, _$lhs, _$rhs, _$threshold...) \
    fn_((tpl_(_$T, _$op)(u_V$raw _$lhs, u_V$raw _$rhs, u_V$raw _$threshold))(_Ret))

#define __step__cmp_fn_u_ordApxAbs$(...) __cmp_fn_u_Apx3$(u_ordApxAbs, cmp_Ord, __VA_ARGS__)
#define __step__cmp_fn_u_ordApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_eqApxAbs$(...) __cmp_fn_u_Apx3$(u_eqApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_eqApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_neApxAbs$(...) __cmp_fn_u_Apx3$(u_neApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_neApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_ltApxAbs$(...) __cmp_fn_u_Apx3$(u_ltApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_ltApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_gtApxAbs$(...) __cmp_fn_u_Apx3$(u_gtApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_gtApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_leApxAbs$(...) __cmp_fn_u_Apx3$(u_leApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_leApxAbs$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_geApxAbs$(...) __cmp_fn_u_Apx3$(u_geApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_geApxAbs$__parse __step__cmp_fn_Apx3$__parse

#define __step__cmp_fn_u_ordApxRel$(...) __cmp_fn_u_Apx3$(u_ordApxRel, cmp_Ord, __VA_ARGS__)
#define __step__cmp_fn_u_ordApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_eqApxRel$(...) __cmp_fn_u_Apx3$(u_eqApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_eqApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_neApxRel$(...) __cmp_fn_u_Apx3$(u_neApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_neApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_ltApxRel$(...) __cmp_fn_u_Apx3$(u_ltApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_ltApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_gtApxRel$(...) __cmp_fn_u_Apx3$(u_gtApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_gtApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_leApxRel$(...) __cmp_fn_u_Apx3$(u_leApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_leApxRel$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_geApxRel$(...) __cmp_fn_u_Apx3$(u_geApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_geApxRel$__parse __step__cmp_fn_Apx3$__parse

#define __step__cmp_fn_eql_default$(...) __cmp_fn_eql_default$(__VA_ARGS__)
#define __step__cmp_fn_eql_default$__parse(_$T...) _$T, __step__cmp_fn_eql_default$__parseNext
#define __step__cmp_fn_eql_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_eql_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(eql)((_$T)(_$lhs, _$rhs)) { \
        return !cmp_neq$(_$T)(_$lhs, _$rhs); \
    }
#define __step__cmp_fn_neq_default$(...) __cmp_fn_neq_default$(__VA_ARGS__)
#define __step__cmp_fn_neq_default$__parse(_$T...) _$T, __step__cmp_fn_neq_default$__parseNext
#define __step__cmp_fn_neq_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_neq_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(neq)((_$T)(_$lhs, _$rhs)) { \
        return !cmp_eql$(_$T)(_$lhs, _$rhs); \
    }

#define __step__cmp_fn_eqlCtx_default$(...) __cmp_fn_eqlCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_eqlCtx_default$__parse(_$T...) _$T, __step__cmp_fn_eqlCtx_default$__parseNext
#define __step__cmp_fn_eqlCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_eqlCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(eqlCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        return !cmp_neqCtx$(_$T)(_$lhs, _$rhs, _$ctx); \
    }
#define __step__cmp_fn_neqCtx_default$(...) __cmp_fn_neqCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_neqCtx_default$__parse(_$T...) _$T, __step__cmp_fn_neqCtx_default$__parseNext
#define __step__cmp_fn_neqCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_neqCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(neqCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        return !cmp_eqlCtx$(_$T)(_$lhs, _$rhs, _$ctx); \
    }

#define __step__cmp_fn_ord_default$(...) __cmp_fn_ord_default$(__VA_ARGS__)
#define __step__cmp_fn_ord_default$__parse(_$T...) _$T, __step__cmp_fn_ord_default$__parseNext
#define __step__cmp_fn_ord_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_ord_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(ord)((_$T)(_$lhs, _$rhs)) { \
        return cmp_lt$(_$T)(_$lhs, _$rhs) ? cmp_Ord_lt \
             : (pri_swap(&_$lhs, &_$rhs), cmp_lt$(_$T)(_$lhs, _$rhs)) \
                 ? cmp_Ord_gt \
                 : cmp_Ord_eq; \
    }
#define __step__cmp_fn_eq_default$(...) __cmp_fn_eq_default$(__VA_ARGS__)
#define __step__cmp_fn_eq_default$__parse(_$T...) _$T, __step__cmp_fn_eq_default$__parseNext
#define __step__cmp_fn_eq_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_eq_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(eq)((_$T)(_$lhs, _$rhs)) { \
        return cmp_ord$(_$T)(_$lhs, _$rhs) == cmp_Ord_eq; \
    }
#define __step__cmp_fn_ne_default$(...) __cmp_fn_ne_default$(__VA_ARGS__)
#define __step__cmp_fn_ne_default$__parse(_$T...) _$T, __step__cmp_fn_ne_default$__parseNext
#define __step__cmp_fn_ne_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_ne_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(ne)((_$T)(_$lhs, _$rhs)) { \
        return cmp_ord$(_$T)(_$lhs, _$rhs) != cmp_Ord_eq; \
    }
#define __step__cmp_fn_lt_default$(...) __cmp_fn_lt_default$(__VA_ARGS__)
#define __step__cmp_fn_lt_default$__parse(_$T...) _$T, __step__cmp_fn_lt_default$__parseNext
#define __step__cmp_fn_lt_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_lt_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(lt)((_$T)(_$lhs, _$rhs)) { \
        return cmp_ord$(_$T)(_$lhs, _$rhs) < cmp_Ord_eq; \
    }
#define __step__cmp_fn_gt_default$(...) __cmp_fn_gt_default$(__VA_ARGS__)
#define __step__cmp_fn_gt_default$__parse(_$T...) _$T, __step__cmp_fn_gt_default$__parseNext
#define __step__cmp_fn_gt_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_gt_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(gt)((_$T)(_$lhs, _$rhs)) { \
        return cmp_ord$(_$T)(_$lhs, _$rhs) > cmp_Ord_eq; \
    }
#define __step__cmp_fn_le_default$(...) __cmp_fn_le_default$(__VA_ARGS__)
#define __step__cmp_fn_le_default$__parse(_$T...) _$T, __step__cmp_fn_le_default$__parseNext
#define __step__cmp_fn_le_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_le_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(le)((_$T)(_$lhs, _$rhs)) { \
        return cmp_ord$(_$T)(_$lhs, _$rhs) <= cmp_Ord_eq; \
    }
#define __step__cmp_fn_ge_default$(...) __cmp_fn_ge_default$(__VA_ARGS__)
#define __step__cmp_fn_ge_default$__parse(_$T...) _$T, __step__cmp_fn_ge_default$__parseNext
#define __step__cmp_fn_ge_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_ge_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(ge)((_$T)(_$lhs, _$rhs)) { \
        return cmp_ord$(_$T)(_$lhs, _$rhs) >= cmp_Ord_eq; \
    }

#define __step__cmp_fn_ordCtx_default$(...) __cmp_fn_ordCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_ordCtx_default$__parse(_$T...) _$T, __step__cmp_fn_ordCtx_default$__parseNext
#define __step__cmp_fn_ordCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_ordCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(ordCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        return cmp_ltCtx$(_$T)(_$lhs, _$rhs, _$ctx) ? cmp_Ord_lt \
             : (pri_swap(&_$lhs, &_$rhs), cmp_ltCtx$(_$T)(_$lhs, _$rhs, _$ctx)) \
                 ? cmp_Ord_gt \
                 : cmp_Ord_eq; \
    }
#define __step__cmp_fn_eqCtx_default$(...) __cmp_fn_eqCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_eqCtx_default$__parse(_$T...) _$T, __step__cmp_fn_eqCtx_default$__parseNext
#define __step__cmp_fn_eqCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_eqCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(eqCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        return cmp_ordCtx$(_$T)(_$lhs, _$rhs, _$ctx) == cmp_Ord_eq; \
    }
#define __step__cmp_fn_neCtx_default$(...) __cmp_fn_neCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_neCtx_default$__parse(_$T...) _$T, __step__cmp_fn_neCtx_default$__parseNext
#define __step__cmp_fn_neCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_neCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(neCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        return cmp_ordCtx$(_$T)(_$lhs, _$rhs, _$ctx) != cmp_Ord_eq; \
    }
#define __step__cmp_fn_ltCtx_default$(...) __cmp_fn_ltCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_ltCtx_default$__parse(_$T...) _$T, __step__cmp_fn_ltCtx_default$__parseNext
#define __step__cmp_fn_ltCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_ltCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(ltCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        return cmp_ordCtx$(_$T)(_$lhs, _$rhs, _$ctx) < cmp_Ord_eq; \
    }
#define __step__cmp_fn_gtCtx_default$(...) __cmp_fn_gtCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_gtCtx_default$__parse(_$T...) _$T, __step__cmp_fn_gtCtx_default$__parseNext
#define __step__cmp_fn_gtCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_gtCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(gtCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        return cmp_ordCtx$(_$T)(_$lhs, _$rhs, _$ctx) > cmp_Ord_eq; \
    }
#define __step__cmp_fn_leCtx_default$(...) __cmp_fn_leCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_leCtx_default$__parse(_$T...) _$T, __step__cmp_fn_leCtx_default$__parseNext
#define __step__cmp_fn_leCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_leCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(leCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        return cmp_ordCtx$(_$T)(_$lhs, _$rhs, _$ctx) <= cmp_Ord_eq; \
    }
#define __step__cmp_fn_geCtx_default$(...) __cmp_fn_geCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_geCtx_default$__parse(_$T...) _$T, __step__cmp_fn_geCtx_default$__parseNext
#define __step__cmp_fn_geCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_geCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(geCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        return cmp_ordCtx$(_$T)(_$lhs, _$rhs, _$ctx) >= cmp_Ord_eq; \
    }

#define __step__cmp_fn_ordApx_default$(...) __cmp_fn_ordApx_default$(__VA_ARGS__)
#define __step__cmp_fn_ordApx_default$__parse(_$T...) _$T, __step__cmp_fn_ordApx_default$__parseNext
#define __step__cmp_fn_ordApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_ordApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(ordApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        claim_assert(_$mode == cmp_ApxMode_abs || _$mode == cmp_ApxMode_rel); \
        return _$mode == cmp_ApxMode_abs ? cmp_ordApxAbs$(_$T)(_$lhs, _$rhs, _$threshold) \
                                         : cmp_ordApxRel$(_$T)(_$lhs, _$rhs, _$threshold); \
    }
#define __step__cmp_fn_eqApx_default$(...) __cmp_fn_eqApx_default$(__VA_ARGS__)
#define __step__cmp_fn_eqApx_default$__parse(_$T...) _$T, __step__cmp_fn_eqApx_default$__parseNext
#define __step__cmp_fn_eqApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_eqApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(eqApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        return cmp_ordApx$(_$T)(_$lhs, _$rhs, _$threshold, _$mode) == cmp_Ord_eq; \
    }
#define __step__cmp_fn_neApx_default$(...) __cmp_fn_neApx_default$(__VA_ARGS__)
#define __step__cmp_fn_neApx_default$__parse(_$T...) _$T, __step__cmp_fn_neApx_default$__parseNext
#define __step__cmp_fn_neApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_neApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(neApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        return cmp_ordApx$(_$T)(_$lhs, _$rhs, _$threshold, _$mode) != cmp_Ord_eq; \
    }
#define __step__cmp_fn_ltApx_default$(...) __cmp_fn_ltApx_default$(__VA_ARGS__)
#define __step__cmp_fn_ltApx_default$__parse(_$T...) _$T, __step__cmp_fn_ltApx_default$__parseNext
#define __step__cmp_fn_ltApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_ltApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(ltApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        return cmp_ordApx$(_$T)(_$lhs, _$rhs, _$threshold, _$mode) < cmp_Ord_eq; \
    }
#define __step__cmp_fn_gtApx_default$(...) __cmp_fn_gtApx_default$(__VA_ARGS__)
#define __step__cmp_fn_gtApx_default$__parse(_$T...) _$T, __step__cmp_fn_gtApx_default$__parseNext
#define __step__cmp_fn_gtApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_gtApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(gtApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        return cmp_ordApx$(_$T)(_$lhs, _$rhs, _$threshold, _$mode) > cmp_Ord_eq; \
    }
#define __step__cmp_fn_leApx_default$(...) __cmp_fn_leApx_default$(__VA_ARGS__)
#define __step__cmp_fn_leApx_default$__parse(_$T...) _$T, __step__cmp_fn_leApx_default$__parseNext
#define __step__cmp_fn_leApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_leApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(leApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        return cmp_ordApx$(_$T)(_$lhs, _$rhs, _$threshold, _$mode) <= cmp_Ord_eq; \
    }
#define __step__cmp_fn_geApx_default$(...) __cmp_fn_geApx_default$(__VA_ARGS__)
#define __step__cmp_fn_geApx_default$__parse(_$T...) _$T, __step__cmp_fn_geApx_default$__parseNext
#define __step__cmp_fn_geApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_geApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(geApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        return cmp_ordApx$(_$T)(_$lhs, _$rhs, _$threshold, _$mode) >= cmp_Ord_eq; \
    }

#define __cmp_fn_ordApx3_default$(_ordOp, _ltOp, _$T, _$lhs, _$rhs, _$threshold...) \
    __cmp_fn_Apx3$(_ordOp, cmp_Ord, _$T, _$lhs, _$rhs, _$threshold) { \
        return tpl_(_$T, _ltOp)(_$lhs, _$rhs, _$threshold) ? cmp_Ord_lt \
             : (pri_swap(&_$lhs, &_$rhs), tpl_(_$T, _ltOp)(_$lhs, _$rhs, _$threshold)) \
                 ? cmp_Ord_gt \
                 : cmp_Ord_eq; \
    }
#define __cmp_fn_relApx3_default$(_$op, _ordOp, _pred, _$T, _$lhs, _$rhs, _$threshold...) \
    __cmp_fn_Apx3$(_$op, bool, _$T, _$lhs, _$rhs, _$threshold) { \
        return _pred(tpl_(_$T, _ordOp)(_$lhs, _$rhs, _$threshold)); \
    }

#define __step__cmp_fn_ordApxAbs_default$(...) __cmp_fn_ordApx3_default$(ordApxAbs, ltApxAbs, __VA_ARGS__)
#define __step__cmp_fn_ordApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_eqApxAbs_default$(...) __cmp_fn_relApx3_default$(eqApxAbs, ordApxAbs, cmp_Ord_isEq, __VA_ARGS__)
#define __step__cmp_fn_eqApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_neApxAbs_default$(...) __cmp_fn_relApx3_default$(neApxAbs, ordApxAbs, cmp_Ord_isNe, __VA_ARGS__)
#define __step__cmp_fn_neApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_ltApxAbs_default$(...) __cmp_fn_relApx3_default$(ltApxAbs, ordApxAbs, cmp_Ord_isLt, __VA_ARGS__)
#define __step__cmp_fn_ltApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_gtApxAbs_default$(...) __cmp_fn_relApx3_default$(gtApxAbs, ordApxAbs, cmp_Ord_isGt, __VA_ARGS__)
#define __step__cmp_fn_gtApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_leApxAbs_default$(...) __cmp_fn_relApx3_default$(leApxAbs, ordApxAbs, cmp_Ord_isLe, __VA_ARGS__)
#define __step__cmp_fn_leApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_geApxAbs_default$(...) __cmp_fn_relApx3_default$(geApxAbs, ordApxAbs, cmp_Ord_isGe, __VA_ARGS__)
#define __step__cmp_fn_geApxAbs_default$__parse __step__cmp_fn_Apx3$__parse

#define __step__cmp_fn_ordApxRel_default$(...) __cmp_fn_ordApx3_default$(ordApxRel, ltApxRel, __VA_ARGS__)
#define __step__cmp_fn_ordApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_eqApxRel_default$(...) __cmp_fn_relApx3_default$(eqApxRel, ordApxRel, cmp_Ord_isEq, __VA_ARGS__)
#define __step__cmp_fn_eqApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_neApxRel_default$(...) __cmp_fn_relApx3_default$(neApxRel, ordApxRel, cmp_Ord_isNe, __VA_ARGS__)
#define __step__cmp_fn_neApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_ltApxRel_default$(...) __cmp_fn_relApx3_default$(ltApxRel, ordApxRel, cmp_Ord_isLt, __VA_ARGS__)
#define __step__cmp_fn_ltApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_gtApxRel_default$(...) __cmp_fn_relApx3_default$(gtApxRel, ordApxRel, cmp_Ord_isGt, __VA_ARGS__)
#define __step__cmp_fn_gtApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_leApxRel_default$(...) __cmp_fn_relApx3_default$(leApxRel, ordApxRel, cmp_Ord_isLe, __VA_ARGS__)
#define __step__cmp_fn_leApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_geApxRel_default$(...) __cmp_fn_relApx3_default$(geApxRel, ordApxRel, cmp_Ord_isGe, __VA_ARGS__)
#define __step__cmp_fn_geApxRel_default$__parse __step__cmp_fn_Apx3$__parse

#define __step__cmp_fn_u_eql_default$(...) __cmp_fn_u_eql_default$(__VA_ARGS__)
#define __step__cmp_fn_u_eql_default$__parse(_$T...) _$T, __step__cmp_fn_u_eql_default$__parseNext
#define __step__cmp_fn_u_eql_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_eql_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(u_eql)((_$T)(_$lhs, _$rhs)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_eql$(_$T)(l, r); \
    }
#define __step__cmp_fn_u_neq_default$(...) __cmp_fn_u_neq_default$(__VA_ARGS__)
#define __step__cmp_fn_u_neq_default$__parse(_$T...) _$T, __step__cmp_fn_u_neq_default$__parseNext
#define __step__cmp_fn_u_neq_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_neq_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(u_neq)((_$T)(_$lhs, _$rhs)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_neq$(_$T)(l, r); \
    }

#define __step__cmp_fn_u_eqlCtx_default$(...) __cmp_fn_u_eqlCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_eqlCtx_default$__parse(_$T...) _$T, __step__cmp_fn_u_eqlCtx_default$__parseNext
#define __step__cmp_fn_u_eqlCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_eqlCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(u_eqlCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_eqlCtx$(_$T)(l, r, _$ctx); \
    }
#define __step__cmp_fn_u_neqCtx_default$(...) __cmp_fn_u_neqCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_neqCtx_default$__parse(_$T...) _$T, __step__cmp_fn_u_neqCtx_default$__parseNext
#define __step__cmp_fn_u_neqCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_neqCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(u_neqCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_neqCtx$(_$T)(l, r, _$ctx); \
    }

#define __step__cmp_fn_u_ord_default$(...) __cmp_fn_u_ord_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ord_default$__parse(_$T...) _$T, __step__cmp_fn_u_ord_default$__parseNext
#define __step__cmp_fn_u_ord_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_ord_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(u_ord)((_$T)(_$lhs, _$rhs)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_ord$(_$T)(l, r); \
    }
#define __step__cmp_fn_u_eq_default$(...) __cmp_fn_u_eq_default$(__VA_ARGS__)
#define __step__cmp_fn_u_eq_default$__parse(_$T...) _$T, __step__cmp_fn_u_eq_default$__parseNext
#define __step__cmp_fn_u_eq_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_eq_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(u_eq)((_$T)(_$lhs, _$rhs)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_eq$(_$T)(l, r); \
    }
#define __step__cmp_fn_u_ne_default$(...) __cmp_fn_u_ne_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ne_default$__parse(_$T...) _$T, __step__cmp_fn_u_ne_default$__parseNext
#define __step__cmp_fn_u_ne_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_ne_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(u_ne)((_$T)(_$lhs, _$rhs)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_ne$(_$T)(l, r); \
    }
#define __step__cmp_fn_u_lt_default$(...) __cmp_fn_u_lt_default$(__VA_ARGS__)
#define __step__cmp_fn_u_lt_default$__parse(_$T...) _$T, __step__cmp_fn_u_lt_default$__parseNext
#define __step__cmp_fn_u_lt_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_lt_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(u_lt)((_$T)(_$lhs, _$rhs)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_lt$(_$T)(l, r); \
    }
#define __step__cmp_fn_u_gt_default$(...) __cmp_fn_u_gt_default$(__VA_ARGS__)
#define __step__cmp_fn_u_gt_default$__parse(_$T...) _$T, __step__cmp_fn_u_gt_default$__parseNext
#define __step__cmp_fn_u_gt_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_gt_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(u_gt)((_$T)(_$lhs, _$rhs)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_gt$(_$T)(l, r); \
    }
#define __step__cmp_fn_u_le_default$(...) __cmp_fn_u_le_default$(__VA_ARGS__)
#define __step__cmp_fn_u_le_default$__parse(_$T...) _$T, __step__cmp_fn_u_le_default$__parseNext
#define __step__cmp_fn_u_le_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_le_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(u_le)((_$T)(_$lhs, _$rhs)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_le$(_$T)(l, r); \
    }
#define __step__cmp_fn_u_ge_default$(...) __cmp_fn_u_ge_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ge_default$__parse(_$T...) _$T, __step__cmp_fn_u_ge_default$__parseNext
#define __step__cmp_fn_u_ge_default$__parseNext(_$lhs, _$rhs...) _$lhs, _$rhs
#define __cmp_fn_u_ge_default$(_$T, _$lhs, _$rhs...) \
    cmp_fn_(u_ge)((_$T)(_$lhs, _$rhs)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_ge$(_$T)(l, r); \
    }

#define __step__cmp_fn_u_ordCtx_default$(...) __cmp_fn_u_ordCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ordCtx_default$__parse(_$T...) _$T, __step__cmp_fn_u_ordCtx_default$__parseNext
#define __step__cmp_fn_u_ordCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_ordCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(u_ordCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_ordCtx$(_$T)(l, r, _$ctx); \
    }
#define __step__cmp_fn_u_eqCtx_default$(...) __cmp_fn_u_eqCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_eqCtx_default$__parse(_$T...) _$T, __step__cmp_fn_u_eqCtx_default$__parseNext
#define __step__cmp_fn_u_eqCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_eqCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(u_eqCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_eqCtx$(_$T)(l, r, _$ctx); \
    }
#define __step__cmp_fn_u_neCtx_default$(...) __cmp_fn_u_neCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_neCtx_default$__parse(_$T...) _$T, __step__cmp_fn_u_neCtx_default$__parseNext
#define __step__cmp_fn_u_neCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_neCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(u_neqCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_neqCtx$(_$T)(l, r, _$ctx); \
    }
#define __step__cmp_fn_u_ltCtx_default$(...) __cmp_fn_u_ltCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ltCtx_default$__parse(_$T...) _$T, __step__cmp_fn_u_ltCtx_default$__parseNext
#define __step__cmp_fn_u_ltCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_ltCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(u_ltCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_ltCtx$(_$T)(l, r, _$ctx); \
    }
#define __step__cmp_fn_u_gtCtx_default$(...) __cmp_fn_u_gtCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_gtCtx_default$__parse(_$T...) _$T, __step__cmp_fn_u_gtCtx_default$__parseNext
#define __step__cmp_fn_u_gtCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_gtCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(u_gtCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_gtCtx$(_$T)(l, r, _$ctx); \
    }
#define __step__cmp_fn_u_leCtx_default$(...) __cmp_fn_u_leCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_leCtx_default$__parse(_$T...) _$T, __step__cmp_fn_u_leCtx_default$__parseNext
#define __step__cmp_fn_u_leCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_leCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(u_leCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_leCtx$(_$T)(l, r, _$ctx); \
    }
#define __step__cmp_fn_u_geCtx_default$(...) __cmp_fn_u_geCtx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_geCtx_default$__parse(_$T...) _$T, __step__cmp_fn_u_geCtx_default$__parseNext
#define __step__cmp_fn_u_geCtx_default$__parseNext(_$lhs, _$rhs, _$ctx...) _$lhs, _$rhs, _$ctx
#define __cmp_fn_u_geCtx_default$(_$T, _$lhs, _$rhs, _$ctx...) \
    cmp_fn_(u_geCtx)((_$T)(_$lhs, _$rhs, _$ctx)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        return cmp_geCtx$(_$T)(l, r, _$ctx); \
    }

#define __step__cmp_fn_u_ordApx_default$(...) __cmp_fn_u_ordApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ordApx_default$__parse(_$T...) _$T, __step__cmp_fn_u_ordApx_default$__parseNext
#define __step__cmp_fn_u_ordApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_ordApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(u_ordApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        let t = u_castV$((_$T)(_$threshold)); \
        return cmp_ordApx$(_$T)(l, r, t, _$mode); \
    }
#define __step__cmp_fn_u_eqApx_default$(...) __cmp_fn_u_eqApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_eqApx_default$__parse(_$T...) _$T, __step__cmp_fn_u_eqApx_default$__parseNext
#define __step__cmp_fn_u_eqApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_eqApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(u_eqApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        let t = u_castV$((_$T)(_$threshold)); \
        return cmp_eqApx$(_$T)(l, r, t, _$mode); \
    }
#define __step__cmp_fn_u_neApx_default$(...) __cmp_fn_u_neApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_neApx_default$__parse(_$T...) _$T, __step__cmp_fn_u_neApx_default$__parseNext
#define __step__cmp_fn_u_neApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_neApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(u_neApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        let t = u_castV$((_$T)(_$threshold)); \
        return cmp_neApx$(_$T)(l, r, t, _$mode); \
    }
#define __step__cmp_fn_u_ltApx_default$(...) __cmp_fn_u_ltApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_ltApx_default$__parse(_$T...) _$T, __step__cmp_fn_u_ltApx_default$__parseNext
#define __step__cmp_fn_u_ltApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_ltApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(u_ltApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        let t = u_castV$((_$T)(_$threshold)); \
        return cmp_ltApx$(_$T)(l, r, t, _$mode); \
    }
#define __step__cmp_fn_u_gtApx_default$(...) __cmp_fn_u_gtApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_gtApx_default$__parse(_$T...) _$T, __step__cmp_fn_u_gtApx_default$__parseNext
#define __step__cmp_fn_u_gtApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_gtApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(u_gtApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        let t = u_castV$((_$T)(_$threshold)); \
        return cmp_gtApx$(_$T)(l, r, t, _$mode); \
    }
#define __step__cmp_fn_u_leApx_default$(...) __cmp_fn_u_leApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_leApx_default$__parse(_$T...) _$T, __step__cmp_fn_u_leApx_default$__parseNext
#define __step__cmp_fn_u_leApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_leApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(u_leApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        let t = u_castV$((_$T)(_$threshold)); \
        return cmp_leApx$(_$T)(l, r, t, _$mode); \
    }
#define __step__cmp_fn_u_geApx_default$(...) __cmp_fn_u_geApx_default$(__VA_ARGS__)
#define __step__cmp_fn_u_geApx_default$__parse(_$T...) _$T, __step__cmp_fn_u_geApx_default$__parseNext
#define __step__cmp_fn_u_geApx_default$__parseNext(_$lhs, _$rhs, _$threshold, _$mode...) _$lhs, _$rhs, _$threshold, _$mode
#define __cmp_fn_u_geApx_default$(_$T, _$lhs, _$rhs, _$threshold, _$mode...) \
    cmp_fn_(u_geApx)((_$T)(_$lhs, _$rhs, _$threshold, _$mode)) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        let t = u_castV$((_$T)(_$threshold)); \
        return cmp_geApx$(_$T)(l, r, t, _$mode); \
    }

#define __cmp_fn_u_Apx3_default$(_uOp, _$op, _Ret, _$T, _$lhs, _$rhs, _$threshold...) \
    __cmp_fn_u_Apx3$(_uOp, _Ret, _$T, _$lhs, _$rhs, _$threshold) { \
        let l = u_castV$((_$T)(_$lhs)); \
        let r = u_castV$((_$T)(_$rhs)); \
        let t = u_castV$((_$T)(_$threshold)); \
        return tpl_(_$T, _$op)(l, r, t); \
    }

#define __step__cmp_fn_u_ordApxAbs_default$(...) __cmp_fn_u_Apx3_default$(u_ordApxAbs, ordApxAbs, cmp_Ord, __VA_ARGS__)
#define __step__cmp_fn_u_ordApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_eqApxAbs_default$(...) __cmp_fn_u_Apx3_default$(u_eqApxAbs, eqApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_eqApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_neApxAbs_default$(...) __cmp_fn_u_Apx3_default$(u_neApxAbs, neApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_neApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_ltApxAbs_default$(...) __cmp_fn_u_Apx3_default$(u_ltApxAbs, ltApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_ltApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_gtApxAbs_default$(...) __cmp_fn_u_Apx3_default$(u_gtApxAbs, gtApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_gtApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_leApxAbs_default$(...) __cmp_fn_u_Apx3_default$(u_leApxAbs, leApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_leApxAbs_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_geApxAbs_default$(...) __cmp_fn_u_Apx3_default$(u_geApxAbs, geApxAbs, bool, __VA_ARGS__)
#define __step__cmp_fn_u_geApxAbs_default$__parse __step__cmp_fn_Apx3$__parse

#define __step__cmp_fn_u_ordApxRel_default$(...) __cmp_fn_u_Apx3_default$(u_ordApxRel, ordApxRel, cmp_Ord, __VA_ARGS__)
#define __step__cmp_fn_u_ordApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_eqApxRel_default$(...) __cmp_fn_u_Apx3_default$(u_eqApxRel, eqApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_eqApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_neApxRel_default$(...) __cmp_fn_u_Apx3_default$(u_neApxRel, neApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_neApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_ltApxRel_default$(...) __cmp_fn_u_Apx3_default$(u_ltApxRel, ltApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_ltApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_gtApxRel_default$(...) __cmp_fn_u_Apx3_default$(u_gtApxRel, gtApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_gtApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_leApxRel_default$(...) __cmp_fn_u_Apx3_default$(u_leApxRel, leApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_leApxRel_default$__parse __step__cmp_fn_Apx3$__parse
#define __step__cmp_fn_u_geApxRel_default$(...) __cmp_fn_u_Apx3_default$(u_geApxRel, geApxRel, bool, __VA_ARGS__)
#define __step__cmp_fn_u_geApxRel_default$__parse __step__cmp_fn_Apx3$__parse

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_cmp__included */
