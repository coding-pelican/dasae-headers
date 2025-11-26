#include "dh/main.h"
#include "dh/builtin/lambda.h"
#include "dh/io/stream.h"
#include "dh/mem/common.h"
#include "dh/math.h"

/* example usage ============================================================*/

/* declarations */
T_use_E$($set(math_Err)(i32));
$attr($must_check)
$static fn_((math_divideSafe(i32 lhs, i32 rhs))(math_Err$i32));

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    debug_assert_true(0 < args.len);
    debug_assert_true(try_(math_divideSafe(10, 2)) == 5);
    io_stream_print(u8_l("Hello World!\n"));
    catch_((math_divideSafe(10, 0))(err, {
        io_stream_println(u8_l("Occurs error"));
        let err_code = mem_spanZ0_const$u8(Err_codeToStrZ(err));
        debug_assert_true(mem_eqBytes(err_code, u8_l("DivisionByZero")));
        let_ignore   = err_code;
        io_stream_eprintln(u8_l("Error: {:e}"), err);
        ErrTrace_print();
    }));
    io_stream_println(u8_l("Error handled!\n"));
    let testLambda = la_((i32 lhs, i32 rhs)(i32)) { return lhs + rhs; };
    io_stream_println(u8_l("testLambda: {:d}"), testLambda(10, 5));
    return_(ok({}));
} $unscoped_(fn);

/* implementations */
fn_((math_divideSafe(i32 lhs, i32 rhs))(math_Err$i32) $scope) {
    if (rhs == 0) {
        return_(err(math_Err_DivisionByZero()));
    }
    return_ok(lhs / rhs);
} $unscoped_(fn);
