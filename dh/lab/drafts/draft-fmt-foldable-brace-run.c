#define main_no_args pp_true
#include "dh-main.h"
#include <stdio.h>

$attr($inline_always)
$static fn_((draft_brace__render(S$u8 out, S_const$u8 fmt, S_const$u8 value))(S$u8));
$attr($inline_always)
$static fn_((draft_brace__writeOpenRun(S$u8 out, usize count))(usize));

fn_((main(void))(E$void) $scope) {
    var_(mem, A$$(64, u8)) = A_zero();
    let out = draft_brace__render(A_ref$((S$u8)mem), u8_l("--- {{{*} {{{{{*} {{{{ ---"), u8_l("ABC"));
    puts(as$(const char*)(out.ptr));
    return_ok({});
} $unscoped(fn);

fn_((draft_brace__render(S$u8 out, S_const$u8 fmt, S_const$u8 value))(S$u8)) {
    var pos = usize_(0);
    var written = usize_(0);

    while (pos < fmt.len) {
        let ch = *S_at((fmt)[pos]);
        if (ch != u8_c('{')) {
            *S_at((out)[written++]) = ch;
            pos += 1;
            continue;
        }

        var run_len = usize_(1);
        while (pos + run_len < fmt.len && *S_at((fmt)[pos + run_len]) == u8_c('{')) {
            run_len += 1;
        }

        written += draft_brace__writeOpenRun(S_suffix((out)written), run_len / 2);
        pos += run_len / 2 * 2;
        if ((run_len % 2) == 0) continue;

        claim_assert(pos < fmt.len && *S_at((fmt)[pos]) == u8_c('{'));
        claim_assert(pos + 2 < fmt.len);
        claim_assert(*S_at((fmt)[pos + 1]) == u8_c('*'));
        claim_assert(*S_at((fmt)[pos + 2]) == u8_c('}'));
        let_ignore = mem_copyBytes(S_suffix((out)written), value);
        written += value.len;
        pos += 3;
    }

    return S_prefix((out)written);
};

fn_((draft_brace__writeOpenRun(S$u8 out, usize count))(usize)) {
    for_(($r(0, count))(i)) {
        *S_at((out)[i]) = u8_c('{');
    } $end(for);
    return count;
};
