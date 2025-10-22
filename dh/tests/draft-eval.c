#if UNUSED_CODE
#define main_no_args (1)
#include "dh/main.h"


#undef blk_return_
#undef comp_syn__expr_return_

// // make early returns
// let scope_1 = expr_(i32,
//     let a = 1;
//     let b = 2;
//     let c = a + b;
//     if (0 < c) {
//         blk_return_(c);
//     }
//     -1
// );

#undef eval
#define blk(...) ({ __VA_ARGS__; })

#define expr_(T_Return, _Expr...) \
    comp_syn__expr_(T_Return, _Expr)
#define blk_return_(_Expr...) \
    comp_syn__expr_return_(_Expr)
#define asg_blk(var_addr, _Expr...) \
    comp_syn__asg_blk(var_addr, _Expr)

#define comp_syn__expr_(T_Return, _Expr...) ({ \
    T_Return* __ret = null; \
    struct { \
        i32 curr; \
    } __expr_state = { .curr = __LINE__ }; \
    switch (__expr_state.curr) { \
    case __LINE__: { \
        _Expr; \
        __expr_state.curr = __LINE__ - 1; \
    } break; \
    case __LINE__ - 1: \
        $fallthrough; \
    default: \
        break; \
    } \
    *__ret; \
})
#define comp_syn__expr_return_(_Expr...) \
    __ret             = createFrom(_Expr); \
    __expr_state.curr = -1; \
    break;
#define comp_syn__asg_blk(var_addr, _Expr...) \
    *var_addr = expr_(TypeOf(*var_addr), _Expr);

fn_((dh_main(void))(Err$void) $scope) {
    let scope_1 = expr_(i32, let a = 1; let b = 2; let c = a + b; if (0 < c) {
        blk_return_(c);
    } blk_return_(-1););
    printf("%d\n", scope_1);

    let scope_2 = expr_(i32, let a = 1; let b = 2; let c = a + b; if (0 < c) {
        blk_return_(-1);
    } blk_return_(c););
    printf("%d\n", scope_2);

    i32 scope_3 = 0;
    asg_blk(&scope_3,
             let a = 1;
             let b = 2;
             let c = a + b;
             if (0 < c) {
                 blk_return_(-1);
             } blk_return_(c););
    printf("%d\n", scope_3);

    // let a = 0 < 1 ? ({ 1; }) : ({ claim_unreachable; });
    let a = blk(
        i32 _return_val;
        var _then = _Generic(
            TypeOf(blk(0)),
            void: blk(
                $ignore_void blk(0);
                (TypeOf(_return_val)){}
            ),
            default: blk(0)
        );
        var _else = _Generic(
            TypeOf(blk(claim_unreachable)),
            void: blk(
                $ignore_void blk(claim_unreachable);
                (TypeOf(_return_val)){}
            ),
            default: blk(claim_unreachable)
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

    let_ignore = blk(
        i32 _return_val;
        var _then = match$(TypeOf(blk(0)), case$(void, blk($ignore_void blk(0); blk_return make$(TypeOf(_return_val)))), otherwise(blk(0)));
        var _else = match$(TypeOf(blk(claim_unreachable)), case$(void, blk($ignore_void blk(claim_unreachable); blk_return make$(TypeOf(_return_val)))), otherwise(blk(claim_unreachable)));
        0 < 1 ? _then : _else;
    );
    printf("%d\n", a);

#define eval_then                                      ,
#define eval_else                                      ,
#define eval_if$(T, cond, ...)                         comp_syn__expr_if$(T, cond, __VA_ARGS__)
#define comp_syn__expr_if$(T, cond, _Then, _Else, ...) ({ \
    cond ? match$(TypeOf(_Then), case$(void, blk($ignore_void _Then; blk_return make$(T))), otherwise(_Then)) : match$(TypeOf(_Else), case$(void, blk($ignore_void _Else; blk_return make$(T))), otherwise(_Else)); \
})

    let c = eval_if$(i32, 0 < 1 eval_then 1 eval_else eval_if$(i32, 0 < 2 eval_then 2 eval_else 3));
    printf("%d\n", c);

    return_ok({});
} $unscoped_(fn);
#endif /* UNUSED_CODE */
