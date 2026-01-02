#ifndef META_OPS_H
#define META_OPS_H

/*========== Includes =======================================================*/

#include "dh/core.h"

#undef pp_not
#undef __pp_not_0
#undef __pp_not_1
#undef pp_if_
#undef __pp_if_0
#undef __pp_if_1
#undef pp_then_
#undef pp_else_

#define pp_compl(_b) pp_cat(__pp_compl_, _b)
#define __pp_compl_0 1
#define __pp_compl_1 0

#define pp_bitand(_x) pp_cat(__pp_bitand_, _x)
#define __pp_bitand_0(_y) 0
#define __pp_bitand_1(_y) _y
#define pp_bitor(_x) pp_cat(__pp_bitor_, _x)
#define __pp_bitor_0(_y) 0
#define __pp_bitor_1(_y) _y
#define pp_bitnot(_x) pp_cat(__pp_bitnot_, _x)
#define __pp_bitnot_0 1
#define __pp_bitnot_1 0
#define pp_bitxor(_x) pp_cat(__pp_bitxor_, _x)
#define __pp_bitxor_0(_y) 0
#define __pp_bitxor_1(_y) _y

#define __pp_check_n(_x, _n, ...) _n
#define pp_check(...) __pp_check_n(__VA_ARGS__, 0, )
#define pp_probe(_x) _x, 1,

#define pp_isParen(_x) pp_check(pp_isParenProbe _x)
#define pp_isParenProbe(...) pp_probe(~)

#define pp_not(_x) pp_check(pp_cat(__pp_not_, _x))
#define __pp_not_0 pp_probe(~)

#define pp_bool(_x) pp_compl(pp_not(_x))

#define pp_iif(_c) pp_cat(__pp_iif_, _c)
#define __pp_iif_0(_t, ...) __VA_ARGS__
#define __pp_iif_1(_t, ...) _t
#define pp_if_(_c) pp_iif(pp_bool(_c))
#define pp_then_(...) __VA_ARGS__
#define pp_else_(...) __VA_ARGS__

#define pp_Tok_prim_ord(x, y) pp_isParen( \
    pp_Tok_cmp__##x(pp_Tok_cmp__##y)(()) \
)
#define pp_Tok_isComparable(x) pp_isParen(pp_cat(pp_Tok_cmp__, x)(()))
#define pp_Tok_ne(x, y) \
    pp_iif(pp_bitand(pp_Tok_isComparable(x))(pp_Tok_isComparable(y)))( \
        pp_Tok_prim_ord, \
        1 pp_ignore \
    )(x, y)
#define pp_Tok_eq(x, y) pp_compl(pp_Tok_ne(x, y))

#define pp_comma() ,
#define pp_comma_if_(_n) pp_if_(_n)(pp_comma, pp_ignore)()

#define pp_Tok_cmp__const(x) x
#define ignore_and_end(...) ignore_end ignore_end
#define ignore_after_const const ignore_and_end(
#define ignore_open (
#define ignore_end )

#define Tok_isConst$(_T...) \
    __isConstType__exec(pp_defer(__isConstType__unwrap) __isConstType__eval(_T))
#define __isConstType__exec(...) __VA_ARGS__
#define __isConstType__unwrap(...) __VA_ARGS__
#define __isConstType__eval(_T...) \
    ignore_open pp_defer(pp_Tok_eq)(const, pp_cat(ignore_after_, _T) ignore_end)
#define Tok_removeConst$(_const_T...) \
    pp_cat(__Tok_removeConst$__remove_, _const_T)
#define __Tok_removeConst$__remove_const

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
#define P$$(_T...) pp_if_(Tok_isConst$(_T))( \
    pp_then_(__P_const$$(Tok_removeConst$(_T))), \
    pp_else_(__P$$(_T)) \
)
#define __P_const$$(_T...) $P_const$(_T)
#define __P$$(_T...) $P$(_T)

/* Pointer Alias */
#define P$(_T...) pp_if_(Tok_isConst$(_T))( \
    pp_then_(__P_const$(Tok_removeConst$(_T))), \
    pp_else_(__P$(_T)) \
)
#define __P_const$(_T...) pp_join($, P_const, _T)
#define __P$(_T...) pp_join($, P, _T)

/* Pointer Template */
#define T_use_P$(_T...) \
    typedef $P$(const _T) P$(const _T); \
    typedef $P$(_T) P$(_T)

/* Slice Type ========================================================*/

/* Slice Anonymous */
#define S$$(_T...) pp_if_(Tok_isConst$(_T))( \
    pp_then_(__S_const$$(Tok_removeConst$(_T))), \
    pp_else_(__S$$(_T)) \
)
#define __S_const$$(_T...) \
    union { \
        struct { \
            var_(ptr, P$$(const _T)); \
            var_(len, usize); \
        }; \
        var_(as_raw, S$(const raw)); \
        var_(ref_raw, TypeOf(S$(const raw) $like_ref)); \
    }
#define __S$$(_T...) \
    union { \
        struct { \
            var_(ptr, P$$(_T)); \
            var_(len, usize); \
        }; \
        var_(as_raw, S$(raw)); \
        var_(ref_raw, TypeOf(S$(raw) $like_ref)); \
        var_(as_const, __S_const$$(_T)); \
    }

/* Slice Alias */
#define S$(_T...) pp_if_(Tok_isConst$(_T))( \
    pp_then_(__S_const$(Tok_removeConst$(_T))), \
    pp_else_(__S$(_T)) \
)
#define __S_const$(_T...) pp_join($, S_const, _T)
#define __S$(_T...) pp_join($, S, _T)

/* Slice Template */
#define T_use_S$(_T...) \
    typedef union S$(const _T) { \
        struct { \
            var_(ptr, $P$(const _T)); \
            var_(len, usize); \
        }; \
        var_(as_raw, S$(const raw)); \
        var_(ref_raw, TypeOf(S$(const raw) $like_ref)); \
    } S$(const _T); \
    typedef union S$(_T) { \
        struct { \
            var_(ptr, $P$(_T)); \
            var_(len, usize); \
        }; \
        var_(as_raw, S$(raw)); \
        var_(ref_raw, TypeOf(S$(raw) $like_ref)); \
        var_(as_const, S$(const _T)); \
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
#define T_use_A$(_N, _T...) \
    typedef union A$(_N, _T) { \
        var_(val, $A$(_N, _T)); \
        var_(as_raw, A$raw); \
        var_(ref_raw, TypeOf(A$raw $like_ref)); \
    } A$(_N, _T)

#define initA(_initial...) { .val = _initial }

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
#define T_use_O$(_T...) \
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
#define T_use_E$(_T...) \
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

#define zero$A() init$A({})
#define zero$A$(/*(_N,_T)*/... /*(A$(_N,_T))*/) init$A$(__VA_ARGS__{})
#define zero$A$$(/*(_N,_T)*/... /*(A$$(_N,_T))*/) init$A$$(__VA_ARGS__{})

#define init$A(_initial...) { .val = _initial }
#define __lit_init$A__step(...) __VA_ARGS__
#define __lit_init$A__parseT(_N, _T...) _N, _T,
#define __lit_init$A__emit(_AliasFn, _N, _T, _initial...) \
    lit$((_AliasFn(_N, _T))init$A(_initial))
#define init$A$(/*(_N, _T){_initial...}*/... /*(A$(_N,_T))*/) \
    __lit_init$A__step(pp_defer(__lit_init$A__emit)(A$, __lit_init$A__parseT __VA_ARGS__))
#define init$A$$(/*(_N, _T){_initial...}*/... /*(A$$(_N,_T))*/) \
    __lit_init$A__step(pp_defer(__lit_init$A__emit)(A$$, __lit_init$A__parseT __VA_ARGS__))

#define zero$S() init$S(null, 0)
#define zero$S$(/*(_T)*/... /*(S$(_T))*/) init$S$(__VA_ARGS__(null, 0))
#define zero$S$$(/*(_T)*/... /*(S$$(_T))*/) init$S$$(__VA_ARGS__(null, 0))

#define init$S(_ptr, _len...) { .ptr = _ptr, .len = _len }
#define __lit_init$S__step(...) __VA_ARGS__
#define __lit_init$S__parseT(_T...) _T, __lit_init$S__parsePtrLen
#define __lit_init$S__parsePtrLen(_ptr, _len...) _ptr, _len
#define __lit_init$S__emit(_AliasFn, _T, _ptr, _len...) \
    lit$((_AliasFn(_T))init$S(_ptr, _len))
#define init$S$(/*(_T)(_ptr: P$$(_T), _len: usize)*/... /*(S$(_T))*/) \
    __lit_init$S__step(pp_defer(__lit_init$S__emit)(S$, __lit_init$S__parseT __VA_ARGS__))
#define init$S$$(/*(_T)(_ptr: P$$(_T), _len: usize)*/... /*(S$$(_T))*/) \
    __lit_init$S__step(pp_defer(__lit_init$S__emit)(S$$, __lit_init$S__parseT __VA_ARGS__))

#define ref$V(_v /*: T*/... /*P$$(_T)*/) ref(_v)
#define ref$A(_a /*: A$$(_N,_T)*/... /*(S$(_T))*/) \
    init$S$$((TypeOf(*ptr$A(_a)))(ptr$A(_a), len$A(_a)))
#define ref$A$(/*((_T))(a: A$$(_N,_T))*/... /*((_T))*/) \
    __ref$A__step(pp_defer(__ref$A__emit)(__ref$A__parseT __VA_ARGS__))
#define __ref$A__step(...) __VA_ARGS__
#define __ref$A__parseT(_T...) _T, __ref$A__parseA
#define __ref$A__parseA(_a...) _a
#define __ref$A__emit(_T, _a...) \
    init$S$((_T)(ptr$A(_a), len$A(_a)))

#define deref$P(_p /*: P$$(_T)*/... /*(_T)*/) deref(_p)
#define deref$S$(/*(_N, _T)(_s: S$$(_T))*/... /*(A$(_N,_T))*/) \
    __step_inline__deref$S$(pp_defer(__emit_inline__deref$S$)(__param_parse__deref$S$ __VA_ARGS__))
#define __step_inline__deref$S$(...) __VA_ARGS__
#define __param_parse__deref$S$(...) __VA_ARGS__, __param_next__deref$S$
#define __param_next__deref$S$(...) pp_uniqTok(s), __VA_ARGS__
#define __emit_inline__deref$S$(_N, _T, __s, _s...) (*({ \
    let_(__s, TypeOf(_s)) = _s; \
    debug_assert_fmt(len$S(__s) == _N, "length mismatch: len(%zu) != N(%zu)", len$S(__s), _N); \
    as$(A$(_N, _T) (*)(ensureNonnull(ptr$S(__s)))); \
}))
#define deref$S$$(/*(_N, _T)(_s: S$$(_T))*/... /*(A$$(_N,_T))*/) \
    __step_inline__deref$S$$(pp_defer(__emit_inline__deref$S$$)(__param_parse__deref$S$$ __VA_ARGS__))
#define __step_inline__deref$S$$(...) __VA_ARGS__
#define __param_parse__deref$S$$(...) __VA_ARGS__, __param_next__deref$S$$
#define __param_next__deref$S$$(...) pp_uniqTok(s), __VA_ARGS__
#define __emit_inline__deref$S$$(_N, _T, __s, _s...) (*({ \
    let_(__s, TypeOf(_s)) = _s; \
    debug_assert_fmt(len$S(__s) == _N, "length mismatch: len(%zu) != N(%zu)", len$S(__s), _N); \
    as$(A$$(_N, _T) (*)(ensureNonnull(ptr$S(__s)))); \
}))

#define mutCast$P(_p /*: P$$(_T)*/... /*(P_const $$(_T))*/) as$(const TypeOfUnqual (*_p) (*)(_p))
#define constCast$P(_p /*: P_const$$(_T)*/... /*(P$$(_T))*/) /* TODO: Implement */
#define mutCast$S(_s /*: S$$(_T)*/... /*(S_const$$(_T))*/) (_s.as_const)
#define constCast$S(_s /*: S_const$$(_T)*/... /*(S$$(_T))*/) /* TODO: Implement */

#define val$A(_a /*: A$$(_N,_T)*/... /*($A$(_N,_T))*/) ((_a).val)
#define ptr$A(_a /*: A$$(_N,_T)*/... /*(P$$(_T))*/) (&*val$A(_a))
#define ptr$S(_s /*: S$$(_T)*/... /*(P$$(_T))*/) ((_s).ptr)

#define len$A(_a /*: A$$(_N,_T)*/... /*(usize)*/) countOf$(TypeOf((_a).val))
#define len$A$(_T...) len$A(*as$(_T*)(null))
#define len$S(_s /*: S$$(_T)*/... /*(usize)*/) ((_s).len)

#define at$A(_a /*: A$$(_N,_T)*/, _idx /*: usize*/... /*(P$$(_T))*/) \
    pp_expand(pp_defer(block_inline__at$A)(param_expand__at$A(_a, _idx)))
#define param_expand__at$A(_a, _idx...) pp_uniqTok(a), pp_uniqTok(idx), _a, _idx
#define block_inline__at$A(__a, __idx, _a, _idx...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__idx, usize) = _idx; \
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

#define slice$S(/*_s: S$$(_T), $r(_begin, _end): R*/... /*(S$$(_T))*/) __param_expand__slice$S(__VA_ARGS__)
#define __param_expand__slice$S(_s, _range...) __block_inline__slice$S(pp_uniqTok(s), pp_uniqTok(range), _s, _range)
#define __block_inline__slice$S(__s, __range, _s, _range...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$S(__s)); \
    lit$((TypeOf(__s))init$S(&ptr$S(__s)[__range.begin], len$R(__range))); \
})
#define slice$S$(/*(_T)(_s: S$$(_T), $r(_begin, _end): R)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__slice$S$)(__param_expand__slice$S$ __VA_ARGS__))
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
    lit$((TypeOf(__s))init$S(ptr$S(__s), __end)); \
})
#define prefix$S$(/*(_T)(_s: S$$(_T), _end: usize)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__prefix$S$)(__param_expand__prefix$S$ __VA_ARGS__))
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
    lit$((TypeOf(__s))init$S(ptr$S(__s) + __begin, len$S(__s) - __begin)); \
})
#define suffix$S$(/*(_T)(_s: S$$(_T), _begin: usize)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__suffix$S$)(__param_expand__suffix$S$ __VA_ARGS__))
#define __param_expand__suffix$S$(...) __VA_ARGS__, pp_uniqTok(s), pp_uniqTok(begin), __param_next__suffix$S$
#define __param_next__suffix$S$(...) __VA_ARGS__
#define __block_inline__suffix$S$(_T, __s, __begin, _s, _begin...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= len$S(__s), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$S(__s)); \
    init$S$((_T)(ptr$S(__s) + __begin, len$S(__s) - __begin)); \
})

#define slice$P(/*_p: P$$(_T), $r(_begin:0, _end:1): R*/... /*(S_const$$(_T))*/) __param_expand__slice$P(__VA_ARGS__)
#define __param_expand__slice$P(_p, _range...) __block_inline__slice$P(pp_uniqTok(p), pp_uniqTok(range), _p, _range)
#define __block_inline__slice$P(__p, __range, _p, _range...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= 1, "Invalid slice range: end(%zu) > 1", __range.end); \
    init$S$$((TypeOf(*__p))(__p, len$R(__range))); \
})
#define slice$P$(/*(_T)(_p: P$$(_T), $r(_begin:0, _end:1): R)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__slice$P$)(__param_expand__slice$P$ __VA_ARGS__))
#define __param_expand__slice$P$(...) __VA_ARGS__, pp_uniqTok(p), pp_uniqTok(range), __param_next__slice$P$
#define __param_next__slice$P$(...) __VA_ARGS__
#define __block_inline__slice$P$(_T, __p, __range, _p, _range...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= 1, "Invalid slice range: end(%zu) > 1", __range.end); \
    init$S$((_T)(__p, len$R(__range))); \
})
#define prefix$P(/*_p: P$$(_T), _end: usize*/... /*(S_const$$(_T))*/) __param_expand__prefix$P(__VA_ARGS__)
#define __param_expand__prefix$P(_p, _end...) __block_inline__prefix$P(pp_uniqTok(p), pp_uniqTok(end), _p, _end)
#define __block_inline__prefix$P(__p, __end, _p, _end...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__end, usize) = _end; \
    debug_assert_fmt(__end <= 1, "Invalid slice range: end(%zu) > 1", __end); \
    init$S$$((TypeOf(*__p))(__p, __end)); \
})
#define prefix$P$(/*(_T)(_p: P$$(_T), _end: usize)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__prefix$P$)(__param_expand__prefix$P$ __VA_ARGS__))
#define __param_expand__prefix$P$(...) __VA_ARGS__, pp_uniqTok(p), pp_uniqTok(end), __param_next__prefix$P$
#define __param_next__prefix$P$(...) __VA_ARGS__
#define __block_inline__prefix$P$(_T, __p, __end, _p, _end...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__end, usize) = _end; \
    debug_assert_fmt(__end <= 1, "Invalid slice range: end(%zu) > 1", __end); \
    init$S$((_T)(__p, __end)); \
})
#define suffix$P(/*_p: P$$(_T), _begin: usize*/... /*(S_const$$(_T))*/) __param_expand__suffix$P(__VA_ARGS__)
#define __param_expand__suffix$P(_p, _begin...) __block_inline__suffix$P(pp_uniqTok(p), pp_uniqTok(begin), _p, _begin)
#define __block_inline__suffix$P(__p, __begin, _p, _begin...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= 1, "Invalid slice range: begin(%zu) > 1", __begin); \
    init$S$$((TypeOf(*__p))(__p, __begin)); \
})
#define suffix$P$(/*(_T)(_p: P$$(_T), _begin: usize)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__suffix$P$)(__param_expand__suffix$P$ __VA_ARGS__))
#define __param_expand__suffix$P$(...) __VA_ARGS__, pp_uniqTok(p), pp_uniqTok(begin), __param_next__suffix$P$
#define __param_next__suffix$P$(...) __VA_ARGS__
#define __block_inline__suffix$P$(_T, __p, __begin, _p, _begin...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= 1, "Invalid slice range: begin(%zu) > 1", __begin); \
    init$S$((_T)(__p, __begin)); \
})

#define slice$A(/*_a: A$$(_N,_T), $r(_begin, _end): R*/... /*(S_const$$(_T))*/) __param_expand__slice$A(__VA_ARGS__)
#define __param_expand__slice$A(_a, _range...) __block_inline__slice$A(pp_uniqTok(a), pp_uniqTok(range), _a, _range)
#define __block_inline__slice$A(__a, __range, _a, _range...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$A(*__a)); \
    init$S$$((TypeOf(*ptr$A(*__a)))(&val$A(*__a)[__range.begin], len$R(__range))); \
})
#define slice$A$(/*(_T)(_a: A$$(_N,_T), $r(_begin, _end): R)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__slice$A$)(__param_expand__slice$A$ __VA_ARGS__))
#define __param_expand__slice$A$(...) __VA_ARGS__, pp_uniqTok(a), pp_uniqTok(range), __param_next__slice$A$
#define __param_next__slice$A$(...) __VA_ARGS__
#define __block_inline__slice$A$(_T, __a, __range, _a, _range...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$A(*__a)); \
    init$S$((_T)(&val$A(*__a)[__range.begin], len$R(__range))); \
})
#define prefix$A(/*_a: A$$(_N,_T), _end: usize*/... /*(S_const$$(_T))*/) __param_expand__prefix$A(__VA_ARGS__)
#define __param_expand__prefix$A(_a, _end...) __block_inline__prefix$A(pp_uniqTok(a), pp_uniqTok(end), _a, _end)
#define __block_inline__prefix$A(__a, __end, _a, _end...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__end, usize) = _end; \
    debug_assert_fmt(__end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __end, len$A(*__a)); \
    init$S$$((TypeOf(*ptr$A(*__a)))(ptr$A(*__a), __end)); \
})
#define prefix$A$(/*(_T)(_a: A$$(_N,_T), _end: usize)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__prefix$A$)(__param_expand__prefix$A$ __VA_ARGS__))
#define __param_expand__prefix$A$(...) __VA_ARGS__, pp_uniqTok(a), pp_uniqTok(end), __param_next__prefix$A$
#define __param_next__prefix$A$(...) __VA_ARGS__
#define __block_inline__prefix$A$(_T, __a, __end, _a, _end...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__end, usize) = _end; \
    debug_assert_fmt(__end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __end, len$A(*__a)); \
    init$S$((_T)(ptr$A(*__a), __end)); \
})
#define suffix$A(/*_a: A$$(_N,_T), _begin: usize*/... /*(S_const$$(_T))*/) __param_expand__suffix$A(__VA_ARGS__)
#define __param_expand__suffix$A(_a, _begin...) __block_inline__suffix$A(pp_uniqTok(a), pp_uniqTok(begin), _a, _begin)
#define __block_inline__suffix$A(__a, __begin, _a, _begin...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= len$A(*__a), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$A(*__a)); \
    init$S$$((TypeOf(*ptr$A(*__a)))(ptr$A(*__a) + __begin, len$A(*__a) - __begin)); \
})
#define suffix$A$(/*(_T)(_a: A$$(_N,_T), _begin: usize)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__suffix$A$)(__param_expand__suffix$A$ __VA_ARGS__))
#define __param_expand__suffix$A$(...) __VA_ARGS__, pp_uniqTok(a), pp_uniqTok(begin), __param_next__suffix$A$
#define __param_next__suffix$A$(...) __VA_ARGS__
#define __block_inline__suffix$A$(_T, __a, __begin, _a, _begin...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= len$A(*__a), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$A(*__a)); \
    init$S$((_T)(ptr$A(*__a) + __begin, len$A(*__a) - __begin)); \
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



void array_asg() {
    A$$(3, int) a = initA({ 0, 1, 2 });
    A$$(3, int) b = initA({ 3, 4, 5 });
    asg(&a.val[0], b.val[0]);
    asg(&a, b, (val));
    A$$(2, A$$(2, i32)) c = initA({
        initA({ 0, 1 }),
        initA({ 2, 3 }),
    });
    A$$(2, A$$(2, i32)) d = initA({
        initA({ 4, 5 }),
        initA({ 6, 7 }),
    });
    asg(&c, d, (val->val));
    A$$(2, O$$(i32)) e = initA({
        some(0),
        some(1),
    });
    A$$(2, O$$(i32)) f = initA({
        some(0),
        some(1),
    });
    asg(&e, f, (val[0].payload->some));
}


#define asg(_p_lhs, _rhs, _field...) ({ \
    let_(__p_lhs, TypeOf(_p_lhs)) = _p_lhs; \
    let_(__rhs, TypeOf(_rhs)) = _rhs; \
    *_p_lhs = *as$(TypeOf(__p_lhs)()(&__rhs)); \
    __p_lhs; \
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

#define EXAMPLE_USAGE 1
#if EXAMPLE_USAGE

T_use_P$(i32);
T_use_S$(i32);
T_use_O$(i32);
T_use_E$(i32);
fn_((example(void))(void)) {
    A$$(32, i32) array = zero$A();
    var inferred_slice = ref$A(array); // anonymous union S_const$$(i32) type
    var inferred_slice_mut = ref$A(array); // anonymous union S$$(i32) type
    // inferred_slice = inferred_slice_mut.as_const;           // can't assign between different anonymous unions
    const A$$(32, i32) array_other = zero$A();
    let inferred_slice_other = ref$A(array_other);
    at$A(array, 30);

    let inferred_subslice_1 = slice$S(inferred_slice, $r(0, 10));
    let inferred_subslice_2 = slice$S$((i32)(inferred_slice, $r(0, 10)));
    let inferred_subslice_other = slice$S$((const i32)(inferred_slice_other, $r(0, 10)));

    S$(const i32) slice = ref$A$((const i32)(array)); // ref$A$ returns S_const$(i32)
    S$(i32) slice_mut = ref$A$((i32)(array)); // refMut$A$ returns S$(i32)
    const i32* ptr = at$S(slice, 0);
    i32* ptr_mut = at$S(slice_mut, 0);
    // i32*               ptr_mut_from_const = at$S(slice, 0); // can't this because the slice element is immutable
    let array_const = init$A$$((3, i32){ 1, 2, 3 });
    let slice_const = ref$A$((const i32)(array_const));
    let_ignore = deref$S$$((3, i32)(slice));
    let_ignore = inferred_slice;
    let_ignore = inferred_slice_mut;
    let_ignore = ptr;
    let_ignore = ptr_mut;
    let_ignore = slice_const;

    let_ignore = inferred_subslice_1;
    let_ignore = inferred_subslice_2;
    let_ignore = inferred_subslice_other;
}

#endif /* EXAMPLE_USAGE */
