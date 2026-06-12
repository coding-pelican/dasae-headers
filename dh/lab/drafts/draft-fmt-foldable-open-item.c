#define main_no_args pp_true
#include "dh-main.h"
#include <stdio.h>

typedef struct draft_open__Iter {
    var_(rest, S_const$u8);
} draft_open__Iter;

$attr($inline_always)
$static fn_((draft_open__render(S$u8 out, S_const$u8 fmt, S_const$u8 value))(S$u8));
$attr($inline_always)
$static fn_((draft_open__Iter_init(S_const$u8 fmt))(draft_open__Iter));
$attr($inline_always)
$static fn_((draft_open__countItems(S_const$u8 fmt))(usize));
$attr($inline_always)
$static fn_((draft_open__writeNext(draft_open__Iter* iter, S$u8 out, S_const$u8 value))(usize));

fn_((main(void))(E$void) $scope) {
    var_(mem, A$$(64, u8)) = A_zero();
    let out = draft_open__render(A_ref$((S$u8)mem), u8_l("--- {{ {*} {{def} } ---"), u8_l("ABC"));
    puts(as$(const char*)(out.ptr));
    return_ok({});
} $unscoped(fn);

fn_((draft_open__render(S$u8 out, S_const$u8 fmt, S_const$u8 value))(S$u8)) {
    let item_count = draft_open__countItems(fmt);
    claim_assert(item_count <= 8);
    var iter = draft_open__Iter_init(fmt);
    var rest_out = out;

    if (0 < item_count) rest_out = S_suffix((rest_out)draft_open__writeNext(&iter, rest_out, value));
    if (1 < item_count) rest_out = S_suffix((rest_out)draft_open__writeNext(&iter, rest_out, value));
    if (2 < item_count) rest_out = S_suffix((rest_out)draft_open__writeNext(&iter, rest_out, value));
    if (3 < item_count) rest_out = S_suffix((rest_out)draft_open__writeNext(&iter, rest_out, value));
    if (4 < item_count) rest_out = S_suffix((rest_out)draft_open__writeNext(&iter, rest_out, value));
    if (5 < item_count) rest_out = S_suffix((rest_out)draft_open__writeNext(&iter, rest_out, value));
    if (6 < item_count) rest_out = S_suffix((rest_out)draft_open__writeNext(&iter, rest_out, value));
    if (7 < item_count) rest_out = S_suffix((rest_out)draft_open__writeNext(&iter, rest_out, value));

    let_ignore = mem_copyBytes(rest_out, iter.rest);
    rest_out = S_suffix((rest_out)iter.rest.len);
    return S_prefix((out)(out.len - rest_out.len));
};

fn_((draft_open__Iter_init(S_const$u8 fmt))(draft_open__Iter)) {
    return (draft_open__Iter){ .rest = fmt };
};

fn_((draft_open__countItems(S_const$u8 fmt))(usize) $scope) {
    var count = usize_(0);
    var rest = fmt;
    while (0 < rest.len) {
        let open = orelse_((mem_findFirstUnitBytes(rest, u8_c('{')))(return count));
        count += 1;
        let after_open = open + 1;
        if (after_open < rest.len && *S_at((rest)[after_open]) == u8_c('{')) {
            rest = S_suffix((rest)(after_open + 1));
            continue;
        }
        let close = unwrap_(mem_findFirstUnitBytes(S_suffix((rest)after_open), u8_c('}')));
        rest = S_suffix((rest)(after_open + close + 1));
    }
    return count;
} $unscoped(fn);

fn_((draft_open__writeNext(draft_open__Iter* iter, S$u8 out, S_const$u8 value))(usize) $scope) {
    claim_assert_nonnull(iter);
    let open = unwrap_(mem_findFirstUnitBytes(iter->rest, u8_c('{')));
    let literal = S_prefix((iter->rest)open);
    let_ignore = mem_copyBytes(out, literal);
    var written = literal.len;

    let after_open = open + 1;
    if (after_open < iter->rest.len && *S_at((iter->rest)[after_open]) == u8_c('{')) {
        *S_at((out)[written++]) = u8_c('{');
        iter->rest = S_suffix((iter->rest)(after_open + 1));
        return written;
    }

    let close = unwrap_(mem_findFirstUnitBytes(S_suffix((iter->rest)after_open), u8_c('}')));
    let spec = S_prefix((S_suffix((iter->rest)after_open))close);
    claim_assert(spec.len == 1 && *S_at((spec)[0]) == u8_c('*'));
    let_ignore = mem_copyBytes(S_suffix((out)written), value);
    written += value.len;
    iter->rest = S_suffix((iter->rest)(after_open + close + 1));
    return written;
} $unscoped(fn);
