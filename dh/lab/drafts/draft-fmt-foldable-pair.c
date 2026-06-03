#include "dh-main.h"
#include "dh/meta.h"
#include <stdio.h>

$attr($inline_always)
$static fn_((my_formatTuple(S$u8 mem, S_const$u8 fmt, u_V$raw arg_tuple))(E$S$u8));

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    var_(mem, A$$(64, u8)) = A_zero();
    let x = $tup((u8_l("world")), (123));
    let str = try_(my_formatTuple(A_ref$((S$u8)mem), u8_l("Hello, {s}! {i}"), u_anyV(x)));
    printf("%s\n", as$(const u8*)(str.ptr));

    return_ok({});
} $unscoped(fn);

fn_((my_formatTuple(S$u8 mem, S_const$u8 fmt, u_V$raw arg_tuple))(E$S$u8) $scope) {
    let tuple = u_load(arg_tuple);
    let ty_fields = typeInfos$(S_const$u8, i32);
    claim_assert(TypeInfo_eql(tuple.type, u_typeInfoRecord(ty_fields)));

    let first_brace = unwrap_(mem_findFirstUnitBytes(fmt, u8_c('{')));
    let first_end = unwrap_(mem_findFirstUnitBytes(fmt, u8_c('}')));
    let first_spec = S_slice((fmt)$r(first_brace + 1, first_end));
    claim_assert(mem_eqlBytes(first_spec, u8_l("s")));

    var written = S_prefix((fmt)first_brace).len;
    let_ignore = mem_copyBytes(mem, S_prefix((fmt)first_brace));

    let field0 = u_fieldPtr(tuple.ref.as_const, ty_fields, 0);
    let text = *u_castP$((const S_const$u8*)(field0));
    let_ignore = mem_copyBytes(S_suffix((mem)written), text);
    written += text.len;

    let rest = S_suffix((fmt)(first_end + 1));
    let second_brace = unwrap_(mem_findFirstUnitBytes(rest, u8_c('{')));
    let second_end = unwrap_(mem_findFirstUnitBytes(rest, u8_c('}')));
    let second_spec = S_slice((rest)$r(second_brace + 1, second_end));
    claim_assert(mem_eqlBytes(second_spec, u8_l("i")));

    let infix = S_prefix((rest)second_brace);
    let_ignore = mem_copyBytes(S_suffix((mem)written), infix);
    written += infix.len;

    let field1 = u_fieldPtr(tuple.ref.as_const, ty_fields, 1);
    let arg = *u_castP$((const i32*)(field1));
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
    let_ignore = mem_copyBytes(S_suffix((mem)written), digits);
    written += digits.len;

    let suffix = S_suffix((rest)(second_end + 1));
    let_ignore = mem_copyBytes(S_suffix((mem)written), suffix);
    written += suffix.len;

    return_ok(S_prefix((mem)written));
} $unscoped(fn);
