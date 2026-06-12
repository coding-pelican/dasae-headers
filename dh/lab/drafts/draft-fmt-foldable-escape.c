#define main_no_args pp_true
#include "dh-main.h"
#include <stdio.h>

#if !defined(DRAFT_ESCAPE_CASE)
#define DRAFT_ESCAPE_CASE 0
#endif

typedef struct draft_escape__Iter {
    var_(rest, S_const$u8);
} draft_escape__Iter;

$attr($inline_always $flatten)
$static fn_((draft_escape__format(S$u8 out, S_const$u8 fmt, S_const$u8 value))(S$u8));
$attr($inline_always)
$static fn_((draft_escape__formatPlain(S$u8 out, S_const$u8 fmt, S_const$u8 value))(S$u8));
$attr($inline_always)
$static fn_((draft_escape__formatEscaped(S$u8 out, S_const$u8 fmt, S_const$u8 value))(S$u8));
$attr($inline_always)
$static fn_((draft_escape__needsEscapedCore(S_const$u8 fmt))(bool));
$attr($inline_always)
$static fn_((draft_escape__Iter_init(S_const$u8 fmt))(draft_escape__Iter));
$attr($inline_always)
$static fn_((draft_escape__countPlainItems(S_const$u8 fmt))(usize));
$attr($inline_always)
$static fn_((draft_escape__writePlainNext(draft_escape__Iter* iter, S$u8 out, S_const$u8 value))(usize));
$attr($inline_always)
$static fn_((draft_escape__writeEscapedRun(S$u8 out, S_const$u8 value, S_const$u8 fmt, usize* pos))(usize));
$attr($inline_always)
$static fn_((draft_escape__writeRepeated(S$u8 out, u8 byte, usize count))(usize));
$attr($inline_always)
$static fn_((draft_escape__writePlaceholder(S$u8 out, S_const$u8 spec, S_const$u8 value))(usize));

fn_((main(void))(E$void) $scope) {
    var_(mem, A$$(96, u8)) = A_zero();

#if DRAFT_ESCAPE_CASE == 0
    let out = draft_escape__format(A_ref$((S$u8)mem), u8_l("case0: {*} ok"), u8_l("ABC"));
#elif DRAFT_ESCAPE_CASE == 1
    let out = draft_escape__format(A_ref$((S$u8)mem), u8_l("case1: {{ {*} {{def}} }}"), u8_l("ABC"));
#elif DRAFT_ESCAPE_CASE == 2
    let out = draft_escape__format(A_ref$((S$u8)mem), u8_l("case2: {{{*} {{{{{*} {{{{"), u8_l("ABC"));
#else
#error "unsupported DRAFT_ESCAPE_CASE"
#endif

    puts(as$(const char*)(out.ptr));
    return_ok({});
} $unscoped(fn);

fn_((draft_escape__format(S$u8 out, S_const$u8 fmt, S_const$u8 value))(S$u8)) {
    return draft_escape__needsEscapedCore(fmt) ? draft_escape__formatEscaped(out, fmt, value)
                                               : draft_escape__formatPlain(out, fmt, value);
};

fn_((draft_escape__formatPlain(S$u8 out, S_const$u8 fmt, S_const$u8 value))(S$u8)) {
    let item_count = draft_escape__countPlainItems(fmt);
    claim_assert(item_count <= 8);
    var iter = draft_escape__Iter_init(fmt);
    var rest_out = out;

    if (0 < item_count) rest_out = S_suffix((rest_out)draft_escape__writePlainNext(&iter, rest_out, value));
    if (1 < item_count) rest_out = S_suffix((rest_out)draft_escape__writePlainNext(&iter, rest_out, value));
    if (2 < item_count) rest_out = S_suffix((rest_out)draft_escape__writePlainNext(&iter, rest_out, value));
    if (3 < item_count) rest_out = S_suffix((rest_out)draft_escape__writePlainNext(&iter, rest_out, value));
    if (4 < item_count) rest_out = S_suffix((rest_out)draft_escape__writePlainNext(&iter, rest_out, value));
    if (5 < item_count) rest_out = S_suffix((rest_out)draft_escape__writePlainNext(&iter, rest_out, value));
    if (6 < item_count) rest_out = S_suffix((rest_out)draft_escape__writePlainNext(&iter, rest_out, value));
    if (7 < item_count) rest_out = S_suffix((rest_out)draft_escape__writePlainNext(&iter, rest_out, value));

    let_ignore = mem_copyBytes(rest_out, iter.rest);
    rest_out = S_suffix((rest_out)iter.rest.len);
    return S_prefix((out)(out.len - rest_out.len));
};

fn_((draft_escape__formatEscaped(S$u8 out, S_const$u8 fmt, S_const$u8 value))(S$u8)) {
    var pos = usize_(0);
    var written = usize_(0);

    while (pos < fmt.len) {
        let ch = *S_at((fmt)[pos]);
        if (ch != u8_c('{') && ch != u8_c('}')) {
            *S_at((out)[written++]) = ch;
            pos += 1;
            continue;
        }
        written += draft_escape__writeEscapedRun(S_suffix((out)written), value, fmt, &pos);
    }

    return S_prefix((out)written);
};

fn_((draft_escape__needsEscapedCore(S_const$u8 fmt))(bool) $scope) {
    var pos = usize_(0);
    while (pos < fmt.len) {
        let ch = *S_at((fmt)[pos]);
        if (ch == u8_c('}')) return true;
        if (ch != u8_c('{')) {
            pos += 1;
            continue;
        }

        var run_len = usize_(1);
        while (pos + run_len < fmt.len && *S_at((fmt)[pos + run_len]) == u8_c('{')) {
            run_len += 1;
        }
        if (run_len != 1) return true;

        let spec_start = pos + 1;
        let close = unwrap_(mem_findFirstUnitBytes(S_suffix((fmt)spec_start), u8_c('}')));
        pos = spec_start + close + 1;
    }
    return false;
} $unscoped(fn);

fn_((draft_escape__Iter_init(S_const$u8 fmt))(draft_escape__Iter)) {
    return (draft_escape__Iter){ .rest = fmt };
};

fn_((draft_escape__countPlainItems(S_const$u8 fmt))(usize)) {
    var count = usize_(0);
    for_(($s(fmt))(ch)) {
        if (*ch == u8_c('{')) count += 1;
    } $end(for);
    return count;
};

fn_((draft_escape__writePlainNext(draft_escape__Iter* iter, S$u8 out, S_const$u8 value))(usize) $scope) {
    claim_assert_nonnull(iter);
    let open = unwrap_(mem_findFirstUnitBytes(iter->rest, u8_c('{')));
    let literal = S_prefix((iter->rest)open);
    let_ignore = mem_copyBytes(out, literal);
    var written = literal.len;

    let spec_start = open + 1;
    let close = unwrap_(mem_findFirstUnitBytes(S_suffix((iter->rest)spec_start), u8_c('}')));
    let spec = S_prefix((S_suffix((iter->rest)spec_start))close);
    written += draft_escape__writePlaceholder(S_suffix((out)written), spec, value);
    iter->rest = S_suffix((iter->rest)(spec_start + close + 1));
    return written;
} $unscoped(fn);

fn_((draft_escape__writeEscapedRun(S$u8 out, S_const$u8 value, S_const$u8 fmt, usize* pos))(usize) $scope) {
    claim_assert_nonnull(pos);
    let start = *pos;
    let ch = *S_at((fmt)[start]);
    var run_len = usize_(1);
    while (start + run_len < fmt.len && *S_at((fmt)[start + run_len]) == ch) {
        run_len += 1;
    }

    var written = draft_escape__writeRepeated(out, ch, run_len / 2);
    *pos += run_len / 2 * 2;

    if ((run_len % 2) == 0) return written;
    claim_assert(ch == u8_c('{'));
    claim_assert(*pos < fmt.len && *S_at((fmt)[*pos]) == u8_c('{'));

    let spec_start = *pos + 1;
    let close = unwrap_(mem_findFirstUnitBytes(S_suffix((fmt)spec_start), u8_c('}')));
    let spec = S_prefix((S_suffix((fmt)spec_start))close);
    written += draft_escape__writePlaceholder(S_suffix((out)written), spec, value);
    *pos = spec_start + close + 1;
    return written;
} $unscoped(fn);

fn_((draft_escape__writeRepeated(S$u8 out, u8 byte, usize count))(usize)) {
    for_(($r(0, count))(i)) {
        *S_at((out)[i]) = byte;
    } $end(for);
    return count;
};

fn_((draft_escape__writePlaceholder(S$u8 out, S_const$u8 spec, S_const$u8 value))(usize)) {
    claim_assert(spec.len == 1);
    claim_assert(*S_at((spec)[0]) == u8_c('*'));
    let_ignore = mem_copyBytes(out, value);
    return value.len;
};
