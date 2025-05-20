#include "dh/main.h"
#include "dh/Arr.h"

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

/*
#define __for_s__unwrap(_Tuple...) _Tuple
#define __for_s__eachSli(_sli)    let pp_cat(__, _sli) = _sli
#define __for_s__eachIter

#define for_s(_Tuple_Sli, _Tuple_Iter, _Expr) eval({                           \
    let_(__Tuple_sli, Sli$T) = _Tuple_Sli;                                     \
    let_(__Tuple_iter, T)    = _Tuple_Iter;                                    \
    for (var_(__i, usize) = __Tuple_sli.begin; __i < __Tuple_sli.end; ++__i) { \
        let_(__Tuple_iter, T) = __Tuple_sli.ptr[__i];                          \
        _Expr;                                                               \
    }                                                                        \
})



#define $ignore_capture                                     \
    __attribute__((unused)) pp_uniqTok(ignored) = (Void){}; \
    $ignore

fn_TEST_scope("test for slice") {
    Arr$$(10, i32) arr = Arr_init({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 });

    let pre = Arr_slice$(Sli$i32, arr, (0, 5));
    let suf = Arr_slice$(Sli$i32, arr, (5, 10));

    for_s((pre), (p), {
        printf("%d\n", *p);
    });
    {
        let __s0 = pre;
        for (usize __i = 0; __i < __s0.len; ++__i) {
            let p = Sli_at(__s0, __i);
            {
                printf("%d\n", *p);
            }
        }
    }

    for_s((pre), (p), idx, {
        printf("[%llu]: %d\n", idx, *p);
    });
    {
        let __s0 = pre;
        for (usize __i = 0; __i < __s0.len; ++__i) {
            let idx = __i;
            let p   = Sli_at(__s0, idx);
            {
                printf("[%llu]: %d\n", idx, *p);
            }
        }
    }

    for_s((pre, suf), (p, s), {
        printf("%d %d\n", *p, *s);
    });
    let __s0 = pre;
    let __s1 = suf;
    debug_assert(__s0.len == __s1.len);
    for (usize __i = 0; __i < __s0.len; ++__i) {
        let p = Sli_at(__s0, __i);
        let s = Sli_at(__s1, __i);
        {
            printf("%d %d\n", *p, *s);
        }
    }

    for_s((pre, suf), (p, $ignore_capture), {
        printf("%d\n", *p);
    });
    let __s0 = pre;
    let __s1 = suf;
    debug_assert(__s0.len == __s1.len);
    for (usize __i = 0; __i < __s0.len; ++__i) {
        let                         p                   = Sli_at(__s0, __i);
        var __attribute__((unused)) pp_uniqTok(ignored) = (Void){};
        $ignore                                         = Sli_at(__s1, __i);
        {
            printf("%d\n", *p);
        }
    }
} TEST_unscoped;
 */

#define pp_tuple_wrap(...)   (__VA_ARGS__)
#define pp_tuple_unwrap(...) __VA_ARGS__

#define pp_tuple_get1st(_a, ...)                             _a
#define pp_tuple_get2nd(_a, _b, ...)                         _b
#define pp_tuple_get3rd(_a, _b, _c, ...)                     _c
#define pp_tuple_get4th(_a, _b, _c, _d, ...)                 _d
#define pp_tuple_get5th(_a, _b, _c, _d, _e, ...)             _e
#define pp_tuple_get6th(_a, _b, _c, _d, _e, _f, ...)         _f
#define pp_tuple_get7th(_a, _b, _c, _d, _e, _f, _g, ...)     _g
#define pp_tuple_get8th(_a, _b, _c, _d, _e, _f, _g, _h, ...) _h

#define pp_tuple_pushFront(_elem, _tuple) pp_tuple_wrap(_elem, pp_tuple_unwrap _tuple)
#define pp_tuple_pushBack(_elem, _tuple)  pp_tuple_wrap(pp_tuple_unwrap _tuple, _elem)



#define $ignore_capture comp_attr__$ignore_capture(pp_uniqTok(ignored))
#define comp_attr__$ignore_capture(_ignored) \
    $maybe_unused _ignored = (Void){};       \
    $ignore

#define for_s1(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s1(pp_uniqTok(i), _Tuple_Slice, _Tuple_Capture, _Expr)
#define for_s1i(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s1i(pp_uniqTok(i), _Tuple_Slice, _Tuple_Capture, _Expr)

#define comp_syn__for_s1(_i, _Tuple_Slice, _Tuple_Capture, _Expr...) ({ \
    let _s0 = pp_tuple_get1st _Tuple_Slice;                             \
    for (usize _i = 0; _i < _s0.len; ++_i) {                            \
        let pp_tuple_get1st _Tuple_Capture = (_s0.ptr + _i);            \
        _Expr;                                                          \
    }                                                                   \
})
#define comp_syn__for_s1i(_i, _Tuple_Slice, _Tuple_Capture, _Expr...) ({                 \
    let _s0 = pp_tuple_get1st _Tuple_Slice;                                              \
    for (usize _i = 0; _i < _s0.len; ++_i) {                                             \
        let pp_tuple_get2nd _Tuple_Capture = _i;                                         \
        let pp_tuple_get1st _Tuple_Capture = (_s0.ptr + pp_tuple_get2nd _Tuple_Capture); \
        _Expr;                                                                           \
    }                                                                                    \
})

#define for_s2(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s2(pp_uniqTok(i), pp_uniqTok(s0), pp_uniqTok(s1), _Tuple_Slice, _Tuple_Capture, _Expr)
#define for_s2i(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s2i(pp_uniqTok(i), pp_uniqTok(s0), pp_uniqTok(s1), _Tuple_Slice, _Tuple_Capture, _Expr)

#define comp_syn__for_s2(_i, _s0, _s1, _Tuple_Slice, _Tuple_Capture, _Expr...) ({ \
    let _s0 = pp_tuple_get1st _Tuple_Slice;                                       \
    let _s1 = pp_tuple_get2nd _Tuple_Slice;                                       \
    debug_assert(_s0.len == _s1.len);                                             \
    for (usize _i = 0; _i < _s0.len; ++_i) {                                      \
        let pp_tuple_get1st _Tuple_Capture = (_s0.ptr + _i);                      \
        let pp_tuple_get2nd _Tuple_Capture = (_s1.ptr + _i);                      \
        _Expr;                                                                    \
    }                                                                             \
})
#define comp_syn__for_s2i(_i, _s0, _s1, _Tuple_Slice, _Tuple_Capture, _Expr...) ({       \
    let _s0 = pp_tuple_get1st _Tuple_Slice;                                              \
    let _s1 = pp_tuple_get2nd _Tuple_Slice;                                              \
    debug_assert(_s0.len == _s1.len);                                                    \
    for (usize _i = 0; _i < _s0.len; ++_i) {                                             \
        let pp_tuple_get3rd _Tuple_Capture = _i;                                         \
        let pp_tuple_get1st _Tuple_Capture = (_s0.ptr + pp_tuple_get3rd _Tuple_Capture); \
        let pp_tuple_get2nd _Tuple_Capture = (_s1.ptr + pp_tuple_get3rd _Tuple_Capture); \
        _Expr;                                                                           \
    }                                                                                    \
})

#define for_s3(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s3(pp_uniqTok(i), pp_uniqTok(s0), pp_uniqTok(s1), pp_uniqTok(s2), _Tuple_Slice, _Tuple_Capture, _Expr)
#define for_s3i(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s3i(pp_uniqTok(i), pp_uniqTok(s0), pp_uniqTok(s1), pp_uniqTok(s2), _Tuple_Slice, _Tuple_Capture, _Expr)

#define comp_syn__for_s3(_i, _s0, _s1, _s2, _Tuple_Slice, _Tuple_Capture, _Expr...) ({ \
    let _s0 = pp_tuple_get1st _Tuple_Slice;                                            \
    let _s1 = pp_tuple_get2nd _Tuple_Slice;                                            \
    let _s2 = pp_tuple_get3rd _Tuple_Slice;                                            \
    debug_assert(_s0.len == _s1.len);                                                  \
    debug_assert(_s1.len == _s2.len);                                                  \
    for (usize _i = 0; _i < _s0.len; ++_i) {                                           \
        let pp_tuple_get1st _Tuple_Capture = (_s0.ptr + _i);                           \
        let pp_tuple_get2nd _Tuple_Capture = (_s1.ptr + _i);                           \
        let pp_tuple_get3rd _Tuple_Capture = (_s2.ptr + _i);                           \
        _Expr;                                                                         \
    }                                                                                  \
})
#define comp_syn__for_s3i(_i, _s0, _s1, _s2, _Tuple_Slice, _Tuple_Capture, _Expr...) ({  \
    let _s0 = pp_tuple_get1st _Tuple_Slice;                                              \
    let _s1 = pp_tuple_get2nd _Tuple_Slice;                                              \
    let _s2 = pp_tuple_get3rd _Tuple_Slice;                                              \
    debug_assert(_s0.len == _s1.len);                                                    \
    debug_assert(_s1.len == _s2.len);                                                    \
    for (usize _i = 0; _i < _s0.len; ++_i) {                                             \
        let pp_tuple_get4th _Tuple_Capture = _i;                                         \
        let pp_tuple_get1st _Tuple_Capture = (_s0.ptr + pp_tuple_get4th _Tuple_Capture); \
        let pp_tuple_get2nd _Tuple_Capture = (_s1.ptr + pp_tuple_get4th _Tuple_Capture); \
        let pp_tuple_get3rd _Tuple_Capture = (_s2.ptr + pp_tuple_get4th _Tuple_Capture); \
        _Expr;                                                                           \
    }                                                                                    \
})

/* #define for_s3i_(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    __for_s3i_expander(pp_uniqTok(i), pp_uniqTok(s0), pp_uniqTok(s1), pp_uniqTok(s2), _Tuple_Slice, pp_tuple_unwrap _Tuple_Capture, _Expr)
#define __for_s3i_expander(_i, _s0, _s1, _s2, _Tuple_Slice, _Untuple_Capture, _Expr...) \
    comp_syn__for_s3i_(_i, _s0, _s1, _s2, _Tuple_Slice, _Untuple_Capture, _Expr)
#define comp_syn__for_s3i_(_i, _s0, _s1, _s2, _Tuple_Slice, _c1, _c2, _c3, _c4, _Expr...) ({ \
    let _s0 = pp_tuple_get1st _Tuple_Slice;                                                  \
    let _s1 = pp_tuple_get2nd _Tuple_Slice;                                                  \
    let _s2 = pp_tuple_get3rd _Tuple_Slice;                                                  \
    debug_assert(_s0.len == _s1.len);                                                        \
    debug_assert(_s1.len == _s2.len);                                                        \
    for (usize _i = 0; _i < _s0.len; ++_i) {                                                 \
        let _c4 = _i;                                                                        \
        let _c1 = (_s0.ptr + _c4);                                                           \
        let _c2 = (_s1.ptr + _c4);                                                           \
        let _c3 = (_s2.ptr + _c4);                                                           \
        _Expr;                                                                               \
    }                                                                                        \
}) */

fn_TEST_scope("test for slices") {
    Arr$$(10, i32) arr = Arr_init({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 });

    let s1 = Arr_slice$(Sli$i32, arr, (0, 5));
    let s2 = Arr_slice$(Sli$i32, arr, (5, 10));
    let s3 = Arr_slice$(Sli$i32, arr, (0 + (s1.len / 2), 10 - (s2.len / 2 + 1)));

    for_s1((s1), (e1), {
        printf("s1: %d\n", *e1);
    });
    printf("\n");
    for_s1i((s1), (e1, i), {
        printf("s1[%llu]: %d\n", i, *e1);
    });
    printf("\n");

    for_s2((s1, s2), (e1, e2), {
        printf("s1: %d, s2: %d\n", *e1, *e2);
    });
    printf("\n");
    for_s2i((s1, s2), (e1, e2, i), {
        printf("s1[%llu]: %d, s2[%llu]: %d\n", i, *e1, i, *e2);
    });
    printf("\n");

    for_s3((s1, s2, s3), (e1, e2, e3), {
        printf("s1: %d, s2: %d, s3: %d\n", *e1, *e2, *e3);
    });
    printf("\n");
    for_s3i((s1, s2, s3), (e1, e2, e3, i), {
        printf("s1[%llu]: %d, s2[%llu]: %d, s3[%llu]: %d\n", i, *e1, i, *e2, i, *e3);
    });
} TEST_unscoped;

fn_TEST_scope("test for slice with $ignore_capture") {
    Arr$$(10, i32) arr  = Arr_init({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 });
    const Sli$(i32) sli = Arr_slice$(Sli$(i32), arr, (0, 10));
    for_s1i((sli), ($ignore_capture, i), {
        printf("%llu\n", i);
    });
} TEST_unscoped;
