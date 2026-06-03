#include "dh-main.h"
#include "dh/meta.h"
#include <stdio.h>

$attr($inline_always)
$static fn_((my_formatTuple(S$u8 mem, S_const$u8 fmt, u_V$raw arg_tuple))(E$S$u8));

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    var_(mem, A$$(64, u8)) = A_zero();
    let str = try_(my_formatTuple(A_ref$((S$u8)mem), u8_l("Hello, world! {i}"), u_anyV($tup((123)))));
    printf("%s\n", as$(const u8*)(str.ptr));

    return_ok({});
} $unscoped(fn);

fn_((my_formatTuple(S$u8 mem, S_const$u8 fmt, u_V$raw arg_tuple))(E$S$u8) $scope) {
    let tuple = u_load(arg_tuple);
    let first_brace = unwrap_(mem_findFirstUnitBytes(fmt, u8_c('{')));
    let last_brace = unwrap_(mem_findFirstUnitBytes(fmt, u8_c('}')));
    let prefix = S_prefix((fmt)first_brace);
    let remaining_mem = S_suffix((mem)prefix.len);

    let_ignore = mem_copyBytes(mem, prefix);

    let brace_inner = S_slice((fmt)$r(first_brace + 1, last_brace));
    if (mem_eqlBytes(brace_inner, u8_l("i"))) {
        let ty_fields = typeInfos$(i32);
        claim_assert(TypeInfo_eql(tuple.type, u_typeInfoRecord(ty_fields)));

        let field = u_fieldPtr(tuple.ref.as_const, ty_fields, 0);
        let arg = *u_castP$((const i32*)(field));
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

        let appended = A_suffix$((S_const$u8)(tmp)(pos));
        let_ignore = mem_copyBytes(remaining_mem, appended);
        return_ok(S_prefix((mem)(prefix.len + appended.len)));
    } else if (mem_eqlBytes(brace_inner, u8_l("u"))) {
        let ty_fields = typeInfos$(u32);
        claim_assert(TypeInfo_eql(tuple.type, u_typeInfoRecord(ty_fields)));

        let field = u_fieldPtr(tuple.ref.as_const, ty_fields, 0);
        let arg = *u_castP$((const u32*)(field));

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

        let appended = A_suffix$((S_const$u8)(tmp)(pos));
        let_ignore = mem_copyBytes(remaining_mem, appended);
        return_ok(S_prefix((mem)(prefix.len + appended.len)));
    }
    return_ok(S_prefix((mem)prefix.len));
} $unscoped(fn);
