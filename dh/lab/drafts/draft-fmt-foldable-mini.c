#define main_no_args pp_true
#include "dh-main.h"
#include "dh/meta.h"
#include "dh/ascii.h"
#include <stdio.h>

#define draft_fmt__max_occ usize_(8)

typedef enum_((draft_fmt__ArgTag $fits($packed))(
    draft_fmt__ArgTag_void = 0,
    draft_fmt__ArgTag_u32,
    draft_fmt__ArgTag_i32,
    draft_fmt__ArgTag_f64,
    draft_fmt__ArgTag_sli_u8,
    count$draft_fmt__ArgTag,
)) draft_fmt__ArgTag;
T_use_O$(draft_fmt__ArgTag);

typedef struct draft_fmt__Iter {
    var_(rest, S_const$u8);
    var_(occ_idx, usize);
} draft_fmt__Iter;

typedef enum_((draft_fmt__EvtKind $fits($packed))(
    draft_fmt__EvtKind_none = 0,
    draft_fmt__EvtKind_placeholder,
    draft_fmt__EvtKind_escaped_percent_brace,
    count$draft_fmt__EvtKind,
)) draft_fmt__EvtKind;
T_use_O$(draft_fmt__EvtKind);

typedef struct draft_fmt__Evt {
    var_(kind, draft_fmt__EvtKind);
    var_(pos, usize);
} draft_fmt__Evt;

$attr($inline_always)
$static fn_((draft_fmt__formatRuntime(S$u8 mem, S_const$u8 fmt, $va_args))(E$S$u8));

$attr($inline_always)
$static fn_((draft_fmt__Iter_init(S_const$u8 fmt))(draft_fmt__Iter));
$attr($inline_always)
$static fn_((draft_fmt__Iter_next(draft_fmt__Iter* iter, S$u8 out, $va_args))(O$usize));
$attr($inline_always)
$static fn_((draft_fmt__Iter_countNext(draft_fmt__Iter* iter))(usize));

$attr($inline_always)
$static fn_((draft_fmt__countEventsBounded(S_const$u8 fmt))(usize));
$attr($inline_always)
$static fn_((draft_fmt__findEventBounded(S_const$u8 fmt))(draft_fmt__Evt));
$attr($inline_always)
$static fn_((draft_fmt__findCloseBraceBounded(S_const$u8 fmt))(O$usize));
$attr($inline_always)
$static fn_((draft_fmt__copyLiteral(S$u8 mem, S_const$u8 fmt, usize start, usize len))(usize));
$attr($inline_always)
$static fn_((draft_fmt__argIdx(S_const$u8 spec, usize occ_idx))(usize));
$attr($inline_always)
$static fn_((draft_fmt__argTag(S_const$u8 spec))(draft_fmt__ArgTag));

$attr($inline_always)
$static fn_((draft_fmt__writeField(S$u8 mem, draft_fmt__ArgTag tag, u_P_const$raw field))(usize));
$static fn_((draft_fmt__writeVoid(S$u8 mem, Void arg))(usize));
$static fn_((draft_fmt__writeU32(S$u8 mem, u32 arg))(usize));
$static fn_((draft_fmt__writeI32(S$u8 mem, i32 arg))(usize));
$static fn_((draft_fmt__writeF64(S$u8 mem, f64 arg))(usize));
$static fn_((draft_fmt__writeSliU8(S$u8 mem, S_const$u8 arg))(usize));

fn_((main(void))(E$void) $scope) {
    var_(mem0, A$$(64, u8)) = A_zero();
    var_(mem1, A$$(64, u8)) = A_zero();
    var_(mem2, A$$(64, u8)) = A_zero();
    var_(mem3, A$$(64, u8)) = A_zero();
    var_(mem4, A$$(64, u8)) = A_zero();
    var_(mem5, A$$(64, u8)) = A_zero();
    var_(mem6, A$$(64, u8)) = A_zero();
    var_(mem7, A$$(64, u8)) = A_zero();
    var_(mem8, A$$(64, u8)) = A_zero();

    let s0 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem0), u8_l("case0: literal ok abc"))()));
    let s1 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem1), u8_l("case1: %{0}def"))(Void_())));
    let s2 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem2), u8_l("case2: %{i} ghi"))(123)));
    let s3 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem3), u8_l("case3: %{s} jkl%{i}mno"))(u8_l("world"), 123)));
    let s4 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem4), u8_l("case4: %{s} pqr %{u} stu %{i} vw"))(u8_l("world"), u32_(7), -5)));
    let s5 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem5), u8_l("case5: %{1:i}%{0:s}%{0:s}%{1:i} xyz"))(u8_l("456"), 123)));
    let s6 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem6), u8_l("case6: literal {ok} abc"))()));
    let s7 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem7), u8_l("case7: {pre} %{i} {post}"))(123)));
    let s8 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem8), u8_l("case8: %%{pre} %{s} %%{post}"))(u8_l("world"))));

    puts(as$(const char*)(s0.ptr));
    puts(as$(const char*)(s1.ptr));
    puts(as$(const char*)(s2.ptr));
    puts(as$(const char*)(s3.ptr));
    puts(as$(const char*)(s4.ptr));
    puts(as$(const char*)(s5.ptr));
    puts(as$(const char*)(s6.ptr));
    puts(as$(const char*)(s7.ptr));
    puts(as$(const char*)(s8.ptr));

    return_ok({});
} $unscoped(fn);

fn_((draft_fmt__formatRuntime(S$u8 mem, S_const$u8 fmt, $va_args))(E$S$u8) $scope) {
    claim_assert(TypeInfo_eql($va_ref_tup.type, u_typeInfoRecord($va_ty_tup_fields)));
    var iter = draft_fmt__Iter_init(fmt);
    let item_count = draft_fmt__countEventsBounded(fmt);
    claim_assert(item_count <= draft_fmt__max_occ);
    var out = mem;

    /// 2^3 = 8
    if (0 < item_count) out = S_suffix((out)unwrap_(draft_fmt__Iter_next(&iter, out, $va_ty_tup_fields, $va_ref_tup)));
    if (1 < item_count) out = S_suffix((out)unwrap_(draft_fmt__Iter_next(&iter, out, $va_ty_tup_fields, $va_ref_tup)));
    if (2 < item_count) out = S_suffix((out)unwrap_(draft_fmt__Iter_next(&iter, out, $va_ty_tup_fields, $va_ref_tup)));
    if (3 < item_count) out = S_suffix((out)unwrap_(draft_fmt__Iter_next(&iter, out, $va_ty_tup_fields, $va_ref_tup)));
    if (4 < item_count) out = S_suffix((out)unwrap_(draft_fmt__Iter_next(&iter, out, $va_ty_tup_fields, $va_ref_tup)));
    if (5 < item_count) out = S_suffix((out)unwrap_(draft_fmt__Iter_next(&iter, out, $va_ty_tup_fields, $va_ref_tup)));
    if (6 < item_count) out = S_suffix((out)unwrap_(draft_fmt__Iter_next(&iter, out, $va_ty_tup_fields, $va_ref_tup)));
    if (7 < item_count) out = S_suffix((out)unwrap_(draft_fmt__Iter_next(&iter, out, $va_ty_tup_fields, $va_ref_tup)));

    out = S_suffix((out)(draft_fmt__copyLiteral(out, iter.rest, 0, iter.rest.len)));
    return_ok(S_prefix((mem)(mem.len - out.len)));
} $unscoped(fn);

fn_((draft_fmt__Iter_init(S_const$u8 fmt))(draft_fmt__Iter)) {
    return (draft_fmt__Iter){
        .rest = fmt,
        .occ_idx = 0,
    };
};

fn_((draft_fmt__Iter_next(draft_fmt__Iter* iter, S$u8 out, $va_args))(O$usize) $scope) {
    claim_assert_nonnull(iter);
    let rest = iter->rest;
    let event = draft_fmt__findEventBounded(rest);
    if (event.kind == draft_fmt__EvtKind_none) return_none();

    var written = draft_fmt__copyLiteral(out, rest, 0, event.pos);
    if (event.kind == draft_fmt__EvtKind_escaped_percent_brace) {
        *S_at((out)[written++]) = u8_c('%');
        *S_at((out)[written++]) = u8_c('{');
        iter->rest = S_suffix((rest)(event.pos + 3));
        return_some(written);
    }

    let spec_start = event.pos + 2;
    let close = unwrap_(draft_fmt__findCloseBraceBounded(S_suffix((rest)spec_start)));
    let spec = S_prefix((S_suffix((rest)spec_start))close);
    let arg_idx = draft_fmt__argIdx(spec, iter->occ_idx);
    claim_assert(arg_idx < $va_ty_tup_fields.len);
    let field = u_fieldPtr($va_ref_tup, $va_ty_tup_fields, arg_idx);
    let tag = draft_fmt__argTag(spec);
    written += draft_fmt__writeField(S_suffix((out)written), tag, field);
    iter->rest = S_suffix((rest)(spec_start + close + 1));
    iter->occ_idx += 1;
    return_some(written);
} $unscoped(fn);

fn_((draft_fmt__Iter_countNext(draft_fmt__Iter* iter))(usize) $scope) {
    claim_assert_nonnull(iter);
    let event = draft_fmt__findEventBounded(iter->rest);
    if (event.kind == draft_fmt__EvtKind_none) return 0;
    if (event.kind == draft_fmt__EvtKind_escaped_percent_brace) {
        iter->rest = S_suffix((iter->rest)(event.pos + 3));
        return 1;
    }

    let spec_start = event.pos + 2;
    let close = unwrap_(draft_fmt__findCloseBraceBounded(S_suffix((iter->rest)spec_start)));
    iter->rest = S_suffix((iter->rest)(spec_start + close + 1));
    return 1;
} $unscoped(fn);

fn_((draft_fmt__countEventsBounded(S_const$u8 fmt))(usize)) {
    var iter = draft_fmt__Iter_init(fmt);
    var count = usize_(0);
    count += draft_fmt__Iter_countNext(&iter);
    count += draft_fmt__Iter_countNext(&iter);
    count += draft_fmt__Iter_countNext(&iter);
    count += draft_fmt__Iter_countNext(&iter);
    count += draft_fmt__Iter_countNext(&iter);
    count += draft_fmt__Iter_countNext(&iter);
    count += draft_fmt__Iter_countNext(&iter);
    count += draft_fmt__Iter_countNext(&iter);
    return count;
};

fn_((draft_fmt__findEventBounded(S_const$u8 fmt))(draft_fmt__Evt)) {
    /// 2^3 + 2^4 + 2^5 = 56
#define draft_fmt__checkEventAt(pos_) \
    if (fmt.len > usize_((pos_) + 2) \
        && *S_at((fmt)[usize_(pos_)]) == u8_c('%') \
        && *S_at((fmt)[usize_((pos_) + 1)]) == u8_c('%') \
        && *S_at((fmt)[usize_((pos_) + 2)]) == u8_c('{')) { \
        return (draft_fmt__Evt){ .kind = draft_fmt__EvtKind_escaped_percent_brace, .pos = usize_(pos_) }; \
    } \
    if (fmt.len > usize_((pos_) + 1) \
        && *S_at((fmt)[usize_(pos_)]) == u8_c('%') \
        && *S_at((fmt)[usize_((pos_) + 1)]) == u8_c('{')) { \
        return (draft_fmt__Evt){ .kind = draft_fmt__EvtKind_placeholder, .pos = usize_(pos_) }; \
    }
    draft_fmt__checkEventAt(0);
    draft_fmt__checkEventAt(1);
    draft_fmt__checkEventAt(2);
    draft_fmt__checkEventAt(3);
    draft_fmt__checkEventAt(4);
    draft_fmt__checkEventAt(5);
    draft_fmt__checkEventAt(6);
    draft_fmt__checkEventAt(7);
    draft_fmt__checkEventAt(8);
    draft_fmt__checkEventAt(9);
    draft_fmt__checkEventAt(10);
    draft_fmt__checkEventAt(11);
    draft_fmt__checkEventAt(12);
    draft_fmt__checkEventAt(13);
    draft_fmt__checkEventAt(14);
    draft_fmt__checkEventAt(15);
    draft_fmt__checkEventAt(16);
    draft_fmt__checkEventAt(17);
    draft_fmt__checkEventAt(18);
    draft_fmt__checkEventAt(19);
    draft_fmt__checkEventAt(20);
    draft_fmt__checkEventAt(21);
    draft_fmt__checkEventAt(22);
    draft_fmt__checkEventAt(23);
    draft_fmt__checkEventAt(24);
    draft_fmt__checkEventAt(25);
    draft_fmt__checkEventAt(26);
    draft_fmt__checkEventAt(27);
    draft_fmt__checkEventAt(28);
    draft_fmt__checkEventAt(29);
    draft_fmt__checkEventAt(30);
    draft_fmt__checkEventAt(31);
    draft_fmt__checkEventAt(32);
    draft_fmt__checkEventAt(33);
    draft_fmt__checkEventAt(34);
    draft_fmt__checkEventAt(35);
    draft_fmt__checkEventAt(36);
    draft_fmt__checkEventAt(37);
    draft_fmt__checkEventAt(38);
    draft_fmt__checkEventAt(39);
    draft_fmt__checkEventAt(40);
    draft_fmt__checkEventAt(41);
    draft_fmt__checkEventAt(42);
    draft_fmt__checkEventAt(43);
    draft_fmt__checkEventAt(44);
    draft_fmt__checkEventAt(45);
    draft_fmt__checkEventAt(46);
    draft_fmt__checkEventAt(47);
    draft_fmt__checkEventAt(48);
    draft_fmt__checkEventAt(49);
    draft_fmt__checkEventAt(50);
    draft_fmt__checkEventAt(51);
    draft_fmt__checkEventAt(52);
    draft_fmt__checkEventAt(53);
    draft_fmt__checkEventAt(54);
    draft_fmt__checkEventAt(55);
#undef draft_fmt__checkEventAt
    return (draft_fmt__Evt){ .kind = draft_fmt__EvtKind_none, .pos = 0 };
};

fn_((draft_fmt__findCloseBraceBounded(S_const$u8 fmt))(O$usize) $scope) {
    /// 2^4 = 16
#define draft_fmt__checkCloseAt(pos_) \
    if (fmt.len > usize_(pos_) && *S_at((fmt)[usize_(pos_)]) == u8_c('}')) return_some(usize_(pos_));
    draft_fmt__checkCloseAt(0);
    draft_fmt__checkCloseAt(1);
    draft_fmt__checkCloseAt(2);
    draft_fmt__checkCloseAt(3);
    draft_fmt__checkCloseAt(4);
    draft_fmt__checkCloseAt(5);
    draft_fmt__checkCloseAt(6);
    draft_fmt__checkCloseAt(7);
    draft_fmt__checkCloseAt(8);
    draft_fmt__checkCloseAt(9);
    draft_fmt__checkCloseAt(10);
    draft_fmt__checkCloseAt(11);
    draft_fmt__checkCloseAt(12);
    draft_fmt__checkCloseAt(13);
    draft_fmt__checkCloseAt(14);
    draft_fmt__checkCloseAt(15);
#undef draft_fmt__checkCloseAt
    return_none();
} $unscoped(fn);

fn_((draft_fmt__copyLiteral(S$u8 mem, S_const$u8 fmt, usize start, usize len))(usize)) {
    let_ignore = mem_copyBytes(mem, S_prefix((S_suffix((fmt)start))len));
    return len;
};

fn_((draft_fmt__argIdx(S_const$u8 spec, usize occ_idx))(usize)) {
    claim_assert(spec.len == 1 || spec.len == 3);
    if (spec.len == 1) return occ_idx;
    claim_assert(ascii_isDigit(*S_at((spec)[0])));
    claim_assert(*S_at((spec)[1]) == u8_c(':'));
    return as$(usize)(ascii_intFromDigit(*S_at((spec)[0])));
};

fn_((draft_fmt__argTag(S_const$u8 spec))(draft_fmt__ArgTag)) {
    claim_assert(spec.len == 1 || spec.len == 3);
    let spec_ch = spec.len == 1 ? *S_at((spec)[0]) : *S_at((spec)[2]);
    switch (spec_ch) {
    case_((u8_c('0'))) return draft_fmt__ArgTag_void $end(case);
    case_((u8_c('u'))) return draft_fmt__ArgTag_u32 $end(case);
    case_((u8_c('i'))) return draft_fmt__ArgTag_i32 $end(case);
    case_((u8_c('d'))) return draft_fmt__ArgTag_i32 $end(case);
    case_((u8_c('f'))) return draft_fmt__ArgTag_f64 $end(case);
    case_((u8_c('s'))) return draft_fmt__ArgTag_sli_u8 $end(case);
    default_() claim_unreachable $end(default);
    };
};

fn_((draft_fmt__writeField(S$u8 mem, draft_fmt__ArgTag tag, u_P_const$raw field))(usize)) {
    switch (tag) {
    case_((draft_fmt__ArgTag_void)) {
        claim_assert(tag == draft_fmt__ArgTag_void);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(Void)));
        return draft_fmt__writeVoid(mem, *u_castP$((const Void*)(field)));
    } $end(case);
    case_((draft_fmt__ArgTag_u32)) {
        claim_assert(tag == draft_fmt__ArgTag_u32);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(u32)));
        return draft_fmt__writeU32(mem, *u_castP$((const u32*)(field)));
    } $end(case);
    case_((draft_fmt__ArgTag_i32)) {
        claim_assert(tag == draft_fmt__ArgTag_i32);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(i32)));
        return draft_fmt__writeI32(mem, *u_castP$((const i32*)(field)));
    } $end(case);
    case_((draft_fmt__ArgTag_f64)) {
        claim_assert(tag == draft_fmt__ArgTag_f64);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(f64)));
        return draft_fmt__writeF64(mem, *u_castP$((const f64*)(field)));
    } $end(case);
    case_((draft_fmt__ArgTag_sli_u8)) {
        claim_assert(tag == draft_fmt__ArgTag_sli_u8);
        claim_assert(TypeInfo_eql(field.type, typeInfo$(S_const$u8)));
        return draft_fmt__writeSliU8(mem, *u_castP$((const S_const$u8*)(field)));
    } $end(case);
    case_((count$draft_fmt__ArgTag)) claim_unreachable $end(case);
    };
};

fn_((draft_fmt__writeVoid(S$u8 mem, Void arg))(usize)) {
    let_ignore = mem;
    let_ignore = arg;
    return 0;
};

fn_((draft_fmt__writeU32(S$u8 mem, u32 arg))(usize)) {
    var_(tmp, A$$(40, u8)) = A_zero();
    var pos = A_len(tmp);
    var val = arg;
    if (isZero(val)) *A_at((tmp)[--pos]) = u8_c('0');
    else {
        while (0 < val) {
            *A_at((tmp)[--pos]) = ascii_digitFromInt(val % 10);
            val /= 10;
        }
    }
    let text = A_suffix$((S_const$u8)(tmp)(pos));
    let_ignore = mem_copyBytes(mem, text);
    return text.len;
};

fn_((draft_fmt__writeI32(S$u8 mem, i32 arg))(usize)) {
    let is_ngtv = cmp_Sgn_isNgtv(pri_sgn(arg));
    let abs_val = intCast$((u32)(pri_abs(arg)));

    var_(tmp, A$$(11, u8)) = A_zero();
    var pos = A_len(tmp);
    if (isZero(abs_val)) *A_at((tmp)[--pos]) = u8_c('0');
    else {
        var val = abs_val;
        while (val > 0) {
            *A_at((tmp)[--pos]) = ascii_digitFromInt(val % 10);
            val /= 10;
        }
    }
    if (is_ngtv) *A_at((tmp)[--pos]) = u8_c('-');

    let digits = A_suffix$((S_const$u8)(tmp)(pos));
    let_ignore = mem_copyBytes(mem, digits);
    return digits.len;
};

fn_((draft_fmt__writeF64(S$u8 mem, f64 arg))(usize)) {
    let negative = arg < 0.0;
    let abs_val = negative ? -arg : arg;
    let int_part = as$(u32)(abs_val);
    let frac_part = as$(u32)((abs_val - as$(f64)(int_part)) * 1000000.0 + 0.5);
    var written = usize_(0);
    if (negative) *S_at((mem)[written++]) = u8_c('-');
    written += draft_fmt__writeU32(S_suffix((mem)written), int_part);
    *S_at((mem)[written++]) = u8_c('.');
    var_(frac, A$$(6, u8)) = A_zero();
    var pos = A_len(frac);
    var val = frac_part;
    for_(($r(0, A_len(frac)))(i)) {
        let_ignore = i;
        *A_at((frac)[--pos]) = ascii_digitFromInt(val % 10);
        val /= 10;
    } $end(for);
    let_ignore = mem_copyBytes(S_suffix((mem)written), A_ref$((S_const$u8)frac));
    return written + A_len(frac);
};

fn_((draft_fmt__writeSliU8(S$u8 mem, S_const$u8 arg))(usize)) {
    let_ignore = mem_copyBytes(mem, arg);
    return arg.len;
};
