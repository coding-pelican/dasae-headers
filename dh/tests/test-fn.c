#include "dh/core.h"
#include "dh/math.h"
#include "dh/main.h"
#include "dh/Err.h"
#include "dh/debug/assert.h"
#include "dh/Str.h"
#include "dh/fn.h"

/*========== Macros and Declarations ========================================*/

#define pp_nothing(...) IMPL__pp_nothing(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define IMPL__pp_nothing(...)

/* example usage ============================================================*/

/* declarations */
use_ErrSet$(math_Err, i32);
pub fn_(math_divideSafe(i32 lhs, i32 rhs), must_check math_Err$i32);

/* implementations */
fn_ext_scope(math_divideSafe(i32 lhs, i32 rhs), math_Err$i32) {
    if (rhs == 0) {
        return_(err(math_Err_DivisionByZero()));
    }
    return_ok(lhs / rhs);
}
ext_unscoped;

fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    debug_assert_true(0 < args.len);
    debug_assert_true(try_(math_divideSafe(10, 2)) == 5);
    printf("Hello World!\n");
    catch_from(math_divideSafe(10, 0), err, {
        printf("Occurs error\n");
        let err_code = Str_viewZ(as$(const u8*, Err_codeToCStr(err)));
        debug_assert_true(Str_const_eq(err_code, Str_l("DivisionByZero")));
        Err_print(err);
        ErrTrace_print();
        // return_err(err);
    });
    printf("Error handled!\n");
    return_(ok({}));
}
ext_unscoped;
