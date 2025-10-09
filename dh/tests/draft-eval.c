#define main_no_args (1)
#include "dh/main.h"


#undef eval_return_
#undef comp_syn__eval_return_

// // make early returns
// let scope_1 = eval_(i32,
//     let a = 1;
//     let b = 2;
//     let c = a + b;
//     if (0 < c) {
//         eval_return_(c);
//     }
//     -1
// );

#undef eval
#define eval(...) ({ __VA_ARGS__; })

#define eval_(T_Return, _Expr...) \
    comp_syn__eval_(T_Return, _Expr)
#define eval_return_(_Expr...) \
    comp_syn__eval_return_(_Expr)
#define asg_eval(var_addr, _Expr...) \
    comp_syn__asg_eval(var_addr, _Expr)

#define comp_syn__eval_(T_Return, _Expr...) ({ \
    T_Return* __ret = null; \
    struct { \
        i32 curr; \
    } __eval_state = { .curr = __LINE__ }; \
    switch (__eval_state.curr) { \
    case __LINE__: { \
        _Expr; \
        __eval_state.curr = __LINE__ - 1; \
    } break; \
    case __LINE__ - 1: \
        $fallthrough; \
    default: \
        break; \
    } \
    *__ret; \
})
#define comp_syn__eval_return_(_Expr...) \
    __ret             = createFrom(_Expr); \
    __eval_state.curr = -1; \
    break;
#define comp_syn__asg_eval(var_addr, _Expr...) \
    *var_addr = eval_(TypeOf(*var_addr), _Expr);

fn_(dh_main(void), Err$void $scope) {
    let scope_1 = eval_(i32, let a = 1; let b = 2; let c = a + b; if (0 < c) {
        eval_return_(c);
    } eval_return_(-1););
    printf("%d\n", scope_1);

    let scope_2 = eval_(i32, let a = 1; let b = 2; let c = a + b; if (0 < c) {
        eval_return_(-1);
    } eval_return_(c););
    printf("%d\n", scope_2);

    i32 scope_3 = 0;
    asg_eval(&scope_3,
             let a = 1;
             let b = 2;
             let c = a + b;
             if (0 < c) {
                 eval_return_(-1);
             } eval_return_(c););
    printf("%d\n", scope_3);

    // let a = 0 < 1 ? ({ 1; }) : ({ claim_unreachable; });
    let a = eval(
        i32 _return_val;
        var _then = _Generic(
            TypeOf(eval(0)),
            void: eval(
                $ignore_void eval(0);
                (TypeOf(_return_val)){}
            ),
            default: eval(0)
        );
        var _else = _Generic(
            TypeOf(eval(claim_unreachable)),
            void: eval(
                $ignore_void eval(claim_unreachable);
                (TypeOf(_return_val)){}
            ),
            default: eval(claim_unreachable)
        );
        0 < 1 ? _then : _else;
    );
    printf("%d\n", a);

#define match$(T, _Expr...) _Generic(T, _Expr)
#define case$(T, _Expr...) \
T: \
    _Expr
#define otherwise(_Expr...) \
    default: \
        _Expr

    $ignore = eval(
        i32 _return_val;
        var _then = match$(TypeOf(eval(0)), case$(void, eval($ignore_void eval(0); eval_return make$(TypeOf(_return_val)))), otherwise(eval(0)));
        var _else = match$(TypeOf(eval(claim_unreachable)), case$(void, eval($ignore_void eval(claim_unreachable); eval_return make$(TypeOf(_return_val)))), otherwise(eval(claim_unreachable)));
        0 < 1 ? _then : _else;
    );
    printf("%d\n", a);

#define eval_then                                      ,
#define eval_else                                      ,
#define eval_if$(T, cond, ...)                         comp_syn__eval_if$(T, cond, __VA_ARGS__)
#define comp_syn__eval_if$(T, cond, _Then, _Else, ...) ({ \
    cond ? match$(TypeOf(_Then), case$(void, eval($ignore_void _Then; eval_return make$(T))), otherwise(_Then)) : match$(TypeOf(_Else), case$(void, eval($ignore_void _Else; eval_return make$(T))), otherwise(_Else)); \
})

    let c = eval_if$(i32, 0 < 1 eval_then 1 eval_else eval_if$(i32, 0 < 2 eval_then 2 eval_else 3));
    printf("%d\n", c);

    return_ok({});
} $unscoped;
