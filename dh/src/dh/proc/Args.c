#include "dh/proc/Args.h"
#include "dh/start/Invoc/Args.h"
#include "dh/mem/common.h"
#include "dh/unicode.h"

/*========== Internal Declarations ==========================================*/

$static fn_((proc_Args__emptyNext(
    P$raw ctx,
    usize* idx,
    usize* offset,
    S$u8 scratch
))(proc_Args_E$O$S_const$u8));
$static fn_((proc_Args__emptySkip(
    P$raw ctx,
    usize* idx,
    usize* offset
))(proc_Args_E$bool));

$static fn_((proc_Args__emitWin32CodeUnit(
    S$u8 scratch,
    P$usize written,
    u16 code_unit,
    P$bool has_last,
    P$u16 last
))(proc_Args_E$void));
$static fn_((proc_Args__emitWin32Backslashes(
    S$u8 scratch,
    P$usize written,
    usize count,
    bool capture,
    P$bool has_last,
    P$u16 last
))(proc_Args_E$void));
$static fn_((proc_Args__parseWin32(
    start_Invoc_Args_WTF16 wtf16,
    P$usize index,
    P$usize offset,
    S$u8 scratch,
    bool capture,
    P$usize written
))(proc_Args_E$bool));
$static fn_((start_Invoc_Args__next(
    P$raw ctx,
    usize* idx,
    usize* offset,
    S$u8 scratch
))(proc_Args_E$O$S_const$u8));
$static fn_((start_Invoc_Args__skip(
    P$raw ctx,
    usize* idx,
    usize* offset
))(proc_Args_E$bool));

$static var_(proc_Args__empty_ctx, Void) = cleared();

$static let_(proc_Args__empty_vtbl, proc_Args_VTbl) = {
    .nextFn = proc_Args__emptyNext,
    .skipFn = proc_Args__emptySkip,
};

$static let_(start_Invoc_Args__vtbl, proc_Args_VTbl) = {
    .nextFn = start_Invoc_Args__next,
    .skipFn = start_Invoc_Args__skip,
};

/*========== External Definitions ===========================================*/

let_(proc_Args_empty, proc_Args) = {
    .ctx = &proc_Args__empty_ctx,
    .vtbl = &proc_Args__empty_vtbl,
};

fn_((proc_Args_iter(proc_Args self))(proc_Args_Iter)) {
    return (proc_Args_Iter){
        .src = proc_Args_ensureValid(self),
        .idx = 0,
        .offset = 0,
    };
};

fn_((proc_Args_Iter_next(
    proc_Args_Iter* self,
    S$u8 scratch
))(proc_Args_E$O$S_const$u8)) {
    claim_assert_nonnull(self);
    let src = proc_Args_ensureValid(self->src);
    return src.vtbl->nextFn(
        src.ctx, &self->idx, &self->offset, scratch
    );
};

fn_((proc_Args_Iter_skip(proc_Args_Iter* self))(proc_Args_E$bool)) {
    claim_assert_nonnull(self);
    let src = proc_Args_ensureValid(self->src);
    return src.vtbl->skipFn(src.ctx, &self->idx, &self->offset);
};

fn_((start_Invoc_Args_self(start_Invoc_Args* self))(proc_Args)) {
    claim_assert_nonnull(self);
    return proc_Args_ensureValid((proc_Args){
        .ctx = self,
        .vtbl = &start_Invoc_Args__vtbl,
    });
};

/*========== Internal Definitions ===========================================*/

/*--- Common ---*/

$static fn_((proc_Args__emptyNext(
    P$raw ctx,
    usize* idx,
    usize* offset,
    S$u8 scratch
))(proc_Args_E$O$S_const$u8) $scope) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(idx);
    claim_assert_nonnull(offset);
    let_ignore = idx;
    let_ignore = offset;
    let_ignore = scratch;
    return_ok(none());
} $unscoped(fn);

$static fn_((proc_Args__emptySkip(
    P$raw ctx,
    usize* idx,
    usize* offset
))(proc_Args_E$bool) $scope) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(idx);
    claim_assert_nonnull(offset);
    let_ignore = idx;
    let_ignore = offset;
    return_ok(false);
} $unscoped(fn);

/*--- Start Invocation ---*/

$static fn_((proc_Args__emitWin32CodeUnit(
    S$u8 scratch,
    P$usize written,
    u16 code_unit,
    P$bool has_last,
    P$u16 last
))(proc_Args_E$void) $scope) {
    claim_assert_nonnull(written);
    claim_assert_nonnull(has_last);
    claim_assert_nonnull(last);
    if (
        *has_last
        && utf16_isHighSurrogate(*last)
        && utf16_isLowSurrogate(code_unit)
    ) {
        var_(pair, A$$(2, u16)) = A_init({ *last, code_unit });
        let pair_slice = A_ref$((S_const$u16)(pair));
        claim_assert(unicode_wtf16ToWTF8Len(pair_slice) == 4);
        claim_assert(*written >= 3);
        if (*written + 1 > scratch.len) {
            return_err(E_cause$proc_ResourceLimitReached());
        }
        let encoded = unicode_wtf16ToWTF8Within(
            pair_slice,
            S_slice((scratch)$r(*written - 3, scratch.len))
        );
        *written = *written - 3 + encoded.len;
        *has_last = false;
        return_ok({});
    }
    let one = P_prefix$((S_const$u16)(&code_unit)(1));
    let required = unicode_wtf16ToWTF8Len(one);
    if (*written + required > scratch.len) {
        return_err(E_cause$proc_ResourceLimitReached());
    }
    let encoded = unicode_wtf16ToWTF8Within(
        one,
        S_slice((scratch)$r(*written, scratch.len))
    );
    *written += encoded.len;
    *last = code_unit;
    *has_last = true;
    return_ok({});
} $unscoped(fn);

$static fn_((proc_Args__emitWin32Backslashes(
    S$u8 scratch,
    P$usize written,
    usize count,
    bool capture,
    P$bool has_last,
    P$u16 last
))(proc_Args_E$void) $scope) {
    if (capture) {
        if (*written + count > scratch.len) {
            return_err(E_cause$proc_ResourceLimitReached());
        }
        for_(($r(0, count))(ignored)) {
            let_ignore = ignored;
            *S_at((scratch)[(*written)++]) = u8_c('\\');
        } $end(for);
    }
    if (count != 0) {
        *last = u16_c('\\');
        *has_last = true;
    }
    return_ok({});
} $unscoped(fn);

$static fn_((proc_Args__parseWin32(
    start_Invoc_Args_WTF16 wtf16,
    P$usize index,
    P$usize offset,
    S$u8 scratch,
    bool capture,
    P$usize written
))(proc_Args_E$bool) $scope) {
    claim_assert_nonnull(index);
    claim_assert_nonnull(offset);
    claim_assert_nonnull(written);
    let command_line = wtf16.cmd_line;
    var src = *offset;
    *written = 0;

    if (*index == 0) {
        if (command_line.len == 0) return_ok(false);
        var_(inside_quotes, bool) = false;
        var_(has_last, bool) = false;
        var_(last, u16) = 0;
        while (src < command_line.len) {
            let code_unit = *S_at((command_line)[src++]);
            if (code_unit == u16_c('"')) {
                inside_quotes = !inside_quotes;
                continue;
            }
            if (
                !inside_quotes
                && (code_unit == u16_c(' ') || code_unit == u16_c('\t'))
            ) {
                *offset = src;
                ++*index;
                return_ok(true);
            }
            if (capture) {
                try_(proc_Args__emitWin32CodeUnit(
                    scratch, written, code_unit, &has_last, &last
                ));
            }
        }
        *offset = src;
        ++*index;
        return_ok(true);
    }

    while (src < command_line.len) {
        let code_unit = *S_at((command_line)[src]);
        if (code_unit != u16_c(' ') && code_unit != u16_c('\t')) break;
        ++src;
    }
    if (src == command_line.len) {
        *offset = src;
        return_ok(false);
    }

    var_(inside_quotes, bool) = false;
    var_(backslashes, usize) = 0;
    var_(has_last, bool) = false;
    var_(last, u16) = 0;
    while (src < command_line.len) {
        let code_unit = *S_at((command_line)[src]);
        if (code_unit == u16_c('\\')) {
            ++backslashes;
            ++src;
            continue;
        }
        if (code_unit == u16_c('"')) {
            let escaped = (backslashes & 1) != 0;
            try_(proc_Args__emitWin32Backslashes(
                scratch, written, backslashes / 2, capture, &has_last, &last
            ));
            backslashes = 0;
            if (escaped) {
                if (capture) {
                    try_(proc_Args__emitWin32CodeUnit(
                        scratch, written, u16_c('"'), &has_last, &last
                    ));
                }
                ++src;
                continue;
            }
            if (
                inside_quotes
                && src + 1 < command_line.len
                && *S_at((command_line)[src + 1]) == u16_c('"')
            ) {
                if (capture) {
                    try_(proc_Args__emitWin32CodeUnit(
                        scratch, written, u16_c('"'), &has_last, &last
                    ));
                }
                src += 2;
                continue;
            }
            inside_quotes = !inside_quotes;
            ++src;
            continue;
        }
        try_(proc_Args__emitWin32Backslashes(
            scratch, written, backslashes, capture, &has_last, &last
        ));
        backslashes = 0;
        if (
            !inside_quotes
            && (code_unit == u16_c(' ') || code_unit == u16_c('\t'))
        ) {
            ++src;
            *offset = src;
            ++*index;
            return_ok(true);
        }
        if (capture) {
            try_(proc_Args__emitWin32CodeUnit(
                scratch, written, code_unit, &has_last, &last
            ));
        }
        ++src;
    }
    try_(proc_Args__emitWin32Backslashes(
        scratch, written, backslashes, capture, &has_last, &last
    ));
    *offset = src;
    ++*index;
    return_ok(true);
} $unscoped(fn);

$static fn_((start_Invoc_Args__next(
    P$raw ctx,
    usize* idx,
    usize* offset,
    S$u8 scratch
))(proc_Args_E$O$S_const$u8) $scope) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(idx);
    claim_assert_nonnull(offset);
    let self = ptrCast$((start_Invoc_Args*)(ctx));
    if (matches(*self, start_Invoc_Args_vec_z)) {
        let vec = union_as((self)(start_Invoc_Args_vec_z));
        if (*idx == vec->count) return_ok(none());
        let item_z = *P_at((vec->items)[(*idx)++]);
        return_ok(some(mem_spanZ0Bytes(item_z)));
    }
    let wtf16 = *union_as((self)(start_Invoc_Args_wtf16));
    var_(written, usize) = 0;
    if (!try_(proc_Args__parseWin32(
            wtf16, idx, offset, scratch, true, &written
        ))) return_ok(none());
    return_ok(some(P_prefix$((S_const$u8)(scratch.ptr)(written))));
} $unscoped(fn);

$static fn_((start_Invoc_Args__skip(
    P$raw ctx,
    usize* idx,
    usize* offset
))(proc_Args_E$bool) $scope) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(idx);
    claim_assert_nonnull(offset);
    let self = ptrCast$((start_Invoc_Args*)(ctx));
    if (matches(*self, start_Invoc_Args_vec_z)) {
        let vec = union_as((self)(start_Invoc_Args_vec_z));
        if (*idx == vec->count) return_ok(false);
        ++*idx;
        return_ok(true);
    }
    let wtf16 = *union_as((self)(start_Invoc_Args_wtf16));
    var_(written, usize) = 0;
    let empty = (S$u8){
        .ptr = as$(P$u8)(mem_emptyAddr(alignOf$(u8))),
        .len = 0,
    };
    return_(proc_Args__parseWin32(
        wtf16, idx, offset, empty, false, &written
    ));
} $unscoped(fn);
