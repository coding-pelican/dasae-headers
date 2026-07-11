#pragma once
#ifndef prl_tpl_Arr__included
#define prl_tpl_Arr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

/* Array Anonymous */
#define A$$(_$N, _$T...) \
    TypeOf(union { \
        var_(val, $A$(_$N, _$T)); \
        var_(as_raw, A$raw); \
        var_(ref_raw $like_ref, A$raw); \
    })
/* Array Alias */
#define A$(_$N, _$T...) tpl$(tpl$(A, _$N), _$T)
/* Array Template */
#define T_decl_A$(_$N, _$T...) \
    $maybe_unused typedef union A$(_$N, _$T) A$(_$N, _$T)
#define T_impl_A$(_$N, _$T...) \
    union A$(_$N, _$T) { \
        var_(val, $A$(_$N, _$T)); \
        var_(as_raw, A$raw); \
        var_(ref_raw $like_ref, A$raw); \
    }
#define T_use_A$(_$N, _$T...) \
    T_decl_A$(_$N, _$T); \
    T_impl_A$(_$N, _$T)

#define A_n$(_$T... /*(usize)*/) A_len$(_$T)
#define A_T$(_$T...) TypeOf(null$(_$T*)->val[0])
#define A_TUnqual$(_$T...) TypeOfUnqual(null$(_$T*)->val[0])

/* Array Operations */
#define asgA(_$p_a, _$v_a...) asgA1(_$p_a, _$v_a)
#define asgA1(_$p_a, _$v_a...) asg(_$p_a, _$v_a, (val))
#define asgA2(_$p_a, _$v_a...) asg(_$p_a, _$v_a, (val.val))
#define asgA3(_$p_a, _$v_a...) asg(_$p_a, _$v_a, (val.val.val))
#define asgA4(_$p_a, _$v_a...) asg(_$p_a, _$v_a, (val.val.val.val))

#define A_zero$(/*(_$T)*/...) l$(__VA_ARGS__ A_zero())
#define zeroA$ A_zero$
#define A_zero() A_init({})
#define zeroA A_zero
#define A_init$(/*(_$T){ _$initial...}*/... /*(_$T)*/) \
    __step_inline__A_init$(pp_defer(__emit_inline__A_init$)(__param_parse__A_init$ __VA_ARGS__))
#define __step_inline__A_init$(...) __VA_ARGS__
#define __param_parse__A_init$(...) __VA_ARGS__,
#define __emit_inline__A_init$(_$T, _$initial...) l$((_$T)A_init(_$initial))
#define initA$ A_init$
#define A_init(_$initial...) { .val = _$initial }
#define initA A_init
#define A_from_static$(/*(_$T){ _$initial...}*/... /*(A(N,_$T))*/) \
    __step__A_from_static$__emit(__step__A_from_static$__parseT __VA_ARGS__)
#define __step__A_from_static$__parseT(_$T...) _$T,
#define __step__A_from_static$__emit(...) \
    __inline__A_from_static$(__VA_ARGS__)
#define __inline__A_from_static$(_$T, _$initial...) \
    l$((A$$((sizeOf$(TypeOf((_$T[])_$initial)) / sizeOf$(_$T)), _$T)){ .val = _$initial })
#define fromA_static$ A_from_static$

#define A_from$(/*(_$T){ _$initial...}*/... /*(A(N,_$T))*/) \
    __step__A_from$__emit(__step__A_from$__parseT __VA_ARGS__)
#define __step__A_from$__parseT(_$T...) _$T,
#define __step__A_from$__emit(...) \
    __inline__A_from$(pp_uniqTok(a), __VA_ARGS__)
#define __inline__A_from$(__a, _$T, _$initial...) copy(local_({ \
    typedef _$T A_T; \
    typedef TypeOf(l$((A_T[])_$initial)) A_Val; \
    typedef A$$(sizeOf$(A_Val) / sizeOf$(A_T), A_T) A_Self; \
    local_return_(*ptrCast$((A_Self*)(raw_memcpy( \
        &l0$((A_Self)), \
        &l$((A_T[])_$initial), \
        sizeOf$(A_Val) \
    )))); \
}))
#define fromA$ A_from$

#define A_cat$(/*(_$T)(_$lhs, _$rhs)*/... /*(_$T)*/) \
    __op__A_cat$(__op__A_cat$__parseT __VA_ARGS__)
#define __op__A_cat$(...) \
    __op__A_cat$__emit(__VA_ARGS__)
#define __op__A_cat$__parseT(_$T...) \
    _$T, __op__A_cat$__parseParams
#define __op__A_cat$__parseParams(_$lhs, _$rhs...) \
    pp_uniqTok(lhs), _$lhs, pp_uniqTok(rhs), _$rhs
#define __op__A_cat$__emit(_$T, __lhs, _$lhs, __rhs, _$rhs...) ({ \
    var __lhs = &(_$lhs); \
    var __rhs = &(_$rhs); \
    typedef union { \
        T_embed$(struct { \
            var_(lhs, TypeOf(*__lhs)); \
            var_(rhs, TypeOf(*__rhs)); \
        }); \
        var_(catted, _$T); \
    } Catting; \
    l$((Catting){ .lhs = *__lhs, .rhs = *__rhs }).catted; \
})
#define catA$ A_cat$
#define A_cat(_$lhs, _$rhs...) \
    A_cat$((A$$(A_n$(TypeOf(_$lhs)) + A_n$(TypeOf(_$rhs)), A_T$(TypeOf(_$lhs))))(_$lhs, _$rhs))
#define catA A_cat

#define A_repeat$(/*(_$T: A(N, T))(_$a: A(M, T))*/... /*(_$T)*/) \
    __op__A_repeat$(__VA_ARGS__)
#define repeatA$ A_repeat$
#define __op__A_repeat$(...) \
    __op__A_repeat$__step(__op__A_repeat$__parseT __VA_ARGS__)
#define __op__A_repeat$__parseT(_$T...) \
    _$T, __op__A_repeat$__parseA
#define __op__A_repeat$__parseA(_$a...) \
    pp_uniqTok(a), pp_uniqTok(ret), pp_uniqTok(i), _$a
#define __op__A_repeat$__step(...) \
    __op__A_repeat$__emit(__VA_ARGS__)
#define __op__A_repeat$__emit(_$T, __a, __ret, __i, _$a...) ({ \
    let __a = _$a; \
    var_(__ret, _$T) = A_zero(); \
    claim_assert(A_len(__a) > 0); \
    claim_assert(eqlType$(A_TUnqual$(_$T), A_TUnqual$(TypeOf(__a)))); \
    loop_inline_(for) (usize __i = 0; __i < A_len(__ret); ++__i) { \
        *A_at((__ret)[__i]) = *A_at((__a)[__i % A_len(__a)]); \
    } \
    __ret; \
})

#define A_extract$(/*(_$T: A(N, T))(_$a: A(M, T), _$first: usize)*/... /*(_$T)*/) \
    __op__A_extract$(__VA_ARGS__)
#define extractA$ A_extract$
#define __op__A_extract$(...) \
    __op__A_extract$__step(__op__A_extract$__parseT __VA_ARGS__)
#define __op__A_extract$__parseT(_$T...) \
    _$T, __op__A_extract$__parseA
#define __op__A_extract$__parseA(_$a, _$first...) \
    pp_uniqTok(a), pp_uniqTok(first), pp_uniqTok(ret), pp_uniqTok(i), _$a, _$first
#define __op__A_extract$__step(...) \
    __op__A_extract$__emit(__VA_ARGS__)
#define __op__A_extract$__emit(_$T, __a, __first, __ret, __i, _$a, _$first...) ({ \
    let __a = _$a; \
    let_(__first, usize) = _$first; \
    var_(__ret, _$T) = A_zero(); \
    claim_assert(eqlType$(A_TUnqual$(_$T), A_TUnqual$(TypeOf(__a)))); \
    claim_assert(__first + A_len(__ret) <= A_len(__a)); \
    loop_inline_(for) (usize __i = 0; __i < A_len(__ret); ++__i) { \
        *A_at((__ret)[__i]) = *A_at((__a)[__first + __i]); \
    } \
    __ret; \
})

#define A_val(_$a /*: A$$(_$N,_$T)*/... /*($A$(_$N,_$T))*/) ((_$a).val)
#define valA A_val
#define A_ptr(_$a /*: A$$(_$N,_$T)*/... /*(P$$(_$T))*/) (&*A_val(_$a))
#define ptrA A_ptr
#define A_len$(_$T...) A_len(*as$(_$T*)(0))
#define lenA$ A_len$
#define A_len(_$a /*: A$$(_$N,_$T)*/... /*(usize)*/) countOf$(TypeOf((_$a).val))
#define lenA A_len

#define A_ref$(/*(_$ST: S(_$T))(_$a: A(_$N,_$T))*/... /*(_$ST)*/) \
    __op__A_ref$(__op__A_ref$__parseST __VA_ARGS__)
#define __op__A_ref$(...) __op__A_ref$__emit(__VA_ARGS__)
#define __op__A_ref$__parseST(_$ST...) _$ST,
#define __op__A_ref$__emit(_$ST, _$a...) \
    l$((_$ST){ .ptr = A_ptr(_$a), .len = A_len(_$a) })
#define A_ref(_$a /*: A(_$N,_$T)*/... /*(_$ST)*/) \
    T_switch$((A_T$(TypeOf(_$a)))( \
        T_qual$((const A_TUnqual$(TypeOf(_$a)))( \
            A_ref$((S_const$$(A_T$(TypeOf(_$a))))(_$a)) \
        )), \
        T_qual$((A_TUnqual$(TypeOf(_$a)))( \
            A_ref$((S$$(A_T$(TypeOf(_$a))))(_$a)) \
        )) \
    ))
#define refA A_ref

#define A_at(/*(_$self: A(_$N,_$T))[_$idx: usize]*/... /*(P(_$T))*/) \
    __op__A_at(__op__A_at__parse __VA_ARGS__)
#define __op__A_at__parse(_$a...) pp_uniqTok(a), _$a, pp_uniqTok(idx),
#define __op__A_at(...) __op__A_at__emit(__VA_ARGS__)
#define __op__A_at__emit(__a, _$a, __idx, _$idx...) ({ \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _$idx)); \
    let_(__a, TypeOf(&(_$a))) = &(_$a); \
    claim_assert_fmt(__idx < A_len(*__a), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, A_len(*__a)); \
    &A_ptr(*__a)[__idx]; \
})
#define atA A_at

#define A_slice$(/*(_$ST: S(_$T))((_$a: A(_$N,_$T))(_$range: R))*/... /*(_$ST)*/) \
    __op__A_slice$(__op__A_slice$__parseST __VA_ARGS__)
#define __op__A_slice$(...) __op__A_slice$__emit(__VA_ARGS__)
#define __op__A_slice$__parseST(_$ST...) _$ST, __op__A_slice$__parseA
#define __op__A_slice$__parseA(_$a...) pp_uniqTok(a), _$a, pp_uniqTok(range),
#define __op__A_slice$__emit(_$ST, __a, _$a, __range, _$range...) ({ \
    let_(__range, R) = _$range; \
    let_(__a, TypeOf(_$a)*) = &(_$a); \
    claim_assert_fmt(R_isValid(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= A_len(*__a), "Invalid slice range: end({:uz}) > len({:uz})", __range.end, A_len(*__a)); \
    l$((_$ST){ .ptr = &A_ptr(*__a)[__range.begin], .len = R_len(__range) }); \
})
#define sliceA$ A_slice$
#define A_slice(/*(_$a: A(_$N,_$T))(_$range: R)*/... /*(S(_$T))*/) \
    __op__A_slice(__op__A_slice__parse __VA_ARGS__)
#define __op__A_slice(...) __op__A_slice__emit(__VA_ARGS__)
#define __op__A_slice__parse(_$a...) pp_uniqTok(a), _$a, pp_uniqTok(range),
#define __op__A_slice__emit(__a, _$a, __range, _$range...) ({ \
    let_(__range, R) = _$range; \
    let_(__a, TypeOf(_$a)*) = &(_$a); \
    claim_assert_fmt(R_isValid(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= A_len(*__a), "Invalid slice range: end({:uz}) > len({:uz})", __range.end, A_len(*__a)); \
    T_switch$((TypeOf(*__a))( \
        T_qual$((const TypeOfUnqual(*__a))( \
            l$((S_const$$(A_T$(TypeOf(*__a)))){ .ptr = &A_ptr(*__a)[__range.begin], .len = R_len(__range) }) \
        )), \
        T_qual$((TypeOfUnqual(*__a))( \
            l$((S$$(A_T$(TypeOf(*__a)))){ .ptr = &A_ptr(*__a)[__range.begin], .len = R_len(__range) }) \
        )) \
    )); \
})
#define sliceA A_slice

#define A_prefix$(/*(_$ST: S(_$T))(_$a: A(_$N,_$T))(_$end: usize)*/... /*(_$ST)*/) \
    __op__A_prefix$(__op__A_prefix$__parseST __VA_ARGS__)
#define __op__A_prefix$(...) __op__A_prefix$__emit(__VA_ARGS__)
#define __op__A_prefix$__parseST(_$ST...) _$ST, __op__A_prefix$__parseA
#define __op__A_prefix$__parseA(_$a...) pp_uniqTok(a), _$a, pp_uniqTok(end),
#define __op__A_prefix$__emit(_$ST, __a, _$a, __end, _$end...) ({ \
    let_(__end, usize) = _$end; \
    let_(__a, TypeOf(_$a)*) = &(_$a); \
    claim_assert_fmt(__end <= A_len(*__a), "Invalid slice range: end({:uz}) > len({:uz})", __end, A_len(*__a)); \
    l$((_$ST){ .ptr = A_ptr(*__a), .len = __end }); \
})
#define prefixA$ A_prefix$
#define A_prefix(/*(_$a: A(_$N,_$T))(_$end: usize)*/... /*(S(_$T))*/) \
    __op__A_prefix(__op__A_prefix__parse __VA_ARGS__)
#define __op__A_prefix(...) __op__A_prefix__emit(__VA_ARGS__)
#define __op__A_prefix__parse(_$a...) pp_uniqTok(a), _$a, pp_uniqTok(end),
#define __op__A_prefix__emit(__a, _$a, __end, _$end...) ({ \
    let_(__end, usize) = _$end; \
    let_(__a, TypeOf(_$a)*) = &(_$a); \
    claim_assert_fmt(__end <= A_len(*__a), "Invalid slice range: end({:uz}) > len({:uz})", __end, A_len(*__a)); \
    T_switch$((TypeOf(*__a))( \
        T_qual$((const TypeOfUnqual(*__a))( \
            l$((S_const$$(A_T$(TypeOf(*__a)))){ .ptr = A_ptr(*__a), .len = __end }) \
        )), \
        T_qual$((TypeOfUnqual(*__a))( \
            l$((S$$(A_T$(TypeOf(*__a)))){ .ptr = A_ptr(*__a), .len = __end }) \
        )) \
    )); \
})
#define prefixA A_prefix

#define A_suffix$(/*(_$ST: S(_$T))((_$a: A(_$N,_$T))(_$begin: usize))*/... /*(_$ST)*/) \
    __op__A_suffix$(__op__A_suffix$__parseST __VA_ARGS__)
#define __op__A_suffix$(...) __op__A_suffix$__emit(__VA_ARGS__)
#define __op__A_suffix$__parseST(_$ST...) _$ST, __op__A_suffix$__parseA
#define __op__A_suffix$__parseA(_$a...) pp_uniqTok(a), _$a, pp_uniqTok(begin),
#define __op__A_suffix$__emit(_$ST, __a, _$a, __begin, _$begin...) ({ \
    let_(__begin, usize) = _$begin; \
    let_(__a, TypeOf(_$a)*) = &(_$a); \
    claim_assert_fmt(__begin <= A_len(*__a), "Invalid slice range: begin({:uz}) > len({:uz})", __begin, A_len(*__a)); \
    l$((_$ST){ .ptr = &A_ptr(*__a)[__begin], .len = A_len(*__a) - __begin }); \
})
#define suffixA$ A_suffix$
#define A_suffix(/*(_$a: A(_$N,_$T))(_$begin: usize)*/... /*(S(_$T))*/) \
    __op__A_suffix(__op__A_suffix__parse __VA_ARGS__)
#define __op__A_suffix(...) __op__A_suffix__emit(__VA_ARGS__)
#define __op__A_suffix__parse(_$a...) pp_uniqTok(a), _$a, pp_uniqTok(begin),
#define __op__A_suffix__emit(__a, _$a, __begin, _$begin...) ({ \
    let_(__begin, usize) = _$begin; \
    let_(__a, TypeOf(_$a)*) = &(_$a); \
    claim_assert_fmt(__begin <= A_len(*__a), "Invalid slice range: begin({:uz}) > len({:uz})", __begin, A_len(*__a)); \
    T_switch$((TypeOf(*__a))( \
        T_qual$((const TypeOfUnqual(*__a))( \
            l$((S_const$$(A_T$(TypeOf(*__a)))){ .ptr = &A_ptr(*__a)[__begin], .len = A_len(*__a) - __begin }) \
        )), \
        T_qual$((TypeOfUnqual(*__a))( \
            l$((S$$(A_T$(TypeOf(*__a)))){ .ptr = &A_ptr(*__a)[__begin], .len = A_len(*__a) - __begin }) \
        )) \
    )); \
})
#define suffixA A_suffix


/*========== Macros and Definitions =========================================*/

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_Arr__included */
