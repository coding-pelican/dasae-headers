#include "dh-main.h"

T_alias$((m_T)(variant_((m_T $fits($packed))(
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
))));
T_use_prl$(m_T);
T_alias$((m_T$Tag)(FieldType$(m_T, tag)));

T_alias$((m_Int)(variant_((m_Int $fits($packed))(
    (m_Int_u8, u8),
    (m_Int_i8, i8),
    (m_Int_u16, u16),
    (m_Int_i16, i16),
    (m_Int_u32, u32),
    (m_Int_i32, i32),
    (m_Int_u64, u64),
    (m_Int_i64, i64),
    (m_Int_usize, usize),
    (m_Int_isize, isize),
))));
T_use_prl$(m_Int);
T_alias$((m_Int$Tag)(FieldType$(m_Int, tag)));
$attr($inline_always)
$static fn_((m_Int_limitMin(m_Int$Tag tag))(m_Int) $scope) {
    match_((reservedReturn().tag = tag, reservedReturn())) {
    patterns_((
        m_Int_u8, m_Int_u16, m_Int_u32, m_Int_u64, m_Int_usize,
        m_Int_i8, m_Int_i16, m_Int_i32, m_Int_i64, m_Int_isize
    )($ignore) return union_matched(int_limit_min$(MatchedType)))
    } $end(match);
} $unscoped(fn);
$attr($inline_always)
$static fn_((m_Int_limitMax(m_Int$Tag tag))(m_Int) $scope) {
    match_((reservedReturn().tag = tag, reservedReturn())) {
    patterns_((
        m_Int_u8, m_Int_u16, m_Int_u32, m_Int_u64, m_Int_usize,
        m_Int_i8, m_Int_i16, m_Int_i32, m_Int_i64, m_Int_isize
    )($ignore) return union_matched(int_limit_max$(MatchedType)))
    } $end(match);
} $unscoped(fn);
$attr($inline_always)
$static fn_((m_Int_isUnsigned(m_Int$Tag tag))(bool)) { return tag % 2 == 0; };
$attr($inline_always)
$static fn_((m_Int_isSigned(m_Int$Tag tag))(bool)) { return tag % 2 == 1; };
$attr($inline_always)
$static fn_((m_Int_as(m_Int$Tag dst_tag, m_Int src_val))(m_Int) $scope) {
    match_((src_val)) {
    patterns_((
        m_Int_u8, m_Int_u16, m_Int_u32, m_Int_u64, m_Int_usize,
        m_Int_i8, m_Int_i16, m_Int_i32, m_Int_i64, m_Int_isize
    )(val) match_((reservedReturn().tag = dst_tag, reservedReturn())) {
        patterns_((
            m_Int_u8, m_Int_u16, m_Int_u32, m_Int_u64, m_Int_usize,
            m_Int_i8, m_Int_i16, m_Int_i32, m_Int_i64, m_Int_isize
        )($ignore) return union_matched(as$(MatchedType)(val)));
    } $end(match));
    } $end(match);
} $unscoped(fn);

$attr($inline_always)
$static fn_((m_intCast(m_Int$Tag dst_tag, m_Int src_val))(O$m_Int) $scope) {
    match_(src_val) {
    patterns_((
        m_Int_u8, m_Int_u16, m_Int_u32, m_Int_u64, m_Int_usize,
        m_Int_i8, m_Int_i16, m_Int_i32, m_Int_i64, m_Int_isize
    )(val) {
        let dst_min = m_Int_limitMin(dst_tag);
        let dst_max = m_Int_limitMax(dst_tag);
        let dst_is_signed = m_Int_isSigned(dst_tag);
        let src_is_signed = m_Int_isSigned(src_val.tag);
        /* Lower bound check: ensure value >= target minimum */
        if (dst_is_signed) {
            if (src_is_signed && as$(i64)(val) < union_to((dst_min)(m_Int_i64))) return_none();
        } else {
            if (src_is_signed && val < 0) return_none();
        }
        /* Upper bound check: ensure value <= target maximum */
        if (!(src_is_signed && val < 0)) {
            if (as$(u64)(val) > union_to((dst_max)(m_Int_u64))) return_none();
        }
        return_some(m_Int_as(dst_tag, src_val));
    });
    } $end(match);
    return_none();
} $unscoped(fn);

T_alias$((m_UInt)(variant_((m_UInt $fits($packed))(
    (m_UInt_u8, u8),
    (m_UInt_u16, u16),
    (m_UInt_u32, u32),
    (m_UInt_u64, u64),
    (m_UInt_usize, usize),
))));
T_use_prl$(m_UInt);
T_alias$((m_UInt$Tag)(FieldType$(m_UInt, tag)));

T_alias$((m_IInt)(variant_((m_IInt $fits($packed))(
    (m_IInt_i8, i8),
    (m_IInt_i16, i16),
    (m_IInt_i32, i32),
    (m_IInt_i64, i64),
    (m_IInt_isize, isize),
))));
T_use_prl$(m_IInt);
T_alias$((m_IInt$Tag)(FieldType$(m_IInt, tag)));
