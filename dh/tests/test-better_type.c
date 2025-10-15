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
#define a$$(_N, _T...) TypeOf(struct { raw$$a$(_N, _T) buf; })
#define tpl$a$(_N, _T...) \
    typedef struct a$(_N, _T) a$(_N, _T); \
    struct a$(_N, _T) { \
        raw$$a$(_N, _T) buf; \
    }

#define a_init$(_N_T__initial...) \
    pp_expand(pp_defer(comp_inline_block__a_init$)(comp_expand_param__a_init$ _N_T__initial))
#define comp_expand_param__a_init$(_N_T...) pp_countArg(_N_T), (_N_T), pp_expand
#define comp_inline_block__a_init$(_count_args, _N_T, _initial...) \
    pp_defer(pp_cat3)(comp_inline, _count_args, __a_init$)(_N_T, _initial)
#define comp_inline_block1__a_init$(_a$N$T, _initial...) \
    (_a$N$T{ .buf = _initial })
#define comp_inline_block2__a_init$(_N_T, _initial...) \
    ((a$ _N_T){ .buf = _initial })

#define a_from$(_T__initial...) \
    pp_expand(pp_defer(comp_inline__a_from$)(comp_param__a_from$ _T__initial))
#define comp_param__a_from$(_T...)            _T, pp_expand
#define comp_inline__a_from$(_T, _initial...) ((struct { \
    raw$$a$(sizeOf$((_T[])_initial) / sizeOf$(_T), _T) buf; \
}){ .buf = _initial })

#define a_buf(_a...) ((_a).buf)
#define a_ptr(_a...) (&*a_buf(_a))
#define a_len(_a...) countOf((_a).buf)

#define a_asg(_p_lhs, _rhs...) \
    comp_inline__a_asg(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__a_asg(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*_p_lhs; \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(__rhs))); \
    claim_assert_static(isSameType$(TypeOf(a_buf(*__p_lhs)), TypeOf(a_buf(__rhs)))); \
    *__p_lhs = *as$(TypeOf(__p_lhs), &__rhs); \
    __p_lhs; \
})

#define a_at(_a, _index...) \
    comp_inline__a_at(pp_uniqTok(a), pp_uniqTok(index), _a, _index)
#define comp_inline__a_at(__a, __index, _a, _index...) ({ \
    let_(__a, TypeOf(&_a)) = &_a; \
    let_(__index, usize)   = _index; \
    claim_assert_static_msg(__builtin_constant_p(__index) ? (__index < a_len(*__a)) : true, "index out of bounds"); \
    debug_assert_fmt(__index < a_len(*__a), "Index out of bounds: %zu >= %zu", __index, a_len(*__a)); \
    &a_buf(*__a)[__index]; \
})

#define s$(_T...)  pp_join($, s, _T)
#define s$$(_T...) TypeOf(struct { p$$(_T) ptr; usize len; })
#define tpl$s$(_T...) \
    typedef struct s$(_T) { \
        p$(_T) ptr; \
        usize len; \
    } s$(_T)

#define s_ptr(_s...) ((_s).ptr)
#define s_len(_s...) ((_s).len)
#define s_asg(_p_lhs, _rhs...) \
    comp_inline__s_asg(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__s_asg(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*_p_lhs; \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(__rhs))); \
    claim_assert_static(isSameType$(TypeOf(s_ptr(*__p_lhs)), TypeOf(s_ptr(__rhs)))); \
    *__p_lhs = *as$(TypeOf(__p_lhs), &__rhs); \
    __p_lhs; \
})

#define a_ref(_T, _p_a...) ((s$(_T)){ \
    .ptr = a_buf(*(_p_a)), \
    .len = a_len(*(_p_a)), \
})
#define s_deref(_N, _T, _s...) (&*({ \
    let __result = &*({ \
        typedef a$$$(_N, _T) a$(_N, _T); \
        let_(__n, usize) = _N; \
        let __s          = _s; \
        debug_assert_fmt(__s.len == __n, "length mismatch: %zu != %zu", __s.len, __n); \
        as$(a$(_N, _T)*, __s.ptr); \
    }); \
    _Generic(__result, a$(_N, _T) *: __result, default: as$(a$(_N, _T)*, __result)); \
    __result; \
}))
// #define s_deref(_N, _T, _s...) (&*({ \
//     typedef a$$$(_N, _T) a$(_N, _T); \
//     let_(__n, usize) = _N; \
//     let __s          = _s; \
//     debug_assert_fmt(__s.len == __n, "length mismatch: %zu != %zu", __s.len, __n); \
//     as$(a$(_N, _T)*, __s.ptr); \
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
    var a = a_from$((i32)({ 0, 1, 2, 3 }));
    let b = a_from$((i32)({ 4, 5, 6, 7 }));
    var c = a_init$((4, i32)({ [0] = 0, [1] = 1, [2] = 2, [3] = 3 }));
    var d = a_init$((a$(4, i32))({ [0] = 0, [1] = 1, [2] = 2, [3] = 3 }));
    let e = a_asg(&a, b);

    var s = a_ref(i32, &a);
    var t = (s$$(i32)){ .ptr = a_ptr(a), .len = a_len(a) };

    a$$(4, a$(4, i32)) mat0 = { {
        { 0, 1, 2, 3 },
        { 4, 5, 6, 7 },
        { 8, 9, 10, 11 },
        { 12, 13, 14, 15 },
    } };
    a$$(4, a$(4, i32)) mat1 = { {
        { 0, 1, 2, 3 },
        { 4, 5, 6, 7 },
        { 8, 9, 10, 11 },
        { 12, 13, 14, 15 },
    } };
    a_asg(&mat0, mat1);

    $ignore = c;
    $ignore = d;
    $ignore = s;
    $ignore = t;

    try_(TEST_expect(*a_at(a, 0) == *a_at(b, 0)));
    try_(TEST_expect(*a_at(a, 1) == *a_at(b, 1)));
    try_(TEST_expect(*a_at(a, 2) == *a_at(b, 2)));
    try_(TEST_expect(*a_at(a, 3) == *a_at(b, 3)));

    try_(TEST_expect(e == &a));
    try_(TEST_expect(a_at(*e, 0) == a_at(a, 0)));
    try_(TEST_expect(a_at(*e, 1) == a_at(a, 1)));
    try_(TEST_expect(a_at(*e, 2) == a_at(a, 2)));
    try_(TEST_expect(a_at(*e, 3) == a_at(a, 3)));

    // int a[4] = {};
    // index(a, 0);
    // index(a, 1);
    // index(a, 2);
    // index(a, 3);
    // index(a, 4);
    // usize       t = 5;
    // const usize n = t;
    // index(a, n);

    // let s = a_ref(i32, &a_from(i32, { 0, 1, 2, 3 }));
    // var a = s_deref(4, i32, s);
    // var b = s_deref(4, i32, s);
    // a     = b;
    // try_(TEST_expect(*a_at(*a, 0) == 0));
    // try_(TEST_expect(*a_at(*a, 1) == 1));
    // try_(TEST_expect(*a_at(*a, 2) == 2));

    // try_(TEST_expect(s.ptr == a_ptr(*a)));
    // try_(TEST_expect(s.len == a_len(*a)));

    // var c = ~(-3);
} $unscoped_TEST_fn
;
