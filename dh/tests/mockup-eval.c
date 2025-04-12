#define main_no_args (1)
#include "dh/main.h"

#undef eval_return_
#undef comp_syn__eval_return_

#define eval$(T_Return, _Expr...) \
    comp_syn__eval$(T_Return, _Expr)
#define eval_return_(_Expr...) \
    comp_syn__eval_return_(_Expr)
#define asg_eval(var_addr, _Expr...) \
    comp_syn__asg_eval(var_addr, _Expr)

#define comp_syn__eval$(T_Return, _Expr...) ({ \
    T_Return* __ret = null;                    \
    struct {                                   \
        i32 curr;                              \
    } __eval_state = { .curr = __LINE__ };     \
    switch (__eval_state.curr) {               \
    case __LINE__: {                           \
        _Expr;                                 \
        __eval_state.curr = __LINE__ - 1;      \
    } break;                                   \
    case __LINE__ - 1:                         \
        fallthrough;                           \
    default:                                   \
        break;                                 \
    }                                          \
    *__ret;                                    \
})
#define comp_syn__eval_return_(_Expr...)   \
    __ret             = createFrom(_Expr); \
    __eval_state.curr = -1;                \
    break;
#define comp_syn__asg_eval(var_addr, _Expr...) \
    *var_addr = eval$(TypeOf(*var_addr), _Expr);

fn_scope(dh_main(void), Err$void) {
    let scope_1 = eval$(i32,
        let a = 1;
        let b = 2;
        let c = a + b;
        if (0 < c) {
            eval_return_(c);
        }
        eval_return_(-1);
    );
    printf("%d\n", scope_1);

    let scope_2 = eval$(i32,
        let a = 1;
        let b = 2;
        let c = a + b;
        if (0 < c) {
            eval_return_(-1);
        }
        eval_return_(c);
    );
    printf("%d\n", scope_2);

    i32 scope_3 = 0;
    asg_eval(&scope_3,
        let a = 1;
        let b = 2;
        let c = a + b;
        if (0 < c) {
            eval_return_(-1);
        }
        eval_return_(c);
    );
    printf("%d\n", scope_3);

    return_ok({});
} unscoped;
