/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cmp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-09 (date of creation)
 * @updated 2026-03-11 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  cmp
 */
#ifndef cmp__included
#define cmp__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations =========================================*/

/// math Type (Scalar)
/// @see dh/m-math/base-T.h
T_alias$((cmp_m_T)(enum_((cmp_m_T $fits($packed))(
    cmp_m_T_u8 = 0,
    cmp_m_T_i8 = 1,
    cmp_m_T_u16 = 2,
    cmp_m_T_i16 = 3,
    cmp_m_T_u32 = 4,
    cmp_m_T_i32 = 5,
    cmp_m_T_u64 = 6,
    cmp_m_T_i64 = 7,
    cmp_m_T_usize = pp_if_(arch_bits_is_64bit)(
        pp_then_(cmp_m_T_u64), pp_else_(cmp_m_T_u32)
    ),
    cmp_m_T_isize = pp_if_(arch_bits_is_64bit)(
        pp_then_(cmp_m_T_i64), pp_else_(cmp_m_T_i32)
    ),
    cmp_m_T_f32 = 8,
    cmp_m_T_f64 = 9
))));

T_alias$((cmp_EqlFn)(u_EqlFn));
$extern fn_((cmp_EqlFn_default(cmp_m_T type))(cmp_EqlFn));
$extern fn_((cmp_eql(u_V$raw lhs, u_V$raw rhs, cmp_EqlFn eqlFn))(bool));
$extern fn_((cmp_eqlP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_EqlFn eqlFn))(bool));
$extern fn_((cmp_eqlS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_EqlFn eqlFn))(bool));
$extern fn_((cmp_neq(u_V$raw lhs, u_V$raw rhs, cmp_EqlFn eqlFn))(bool));
$extern fn_((cmp_neqP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_EqlFn eqlFn))(bool));
$extern fn_((cmp_neqS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_EqlFn eqlFn))(bool));
T_alias$((cmp_EqlCtxFn)(u_EqlCtxFn));
$extern fn_((cmp_EqlCtxFn_default(cmp_m_T type))(cmp_EqlCtxFn));
$extern fn_((cmp_eqlCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool));
$extern fn_((cmp_eqlCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool));
$extern fn_((cmp_eqlCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool));
$extern fn_((cmp_neqCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool));
$extern fn_((cmp_neqCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool));
$extern fn_((cmp_neqCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool));

T_alias$((cmp_EqlNoCtxFnAsCtx)(struct cmp_EqlNoCtxFnAsCtx {
    var_(eqlFn, cmp_EqlFn);
}));
$extern fn_((cmp_eqlNoCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool));

T_alias$((cmp_Dir)(enum_((cmp_Dir $fits($packed))(
    cmp_Dir_asc = 0,
    cmp_Dir_desc = 1
))));

T_alias$((cmp_OrdFn)(u_OrdFn));
$extern fn_((cmp_OrdFn_default(cmp_m_T type, cmp_Dir dir))(cmp_OrdFn));
$extern fn_((cmp_OrdFn_defaultAsc(cmp_m_T type))(cmp_OrdFn));
$extern fn_((cmp_OrdFn_defaultDesc(cmp_m_T type))(cmp_OrdFn));
$extern fn_((cmp_ord(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(cmp_Ord));
$extern fn_((cmp_ordP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(cmp_Ord));
$extern fn_((cmp_ordS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(cmp_Ord));
$extern fn_((cmp_eq(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_eqP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_eqS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_ne(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_neP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_neS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_lt(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_ltP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_ltS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_gt(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_gtP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_gtS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_le(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_leP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_leS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_ge(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_geP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool));
$extern fn_((cmp_geS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool));
T_alias$((cmp_OrdCtxFn)(u_OrdCtxFn));
$extern fn_((cmp_OrdCtxFn_default(cmp_m_T type, cmp_Dir dir))(cmp_OrdCtxFn));
$extern fn_((cmp_OrdCtxFn_defaultAsc(cmp_m_T type))(cmp_OrdCtxFn));
$extern fn_((cmp_OrdCtxFn_defaultDesc(cmp_m_T type))(cmp_OrdCtxFn));
$extern fn_((cmp_ordCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(cmp_Ord));
$extern fn_((cmp_ordCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(cmp_Ord));
$extern fn_((cmp_ordCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(cmp_Ord));
$extern fn_((cmp_eqCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_eqCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_eqCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_neCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_neCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_neCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_ltCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_ltCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_ltCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_gtCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_gtCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_gtCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_leCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_leCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_leCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_geCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_geCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));
$extern fn_((cmp_geCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool));

T_alias$((cmp_OrdNoCtxFnAsCtx)(struct cmp_OrdNoCtxFnAsCtx {
    var_(ordFn, cmp_OrdFn);
}));
$extern fn_((cmp_ordNoCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* cmp__included */
