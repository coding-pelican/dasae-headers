#ifndef META_OPS_H
#define META_OPS_H

#include "dh/core.h"

/* Raw Types =========================================================*/

typedef const void* p_const$raw;
typedef void*       p$raw;

typedef struct v_const$raw v_const$raw;
struct v_const$raw {
    const u8 val $flexible;
};
typedef union v$raw v$raw;
union v$raw {
    struct {
        u8 val $flexible;
    };
    v_const$raw as_const;
};

typedef struct s_const$raw s_const$raw;
struct s_const$raw {
    p_const$raw ptr;
    usize       len;
};
typedef union s$raw s$raw;
union s$raw {
    struct {
        p$raw ptr;
        usize len;
    };
    s_const$raw as_const;
};

typedef struct a_const$raw a_const$raw;
struct a_const$raw {
    const u8 arr $flexible;
};
typedef union a$raw a$raw;
union a$raw {
    struct {
        u8 arr $flexible;
    };
    a_const$raw as_const;
};

typedef union o_payload$raw o_payload$raw;
union o_payload$raw {
    Void    none;
    u8 some $flexible;
};
typedef struct o$raw o$raw;
struct o$raw {
    bool is_some;
    union {
        Void          none;
        u8 some       $flexible;
        o_payload$raw raw[1];
    } payload[1];
};

typedef union e_payload$raw e_payload$raw;
union e_payload$raw {
    u32   err;
    u8 ok $flexible;
};

typedef struct e$raw e$raw;
struct e$raw {
    bool is_ok;
    union {
        u32           err;
        u8 ok         $flexible;
        e_payload$raw raw[1];
    } payload[1];
};

/* Base Types ========================================================*/

#define $p_const$(_T...)     TypeOf(const _T*)
#define $p$(_T...)           TypeOf(_T*)
#define $a_const$(_N, _T...) TypeOf(const _T[_N])
#define $a$(_N, _T...)       TypeOf(_T[_N])

/* Pointer Type ======================================================*/

/* Pointer Anonymous */
#define p_const$$(_T...) $p_const$(_T)
#define p$$(_T...)       $p$(_T)
/* Pointer Alias */
#define p_const$(_T...)  pp_join($, p_const, _T)
#define p$(_T...)        pp_join($, p, _T)
/* Pointer Template */
#define tpl$p$(_T...) \
    typedef $p_const$(_T) p_const$(_T); \
    typedef $p$(_T) p$(_T);

/* Slice Type ========================================================*/

/* Slice Anonymous */
#define s_const$$(_T...) \
    union { \
        struct { \
            p_const$$(_T) ptr; \
            usize len; \
        }; \
        s_const$raw         as_raw; \
        s_const$raw ref_raw $like_ptr; \
    }
#define s$$(_T...) \
    union { \
        struct { \
            p$$(_T) ptr; \
            usize len; \
        }; \
        s$raw         as_raw; \
        s$raw ref_raw $like_ptr; \
        s_const$$(_T) as_const; \
    }
/* Slice Alias */
#define s_const$(_T...) pp_join($, s_const, _T)
#define s$(_T...)       pp_join($, s, _T)
/* Slice Template */
#define tpl$s$(_T...) \
    typedef union s_const$(_T) s_const$(_T); \
    union s_const$(_T) { \
        struct { \
            p_const$(_T) ptr; \
            usize len; \
        }; \
        s_const$raw         as_raw; \
        s_const$raw ref_raw $like_ptr; \
    }; \
    typedef union s$(_T) s$(_T); \
    union s$(_T) { \
        struct { \
            p$(_T) ptr; \
            usize len; \
        }; \
        s$raw         as_raw; \
        s$raw ref_raw $like_ptr; \
        s_const$(_T) as_const; \
    }

/* Array Type ========================================================*/

/* Array Anonymous */
#define a_const$$(_N, _T...) \
    union { \
        $a_const$(_N, _T) buf; \
        a_const$raw         as_raw; \
        a_const$raw ref_raw $like_ptr; \
    }
#define a$$(_N, _T...) \
    union { \
        $a$(_N, _T) buf; \
        a$raw         as_raw; \
        a$raw ref_raw $like_ptr; \
        a_const$$(_N, _T) as_const; \
    }
/* Array Alias */
#define a_const$(_N, _T...) pp_join3($, a_const, _N, _T)
#define a$(_N, _T...)       pp_join3($, a, _N, _T)
/* Array Template */
#define tpl$a$(_N, _T...) \
    typedef union a_const$(_N, _T) a_const$(_N, _T); \
    union a_const$(_N, _T) { \
        $a_const$(_N, _T) buf; \
        a_const$raw         as_raw; \
        a_const$raw ref_raw $like_ptr; \
    }; \
    typedef union a$(_N, _T) a$(_N, _T); \
    union a$(_N, _T) { \
        $a$(_N, _T) buf; \
        a$raw         as_raw; \
        a$raw ref_raw $like_ptr; \
        a_const$(_N, _T) as_const; \
    }

/* Optional Type =====================================================*/

/* Optional Anonymous */
#define o$$(_T...) \
    union { \
        struct { \
            bool is_some; \
            union { \
                Void          none; \
                _T            some; \
                o_payload$raw raw[1]; \
            } payload[1]; \
        }; \
        o$raw         as_raw; \
        o$raw ref_raw $like_ptr; \
    }
/* Optional Alias */
#define o$(_T...) pp_join($, o, _T)
/* Optional Template */
#define tpl$o$(_T...) \
    typedef union o$(_T) o$(_T); \
    union o$(_T) { \
        struct { \
            bool is_some; \
            union { \
                Void          none; \
                _T            some; \
                o_payload$raw raw[1]; \
            } payload[1]; \
        }; \
        o$raw         as_raw; \
        o$raw ref_raw $like_ptr; \
    }

/* Result Type =======================================================*/

/* Result Anonymous */
#define e$$(_T...) \
    union { \
        struct { \
            bool is_ok; \
            union { \
                u32           err; \
                _T            ok; \
                e_payload$raw raw[1]; \
            } payload[1]; \
        }; \
        e$raw         as_raw; \
        e$raw ref_raw $like_ptr; \
    }
/* Result Alias */
#define e$(_T...) pp_join($, e, _T)
/* Result Template */
#define tpl$e$(_T...) \
    typedef union e$(_T) e$(_T); \
    union e$(_T) { \
        struct { \
            bool is_ok; \
            union { \
                u32           err; \
                _T            ok; \
                e_payload$raw raw[1]; \
            } payload[1]; \
        }; \
        e$raw         as_raw; \
        e$raw ref_raw $like_ptr; \
    }

/* Operations ========================================================*/

#define zeros$a()                             { .buf = { 0 } }
#define zeros$a$(/*(_N, _T)*/...)             /* TODO: Implement */
#define zeros$s()                             { .ptr = null, .len = 0 }
#define zeros$s$(/*(_T)*/...)                 /* TODO: Implement */
#define init$a(_initial...)                   { .buf = _initial }
#define init$a$(/*(_N, _T)(_initial...)*/...) /* TODO: Implement */
#define init$s(_ptr, _len...)                 { .ptr = (_ptr), .len = (_len) }
#define init$s$(/*(_T)(_ptr, _len)*/...)      /* TODO: Implement */

#define ref$v(_v...) ref(_v)
#define ref$a$(/*(_T)(_a...)*/...) \
    pp_expand(pp_defer(block_inline__ref$a$)(param_expand__ref$a$ __VA_ARGS__))
#define param_expand__ref$a$(...)       __VA_ARGS__, pp_expand
#define block_inline__ref$a$(_T, _a...) ((s$(_T)){ \
    .ptr = ptr$a(_a), \
    .len = len$a(_a), \
})

#define deref$p(_p...) deref(_p)
#define deref$s$(/*(_N, _T)(_Expr...)*/...) \
    pp_expand(pp_defer(block_inline__deref$s$)(param_expand__deref$s$ __VA_ARGS__))
#define param_expand__deref$s$(...)                __VA_ARGS__, pp_uniqTok(s), pp_expand
#define block_inline__deref$s$(_N, _T, __s, _s...) (*({ \
    let_(__s, TypeOf(_s)) = (_s); \
    debug_assert_fmt(len$s(__s) == _N, "length mismatch: %zu != %zu", len$s(__s), _N); \
    as$(a$(_N, _T)*, ensureNonnull(ptr$s(__s))); \
}))

#define a_claim_assert_staticSameType(_p_lhs, _p_rhs...) a_claim_assert_staticSameType1(_p_lhs, _p_rhs)
#define a_claim_assert_staticSameType1(_p_lhs, _p_rhs...) \
    comp_inline__a_claim_assert_staticSameType1(pp_uniqTok(p_lhs), pp_uniqTok(p_rhs), _p_lhs, _p_rhs)
#define comp_inline__a_claim_assert_staticSameType1(__p_lhs, __p_rhs, _p_lhs, _p_rhs...) ({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    let_(__p_rhs, TypeOf(&*_p_rhs)) = &*(_p_rhs); \
    claim_assert_static(len$a(*__p_lhs) == len$a(*__p_rhs)); \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(*__p_rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(*__p_rhs))); \
    claim_assert_static(isSameType$(TypeOf(buf$a(*__p_lhs)), TypeOf(buf$a(*__p_rhs)))); \
})
#define a_claim_assert_staticSameType2(_p_lhs, _p_rhs...) \
    comp_inline__a_claim_assert_staticSameType2(pp_uniqTok(p_lhs), pp_uniqTok(p_rhs), _p_lhs, _p_rhs)
#define comp_inline__a_claim_assert_staticSameType2(__p_lhs, __p_rhs, _p_lhs, _p_rhs...) ({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    let_(__p_rhs, TypeOf(&*_p_rhs)) = &*(_p_rhs); \
    claim_assert_static(len$a(*__p_lhs) == len$a(*__p_rhs)); \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(*__p_rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(*__p_rhs))); \
    a_claim_assert_staticSameType1(&buf$a(*__p_lhs)[0], &buf$a(*__p_rhs)[0]); \
})
#define a_claim_assert_staticSameType3(_p_lhs, _p_rhs...) \
    comp_inline__a_claim_assert_staticSameType3(pp_uniqTok(p_lhs), pp_uniqTok(p_rhs), _p_lhs, _p_rhs)
#define comp_inline__a_claim_assert_staticSameType3(__p_lhs, __p_rhs, _p_lhs, _p_rhs...) ({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    let_(__p_rhs, TypeOf(&*_p_rhs)) = &*(_p_rhs); \
    claim_assert_static(len$a(*__p_lhs) == len$a(*__p_rhs)); \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(*__p_rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(*__p_rhs))); \
    a_claim_assert_staticSameType2(&buf$a(*__p_lhs)[0], &buf$a(*__p_rhs)[0]); \
})
#define a_claim_assert_staticSameType4(_p_lhs, _p_rhs...) \
    comp_inline__a_claim_assert_staticSameType4(pp_uniqTok(p_lhs), pp_uniqTok(p_rhs), _p_lhs, _p_rhs)
#define comp_inline__a_claim_assert_staticSameType4(__p_lhs, __p_rhs, _p_lhs, _p_rhs...) ({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    let_(__p_rhs, TypeOf(&*_p_rhs)) = &*(_p_rhs); \
    claim_assert_static(len$a(*__p_lhs) == len$a(*__p_rhs)); \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(*__p_rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(*__p_rhs))); \
    a_claim_assert_staticSameType3(&buf$a(*__p_lhs)[0], &buf$a(*__p_rhs)[0]); \
})

#define asg$a(_p_lhs, _rhs...) asg$a1(_p_lhs, _rhs)
#define asg$a1(_p_lhs, _rhs...) \
    comp_inline__asg$a1(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$a1(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    a_claim_assert_staticSameType1(__p_lhs, &__rhs); \
    bti_memcpy(__p_lhs, &__rhs, sizeOf$(TypeOf(*__p_lhs))); \
    __p_lhs; \
})
#define asg$a2(_p_lhs, _rhs...) \
    comp_inline__asg$a2(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$a2(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    a_claim_assert_staticSameType2(__p_lhs, &__rhs); \
    bti_memcpy(__p_lhs, &__rhs, sizeOf$(TypeOf(*__p_lhs))); \
    __p_lhs; \
})
#define asg$a3(_p_lhs, _rhs...) \
    comp_inline__asg$a3(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$a3(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    a_claim_assert_staticSameType3(__p_lhs, &__rhs); \
    bti_memcpy(__p_lhs, &__rhs, sizeOf$(TypeOf(*__p_lhs))); \
    __p_lhs; \
})
#define asg$a4(_p_lhs, _rhs...) \
    comp_inline__asg$a4(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$a4(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    a_claim_assert_staticSameType4(__p_lhs, &__rhs); \
    bti_memcpy(__p_lhs, &__rhs, sizeOf$(TypeOf(*__p_lhs))); \
    __p_lhs; \
})

#define s_claim_assert_staticSameType(_lhs, _rhs...) s_claim_assert_staticSameType1(_lhs, _rhs)
#define s_claim_assert_staticSameType1(_lhs, _rhs...) \
    comp_inline__s_claim_assert_staticSameType1(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define comp_inline__s_claim_assert_staticSameType1(__lhs, __rhs, _lhs, _rhs...) ({ \
    let_(__lhs, TypeOf(_lhs)) = _lhs; \
    let_(__rhs, TypeOf(_rhs)) = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(__lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(__lhs)) == alignOf$(TypeOf(__rhs))); \
    claim_assert_static(isSameType$(TypeOf(ptr$s(__lhs)), TypeOf(ptr$s(__rhs)))); \
})
#define s_claim_assert_staticSameType2(_lhs, _rhs...) \
    comp_inline__s_claim_assert_staticSameType2(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define comp_inline__s_claim_assert_staticSameType2(__lhs, __rhs, _lhs, _rhs...) ({ \
    let_(__lhs, TypeOf(_lhs)) = _lhs; \
    let_(__rhs, TypeOf(_rhs)) = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(__lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(__lhs)) == alignOf$(TypeOf(__rhs))); \
    s_claim_assert_staticSameType1(ptr$s(__lhs)[0], ptr$s(__rhs)[0]); \
})
#define s_claim_assert_staticSameType3(_lhs, _rhs...) \
    comp_inline__s_claim_assert_staticSameType3(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define comp_inline__s_claim_assert_staticSameType3(__lhs, __rhs, _lhs, _rhs...) ({ \
    let_(__lhs, TypeOf(_lhs)) = _lhs; \
    let_(__rhs, TypeOf(_rhs)) = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(__lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(__lhs)) == alignOf$(TypeOf(__rhs))); \
    s_claim_assert_staticSameType2(ptr$s(__lhs)[0], ptr$s(__rhs)[0]); \
})
#define s_claim_assert_staticSameType4(_lhs, _rhs...) \
    comp_inline__s_claim_assert_staticSameType4(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define comp_inline__s_claim_assert_staticSameType4(__lhs, __rhs, _lhs, _rhs...) ({ \
    let_(__lhs, TypeOf(_lhs)) = _lhs; \
    let_(__rhs, TypeOf(_rhs)) = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(__lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(__lhs)) == alignOf$(TypeOf(__rhs))); \
    s_claim_assert_staticSameType3(ptr$s(__lhs)[0], ptr$s(__rhs)[0]); \
})

#define asg$s(_p_lhs, _rhs...) asg$s1(_p_lhs, _rhs)
#define asg$s1(_p_lhs, _rhs...) \
    comp_inline__asg$s1(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$s1(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    s_claim_assert_staticSameType1(*__p_lhs, __rhs); \
    *__p_lhs = *as$(TypeOf(__p_lhs), &__rhs); \
    __p_lhs; \
})
#define asg$s2(_p_lhs, _rhs...) \
    comp_inline__asg$s2(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$s2(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    s_claim_assert_staticSameType2(*__p_lhs, __rhs); \
    *__p_lhs = *as$(TypeOf(__p_lhs), &__rhs); \
    __p_lhs; \
})
#define asg$s3(_p_lhs, _rhs...) \
    comp_inline__asg$s3(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$s3(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    s_claim_assert_staticSameType3(*__p_lhs, __rhs); \
    *__p_lhs = *as$(TypeOf(__p_lhs), &__rhs); \
    __p_lhs; \
})
#define asg$s4(_p_lhs, _rhs...) \
    comp_inline__asg$s4(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$s4(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    s_claim_assert_staticSameType4(__lhs, __rhs); \
    *__p_lhs = *as$(TypeOf(__p_lhs), &__rhs); \
    __p_lhs; \
})

#define mutCast$p(_p...)   as$(const TypeOfUnqual(*_p)*, _p)
#define constCast$p(_p...) /* TODO: Implement */
#define mutCast$a(_a...)   (_a.as_const)
#define constCast$a(_a...) /* TODO: Implement */
#define mutCast$s(_s...)   (_s.as_const)
#define constCast$s(_s...) /* TODO: Implement */

#define buf$a(_a...) ((_a).buf)
#define ptr$a(_a...) (&*buf$a(_a))
#define ptr$s(_s...) ((_s).ptr)

#define len$a(_a...) countOf((_a).buf)
#define len$s(_s...) ((_s).len)

#define at$a(_a, _idx...) \
    pp_expand(pp_defer(block_inline__at$a)(param_expand__at$a(_a, _idx)))
#define param_expand__at$a(_a, _idx...)             pp_uniqTok(a), pp_uniqTok(idx), _a, _idx
#define block_inline__at$a(__a, __idx, _a, _idx...) ({ \
    let_(__a, TypeOf(&_a)) = &(_a); \
    let_(__idx, usize)     = (_idx); \
    claim_assert_static_msg(__builtin_constant_p(__idx) ? (__idx < len$a(*__a)) : true, "index out of bounds"); \
    debug_assert_fmt(__idx < len$a(*__a), "Index out of bounds: %zu >= %zu", __idx, len$a(*__a)); \
    &buf$a(*__a)[__idx]; \
})
#define at$s(_s, _idx...) \
    pp_expand(pp_defer(block_inline__at$s)(param_expand__at$s(_s, _idx)))
#define param_expand__at$s(_s, _idx...)             pp_uniqTok(s), pp_uniqTok(idx), _s, _idx
#define block_inline__at$s(__s, __idx, _s, _idx...) ({ \
    let_(__s, TypeOf(_s)) = (_s); \
    let_(__idx, usize)    = (_idx); \
    debug_assert_fmt(__idx < len$s(__s), "Index out of bounds: %zu >= %zu", __idx, len$s(__s)); \
    &ptr$s(__s)[__idx]; \
})

#define slice$p(/*(_p)(_begin:0, _end:1)*/...) /* TODO: Implement */
#define slice$p$()
#define slice$a(/*(_a)(_begin, _end)*/...) /* TODO: Implement */
#define slice$a$()
#define slice$s(/*(_s)(_begin, _end)*/...) /* TODO: Implement */
#define slice$s$()

#define init$o()
#define init$o$()
#define asg$o()
#define asg$o$()
#define some()
#define none()
#define isSome()
#define isNone()
#define unwrap_()
#define orelse_()

#define init$e()
#define init$e$()
#define asg$e()
#define asg$e$()
#define ok()
#define err()
#define isOk()
#define isErr()
#define try_()
#define catch_()
#define errdefer_()

// ============================================================================
// Generic Meta Types - The Foundation
// ============================================================================

typedef struct meta$p_const meta$p_const;
struct meta$p_const {
    TypeInfo type;
    union {
        p_const$raw ptr;
        p_const$raw raw;
    };
};
typedef union meta$p meta$p;
union meta$p {
    struct {
        TypeInfo type;
        union {
            p$raw ptr;
            p$raw raw;
        };
    };
    meta$p_const as_const;
};

typedef union meta$v_const meta$v_const;
union meta$v_const {
    struct {
        TypeInfo    inner_type;
        p_const$raw inner;
    };
    meta$p_const ref;
};

typedef union meta$v meta$v;
union meta$v {
    struct {
        TypeInfo inner_type;
        p$raw    inner;
    };
    meta$p       ref;
    meta$v_const as_const;
};

typedef struct meta$s_const meta$s_const;
struct meta$s_const {
    TypeInfo type;
    union {
        struct {
            p_const$raw ptr;
            usize       len;
        };
        s_const$raw raw;
    };
};
typedef union meta$s meta$s;
union meta$s {
    struct {
        TypeInfo type;
        union {
            struct {
                p$raw ptr;
                usize len;
            };
            s$raw raw;
        };
    };
    meta$s_const as_const;
};

typedef union meta$a_const meta$a_const;
union meta$a_const {
    struct {
        TypeInfo    inner_type;
        s_const$raw inner;
    };
    meta$s_const ref;
};

typedef union meta$a meta$a;
union meta$a {
    struct {
        TypeInfo inner_type;
        s$raw    inner;
    };
    meta$s_const ref;
    meta$a_const as_const;
};

typedef union meta$o meta$o;
union meta$o {
    struct {
        TypeInfo inner_type; // Type of T in Optional<T>
        o$raw*   inner;      // Points to {is_some, {T}} structure
    };
    meta$v as_value;
};

typedef union meta$e meta$e;
union meta$e {
    struct {
        TypeInfo inner_type; // Type of T in Result<T>
        e$raw*   inner;      // Points to {is_ok, {err, T}}} structure
    };
    meta$v as_value;
};

// ============================================================================
// Conversion Patterns - The Core Innovation
// ============================================================================

#define meta_ret$v(_T)     ((meta$v){ .inner_type = typeInfo$(_T), .inner = &((_T){}) })
#define meta_ret$a(_N, _T) ((meta$a){ .inner_type = typeInfo$(FieldTypeOf(a$(_N, _T), buf[0])), .inner = ref$a((a$(_N, _T)){}) })
#define meta_ret$o(_T)     ((meta$o){ .inner_type = typeInfo$(FieldTypeOf(o$(_T), payload->some)), .inner = ((o$(_T)){}).ref_raw })
#define meta_ret$e(_T)     ((meta$e){ .inner_type = typeInfo$(FieldTypeOf(e$(_T), payload->ok)), .inner = ((e$(_T)){}).ref_raw })

#define p_meta(_ptr...)    ((meta$p_const){ .type = typeInfo$(TypeOf(*_ptr)), .raw = (_ptr) })
#define p_metaMut(_ptr...) ((meta$p){ .type = typeInfo$(TypeOf(*_ptr)), .raw = (_ptr) })
#define s_meta(_sli...)    ((meta$s_const){ .type = typeInfo$(TypeOf(*(_sli).ptr)), .raw = (_sli).as_raw })
#define s_metaMut(_sli...) ((meta$s){ .type = typeInfo$(TypeOf(*(_sli).ptr)), .raw = (_sli).as_raw })

#define v_meta(_val...) ((meta$v){ .inner_type = typeInfo$(TypeOf(_val)), .inner = &(_val) })
#define a_meta(_arr...) ((meta$a){ .inner_type = typeInfo$(TypeOf(*(_arr).buf)), .inner = ref$a(_arr).as_raw })
#define o_meta(_opt...) ((meta$o){ .inner_type = typeInfo$(TypeOf((_opt).payload.some)), .inner = &(_opt).as_raw })
#define e_meta(_err...) ((meta$e){ .inner_type = typeInfo$(TypeOf((_err).payload.ok)), .inner = &(_err).as_raw })

#define meta$p$(/*(_T)(_Expr...)*/...) \
    pp_expand(pp_defer(block_inline__meta_p$)(param_expand__meta_p$ __VA_ARGS__))
#define param_expand__meta_p$(...)          __VA_ARGS__, pp_expand
#define block_inline__meta_p$(_T, _meta...) (*as$(p_const$(_T)*, &(_meta).raw))

#define meta_pMut$(/*(_T)(_Expr...)*/...) \
    pp_expand(pp_defer(block_inline__meta_pMut$)(param_expand__meta_pMut$ __VA_ARGS__))
#define param_expand__meta_pMut$(...)          __VA_ARGS__, pp_expand
#define block_inline__meta_pMut$(_T, _meta...) (*as$(p$(_T)*, &(_meta).raw))

#define meta_v$(/*(_T)(_Expr...)*/...) \
    pp_expand(pp_defer(block_inline__meta_v$)(param_expand__meta_v$ __VA_ARGS__))
#define param_expand__meta_v$(...)          __VA_ARGS__, pp_expand
#define block_inline__meta_v$(_T, _meta...) (*as$(_T*, (_meta).inner))

#define meta_s$(/*(_T)(_Expr...)*/...) \
    pp_expand(pp_defer(block_inline__meta_s$)(param_expand__meta_s$ __VA_ARGS__))
#define param_expand__meta_s$(...)          __VA_ARGS__, pp_expand
#define block_inline__meta_s$(_T, _meta...) (*as$(s_const$(_T)*, &(_meta).raw))

#define meta_sMut$(/*(_T)(_Expr...)*/...) \
    pp_expand(pp_defer(block_inline__meta_sMut$)(param_expand__meta_sMut$ __VA_ARGS__))
#define param_expand__meta_sMut$(...)          __VA_ARGS__, pp_expand
#define block_inline__meta_sMut$(_T, _meta...) (*as$(s$(_T)*, &(_meta).raw))

#define meta_a$(/*(_N, _T)(_Expr...)*/...) \
    pp_expand(pp_defer(block_inline__meta_a$)(param_expand__meta_a$ __VA_ARGS__))
#define param_expand__meta_a$(...)              __VA_ARGS__, pp_expand
#define block_inline__meta_a$(_N, _T, _meta...) (*as$(a$(_N, _T)*, (_meta).inner))

#define meta_o$(/*(_T)(_Expr...)*/...) \
    pp_expand(pp_defer(block_inline__meta_o$)(param_expand__meta_o$ __VA_ARGS__))
#define param_expand__meta_o$(...)          __VA_ARGS__, pp_expand
#define block_inline__meta_o$(_T, _meta...) (*as$(o$(_T)*, (_meta).inner))

#define meta_e$(/*(_T)(_Expr...)*/...) \
    pp_expand(pp_defer(block_inline__meta_e$)(param_expand__meta_e$ __VA_ARGS__))
#define param_expand__meta_e$(...)          __VA_ARGS__, pp_expand
#define block_inline__meta_e$(_T, _meta...) (*as$(e$(_T)*, (_meta).inner))

// ============================================================================
// Core Operations for Generic Programming
// ============================================================================

static inline meta$p_const meta_ref$v(meta$v_const self) {
    return (meta$p_const){ .type = self.inner_type, .ptr = self.inner };
}
static inline meta$p meta_refMut$v(meta$v self) {
    return (meta$p){ .type = self.inner_type, .ptr = self.inner };
}
static inline meta$v_const meta_deref$p(meta$p_const self) {
    return (meta$v_const){ .inner_type = self.type, .inner = self.ptr };
}
static inline meta$v meta_derefMut$p(meta$p self) {
    return (meta$v){ .inner_type = self.type, .inner = self.ptr };
}

static inline meta$p meta_zero$v(meta$p self) {
    debug_assert_nonnull(self.ptr);
    bti_memset(self.ptr, 0, self.type.size);
    return (meta$p){ .type = self.type, .ptr = self.ptr };
}
static inline meta$s meta_zero$a(meta$s self) {
    debug_assert_nonnull(self.ptr);
    bti_memset(self.ptr, 0, self.type.size * self.len);
    return (meta$s){ .type = self.type, .ptr = self.ptr, .len = self.len };
}

static inline meta$p meta_set$p(meta$p lhs_dest, meta$v_const rhs_src) {
    debug_assert(lhs_dest.type.size == rhs_src.inner_type.size);
    debug_assert(lhs_dest.type.align == rhs_src.inner_type.align);
    debug_assert_nonnull(lhs_dest.ptr);
    debug_assert_nonnull(rhs_src.inner);

    bti_memcpy(lhs_dest.ptr, rhs_src.inner, rhs_src.inner_type.size);
    return lhs_dest;
}
static inline meta$s meta_set$s(meta$s lhs_dest, meta$a_const rhs_src) {
    debug_assert(lhs_dest.type.size == rhs_src.inner_type.size);
    debug_assert(lhs_dest.type.align == rhs_src.inner_type.align);
    debug_assert_nonnull(lhs_dest.ptr);
    debug_assert_nonnull(rhs_src.inner.ptr);

    bti_memcpy(lhs_dest.ptr, rhs_src.inner.ptr, rhs_src.inner_type.size * rhs_src.inner.len);
    return lhs_dest;
}

static inline meta$p meta_asg$v(meta$p lhs_dest, meta$v_const rhs_src) {
    return meta_set$p(lhs_dest, rhs_src);
}
static inline meta$s meta_asg$a(meta$s lhs_dest, meta$a_const rhs_src) {
    return meta_set$s(lhs_dest, rhs_src);
}

static inline meta$p meta_copy$v(meta$p lhs_dest, meta$v_const rhs_src, usize len) {
    debug_assert(lhs_dest.type.size == rhs_src.inner_type.size);
    debug_assert(lhs_dest.type.align == rhs_src.inner_type.align);
    debug_assert_nonnull(lhs_dest.ptr);
    debug_assert_nonnull(rhs_src.inner);

    bti_memcpy(lhs_dest.ptr, rhs_src.inner, rhs_src.inner_type.size * len);
    return lhs_dest;
}
static inline meta$s meta_copy$a(meta$s lhs_dest, meta$a_const rhs_src) {
    debug_assert(lhs_dest.type.size == rhs_src.inner_type.size);
    debug_assert(lhs_dest.type.align == rhs_src.inner_type.align);
    debug_assert(lhs_dest.len == rhs_src.inner.len);
    debug_assert_nonnull(lhs_dest.ptr);
    debug_assert_nonnull(rhs_src.inner.ptr);

    bti_memcpy(lhs_dest.ptr, rhs_src.inner.ptr, rhs_src.inner_type.size * lhs_dest.len);
    return lhs_dest;
}

static inline void meta_swap$v(meta$p lhs, meta$p rhs) {
    debug_assert(lhs.type.size == rhs.type.size);
    debug_assert(lhs.type.align == rhs.type.align);
    debug_assert_nonnull(lhs.ptr);
    debug_assert_nonnull(rhs.ptr);

    let tmp = (meta$v){ .inner_type = lhs.type, .inner = bti_alloca(lhs.type.size) };
    meta_asg$v(tmp.ref, meta_deref$p(lhs.as_const));
    meta_asg$v(lhs, meta_deref$p(rhs.as_const));
    meta_asg$v(rhs, tmp.as_const);
}

// ============================================================================
// Slice Operations
// ============================================================================

static inline meta$p_const meta_at$s(meta$s_const self, usize index) {
    debug_assert(index < self.len);
    let elem_ptr = as$(const u8*, self.ptr) + (index * self.type.size);
    return (meta$p_const){ .type = self.type, .ptr = elem_ptr };
}

static inline meta$p meta_atMut$s(meta$s self, usize index) {
    debug_assert(index < self.len);
    let elem_ptr = as$(u8*, self.ptr) + (index * self.type.size);
    return (meta$p){ .type = self.type, .ptr = elem_ptr };
}

static inline meta$s_const meta_slice$s(meta$s_const self, usize begin, usize end) {
    debug_assert(begin < end);
    debug_assert(end <= self.len);
    return (meta$s_const){
        .type = self.type,
        .ptr  = as$(const u8*, self.ptr) + (begin * self.type.size),
        .len  = end - begin
    };
}

static inline meta$s meta_sliceMut$s(meta$s self, usize begin, usize end) {
    debug_assert(begin < end);
    debug_assert(end <= self.len);
    return (meta$s){
        .type = self.type,
        .ptr  = as$(u8*, self.ptr) + (begin * self.type.size),
        .len  = end - begin
    };
}

static inline bool meta_isSome$o(meta$o self) {
    return self.inner->is_some;
}
static inline bool meta_isNone$o(meta$o self) {
    return !self.inner->is_some;
}

static inline meta$p_const meta_unwrap$o(meta$o self) {
    debug_assert(meta_isSome$o(self));
    return (meta$p_const){ .type = self.inner_type, .ptr = self.inner->payload->some };
}
static inline meta$p meta_unwrapMut$o(meta$o self) {
    debug_assert(meta_isSome$o(self));
    return (meta$p){ .type = self.inner_type, .ptr = self.inner->payload->some };
}

static inline meta$o meta_asgNone$o(meta$o self) {
    self.inner->is_some = false;
    meta_zero$v(meta_unwrapMut$o(self));
    return self;
}
static inline meta$o meta_asgSome$o(meta$o self, meta$v_const value) {
    debug_assert(self.inner_type.size == value.inner_type.size);
    debug_assert(self.inner_type.align == value.inner_type.align);
    self.inner->is_some = true;
    meta_asg$v(meta_unwrapMut$o(self), value);
    return self;
}

static inline bool meta_isOk$e(meta$e self) {
    return self.inner->is_ok;
}
static inline bool meta_isErr$e(meta$e self) {
    return !self.inner->is_ok;
}

static inline meta$p_const meta_unwrapOk$e(meta$e self) {
    debug_assert(meta_isOk$e(self));
    return (meta$p_const){ .type = self.inner_type, .ptr = self.inner->payload->ok };
}
static inline meta$p meta_unwrapOkMut$e(meta$e self) {
    debug_assert(meta_isOk$e(self));
    return (meta$p){ .type = self.inner_type, .ptr = self.inner->payload->ok };
}

static inline const u32* meta_unwrapErr$e(meta$e self) {
    debug_assert(meta_isErr$e(self));
    return &self.inner->payload->err;
}
static inline u32* meta_unwrapErrMut$e(meta$e self) {
    debug_assert(meta_isErr$e(self));
    return &self.inner->payload->err;
}

static inline meta$e meta_asgOk$e(meta$e self, meta$v_const value) {
    debug_assert(self.inner_type.size == value.inner_type.size);
    debug_assert(self.inner_type.align == value.inner_type.align);
    self.inner->is_ok = true;
    meta_asg$v(meta_unwrapOkMut$e(self), value);
    return self;
}
static inline meta$e meta_asgErr$e(meta$e self, u32 err) {
    debug_assert(err != 0);
    self.inner->is_ok          = false;
    *meta_unwrapErrMut$e(self) = err;
    return self;
}
/* // Reverse a slice in place
static inline void s$reverse(s$meta slice) {
    if (slice.len <= 1) {
        return;
    }

    char* start = (char*)slice.ptr;
    char* end   = start + ((slice.len - 1) * slice.elem_type.size);

    // Use stack buffer for swapping
    if (slice.elem_type.size <= 256) {
        char temp[256];
        while (start < end) {
            mem_copy_bytes(temp, start, slice.elem_type.size);
            mem_copy_bytes(start, end, slice.elem_type.size);
            mem_copy_bytes(end, temp, slice.elem_type.size);
            start += slice.elem_type.size;
            end -= slice.elem_type.size;
        }
    } else {
        // Byte-by-byte swap for large elements
        for (usize i = 0; i < slice.len / 2; i++) {
            char* a = (char*)slice.ptr + (i * slice.elem_type.size);
            char* b = (char*)slice.ptr + ((slice.len - 1 - i) * slice.elem_type.size);
            for (usize j = 0; j < slice.elem_type.size; j++) {
                char temp = a[j];
                a[j]      = b[j];
                b[j]      = temp;
            }
        }
    }
}

// Find element in slice (returns index or SIZE_MAX if not found)
static inline usize s$find(s$meta slice, const void* value) {
    char* ptr = (char*)slice.ptr;
    for (usize i = 0; i < slice.len; i++) {
        if (mem_compare_bytes(ptr, value, slice.elem_type.size) == 0) {
            return i;
        }
        ptr += slice.elem_type.size;
    }
    return SIZE_MAX;
}

// Check if slice contains element
static inline bool s$contains(s$meta slice, const void* value) {
    return s$find(slice, value) != SIZE_MAX;
}

// Rotate slice left by n positions
static inline void s$rotate_left(s$meta slice, usize n) {
    if (slice.len <= 1 || n == 0) {
        return;
    }
    n = n % slice.len; // Handle n > len
    if (n == 0) {
        return;
    }

    // Create three slices: [0..n), [n..len), entire
    s$meta left  = s$range(slice, 0, n);
    s$meta right = s$range(slice, n, slice.len);

    // Reverse each part then reverse whole
    s$reverse(left);
    s$reverse(right);
    s$reverse(slice);
}

// Rotate slice right by n positions
static inline void s$rotate_right(s$meta slice, usize n) {
    if (slice.len <= 1 || n == 0) {
        return;
    }
    n = n % slice.len;
    s$rotate_left(slice, slice.len - n);
}
 */

/* // ============================================================================
// Optionalal Type Definition and Operations
// ============================================================================

// Generic optional structure (what o$T expands to)
#define DEFINE_OPTIONAL(_T) \
    typedef struct o$##_T { \
        bool is_some; \
        _T   value; \
    } o$##_T

// Calculate optional size/alignment from inner type
static inline TypeInfo o$type_info(TypeInfo inner) {
    usize offset = sizeOf$(bool) + padding_for(sizeOf$(bool), inner.align);
    return (TypeInfo){
        .size  = offset + inner.size,
        .align = inner.align > alignOf$(bool) ? inner.align : alignOf$(bool)
    };
}

// Generic optional operations - work with meta$o
static inline meta$o o$some(meta$o opt, const void* value) {
    bool* is_some = (bool*)opt.ptr;
    usize offset    = sizeOf$(bool) + padding_for(sizeOf$(bool), opt.inner_type.align);
    void* value_ptr = (char*)opt.ptr + offset;

    *is_some = true;
    mem_copy_bytes(value_ptr, value, opt.inner_type.size);
    return opt;
}

static inline meta$o o$none(meta$o opt) {
    bool* is_some = (bool*)opt.ptr;
    usize offset    = sizeOf$(bool) + padding_for(sizeOf$(bool), opt.inner_type.align);
    void* value_ptr = (char*)opt.ptr + offset;

    *is_some = false;
    mem_set_bytes(value_ptr, 0, opt.inner_type.size);
    return opt;
}

static inline bool o$is_some(meta$o opt) {
    return *(bool*)opt.ptr;
}

static inline meta o$unwrap(meta$o opt) {
    debug_assert(o$is_some(opt));
    usize offset    = sizeOf$(bool) + padding_for(sizeOf$(bool), opt.inner_type.align);
    void* value_ptr = (char*)opt.ptr + offset;
    return (meta){ .type = opt.inner_type, .ptr = value_ptr };
}

// ============================================================================
// Error Union Type Definition and Operations
// ============================================================================

typedef uint32_t ErrorCode;

// Generic error union structure (what e$T expands to)
#define DEFINE_ERROR_UNION(_T) \
    typedef struct e$##_T { \
        ErrorCode error; \
        _T        value; \
    } e$##_T

// Calculate error union size/alignment from inner type
static inline TypeInfo e$type_info(TypeInfo inner) {
    usize offset = sizeOf$(ErrorCode) + padding_for(sizeOf$(ErrorCode), inner.align);
    return (TypeInfo){
        .size  = offset + inner.size,
        .align = inner.align > alignOf$(ErrorCode) ? inner.align : alignOf$(ErrorCode)
    };
}

// Generic error union operations - work with meta$e
static inline meta$e e$ok(meta$e eu, const void* value) {
    ErrorCode* error     = (ErrorCode*)eu.ptr;
    usize      offset    = sizeOf$(ErrorCode) + padding_for(sizeOf$(ErrorCode), eu.inner_type.align);
    void*      value_ptr = (char*)eu.ptr + offset;

    *error = 0; // 0 = success
    mem_copy_bytes(value_ptr, value, eu.inner_type.size);
    return eu;
}

static inline meta$e e$err(meta$e eu, ErrorCode error) {
    debug_assert(error != 0); // 0 is reserved for success
    ErrorCode* error_ptr = (ErrorCode*)eu.ptr;
    usize      offset    = sizeOf$(ErrorCode) + padding_for(sizeOf$(ErrorCode), eu.inner_type.align);
    void*      value_ptr = (char*)eu.ptr + offset;

    *error_ptr = error;
    mem_set_bytes(value_ptr, 0, eu.inner_type.size);
    return eu;
}

static inline bool e$is_ok(meta$e eu) {
    return *(ErrorCode*)eu.ptr == 0;
}

static inline ErrorCode e$get_err(meta$e eu) {
    ErrorCode err = *(ErrorCode*)eu.ptr;
    debug_assert(err != 0);
    return err;
}

static inline meta e$unwrap(meta$e eu) {
    debug_assert(e$is_ok(eu));
    usize offset    = sizeOf$(ErrorCode) + padding_for(sizeOf$(ErrorCode), eu.inner_type.align);
    void* value_ptr = (char*)eu.ptr + offset;
    return (meta){ .type = eu.inner_type, .ptr = value_ptr };
} */


// ============================================================================
// Example Generic Data Structure Pattern
// ============================================================================

// Generic ArrayList operations that work with meta
typedef struct ArrList$raw {
    meta$s items;
    usize  cap;
} ArrList$raw;

#define ArrList$(_T...) pp_join($, ArrList, _T)
#define tpl$ArrList$(_T...) \
    typedef union ArrList$(_T) ArrList$(_T); \
    union ArrList$(_T) { \
        struct { \
            TypeInfo type; \
            s$(_T) items; \
            usize cap; \
        }; \
        ArrList$raw         as_raw; \
        ArrList$raw ref_raw $like_ptr; \
    }

tpl$p$(i32);
tpl$s$(i32);
tpl$ArrList$(i32);

static inline ArrList$raw ArrList_empty(TypeInfo type) {
    return (ArrList$raw){
        .items = {
            .type = type,
            .ptr  = null,
            .len  = 0,
        },
        .cap = 0,
    };
}
#define tpl$ArrList_empty$(_T...) \
    static ArrList$(_T) pp_join($, ArrList_empty, _T)(void) { \
        return (ArrList$(_T)){ .as_raw = ArrList_empty(typeInfo$(_T)) }; \
    }

// Example: Generic pop that returns optional
static inline meta$o ArrList_popOrNull(ArrList$raw* self, meta$o ret) {
    if (self->items.len == 0) {
        return meta_asgNone$o(ret);
    }
    self->items.len--;
    let last = meta_at$s(self->items.as_const, self->items.len);
    return meta_asgSome$o(ret, meta_deref$p(last));
}
#define tpl$ArrList_popOrNull$(_T...) \
    static o$(_T) pp_join($, ArrList_popOrNull, _T)(ArrList$(_T) * self) { \
        return meta_o$((_T)(ArrList_popOrNull(self->ref_raw, meta_ret$o(_T)))); \
    }

// Example: Generic get that returns error union
static inline meta$e ArrList_get(ArrList$raw* self, usize index, meta$e ret) {
    if (index >= self->items.len) {
        return meta_asgErr$e(ret, 1); // Error: index out of boundsZ
    }
    let elem = meta_at$s(self->items.as_const, index);
    return meta_asgOk$e(ret, meta_deref$p(elem));
}

static inline meta$v ArrList_getWithin(ArrList$raw* self, usize index, meta$v ret) {
    let elem = meta_at$s(self->items.as_const, index);
    return meta_derefMut$p(meta_asg$v(ret.ref, meta_deref$p(elem)));
}

tpl$o$(i32);
tpl$ArrList_empty$(i32);
tpl$ArrList_popOrNull$(i32);
void test() {
    var list = ArrList_empty$i32();
    $ignore  = meta_o$((i32)(ArrList_popOrNull(&list.as_raw, meta_ret$o(i32))));
    $ignore  = ArrList_popOrNull$i32(&list);
    $ignore  = meta_v$((i32)(ArrList_getWithin(&list.as_raw, 0, meta_ret$v(i32))));
}

/* // ============================================================================
// Usage Examples
// ============================================================================

// Define concrete optional and error types
DEFINE_OPTIONAL(int);
DEFINE_ERROR_UNION(int);

// Example of how users would use the API:
//
// // Generic function that returns optional
// meta$o some_generic_func(void* data, meta$o out) {
//     if (condition) {
//         int value = 42;
//         return o$some(out, &value);
//     }
//     return o$none(out);
// }
//
// // User code - no wrapper needed!
// o$int result;
// some_generic_func(data, TO_O_META(int, &result));
// if (o$is_some(TO_O_META(int, &result))) {
//     int value = CALL_META(int, o$unwrap(TO_O_META(int, &result)));
//     // use value...
// }
//
// // Or with a simple one-liner:
// o$int opt = CALL_O_META(o$int, some_generic_func(data, TO_O_META(int, &(o$int){})));

// Convenience macro for the full pattern
#define CALL_GENERIC_OPTIONAL(_T, _func, ...) \
    CALL_O_META(o$##_T, _func(__VA_ARGS__, TO_O_META(_T, &(o$##_T){})))

#define CALL_GENERIC_ERROR(_T, _func, ...) \
    CALL_E_META(e$##_T, _func(__VA_ARGS__, TO_E_META(_T, &(e$##_T){}))) */

#endif // META_OPS_H

typedef struct O$i32 {
    bool is_some;
    union {
        Void none;
        i32  some;
    } payload[1];
} O$i32;

typedef struct meta$O {
    bool is_some;
    union {
        union {
            Void   none;
            meta$v some;
        } payload[1];
        meta$v inner;
    };
} meta$O;

#define O_meta(_o...) ({ \
    let __o = ((TypeOf(_o)[1]){ [0] = _o }); \
    ((meta$O){ .is_some = __o->is_some, .payload = { { .some = v_meta(__o->payload->some) } } }); \
})

#include <stdio.h>

int main(void) {
    var o     = (O$i32){ .is_some = true, .payload = { { .some = 123 } } };
    let meta  = O_meta(o);
    let value = meta_v$((i32)(meta.payload->some));
    printf("o.some: %p\n", &o.payload->some);
    printf("meta.some: %p\n", meta.payload->some.inner);
    printf("value: %d\n", value);
    return 0;
}
