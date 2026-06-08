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

typedef void (*draft_fmt__WriteFn)(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u_P_const$raw arg);
typedef struct draft_fmt__Arg {
    var_(tag, draft_fmt__ArgTag);
    var_(type, TypeInfo);
    var_(ref, u_P_const$raw);
    var_(write, draft_fmt__WriteFn);
} draft_fmt__Arg;
T_use_S$(draft_fmt__Arg);

$attr($inline_always)
$static fn_((draft_fmt__formatRuntime(S$u8 mem, S_const$u8 fmt, S_const$draft_fmt__Arg args))(E$S$u8));
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
$attr($inline_always)
$static fn_((draft_fmt__writeVoidRef(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u_P_const$raw arg))(void));
$attr($inline_always)
$static fn_((draft_fmt__writeI32Ref(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u_P_const$raw arg))(void));
$attr($inline_always)
$static fn_((draft_fmt__writeU32Ref(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u_P_const$raw arg))(void));
$attr($inline_always)
$static fn_((draft_fmt__writeF64Ref(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u_P_const$raw arg))(void));
$attr($inline_always)
$static fn_((draft_fmt__writeSliU8Ref(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u_P_const$raw arg))(void));

#define draft_fmt__arg(_arg...) _arg

#define draft_fmt__writeArg(_mem, _written, _tag, _arg...) \
    T_switch$((TypeOf(_arg))( \
        T_case$((Void)(draft_fmt__writeVoid)), \
        T_case$((i32)(draft_fmt__writeI32)), \
        T_case$((u32)(draft_fmt__writeU32)), \
        T_case$((f64)(draft_fmt__writeF64)), \
        T_case$((S_const$u8)(draft_fmt__writeSliU8)) \
    ))(_mem, _written, _tag, _arg)

#define draft_fmt__argTag(_arg...) \
    T_switch$((TypeOf(_arg))( \
        T_case$((Void)(draft_fmt__ArgTag_void)), \
        T_case$((i32)(draft_fmt__ArgTag_i32)), \
        T_case$((u32)(draft_fmt__ArgTag_u32)), \
        T_case$((f64)(draft_fmt__ArgTag_f64)), \
        T_case$((S_const$u8)(draft_fmt__ArgTag_sli_u8)) \
    ))

#define draft_fmt__argWrite(_arg...) \
    T_switch$((TypeOf(_arg))( \
        T_case$((Void)(draft_fmt__writeVoidRef)), \
        T_case$((i32)(draft_fmt__writeI32Ref)), \
        T_case$((u32)(draft_fmt__writeU32Ref)), \
        T_case$((f64)(draft_fmt__writeF64Ref)), \
        T_case$((S_const$u8)(draft_fmt__writeSliU8Ref)) \
    ))

#define draft_fmt__argSlot(_arg...) \
    ((draft_fmt__Arg){ \
        .tag = draft_fmt__argTag(_arg), \
        .type = typeInfo$(TypeOf(_arg)), \
        .ref = u_anyP(&(_arg)), \
        .write = draft_fmt__argWrite(_arg), \
    })

#define draft_fmt__formatTuple(_args...) \
    pp_overload(__draft_fmt__formatTuple, _args)(_args)

#define __draft_fmt__formatTuple_2(_mem, _fmt) ({ \
    let __mem = _mem; \
    let __fmt = _fmt; \
    draft_fmt__formatRuntime(__mem, __fmt, (S_const$draft_fmt__Arg){ .ptr = null, .len = 0 }); \
})

#define __draft_fmt__formatTuple_3(_mem, _fmt, _arg0...) ({ \
    let __mem = _mem; \
    let __fmt = _fmt; \
    let __arg0 = draft_fmt__arg _arg0; \
    let __args = A_from$((draft_fmt__Arg){ draft_fmt__argSlot(__arg0) }); \
    draft_fmt__formatRuntime(__mem, __fmt, A_ref$((S_const$draft_fmt__Arg)__args)); \
})

#define __draft_fmt__formatTuple_4(_mem, _fmt, _arg0, _arg1...) ({ \
    let __mem = _mem; \
    let __fmt = _fmt; \
    let __arg0 = draft_fmt__arg _arg0; \
    let __arg1 = draft_fmt__arg _arg1; \
    let __args = A_from$((draft_fmt__Arg){ draft_fmt__argSlot(__arg0), draft_fmt__argSlot(__arg1) }); \
    draft_fmt__formatRuntime(__mem, __fmt, A_ref$((S_const$draft_fmt__Arg)__args)); \
})

#define __draft_fmt__formatTuple_5(_mem, _fmt, _arg0, _arg1, _arg2...) ({ \
    let __mem = _mem; \
    let __fmt = _fmt; \
    let __arg0 = draft_fmt__arg _arg0; \
    let __arg1 = draft_fmt__arg _arg1; \
    let __arg2 = draft_fmt__arg _arg2; \
    let __args = A_from$((draft_fmt__Arg){ draft_fmt__argSlot(__arg0), draft_fmt__argSlot(__arg1), draft_fmt__argSlot(__arg2) }); \
    draft_fmt__formatRuntime(__mem, __fmt, A_ref$((S_const$draft_fmt__Arg)__args)); \
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

fn_((draft_fmt__formatRuntime(S$u8 mem, S_const$u8 fmt, S_const$draft_fmt__Arg args))(E$S$u8) $scope) {
    let parsed = draft_fmt__parse(fmt);
    claim_assert(parsed.count <= draft_fmt__max_occurrences);

    var_(written, usize) = 0;
    for_(($r(0, parsed.count))(occ)) {
        draft_fmt__copyLiteral(
            mem, &written, fmt,
            *A_at((parsed.literal_starts)[occ]),
            *A_at((parsed.literal_lens)[occ])
        );

        let arg_idx = *A_at((parsed.arg_indices)[occ]);
        claim_assert(arg_idx < args.len);
        let arg = S_at((args)[arg_idx]);
        let tag = *A_at((parsed.tags)[occ]);
        claim_assert(arg->tag == tag);
        arg->write(mem, &written, tag, arg->ref);
    } $end(for);

    draft_fmt__copyLiteral(mem, &written, fmt, parsed.trailing_start, parsed.trailing_len);
    return_ok(S_prefix((mem)written));
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

fn_((draft_fmt__writeVoidRef(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u_P_const$raw arg))(void)) {
    draft_fmt__writeVoid(mem, written, tag, *u_castP$((const Void*)(arg)));
};

fn_((draft_fmt__writeI32Ref(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u_P_const$raw arg))(void)) {
    draft_fmt__writeI32(mem, written, tag, *u_castP$((const i32*)(arg)));
};

fn_((draft_fmt__writeU32Ref(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u_P_const$raw arg))(void)) {
    draft_fmt__writeU32(mem, written, tag, *u_castP$((const u32*)(arg)));
};

fn_((draft_fmt__writeF64Ref(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u_P_const$raw arg))(void)) {
    draft_fmt__writeF64(mem, written, tag, *u_castP$((const f64*)(arg)));
};

fn_((draft_fmt__writeSliU8Ref(S$u8 mem, usize* written, draft_fmt__ArgTag tag, u_P_const$raw arg))(void)) {
    draft_fmt__writeSliU8(mem, written, tag, *u_castP$((const S_const$u8*)(arg)));
};
