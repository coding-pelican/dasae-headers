#include "dh/main.h"
#include "dh/core.h"
#include "dh/Err.h"
#include "dh/math.h"
#include "dh/fn.h"
#include "dh/Str.h"
#include "dh/builtin/lambda.h"

/* example usage ============================================================*/

/* declarations */
use_ErrSet$(math_Err, i32);
static fn_(math_divideSafe(i32 lhs, i32 rhs), $must_check math_Err$i32);

/* implementations */
fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    $ignore args;
    debug_assert_true(0 < args.len);
    debug_assert_true(try_(math_divideSafe(10, 2)) == 5);
    printf("Hello World!\n");
    catch_from(math_divideSafe(10, 0), err, ({
        printf("Occurs error\n");
        let err_code = Str_viewZ(as$(const u8*, Err_codeToCStr(err)));
        $ignore err_code;
        debug_assert_true(Str_const_eq(err_code, Str_l("DivisionByZero")));
        Err_print(err);
        ErrTrace_print();
    }));
    printf("Error handled!\n");
    let testLambda = lam_((i32 lhs, i32 rhs), i32) { return lhs + rhs; };
    printf("testLambda: %d\n", testLambda(10, 5));
    return_(ok({}));
} ext_unscoped;

fn_ext_scope(math_divideSafe(i32 lhs, i32 rhs), math_Err$i32) {
    if (rhs == 0) {
        return_(err(math_Err_DivisionByZero()));
    }
    return_ok(lhs / rhs);
} ext_unscoped;
