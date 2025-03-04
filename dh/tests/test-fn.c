#include "dh/core.h"
#include "dh/math.h"
#include "dh/main.h"
#include "dh/Err.h"
#include "dh/debug/assert.h"
#include "dh/Str.h"

/* SYN macros declaration ===================================================*/

#define pp_nothing(...) IMPL__pp_nothing(__VA_ARGS__)

#define pub                                            SYN__pub
#define pvt                                            SYN__pvt
#define fn_(_Name_With_Params, T_Return, _Body...)     SYN__fn_(_Name_With_Params, T_Return, pp_nothing("function body") _Body)
#define fn_decl(_Name_With_Params, T_Return)           SYN__fn_GEN_declaration(_Name_With_Params, T_Return)
#define fn_impl(_Name_With_Params, T_Return, _Body...) SYN__fn_GEN_definition(_Name_With_Params, T_Return, _Body)

/* SYN macros implementation ================================================*/

#define IMPL__pp_nothing(...)

#define SYN__pub                                                  extern
#define SYN__pvt                                                  static
#define SYN__fn_(_Name_With_Params, T_Return, ...)                T_Return _Name_With_Params __VA_ARGS__
#define SYN__fn_GEN_declaration(_Name_With_Params, T_Return, ...) T_Return _Name_With_Params
#define SYN__fn_GEN_definition(_Name_With_Params, T_Return, ...) \
    T_Return _Name_With_Params {                                 \
        scope_reserveReturn(T_Return){                           \
            __VA_ARGS__                                          \
        } scope_returnReserved;                                  \
    }

/* example usage ============================================================*/

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
