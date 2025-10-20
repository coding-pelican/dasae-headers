#include "dh/main.h"
#include "dh/Err.h"
#include "dh/math.h"
#include "dh/Str.h"
#include "dh/builtin/lambda.h"

/* example usage ============================================================*/

/* declarations */
use_ErrSet$(math_Err, i32);
$static $must_check
fn_((math_divideSafe(i32 lhs, i32 rhs))(math_Err$i32));

fn_((dh_main(Sli$Sli_const$u8 args))(Err$void) $scope) {
    let_ignore = args;
    debug_assert_true(0 < args.len);
    debug_assert_true(try_(math_divideSafe(10, 2)) == 5);
    printf("Hello World!\n");
    catch_((math_divideSafe(10, 0))(err, {
        printf("Occurs error\n");
        let err_code = Str_viewZ(as$(const u8*, Err_codeToCStr(err)));
        let_ignore   = err_code;
        debug_assert_true(Sli_const$u8_eq(err_code, u8_l("DivisionByZero")));
        Err_print(err);
        ErrTrace_print();
    }));
    printf("Error handled!\n");
    let testLambda = lam_((i32 lhs, i32 rhs), i32) { return lhs + rhs; };
    printf("testLambda: %d\n", testLambda(10, 5));
    return_(ok({}));
} $unscoped_(fn);

/* implementations */
fn_((math_divideSafe(i32 lhs, i32 rhs))(math_Err$i32) $scope) {
    if (rhs == 0) {
        return_(err(math_Err_DivisionByZero()));
    }
    return_ok(lhs / rhs);
} $unscoped_(fn);
