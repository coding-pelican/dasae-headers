#include "dh/cmp.h"

$static fn_((cmp_OrdFn_defaultAsc$u8(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((u8)(lhs)), u_castV$((u8)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$u16(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((u16)(lhs)), u_castV$((u16)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$u32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((u32)(lhs)), u_castV$((u32)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$u64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((u64)(lhs)), u_castV$((u64)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$usize(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((usize)(lhs)), u_castV$((usize)(rhs))); };

$static fn_((cmp_OrdFn_defaultAsc$i8(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((i8)(lhs)), u_castV$((i8)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$i16(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((i16)(lhs)), u_castV$((i16)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$i32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((i32)(lhs)), u_castV$((i32)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$i64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((i64)(lhs)), u_castV$((i64)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$isize(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((isize)(lhs)), u_castV$((isize)(rhs))); };

$static fn_((cmp_OrdFn_defaultAsc$f32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((f32)(lhs)), u_castV$((f32)(rhs))); };
$static fn_((cmp_OrdFn_defaultAsc$f64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return prim_ord(u_castV$((f64)(lhs)), u_castV$((f64)(rhs))); };

fn_((cmp_OrdFn_defaultAsc(cmp_MathType type))(cmp_OrdFn)) {
    switch (type) { /* clang-format off */
        case cmp_MathType_u8: return cmp_OrdFn_defaultAsc$u8;
        case cmp_MathType_u16: return cmp_OrdFn_defaultAsc$u16;
        case cmp_MathType_u32: return cmp_OrdFn_defaultAsc$u32;
        case cmp_MathType_u64: return cmp_OrdFn_defaultAsc$u64;
        case cmp_MathType_usize: return cmp_OrdFn_defaultAsc$usize;
        case cmp_MathType_i8: return cmp_OrdFn_defaultAsc$i8;
        case cmp_MathType_i16: return cmp_OrdFn_defaultAsc$i16;
        case cmp_MathType_i32: return cmp_OrdFn_defaultAsc$i32;
        case cmp_MathType_i64: return cmp_OrdFn_defaultAsc$i64;
        case cmp_MathType_isize: return cmp_OrdFn_defaultAsc$isize;
        case cmp_MathType_f32: return cmp_OrdFn_defaultAsc$f32;
        case cmp_MathType_f64: return cmp_OrdFn_defaultAsc$f64;
    } /* clang-format on */
};

$static fn_((cmp_OrdFn_defaultDesc$u8(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$u8(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$u16(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$u16(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$u32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$u32(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$u64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$u64(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$usize(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$usize(lhs, rhs)); };

$static fn_((cmp_OrdFn_defaultDesc$i8(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$i8(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$i16(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$i16(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$i32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$i32(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$i64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$i64(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$isize(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$isize(lhs, rhs)); };

$static fn_((cmp_OrdFn_defaultDesc$f32(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$f32(lhs, rhs)); };
$static fn_((cmp_OrdFn_defaultDesc$f64(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdFn_defaultAsc$f64(lhs, rhs)); };

fn_((cmp_OrdFn_defaultDesc(cmp_MathType type))(cmp_OrdFn)) {
    switch (type) { /* clang-format off */
        case cmp_MathType_u8: return cmp_OrdFn_defaultDesc$u8;
        case cmp_MathType_u16: return cmp_OrdFn_defaultDesc$u16;
        case cmp_MathType_u32: return cmp_OrdFn_defaultDesc$u32;
        case cmp_MathType_u64: return cmp_OrdFn_defaultDesc$u64;
        case cmp_MathType_usize: return cmp_OrdFn_defaultDesc$usize;
        case cmp_MathType_i8: return cmp_OrdFn_defaultDesc$i8;
        case cmp_MathType_i16: return cmp_OrdFn_defaultDesc$i16;
        case cmp_MathType_i32: return cmp_OrdFn_defaultDesc$i32;
        case cmp_MathType_i64: return cmp_OrdFn_defaultDesc$i64;
        case cmp_MathType_isize: return cmp_OrdFn_defaultDesc$isize;
        case cmp_MathType_f32: return cmp_OrdFn_defaultDesc$f32;
        case cmp_MathType_f64: return cmp_OrdFn_defaultDesc$f64;
    } /* clang-format on */
};

$static fn_((cmp_OrdCtxFn_defaultAsc$u8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((u8)(lhs)), u_castV$((u8)(rhs))); };
$static fn_((cmp_OrdCtxFn_defaultAsc$u16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((u16)(lhs)), u_castV$((u16)(rhs))); };
$static fn_((cmp_OrdCtxFn_defaultAsc$u32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((u32)(lhs)), u_castV$((u32)(rhs))); };
$static fn_((cmp_OrdCtxFn_defaultAsc$u64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((u64)(lhs)), u_castV$((u64)(rhs))); };
$static fn_((cmp_OrdCtxFn_defaultAsc$usize(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((usize)(lhs)), u_castV$((usize)(rhs))); };

$static fn_((cmp_OrdCtxFn_defaultAsc$i8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((i8)(lhs)), u_castV$((i8)(rhs))); };
$static fn_((cmp_OrdCtxFn_defaultAsc$i16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((i16)(lhs)), u_castV$((i16)(rhs))); };
$static fn_((cmp_OrdCtxFn_defaultAsc$i32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((i32)(lhs)), u_castV$((i32)(rhs))); };
$static fn_((cmp_OrdCtxFn_defaultAsc$i64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((i64)(lhs)), u_castV$((i64)(rhs))); };
$static fn_((cmp_OrdCtxFn_defaultAsc$isize(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((isize)(lhs)), u_castV$((isize)(rhs))); };

$static fn_((cmp_OrdCtxFn_defaultAsc$f32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((f32)(lhs)), u_castV$((f32)(rhs))); };
$static fn_((cmp_OrdCtxFn_defaultAsc$f64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return $ignore_void ctx, prim_ord(u_castV$((f64)(lhs)), u_castV$((f64)(rhs))); };

fn_((cmp_OrdCtxFn_default(cmp_MathType type))(cmp_OrdCtxFn)) {
    switch (type) { /* clang-format off */
        case cmp_MathType_u8: return cmp_OrdCtxFn_defaultAsc$u8;
        case cmp_MathType_u16: return cmp_OrdCtxFn_defaultAsc$u16;
        case cmp_MathType_u32: return cmp_OrdCtxFn_defaultAsc$u32;
        case cmp_MathType_u64: return cmp_OrdCtxFn_defaultAsc$u64;
        case cmp_MathType_usize: return cmp_OrdCtxFn_defaultAsc$usize;
        case cmp_MathType_i8: return cmp_OrdCtxFn_defaultAsc$i8;
        case cmp_MathType_i16: return cmp_OrdCtxFn_defaultAsc$i16;
        case cmp_MathType_i32: return cmp_OrdCtxFn_defaultAsc$i32;
        case cmp_MathType_i64: return cmp_OrdCtxFn_defaultAsc$i64;
        case cmp_MathType_isize: return cmp_OrdCtxFn_defaultAsc$isize;
        case cmp_MathType_f32: return cmp_OrdCtxFn_defaultAsc$f32;
        case cmp_MathType_f64: return cmp_OrdCtxFn_defaultAsc$f64;
    } /* clang-format on */
};

$static fn_((cmp_OrdCtxFn_defaultDesc$u8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$u8(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$u16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$u16(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$u32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$u32(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$u64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$u64(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$usize(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$usize(lhs, rhs, ctx)); };

$static fn_((cmp_OrdCtxFn_defaultDesc$i8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$i8(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$i16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$i16(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$i32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$i32(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$i64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$i64(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$isize(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$isize(lhs, rhs, ctx)); };

$static fn_((cmp_OrdCtxFn_defaultDesc$f32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$f32(lhs, rhs, ctx)); };
$static fn_((cmp_OrdCtxFn_defaultDesc$f64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) { return cmp_Ord_inv(cmp_OrdCtxFn_defaultAsc$f64(lhs, rhs, ctx)); };

fn_((cmp_OrdCtxFn_defaultDesc(cmp_MathType type))(cmp_OrdCtxFn)) {
    switch (type) { /* clang-format off */
        case cmp_MathType_u8: return cmp_OrdCtxFn_defaultDesc$u8;
        case cmp_MathType_u16: return cmp_OrdCtxFn_defaultDesc$u16;
        case cmp_MathType_u32: return cmp_OrdCtxFn_defaultDesc$u32;
        case cmp_MathType_u64: return cmp_OrdCtxFn_defaultDesc$u64;
        case cmp_MathType_usize: return cmp_OrdCtxFn_defaultDesc$usize;
        case cmp_MathType_i8: return cmp_OrdCtxFn_defaultDesc$i8;
        case cmp_MathType_i16: return cmp_OrdCtxFn_defaultDesc$i16;
        case cmp_MathType_i32: return cmp_OrdCtxFn_defaultDesc$i32;
        case cmp_MathType_i64: return cmp_OrdCtxFn_defaultDesc$i64;
        case cmp_MathType_isize: return cmp_OrdCtxFn_defaultDesc$isize;
        case cmp_MathType_f32: return cmp_OrdCtxFn_defaultDesc$f32;
        case cmp_MathType_f64: return cmp_OrdCtxFn_defaultDesc$f64;
    } /* clang-format on */
};

$static fn_((cmp_EqlFn_default$u8(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((u8)(lhs)), u_castV$((u8)(rhs))); };
$static fn_((cmp_EqlFn_default$u16(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((u16)(lhs)), u_castV$((u16)(rhs))); };
$static fn_((cmp_EqlFn_default$u32(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((u32)(lhs)), u_castV$((u32)(rhs))); };
$static fn_((cmp_EqlFn_default$u64(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((u64)(lhs)), u_castV$((u64)(rhs))); };
$static fn_((cmp_EqlFn_default$usize(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((usize)(lhs)), u_castV$((usize)(rhs))); };

$static fn_((cmp_EqlFn_default$i8(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((i8)(lhs)), u_castV$((i8)(rhs))); };
$static fn_((cmp_EqlFn_default$i16(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((i16)(lhs)), u_castV$((i16)(rhs))); };
$static fn_((cmp_EqlFn_default$i32(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((i32)(lhs)), u_castV$((i32)(rhs))); };
$static fn_((cmp_EqlFn_default$i64(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((i64)(lhs)), u_castV$((i64)(rhs))); };
$static fn_((cmp_EqlFn_default$isize(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((isize)(lhs)), u_castV$((isize)(rhs))); };

$static fn_((cmp_EqlFn_default$f32(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((f32)(lhs)), u_castV$((f32)(rhs))); };
$static fn_((cmp_EqlFn_default$f64(u_V$raw lhs, u_V$raw rhs))(bool)) { return prim_eql(u_castV$((f64)(lhs)), u_castV$((f64)(rhs))); };

fn_((cmp_EqlFn_default(cmp_MathType type))(cmp_EqlFn)) {
    switch (type) { /* clang-format off */
        case cmp_MathType_u8: return cmp_EqlFn_default$u8;
        case cmp_MathType_u16: return cmp_EqlFn_default$u16;
        case cmp_MathType_u32: return cmp_EqlFn_default$u32;
        case cmp_MathType_u64: return cmp_EqlFn_default$u64;
        case cmp_MathType_usize: return cmp_EqlFn_default$usize;
        case cmp_MathType_i8: return cmp_EqlFn_default$i8;
        case cmp_MathType_i16: return cmp_EqlFn_default$i16;
        case cmp_MathType_i32: return cmp_EqlFn_default$i32;
        case cmp_MathType_i64: return cmp_EqlFn_default$i64;
        case cmp_MathType_isize: return cmp_EqlFn_default$isize;
        case cmp_MathType_f32: return cmp_EqlFn_default$f32;
        case cmp_MathType_f64: return cmp_EqlFn_default$f64;
    } /* clang-format on */
};

$static fn_((cmp_EqlCtxFn_default$u8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((u8)(lhs)), u_castV$((u8)(rhs))); };
$static fn_((cmp_EqlCtxFn_default$u16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((u16)(lhs)), u_castV$((u16)(rhs))); };
$static fn_((cmp_EqlCtxFn_default$u32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((u32)(lhs)), u_castV$((u32)(rhs))); };
$static fn_((cmp_EqlCtxFn_default$u64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((u64)(lhs)), u_castV$((u64)(rhs))); };
$static fn_((cmp_EqlCtxFn_default$usize(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((usize)(lhs)), u_castV$((usize)(rhs))); };

$static fn_((cmp_EqlCtxFn_default$i8(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((i8)(lhs)), u_castV$((i8)(rhs))); };
$static fn_((cmp_EqlCtxFn_default$i16(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((i16)(lhs)), u_castV$((i16)(rhs))); };
$static fn_((cmp_EqlCtxFn_default$i32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((i32)(lhs)), u_castV$((i32)(rhs))); };
$static fn_((cmp_EqlCtxFn_default$i64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((i64)(lhs)), u_castV$((i64)(rhs))); };
$static fn_((cmp_EqlCtxFn_default$isize(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((isize)(lhs)), u_castV$((isize)(rhs))); };

$static fn_((cmp_EqlCtxFn_default$f32(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((f32)(lhs)), u_castV$((f32)(rhs))); };
$static fn_((cmp_EqlCtxFn_default$f64(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) { return $ignore_void ctx, prim_eql(u_castV$((f64)(lhs)), u_castV$((f64)(rhs))); };

fn_((cmp_EqlCtxFn_default(cmp_MathType type))(cmp_EqlCtxFn)) {
    switch (type) { /* clang-format off */
        case cmp_MathType_u8: return cmp_EqlCtxFn_default$u8;
        case cmp_MathType_u16: return cmp_EqlCtxFn_default$u16;
        case cmp_MathType_u32: return cmp_EqlCtxFn_default$u32;
        case cmp_MathType_u64: return cmp_EqlCtxFn_default$u64;
        case cmp_MathType_usize: return cmp_EqlCtxFn_default$usize;
        case cmp_MathType_i8: return cmp_EqlCtxFn_default$i8;
        case cmp_MathType_i16: return cmp_EqlCtxFn_default$i16;
        case cmp_MathType_i32: return cmp_EqlCtxFn_default$i32;
        case cmp_MathType_i64: return cmp_EqlCtxFn_default$i64;
        case cmp_MathType_isize: return cmp_EqlCtxFn_default$isize;
        case cmp_MathType_f32: return cmp_EqlCtxFn_default$f32;
        case cmp_MathType_f64: return cmp_EqlCtxFn_default$f64;
    } /* clang-format on */
};
