#include "dh/cmp.h"

$static fn_((cmp_EqlFn_default$u8(u_V$raw lhs, u_V$raw rhs))(bool)) { return pri_eql(u_castV$((u8)(lhs)), u_castV$((u8)(rhs))); };
$static fn_((cmp_EqlFn_default$u16(u_V$raw lhs, u_V$raw rhs))(bool)) { return pri_eql(u_castV$((u16)(lhs)), u_castV$((u16)(rhs))); };
$static fn_((cmp_EqlFn_default$u32(u_V$raw lhs, u_V$raw rhs))(bool)) { return pri_eql(u_castV$((u32)(lhs)), u_castV$((u32)(rhs))); };
$static fn_((cmp_EqlFn_default$u64(u_V$raw lhs, u_V$raw rhs))(bool)) { return pri_eql(u_castV$((u64)(lhs)), u_castV$((u64)(rhs))); };
$static fn_((cmp_EqlFn_default$i8(u_V$raw lhs, u_V$raw rhs))(bool)) { return pri_eql(u_castV$((i8)(lhs)), u_castV$((i8)(rhs))); };
$static fn_((cmp_EqlFn_default$i16(u_V$raw lhs, u_V$raw rhs))(bool)) { return pri_eql(u_castV$((i16)(lhs)), u_castV$((i16)(rhs))); };
$static fn_((cmp_EqlFn_default$i32(u_V$raw lhs, u_V$raw rhs))(bool)) { return pri_eql(u_castV$((i32)(lhs)), u_castV$((i32)(rhs))); };
$static fn_((cmp_EqlFn_default$i64(u_V$raw lhs, u_V$raw rhs))(bool)) { return pri_eql(u_castV$((i64)(lhs)), u_castV$((i64)(rhs))); };
$static fn_((cmp_EqlFn_default$f32(u_V$raw lhs, u_V$raw rhs))(bool)) { return pri_eql(u_castV$((f32)(lhs)), u_castV$((f32)(rhs))); };
$static fn_((cmp_EqlFn_default$f64(u_V$raw lhs, u_V$raw rhs))(bool)) { return pri_eql(u_castV$((f64)(lhs)), u_castV$((f64)(rhs))); };
fn_((cmp_EqlFn_default(cmp_m_T type))(cmp_EqlFn)) {
    switch (type) {
    case cmp_m_T_u8: return cmp_EqlFn_default$u8;
    case cmp_m_T_u16: return cmp_EqlFn_default$u16;
    case cmp_m_T_u32: return cmp_EqlFn_default$u32;
    case cmp_m_T_u64: return cmp_EqlFn_default$u64;
    case cmp_m_T_i8: return cmp_EqlFn_default$i8;
    case cmp_m_T_i16: return cmp_EqlFn_default$i16;
    case cmp_m_T_i32: return cmp_EqlFn_default$i32;
    case cmp_m_T_i64: return cmp_EqlFn_default$i64;
    case cmp_m_T_f32: return cmp_EqlFn_default$f32;
    case cmp_m_T_f64: return cmp_EqlFn_default$f64;
    }
};

fn_((cmp_eql(u_V$raw lhs, u_V$raw rhs, cmp_EqlFn eqlFn))(bool)) {
    let_(no_ctx, cmp_EqlNoCtxFnAsCtx) = { .eqlFn = eqlFn };
    return cmp_eqlCtx(lhs, rhs, u_anyV(no_ctx), cmp_eqlNoCtx);
};
fn_((cmp_eqlP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_EqlFn eqlFn))(bool)) {
    let_(no_ctx, cmp_EqlNoCtxFnAsCtx) = { .eqlFn = eqlFn };
    return cmp_eqlCtxP(lhs, rhs, u_anyV(no_ctx), cmp_eqlNoCtx);
}
fn_((cmp_eqlS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_EqlFn eqlFn))(bool)) {
    let_(no_ctx, cmp_EqlNoCtxFnAsCtx) = { .eqlFn = eqlFn };
    return cmp_eqlCtxS(lhs, rhs, u_anyV(no_ctx), cmp_eqlNoCtx);
};
fn_((cmp_neq(u_V$raw lhs, u_V$raw rhs, cmp_EqlFn eqlFn))(bool)) { return !cmp_eql(lhs, rhs, eqlFn); };
fn_((cmp_neqP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_EqlFn eqlFn))(bool)) { return !cmp_eqlP(lhs, rhs, eqlFn); };
fn_((cmp_neqS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_EqlFn eqlFn))(bool)) { return !cmp_eqlS(lhs, rhs, eqlFn); };

$static fn_((cmp_EqlCtxFn_default$u8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, cmp_EqlFn_default$u8(lhs, rhs); };
$static fn_((cmp_EqlCtxFn_default$u16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, cmp_EqlFn_default$u16(lhs, rhs); };
$static fn_((cmp_EqlCtxFn_default$u32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, cmp_EqlFn_default$u32(lhs, rhs); };
$static fn_((cmp_EqlCtxFn_default$u64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, cmp_EqlFn_default$u64(lhs, rhs); };
$static fn_((cmp_EqlCtxFn_default$i8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, cmp_EqlFn_default$i8(lhs, rhs); };
$static fn_((cmp_EqlCtxFn_default$i16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, cmp_EqlFn_default$i16(lhs, rhs); };
$static fn_((cmp_EqlCtxFn_default$i32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, cmp_EqlFn_default$i32(lhs, rhs); };
$static fn_((cmp_EqlCtxFn_default$i64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, cmp_EqlFn_default$i64(lhs, rhs); };
$static fn_((cmp_EqlCtxFn_default$f32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, cmp_EqlFn_default$f32(lhs, rhs); };
$static fn_((cmp_EqlCtxFn_default$f64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, cmp_EqlFn_default$f64(lhs, rhs); };
fn_((cmp_EqlCtxFn_default(cmp_m_T type))(cmp_EqlCtxFn)) {
    switch (type) {
    case cmp_m_T_u8: return cmp_EqlCtxFn_default$u8;
    case cmp_m_T_u16: return cmp_EqlCtxFn_default$u16;
    case cmp_m_T_u32: return cmp_EqlCtxFn_default$u32;
    case cmp_m_T_u64: return cmp_EqlCtxFn_default$u64;
    case cmp_m_T_i8: return cmp_EqlCtxFn_default$i8;
    case cmp_m_T_i16: return cmp_EqlCtxFn_default$i16;
    case cmp_m_T_i32: return cmp_EqlCtxFn_default$i32;
    case cmp_m_T_i64: return cmp_EqlCtxFn_default$i64;
    case cmp_m_T_f32: return cmp_EqlCtxFn_default$f32;
    case cmp_m_T_f64: return cmp_EqlCtxFn_default$f64;
    }
};

fn_((cmp_eqlCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool)) {
    claim_assert_nonnull(lhs.inner), claim_assert_nonnull(rhs.inner), claim_assert_nonnull(ctx.inner);
    return eqlFn(u_load(lhs), u_load(rhs), u_load(ctx));
};
fn_((cmp_eqlCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool)) {
    claim_assert_nonnull(lhs.raw), claim_assert_nonnull(rhs.raw), claim_assert_nonnull(ctx.inner);
    return cmp_eqlCtx(u_deref(lhs), u_deref(rhs), ctx, eqlFn);
};
fn_((cmp_eqlCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs), claim_assert_nonnull(ctx.inner);
    for_(($us(lhs), $us(rhs))(l, r)) {
        if (cmp_eqlCtxP(l, r, ctx, eqlFn)) continue;
        return false;
    } $end(for);
    return true;
};
fn_((cmp_neqCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool)) { return !cmp_eqlCtx(lhs, rhs, ctx, eqlFn); };
fn_((cmp_neqCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool)) { return !cmp_eqlCtxP(lhs, rhs, ctx, eqlFn); };
fn_((cmp_neqCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_EqlCtxFn eqlFn))(bool)) { return !cmp_eqlCtxS(lhs, rhs, ctx, eqlFn); };

fn_((cmp_eqlNoCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) {
    claim_assert_nonnull(lhs.inner), claim_assert_nonnull(rhs.inner), claim_assert_nonnull(ctx.inner);
    let no_ctx = u_castV$((cmp_EqlNoCtxFnAsCtx)(ctx));
    return no_ctx.eqlFn(lhs, rhs);
};

fn_((cmp_OrdFn_default(cmp_m_T type, cmp_Dir dir))(cmp_OrdFn)) {
    switch (dir) {
    case cmp_Dir_asc: return cmp_OrdFn_defaultAsc(type);
    case cmp_Dir_desc: return cmp_OrdFn_defaultDesc(type);
    }
};

$static fn_((cmp_OrdFn_defaultAsc$u8(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return pri_ord(u_castV$((u8)(lhs)), u_castV$((u8)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$u16(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return pri_ord(u_castV$((u16)(lhs)), u_castV$((u16)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$u32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return pri_ord(u_castV$((u32)(lhs)), u_castV$((u32)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$u64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return pri_ord(u_castV$((u64)(lhs)), u_castV$((u64)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$i8(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return pri_ord(u_castV$((i8)(lhs)), u_castV$((i8)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$i16(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return pri_ord(u_castV$((i16)(lhs)), u_castV$((i16)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$i32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return pri_ord(u_castV$((i32)(lhs)), u_castV$((i32)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$i64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return pri_ord(u_castV$((i64)(lhs)), u_castV$((i64)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$f32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return pri_ord(u_castV$((f32)(lhs)), u_castV$((f32)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$f64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return pri_ord(u_castV$((f64)(lhs)), u_castV$((f64)(rhs))); };
fn_((cmp_OrdFn_defaultAsc(cmp_m_T type))(cmp_OrdFn)) {
    switch (type) {
    case cmp_m_T_u8: return cmp_OrdFn_defaultAsc$u8;
    case cmp_m_T_u16: return cmp_OrdFn_defaultAsc$u16;
    case cmp_m_T_u32: return cmp_OrdFn_defaultAsc$u32;
    case cmp_m_T_u64: return cmp_OrdFn_defaultAsc$u64;
    case cmp_m_T_i8: return cmp_OrdFn_defaultAsc$i8;
    case cmp_m_T_i16: return cmp_OrdFn_defaultAsc$i16;
    case cmp_m_T_i32: return cmp_OrdFn_defaultAsc$i32;
    case cmp_m_T_i64: return cmp_OrdFn_defaultAsc$i64;
    case cmp_m_T_f32: return cmp_OrdFn_defaultAsc$f32;
    case cmp_m_T_f64: return cmp_OrdFn_defaultAsc$f64;
    }
};

$static fn_((cmp_OrdFn_defaultDesc$u8(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$u8(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$u16(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$u16(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$u32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$u32(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$u64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$u64(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$i8(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$i8(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$i16(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$i16(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$i32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$i32(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$i64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$i64(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$f32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$f32(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$f64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$f64(lhs, rhs)); };
fn_((cmp_OrdFn_defaultDesc(cmp_m_T type))(cmp_OrdFn)) {
    switch (type) {
    case cmp_m_T_u8: return cmp_OrdFn_defaultDesc$u8;
    case cmp_m_T_u16: return cmp_OrdFn_defaultDesc$u16;
    case cmp_m_T_u32: return cmp_OrdFn_defaultDesc$u32;
    case cmp_m_T_u64: return cmp_OrdFn_defaultDesc$u64;
    case cmp_m_T_i8: return cmp_OrdFn_defaultDesc$i8;
    case cmp_m_T_i16: return cmp_OrdFn_defaultDesc$i16;
    case cmp_m_T_i32: return cmp_OrdFn_defaultDesc$i32;
    case cmp_m_T_i64: return cmp_OrdFn_defaultDesc$i64;
    case cmp_m_T_f32: return cmp_OrdFn_defaultDesc$f32;
    case cmp_m_T_f64: return cmp_OrdFn_defaultDesc$f64;
    }
};

fn_((cmp_ord(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(cmp_Ord)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    return cmp_ordCtx(lhs, rhs, u_anyV(no_ctx), (cmp_OrdCtxFn)(cmp_ordNoCtx));
};
fn_((cmp_ordP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(cmp_Ord)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    return cmp_ordCtxP(lhs, rhs, u_anyV(no_ctx), (cmp_OrdCtxFn)(cmp_ordNoCtx));
};
fn_((cmp_ordS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(cmp_Ord)) {
    let_(no_ctx, cmp_OrdNoCtxFnAsCtx) = { .ordFn = ordFn };
    return cmp_ordCtxS(lhs, rhs, u_anyV(no_ctx), (cmp_OrdCtxFn)(cmp_ordNoCtx));
};
fn_((cmp_eq(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isEq(cmp_ord(lhs, rhs, ordFn)); };
fn_((cmp_eqP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isEq(cmp_ordP(lhs, rhs, ordFn)); };
fn_((cmp_eqS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isEq(cmp_ordS(lhs, rhs, ordFn)); };
fn_((cmp_ne(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isNe(cmp_ord(lhs, rhs, ordFn)); };
fn_((cmp_neP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isNe(cmp_ordP(lhs, rhs, ordFn)); };
fn_((cmp_neS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isNe(cmp_ordS(lhs, rhs, ordFn)); };
fn_((cmp_lt(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isLt(cmp_ord(lhs, rhs, ordFn)); };
fn_((cmp_ltP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isLt(cmp_ordP(lhs, rhs, ordFn)); };
fn_((cmp_ltS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isLt(cmp_ordS(lhs, rhs, ordFn)); };
fn_((cmp_gt(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isGt(cmp_ord(lhs, rhs, ordFn)); };
fn_((cmp_gtP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isGt(cmp_ordP(lhs, rhs, ordFn)); };
fn_((cmp_gtS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isGt(cmp_ordS(lhs, rhs, ordFn)); };
fn_((cmp_le(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isLe(cmp_ord(lhs, rhs, ordFn)); };
fn_((cmp_leP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isLe(cmp_ordP(lhs, rhs, ordFn)); };
fn_((cmp_leS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isLe(cmp_ordS(lhs, rhs, ordFn)); };
fn_((cmp_ge(u_V$raw lhs, u_V$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isGe(cmp_ord(lhs, rhs, ordFn)); };
fn_((cmp_geP(u_P_const$raw lhs, u_P_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isGe(cmp_ordP(lhs, rhs, ordFn)); };
fn_((cmp_geS(u_S_const$raw lhs, u_S_const$raw rhs, cmp_OrdFn ordFn))(bool)) { return cmp_Ord_isGe(cmp_ordS(lhs, rhs, ordFn)); };

fn_((cmp_OrdCtxFn_default(cmp_m_T type, cmp_Dir dir))(cmp_OrdCtxFn)) {
    switch (dir) {
    case cmp_Dir_asc: return cmp_OrdCtxFn_defaultAsc(type);
    case cmp_Dir_desc: return cmp_OrdCtxFn_defaultDesc(type);
    }
};

$static fn_((cmp_OrdCtxFn_defaultAsc$u8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, cmp_OrdFn_defaultAsc$u8(lhs, rhs); };
$static fn_((cmp_OrdCtxFn_defaultAsc$u16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, cmp_OrdFn_defaultAsc$u16(lhs, rhs); };
$static fn_((cmp_OrdCtxFn_defaultAsc$u32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, cmp_OrdFn_defaultAsc$u32(lhs, rhs); };
$static fn_((cmp_OrdCtxFn_defaultAsc$u64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, cmp_OrdFn_defaultAsc$u64(lhs, rhs); };
$static fn_((cmp_OrdCtxFn_defaultAsc$i8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, cmp_OrdFn_defaultAsc$i8(lhs, rhs); };
$static fn_((cmp_OrdCtxFn_defaultAsc$i16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, cmp_OrdFn_defaultAsc$i16(lhs, rhs); };
$static fn_((cmp_OrdCtxFn_defaultAsc$i32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, cmp_OrdFn_defaultAsc$i32(lhs, rhs); };
$static fn_((cmp_OrdCtxFn_defaultAsc$i64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, cmp_OrdFn_defaultAsc$i64(lhs, rhs); };
$static fn_((cmp_OrdCtxFn_defaultAsc$f32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, cmp_OrdFn_defaultAsc$f32(lhs, rhs); };
$static fn_((cmp_OrdCtxFn_defaultAsc$f64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, cmp_OrdFn_defaultAsc$f64(lhs, rhs); };
fn_((cmp_OrdCtxFn_defaultAsc(cmp_m_T type))(cmp_OrdCtxFn)) {
    switch (type) {
    case cmp_m_T_u8: return cmp_OrdCtxFn_defaultAsc$u8;
    case cmp_m_T_u16: return cmp_OrdCtxFn_defaultAsc$u16;
    case cmp_m_T_u32: return cmp_OrdCtxFn_defaultAsc$u32;
    case cmp_m_T_u64: return cmp_OrdCtxFn_defaultAsc$u64;
    case cmp_m_T_i8: return cmp_OrdCtxFn_defaultAsc$i8;
    case cmp_m_T_i16: return cmp_OrdCtxFn_defaultAsc$i16;
    case cmp_m_T_i32: return cmp_OrdCtxFn_defaultAsc$i32;
    case cmp_m_T_i64: return cmp_OrdCtxFn_defaultAsc$i64;
    case cmp_m_T_f32: return cmp_OrdCtxFn_defaultAsc$f32;
    case cmp_m_T_f64: return cmp_OrdCtxFn_defaultAsc$f64;
    }
};

$static fn_((cmp_OrdCtxFn_defaultDesc$u8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$u8(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$u16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$u16(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$u32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$u32(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$u64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$u64(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$i8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$i8(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$i16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$i16(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$i32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$i32(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$i64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$i64(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$f32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$f32(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$f64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$f64(lhs, rhs, ctx)); };
fn_((cmp_OrdCtxFn_defaultDesc(cmp_m_T type))(cmp_OrdCtxFn)) {
    switch (type) {
    case cmp_m_T_u8: return cmp_OrdCtxFn_defaultDesc$u8;
    case cmp_m_T_u16: return cmp_OrdCtxFn_defaultDesc$u16;
    case cmp_m_T_u32: return cmp_OrdCtxFn_defaultDesc$u32;
    case cmp_m_T_u64: return cmp_OrdCtxFn_defaultDesc$u64;
    case cmp_m_T_i8: return cmp_OrdCtxFn_defaultDesc$i8;
    case cmp_m_T_i16: return cmp_OrdCtxFn_defaultDesc$i16;
    case cmp_m_T_i32: return cmp_OrdCtxFn_defaultDesc$i32;
    case cmp_m_T_i64: return cmp_OrdCtxFn_defaultDesc$i64;
    case cmp_m_T_f32: return cmp_OrdCtxFn_defaultDesc$f32;
    case cmp_m_T_f64: return cmp_OrdCtxFn_defaultDesc$f64;
    }
};

fn_((cmp_ordCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(cmp_Ord)) {
    claim_assert_nonnull(lhs.inner), claim_assert_nonnull(rhs.inner), claim_assert_nonnull(ctx.inner);
    return ordFn(u_load(lhs), u_load(rhs), u_load(ctx));
};
fn_((cmp_ordCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(cmp_Ord)) {
    claim_assert_nonnull(lhs.raw), claim_assert_nonnull(rhs.raw), claim_assert_nonnull(ctx.inner);
    return cmp_ordCtx(u_deref(lhs), u_deref(rhs), ctx, ordFn);
};
fn_((cmp_ordCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(cmp_Ord)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs), claim_assert_nonnull(ctx.inner);
    for_(($us(lhs), $us(rhs))(l, r)) {
        if (cmp_ordCtxP(l, r, ctx, ordFn)) continue;
        return false;
    } $end(for);
    return true;
};
fn_((cmp_eqCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isEq(cmp_ordCtx(lhs, rhs, ctx, ordFn)); };
fn_((cmp_eqCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isEq(cmp_ordCtxP(lhs, rhs, ctx, ordFn)); };
fn_((cmp_eqCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isEq(cmp_ordCtxS(lhs, rhs, ctx, ordFn)); };
fn_((cmp_neCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isNe(cmp_ordCtx(lhs, rhs, ctx, ordFn)); };
fn_((cmp_neCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isNe(cmp_ordCtxP(lhs, rhs, ctx, ordFn)); };
fn_((cmp_neCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isNe(cmp_ordCtxS(lhs, rhs, ctx, ordFn)); };
fn_((cmp_ltCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isLt(cmp_ordCtx(lhs, rhs, ctx, ordFn)); };
fn_((cmp_ltCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isLt(cmp_ordCtxP(lhs, rhs, ctx, ordFn)); };
fn_((cmp_ltCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isLt(cmp_ordCtxS(lhs, rhs, ctx, ordFn)); };
fn_((cmp_gtCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isGt(cmp_ordCtx(lhs, rhs, ctx, ordFn)); };
fn_((cmp_gtCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isGt(cmp_ordCtxP(lhs, rhs, ctx, ordFn)); };
fn_((cmp_gtCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isGt(cmp_ordCtxS(lhs, rhs, ctx, ordFn)); };
fn_((cmp_leCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isLe(cmp_ordCtx(lhs, rhs, ctx, ordFn)); };
fn_((cmp_leCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isLe(cmp_ordCtxP(lhs, rhs, ctx, ordFn)); };
fn_((cmp_leCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isLe(cmp_ordCtxS(lhs, rhs, ctx, ordFn)); };
fn_((cmp_geCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isGe(cmp_ordCtx(lhs, rhs, ctx, ordFn)); };
fn_((cmp_geCtxP(u_P_const$raw lhs, u_P_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isGe(cmp_ordCtxP(lhs, rhs, ctx, ordFn)); };
fn_((cmp_geCtxS(u_S_const$raw lhs, u_S_const$raw rhs, u_V$raw ctx, cmp_OrdCtxFn ordFn))(bool)) { return cmp_Ord_isGe(cmp_ordCtxS(lhs, rhs, ctx, ordFn)); };

fn_((cmp_ordNoCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) {
    claim_assert_nonnull(lhs.inner), claim_assert_nonnull(rhs.inner), claim_assert_nonnull(ctx.inner);
    let no_ctx = u_castV$((cmp_OrdNoCtxFnAsCtx)(ctx));
    return no_ctx.ordFn(lhs, rhs);
};
