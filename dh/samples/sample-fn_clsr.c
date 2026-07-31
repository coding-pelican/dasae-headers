#include "dh-main.h"
#include "dh/clsr.h"
#include "dh/io/stream.h"

errset_((my_math_E)(
    my_math_DivByZero
));

T_use_E$($set(my_math_E)(i32));
T_use$((i32)(Clsr_Ctx, Clsr_Rtn, Clsr));

$attr($must_check)
$static fn_((my_math_divSafe(i32 lhs, i32 rhs))(my_math_E$i32) $scope) {
    if (rhs == 0) return_err(E_cause$my_math_DivByZero());
    return_ok(lhs / rhs);
} $unscoped(fn);

$static fn_((my_math_add(i32 lhs, i32 rhs))(i32)) { return lhs + rhs; };
fn_use_Clsr_((my_math_add)(i32, i32)(i32));

$static fn_((my_math_mul(i32 lhs, i32 rhs))(i32)) { return lhs * rhs; };
fn_use_Clsr_((my_math_mul)(i32, i32)(i32));

fn_((main(proc_Entry entry))(E$void) $scope) {
    let_ignore = entry;

    let quotient = try_(my_math_divSafe(10, 2));
    claim_assert_true(quotient == 5);
    io_stream_println(u8_l("10 / 2 = {:d}"), quotient);

    catch_((my_math_divSafe(10, 0))(err, {
        io_stream_eprintln(u8_l("my_math_divSafe failed: {:e}"), err);
    }));

    var add_work = clsr_((my_math_add)(10, 5));
    let add_result = add_work.rtn(&add_work.ctx)->ret;
    claim_assert_true(add_result == 15);
    io_stream_println(u8_l("my_math_add closure: {:d}"), add_result);

    var mul_work = clsr_((my_math_mul)(10, 5));
    let mul_result = mul_work.rtn(&mul_work.ctx)->ret;
    claim_assert_true(mul_result == 50);
    io_stream_println(u8_l("my_math_mul closure: {:d}"), mul_result);

    return_ok({});
} $unscoped(fn);
