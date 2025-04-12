#include "dh/main.h"

#define break_(Expr...) eval({   \
    __ret = (TypeOf(__ret))Expr; \
    break;                       \
})
#define for_(_Range, _Iter, _Expr...) eval({                           \
    let_(__range, Range) = Range_from _Range;                          \
    for (var_(__i, usize) = __range.begin; __i < __range.end; ++__i) { \
        let_(_Iter, usize) = __i;                                      \
        _Expr;                                                         \
    }                                                                  \
})
#define for$(T, _Range, _Iter, _Expr...) eval({                        \
    var_(__ret, T)       = {};                                         \
    let_(__range, Range) = Range_from _Range;                          \
    for (var_(__i, usize) = __range.begin; __i < __range.end; ++__i) { \
        let_(_Iter, usize) = __i;                                      \
        _Expr;                                                         \
    }                                                                  \
    eval_return __ret;                                                 \
})

fn_TEST_scope("test for") {
    for_((0, 10), i, {
        if (i == 5) { break; }
        try_(TEST_expect(i != 5));
    });

    let res = for$(i32, (0, 10), i, {
        if (i == 5) { break_(i); }
    });
    try_(TEST_expect(res == 5));
} TEST_unscoped;

fn_TEST_scope("test for with break") {
    for_((0, 10), i, {
        if (i == 5) { break; }
        try_(TEST_expect(i != 5));
    });

    let res = orelse(for$(Opt$i32, (0, 10), i, {
        if (i == 5) { break_(some(i)); }
    }), -1);
    try_(TEST_expect(res == 5));
} TEST_unscoped;
