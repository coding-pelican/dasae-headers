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

$attr($inline_always)
$static fn_((draft_fmt__formatRuntime(S$u8 mem, S_const$u8 fmt, $va_args))(E$S$u8));

$attr($inline_always)
$static fn_((draft_fmt__Iter_init(S_const$u8 fmt))(draft_fmt__Iter));
$attr($inline_always)
$static fn_((draft_fmt__Iter_next(draft_fmt__Iter* iter, S$u8 out, $va_args))(O$usize));

$attr($inline_always)
$static fn_((draft_fmt__countUnit(S_const$u8 fmt, u8 unit))(usize));
$attr($inline_always)
$static fn_((draft_fmt__findFirstUnit(S_const$u8 mem, u8 unit))(O$usize));
$attr($inline_always)
$static fn_((draft_fmt__findRequiredUnit(S_const$u8 mem, u8 unit))(usize));
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

    let s0 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem0), u8_l("case0: literal ok abc"))()));
    let s1 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem1), u8_l("case1: {0}def"))(Void_())));
    let s2 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem2), u8_l("case2: {i} ghi"))(123)));
    let s3 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem3), u8_l("case3: {s} jkl{i}mno"))(u8_l("world"), 123)));
    let s4 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem4), u8_l("case4: {s} pqr {u} stu {i} vw"))(u8_l("world"), u32_(7), -5)));
    let s5 = try_(va_((draft_fmt__formatRuntime)(A_ref$((S$u8)mem5), u8_l("case5: {1:i}{0:s}{0:s}{1:i} xyz"))(u8_l("456"), 123)));

    puts(as$(const char*)(s0.ptr));
    puts(as$(const char*)(s1.ptr));
    puts(as$(const char*)(s2.ptr));
    puts(as$(const char*)(s3.ptr));
    puts(as$(const char*)(s4.ptr));
    puts(as$(const char*)(s5.ptr));

    return_ok({});
} $unscoped(fn);

fn_((draft_fmt__formatRuntime(S$u8 mem, S_const$u8 fmt, $va_args))(E$S$u8) $scope) {
    claim_assert(TypeInfo_eql($va_ref_tup.type, u_typeInfoRecord($va_ty_tup_fields)));
    var iter = draft_fmt__Iter_init(fmt);
    let item_count = draft_fmt__countUnit(fmt, u8_c('{'));
    claim_assert(item_count <= draft_fmt__max_occ);
    var out = mem;

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
    let brace = orelse_((draft_fmt__findFirstUnit(rest, u8_c('{')))(return_none()));
    let spec_start = brace + 1;
    let end = spec_start + draft_fmt__findRequiredUnit(S_suffix((rest)spec_start), u8_c('}'));
    let spec = S_slice((rest)$r(brace + 1, end));

    var written = draft_fmt__copyLiteral(out, rest, 0, brace);
    let arg_idx = draft_fmt__argIdx(spec, iter->occ_idx);
    claim_assert(arg_idx < $va_ty_tup_fields.len);
    let field = u_fieldPtr($va_ref_tup, $va_ty_tup_fields, arg_idx);
    let tag = draft_fmt__argTag(spec);
    written += draft_fmt__writeField(S_suffix((out)written), tag, field);
    iter->rest = S_suffix((rest)(end + 1));
    iter->occ_idx += 1;
    return_some(written);
} $unscoped(fn);

fn_((draft_fmt__countUnit(S_const$u8 fmt, u8 unit))(usize)) {
    var_(count, usize) = 0;
    for_(($s(fmt))(ch)) {
        if (*ch == unit) count++;
    } $end(for);
    return count;
};

fn_((draft_fmt__findFirstUnit(S_const$u8 mem, u8 unit))(O$usize)) {
    return mem_findFirstUnitBytes(mem, unit);
};

fn_((draft_fmt__findRequiredUnit(S_const$u8 mem, u8 unit))(usize)) {
    return unwrap_(draft_fmt__findFirstUnit(mem, unit));
};

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
