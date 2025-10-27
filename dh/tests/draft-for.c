#if UNUSED_CODE
#include "dh/main.h"
#include "dh/Arr.h"

// #undef eval_
// #define expr_(Expr...) ({ \
//     Expr; \
// })
#define for$(T, _Range, _Iter, _Expr, _Else...) blk({ \
    var_(__ret, T)          = {}; \
    var_(__is_broken, bool) = false; \
    let_(__range, Range)    = Range_from _Range; \
    for (var_(__i, usize) = __range.begin; __i < __range.end; ++__i) { \
        let_(_Iter, usize) = __i; \
        _Expr; \
    } \
    __is_broken ? __ret : (T)_Else; \
})
// #undef break_
#define break_(Expr...) blk({ \
    __ret       = (TypeOf(__ret))Expr; \
    __is_broken = true; \
    break; \
})

// #define for$(T, _Range, _Iter, _Expr...) blk({                        \
//     var_(__ret, T)       = {};                                         \
//     let_(__range, Range) = Range_from _Range;                          \
//     for (var_(__i, usize) = __range.begin; __i < __range.end; ++__i) { \
//         let_(_Iter, usize) = __i;                                      \
//         _Expr;                                                         \
//     }                                                                  \
//     blk_return __ret;                                                 \
// })

TEST_fn_("test for" $scope) {
    for_($r(0, 10), i) {
        if (i == 5) { break; }
        try_(TEST_expect(i != 5));
    }

    let res = eval_(i32 $scope)(for_($r(0, 10), i) {
        if (i == 5) { $break_(i); }
        continue;
    }) eval_(else)({ $break_(0); }) $unscoped_(expr);
    try_(TEST_expect(res == 5));
} $unscoped_(TEST_fn);

TEST_fn_("test for with break" $scope) {
    for_($r(0, 10), i) {
        if (i == 5) { break; }
        try_(TEST_expect(i != 5));
    }

    let res = eval_(O$i32 $scope)(for_($r(0, 10), i) {
        if (i == 5) { $break_(some(i)); }
    }) eval_(else)({ $break_(some(-1)); }) $unscoped_(expr);
    try_(TEST_expect(unwrap(res) == 5));
} $unscoped_(TEST_fn);

/*
#define __for_s__unwrap(_Tuple...) _Tuple
#define __for_s__eachSli(_sli)    let pp_cat(__, _sli) = _sli
#define __for_s__eachIter

#define for_s(_Tuple_Sli, _Tuple_Iter, _Expr) blk({                           \
    let_(__Tuple_sli, S$T) = _Tuple_Sli;                                     \
    let_(__Tuple_iter, T)    = _Tuple_Iter;                                    \
    for (var_(__i, usize) = __Tuple_sli.begin; __i < __Tuple_sli.end; ++__i) { \
        let_(__Tuple_iter, T) = __Tuple_sli.ptr[__i];                          \
        _Expr;                                                               \
    }                                                                        \
})



#define $ignore                                     \
    __attribute__((unused)) pp_uniqTok(ignored) = (Void){}; \
    $ignore

fn_TEST_scope("test for slice") {
    A$$(10, i32) arr = A_init({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 });

    let pre = A_slice$(S$i32, arr, (0, 5));
    let suf = A_slice$(S$i32, arr, (5, 10));

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

    for_s((pre, suf), (p, $ignore), {
        printf("%d\n", *p);
    });
    let __s0 = pre;
    let __s1 = suf;
    debug_assert(__s0.len == __s1.len);
    for (usize __i = 0; __i < __s0.len; ++__i) {
        let                         p                   = Sli_at(__s0, __i);
        var __attribute__((unused)) pp_uniqTok(ignored) = (Void){};
        let_ignore                                         = Sli_at(__s1, __i);
        {
            printf("%d\n", *p);
        }
    }
} TEST_unscoped;
 */



// #define $ignore comp_attr__$ignore(pp_uniqTok(ignored))
// #define comp_attr__$ignore(_ignored) \
//     $maybe_unused _ignored = (Void){}; \
//     $ignore

#define for_s1(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s1(pp_uniqTok(i), _Tuple_Slice, _Tuple_Capture, _Expr)
#define for_s1i(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s1i(pp_uniqTok(i), _Tuple_Slice, _Tuple_Capture, _Expr)

#define comp_syn__for_s1(_i, _Tuple_Slice, _Tuple_Capture, _Expr...) ({ \
    let _s0 = pp_Tuple_get1st _Tuple_Slice; \
    for (usize _i = 0; _i < _s0.len; ++_i) { \
        let pp_Tuple_get1st _Tuple_Capture = (_s0.ptr + _i); \
        _Expr; \
    } \
})
#define comp_syn__for_s1i(_i, _Tuple_Slice, _Tuple_Capture, _Expr...) ({ \
    let _s0 = pp_Tuple_get1st _Tuple_Slice; \
    for (usize _i = 0; _i < _s0.len; ++_i) { \
        const usize pp_Tuple_get2nd _Tuple_Capture = _i; \
        let pp_Tuple_get1st         _Tuple_Capture = (_s0.ptr + pp_Tuple_get2nd _Tuple_Capture); \
        _Expr; \
    } \
})

#define for_s2(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s2(pp_uniqTok(i), pp_uniqTok(s0), pp_uniqTok(s1), _Tuple_Slice, _Tuple_Capture, _Expr)
#define for_s2i(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s2i(pp_uniqTok(i), pp_uniqTok(s0), pp_uniqTok(s1), _Tuple_Slice, _Tuple_Capture, _Expr)

#define comp_syn__for_s2(_i, _s0, _s1, _Tuple_Slice, _Tuple_Capture, _Expr...) ({ \
    let _s0 = pp_Tuple_get1st _Tuple_Slice; \
    let _s1 = pp_Tuple_get2nd _Tuple_Slice; \
    debug_assert(_s0.len == _s1.len); \
    for (usize _i = 0; _i < _s0.len; ++_i) { \
        let pp_Tuple_get1st _Tuple_Capture = (_s0.ptr + _i); \
        let pp_Tuple_get2nd _Tuple_Capture = (_s1.ptr + _i); \
        _Expr; \
    } \
})
#define comp_syn__for_s2i(_i, _s0, _s1, _Tuple_Slice, _Tuple_Capture, _Expr...) ({ \
    let _s0 = pp_Tuple_get1st _Tuple_Slice; \
    let _s1 = pp_Tuple_get2nd _Tuple_Slice; \
    debug_assert(_s0.len == _s1.len); \
    for (usize _i = 0; _i < _s0.len; ++_i) { \
        const usize pp_Tuple_get3rd _Tuple_Capture = _i; \
        let pp_Tuple_get1st         _Tuple_Capture = (_s0.ptr + pp_Tuple_get3rd _Tuple_Capture); \
        let pp_Tuple_get2nd         _Tuple_Capture = (_s1.ptr + pp_Tuple_get3rd _Tuple_Capture); \
        _Expr; \
    } \
})

#define for_s3(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s3(pp_uniqTok(i), pp_uniqTok(s0), pp_uniqTok(s1), pp_uniqTok(s2), _Tuple_Slice, _Tuple_Capture, _Expr)
#define for_s3i(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    comp_syn__for_s3i(pp_uniqTok(i), pp_uniqTok(s0), pp_uniqTok(s1), pp_uniqTok(s2), _Tuple_Slice, _Tuple_Capture, _Expr)

#define comp_syn__for_s3(_i, _s0, _s1, _s2, _Tuple_Slice, _Tuple_Capture, _Expr...) ({ \
    let _s0 = pp_Tuple_get1st _Tuple_Slice; \
    let _s1 = pp_Tuple_get2nd _Tuple_Slice; \
    let _s2 = pp_Tuple_get3rd _Tuple_Slice; \
    debug_assert(_s0.len == _s1.len); \
    debug_assert(_s1.len == _s2.len); \
    for (usize _i = 0; _i < _s0.len; ++_i) { \
        let pp_Tuple_get1st _Tuple_Capture = (_s0.ptr + _i); \
        let pp_Tuple_get2nd _Tuple_Capture = (_s1.ptr + _i); \
        let pp_Tuple_get3rd _Tuple_Capture = (_s2.ptr + _i); \
        _Expr; \
    } \
})
#define comp_syn__for_s3i(_i, _s0, _s1, _s2, _Tuple_Slice, _Tuple_Capture, _Expr...) ({ \
    let _s0 = pp_Tuple_get1st _Tuple_Slice; \
    let _s1 = pp_Tuple_get2nd _Tuple_Slice; \
    let _s2 = pp_Tuple_get3rd _Tuple_Slice; \
    debug_assert(_s0.len == _s1.len); \
    debug_assert(_s1.len == _s2.len); \
    for (usize _i = 0; _i < _s0.len; ++_i) { \
        const usize pp_Tuple_get4th _Tuple_Capture = _i; \
        let pp_Tuple_get1st         _Tuple_Capture = (_s0.ptr + pp_Tuple_get4th _Tuple_Capture); \
        let pp_Tuple_get2nd         _Tuple_Capture = (_s1.ptr + pp_Tuple_get4th _Tuple_Capture); \
        let pp_Tuple_get3rd         _Tuple_Capture = (_s2.ptr + pp_Tuple_get4th _Tuple_Capture); \
        _Expr; \
    } \
})

/* #define for_s3i_(_Tuple_Slice, _Tuple_Capture, _Expr...) \
    __for_s3i_expander(pp_uniqTok(i), pp_uniqTok(s0), pp_uniqTok(s1), pp_uniqTok(s2), _Tuple_Slice, pp_Tuple_unwrap _Tuple_Capture, _Expr)
#define __for_s3i_expander(_i, _s0, _s1, _s2, _Tuple_Slice, _Untuple_Capture, _Expr...) \
    comp_syn__for_s3i_(_i, _s0, _s1, _s2, _Tuple_Slice, _Untuple_Capture, _Expr)
#define comp_syn__for_s3i_(_i, _s0, _s1, _s2, _Tuple_Slice, _c1, _c2, _c3, _c4, _Expr...) ({ \
    let _s0 = pp_Tuple_get1st _Tuple_Slice;                                                  \
    let _s1 = pp_Tuple_get2nd _Tuple_Slice;                                                  \
    let _s2 = pp_Tuple_get3rd _Tuple_Slice;                                                  \
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

TEST_fn_("test for slices" $scope) {
    A$$(10, i32) arr = A_init({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 });

    let s1 = A_slice$(S$i32, arr, (0, 5));
    let s2 = A_slice$(S$i32, arr, (5, 10));
    let s3 = A_slice$(S$i32, arr, (0 + (s1.len / 2), 10 - (s2.len / 2 + 1)));

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
} $unscoped_(TEST_fn);

TEST_fn_("test for slice with $ignore" $scope) {
    A$$(10, i32) arr  = A_init({ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 });
    const S$(i32) sli = A_slice$(S$(i32), arr, (0, 10));
    for_s1i((sli), ($ignore, i), {
        printf("%llu\n", i);
    });
} $unscoped_(TEST_fn);

typedef struct {
} TypeHint$R;
typedef struct R {
    TypeHint$R __hint;
    usize      begin;
    usize      end;
} R;
#define $r(_begin, _end) ((R){ .begin = (_begin), .end = (_end) })

#define $for_(_range, _iter, _body...) \
    for ( \
        struct { bool once; R range; usize iter; } __state = { .once = true, .range = (_range), .iter = (_range).begin }; \
        __state.once; \
        __state.once = false \
    ) \
        for ( \
            const usize _iter = __state.iter; \
            __state.iter < __state.range.end; \
            ++(__state.iter) \
        ) _body

// // clang-format off
// #define eval_(...) pp_overload(inline__expr, __VA_ARGS__)(__VA_ARGS__)
// #define inline__eval_1(_keyword...) if (!__has_broken) {} _keyword
// #define inline__eval_2(T, _keyword...) ({ T __reserved_break; bool __has_broken = false;
// #define inline__$unscoped_expr() __reserved_break; })
#define $break_(_Expr...) ({ \
    bti_memcpy( \
        as$((u8*)(__reserved_break)), \
        as$((u8*)((TypeOf (*__reserved_break)[1]){ [0] = _Expr })), \
        sizeOf$(*__reserved_break) \
    ); \
    __has_broken = true; \
    goto __step_break; \
})
// // clang-format on



// clang-format off
#define eval_(T_Break_w_Ext...) inline__expr(T_Break_w_Ext)
#define inline__expr(T_Break_w_Ext...) pp_overload(inline__expr, T_Break_w_Ext)(T_Break_w_Ext)
#define inline__eval_1(_keyword...) ; if (__has_broken) { goto __step_break; } _keyword
#define inline__eval_2(T_Break, _Ext...) pp_cat(inline__eval_2, _Ext)(T_Break)
#define inline__eval_2$_scope(T_Break...) ({ \
    local_label __step_break; \
    let __reserved_break = as$((T_Break*)((u8[_Generic(T_Break, \
        void: 0, \
        default: sizeOf$(T_Break) \
    )]){})); \
    $maybe_unused bool __has_broken = false;\
    /* do */
#define $unscoped_expr comp_syn__eval_$unscoped
#define comp_syn__eval_$unscoped \
    /* while(false) */; \
    __step_break: \
    _Generic(TypeOf(*__reserved_break), \
        void: ({}), \
        default: __reserved_break[0] \
    ); \
})
// clang-format on

#define $unscoped_(_keyword) \
    pp_cat(inline__$unscoped_, _keyword)()
#define inline__$unscoped_fn()      $unscoped
#define inline__$unscoped_(TEST_fn) () $unscoped_(TEST_fn)
#define inline__$unscoped_blk()    $unscoped_eval
#define inline__$unscoped_expr()    $unscoped_expr


#define $if_(_cond, _body...) \
    if (_cond) _body

TEST_fn_("test eval function" $scope) {
    const usize key = 12;

    let value_for = eval_(S_const$u8 $scope) $for_($r(0, 10), i, {
        if (i == key) { $break_(u8_l("first")); }
    }) eval_(else) $for_($r(10, 20), i, {
        if (i == key) { $break_(u8_l("second")); }
    }) eval_(else) {
        $break_(u8_l("third"));
    } $unscoped_(expr);

    let value_if = expr_(S_const$u8 $scope) if (false) {
        $break_(u8_l("first"));
    }
    else if (true) {
        $break_(u8_l("second"));
    }
    else {
        claim_unreachable;
    } $unscoped_(expr);

    try_(TEST_expect(Str_eql(value_for, value_if)));
} $unscoped_(TEST_fn);
#endif /* UNUSED_CODE */
