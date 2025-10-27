#include "dh/main.h"


#undef tpl$
#undef const$
#undef p$
// #undef a$
// #undef s$

#define raw$$p$(_T)        TypeOf(_T*)
#define raw$$a$(_N, _T...) TypeOf(_T[_N])

#define tpl$(_T) pp_join($, tpl, _T)

#define const$(_T...)  pp_join($, const, _T)
#define const$$(_T...) TypeOf(const _T)
#define tpl$const$(_T...) \
    typedef const$$(_T) const$(_T)

#define p$(_T...)  pp_join($, p, _T)
#define p$$(_T...) raw$$p$(_T)
#define tpl$p$(_T...) \
    typedef raw$$p$(_T) p$(_T)

#define a$(_N, _T...)  pp_join($, a##$##_N, _T)
#define a$$(_N, _T...) TypeOf(struct { raw$$a$(_N, _T) val; })
#define tpl$a$(_N, _T...) \
    typedef struct a$(_N, _T) a$(_N, _T); \
    struct a$(_N, _T) { \
        raw$$a$(_N, _T) val; \
    }

#define init$a(_initial...) { .val = _initial }
#define init$a$(_N_T__initial...) \
    pp_expand(pp_defer(comp_inline_block__init$a$)(comp_expand_param__init$a$ _N_T__initial))
#define comp_expand_param__init$a$(_N_T...) pp_countArg(_N_T), (_N_T), pp_expand
#define comp_inline_block__init$a$(_count_args, _N_T, _initial...) \
    pp_defer(pp_cat3)(comp_inline_block, _count_args, __init$a$)(_N_T, _initial)
#define comp_inline_block1__init$a$(_a$N$T, _initial...) \
    (_a$N$T{ .val = _initial })
#define comp_inline_block2__init$a$(_N_T, _initial...) \
    ((a$ _N_T){ .val = _initial })

#define from$a$(_T__initial...) \
    pp_expand(pp_defer(comp_inline__from$a$)(comp_param__from$a$ _T__initial))
#define comp_param__from$a$(_T...)            _T, pp_expand
#define comp_inline__from$a$(_T, _initial...) ((struct { \
    raw$$a$(sizeOf$((_T[])_initial) / sizeOf$(_T), _T) val; \
}){ .val = _initial })

#define val$a(_a...) ((_a).val)
#define ptr$a(_a...) (&*val$a(_a))
#define len$a(_a...) countOf$(TypeOf((_a).val))

#define asg$a(_p_lhs, _rhs...) \
    comp_inline__asg$a(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$a(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*_p_lhs; \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(__rhs))); \
    claim_assert_static(isSameType$(TypeOf(val$a(*__p_lhs)), TypeOf(val$a(__rhs)))); \
    *__p_lhs = *as$((TypeOf(__p_lhs)*)(&__rhs)); \
    __p_lhs; \
})

#define at$a(_a, _index...) \
    comp_inline__at$a(pp_uniqTok(a), pp_uniqTok(index), _a, _index)
#define comp_inline__at$a(__a, __index, _a, _index...) ({ \
    let_(__a, TypeOf(&_a)) = &_a; \
    let_(__index, usize)   = _index; \
    claim_assert_static_msg(__builtin_constant_p(__index) ? (__index < len$a(*__a)) : true, "index out of bounds"); \
    debug_assert_fmt(__index < len$a(*__a), "Index out of bounds: %zu >= %zu", __index, len$a(*__a)); \
    &val$a(*__a)[__index]; \
})

#define s$(_T...)  pp_join($, s, _T)
#define s$$(_T...) TypeOf(struct { p$$(_T) ptr; usize len; })
#define tpl$s$(_T...) \
    typedef struct s$(_T) { \
        p$(_T) ptr; \
        usize len; \
    } s$(_T)

#define ptr$s(_s...) ((_s).ptr)
#define len$s(_s...) ((_s).len)
#define asg$s(_p_lhs, _rhs...) \
    comp_inline__asg$s(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$s(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*_p_lhs; \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(__rhs))); \
    claim_assert_static(isSameType$(TypeOf(ptr$s(*__p_lhs)), TypeOf(ptr$s(__rhs)))); \
    *__p_lhs = *as$((TypeOf(__p_lhs)*)(&__rhs)); \
    __p_lhs; \
})

#define ref$a(_T, _p_a...) ((s$(_T)){ \
    .ptr = val$a(*(_p_a)), \
    .len = len$a(*(_p_a)), \
})
#define deref$s(_N, _T, _s...) (&*({ \
    let __result = &*({ \
        typedef a$$$(_N, _T) a$(_N, _T); \
        let_(__n, usize) = _N; \
        let __s          = _s; \
        debug_assert_fmt(__s.len == __n, "length mismatch: %zu != %zu", __s.len, __n); \
        as$((a$(_N, _T)*)(__s.ptr)); \
    }); \
    _Generic(__result, a$(_N, _T)*: __result, default: as$(a$(_N, _T)*, __result)); \
    __result; \
}))
// #define deref$s(_N, _T, _s...) (&*({ \
//     typedef a$$$(_N, _T) a$(_N, _T); \
//     let_(__n, usize) = _N; \
//     let __s          = _s; \
//     debug_assert_fmt(__s.len == __n, "length mismatch: %zu != %zu", __s.len, __n); \
//     as$((a$(_N, _T)*)(__s.ptr)); \
// }))

// #define index(_arr, _idx) ({ \
//     let __arr_len = sizeOf$(_arr) / sizeOf$((_arr)[0]); \
//     let __idx     = _idx; \
//     _Static_assert(__builtin_constant_p(__idx) ? ((__idx) < (__arr_len)) : true, "index out of bounds"); \
//     assert((__idx) < (__arr_len)); \
//     (_arr)[__idx]; \
// })

tpl$(p$)(i32);
tpl$(s$)(i32);
tpl$(const$)(i32);
tpl$(p$)(const$i32);
tpl$(s$)(const$i32);
tpl$(a$)(4, i32);
TEST_fn_("test" $scope) {
    var a = from$a$((i32)({ 0, 1, 2, 3 }));
    let b = from$a$((i32)({ 4, 5, 6, 7 }));
    var c = init$a$((4, i32)({ [0] = 0, [1] = 1, [2] = 2, [3] = 3 }));
    var d = init$a$((a$(4, i32))({ [0] = 0, [1] = 1, [2] = 2, [3] = 3 }));
    let e = asg$a(&a, b);

    var s = ref$a(i32, &a);
    var t = (s$$(i32)){ .ptr = ptr$a(a), .len = len$a(a) };

    a$$(4, a$(4, i32)) mat0 = init$a({
        init$a({ 0, 1, 2, 3 }),
        init$a({ 4, 5, 6, 7 }),
        init$a({ 8, 9, 10, 11 }),
        init$a({ 12, 13, 14, 15 }),
    });
    a$$(4, a$(4, i32)) mat1 = init$a({
        init$a({ 0, 1, 2, 3 }),
        init$a({ 4, 5, 6, 7 }),
        init$a({ 8, 9, 10, 11 }),
        init$a({ 12, 13, 14, 15 }),
    });
    asg$a(&mat0, mat1);

    let_ignore = c;
    let_ignore = d;
    let_ignore = s;
    let_ignore = t;

    try_(TEST_expect(*at$a(a, 0) == *at$a(b, 0)));
    try_(TEST_expect(*at$a(a, 1) == *at$a(b, 1)));
    try_(TEST_expect(*at$a(a, 2) == *at$a(b, 2)));
    try_(TEST_expect(*at$a(a, 3) == *at$a(b, 3)));

    try_(TEST_expect(e == &a));
    try_(TEST_expect(at$a(*e, 0) == at$a(a, 0)));
    try_(TEST_expect(at$a(*e, 1) == at$a(a, 1)));
    try_(TEST_expect(at$a(*e, 2) == at$a(a, 2)));
    try_(TEST_expect(at$a(*e, 3) == at$a(a, 3)));

    // int a[4] = {};
    // index(a, 0);
    // index(a, 1);
    // index(a, 2);
    // index(a, 3);
    // index(a, 4);
    // usize       t = 5;
    // const usize n = t;
    // index(a, n);

    // let s = ref$a(i32, &a_from(i32, { 0, 1, 2, 3 }));
    // var a = deref$s(4, i32, s);
    // var b = deref$s(4, i32, s);
    // a     = b;
    // try_(TEST_expect(*at$a(*a, 0) == 0));
    // try_(TEST_expect(*at$a(*a, 1) == 1));
    // try_(TEST_expect(*at$a(*a, 2) == 2));

    // try_(TEST_expect(s.ptr == ptr$a(*a)));
    // try_(TEST_expect(s.len == len$a(*a)));

    // var c = ~(-3);
} $unscoped_(TEST_fn);
