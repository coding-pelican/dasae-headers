#include "dh-main.h"
#include "dh/meta.h"
#include <stdio.h>

#define draft_fmt__max_args usize_(3)
#define draft_fmt__max_occurrences usize_(4)

typedef enum_((draft_fmt__ArgTag $fits($packed))(
    draft_fmt__ArgTag_void = 0,
    draft_fmt__ArgTag_u32,
    draft_fmt__ArgTag_i32,
    draft_fmt__ArgTag_f64,
    draft_fmt__ArgTag_sli_u8,
    count$draft_fmt__ArgTag,
)) draft_fmt__ArgTag;
T_use_O$(draft_fmt__ArgTag);

$static let draft_fmt__tag_by_spec = A_from$((draft_fmt__ArgTag){
    [u8_c('0')] = draft_fmt__ArgTag_void,
    [u8_c('u')] = draft_fmt__ArgTag_u32,
    [u8_c('d')] = draft_fmt__ArgTag_i32,
    [u8_c('i')] = draft_fmt__ArgTag_i32,
    [u8_c('f')] = draft_fmt__ArgTag_f64,
    [u8_c('s')] = draft_fmt__ArgTag_sli_u8,
});
typedef struct draft_fmt__Parsed {
    var_(arg_indices, A$$(draft_fmt__max_occurrences, usize));
    var_(tags, A$$(draft_fmt__max_occurrences, draft_fmt__ArgTag));
    var_(literal_starts, A$$(draft_fmt__max_occurrences, usize));
    var_(literal_lens, A$$(draft_fmt__max_occurrences, usize));
    var_(count, usize);
    var_(trailing_start, usize);
    var_(trailing_len, usize);
} draft_fmt__Parsed;

$attr($inline_always)
$static fn_((draft_fmt__parse(S_const$u8 fmt))(draft_fmt__Parsed));
$attr($inline_always)
$static fn_((draft_fmt__parseOne(draft_fmt__Parsed* parsed, S_const$u8* rest, usize* offset))(bool));
$attr($inline_always)
$static fn_((draft_fmt__copyLiteral(S$u8 mem, usize* written, S_const$u8 fmt, usize start, usize len))(void));
$attr($inline_always)
$static fn_((draft_fmt__writeVoid(S$u8 mem, usize* written, draft_fmt__ArgTag tag, Void arg))(void));
$attr($inline_always)
$static fn_((draft_fmt__writeI32(S$u8 mem, usize* written, draft_fmt__ArgTag tag, i32 arg))(void));
$attr($inline_always)
$static fn_((draft_fmt__writeU32(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u32 arg))(void));
$attr($inline_always)
$static fn_((draft_fmt__writeF64(S$u8 mem, usize* written, draft_fmt__ArgTag tag, f64 arg))(void));
$attr($inline_always)
$static fn_((draft_fmt__writeSliU8(S$u8 mem, usize* written, draft_fmt__ArgTag tag, S_const$u8 arg))(void));

#define draft_fmt__arg(_arg...) _arg

#define draft_fmt__writeArg(_mem, _written, _tag, _arg...) \
    T_switch$((TypeOf(_arg))( \
        T_case$((Void)(draft_fmt__writeVoid)), \
        T_case$((i32)(draft_fmt__writeI32)), \
        T_case$((u32)(draft_fmt__writeU32)), \
        T_case$((f64)(draft_fmt__writeF64)), \
        T_case$((S_const$u8)(draft_fmt__writeSliU8)) \
    ))(_mem, _written, _tag, _arg)

#define draft_fmt__writeOcc1(_mem, _written, _parsed, _occ, _arg0...) { \
    claim_assert(*A_at(((_parsed).arg_indices)[_occ]) == 0); \
    draft_fmt__writeArg(_mem, _written, *A_at(((_parsed).tags)[_occ]), _arg0); \
}

#define draft_fmt__writeOcc2(_mem, _written, _parsed, _occ, _arg0, _arg1...) { \
    let __idx = *A_at(((_parsed).arg_indices)[_occ]); \
    if (__idx == 0) draft_fmt__writeArg(_mem, _written, *A_at(((_parsed).tags)[_occ]), _arg0); \
    else if (__idx == 1) draft_fmt__writeArg(_mem, _written, *A_at(((_parsed).tags)[_occ]), _arg1); \
    else claim_unreachable; \
}

#define draft_fmt__writeOcc3(_mem, _written, _parsed, _occ, _arg0, _arg1, _arg2...) { \
    let __idx = *A_at(((_parsed).arg_indices)[_occ]); \
    if (__idx == 0) draft_fmt__writeArg(_mem, _written, *A_at(((_parsed).tags)[_occ]), _arg0); \
    else if (__idx == 1) draft_fmt__writeArg(_mem, _written, *A_at(((_parsed).tags)[_occ]), _arg1); \
    else if (__idx == 2) draft_fmt__writeArg(_mem, _written, *A_at(((_parsed).tags)[_occ]), _arg2); \
    else claim_unreachable; \
}

#define draft_fmt__copyOcc(_mem, _written, _fmt, _parsed, _occ) \
    draft_fmt__copyLiteral( \
        _mem, &(_written), _fmt, \
        *A_at(((_parsed).literal_starts)[_occ]), \
        *A_at(((_parsed).literal_lens)[_occ]) \
    )

#define draft_fmt__formatTuple(_args...) \
    pp_overload(__draft_fmt__formatTuple, _args)(_args)

#define __draft_fmt__formatTuple_2(_mem, _fmt) ({ \
    let __mem = _mem; \
    let __fmt = _fmt; \
    let __tuple = $tup(); \
    let __ty_fields = typeInfos$(Void); \
    let __tuple_meta = u_load(u_anyV(__tuple)); \
    claim_assert(TypeInfo_eql(__tuple_meta.type, u_typeInfoRecord(__ty_fields))); \
    let __parsed = draft_fmt__parse(__fmt); \
    claim_assert(__parsed.count == 0); \
    var_(__written, usize) = 0; \
    draft_fmt__copyLiteral(__mem, &__written, __fmt, __parsed.trailing_start, __parsed.trailing_len); \
    (E$S$u8)ok(S_prefix((__mem)__written)); \
})

#define __draft_fmt__formatTuple_3(_mem, _fmt, _arg0...) ({ \
    let __mem = _mem; \
    let __fmt = _fmt; \
    let __arg0 = draft_fmt__arg _arg0; \
    let __tuple = $tup((__arg0)); \
    let __ty_fields = typeInfos$(TypeOf(__tuple.$0)); \
    let __tuple_meta = u_load(u_anyV(__tuple)); \
    claim_assert(TypeInfo_eql(__tuple_meta.type, u_typeInfoRecord(__ty_fields))); \
    let __parsed = draft_fmt__parse(__fmt); \
    claim_assert(__parsed.count == 1); \
    var_(__written, usize) = 0; \
    draft_fmt__copyOcc(__mem, __written, __fmt, __parsed, 0); \
    draft_fmt__writeArg(__mem, &__written, *A_at((__parsed.tags)[0]), __tuple.$0); \
    draft_fmt__copyLiteral(__mem, &__written, __fmt, __parsed.trailing_start, __parsed.trailing_len); \
    (E$S$u8)ok(S_prefix((__mem)__written)); \
})

#define __draft_fmt__formatTuple_4(_mem, _fmt, _arg0, _arg1...) ({ \
    let __mem = _mem; \
    let __fmt = _fmt; \
    let __arg0 = draft_fmt__arg _arg0; \
    let __arg1 = draft_fmt__arg _arg1; \
    let __tuple = $tup((__arg0), (__arg1)); \
    let __ty_fields = typeInfos$(TypeOf(__tuple.$0), TypeOf(__tuple.$1)); \
    let __tuple_meta = u_load(u_anyV(__tuple)); \
    claim_assert(TypeInfo_eql(__tuple_meta.type, u_typeInfoRecord(__ty_fields))); \
    let __parsed = draft_fmt__parse(__fmt); \
    claim_assert(__parsed.count <= draft_fmt__max_occurrences); \
    var_(__written, usize) = 0; \
    if (__parsed.count > 0) { draft_fmt__copyOcc(__mem, __written, __fmt, __parsed, 0); draft_fmt__writeOcc2(__mem, &__written, __parsed, 0, __tuple.$0, __tuple.$1); } \
    if (__parsed.count > 1) { draft_fmt__copyOcc(__mem, __written, __fmt, __parsed, 1); draft_fmt__writeOcc2(__mem, &__written, __parsed, 1, __tuple.$0, __tuple.$1); } \
    if (__parsed.count > 2) { draft_fmt__copyOcc(__mem, __written, __fmt, __parsed, 2); draft_fmt__writeOcc2(__mem, &__written, __parsed, 2, __tuple.$0, __tuple.$1); } \
    if (__parsed.count > 3) { draft_fmt__copyOcc(__mem, __written, __fmt, __parsed, 3); draft_fmt__writeOcc2(__mem, &__written, __parsed, 3, __tuple.$0, __tuple.$1); } \
    draft_fmt__copyLiteral(__mem, &__written, __fmt, __parsed.trailing_start, __parsed.trailing_len); \
    (E$S$u8)ok(S_prefix((__mem)__written)); \
})

#define __draft_fmt__formatTuple_5(_mem, _fmt, _arg0, _arg1, _arg2...) ({ \
    let __mem = _mem; \
    let __fmt = _fmt; \
    let __arg0 = draft_fmt__arg _arg0; \
    let __arg1 = draft_fmt__arg _arg1; \
    let __arg2 = draft_fmt__arg _arg2; \
    let __tuple = $tup((__arg0), (__arg1), (__arg2)); \
    let __ty_fields = typeInfos$(TypeOf(__tuple.$0), TypeOf(__tuple.$1), TypeOf(__tuple.$2)); \
    let __tuple_meta = u_load(u_anyV(__tuple)); \
    claim_assert(TypeInfo_eql(__tuple_meta.type, u_typeInfoRecord(__ty_fields))); \
    let __parsed = draft_fmt__parse(__fmt); \
    claim_assert(__parsed.count <= draft_fmt__max_occurrences); \
    var_(__written, usize) = 0; \
    if (__parsed.count > 0) { draft_fmt__copyOcc(__mem, __written, __fmt, __parsed, 0); draft_fmt__writeOcc3(__mem, &__written, __parsed, 0, __tuple.$0, __tuple.$1, __tuple.$2); } \
    if (__parsed.count > 1) { draft_fmt__copyOcc(__mem, __written, __fmt, __parsed, 1); draft_fmt__writeOcc3(__mem, &__written, __parsed, 1, __tuple.$0, __tuple.$1, __tuple.$2); } \
    if (__parsed.count > 2) { draft_fmt__copyOcc(__mem, __written, __fmt, __parsed, 2); draft_fmt__writeOcc3(__mem, &__written, __parsed, 2, __tuple.$0, __tuple.$1, __tuple.$2); } \
    if (__parsed.count > 3) { draft_fmt__copyOcc(__mem, __written, __fmt, __parsed, 3); draft_fmt__writeOcc3(__mem, &__written, __parsed, 3, __tuple.$0, __tuple.$1, __tuple.$2); } \
    draft_fmt__copyLiteral(__mem, &__written, __fmt, __parsed.trailing_start, __parsed.trailing_len); \
    (E$S$u8)ok(S_prefix((__mem)__written)); \
})

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    var_(mem0, A$$(64, u8)) = A_zero();
    var_(mem1, A$$(64, u8)) = A_zero();
    var_(mem2, A$$(64, u8)) = A_zero();
    var_(mem3, A$$(64, u8)) = A_zero();
    var_(mem4, A$$(64, u8)) = A_zero();

    let s0 = try_(draft_fmt__formatTuple(A_ref$((S$u8)mem0), u8_l("case0: literal")));
    let s1 = try_(draft_fmt__formatTuple(A_ref$((S$u8)mem1), u8_l("case1: {i}"), (123)));
    let s2 = try_(draft_fmt__formatTuple(A_ref$((S$u8)mem2), u8_l("case2: {s} {i}"), (u8_l("world")), (123)));
    let s3 = try_(draft_fmt__formatTuple(A_ref$((S$u8)mem3), u8_l("case3: {s} {u} {i}"), (u8_l("world")), (u32_(7)), (-5)));
    let s4 = try_(draft_fmt__formatTuple(A_ref$((S$u8)mem4), u8_l("case4: {1:i}{0:s}{0:s}{1:i}"), (u8_l("456")), (123)));

    puts(as$(const char*)(s0.ptr));
    puts(as$(const char*)(s1.ptr));
    puts(as$(const char*)(s2.ptr));
    puts(as$(const char*)(s3.ptr));
    puts(as$(const char*)(s4.ptr));

    return_ok({});
} $unscoped(fn);

fn_((draft_fmt__parse(S_const$u8 fmt))(draft_fmt__Parsed)) {
    var_(parsed, draft_fmt__Parsed) = {};
    var rest = fmt;
    var_(offset, usize) = 0;
    if (draft_fmt__parseOne(&parsed, &rest, &offset)) {
        if (draft_fmt__parseOne(&parsed, &rest, &offset)) {
            if (draft_fmt__parseOne(&parsed, &rest, &offset)) {
                let_ignore = draft_fmt__parseOne(&parsed, &rest, &offset);
            }
        }
    }
    parsed.trailing_start = offset;
    parsed.trailing_len = rest.len;
    return parsed;
};

fn_((draft_fmt__parseOne(draft_fmt__Parsed* parsed, S_const$u8* rest, usize* offset))(bool)) {
    let brace = orelse_((mem_findFirstUnitBytes(*rest, u8_c('{')))(return false));
    let end = unwrap_(mem_findFirstUnitBytes(*rest, u8_c('}')));
    let spec = S_slice((*rest)$r(brace + 1, end));
    claim_assert(spec.len == 1 || spec.len == 3);

    let idx = parsed->count++;
    *A_at((parsed->literal_starts)[idx]) = *offset;
    *A_at((parsed->literal_lens)[idx]) = brace;
    if (spec.len == 1) {
        *A_at((parsed->arg_indices)[idx]) = idx;
        *A_at((parsed->tags)[idx]) = *A_at((draft_fmt__tag_by_spec)[*S_at((spec)[0])]);
    } else {
        claim_assert(*S_at((spec)[0]) >= u8_c('0') && *S_at((spec)[0]) <= u8_c('9'));
        claim_assert(*S_at((spec)[1]) == u8_c(':'));
        *A_at((parsed->arg_indices)[idx]) = as$(usize)(*S_at((spec)[0]) - u8_c('0'));
        *A_at((parsed->tags)[idx]) = *A_at((draft_fmt__tag_by_spec)[*S_at((spec)[2])]);
    }

    *offset += end + 1;
    *rest = S_suffix((*rest)(end + 1));
    return true;
};

fn_((draft_fmt__copyLiteral(S$u8 mem, usize* written, S_const$u8 fmt, usize start, usize len))(void)) {
    let_ignore = mem_copyBytes(S_suffix((mem)*written), S_slice((fmt)$r(start, start + len)));
    *written += len;
};

fn_((draft_fmt__writeVoid(S$u8 mem, usize* written, draft_fmt__ArgTag tag, Void arg))(void)) {
    let_ignore = mem;
    let_ignore = written;
    let_ignore = arg;
    claim_assert(tag == draft_fmt__ArgTag_void);
};

fn_((draft_fmt__writeI32(S$u8 mem, usize* written, draft_fmt__ArgTag tag, i32 arg))(void)) {
    claim_assert(tag == draft_fmt__ArgTag_i32);
    let is_ngtv = cmp_Sgn_isNgtv(pri_sgn(arg));
    let abs_val = pri_abs(arg);

    var_(tmp, A$$(11, u8)) = A_zero();
    var pos = A_len(tmp);
    if (isZero(abs_val)) *A_at((tmp)[--pos]) = u8_c('0');
    else {
        var val = abs_val;
        while (val > 0) {
            *A_at((tmp)[--pos]) = *S_at((u8_l("0123456789"))[val % 10]);
            val /= 10;
        }
    }
    if (is_ngtv) *A_at((tmp)[--pos]) = u8_c('-');

    let digits = A_suffix$((S_const$u8)(tmp)(pos));
    let_ignore = mem_copyBytes(S_suffix((mem)*written), digits);
    *written += digits.len;
};

fn_((draft_fmt__writeU32(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u32 arg))(void)) {
    claim_assert(tag == draft_fmt__ArgTag_u32);

    var_(tmp, A$$(10, u8)) = A_zero();
    var pos = A_len(tmp);
    if (isZero(arg)) *A_at((tmp)[--pos]) = u8_c('0');
    else {
        var val = arg;
        while (val > 0) {
            *A_at((tmp)[--pos]) = *S_at((u8_l("0123456789"))[val % 10]);
            val /= 10;
        }
    }

    let digits = A_suffix$((S_const$u8)(tmp)(pos));
    let_ignore = mem_copyBytes(S_suffix((mem)*written), digits);
    *written += digits.len;
};

fn_((draft_fmt__writeF64(S$u8 mem, usize* written, draft_fmt__ArgTag tag, f64 arg))(void)) {
    claim_assert(tag == draft_fmt__ArgTag_f64);

    var_(tmp, A$$(64, u8)) = A_zero();
    let len = snprintf(as$(char*)(A_ptr(tmp)), A_len(tmp), "%f", as$(double)(arg));
    claim_assert(len >= 0);
    let text = A_prefix$((S_const$u8)(tmp)(as$(usize)(len)));
    let_ignore = mem_copyBytes(S_suffix((mem)*written), text);
    *written += text.len;
};

fn_((draft_fmt__writeSliU8(S$u8 mem, usize* written, draft_fmt__ArgTag tag, S_const$u8 arg))(void)) {
    claim_assert(tag == draft_fmt__ArgTag_sli_u8);
    let_ignore = mem_copyBytes(S_suffix((mem)*written), arg);
    *written += arg.len;
};
