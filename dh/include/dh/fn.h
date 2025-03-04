/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    fn.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-04 (date of creation)
 * @updated 2025-03-04 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef FN_INCLUDED
#define FN_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/scope.h"

/*========== Macros and Definitions =========================================*/

#define pub                                            SYN__pub
#define pvt                                            SYN__pvt
#define fn_decl(_Name_With_Params, T_Return)           SYN__fn_decl(_Name_With_Params, T_Return)
#define fn_impl(_Name_With_Params, T_Return, _Body...) SYN__fn_impl(_Name_With_Params, T_Return, _Body)

/*========== Macros and Implementations =====================================*/

#define SYN__pub                                       extern
#define SYN__pvt                                       static
#define SYN__fn_decl(_Name_With_Params, T_Return, ...) T_Return _Name_With_Params
#define SYN__fn_impl(_Name_With_Params, T_Return, ...) \
    T_Return _Name_With_Params {                       \
        scope_reserveReturn(T_Return){                 \
            __VA_ARGS__                                \
        } scope_returnReserved;                        \
    }

/*========== Example usage ==================================================*/

#if EXAMPLE_USAGE
#include "dh/main.h"
#include "dh/core.h"
#include "dh/math.h"
#include "dh/Str.h"

/* declarations */
use_ErrSet$(math_Err, i32);
pub fn_decl(math_divideSafe(i32 lhs, i32 rhs), math_Err$i32) must_check;

/* implementations */
pub fn_impl(math_divideSafe(i32 lhs, i32 rhs), math_Err$i32, {
    if (rhs == 0) {
        return_(err(math_Err_DivisionByZero()));
    }
    return_ok(lhs / rhs);
});

/* main */
pub fn_impl(dh_main(Sli$Str_const args), Err$void, {
    debug_assert_true(0 < args.len);
    debug_assert_true(try(math_divideSafe(10, 2)) == 5);
    catch (math_divideSafe(10, 0), err, {
        let err_code = Str_viewZ(as$(const u8*, Err_codeToCStr(err)));
        debug_assert_true(Str_const_eq(err_code, Str_l("DivisionByZero")));
        return_err(err);
    });
    return_(ok({}));
})
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* FN_INCLUDED */
