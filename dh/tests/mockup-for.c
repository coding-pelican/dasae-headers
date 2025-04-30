#include "dh/main.h"

#define eval_(Expr...) ({ \
    Expr;                 \
})
#define for$(T, _Range, _Iter, _Expr, _Else...) eval({                 \
    var_(__ret, T)          = {};                                      \
    var_(__is_broken, bool) = false;                                   \
    let_(__range, Range)    = Range_from _Range;                       \
    for (var_(__i, usize) = __range.begin; __i < __range.end; ++__i) { \
        let_(_Iter, usize) = __i;                                      \
        _Expr;                                                         \
    }                                                                  \
    __is_broken ? __ret : (T)_Else;                                    \
})
#define break_(Expr...) eval({         \
    __ret       = (TypeOf(__ret))Expr; \
    __is_broken = true;                \
    break;                             \
})

#define for_(_Range, _Iter, _Expr...) eval({                           \
    let_(__range, Range) = Range_from _Range;                          \
    for (var_(__i, usize) = __range.begin; __i < __range.end; ++__i) { \
        let_(_Iter, usize) = __i;                                      \
        _Expr;                                                         \
    }                                                                  \
})
// #define for$(T, _Range, _Iter, _Expr...) eval({                        \
//     var_(__ret, T)       = {};                                         \
//     let_(__range, Range) = Range_from _Range;                          \
//     for (var_(__i, usize) = __range.begin; __i < __range.end; ++__i) { \
//         let_(_Iter, usize) = __i;                                      \
//         _Expr;                                                         \
//     }                                                                  \
//     eval_return __ret;                                                 \
// })

fn_TEST_scope("test for") {
    for_((0, 10), i, {
        if (i == 5) { break; }
        try_(TEST_expect(i != 5));
    });

    let res = for$(i32, (0, 10), i, {
        if (i == 5) { break_(i); } continue; }, 0);
    try_(TEST_expect(res == 5));
} TEST_unscoped;

fn_TEST_scope("test for with break") {
    for_((0, 10), i, {
        if (i == 5) { break; }
        try_(TEST_expect(i != 5));
    });

    let res = for$(Opt$i32, (0, 10), i, {
        if (i == 5) {
            break_(some(i));
        } }, some(-1));
    try_(TEST_expect(unwrap(res) == 5));
} TEST_unscoped;
