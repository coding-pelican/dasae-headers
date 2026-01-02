#ifndef META_OPS_H
#define META_OPS_H

#include "dh/core.h"
#include "dh/Range.h"
#include "dh/core/pp/common.h"

/* Raw Types =========================================================*/

typedef const void* P_const$raw;
typedef void* P$raw;

typedef union V$raw {
    u8 inner $flexible;
} V$raw;

typedef struct S_const$raw {
    P_const$raw ptr;
    usize len;
} S_const$raw;
typedef union S$raw {
    struct {
        P$raw ptr;
        usize len;
    };
    S_const$raw as_const;
} S$raw;

typedef struct A$raw {
    u8 inner $flexible;
} A$raw;

typedef union O_Payload$raw {
    Void none;
    u8 some $flexible;
} O_Payload$raw;
typedef struct O$raw {
    bool is_some;
    union {
        Void none;
        u8 some $flexible;
        O_Payload$raw raw[1];
    } payload[1];
} O$raw;

typedef i32 ErrCode;
enum ErrCode {
    ErrCode_Unknown = -6,
    ErrCode_Unexpected = -5,
    ErrCode_Unspecified = -4,
    ErrCode_Unsupported = -3,
    ErrCode_NotImplemented = -2,
    ErrCode_InvalidArgument = -1,
    ErrCode_None = 0
};
typedef struct E_VT E_VT;
typedef struct Err {
    ErrCode ctx;
    const E_VT* vt;
} Err;

typedef union E_Payload$raw E_Payload$raw;
union E_Payload$raw {
    u32 err;
    u8 ok $flexible;
};

typedef struct E$raw E$raw;
struct E$raw {
    bool is_ok;
    union {
        u32 err;
        u8 ok $flexible;
        E_Payload$raw raw[1];
    } payload[1];
};

/* Base Types ========================================================*/

#define $P_const$(_T...) TypeOf(const _T*)
#define $P$(_T...) TypeOf(_T*)
#define $A$(_N, _T...) TypeOf(_T[_N])

/* Pointer Type ======================================================*/

/* Pointer Anonymous */
#define P_const$$(_T...) $P_const$(_T)
#define P$$(_T...) $P$(_T)
/* Pointer Alias */
#define P_const$(_T...) pp_join($, P_const, _T)
#define P$(_T...) pp_join($, P, _T)
/* Pointer Template */
#define tpl$P$(_T...) \
    typedef $P_const$(_T) P_const$(_T); \
    typedef $P$(_T) P$(_T)

/* Slice Type ========================================================*/

/* Slice Anonymous */
#define S_const$$(_T...) \
    union { \
        struct { \
            var_(ptr, P_const$$(_T)); \
            var_(len, usize); \
        }; \
        var_(as_raw, S_const$raw); \
        var_(ref_raw, TypeOf(S_const$raw $like_ref)); \
    }
#define S$$(_T...) gen__S$$(_T)
#define gen__S$$(_T...) \
    union { \
        struct { \
            var_(ptr, P$$(_T)); \
            var_(len, usize); \
        }; \
        var_(as_raw, S$raw); \
        var_(ref_raw, TypeOf(S$raw $like_ref)); \
        var_(as_const, S_const$$(_T)); \
    }
/* Slice Alias */
#define S_const$(_T...) pp_join($, S_const, _T)
#define S$(_T...) pp_join($, S, _T)
/* Slice Template */
#define tpl$S$(_T...) \
    typedef union S_const$(_T) { \
        struct { \
            var_(ptr, P_const$(_T)); \
            var_(len, usize); \
        }; \
        var_(as_raw, S_const$raw); \
        var_(ref_raw, TypeOf(S_const$raw $like_ref)); \
    } S_const$(_T); \
    typedef union S$(_T) { \
        struct { \
            var_(ptr, P$(_T)); \
            var_(len, usize); \
        }; \
        var_(as_raw, S$raw); \
        var_(ref_raw, TypeOf(S$raw $like_ref)); \
        var_(as_const, S_const$(_T)); \
    } S$(_T)

/* Array Type ========================================================*/

/* Array Anonymous */
#define A$$(_N, _T...) \
    union { \
        var_(val, $A$(_N, _T)); \
        var_(as_raw, A$raw); \
        var_(ref_raw, TypeOf(A$raw $like_ref)); \
    }
/* Array Alias */
#define A$(_N, _T...) pp_join3($, A, _N, _T)
/* Array Template */
#define tpl$A$(_N, _T...) \
    typedef union A$(_N, _T) { \
        var_(val, $A$(_N, _T)); \
        var_(as_raw, A$raw); \
        var_(ref_raw, TypeOf(A$raw $like_ref)); \
    } A$(_N, _T)

/* Optional Type =====================================================*/

/* Optional Anonymous */
#define O$$(_T...) \
    union { \
        struct { \
            var_(is_some, bool); \
            TypeOf(union { \
                var_(none, Void); \
                var_(some, _T); \
                var_(raw, TypeOf(O_Payload$raw $like_ref)); \
            } $like_ref) payload; \
        }; \
        var_(as_raw, O$raw); \
        var_(ref_raw, TypeOf(O$raw $like_ref)); \
    }
/* Optional Alias */
#define O$(_T...) pp_join($, O, _T)
/* Optional Template */
#define tpl$O$(_T...) \
    typedef union O$(_T) { \
        struct { \
            var_(is_some, bool); \
            TypeOf(union { \
                var_(none, Void); \
                var_(some, _T); \
                var_(raw, TypeOf(O_Payload$raw $like_ref)); \
            } $like_ref) payload; \
        }; \
        var_(as_raw, O$raw); \
        var_(ref_raw, TypeOf(O$raw $like_ref)); \
    } O$(_T)

/* Result Type =======================================================*/

/* Result Anonymous */
#define E$$(_T...) \
    union { \
        struct { \
            var_(is_ok, bool); \
            TypeOf(union { \
                var_(err, Err); \
                var_(ok, _T); \
                var_(raw, TypeOf(E_Payload$raw $like_ref)); \
            } $like_ref) payload; \
        }; \
        var_(as_raw, E$raw); \
        var_(ref_raw, TypeOf(E$raw $like_ref)); \
    }
/* Result Alias */
#define E$(_T...) pp_join($, E, _T)
/* Result Template */
#define tpl$E$(_T...) \
    typedef union E$(_T) { \
        struct { \
            var_(is_ok, bool); \
            TypeOf(union { \
                var_(err, Err); \
                var_(ok, _T); \
                var_(raw, TypeOf(E_Payload$raw $like_ref)); \
            } $like_ref) payload; \
        }; \
        var_(as_raw, E$raw); \
        var_(ref_raw, TypeOf(E$raw $like_ref)); \
    } E$(_T)

/* Operations ========================================================*/

#define zero$A() { .val = { 0 } }
#define zero$A$(/*(_N,_T)*/... /*(A$(_N,_T))*/) ((A$ __VA_ARGS__)zero$A())
#define zero$A$$(/*(_N,_T)*/... /*(A$$(_N,_T))*/) ((A$$ __VA_ARGS__)zero$A())
#define zero$S() { .ptr = null, .len = 0 }
#define zero$S$(/*(_T)*/... /*(S$(_T))*/) ((S$ __VA_ARGS__)zero$S())
#define zero$S$$(/*(_T)*/... /*(S$$(_T))*/) ((S$$ __VA_ARGS__)zero$S())
#define init$A(_initial...) { .val = _initial }
#define init$A$(/*(_N, _T)(_initial...)*/... /*(A$(_N,_T))*/) \
    __step_inline__init$A$(pp_defer(__emit_inline__init$A$)(__param_parse__init$A$ __VA_ARGS__))
#define __step_inline__init$A$(...) __VA_ARGS__
#define __param_parse__init$A$(...) __VA_ARGS__, __param_next__init$A$
#define __param_next__init$A$(...) __VA_ARGS__
#define __emit_inline__init$A$(_N, _T, _initial...) ((A$(_N, _T))init$A(_initial))
#define init$A$$(/*(_N, _T)(_initial...)*/... /*(A$$(_N,_T))*/) \
    __step_inline__init$A$$(pp_defer(__emit_inline__init$A$$)(__param_parse__init$A$$ __VA_ARGS__))
#define __step_inline__init$A$$(...) __VA_ARGS__
#define __param_parse__init$A$$(...) __VA_ARGS__, __param_next__init$A$$
#define __param_next__init$A$$(...) __VA_ARGS__
#define __emit_inline__init$A$$(_N, _T, _initial...) ((A$$(_N, _T))init$A(_initial))
#define init$S(_ptr, _len...) { .ptr = _ptr, .len = _len }
#define init$S$(/*(_T)(_ptr: P$$(_T), _len: usize)*/... /*(S$(_T))*/) \
    __step_inline__init$S$(pp_defer(__emit_inline__init$S$)(__param_parse__init$S$ __VA_ARGS__))
#define __step_inline__init$S$(...) __VA_ARGS__
#define __param_parse__init$S$(...) __VA_ARGS__, __param_next__init$S$
#define __param_next__init$S$(...) __VA_ARGS__
#define __emit_inline__init$S$(_T, _ptr, _len...) ((S$(_T))init$S(_ptr, _len))
#define init$S$$(/*(_T)(_ptr: P$$(_T), _len: usize)*/... /*(S$$(_T))*/) \
    __step_inline__init$S$$(pp_defer(__emit_inline__init$S$$)(__param_parse__init$S$$ __VA_ARGS__))
#define __step_inline__init$S$$(...) __VA_ARGS__
#define __param_parse__init$S$$(...) __VA_ARGS__, __param_next__init$S$$
#define __param_next__init$S$$(...) __VA_ARGS__
#define __emit_inline__init$S$$(_T, _ptr, _len...) ((S$$(_T))init$S(_ptr, _len))

#define ref$V(_v /*: T*/... /*P$$(_T)*/) ref(_v)
#define ref$A(_a /*: A$$(_N,_T)*/... /*(S$$(_T))*/) ((S$$(TypeOf(*ptr$A(_a)))){ \
    .ptr = ptr$A(_a), \
    .len = len$A(_a), \
})
#define ref$A$(/*(_T)(_a: A$$(_N,_T))*/... /*(S$(_T))*/) \
    __step_inline__ref$A$(pp_defer(__emit_inline__ref$A$)(__param_parse__ref$A$ __VA_ARGS__))
#define __step_inline__ref$A$(...) __VA_ARGS__
#define __param_parse__ref$A$(...) __VA_ARGS__, __param_next__ref$A$
#define __param_next__ref$A$(...) __VA_ARGS__
#define __emit_inline__ref$A$(_T, _a...) ((S$(_T)){ \
    .ptr = ptr$A(_a), \
    .len = len$A(_a), \
})

#define deref$P(_p /*: P$$(_T)*/... /*(_T)*/) deref(_p)
#define deref$S$(/*(_N, _T)(_s: S$$(_T))*/... /*(A$(_N,_T))*/) \
    __step_inline__deref$S$(pp_defer(__emit_inline__deref$S$)(__param_parse__deref$S$ __VA_ARGS__))
#define __step_inline__deref$S$(...) __VA_ARGS__
#define __param_parse__deref$S$(...) __VA_ARGS__, __param_next__deref$S$
#define __param_next__deref$S$(...) __VA_ARGS__
#define __emit_inline__deref$S$(_N, _T, __s, _s...) (*({ \
    let_(__s, TypeOf(_s)) = _s; \
    debug_assert_fmt(len$S(__s) == _N, "length mismatch: len(%zu) != N(%zu)", len$S(__s), _N); \
    as$(A$(_N, _T) (*)(ensureNonnull(ptr$S(__s)))); \
}))
#define deref$S$$(/*(_N, _T)(_s: S$$(_T))*/... /*(A$$(_N,_T))*/) \
    __step_inline__deref$S$$(pp_defer(__emit_inline__deref$S$$)(__param_parse__deref$S$$ __VA_ARGS__))
#define __step_inline__deref$S$$(...) __VA_ARGS__
#define __param_parse__deref$S$$(...) __VA_ARGS__, __param_next__deref$S$$
#define __param_next__deref$S$$(...) __VA_ARGS__
#define __emit_inline__deref$S$$(_N, _T, __s...) (*({ \
    let_(__s, TypeOf(_s)) = _s; \
    debug_assert_fmt(len$S(__s) == _N, "length mismatch: len(%zu) != N(%zu)", len$S(__s), _N); \
    as$(A$$(_N, _T) (*)(ensureNonnull(ptr$S(__s)))); \
}))

/* #define a_claim_assert_staticSameType(_p_lhs, _p_rhs...) a_claim_assert_staticSameType1(_p_lhs, _p_rhs)
#define a_claim_assert_staticSameType1(_p_lhs, _p_rhs...) \
    comp_inline__a_claim_assert_staticSameType1(pp_uniqTok(p_lhs), pp_uniqTok(p_rhs), _p_lhs, _p_rhs)
#define comp_inline__a_claim_assert_staticSameType1(__p_lhs, __p_rhs, _p_lhs, _p_rhs...) ({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    let_(__p_rhs, TypeOf(&*_p_rhs)) = &*(_p_rhs); \
    claim_assert_static(len$A(*__p_lhs) == len$A(*__p_rhs)); \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(*__p_rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(*__p_rhs))); \
    claim_assert_static(isSameTypE$(TypeOf(val$A(*__p_lhs)), TypeOf(val$A(*__p_rhs)))); \
})
#define a_claim_assert_staticSameType2(_p_lhs, _p_rhs...) \
    comp_inline__a_claim_assert_staticSameType2(pp_uniqTok(p_lhs), pp_uniqTok(p_rhs), _p_lhs, _p_rhs)
#define comp_inline__a_claim_assert_staticSameType2(__p_lhs, __p_rhs, _p_lhs, _p_rhs...) ({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    let_(__p_rhs, TypeOf(&*_p_rhs)) = &*(_p_rhs); \
    claim_assert_static(len$A(*__p_lhs) == len$A(*__p_rhs)); \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(*__p_rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(*__p_rhs))); \
    a_claim_assert_staticSameType1(&val$A(*__p_lhs)[0], &val$A(*__p_rhs)[0]); \
})
#define a_claim_assert_staticSameType3(_p_lhs, _p_rhs...) \
    comp_inline__a_claim_assert_staticSameType3(pp_uniqTok(p_lhs), pp_uniqTok(p_rhs), _p_lhs, _p_rhs)
#define comp_inline__a_claim_assert_staticSameType3(__p_lhs, __p_rhs, _p_lhs, _p_rhs...) ({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    let_(__p_rhs, TypeOf(&*_p_rhs)) = &*(_p_rhs); \
    claim_assert_static(len$A(*__p_lhs) == len$A(*__p_rhs)); \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(*__p_rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(*__p_rhs))); \
    a_claim_assert_staticSameType2(&val$A(*__p_lhs)[0], &val$A(*__p_rhs)[0]); \
})
#define a_claim_assert_staticSameType4(_p_lhs, _p_rhs...) \
    comp_inline__a_claim_assert_staticSameType4(pp_uniqTok(p_lhs), pp_uniqTok(p_rhs), _p_lhs, _p_rhs)
#define comp_inline__a_claim_assert_staticSameType4(__p_lhs, __p_rhs, _p_lhs, _p_rhs...) ({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    let_(__p_rhs, TypeOf(&*_p_rhs)) = &*(_p_rhs); \
    claim_assert_static(len$A(*__p_lhs) == len$A(*__p_rhs)); \
    claim_assert_static(sizeOf$(TypeOf(*__p_lhs)) == sizeOf$(TypeOf(*__p_rhs))); \
    claim_assert_static(alignOf$(TypeOf(*__p_lhs)) == alignOf$(TypeOf(*__p_rhs))); \
    a_claim_assert_staticSameType3(&val$A(*__p_lhs)[0], &val$A(*__p_rhs)[0]); \
})

#define asg$A(_p_lhs, _rhs...) asg$A1(_p_lhs, _rhs)
#define asg$A1(_p_lhs, _rhs...) \
    comp_inline__asg$A1(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$A1(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    a_claim_assert_staticSameType1(__p_lhs, &__rhs); \
    prim_memcpy(__p_lhs, &__rhs, sizeOf$(TypeOf(*__p_lhs))); \
    __p_lhs; \
})
#define asg$A2(_p_lhs, _rhs...) \
    comp_inline__asg$A2(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$A2(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    a_claim_assert_staticSameType2(__p_lhs, &__rhs); \
    prim_memcpy(__p_lhs, &__rhs, sizeOf$(TypeOf(*__p_lhs))); \
    __p_lhs; \
})
#define asg$A3(_p_lhs, _rhs...) \
    comp_inline__asg$A3(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$A3(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    a_claim_assert_staticSameType3(__p_lhs, &__rhs); \
    prim_memcpy(__p_lhs, &__rhs, sizeOf$(TypeOf(*__p_lhs))); \
    __p_lhs; \
})
#define asg$A4(_p_lhs, _rhs...) \
    comp_inline__asg$A4(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$A4(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    a_claim_assert_staticSameType4(__p_lhs, &__rhs); \
    prim_memcpy(__p_lhs, &__rhs, sizeOf$(TypeOf(*__p_lhs))); \
    __p_lhs; \
}) */

/* #define s_claim_assert_staticSameType(_lhs, _rhs...) s_claim_assert_staticSameType1(_lhs, _rhs)
#define s_claim_assert_staticSameType1(_lhs, _rhs...) \
    comp_inline__s_claim_assert_staticSameType1(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define comp_inline__s_claim_assert_staticSameType1(__lhs, __rhs, _lhs, _rhs...) ({ \
    let_(__lhs, TypeOf(_lhs)) = _lhs; \
    let_(__rhs, TypeOf(_rhs)) = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(__lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(__lhs)) == alignOf$(TypeOf(__rhs))); \
    claim_assert_static(isSameTypE$(TypeOf(ptr$S(__lhs)), TypeOf(ptr$S(__rhs)))); \
})
#define s_claim_assert_staticSameType2(_lhs, _rhs...) \
    comp_inline__s_claim_assert_staticSameType2(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define comp_inline__s_claim_assert_staticSameType2(__lhs, __rhs, _lhs, _rhs...) ({ \
    let_(__lhs, TypeOf(_lhs)) = _lhs; \
    let_(__rhs, TypeOf(_rhs)) = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(__lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(__lhs)) == alignOf$(TypeOf(__rhs))); \
    s_claim_assert_staticSameType1(ptr$S(__lhs)[0], ptr$S(__rhs)[0]); \
})
#define s_claim_assert_staticSameType3(_lhs, _rhs...) \
    comp_inline__s_claim_assert_staticSameType3(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define comp_inline__s_claim_assert_staticSameType3(__lhs, __rhs, _lhs, _rhs...) ({ \
    let_(__lhs, TypeOf(_lhs)) = _lhs; \
    let_(__rhs, TypeOf(_rhs)) = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(__lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(__lhs)) == alignOf$(TypeOf(__rhs))); \
    s_claim_assert_staticSameType2(ptr$S(__lhs)[0], ptr$S(__rhs)[0]); \
})
#define s_claim_assert_staticSameType4(_lhs, _rhs...) \
    comp_inline__s_claim_assert_staticSameType4(pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs)
#define comp_inline__s_claim_assert_staticSameType4(__lhs, __rhs, _lhs, _rhs...) ({ \
    let_(__lhs, TypeOf(_lhs)) = _lhs; \
    let_(__rhs, TypeOf(_rhs)) = _rhs; \
    claim_assert_static(sizeOf$(TypeOf(__lhs)) == sizeOf$(TypeOf(__rhs))); \
    claim_assert_static(alignOf$(TypeOf(__lhs)) == alignOf$(TypeOf(__rhs))); \
    s_claim_assert_staticSameType3(ptr$S(__lhs)[0], ptr$S(__rhs)[0]); \
})

#define asg$S(_p_lhs, _rhs...) asg$S1(_p_lhs, _rhs)
#define asg$S1(_p_lhs, _rhs...) \
    comp_inline__asg$S1(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$S1(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    s_claim_assert_staticSameType1(*__p_lhs, __rhs); \
    *__p_lhs = *as$(TypeOf(__p_lhs)()(&__rhs)); \
    __p_lhs; \
})
#define asg$S2(_p_lhs, _rhs...) \
    comp_inline__asg$S2(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$S2(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    s_claim_assert_staticSameType2(*__p_lhs, __rhs); \
    *__p_lhs = *as$(TypeOf(__p_lhs)()(&__rhs)); \
    __p_lhs; \
})
#define asg$S3(_p_lhs, _rhs...) \
    comp_inline__asg$S3(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$S3(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    s_claim_assert_staticSameType3(*__p_lhs, __rhs); \
    *__p_lhs = *as$(TypeOf(__p_lhs)()(&__rhs)); \
    __p_lhs; \
})
#define asg$S4(_p_lhs, _rhs...) \
    comp_inline__asg$S4(pp_uniqTok(p_lhs), pp_uniqTok(rhs), _p_lhs, _rhs)
#define comp_inline__asg$S4(__p_lhs, __rhs, _p_lhs, _rhs...) &*({ \
    let_(__p_lhs, TypeOf(&*_p_lhs)) = &*(_p_lhs); \
    var_(__rhs, TypeOf(_rhs))       = _rhs; \
    s_claim_assert_staticSameType4(__lhs, __rhs); \
    *__p_lhs = *as$(TypeOf(__p_lhs)()(&__rhs)); \
    __p_lhs; \
}) */

#define mutCast$P(_p /*: P$$(_T)*/... /*(P_const $$(_T))*/) as$(const TypeOfUnqual (*_p) (*)(_p))
#define constCast$P(_p /*: P_const$$(_T)*/... /*(P$$(_T))*/) /* TODO: Implement */
#define mutCast$S(_s /*: S$$(_T)*/... /*(S_const$$(_T))*/) (_s.as_const)
#define constCast$S(_s /*: S_const$$(_T)*/... /*(S$$(_T))*/) /* TODO: Implement */

#define val$A(_a /*: A$$(_N,_T)*/... /*($A$(_N,_T))*/) ((_a).val)
#define ptr$A(_a /*: A$$(_N,_T)*/... /*(P$$(_T))*/) (&*val$A(_a))
#define ptr$S(_s /*: S$$(_T)*/... /*(P$$(_T))*/) ((_s).ptr)

#define len$A(_a /*: A$$(_N,_T)*/... /*(usize)*/) countOf$(TypeOf((_a).val))
#define len$S(_s /*: S$$(_T)*/... /*(usize)*/) ((_s).len)

#define at$A(_a /*: A$$(_N,_T)*/, _idx /*: usize*/... /*(P$$(_T))*/) \
    pp_expand(pp_defer(block_inline__at$A)(param_expand__at$A(_a, _idx)))
#define param_expand__at$A(_a, _idx...) pp_uniqTok(a), pp_uniqTok(idx), _a, _idx
#define block_inline__at$A(__a, __idx, _a, _idx...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__idx, usize) = _idx; \
    claim_assert_static_msg(__builtin_constant_p(__idx) ? (__idx < len$A(*__a)) : true, "index out of bounds"); \
    debug_assert_fmt(__idx < len$A(*__a), "Index out of bounds: idx(%zu) >= len(%zu)", __idx, len$A(*__a)); \
    &val$A(*__a)[__idx]; \
})
#define at$S(_s /*: S$$(_T)*/, _idx /*: usize*/... /*(P$$(_T))*/) \
    pp_expand(pp_defer(block_inline__at$S)(param_expand__at$S(_s, _idx)))
#define param_expand__at$S(_s, _idx...) pp_uniqTok(s), pp_uniqTok(idx), _s, _idx
#define block_inline__at$S(__s, __idx, _s, _idx...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__idx, usize) = _idx; \
    debug_assert_fmt(__idx < len$S(__s), "Index out of bounds: idx(%zu) >= len(%zu)", __idx, len$S(__s)); \
    &ptr$S(__s)[__idx]; \
})

#define slice$P(/*_p: P$$(_T), $r(_begin:0, _end:1): R*/... /*(S$$(_T))*/) __param_expand__slice$P(__VA_ARGS__)
#define __param_expand__slice$P(_p, _range...) __block_inline__slice$P(pp_uniqTok(p), pp_uniqTok(range), _p, _range)
#define __block_inline__slice$P(__p, __range, _p, _range...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= 1, "Invalid slice range: end(%zu) > 1", __range.end); \
    init$S$$((TypeOf(*__p))(__p, len$R(__range))); \
})
#define slice$P$(/*(_T)(_p: P$$(_T), $r(_begin:0, _end:1): R)*/... /*(S$(_T))*/) pp_expand(pp_defer(__block_inline__slice$P$)(__param_expand__slice$P$ __VA_ARGS__))
#define __param_expand__slice$P$(...) __VA_ARGS__, pp_uniqTok(p), pp_uniqTok(range), __param_next__slice$P$
#define __param_next__slice$P$(...) __VA_ARGS__
#define __block_inline__slice$P$(_T, __p, __range, _p, _range...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= 1, "Invalid slice range: end(%zu) > 1", __range.end); \
    init$S$((_T)(__p, len$R(__range))); \
})
#define prefix$P(/*_p: P$$(_T), _end: usize*/... /*(S$$(_T))*/) __param_expand__prefix$P(__VA_ARGS__)
#define __param_expand__prefix$P(_p, _end...) __block_inline__prefix$P(pp_uniqTok(p), pp_uniqTok(end), _p, _end)
#define __block_inline__prefix$P(__p, __end, _p, _end...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__end, usize) = _end; \
    debug_assert_fmt(__end <= 1, "Invalid slice range: end(%zu) > 1", __end); \
    init$S$$((TypeOf(*__p))(__p, __end)); \
})
#define prefix$P$(/*(_T)(_p: P$$(_T), _end: usize)*/... /*(S$(_T))*/) pp_expand(pp_defer(__block_inline__prefix$P$)(__param_expand__prefix$P$ __VA_ARGS__))
#define __param_expand__prefix$P$(...) __VA_ARGS__, pp_uniqTok(p), pp_uniqTok(end), __param_next__prefix$P$
#define __param_next__prefix$P$(...) __VA_ARGS__
#define __block_inline__prefix$P$(_T, __p, __end, _p, _end...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__end, usize) = _end; \
    debug_assert_fmt(__end <= 1, "Invalid slice range: end(%zu) > 1", __end); \
    init$S$((_T)(__p, __end)); \
})
#define suffix$P(/*_p: P$$(_T), _begin: usize*/... /*(S$$(_T))*/) __param_expand__suffix$P(__VA_ARGS__)
#define __param_expand__suffix$P(_p, _begin...) __block_inline__suffix$P(pp_uniqTok(p), pp_uniqTok(begin), _p, _begin)
#define __block_inline__suffix$P(__p, __begin, _p, _begin...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= 1, "Invalid slice range: begin(%zu) > 1", __begin); \
    init$S$$((TypeOf(*__p))(__p, __begin)); \
})
#define suffix$P$(/*(_T)(_p: P$$(_T), _begin: usize)*/... /*(S$(_T))*/) pp_expand(pp_defer(__block_inline__suffix$P$)(__param_expand__suffix$P$ __VA_ARGS__))
#define __param_expand__suffix$P$(...) __VA_ARGS__, pp_uniqTok(p), pp_uniqTok(begin), __param_next__suffix$P$
#define __param_next__suffix$P$(...) __VA_ARGS__
#define __block_inline__suffix$P$(_T, __p, __begin, _p, _begin...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= 1, "Invalid slice range: begin(%zu) > 1", __begin); \
    init$S$((_T)(__p, __begin)); \
})

#define slice$A(/*_a: A$$(_N,_T), $r(_begin, _end): R*/... /*(S$$(_T))*/) __param_expand__slice$A(__VA_ARGS__)
#define __param_expand__slice$A(_a, _range...) __block_inline__slice$A(pp_uniqTok(a), pp_uniqTok(range), _a, _range)
#define __block_inline__slice$A(__a, __range, _a, _range...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$A(*__a)); \
    init$S$$((TypeOf(*ptr$A(*__a)))(&val$A(*__a)[__range.begin], len$R(__range))); \
})
#define slice$A$(/*(_T)(_a: A$$(_N,_T), $r(_begin, _end): R)*/... /*(S$(_T))*/) pp_expand(pp_defer(__block_inline__slice$A$)(__param_expand__slice$A$ __VA_ARGS__))
#define __param_expand__slice$A$(...) __VA_ARGS__, pp_uniqTok(a), pp_uniqTok(range), __param_next__slice$A$
#define __param_next__slice$A$(...) __VA_ARGS__
#define __block_inline__slice$A$(_T, __a, __range, _a, _range...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$A(*__a)); \
    init$S$((_T)(&val$A(*__a)[__range.begin], len$R(__range))); \
})
#define prefix$A(/*_a: A$$(_N,_T), _end: usize*/... /*(S$$(_T))*/) __param_expand__prefix$A(__VA_ARGS__)
#define __param_expand__prefix$A(_a, _end...) __block_inline__prefix$A(pp_uniqTok(a), pp_uniqTok(end), _a, _end)
#define __block_inline__prefix$A(__a, __end, _a, _end...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__end, usize) = _end; \
    debug_assert_fmt(__end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __end, len$A(*__a)); \
    init$S$$((TypeOf(*ptr$A(*__a)))(ptr$A(*__a), __end)); \
})
#define prefix$A$(/*(_T)(_a: A$$(_N,_T), _end: usize)*/... /*(S$(_T))*/) pp_expand(pp_defer(__block_inline__prefix$A$)(__param_expand__prefix$A$ __VA_ARGS__))
#define __param_expand__prefix$A$(...) __VA_ARGS__, pp_uniqTok(a), pp_uniqTok(end), __param_next__prefix$A$
#define __param_next__prefix$A$(...) __VA_ARGS__
#define __block_inline__prefix$A$(_T, __a, __end, _a, _end...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__end, usize) = _end; \
    debug_assert_fmt(__end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __end, len$A(*__a)); \
    init$S$((_T)(ptr$A(*__a), __end)); \
})
#define suffix$A(/*_a: A$$(_N,_T), _begin: usize*/... /*(S$$(_T))*/) __param_expand__suffix$A(__VA_ARGS__)
#define __param_expand__suffix$A(_a, _begin...) __block_inline__suffix$A(pp_uniqTok(a), pp_uniqTok(begin), _a, _begin)
#define __block_inline__suffix$A(__a, __begin, _a, _begin...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= len$A(*__a), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$A(*__a)); \
    init$S$$((TypeOf(*ptr$A(*__a)))(ptr$A(*__a) + __begin, len$A(*__a) - __begin)); \
})
#define suffix$A$(/*(_T)(_a: A$$(_N,_T), _begin: usize)*/... /*(S$(_T))*/) pp_expand(pp_defer(__block_inline__suffix$A$)(__param_expand__suffix$A$ __VA_ARGS__))
#define __param_expand__suffix$A$(...) __VA_ARGS__, pp_uniqTok(a), pp_uniqTok(begin), __param_next__suffix$A$
#define __param_next__suffix$A$(...) __VA_ARGS__
#define __block_inline__suffix$A$(_T, __a, __begin, _a, _begin...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= len$A(*__a), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$A(*__a)); \
    init$S$((_T)(ptr$A(*__a) + __begin, len$A(*__a) - __begin)); \
})

#define slice$S(/*_s: S$$(_T), $r(_begin, _end): R*/... /*(S$$(_T))*/) __param_expand__slice$S(__VA_ARGS__)
#define __param_expand__slice$S(_s, _range...) __block_inline__slice$S(pp_uniqTok(s), pp_uniqTok(range), _s, _range)
#define __block_inline__slice$S(__s, __range, _s, _range...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$S(__s)); \
    (TypeOf(__s)) init$S(&ptr$S(__s)[__range.begin], len$R(__range)); \
})
#define slice$S$(/*(_T)(_s: S$$(_T), $r(_begin, _end): R)*/... /*(S$(_T))*/) pp_expand(pp_defer(__block_inline__slice$S$)(__param_expand__slice$S$ __VA_ARGS__))
#define __param_expand__slice$S$(...) __VA_ARGS__, pp_uniqTok(s), pp_uniqTok(range), __param_next__slice$S$
#define __param_next__slice$S$(...) __VA_ARGS__
#define __block_inline__slice$S$(_T, __s, __range, _s, _range...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$S(__s)); \
    init$S$((_T)(&ptr$S(__s)[__range.begin], len$R(__range))); \
})
#define prefix$S(/*_s: S$$(_T), _end: usize*/... /*(S$$(_T))*/) __param_expand__prefix$S(__VA_ARGS__)
#define __param_expand__prefix$S(_s, _end...) __block_inline__prefix$S(pp_uniqTok(s), pp_uniqTok(end), _s, _end)
#define __block_inline__prefix$S(__s, __end, _s, _end...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__end, usize) = _end; \
    debug_assert_fmt(__end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __end, len$S(__s)); \
    (TypeOf(__s)) init$S(ptr$S(__s), __end); \
})
#define prefix$S$(/*(_T)(_s: S$$(_T), _end: usize)*/... /*(S$(_T))*/) pp_expand(pp_defer(__block_inline__prefix$S$)(__param_expand__prefix$S$ __VA_ARGS__))
#define __param_expand__prefix$S$(...) __VA_ARGS__, pp_uniqTok(s), pp_uniqTok(end), __param_next__prefix$S$
#define __param_next__prefix$S$(...) __VA_ARGS__
#define __block_inline__prefix$S$(_T, __s, __end, _s, _end...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__end, usize) = _end; \
    debug_assert_fmt(__end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __end, len$S(__s)); \
    init$S$((_T)(ptr$S(__s), __end)); \
})
#define suffix$S(/*_s: S$$(_T), _begin: usize*/... /*(S$$(_T))*/) __param_expand__suffix$S(__VA_ARGS__)
#define __param_expand__suffix$S(_s, _begin...) __block_inline__suffix$S(pp_uniqTok(s), pp_uniqTok(begin), _s, _begin)
#define __block_inline__suffix$S(__s, __begin, _s, _begin...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= len$S(__s), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$S(__s)); \
    (TypeOf(__s)) init$S(ptr$S(__s) + __begin, len$S(__s) - __begin); \
})
#define suffix$S$(/*(_T)(_s: S$$(_T), _begin: usize)*/... /*(S$(_T))*/) pp_expand(pp_defer(__block_inline__suffix$S$)(__param_expand__suffix$S$ __VA_ARGS__))
#define __param_expand__suffix$S$(...) __VA_ARGS__, pp_uniqTok(s), pp_uniqTok(begin), __param_next__suffix$S$
#define __param_next__suffix$S$(...) __VA_ARGS__
#define __block_inline__suffix$S$(_T, __s, __begin, _s, _begin...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= len$S(__s), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$S(__s)); \
    init$S$((_T)(ptr$S(__s) + __begin, len$S(__s) - __begin)); \
})

// #define init$O()
// #define init$O$()
// #define asg$O()
// #define asg$O$()

#define some(_val...) { \
    .is_some = true, \
    .payload[0] = { .some = _val }, \
}
#define some$(/*(_T)(_val: _T))*/... /*(O$(_T))*/) pp_expand(pp_defer(__block_inline__some$)(__param_expand__some$ __VA_ARGS__))
#define __param_expand__some$(...) __VA_ARGS__, pp_expand
#define __block_inline__some$(...) __block_inline1__some$(__VA_ARGS__)
#define __block_inline1__some$(_T, _val...) ((O$(_T))some(_val))
#define some$$(/*(_T)(_val: _T))*/... /*(O$$(_T))*/) pp_expand(pp_defer(__block_inline__some$$)(__param_expand__some$$ __VA_ARGS__))
#define __param_expand__some$$(...) __VA_ARGS__, pp_expand
#define __block_inline__some$$(...) __block_inline1__some$$$(__VA_ARGS__)
#define __block_inline1__some$$$(_T, _val...) ((O$$(_T))some(_val))

#define none() { .is_some = false }
#define none$(/*(_T)(none())*/... /*(O$(_T))*/) pp_expand(pp_defer(__block_inline__none$)(__param_expand__none$ __VA_ARGS__))
#define __param_expand__none$(...) __VA_ARGS__, pp_expand
#define __block_inline__none$(...) __block_inline1__none$(__VA_ARGS__)
#define __block_inline1__none$(_T, _val...) ((O$(_T))none())
#define none$$(/*(_T)(none())*/... /*(O$$(_T))*/) pp_expand(pp_defer(__block_inline__none$$)(__param_expand__none$$ __VA_ARGS__))
#define __param_expand__none$$(...) __VA_ARGS__, pp_expand
#define __block_inline__none$$(...) __block_inline1__none$$$(__VA_ARGS__)
#define __block_inline1__none$$$(_T, _val...) ((O$$(_T))none())

#define isSome(_o /*: O$$(_T)*/... /*(bool)*/) as$(bool)((_o).is_some)
#define isNone(_o /*: O$$(_T)*/... /*(bool)*/) as$(bool)(!(_o).is_some)

#define return_some(_val...) \
    return_(some(_val))
#define return_none() \
    return_(none())

#define orelse_(/*(_Expr: O$$(_T))(_DefaultExpr_OR_Body...: _T|void)*/... /*(_T)*/) \
    pp_expand(pp_defer(__block_inline__orelse_)(__param_expand__orelse_ __VA_ARGS__))
#define __param_expand__orelse_(...) __VA_ARGS__, pp_expand
#define __block_inline__orelse_(_Expr, _DefaultExpr_OR_Body...) __block_inline1__orelse_(pp_uniqTok(result), _Expr, ({ _DefaultExpr_OR_Body; }))
#define __block_inline1__orelse_(__result, _Expr, _DefaultExpr_OR_Body...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcompound-token-split-by-macro\"", \
    "clang diagnostic pop", \
    ({ \
        var __result = _Expr; \
        if (isNone(__result)) { \
            __result.payload->some = _Generic( \
                TypeOfUnqual(_DefaultExpr_OR_Body), \
                void: ({ \
                    $ignore_void _DefaultExpr_OR_Body; \
                    ((TypeOf(__result.payload->some)){}); \
                }), \
                default: _DefaultExpr_OR_Body \
            ); \
        } \
        __result.payload->some; \
    }) \
)
#define unwrap_(/*(_Expr: O$$(_T))*/... /*(_T)*/) \
    __unwrap(__VA_ARGS__)
#define __unwrap(_Expr...) orelse_((_Expr)(claim_unreachable))

// #define init$E()
// #define init$E$()
// #define asg$E()
// #define asg$E$()

#define ok(_val...) { \
    .is_ok = true, \
    .payload[0] = { .ok = _val }, \
}
#define ok$(/*(_T)(_val: _T))*/... /*(E$(_T))*/) pp_expand(pp_defer(__block_inline__ok$)(__param_expand__ok$ __VA_ARGS__))
#define __param_expand__ok$(...) __VA_ARGS__, pp_expand
#define __block_inline__ok$(...) __block_inline1__ok$(__VA_ARGS__)
#define __block_inline1__ok$(_T, _val...) ((E$(_T))ok(_val))
#define ok$$(/*(_T)(_val: _T))*/... /*(E$$(_T))*/) pp_expand(pp_defer(__block_inline__ok$$)(__param_expand__ok$$ __VA_ARGS__))
#define __param_expand__ok$$(...) __VA_ARGS__, pp_expand
#define __block_inline__ok$$(...) __block_inline1__ok$$$(__VA_ARGS__)
#define __block_inline1__ok$$$(_T, _val...) ((E$$(_T))ok(_val))

#define err(_val...) { \
    .is_err = true, \
    .payload[0] = { .err = _val }, \
}
#define err$(/*(_T)(_val: _T))*/... /*(E$(_T))*/) pp_expand(pp_defer(__block_inline__err$)(__param_expand__err$ __VA_ARGS__))
#define __param_expand__err$(...) __VA_ARGS__, pp_expand
#define __block_inline__err$(...) __block_inline1__err$(__VA_ARGS__)
#define __block_inline1__err$(_T, _val...) ((E$(_T))err(_val))
#define err$$(/*(_T)(_val: _T))*/... /*(E$$(_T))*/) pp_expand(pp_defer(__block_inline__err$$)(__param_expand__err$$ __VA_ARGS__))
#define __param_expand__err$$(...) __VA_ARGS__, pp_expand
#define __block_inline__err$$(...) __block_inline1__err$$$(__VA_ARGS__)
#define __block_inline1__err$$$(_T, _val...) ((E$$(_T))err(_val))

#define isOk(_e /*: E$$(_T)*/... /*(bool)*/) as$(bool)((_e).is_ok)
#define isErr(_e /*: E$$(_T)*/... /*(bool)*/) as$(bool)(!(_e).is_ok)

#define return_ok(_val...) \
    return_(ok(_val))
#define return_err(_val...) \
    ($debug_point ErrTrace_captureFrame()); \
    return_(err(_val))

#define try_(/*(_Expr: E$$(_T))*/... /*(_T)*/) \
    __try_(pp_uniqTok(result), __VA_ARGS__)
#define __try_(__result, _Expr...) ({ \
    let __result = _Expr; \
    if (isErr(__result)) { \
        return_err(__result.payload->err); \
    } \
    __result.payload->ok; \
})

#define catch_(/*(_Expr: E$$(_T))(_Capture|$ignore, _DefaultExpr_OR_Body...: _T|void)*/... /*(_T)*/) \
    pp_expand(pp_defer(__block_inline__catch_)(__param_expand__catch_ __VA_ARGS__))
#define __param_expand__catch_(...) __VA_ARGS__, pp_expand
#define __block_inline__catch_(_Expr, _Payload_Capture, _DefaultExpr_OR_Body...) __block_inline1__catch_(pp_uniqTok(result), _Expr, _Payload_Capture, ({ _DefaultExpr_OR_Body; }))
#define __block_inline1__catch_(__result, _Expr, _Payload_Capture, _DefaultExpr_OR_Body...) $pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcompound-token-split-by-macro\"", \
    "clang diagnostic pop", \
    ({ \
        var __result = _Expr; \
        if (isErr(__result)) { \
            let _Payload_Capture = __result.payload->err; \
            __result.payload->ok = _Generic( \
                TypeOfUnqual(_DefaultExpr_OR_Body), \
                void: ({ \
                    _DefaultExpr_OR_Body; \
                    ((TypeOf(__result.payload->ok)){}); \
                }), \
                default: _DefaultExpr_OR_Body \
            ); \
            ErrTrace_reset(); \
        } \
        __result.payload->ok; \
    }) \
)
#define errdefer_(/*(_O_Capture|$ignore, _Expr_OR_Body...:void)*/... /*(void)*/) \
    __errdefer_(__VA_ARGS__)
#define __errdefer_(_Payload_Capture, _Expr...) defer_(if (__reserved_return->is_err) { \
    let _Payload_Capture = __reserved_return->data.err; \
    _Expr; \
})

// ============================================================================
// Generic Meta Types - The Foundation
// ============================================================================

typedef struct meta_P_const {
    TypeInfo type;
    union {
        P_const$raw ptr;
        P_const$raw raw;
    };
} meta_P_const$raw;
typedef union meta_P$raw {
    struct {
        TypeInfo type;
        union {
            P$raw ptr;
            P$raw raw;
        };
    };
    meta_P_const$raw as_const;
} meta_P$raw;

typedef union meta_V$raw {
    struct {
        TypeInfo inner_type;
        P$raw inner;
    };
    meta_P$raw ref;
} meta_V$raw;

typedef struct meta_S_const$raw {
    TypeInfo type;
    union {
        struct {
            P_const$raw ptr;
            usize len;
        };
        S_const$raw raw;
    };
} meta_S_const$raw;
typedef union meta_S$raw {
    struct {
        TypeInfo type;
        union {
            struct {
                P$raw ptr;
                usize len;
            };
            S$raw raw;
        };
    };
    meta_S_const$raw as_const;
} meta_S$raw;

typedef union meta_A$raw {
    struct {
        TypeInfo inner_type;
        S$raw inner;
    };
    meta_S$raw ref;
} meta_A$raw;

typedef struct meta_O$raw {
    bool is_some;
    union {
        union {
            Void none;
            meta_V$raw some;
        } payload[1];
        meta_V$raw inner;
    };
} meta_O$raw;

typedef struct meta_E$raw {
    bool is_ok;
    union {
        union {
            ErrCode err;
            meta_V$raw ok;
        } payload[1];
        meta_V$raw inner;
    };
} meta_E$raw;

// ============================================================================
// Conversion Patterns - The Core Innovation
// ============================================================================

#define meta_create(_type...) ({ \
    const TypeInfo __type = _type; \
    const P$raw __ptr = alloca(__type.size); \
    prim_memset(__ptr, 0, __type.size); \
    ((meta_P$raw){ .type = __type, .ptr = __ptr }); \
})
#define meta_alloc(_type, _len...) ({ \
    const TypeInfo __type = _type; \
    const usize __len = _len; \
    const P$raw __ptr = alloca(__type.size * __len); \
    prim_memset(__ptr, 0, __type.size * __len); \
    ((meta_S$raw){ .type = __type, .ptr = __ptr, .len = __len }); \
})

#define meta_ret$V(_T) ((meta_V$raw){ .inner_type = typeInfo$(_T), .inner = &((_T){}) })
#define meta_ret$A(_N, _T) ((meta_A$raw){ .inner_type = typeInfo$(FieldType$(A$$(_N, _T), val[0])), .inner = ref$A((A$$(_N, _T)){}) })
#define meta_ret$O(_T) ((meta_O$raw){ .inner_type = typeInfo$(FieldType$(O$$(_T), payload->some)), .inner = ((O$$(_T)){}).ref_raw })
#define meta_ret$E(_T) ((meta_E$raw){ .inner_type = typeInfo$(FieldType$(E$$(_T), payload->ok)), .inner = ((E$$(_T)){}).ref_raw })

#define metaRef$P(_p...) ((meta_P_const$raw){ .type = typeInfo$(TypeOf(*_p)), .raw = _p })
#define metaMut$P(_p...) ((meta_P$raw){ .type = typeInfo$(TypeOf(*_p)), .raw = _p })
#define metaRef$S(_s...) ((meta_S_const$raw){ .type = typeInfo$(TypeOf(*_s.ptr)), .raw = _s.as_raw })
#define metaMut$S(_s...) ((meta_S$raw){ .type = typeInfo$(TypeOf(*_s.ptr)), .raw = _s.as_raw })

#define meta$V(_v...) ({ \
    let __p_v = &copy(_v); \
    ((meta_V$raw){ .inner_type = typeInfo$(TypeOf(*__p_v)), .inner = __p_v }); \
})
#define meta$A(_a...) ({ \
    let __p_a = &copy(_a); \
    ((meta_A$raw){ .inner_type = typeInfo$(TypeOf(*__p_a->val)), .inner = ref$A(*__p_a).as_raw }); \
})
#define meta$O(_o...) ({ \
    let __p_o = &copy(_o); \
    __p_o->is_some \
        ? (meta_O$raw)some(meta$V(__p_o->payload->some)) \
        : (meta_O$raw)none(); \
})
#define meta$E(_e...) ({ \
    let __p_e = &copy(_e); \
    __p_e->is_ok \
        ? (meta_E$raw)ok(meta$V(__p_e->payload->ok)) \
        : (meta_E$raw)err(__p_e->payload->err); \
})

#define meta_pRef$(/*(_T)(_Expr...)*/... /*(P_const$(_T))*/) \
    __step_inline__meta_pRef$(pp_defer(__emit_inline__meta_pRef$)(__param_parse__meta_pRef$ __VA_ARGS__))
#define __step_inline__meta_pRef$(...) __VA_ARGS__
#define __param_parse__meta_pRef$(...) __VA_ARGS__, __param_next__meta_pRef$
#define __param_next__meta_pRef$(...) __VA_ARGS__
#define __emit_inline__meta_pRef$(_T, _meta...) (*as$(P_const$(_T) (*)(&__meta.raw)))
#define meta_pRef$$(/*(_T)(_Expr...)*/... /*(P_const$$(_T))*/) \
    __step_inline__meta_pRef$$(pp_defer(__emit_inline__meta_pRef$$)(__param_parse__meta_pRef$$ __VA_ARGS__))
#define __step_inline__meta_pRef$$(...) __VA_ARGS__
#define __param_parse__meta_pRef$$(...) __VA_ARGS__, __param_next__meta_pRef$$
#define __param_next__meta_pRef$$(...) __VA_ARGS__
#define __emit_inline__meta_pRef$$(_T, _meta...) (*as$(P_const$$(_T) (*)(&__meta.raw)))

#define meta_pMut$(/*(_T)(_Expr...)*/... /*(P$(_T))*/) \
    __step_inline__meta_pMut$(pp_defer(__emit_inline__meta_pMut$)(__param_parse__meta_pMut$ __VA_ARGS__))
#define __param_parse__meta_pMut$(...) __VA_ARGS__, __param_next__meta_pMut$
#define __param_next__meta_pMut$(...) __VA_ARGS__
#define __emit_inline__meta_pMut$(_T, _meta...) (*as$(P$(_T) (*)(&__meta.raw)))
#define meta_pMut$$(/*(_T)(_Expr...)*/... /*(P$$(_T))*/) \
    __step_inline__meta_pMut$$(pp_defer(__emit_inline__meta_pMut$$)(__param_parse__meta_pMut$$ __VA_ARGS__))
#define __param_parse__meta_pMut$$(...) __VA_ARGS__, __param_next__meta_pMut$$
#define __param_next__meta_pMut$$(...) __VA_ARGS__
#define __emit_inline__meta_pMut$$(_T, _meta...) (*as$(P$$(_T) (*)(&__meta.raw)))

#define meta_v$(/*(_T)(_Expr...)*/... /*(_T)*/) \
    __step_inline__meta_v$(pp_defer(__emit_inline__meta_v$)(__param_parse__meta_v$ __VA_ARGS__))
#define __step_inline__meta_v$(...) __VA_ARGS__
#define __param_parse__meta_v$(...) __VA_ARGS__, __param_next__meta_v$
#define __param_next__meta_v$(...) __VA_ARGS__
#define __emit_inline__meta_v$(_T, _meta...) (*as$(_T*)(_meta.inner))

#define meta_sRef$(/*(_T)(_Expr...)*/... /*(S_const$(_T))*/) \
    __step_inline__meta_sRef$(pp_defer(__emit_inline__meta_sRef$)(__param_parse__meta_sRef$ __VA_ARGS__))
#define __step_inline__meta_sRef$(...) __VA_ARGS__
#define __param_parse__meta_sRef$(...) __VA_ARGS__, __param_next__meta_sRef$
#define __param_next__meta_sRef$(...) __VA_ARGS__
#define __emit_inline__meta_sRef$(_T, _meta...) (*as$(S_const$(_T) (*)(&__meta.raw)))
#define meta_sRef$$(/*(_T)(_Expr...)*/... /*(S_const$$(_T))*/) \
    __step_inline__meta_sRef$$(pp_defer(__emit_inline__meta_sRef$$)(__param_parse__meta_sRef$$ __VA_ARGS__))
#define __step_inline__meta_sRef$$(...) __VA_ARGS__
#define __param_parse__meta_sRef$$(...) __VA_ARGS__, __param_next__meta_sRef$$
#define __param_next__meta_sRef$$(...) __VA_ARGS__
#define __emit_inline__meta_sRef$$(_T, _meta...) (*as$(S_const$$(_T) (*)(&__meta.raw)))

#define meta_sMut$(/*(_T)(_Expr...)*/... /*(S$(_T))*/) \
    __step_inline__meta_sMut$(pp_defer(__emit_inline__meta_sMut$)(__param_parse__meta_sMut$ __VA_ARGS__))
#define __step_inline__meta_sMut$(...) __VA_ARGS__
#define __param_parse__meta_sMut$(...) __VA_ARGS__, __param_next__meta_sMut$
#define __param_next__meta_sMut$(...) __VA_ARGS__
#define __emit_inline__meta_sMut$(_T, _meta...) (*as$(S$(_T) (*)(&__meta.raw)))
#define meta_sMut$$(/*(_T)(_Expr...)*/... /*(S$$(_T))*/) \
    __step_inline__meta_sMut$$(pp_defer(__emit_inline__meta_sMut$$)(__param_parse__meta_sMut$$ __VA_ARGS__))
#define __step_inline__meta_sMut$$(...) __VA_ARGS__
#define __param_parse__meta_sMut$$(...) __VA_ARGS__, __param_next__meta_sMut$$
#define __param_next__meta_sMut$$(...) __VA_ARGS__
#define __emit_inline__meta_sMut$$(_T, _meta...) (*as$(S$$(_T) (*)(&__meta.raw)))

#define meta_a$(/*(_N,_T)(_Expr...)*/... /*(A$(_N,_T))*/) \
    __step_inline__meta_a$(pp_defer(__emit_inline__meta_a$)(__param_parse__meta_a$ __VA_ARGS__))
#define __step_inline__meta_a$(...) __VA_ARGS__
#define __param_parse__meta_a$(...) __VA_ARGS__, __param_next__meta_a$
#define __param_next__meta_a$(...) __VA_ARGS__
#define __emit_inline__meta_a$(_N, _T, _meta...) (*as$(A$(_N, _T) (*)(__meta.inner)))
#define meta_a$$(/*(_N,_T)(_Expr...)*/... /*(A$$(_N,_T))*/) \
    __step_inline__meta_a$$(pp_defer(__emit_inline__meta_a$$)(__param_parse__meta_a$$ __VA_ARGS__))
#define __step_inline__meta_a$$(...) __VA_ARGS__
#define __param_parse__meta_a$$(...) __VA_ARGS__, __param_next__meta_a$$
#define __param_next__meta_a$$(...) __VA_ARGS__
#define __emit_inline__meta_a$$(_N, _T, _meta...) (*as$(A$$(_N, _T) (*)(__meta.inner)))

#define meta_o$(/*(_T)(_Expr...)*/... /*(O$(_T))*/) \
    pp_expand(pp_defer(__block_inline__meta_o$)(__param_expand__meta_o$ __VA_ARGS__))
#define __param_expand__meta_o$(...) pp_uniqTok(meta), __VA_ARGS__, __param_expand1__meta_o$
#define __param_expand1__meta_o$(...) __VA_ARGS__
#define __block_inline__meta_o$(__meta, _T, _meta...) ({ \
    let __meta = _meta; \
    __meta.is_some \
        ? ((O$(_T))some(*as$(_T*)(__meta.payload->some.inner))) \
        : ((O$(_T))none()); \
})
#define meta_o$$(/*(_T)(_Expr...)*/... /*(O$$(_T))*/) \
    pp_expand(pp_defer(__block_inline__meta_o$$)(__param_expand__meta_o$$ __VA_ARGS__))
#define __param_expand__meta_o$$(...) pp_uniqTok(meta), __VA_ARGS__, __param_expand1__meta_o$$
#define __param_expand1__meta_o$$(...) __VA_ARGS__
#define __block_inline__meta_o$$(__meta, _T, _meta...) ({ \
    let __meta = _meta; \
    __meta.is_some \
        ? ((O$$(_T))some(*as$(_T*)(__meta.payload->some.inner))) \
        : ((O$$(_T))none()); \
})

#define meta_e$(/*(_T)(_Expr...)*/... /*(E$(_T))*/) \
    pp_expand(pp_defer(__block_inline__meta_e$)(__param_expand__meta_e$ __VA_ARGS__))
#define __param_expand__meta_e$(...) pp_uniqTok(meta), __VA_ARGS__, __param_expand1__meta_e$
#define __param_expand1__meta_e$(...) __VA_ARGS__
#define __block_inline__meta_e$(__meta, _T, _meta...) ({ \
    let __meta = _meta; \
    __meta.is_ok \
        ? ((E$(_T))ok(meta_v$((_T)(__meta.payload->ok)))) \
        : ((E$(_T))err(__meta.payload->err)); \
})
#define meta_e$$(/*(_T)(_Expr...)*/... /*(E$$(_T))*/) \
    pp_expand(pp_defer(__block_inline__meta_e$$)(__param_expand__meta_e$$ __VA_ARGS__))
#define __param_expand__meta_e$$(...) pp_uniqTok(meta), __VA_ARGS__, __param_expand1__meta_e$$
#define __param_expand1__meta_e$$(...) __VA_ARGS__
#define __block_inline__meta_e$$(__meta, _T, _meta...) ({ \
    let __meta = _meta; \
    __meta.is_ok \
        ? ((E$$(_T))ok(meta_v$((_T)(__meta.payload->ok)))) \
        : ((E$$(_T))err(__meta.payload->err)); \
})


#endif // META_OPS_H

typedef struct O$i32 {
    bool is_some;
    union {
        Void none;
        i32 some;
    } payload[1];
} O$i32;

typedef struct E$i32 {
    bool is_ok;
    union {
        u32 err;
        i32 ok;
    } payload[1];
} E$i32;

typedef struct meta$O {
    bool is_some;
    union {
        union {
            Void none;
            meta_V$raw some;
        } payload[1];
        meta_V$raw inner;
    };
} meta$O;

typedef struct meta$E {
    bool is_ok;
    union {
        union {
            u32 err;
            meta_V$raw ok;
        } payload[1];
        meta_V$raw inner;
    };
} meta$E;

/* #define meta$O(_o...) ({ \
    let __p_o = &copy(_o); \
    __p_o->is_some \
        ? ((meta$O){ .is_some = __p_o->is_some, .payload = { [0] = { .some = meta$V(__p_o->payload->some) } } }) \
        : ((meta$O){ .is_some = __p_o->is_some, .payload = { [0] = { .none = __p_o->payload->none } } }); \
})

#define meta$E(_e...) ({ \
    let __p_e = &copy(_e); \
    __p_e->is_ok \
        ? ((meta$E){ .is_ok = __p_e->is_ok, .payload = { [0] = { .ok = meta$V(__p_e->payload->ok) } } }) \
        : ((meta$E){ .is_ok = __p_e->is_ok, .payload = { [0] = { .err = __p_e->payload->err } } }); \
}) */

/* #define meta$O(_o...) ({ \
    let __o = ((TypeOf(_o)[1]){ [0] = _o }); \
    ((meta$O){ .is_some = __o->is_some, .payload = { { .some = meta$V(__o->payload->some) } } }); \
}) */



#include <stdio.h>

tpl$P$(u32);
tpl$S$(u32);
tpl$P$(i32);
tpl$S$(i32);
tpl$P$(f32);
tpl$S$(f32);
void better(void);
int main(void) {
#if UNUSED_CODE
    var arr_u = init$A$$((8, u32)({ 0, 1, 2, 3, 4, 5, 6, 7 }));
    var arr_i = init$A$$((9, i32)({ 8, 9, 10, 11, 12, 13, 14, 15, 16 }));
    var arr_f = init$A$$((10, f32)({ 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f }));
    var range = $r(3, 15);

    for (struct {
             S$u32 sli_u;
             S$i32 sli_i;
             S$f32 sli_f;
             R range_r;
             usize idx;
         } __state
         = {
             .sli_u = suffix$A$((u32)(arr_u, 0)), // suffix$A$((u32)(arr_u, 0))
             .sli_i = suffix$A$((i32)(arr_i, 1)), // suffix$A$((i32)(arr_i, 1))
             .sli_f = suffix$A$((f32)(arr_f, 2)), // suffix$A$((f32)(arr_f, 2))
             .range_r = suffix$R(range, 3),
             .idx = 0,
         };
         __state.idx < prim_min4(__state.sli_u.len, __state.sli_i.len, __state.sli_f.len, len$R(__state.range_r)); ++__state.idx) {
        let item_u = at$S(__state.sli_u, __state.idx);
        let item_i = at$S(__state.sli_i, __state.idx);
        let item_f = at$S(__state.sli_f, __state.idx);
        let item_r = at$R(__state.range_r, __state.idx);
        let idx_u = __state.idx + 0;
        let idx_i = __state.idx + 1;
        let idx_f = __state.idx + 2;
        let idx_r = __state.idx + 3;
        printf("item_u[%zu]: %u\n", idx_u, *item_u);
        printf("item_i[%zu]: %d\n", idx_i, *item_i);
        printf("item_f[%zu]: %f\n", idx_f, *item_f);
        printf("item_r[%zu]: %zu\n", idx_r, item_r);
    }

    ({
        let __s0 = suffix$A(arr_u, 0);
        const usize __s0_offset = 0;
        let __s1 = suffix$A(arr_i, 1);
        const usize __s1_offset = 1;
        let __s2 = suffix$A(arr_f, 2);
        const usize __s2_offset = 2;
        let __r3 = suffix$R(range, 3);
        const usize __r3_offset = 3;
        let __len = prim_min4(len$S(__s0), len$S(__s1), len$S(__s2), len$R(__r3));
        for (usize __i = 0; __i < __len; ++__i) {
            let item_u = at$S(__s0, __i);
            const usize idx_u = __i + __s0_offset;
            let item_i = at$S(__s1, __i);
            const usize idx_i = __i + __s1_offset;
            let item_f = at$S(__s2, __i);
            const usize idx_f = __i + __s2_offset;
            let item_r = at$R(__r3, __i);
            const usize idx_r = __i + __r3_offset;
            printf("item_u[%zu]: %u\n", idx_u, *item_u);
            printf("item_i[%zu]: %d\n", idx_i, *item_i);
            printf("item_f[%zu]: %f\n", idx_f, *item_f);
            printf("item_r[%zu]: %zu\n", idx_r, item_r);
        }
    });
#endif /* UNUSED_CODE */

#define for_iter_init(__offset, __r, _target, _offset...) \
    const R __r = _target; \
    const usize __offset = _offset
#define for_iter_init_$s(__offset, __s, _target, _offset...) \
    let __s = _target; \
    const usize __offset = _offset
#define for_iter_init_$a(__offset, __a, _target, _offset...) \
    let __a = _target; \
    const usize __offset = _offset

#define for_iter_capture(__idx, _c_offset, _c_r, __r, _offset...) \
    const R _c_r = at$R(__r, __idx); \
    let _c_offset = __idx + _offset
#define for_iter_capture_$s(__idx, _c_offset, _c_s, __s, _offset...) \
    let _c_s = at$S(__s, __idx); \
    let _c_offset = __idx + _offset
#define for_iter_capture_$a(__idx, _c_offset, _c_a, __a, _offset...) \
    let _c_a = at$A(__a, __idx); \
    let _c_offset = __idx + _offset

#if CONCEPT
    foreach_((
        $a(arr_u, (item_u, idx_u)),
        $s(suffix$A(arr_i, 1), (item_i, idx_i, 1)),
        $s(suffix$A(arr_f, 2), (item_f, idx_f, 2)),
        (range, (item_r, idx_r))
    ) {
        printf("item_u[%zu]: %u\n", idx_u, *item_u);
        printf("item_i[%zu]: %d\n", idx_i, *item_i);
        printf("item_f[%zu]: %f\n", idx_f, *item_f);
        printf("item_r[%zu]: %zu\n", idx_r, item_r);
    });
#endif /* CONCEPT */

#define $rf(_expr...) $r(_expr, usize_limit_max)
#define $rt(_expr...) $r(0, _expr)
#define $a(_a...) ($A, (_a))
#define $s(_s...) ($S, (_s))

#define foreach_(/*(<$a|$s>(range|slice, (iter, index, offset))...) { ... }*/...) \
    __foreach_exec(pp_defer(__foreach_impl)(__foreach_sep __VA_ARGS__))
#define __foreach_exec(...) __VA_ARGS__
#define __foreach_sep(...) (__VA_ARGS__),
#define __foreach_expandIters(...) __VA_ARGS__
#define __foreach_impl(_Iters, _Expr...) pp_overload(__foreach_impl, __foreach_expandIters _Iters)(_Iters, _Expr)

#define __foreach_impl_1(_Iters, _Expr...) __foreach_1(pp_uniqTok(len), pp_uniqTok(i), ((pp_uniqTok(r0), pp_uniqTok(o0))), _Iters, _Expr)
#define __foreach_1(__len, __i, __Names, _Iters, _Expr...) ({ \
    __foreach_1_initIters(__Names, _Iters); \
    const usize __len = __foreach_1_measureLen(__Names, _Iters); \
    for (usize __i = 0; __i < __len; ++__i) { \
        __foreach_1_captureIters(__i, __Names, _Iters); \
        _Expr; \
    } \
})

#define __foreach_expandIter(/*_Tag,_Target,_Captures*/...) __foreach_expandIter_Next(__VA_ARGS__)
#define __foreach_expandIter_Next(...) __VA_ARGS__
#define __foreach_expandName(/*_Name,_Offset*/...) __foreach_expandName_Next(__VA_ARGS__)
#define __foreach_expandName_Next(...) __VA_ARGS__
#define __foreach_expandCapture(/*_Elem,_Index,_Offset*/...) __foreach_expandCapture_Next(__VA_ARGS__)
#define __foreach_expandCapture_Next(...) __VA_ARGS__
#define __foreach_initIter_getOffsetOrZero_1(_Captures...) 0
#define __foreach_initIter_getOffsetOrZero_2(_Captures...) 0
#define __foreach_initIter_getOffsetOrZero_3(_Captures...) pp_Tuple_get3rd _Captures

#define __foreach_1_initIters(__Names, _Iters...) \
    pp_overload(__foreach_initIter, __foreach_expandIter pp_Tuple_get1st _Iters)(__foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters)
#define __foreach_initIter_2(...) __foreach_initIter_2Tuple_get(__VA_ARGS__)
#define __foreach_initIter_2Tuple_get(...) __foreach_initIter_2expand(__VA_ARGS__)
#define __foreach_initIter_2expand(__Name, __Offset, _TargetRange, _Captures...) \
    const R __Name = _TargetRange __foreach_initIter_defOffset(__Offset, _Captures)
#define __foreach_initIter_3(...) __foreach_initIter_3Tuple_get(__VA_ARGS__)
#define __foreach_initIter_3Tuple_get(...) __foreach_initIter_3expand(__VA_ARGS__)
#define __foreach_initIter_3expand(__Name, __Offset, _Tag, _Target, _Captures...) \
    let __Name = _Target __foreach_initIter_defOffset(__Offset, _Captures)

#define __foreach_initIter_defOffset(__Offset, _Captures...) \
    pp_overload(__foreach_initIter_defOffset, __foreach_expandCapture _Captures)(__Offset, _Captures)
#define __foreach_initIter_defOffset_1(__Offset, _Captures...)
#define __foreach_initIter_defOffset_2(__Offset, _Captures...)
#define __foreach_initIter_defOffset_3(__Offset, _Captures...) \
    ; \
    const usize __Offset = pp_Tuple_get3rd _Captures

#define __foreach_1_measureLen(__Names, _Iters...) \
    pp_overload(__foreach_lenMethod, __foreach_expandIter pp_Tuple_get1st _Iters)(__foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters)
#define __foreach_lenMethod_2(...) __foreach_lenMethod_2Tuple_get(__VA_ARGS__)
#define __foreach_lenMethod_2Tuple_get(...) __foreach_lenMethod_2expand(__VA_ARGS__)
#define __foreach_lenMethod_2expand(__Name, __Offset, _TargetRange, _Captures...) \
    len$R(__Name)
#define __foreach_lenMethod_3(...) __foreach_lenMethod_3Tuple_get(__VA_ARGS__)
#define __foreach_lenMethod_3Tuple_get(...) __foreach_lenMethod_3expand(__VA_ARGS__)
#define __foreach_lenMethod_3expand(__Name, __Offset, _Tag, _Target, _Capture...) \
    pp_cat(len, _Tag)(__Name)

#define __foreach_1_captureIters(__i, __Names, _Iters...) \
    pp_overload(__foreach_captureIters, __foreach_expandIter pp_Tuple_get1st _Iters)(__i, __foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters)
#define __foreach_captureIters_2(...) __foreach_captureIters_2Tuple_get(__VA_ARGS__)
#define __foreach_captureIters_2Tuple_get(...) __foreach_captureIters_2expand(__VA_ARGS__)
#define __foreach_captureIters_2expand(__i, __Name, __Offset, _TargetRange, _Captures...) \
    let pp_Tuple_get1st _Captures = at$R(__Name, __i) __foreach_captureIters_defOffset(__i, __Offset, _Captures)
#define __foreach_captureIters_3(...) __foreach_captureIters_3Tuple_get(__VA_ARGS__)
#define __foreach_captureIters_3Tuple_get(...) __foreach_captureIters_3expand(__VA_ARGS__)
#define __foreach_captureIters_3expand(__i, __Name, __Offset, _Tag, _Target, _Captures...) \
    let pp_Tuple_get1st _Captures = pp_cat(at, _Tag)(__Name, __i) __foreach_captureIters_defOffset(__i, __Offset, _Captures)

#define __foreach_captureIters_defOffset(__i, __Offset, _Captures...) \
    pp_overload(__foreach_captureIters_defOffset, __foreach_expandCapture _Captures)(__i, __Offset, _Captures)
#define __foreach_captureIters_defOffset_1(__i, __Offset, _Captures...)
#define __foreach_captureIters_defOffset_2(__i, __Offset, _Captures...) \
    ; \
    let pp_Tuple_get2nd _Captures = __i
#define __foreach_captureIters_defOffset_3(__i, __Offset, _Captures...) \
    ; \
    let pp_Tuple_get2nd _Captures = __i + __Offset


#define __foreach_impl_2(_Iters, _Expr...) __foreach_2( \
    pp_uniqTok(len), pp_uniqTok(i), \
    ( \
        (pp_uniqTok(r0), pp_uniqTok(o0)), \
        (pp_uniqTok(r1), pp_uniqTok(o1)) \
    ), \
    _Iters, _Expr \
)
#define __foreach_2(__len, __i, __Names, _Iters, _Expr...) ({ \
    __foreach_2_initIters(__Names, _Iters); \
    const usize __len = __foreach_2_measureLen(__Names, _Iters); \
    for (usize __i = 0; __i < __len; ++__i) { \
        __foreach_2_captureIters(__i, __Names, _Iters); \
        _Expr; \
    } \
})
#define __foreach_2_initIters(__Names, _Iters...) \
    pp_overload(__foreach_initIter, __foreach_expandIter pp_Tuple_get1st _Iters)(__foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters); \
    pp_overload(__foreach_initIter, __foreach_expandIter pp_Tuple_get2nd _Iters)(__foreach_expandName pp_Tuple_get2nd __Names, __foreach_expandIter pp_Tuple_get2nd _Iters)
#define __foreach_2_measureLen(__Names, _Iters...) \
    prim_min2(pp_overload(__foreach_lenMethod, __foreach_expandIter pp_Tuple_get1st _Iters)(__foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters), pp_overload(__foreach_lenMethod, __foreach_expandIter pp_Tuple_get2nd _Iters)(__foreach_expandName pp_Tuple_get2nd __Names, __foreach_expandIter pp_Tuple_get2nd _Iters))
#define __foreach_2_captureIters(__i, __Names, _Iters...) \
    pp_overload(__foreach_captureIters, __foreach_expandIter pp_Tuple_get1st _Iters)(__i, __foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters); \
    pp_overload(__foreach_captureIters, __foreach_expandIter pp_Tuple_get2nd _Iters)(__i, __foreach_expandName pp_Tuple_get2nd __Names, __foreach_expandIter pp_Tuple_get2nd _Iters)


#define __foreach_impl_3(_Iters, _Expr...) __foreach_3( \
    pp_uniqTok(len), pp_uniqTok(i), \
    ( \
        (pp_uniqTok(r0), pp_uniqTok(o0)), \
        (pp_uniqTok(r1), pp_uniqTok(o1)), \
        (pp_uniqTok(r2), pp_uniqTok(o2)) \
    ), \
    _Iters, _Expr \
)
#define __foreach_3(__len, __i, __Names, _Iters, _Expr...) ({ \
    __foreach_3_initIters(__Names, _Iters); \
    const usize __len = __foreach_3_measureLen(__Names, _Iters); \
    for (usize __i = 0; __i < __len; ++__i) { \
        __foreach_3_captureIters(__i, __Names, _Iters); \
        _Expr; \
    } \
})
#define __foreach_3_initIters(__Names, _Iters...) \
    pp_overload(__foreach_initIter, __foreach_expandIter pp_Tuple_get1st _Iters)(__foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters); \
    pp_overload(__foreach_initIter, __foreach_expandIter pp_Tuple_get2nd _Iters)(__foreach_expandName pp_Tuple_get2nd __Names, __foreach_expandIter pp_Tuple_get2nd _Iters); \
    pp_overload(__foreach_initIter, __foreach_expandIter pp_Tuple_get3rd _Iters)(__foreach_expandName pp_Tuple_get3rd __Names, __foreach_expandIter pp_Tuple_get3rd _Iters)
#define __foreach_3_measureLen(__Names, _Iters...) \
    prim_min3(pp_overload(__foreach_lenMethod, __foreach_expandIter pp_Tuple_get1st _Iters)(__foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters), pp_overload(__foreach_lenMethod, __foreach_expandIter pp_Tuple_get2nd _Iters)(__foreach_expandName pp_Tuple_get2nd __Names, __foreach_expandIter pp_Tuple_get2nd _Iters), pp_overload(__foreach_lenMethod, __foreach_expandIter pp_Tuple_get3rd _Iters)(__foreach_expandName pp_Tuple_get3rd __Names, __foreach_expandIter pp_Tuple_get3rd _Iters))
#define __foreach_3_captureIters(__i, __Names, _Iters...) \
    pp_overload(__foreach_captureIters, __foreach_expandIter pp_Tuple_get1st _Iters)(__i, __foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters); \
    pp_overload(__foreach_captureIters, __foreach_expandIter pp_Tuple_get2nd _Iters)(__i, __foreach_expandName pp_Tuple_get2nd __Names, __foreach_expandIter pp_Tuple_get2nd _Iters); \
    pp_overload(__foreach_captureIters, __foreach_expandIter pp_Tuple_get3rd _Iters)(__i, __foreach_expandName pp_Tuple_get3rd __Names, __foreach_expandIter pp_Tuple_get3rd _Iters)

#define __foreach_impl_4(_Iters, _Expr...) __foreach_4( \
    pp_uniqTok(len), pp_uniqTok(i), \
    ( \
        (pp_uniqTok(r0), pp_uniqTok(o0)), \
        (pp_uniqTok(r1), pp_uniqTok(o1)), \
        (pp_uniqTok(r2), pp_uniqTok(o2)), \
        (pp_uniqTok(r3), pp_uniqTok(o3)) \
    ), \
    _Iters, _Expr \
)
#define __foreach_4(__len, __i, __Names, _Iters, _Expr...) ({ \
    __foreach_4_initIters(__Names, _Iters); \
    const usize __len = __foreach_4_measureLen(__Names, _Iters); \
    for (usize __i = 0; __i < __len; ++__i) { \
        __foreach_4_captureIters(__i, __Names, _Iters); \
        _Expr; \
    } \
})
#define __foreach_4_initIters(__Names, _Iters...) \
    pp_overload(__foreach_initIter, __foreach_expandIter pp_Tuple_get1st _Iters)(__foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters); \
    pp_overload(__foreach_initIter, __foreach_expandIter pp_Tuple_get2nd _Iters)(__foreach_expandName pp_Tuple_get2nd __Names, __foreach_expandIter pp_Tuple_get2nd _Iters); \
    pp_overload(__foreach_initIter, __foreach_expandIter pp_Tuple_get3rd _Iters)(__foreach_expandName pp_Tuple_get3rd __Names, __foreach_expandIter pp_Tuple_get3rd _Iters); \
    pp_overload(__foreach_initIter, __foreach_expandIter pp_Tuple_get4th _Iters)(__foreach_expandName pp_Tuple_get4th __Names, __foreach_expandIter pp_Tuple_get4th _Iters)
#define __foreach_4_measureLen(__Names, _Iters...) \
    prim_min4(pp_overload(__foreach_lenMethod, __foreach_expandIter pp_Tuple_get1st _Iters)(__foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters), pp_overload(__foreach_lenMethod, __foreach_expandIter pp_Tuple_get2nd _Iters)(__foreach_expandName pp_Tuple_get2nd __Names, __foreach_expandIter pp_Tuple_get2nd _Iters), pp_overload(__foreach_lenMethod, __foreach_expandIter pp_Tuple_get3rd _Iters)(__foreach_expandName pp_Tuple_get3rd __Names, __foreach_expandIter pp_Tuple_get3rd _Iters), pp_overload(__foreach_lenMethod, __foreach_expandIter pp_Tuple_get4th _Iters)(__foreach_expandName pp_Tuple_get4th __Names, __foreach_expandIter pp_Tuple_get4th _Iters))
#define __foreach_4_captureIters(__i, __Names, _Iters...) \
    pp_overload(__foreach_captureIters, __foreach_expandIter pp_Tuple_get1st _Iters)(__i, __foreach_expandName pp_Tuple_get1st __Names, __foreach_expandIter pp_Tuple_get1st _Iters); \
    pp_overload(__foreach_captureIters, __foreach_expandIter pp_Tuple_get2nd _Iters)(__i, __foreach_expandName pp_Tuple_get2nd __Names, __foreach_expandIter pp_Tuple_get2nd _Iters); \
    pp_overload(__foreach_captureIters, __foreach_expandIter pp_Tuple_get3rd _Iters)(__i, __foreach_expandName pp_Tuple_get3rd __Names, __foreach_expandIter pp_Tuple_get3rd _Iters); \
    pp_overload(__foreach_captureIters, __foreach_expandIter pp_Tuple_get4th _Iters)(__i, __foreach_expandName pp_Tuple_get4th __Names, __foreach_expandIter pp_Tuple_get4th _Iters)

#if UNUSED_CODE
    var_(arr_u, A$$(8, u32)) = init$A({ 0, 1, 2, 3, 4, 5, 6, 7 });
    var_(arr_i, A$$(9, i32)) = init$A({ 8, 9, 10, 11, 12, 13, 14, 15, 16 });
    var_(arr_f, A$$(10, f32)) = init$A({ 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f });
    var_(range, R) = $r(3, 15);

    foreach_(($a(arr_u, (item_u))){
        // try_(TEST_expect(Type_eq$(TypeOf(*item_u), u32)));
        // io_stream_println(u8_l("item_u: {:u}\n"), *item_u);
    });

    foreach_(((range, (item_r))){
        // try_(TEST_expect(Type_eq$(TypeOf(item_r), usize)));
        // io_stream_println(u8_l("item_r: {:zu}\n"), item_r);
    });

    foreach_((
        $a(arr_u, (item_u, idx_u)),
        $s(suffix$A(arr_i, 1), (item_i, idx_i, 1)),
        $s(suffix$A(arr_f, 2), (item_f, idx_f, 2)),
        (range, (item_r, idx_r))
    ){
        // try_(TEST_expect(Type_eq$(TypeOf(*item_u), u32)));
        // io_stream_println(u8_l("item_u[{:zu}]: {:u}\n"), idx_u, *item_u);
        // try_(TEST_expect(Type_eq$(TypeOf(*item_i), i32)));
        // io_stream_println(u8_l("item_i[{:zu}]: {:d}\n"), idx_i, *item_i);
        // try_(TEST_expect(Type_eq$(TypeOf(*item_f), f32)));
        // io_stream_println(u8_l("item_f[{:zu}]: {:f}\n"), idx_f, *item_f);
        // try_(TEST_expect(Type_eq$(TypeOf(item_r), usize)));
        // io_stream_println(u8_l("item_r[{:zu}]: {:zu}\n"), idx_r, item_r);
    });

    for (struct {
             S$(i32) sli;
             usize idx;
         } __state
         = {
             .sli = ref$A$((i32)(arr_i)),
             .idx = 0,
         };
         __state.idx < __state.sli.len; ++__state.idx) {
        let item = at$S(__state.sli, __state.idx);
        let idx = __state.idx;
        printf("item: %d\n", *item);
        printf("idx: %zu\n", idx);
    }
#endif /* UNUSED_CODE */

    var o = some$((i32)(123));
    let meta = meta$O(o);
    let value1 = unwrap_(meta_o$((i32)(meta)));
    let value2 = meta_v$((i32)(unwrap_(meta)));

    // var o     = (O$i32){ .is_some = true, .payload = { { .some = 123 } } };
    // let meta  = meta$O(o);
    // let value = meta_V$((i32)(meta.payload->some));

    printf("o.some: %p\n", &o.payload->some);
    printf("meta.some: %p\n", meta.payload->some.inner);
    printf("value1: %d\n", value1);
    printf("value2: %d\n", value2);

    let_ignore = as$(bool)(as$(u32)((as$(isize)((null)))));
    better();
    return 0;
}

// TODO: better for
// for ( iter(...), capture(...) )

#if UNUSED_CODE
void better(void) {
    var_(a_u, A$$(8, u32)) = init$A({ 0, 1, 2, 3, 4, 5, 6, 7 });
    var_(a_i, A$$(9, i32)) = init$A({ 0, 1, 2, 3, 4, 5, 6, 7, 8 });
    var_(a_f, A$$(10, f32)) = init$A({ 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f });
    var_(r_u, R) = $r(0, 11);

    for_(((r_u))(i_u) {
        printf("i_u: %zu\n", i_u);
    });

    for_(($rf(0), $a(a_u))(i_u, e_u) {
        printf("e_u[%zu]: %u\n", i_u, *e_u);
    });

    for_(((r_u), $a(a_u), $a(a_i))(i_u, e_u, e_i) {
        printf("e_u[%zu]: %u\n", i_u, *e_u);
        printf("e_i[%zu]: %d\n", i_u, *e_i);
    });

    for_(($rf(0), $a(a_u), $a(a_i), $a(a_f))(i_u, e_u, e_i, e_f) {
        printf("e_u[%zu]: %u\n", i_u, *e_u);
        printf("e_i[%zu]: %d\n", i_u, *e_i);
        printf("e_f[%zu]: %f\n", i_u, *e_f);
    });
}
#endif /* UNUSED_CODE */

/* for: declaration =========================================================*/
#define $fwd $_fwd,
#define $rev $_rev,
#define $asc $_asc,
#define $desc $_desc,

#define for_(...) pp_overload(__for, __VA_ARGS__)(__VA_ARGS__)
#define __for_1(/*(_iter...)(_capture...) { ... }*/...) \
    __exec__for_(pp_defer(__emit__for_$_fwd)(__sep0__for_ __VA_ARGS__))
#define __for_2(_$dir_type, /*(_iter...)(_capture...) { ... }*/...) \
    __exec__for_(pp_defer(pp_cat(__emit__for_, _$dir_type))(__sep0__for_ __VA_ARGS__))
#define __exec__for_(...) __VA_ARGS__
#define __sep0__for_(_iters...) (_iters), __sep1__for_
#define __sep1__for_(_captures...) (_captures),
#define __emit__for_$_fwd(_iters, _captures, _block...) __emitNext__for_(_iters, _captures, (_block))
// #define __emitNext__for_(_iters, _captures, _block) \
//     0(_iters), 1(_captures), 2(_block)
#define __emit__for_$_rev(_iters, _captures, _block...) __emitNext__for_$_rev(_iters, _captures, (_block))
// #define __emitNext__for_$_rev(_iters, _captures, _block) \
//     0(_iters), 1(_captures), 2(_block)
#define __emit__for_$_asc(_iters, _captures, _block...) __emitNext__for_(_iters, _captures, (_block))
#define __emit__for_$_desc(_iters, _captures, _block...) __emitNext__for_$_rev(_iters, _captures, (_block))

/* for: implementation ======================================================*/
/* for - common functions ===================================================*/
#define __for__expandIters(_iters...) _iters
#define __for__expandIter(/*<_iter>|<_tag,_iter>*/...) __VA_ARGS__
#define __for__expandIterIds(__ids...) __ids
#define __for__expandIterId(__id) __id
#define __for__expandCaptures(_captures...) _captures
#define __for__expandCapture(/*<_capture>|$ignore*/...) __VA_ARGS__
#define __for__expandBlock(_block...) _block

#define __for__initIter(__iter_id, _iter) \
    pp_overload(__for__initIter, __for__expandIter _iter)(__iter_id, __for__expandIter _iter)
#define __for__initIter_1(__iter_id, ...) __for__initIter_1Emit(__iter_id, __VA_ARGS__)
#define __for__initIter_1Emit(__iter_id, _iter) \
    const R __iter_id = _iter
#define __for__initIter_2(__iter_id, ...) __for__initIter_2Emit(__iter_id, __VA_ARGS__)
#define __for__initIter_2Emit(__iter_id, _tag, _iter...) \
    let __iter_id = _iter

#define __for__lenIter(_iter) \
    pp_overload(__for__lenIter, __for__expandIter _iter)(__for__expandIter _iter)
#define __for__lenIter_1(...) __for__lenIter_1Emit(__VA_ARGS__)
#define __for__lenIter_1Emit(_iter) len$R
#define __for__lenIter_2(...) __for__lenIter_2Emit(__VA_ARGS__)
#define __for__lenIter_2Emit(_tag, _iter) pp_cat(len, _tag)

#define __for__atIter(__step, __iter_id, _iter) \
    pp_overload(__for__atIter, __for__expandIter _iter)(__step, __iter_id, __for__expandIter _iter)
#define __for__atIter_1(...) __for__atIter_1Emit(__VA_ARGS__)
#define __for__atIter_1Emit(__step, __iter_id, _iter) \
    at$R(__iter_id, __step)
#define __for__atIter_2(...) __for__atIter_2Emit(__VA_ARGS__)
#define __for__atIter_2Emit(__step, __iter_id, _tag, _iter) \
    pp_cat(at, _tag)(__iter_id, __step)

/* for - emitters (forward) =================================================*/
#define __emitNext__for_(_iters, _captures, _block) \
    pp_overload(__emitNext__for, __for__expandIters _iters)(_iters, _captures, _block)
#define __emitNext__for_1(_iters, _captures, _block) __emit__for_1( \
    pp_uniqTok(len), pp_uniqTok(step), (pp_uniqTok(iter_id0)), \
    _iters, _captures, _block \
)
#define __emitNext__for_2(_iters, _captures, _block) __emit__for_2( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1) \
    ), \
    _iters, _captures, _block \
)
#define __emitNext__for_3(_iters, _captures, _block) __emit__for_3( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1), \
        pp_uniqTok(iter_id2) \
    ), \
    _iters, _captures, _block \
)
#define __emitNext__for_4(_iters, _captures, _block) __emit__for_4( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1), \
        pp_uniqTok(iter_id2), \
        pp_uniqTok(iter_id3) \
    ), \
    _iters, _captures, _block \
)

// #define __emit__for_1(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_1(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_1__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_1__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = 0; __step < __len; ++__step) { \
        __for_1__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})
#define __for_1__initIters(__iter_id0, _iter0) \
    __for__initIter(__iter_id0, _iter0)
#define __for_1__measureLen(__iter_id0, _iter0...) \
    __for__lenIter(_iter0)(__iter_id0)
#define __for_1__captureIters(__step, _capture0, __iter_id0, _iter0...) \
    let _capture0 = __for__atIter(__step, __iter_id0, _iter0)

// #define __emit__for_2(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_2(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_2__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_2__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = 0; __step < __len; ++__step) { \
        __for_2__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})
#define __for_2__initIters(...) __for_2__initItersNext(__VA_ARGS__)
#define __for_2__initItersNext(__iter_id0, __iter_id1, _iter0, _iter1) \
    __for__initIter(__iter_id0, _iter0); \
    __for__initIter(__iter_id1, _iter1)
#define __for_2__measureLen(...) __for_2__measureLenNext(__VA_ARGS__)
#define __for_2__measureLenNext(__iter_id0, __iter_id1, _iter0, _iter1) \
    prim_min2( \
        __for__lenIter(_iter0)(__iter_id0), \
        __for__lenIter(_iter1)(__iter_id1) \
    )
#define __for_2__captureIters(...) __for_2__captureItersNext(__VA_ARGS__)
#define __for_2__captureItersNext(__step, _capture0, _capture1, __iter_id0, __iter_id1, _iter0, _iter1) \
    let _capture0 = __for__atIter(__step, __iter_id0, _iter0); \
    let _capture1 = __for__atIter(__step, __iter_id1, _iter1)

// #define __emit__for_3(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_3(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_3__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_3__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = 0; __step < __len; ++__step) { \
        __for_3__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})
#define __for_3__initIters(...) __for_3__initItersNext(__VA_ARGS__)
#define __for_3__initItersNext(__iter_id0, __iter_id1, __iter_id2, _iter0, _iter1, _iter2) \
    __for__initIter(__iter_id0, _iter0); \
    __for__initIter(__iter_id1, _iter1); \
    __for__initIter(__iter_id2, _iter2)
#define __for_3__measureLen(...) __for_3__measureLenNext(__VA_ARGS__)
#define __for_3__measureLenNext(__iter_id0, __iter_id1, __iter_id2, _iter0, _iter1, _iter2) \
    prim_min3( \
        __for__lenIter(_iter0)(__iter_id0), \
        __for__lenIter(_iter1)(__iter_id1), \
        __for__lenIter(_iter2)(__iter_id2) \
    )
#define __for_3__captureIters(...) __for_3__captureItersNext(__VA_ARGS__)
#define __for_3__captureItersNext(__step, _capture0, _capture1, _capture2, __iter_id0, __iter_id1, __iter_id2, _iter0, _iter1, _iter2) \
    let _capture0 = __for__atIter(__step, __iter_id0, _iter0); \
    let _capture1 = __for__atIter(__step, __iter_id1, _iter1); \
    let _capture2 = __for__atIter(__step, __iter_id2, _iter2)

// #define __emit__for_4(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_4(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_4__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_4__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = 0; __step < __len; ++__step) { \
        __for_4__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})
#define __for_4__initIters(...) __for_4__initItersNext(__VA_ARGS__)
#define __for_4__initItersNext(__iter_id0, __iter_id1, __iter_id2, __iter_id3, _iter0, _iter1, _iter2, _iter3) \
    __for__initIter(__iter_id0, _iter0); \
    __for__initIter(__iter_id1, _iter1); \
    __for__initIter(__iter_id2, _iter2); \
    __for__initIter(__iter_id3, _iter3)
#define __for_4__measureLen(...) __for_4__measureLenNext(__VA_ARGS__)
#define __for_4__measureLenNext(__iter_id0, __iter_id1, __iter_id2, __iter_id3, _iter0, _iter1, _iter2, _iter3) \
    prim_min4( \
        __for__lenIter(_iter0)(__iter_id0), \
        __for__lenIter(_iter1)(__iter_id1), \
        __for__lenIter(_iter2)(__iter_id2), \
        __for__lenIter(_iter3)(__iter_id3) \
    )
#define __for_4__captureIters(...) __for_4__captureItersNext(__VA_ARGS__)
#define __for_4__captureItersNext(__step, _capture0, _capture1, _capture2, _capture3, __iter_id0, __iter_id1, __iter_id2, __iter_id3, _iter0, _iter1, _iter2, _iter3) \
    let _capture0 = __for__atIter(__step, __iter_id0, _iter0); \
    let _capture1 = __for__atIter(__step, __iter_id1, _iter1); \
    let _capture2 = __for__atIter(__step, __iter_id2, _iter2); \
    let _capture3 = __for__atIter(__step, __iter_id3, _iter3)

/* for - emitters (reverse) =================================================*/
#define __emitNext__for_$_rev(_iters, _captures, _block) \
    pp_overload(__emitNext__for_$_rev, __for__expandIters _iters)(_iters, _captures, _block)
#define __emitNext__for_$_rev_1(_iters, _captures, _block) __emit__for_$_rev_1( \
    pp_uniqTok(len), pp_uniqTok(step), (pp_uniqTok(iter_id0)), \
    _iters, _captures, _block \
)
#define __emitNext__for_$_rev_2(_iters, _captures, _block) __emit__for_$_rev_2( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1) \
    ), \
    _iters, _captures, _block \
)
#define __emitNext__for_$_rev_3(_iters, _captures, _block) __emit__for_$_rev_3( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1), \
        pp_uniqTok(iter_id2) \
    ), \
    _iters, _captures, _block \
)
#define __emitNext__for_$_rev_4(_iters, _captures, _block) __emit__for_$_rev_4( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1), \
        pp_uniqTok(iter_id2), \
        pp_uniqTok(iter_id3) \
    ), \
    _iters, _captures, _block \
)

// #define __emit__for_$_rev_1(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_1(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_1__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_1__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = __len; __step > 0; --__step) { \
        __for_1__captureIters(__step - 1, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})

// #define __emit__for_$_rev_2(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_2(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_2__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_2__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = __len; __step > 0; --__step) { \
        __for_2__captureIters(__step - 1, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})

// #define __emit__for_$_rev_3(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_3(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_3__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_3__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = __len; __step > 0; --__step) { \
        __for_3__captureIters(__step - 1, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})

// #define __emit__for_$_rev_4(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_4(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_4__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_4__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = __len; __step > 0; --__step) { \
        __for_4__captureIters(__step - 1, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})

void better(void) {
    var_(a_u, A$$(8, u32)) = init$A({ 0, 1, 2, 3, 4, 5, 6, 7 });
    var_(a_i, A$$(9, i32)) = init$A({ 0, 1, 2, 3, 4, 5, 6, 7, 8 });
    var_(a_f, A$$(10, f32)) = init$A({ 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f });
    var_(r_u, R) = $r(0, 11);

    printf("--- for_((r_u))(i_u) {}) ---\n");
    for_(((r_u))(i_u) {
        printf("next: ");
        printf("- i_u: %zu\n", i_u);
    });
    printf("--- for_(($r(0, 11))($ignore) {}) ---\n");
    {
        usize i_u = 0;
        for_(($r(0, 11))($ignore) {
            printf("- i_u: %zu\n", i_u++);
        });
    }
    printf("--- for_(($a(a_u))(e_u) {}) ---\n");
    {
        usize i_u = 0;
        for_(($a(a_u))(e_u) {
            printf("- a_u[%zu]: %u\n", i_u++, *e_u);
        });
    }

    printf("--- for_(($rf(0), $a(a_u))(i_u, e_u) {}) ---\n");
    for_(($rf(0), $a(a_u))(i_u, e_u) {
        printf("- e_u[%zu]: %u\n", i_u, *e_u);
    });
    printf("--- for_(($s(ref$A(a_u)), (r_u))(e_u, i_u) {}) ---\n");
    for_(($s(ref$A(a_u)), (r_u))(e_u, i_u) {
        printf("- e_u[%zu]: %u\n", i_u, *e_u);
    });

    printf("--- for_(((r_u), $a(a_u), $a(a_i))(i_u, e_u, e_i) {}) ---\n");
    for_($rev((r_u), $a(a_u), $a(a_i))(i_u, e_u, e_i) {
        printf("- e_u[%zu]: %u\n", i_u, *e_u);
        printf("- e_i[%zu]: %d\n", i_u, *e_i);
    });

    printf("--- for_(($rf(0), $a(a_u), $s(suffix$A(a_i, 7)), $a(a_f))(i_u, e_u, e_i, e_f) {}) ---\n");
    for_(($rf(0), $a(a_u), $s(suffix$A(a_i, 3)), $a(a_f))(i_u, e_u, e_i, e_f) {
        printf("- e_u[%zu]: %u\n", i_u, *e_u);
        printf("- e_i[%zu]: %d\n", i_u, *e_i);
        printf("- e_f[%zu]: %.2f\n", i_u, *e_f);
    });
    printf("--- for_(($rf(0), $a(a_u), $s(suffix$A(a_i, 7)), $a(a_f))(i_u, e_u, e_i, e_f) {}) ---\n");
    for_($rev($rf(0), $a(a_u), $s(suffix$A(a_i, 3)), $a(a_f))(i_u, e_u, e_i, e_f) {
        printf("- e_u[%zu]: %u\n", i_u, *e_u);
        printf("- e_i[%zu]: %d\n", i_u, *e_i);
        printf("- e_f[%zu]: %.2f\n", i_u, *e_f);
    });
}

#undef lit$
#define lit$(/*(_T){_initial...}*/... /*(_T)*/) \
    __lit$__exec(pp_defer(__lit$__emit)(__lit$__sep __VA_ARGS__))
#define __lit$__exec(...) __VA_ARGS__
#define __lit$__sep(_T...) _T,
#define __lit$__emit(_T, _initial...) __lit$__emitNext(_T, (_initial))
#define __lit$__emitNext(_T, _initial...) \
    ((_T)__lit$__expandInitial _initial)
#define __lit$__expandInitial(_initial...) _initial

#undef make$
#define make$(/*(_T){_initial...}*/... /*(_T)*/) \
    __make$__exec(pp_defer(__make$__emit)(__make$__sep __VA_ARGS__))
#define __make$__exec(...) __VA_ARGS__
#define __make$__sep(_T...) _T,
#define __make$__emit(_T, _initial...) __make$__emitNext(_T, (_initial))
#define __make$__emitNext(_T, _initial...) \
    (*lit$((_T[1]){ [0] = __make$__expandInitial _initial }))
#define __make$__expandInitial(_initial...) _initial

#undef create$
#define create$(/*(_T){_initial...}*/... /*(P$$(_T))*/) \
    __create$__exec(pp_defer(__create$__emit)(__create$__sep __VA_ARGS__))
#define __create$__exec(...) __VA_ARGS__
#define __create$__sep(_T...) _T,
#define __create$__emit(_T, _initial...) __create$__emitNext(_T, (_initial))
#define __create$__emitNext(_T, _initial...) \
    (&make$((_T)__create$__expandInitial _initial))
#define __create$__expandInitial(_initial...) _initial

#undef type$
#define type$(/*(_T)(_raw...)*/... /*(_T)*/) \
    /* TODO: Add type checking */ \
    __type$__exec(pp_defer(__type$__emit)(__type$__sep __VA_ARGS__))
#define __type$__exec(...) __VA_ARGS__
#define __type$__sep(_T...) _T, __type$__sepRaw
#define __type$__sepRaw(_raw...) _raw
#define __type$__emit(_T, _raw...) __type$__emitNext(_T, _raw)
#define __type$__emitNext(_T, _raw...) make$((_T){ .as_raw = _raw })
#define type$O$(/*(_T)(_raw...)*/... /*(_T)*/) \
    __type$O$__exec(pp_defer(__type$O$__emit)(__type$O$__sep __VA_ARGS__))
#define __type$O$__exec(...) __VA_ARGS__
#define __type$O$__sep(_T...) _T, __type$O$__sepRaw
#define __type$O$__sepRaw(_raw...) _raw
#define __type$O$__emit(_T, _raw...) __type$O$__emitNext(_T, _raw)
#define __type$O$__emitNext(_T, _raw...) make$((O$(_T)){ .as_raw = _raw.as_raw })
#define type$E$(/*(_T)(_raw...)*/... /*(_T)*/) \
    __type$E$__exec(pp_defer(__type$E$__emit)(__type$E$__sep __VA_ARGS__))
#define __type$E$__exec(...) __VA_ARGS__
#define __type$E$__sep(_T...) _T, __type$E$__sepRaw
#define __type$E$__sepRaw(_raw...) _raw
#define __type$E$__emit(_T, _raw...) __type$E$__emitNext(_T, _raw)
#define __type$E$__emitNext(_T, _raw...) make$((E$(_T)){ .as_raw = _raw.as_raw })

#undef tpl_id
#define tpl_id$T(_id, _T) pp_join($, _id, _T)

#define ArrList$(_T...) tpl_id$T(ArrList, _T)
typedef struct ArrList$raw {
    var_(type, TypeInfo);
    var_(items, S$raw);
    var_(cap, usize);
} ArrList$raw;
tpl$E$(ArrList$raw);

typedef struct Foo {
    usize id;
    f64 value;
} Foo;
tpl$P$(Foo);
tpl$S$(Foo);

#define tpl$ArrList$(T) \
    typedef union ArrList$(T) { \
        ArrList$raw as_raw; \
        struct { \
            var_(type, TypeInfo); \
            var_(items, S$(T)); \
            var_(cap, usize); \
        }; \
    } ArrList$(T)

#define tpl$ArrList_empty$(T...) \
    $static fn_((tpl_id$T(ArrList_empty, T)(void))(E$(ArrList$(T)))) { \
        return type$((ArrList$(T))(ArrList_empty$raw(typeInfo$(T)))); \
    }
#define tpl$ArrList_init$(T...) \
    $static fn_((tpl_id$T(ArrList_init, T)(mem_Allocator allocator, usize cap))(E$(ArrList$(T)))) { \
        return type$E$((ArrList$(T))(ArrList_init$raw(typeInfo$(T), allocator, cap))); \
    }
#define tpl$ArrList_initBuf$(T...) \
    $static fn_((tpl_id$T(ArrList_initBuf, T)(S$(T) buf))(ArrList$(T))) { \
        return type$((ArrList$(T))(ArrList_initBuf$raw(typeInfo$(T), metaMut$S(buf)))); \
    }
#define tpl$ArrList_fini$(T...) \
    $static fn_((tpl_id$T(ArrList_fini, T)(P$$(ArrList$(T)) self, mem_Allocator allocator))(void)) { \
        return ArrList_fini$raw(&self->as_raw, allocator); \
    }

/* typedef struct mem_Allocator {
} mem_Allocator;

$inline_always
fn_((ArrList_empty$raw(TypeInfo type))(ArrList$raw)) {
    return make$((ArrList$raw){ .type = type, .items = { .ptr = null, .len = 0 }, .cap = 0 });
}

$inline_always
fn_((ArrList_init$raw(TypeInfo type, mem_Allocator allocator, usize cap))(E$ArrList$raw) $scope) {
    // let items = try_(mem_Allocator_alloc(allocator, type, cap));
    // return_ok({ .type = type, .items = items, .cap = cap });
} $unscoped_(fn);

$inline_always
fn_((ArrList_initBuf$raw(TypeInfo type, meta_S$raw buf))(ArrList$raw)) {
    return make$((ArrList$raw){ .type = type, .items = { .ptr = buf.ptr, .len = 0 }, .cap = buf.len });
}

$inline_always
fn_((ArrList_fini$raw(ArrList$raw* self, mem_Allocator allocator))(void)) {
    // mem_Allocator_free(allocator, meta_any(self->items));
}

tpl$ArrList$(Foo);
tpl$E$(ArrList$Foo);
tpl$ArrList_init$(Foo);
tpl$ArrList_initBuf$(Foo);
tpl$ArrList_fini$(Foo); */


// $inline_always
// fn_((ArrList_init$Foo(void))(ArrList$Foo)) {
//     return type$((ArrList$Foo)(ArrList_init$raw(typeInfo$(Foo))));
// }
