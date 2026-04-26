#include "dh-main.h"

/// math Scalar
typedef enum_((m_T$Tag $fits($packed))(
    m_T_u8 = 0,
    m_T_i8 = 1,
    m_T_u16 = 2,
    m_T_i16 = 3,
    m_T_u32 = 4,
    m_T_i32 = 5,
    m_T_u64 = 6,
    m_T_i64 = 7,
    m_T_usize = pp_if_(arch_bits_is_64bit)(
        pp_then_(m_T_u64),
        pp_else_(m_T_u32)
    ),
    m_T_isize = pp_if_(arch_bits_is_64bit)(
        pp_then_(m_T_i64),
        pp_else_(m_T_i32)
    ),
    m_T_f32 = 8,
    m_T_f64 = 9
)) m_T$Tag;
typedef variant_((m_T $maps(m_T$Tag))(
    (m_T_u8, u8),
    (m_T_i8, i8),
    (m_T_u16, u16),
    (m_T_i16, i16),
    (m_T_u32, u32),
    (m_T_i32, i32),
    (m_T_u64, u64),
    (m_T_i64, i64),
    (m_T_usize, usize),
    (m_T_isize, isize),
    (m_T_f32, f32),
    (m_T_f64, f64)
)) m_T;
/// math Unsigned Scalar
typedef enum_((m_U$Tag $fits($packed))(
    m_U_u8 = m_T_u8,
    m_U_u16 = m_T_u16,
    m_U_u32 = m_T_u32,
    m_U_u64 = m_T_u64,
    m_U_usize = m_T_usize
)) m_U$Tag;
typedef variant_((m_U $maps(m_U$Tag))(
    (m_U_u8, u8),
    (m_U_u16, u16),
    (m_U_u32, u32),
    (m_U_u64, u64),
    (m_U_usize, usize)
)) m_U;
/// math Signed Scalar
typedef enum_((m_S$Tag $fits($packed))(
    m_S_i8 = m_T_i8,
    m_S_i16 = m_T_i16,
    m_S_i32 = m_T_i32,
    m_S_i64 = m_T_i64,
    m_S_isize = m_T_isize,
    m_S_f32 = m_T_f32,
    m_S_f64 = m_T_f64
)) m_S$Tag;
typedef variant_((m_S $maps(m_S$Tag))(
    (m_S_i8, i8),
    (m_S_i16, i16),
    (m_S_i32, i32),
    (m_S_i64, i64),
    (m_S_isize, isize),
    (m_S_f32, f32),
    (m_S_f64, f64)
)) m_S;
/// math Unsigned-Integer
typedef enum_((m_UInt$Tag $fits($packed))(
    m_UInt_u8 = m_U_u8,
    m_UInt_u16 = m_U_u16,
    m_UInt_u32 = m_U_u32,
    m_UInt_u64 = m_U_u64,
    m_UInt_usize = m_U_usize
)) m_UInt$Tag;
typedef variant_((m_UInt $maps(m_UInt$Tag))(
    (m_UInt_u8, u8),
    (m_UInt_u16, u16),
    (m_UInt_u32, u32),
    (m_UInt_u64, u64),
    (m_UInt_usize, usize)
)) m_UInt;
/// math Signed-Integer
typedef enum_((m_IInt$Tag $fits($packed))(
    m_IInt_i8 = m_S_i8,
    m_IInt_i16 = m_S_i16,
    m_IInt_i32 = m_S_i32,
    m_IInt_i64 = m_S_i64,
    m_IInt_isize = m_S_isize
)) m_IInt$Tag;
typedef variant_((m_IInt $maps(m_IInt$Tag))(
    (m_IInt_i8, i8),
    (m_IInt_i16, i16),
    (m_IInt_i32, i32),
    (m_IInt_i64, i64),
    (m_IInt_isize, isize)
)) m_IInt;
/// math Integer
typedef enum_((m_Int$Tag $fits($packed))(
    m_Int_u8 = m_UInt_u8,
    m_Int_i8 = m_IInt_i8,
    m_Int_u16 = m_UInt_u16,
    m_Int_i16 = m_IInt_i16,
    m_Int_u32 = m_UInt_u32,
    m_Int_i32 = m_IInt_i32,
    m_Int_u64 = m_UInt_u64,
    m_Int_i64 = m_IInt_i64,
    m_Int_usize = m_UInt_usize,
    m_Int_isize = m_IInt_isize
)) m_Int$Tag;
typedef variant_((m_Int $maps(m_Int$Tag))(
    (m_Int_u8, u8),
    (m_Int_i8, i8),
    (m_Int_u16, u16),
    (m_Int_i16, i16),
    (m_Int_u32, u32),
    (m_Int_i32, i32),
    (m_Int_u64, u64),
    (m_Int_i64, i64),
    (m_Int_usize, usize)
)) m_Int;
/// math Floating-point
typedef enum_((m_Flt$Tag $fits($packed))(
    m_Flt_f32 = m_S_f32,
    m_Flt_f64 = m_S_f64
)) m_Flt$Tag;
typedef variant_((m_Flt $maps(m_Flt$Tag))(
    (m_Flt_f32, f32),
    (m_Flt_f64, f64)
)) m_Flt;


$attr($inline_always)
$static fn_((m_eql(m_T lhs, m_T rhs))(bool) $scope) {
    claim_assert(lhs.tag == rhs.tag);
    match_(lhs) {
    patterns_((
        m_T_u8, m_T_u16, m_T_u32, m_T_u64,
        m_T_i8, m_T_i16, m_T_i32, m_T_i64,
        m_T_f32, m_T_f64
    )(lhs) return pri_eql(lhs, union_cast$((MatchedType)(matchedEnum())(rhs))));
    } $end(match);
} $unscoped(fn);
$attr($inline_always)
$static fn_((m_neq(m_T lhs, m_T rhs))(bool)) { return !m_eql(lhs, rhs); };

$attr($inline_always)
$static fn_((m_ord(m_T lhs, m_T rhs))(cmp_Ord)) {
    claim_assert(lhs.tag == rhs.tag);
    match_(lhs) {
    patterns_((
        m_T_u8, m_T_u16, m_T_u32, m_T_u64,
        m_T_i8, m_T_i16, m_T_i32, m_T_i64,
        m_T_f32, m_T_f64
    )(lhs) return pri_ord(lhs, union_cast$((MatchedType)(matchedEnum())(rhs))));
    } $end(match);
};
$attr($inline_always)
$static fn_((m_eq(m_T lhs, m_T rhs))(bool)) { return cmp_Ord_isEq(m_ord(lhs, rhs)); };
$attr($inline_always)
$static fn_((m_ne(m_T lhs, m_T rhs))(bool)) { return cmp_Ord_isNe(m_ord(lhs, rhs)); };
$attr($inline_always)
$static fn_((m_lt(m_T lhs, m_T rhs))(bool)) { return cmp_Ord_isLt(m_ord(lhs, rhs)); };
$attr($inline_always)
$static fn_((m_gt(m_T lhs, m_T rhs))(bool)) { return cmp_Ord_isGt(m_ord(lhs, rhs)); };
$attr($inline_always)
$static fn_((m_le(m_T lhs, m_T rhs))(bool)) { return cmp_Ord_isLe(m_ord(lhs, rhs)); };
$attr($inline_always)
$static fn_((m_ge(m_T lhs, m_T rhs))(bool)) { return cmp_Ord_isGe(m_ord(lhs, rhs)); };

$attr($inline_always)
$static fn_((m_neg(m_S val))(m_S)) {
    match_((val)) {
    patterns_((
        m_S_i8, m_S_i16, m_S_i32, m_S_i64,
        m_S_f32, m_S_f64
    )(val) return union_matched(pri_neg(val)));
    } $end(match);
};
$attr($inline_always)
$static fn_((m_abs(m_S val))(m_S)) {
    match_((val)) {
    patterns_((
        m_S_i8, m_S_i16, m_S_i32, m_S_i64,
        m_S_f32, m_S_f64
    )(val) return union_matched(pri_abs(val)));
    } $end(match);
};
$attr($inline_always)
$static fn_((m_sgn(m_T val))(cmp_Sgn)) {
    match_((val)) {
    patterns_((
        m_T_u8, m_T_u16, m_T_u32, m_T_u64,
        m_T_i8, m_T_i16, m_T_i32, m_T_i64,
        m_T_f32, m_T_f64
    )(val) return pri_sgn(val));
    } $end(match);
};

/* clang-format off */
$attr($inline_always)
$static fn_((m_sin(m_Flt rad))(m_Flt)) { match_((rad)) { patterns_((m_Flt_f32, m_Flt_f64)(r) return union_matched(flt_sin(r))); } $end(match); };
$attr($inline_always)
$static fn_((m_cos(m_Flt rad))(m_Flt)) { match_((rad)) { patterns_((m_Flt_f32, m_Flt_f64)(r) return union_matched(flt_cos(r))); } $end(match); };
$attr($inline_always)
$static fn_((m_tan(m_Flt rad))(m_Flt)) { match_((rad)) { patterns_((m_Flt_f32, m_Flt_f64)(r) return union_matched(flt_tan(r))); } $end(match); };
$attr($inline_always)
$static fn_((m_asin(m_Flt rad))(m_Flt)) { match_((rad)) { patterns_((m_Flt_f32, m_Flt_f64)(r) return union_matched(flt_asin(r))); } $end(match); };
$attr($inline_always)
$static fn_((m_acos(m_Flt rad))(m_Flt)) { match_((rad)) { patterns_((m_Flt_f32, m_Flt_f64)(r) return union_matched(flt_acos(r))); } $end(match); };
$attr($inline_always)
$static fn_((m_atan(m_Flt rad))(m_Flt)) { match_((rad)) { patterns_((m_Flt_f32, m_Flt_f64)(r) return union_matched(flt_atan(r))); } $end(match); };
/* clang-format on */
$attr($inline_always)
$static fn_((m_atan2(m_Flt y, m_Flt x))(m_Flt)) {
    match_((claim_assert(y.tag == x.tag), y)) {
    patterns_((
        m_Flt_f32, m_Flt_f64
    )(y) return union_matched(flt_atan2(y, union_cast$((MatchedType)(matchedEnum())(x)))));
    } $end(match);
};



#define m_V2_A$(_T...) __alias__m_V2_A$(_T)
#define T_use_m_V2_A$(_T...) __stmt__T_use_m_V2_A$(_T)
#define m_V3_A$(_T...) __alias__m_V3_A$(_T)
#define T_use_m_V3_A$(_T...) __stmt__T_use_m_V3_A$(_T)
#define m_V4_A$(_T...) __alias__m_V4_A$(_T)
#define T_use_m_V4_A$(_T...) __stmt__T_use_m_V4_A$(_T)

#define m_V2$(_T...) __alias__m_V2$(_T)
#define T_decl_m_V2$(_T...) __stmt__T_decl_m_V2$(_T)
#define T_impl_m_V2$(_T...) __stmt__T_impl_m_V2$(_T)
#define T_use_m_V2$(_T...) __stmt__T_use_m_V2$(_T)
#define m_V3$(_T...) __alias__m_V3$(_T)
#define T_decl_m_V3$(_T...) __stmt__T_decl_m_V3$(_T)
#define T_impl_m_V3$(_T...) __stmt__T_impl_m_V3$(_T)
#define T_use_m_V3$(_T...) __stmt__T_use_m_V3$(_T)
#define m_V4$(_T...) __alias__m_V4$(_T)
#define T_decl_m_V4$(_T...) __stmt__T_decl_m_V4$(_T)
#define T_impl_m_V4$(_T...) __stmt__T_impl_m_V4$(_T)
#define T_use_m_V4$(_T...) __stmt__T_use_m_V4$(_T)

#define __alias__m_V2_A$(_T...) tpl$(m_V2_A, _T)
#define __stmt__T_use_m_V2_A$(_T...) \
    T_alias$((m_V2_A$(_T))(A$$(2, _T)))
#define __alias__m_V3_A$(_T...) tpl$(m_V3_A, _T)
#define __stmt__T_use_m_V3_A$(_T...) \
    T_alias$((m_V3_A$(_T))(A$$(3, _T)))
#define __alias__m_V4_A$(_T...) tpl$(m_V4_A, _T)
#define __stmt__T_use_m_V4_A$(_T...) \
    T_alias$((m_V4_A$(_T))(A$$(4, _T)))

#define __alias__m_V2$(_T...) tpl$(m_V2, _T)
#define __stmt__T_decl_m_V2$(_T...) \
    T_alias$((m_V2$(_T))(union m_V2$(_T)))
#define __stmt__T_impl_m_V2$(_T...) \
    union m_V2$(_T) { \
        var_(s, m_V2_A$(_T)); \
        T_embed$(struct { \
            var_(x, _T); \
            var_(y, _T); \
        }); \
    }
#define __stmt__T_use_m_V2$(_T...) \
    T_decl_m_V2$(_T); \
    T_impl_m_V2$(_T)
#define __alias__m_V3$(_T...) tpl$(m_V3, _T)
#define __stmt__T_decl_m_V3$(_T...) \
    T_alias$((m_V3$(_T))(union m_V3$(_T)))
#define __stmt__T_impl_m_V3$(_T...) \
    union m_V3$(_T) { \
        var_(s, m_V3_A$(_T)); \
        T_embed$(struct { \
            var_(x, _T); \
            var_(y, _T); \
            var_(z, _T); \
        }); \
    }
#define __stmt__T_use_m_V3$(_T...) \
    T_decl_m_V3$(_T); \
    T_impl_m_V3$(_T)
#define __alias__m_V4$(_T...) tpl$(m_V4, _T)
#define __stmt__T_decl_m_V4$(_T...) \
    T_alias$((m_V4$(_T))(union m_V4$(_T)))
#define __stmt__T_impl_m_V4$(_T...) \
    union m_V4$(_T) { \
        var_(s, m_V4_A$(_T)); \
        T_embed$(struct { \
            var_(x, _T); \
            var_(y, _T); \
            var_(z, _T); \
            var_(w, _T); \
        }); \
    }
#define __stmt__T_use_m_V4$(_T...) \
    T_decl_m_V4$(_T); \
    T_impl_m_V4$(_T)

T_use$((u8)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));
T_use$((u16)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));
T_use$((u32)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));
T_use$((u64)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));
T_use$((usize)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));
T_use$((i8)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));
T_use$((i16)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));
T_use$((i32)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));
T_use$((i64)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));
T_use$((isize)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));
T_use$((f32)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));
T_use$((f64)(m_V2_A, m_V2, m_V3_A, m_V3, m_V4_A, m_V4));

/// math Vector2 Unsigned-Integer
typedef enum_((m_V2UInt$Tag $fits($packed))(
    m_V2UInt_u8 = m_UInt_u8,
    m_V2UInt_u16 = m_UInt_u16,
    m_V2UInt_u32 = m_UInt_u32,
    m_V2UInt_u64 = m_UInt_u64,
    m_V2UInt_usize = m_UInt_usize
)) m_V2UInt$Tag;
typedef variant_((m_V2UInt $maps(m_V2UInt$Tag))(
    (m_V2UInt_u8, m_V2$u8),
    (m_V2UInt_u16, m_V2$u16),
    (m_V2UInt_u32, m_V2$u32),
    (m_V2UInt_u64, m_V2$u64),
    (m_V2UInt_usize, m_V2$usize)
)) m_V2UInt;
/// math Vector2 Signed-Integer
typedef enum_((m_V2IInt$Tag $fits($packed))(
    m_V2IInt_i8 = m_IInt_i8,
    m_V2IInt_i16 = m_IInt_i16,
    m_V2IInt_i32 = m_IInt_i32,
    m_V2IInt_i64 = m_IInt_i64,
    m_V2IInt_isize = m_IInt_isize
)) m_V2IInt$Tag;
typedef variant_((m_V2IInt $maps(m_V2IInt$Tag))(
    (m_V2IInt_i8, m_V2$i8),
    (m_V2IInt_i16, m_V2$i16),
    (m_V2IInt_i32, m_V2$i32),
    (m_V2IInt_i64, m_V2$i64),
    (m_V2IInt_isize, m_V2$isize)
)) m_V2IInt;
/// math Vector2 Integer
typedef enum_((m_V2Int$Tag $fits($packed))(
    m_V2Int_uint = m_Int_uint,
    m_V2Int_iint = m_Int_iint
)) m_V2Int$Tag;
typedef variant_((m_V2Int $maps(m_V2Int$Tag))(
    (m_V2Int_uint, m_V2UInt),
    (m_V2Int_iint, m_V2IInt)
)) m_V2Int;
/// math Vector2 Floating-point
typedef enum_((m_V2Flt$Tag $fits($packed))(
    m_V2Flt_f32 = m_Flt_f32,
    m_V2Flt_f64 = m_Flt_f64
)) m_V2Flt$Tag;
typedef variant_((m_V2Flt $maps(m_V2Flt$Tag))(
    (m_V2Flt_f32, m_V2$f32),
    (m_V2Flt_f64, m_V2$f64)
)) m_V2Flt;
/// math Vector2 Unsigned
typedef enum_((m_V2U$Tag $fits($packed))(
    m_V2U_uint = m_U_uint
)) m_V2U$Tag;
typedef variant_((m_V2U $maps(m_V2U$Tag))(
    (m_V2U_uint, m_V2UInt)
)) m_V2U;
/// math Vector2 Signed
typedef enum_((m_V2S$Tag $fits($packed))(
    m_V2S_iint = m_S_iint,
    m_V2S_flt = m_S_flt
)) m_V2S$Tag;
typedef variant_((m_V2S $maps(m_V2S$Tag))(
    (m_V2S_iint, m_V2IInt),
    (m_V2S_flt, m_V2Flt)
)) m_V2S;
/// math Vector2
typedef enum_((m_V2T$Tag $fits($packed))(
    m_V2T_int = m_T_int,
    m_V2T_flt = m_T_flt
)) m_V2T$Tag;
typedef variant_((m_V2T $maps(m_V2T$Tag))(
    (m_V2T_int, m_V2Int),
    (m_V2T_flt, m_V2Flt)
)) m_V2T;

/// math Vector3 Unsigned-Integer
typedef enum_((m_V3UInt$Tag $fits($packed))(
    m_V3UInt_u8 = m_V2UInt_u8,
    m_V3UInt_u16 = m_V2UInt_u16,
    m_V3UInt_u32 = m_V2UInt_u32,
    m_V3UInt_u64 = m_V2UInt_u64,
    m_V3UInt_usize = m_V2UInt_usize
)) m_V3UInt$Tag;
typedef variant_((m_V3UInt $maps(m_V3UInt$Tag))(
    (m_V3UInt_u8, m_V3$u8),
    (m_V3UInt_u16, m_V3$u16),
    (m_V3UInt_u32, m_V3$u32),
    (m_V3UInt_u64, m_V3$u64),
    (m_V3UInt_usize, m_V3$usize)
)) m_V3UInt;
/// math Vector3 Signed-Integer
typedef enum_((m_V3IInt$Tag $fits($packed))(
    m_V3IInt_i8 = m_V2IInt_i8,
    m_V3IInt_i16 = m_V2IInt_i16,
    m_V3IInt_i32 = m_V2IInt_i32,
    m_V3IInt_i64 = m_V2IInt_i64,
    m_V3IInt_isize = m_V2IInt_isize
)) m_V3IInt$Tag;
typedef variant_((m_V3IInt $maps(m_V3IInt$Tag))(
    (m_V3IInt_i8, m_V3$i8),
    (m_V3IInt_i16, m_V3$i16),
    (m_V3IInt_i32, m_V3$i32),
    (m_V3IInt_i64, m_V3$i64),
    (m_V3IInt_isize, m_V3$isize)
)) m_V3IInt;
/// math Vector3 Integer
typedef enum_((m_V3Int$Tag $fits($packed))(
    m_V3Int_uint = m_V2Int_uint,
    m_V3Int_iint = m_V2Int_iint
)) m_V3Int$Tag;
typedef variant_((m_V3Int $maps(m_V3Int$Tag))(
    (m_V3Int_uint, m_V3UInt),
    (m_V3Int_iint, m_V3IInt)
)) m_V3Int;
/// math Vector3 Floating-point
typedef enum_((m_V3Flt$Tag $fits($packed))(
    m_V3Flt_f32 = m_V2Flt_f32,
    m_V3Flt_f64 = m_V2Flt_f64
)) m_V3Flt$Tag;
typedef variant_((m_V3Flt $maps(m_V3Flt$Tag))(
    (m_V3Flt_f32, m_V3$f32),
    (m_V3Flt_f64, m_V3$f64)
)) m_V3Flt;
/// math Vector3 Unsigned
typedef enum_((m_V3U$Tag $fits($packed))(
    m_V3U_uint = m_V2U_uint
)) m_V3U$Tag;
typedef variant_((m_V3U $maps(m_V3U$Tag))(
    (m_V3U_uint, m_V3UInt)
)) m_V3U;
/// math Vector3 Signed
typedef enum_((m_V3S$Tag $fits($packed))(
    m_V3S_iint = m_V2S_iint,
    m_V3S_flt = m_V2S_flt
)) m_V3S$Tag;
typedef variant_((m_V3S $maps(m_V3S$Tag))(
    (m_V3S_iint, m_V3IInt),
    (m_V3S_flt, m_V3Flt)
)) m_V3S;
/// math Vector3
typedef enum_((m_V3T$Tag $fits($packed))(
    m_V3T_int = m_V2T_int,
    m_V3T_flt = m_V2T_flt
)) m_V3T$Tag;
typedef variant_((m_V3T $maps(m_V3T$Tag))(
    (m_V3T_int, m_V3Int),
    (m_V3T_flt, m_V3Flt)
)) m_V3T;

/// math Vector4 Unsigned-Integer
typedef enum_((m_V4UInt$Tag $fits($packed))(
    m_V4UInt_u8 = m_V3UInt_u8,
    m_V4UInt_u16 = m_V3UInt_u16,
    m_V4UInt_u32 = m_V3UInt_u32,
    m_V4UInt_u64 = m_V3UInt_u64,
    m_V4UInt_usize = m_V3UInt_usize
)) m_V4UInt$Tag;
typedef variant_((m_V4UInt $maps(m_V4UInt$Tag))(
    (m_V4UInt_u8, m_V4$u8),
    (m_V4UInt_u16, m_V4$u16),
    (m_V4UInt_u32, m_V4$u32),
    (m_V4UInt_u64, m_V4$u64),
    (m_V4UInt_usize, m_V4$usize)
)) m_V4UInt;
/// math Vector4 Signed-Integer
typedef enum_((m_V4IInt$Tag $fits($packed))(
    m_V4IInt_i8 = m_V3IInt_i8,
    m_V4IInt_i16 = m_V3IInt_i16,
    m_V4IInt_i32 = m_V3IInt_i32,
    m_V4IInt_i64 = m_V3IInt_i64,
    m_V4IInt_isize = m_V3IInt_isize
)) m_V4IInt$Tag;
typedef variant_((m_V4IInt $maps(m_V4IInt$Tag))(
    (m_V4IInt_i8, m_V4$i8),
    (m_V4IInt_i16, m_V4$i16),
    (m_V4IInt_i32, m_V4$i32),
    (m_V4IInt_i64, m_V4$i64),
    (m_V4IInt_isize, m_V4$isize)
)) m_V4IInt;
/// math Vector4 Integer
typedef enum_((m_V4Int$Tag $fits($packed))(
    m_V4Int_uint = m_V3Int_uint,
    m_V4Int_iint = m_V3Int_iint
)) m_V4Int$Tag;
typedef variant_((m_V4Int $maps(m_V4Int$Tag))(
    (m_V4Int_uint, m_V4UInt),
    (m_V4Int_iint, m_V4IInt)
)) m_V4Int;
/// math Vector4 Floating-point
typedef enum_((m_V4Flt$Tag $fits($packed))(
    m_V4Flt_f32 = m_V3Flt_f32,
    m_V4Flt_f64 = m_V3Flt_f64
)) m_V4Flt$Tag;
typedef variant_((m_V4Flt $maps(m_V4Flt$Tag))(
    (m_V4Flt_f32, m_V4$f32),
    (m_V4Flt_f64, m_V4$f64)
)) m_V4Flt;
/// math Vector4 Unsigned
typedef enum_((m_V4U$Tag $fits($packed))(
    m_V4U_uint = m_V3U_uint
)) m_V4U$Tag;
typedef variant_((m_V4U $maps(m_V4U$Tag))(
    (m_V4U_uint, m_V4UInt)
)) m_V4U;
/// math Vector4 Signed
typedef enum_((m_V4S$Tag $fits($packed))(
    m_V4S_iint = m_V3S_iint,
    m_V4S_flt = m_V3S_flt
)) m_V4S$Tag;
typedef variant_((m_V4S $maps(m_V4S$Tag))(
    (m_V4S_iint, m_V4IInt),
    (m_V4S_flt, m_V4Flt)
)) m_V4S;
/// math Vector4
typedef enum_((m_V4T$Tag $fits($packed))(
    m_V4T_int = m_V3T_int,
    m_V4T_flt = m_V3T_flt
)) m_V4T$Tag;
typedef variant_((m_V4T $maps(m_V4T$Tag))(
    (m_V4T_int, m_V4Int),
    (m_V4T_flt, m_V4Flt)
)) m_V4T;
