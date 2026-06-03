#include "dh-main.h"
#include <stdio.h>

$attr($inline_always)
$static fn_((my_formatVaArgs(S$u8 mem, S_const$u8 fmt, ...))(E$S$u8));

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    var_(mem, A$$(64, u8)) = A_zero();
    let str = try_(my_formatVaArgs(A_ref$((S$u8)mem), u8_l("Hello, world! {i}"), 123));
    printf("%s\n", as$(const u8*)(str.ptr));

    return_ok({});
} $unscoped(fn);

fn_((my_formatVaArgs(S$u8 mem, S_const$u8 fmt, ...))(E$S$u8) $guard) {
    var_(va_args, va_list) $undefined;
    va_start(va_args, fmt);
    defer_(va_end(va_args));

    let first_brace = unwrap_(mem_findFirstUnitBytes(fmt, u8_c('{')));
    let last_brace = unwrap_(mem_findFirstUnitBytes(fmt, u8_c('}')));
    let remaining_mem = S_suffix((mem)mem_copyBytes(mem, S_prefix((fmt)first_brace)).len);

    let brace_inner = S_slice((fmt)$r(first_brace + 1, last_brace));
    if (mem_eqlBytes(brace_inner, u8_l("i"))) {
        let arg = va_arg(va_args, i32);
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

        let appended = mem_copyBytes(remaining_mem, A_suffix$((S_const$u8)(tmp)(pos)));
        return_ok(S_prefix((mem)(appended.len)));
    } else if (mem_eqlBytes(brace_inner, u8_l("u"))) {
        let arg = va_arg(va_args, u32);

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

        let appended = mem_copyBytes(remaining_mem, A_suffix$((S_const$u8)(tmp)(pos)));
        return_ok(S_prefix((mem)(appended.len)));
    }
    return_ok(S_prefix((mem)(remaining_mem.len)));
} $unguarded(fn);
