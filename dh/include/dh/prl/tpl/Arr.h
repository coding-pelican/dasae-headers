#ifndef prl_tpl_Arr__included
#define prl_tpl_Arr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../../core.h"

/*========== Macros and Declarations ========================================*/

/* Array Anonymous */
#define A$$(_N, _T...) \
    union { \
        var_(val, $A$(_N, _T)); \
        var_(as_raw, A$raw); \
        var_(ref_raw $like_ref, A$raw); \
    }
/* Array Alias */
#define A$(_N, _T...) tpl$(tpl$(A, _N), _T)
/* Array Template */
#define T_decl_A$(_N, _T...) \
    $maybe_unused typedef union A$(_N, _T) A$(_N, _T)
#define T_impl_A$(_N, _T...) \
    union A$(_N, _T) { \
        var_(val, $A$(_N, _T)); \
        var_(as_raw, A$raw); \
        var_(ref_raw $like_ref, A$raw); \
    }
#define T_use_A$(_N, _T...) \
    T_decl_A$(_N, _T); \
    T_impl_A$(_N, _T)

#define A_n$(_T... /*(usize)*/) A_len$(_T)
#define A_T$(_T...) TypeOf(null$(_T*)->val[0])
#define A_TUnqual$(_T...) TypeOfUnqual(null$(_T*)->val[0])

/* Array Operations */
#define asgA(_p_a, _v_a...) asgA1(_p_a, _v_a)
#define asgA1(_p_a, _v_a...) asg(_p_a, _v_a, (val))
#define asgA2(_p_a, _v_a...) asg(_p_a, _v_a, (val.val))
#define asgA3(_p_a, _v_a...) asg(_p_a, _v_a, (val.val.val))
#define asgA4(_p_a, _v_a...) asg(_p_a, _v_a, (val.val.val.val))

#define A_zero$(_T...) l$((_T)A_zero())
#define zeroA$ A_zero$
#define A_zero() A_init({})
#define zeroA A_zero
#define A_init$(/*(_T){ _initial...}*/... /*(_T)*/) \
    __step_inline__A_init$(pp_defer(__emit_inline__A_init$)(__param_parse__A_init$ __VA_ARGS__))
#define __step_inline__A_init$(...) __VA_ARGS__
#define __param_parse__A_init$(...) __VA_ARGS__,
#define __emit_inline__A_init$(_T, _initial...) l$((_T)A_init(_initial))
#define initA$ A_init$
#define A_init(_initial...) { .val = _initial }
#define initA A_init
#define A_from$(/*(_T){_initial...}*/...) __A_from__step(pp_defer(__A_from__emit)(__A_from__parseT __VA_ARGS__))
#define __A_from__step(...) __VA_ARGS__
#define __A_from__parseT(_T...) _T,
#define __A_from__emit(_T, _a...) \
    l$((A$$((sizeOf$(TypeOf((_T[])_a)) / sizeOf$(_T)), _T)){ .val = _a })
#define fromA$ A_from$

#define A_val(_a /*: A$$(_N,_T)*/... /*($A$(_N,_T))*/) ((_a).val)
#define valA A_val
#define A_ptr(_a /*: A$$(_N,_T)*/... /*(P$$(_T))*/) (&*A_val(_a))
#define ptrA A_ptr
#define A_len$(_T...) A_len(*as$(_T*)(0))
#define lenA$ A_len$
#define A_len(_a /*: A$$(_N,_T)*/... /*(usize)*/) countOf$(TypeOf((_a).val))
#define lenA A_len

#define A_ref$(/*(_ST: S(_T))(_a: A(_N,_T))*/... /*(_ST)*/) \
    __op__A_ref$(__op__A_ref$__parseST __VA_ARGS__)
#define __op__A_ref$(...) __op__A_ref$__emit(__VA_ARGS__)
#define __op__A_ref$__parseST(_ST...) _ST,
#define __op__A_ref$__emit(_ST, _a...) \
    l$((_ST){ .ptr = A_ptr(_a), .len = A_len(_a) })
#define A_ref(_a /*: A(_N,_T)*/... /*(_ST)*/) \
    T_switch$((A_T$(TypeOf(_a)))( \
        T_qual$((const A_TUnqual$(TypeOf(_a)))( \
            A_ref$((S_const$$(A_T$(TypeOf(_a))))(_a)) \
        )), \
        T_qual$((A_TUnqual$(TypeOf(_a)))( \
            A_ref$((S$$(A_T$(TypeOf(_a))))(_a)) \
        )) \
    ))
#define refA A_ref

#define A_at(/*(_self: A(_N,_T))[_idx: usize]*/... /*(P(_T))*/) \
    __op__A_at(__op__A_at__parse __VA_ARGS__)
#define __op__A_at__parse(_a...) pp_uniqTok(a), _a, pp_uniqTok(idx),
#define __op__A_at(...) __op__A_at__emit(__VA_ARGS__)
#define __op__A_at__emit(__a, _a, __idx, _idx...) ({ \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _idx)); \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    claim_assert_fmt(__idx < A_len(*__a), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, A_len(*__a)); \
    A_ptr(*__a) + __idx; \
})
#define atA A_at

#define A_slice$(/*(_ST: S(_T))((_a: A(_N,_T))(_range: R))*/... /*(_ST)*/) \
    __op__A_slice$(__op__A_slice$__parseST __VA_ARGS__)
#define __op__A_slice$(...) __op__A_slice$__emit(__VA_ARGS__)
#define __op__A_slice$__parseST(_ST...) _ST, __op__A_slice$__parseA
#define __op__A_slice$__parseA(_a...) pp_uniqTok(a), _a, pp_uniqTok(range),
#define __op__A_slice$__emit(_ST, __a, _a, __range, _range...) ({ \
    let_(__range, R) = _range; \
    let_(__a, TypeOf(_a)*) = &(_a); \
    claim_assert_fmt(R_isValid(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= A_len(*__a), "Invalid slice range: end({:uz}) > len({:uz})", __range.end, A_len(*__a)); \
    l$((_ST){ .ptr = &A_ptr(*__a)[__range.begin], .len = R_len(__range) }); \
})
#define sliceA$ A_slice$
#define A_slice(/*(_a: A(_N,_T))(_range: R)*/... /*(S(_T))*/) \
    __op__A_slice(__op__A_slice__parse __VA_ARGS__)
#define __op__A_slice(...) __op__A_slice__emit(__VA_ARGS__)
#define __op__A_slice__parse(_a...) pp_uniqTok(a), _a, pp_uniqTok(range),
#define __op__A_slice__emit(__a, _a, __range, _range...) ({ \
    let_(__range, R) = _range; \
    let_(__a, TypeOf(_a)*) = &(_a); \
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

#define A_prefix$(/*(_ST: S(_T))(_a: A(_N,_T))(_end: usize)*/... /*(_ST)*/) \
    __op__A_prefix$(__op__A_prefix$__parseST __VA_ARGS__)
#define __op__A_prefix$(...) __op__A_prefix$__emit(__VA_ARGS__)
#define __op__A_prefix$__parseST(_ST...) _ST, __op__A_prefix$__parseA
#define __op__A_prefix$__parseA(_a...) pp_uniqTok(a), _a, pp_uniqTok(end),
#define __op__A_prefix$__emit(_ST, __a, _a, __end, _end...) ({ \
    let_(__end, usize) = _end; \
    let_(__a, TypeOf(_a)*) = &(_a); \
    claim_assert_fmt(__end <= A_len(*__a), "Invalid slice range: end({:uz}) > len({:uz})", __end, A_len(*__a)); \
    l$((_ST){ .ptr = A_ptr(*__a), .len = __end }); \
})
#define prefixA$ A_prefix$
#define A_prefix(/*(_a: A(_N,_T))(_end: usize)*/... /*(S(_T))*/) \
    __op__A_prefix(__op__A_prefix__parse __VA_ARGS__)
#define __op__A_prefix(...) __op__A_prefix__emit(__VA_ARGS__)
#define __op__A_prefix__parse(_a...) pp_uniqTok(a), _a, pp_uniqTok(end),
#define __op__A_prefix__emit(__a, _a, __end, _end...) ({ \
    let_(__end, usize) = _end; \
    let_(__a, TypeOf(_a)*) = &(_a); \
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

#define A_suffix$(/*(_ST: S(_T))((_a: A(_N,_T))(_begin: usize))*/... /*(_ST)*/) \
    __op__A_suffix$(__op__A_suffix$__parseST __VA_ARGS__)
#define __op__A_suffix$(...) __op__A_suffix$__emit(__VA_ARGS__)
#define __op__A_suffix$__parseST(_ST...) _ST, __op__A_suffix$__parseA
#define __op__A_suffix$__parseA(_a...) pp_uniqTok(a), _a, pp_uniqTok(begin),
#define __op__A_suffix$__emit(_ST, __a, _a, __begin, _begin...) ({ \
    let_(__begin, usize) = _begin; \
    let_(__a, TypeOf(_a)*) = &(_a); \
    claim_assert_fmt(__begin <= A_len(*__a), "Invalid slice range: begin({:uz}) > len({:uz})", __begin, A_len(*__a)); \
    l$((_ST){ .ptr = A_ptr(*__a) + __begin, .len = A_len(*__a) - __begin }); \
})
#define suffixA$ A_suffix$
#define A_suffix(/*(_a: A(_N,_T))(_begin: usize)*/... /*(S(_T))*/) \
    __op__A_suffix(__op__A_suffix__parse __VA_ARGS__)
#define __op__A_suffix(...) __op__A_suffix__emit(__VA_ARGS__)
#define __op__A_suffix__parse(_a...) pp_uniqTok(a), _a, pp_uniqTok(begin),
#define __op__A_suffix__emit(__a, _a, __begin, _begin...) ({ \
    let_(__begin, usize) = _begin; \
    let_(__a, TypeOf(_a)*) = &(_a); \
    claim_assert_fmt(__begin <= A_len(*__a), "Invalid slice range: begin({:uz}) > len({:uz})", __begin, A_len(*__a)); \
    T_switch$((TypeOf(*__a))( \
        T_qual$((const TypeOfUnqual(*__a))( \
            l$((S_const$$(A_T$(TypeOf(*__a)))){ .ptr = A_ptr(*__a) + __begin, .len = A_len(*__a) - __begin }) \
        )), \
        T_qual$((TypeOfUnqual(*__a))( \
            l$((S$$(A_T$(TypeOf(*__a)))){ .ptr = A_ptr(*__a) + __begin, .len = A_len(*__a) - __begin }) \
        )) \
    )); \
})
#define suffixA A_suffix

#define A_cat$(/*(_T)(_lhs, _rhs)*/... /*(_T)*/) \
    __op__A_cat$(__op__A_cat$__parseT __VA_ARGS__)
#define __op__A_cat$(...) \
    __op__A_cat$__emit(__VA_ARGS__)
#define __op__A_cat$__parseT(_T...) \
    _T, __op__A_cat$__parseParams
#define __op__A_cat$__parseParams(_lhs, _rhs...) \
    pp_uniqTok(lhs), _lhs, pp_uniqTok(rhs), _rhs
#define __op__A_cat$__emit(_T, __lhs, _lhs, __rhs, _rhs...) ({ \
    var __lhs = &(_lhs); \
    var __rhs = &(_rhs); \
    typedef union { \
        struct { \
            var_(lhs, TypeOf(*__lhs)); \
            var_(rhs, TypeOf(*__rhs)); \
        }; \
        var_(catted, _T); \
    } Catting; \
    l$((Catting){ .lhs = *__lhs, .rhs = *__rhs }).catted; \
})
#define catA$ A_cat$
#define A_cat(_lhs, _rhs...) \
    A_cat$((A$$(A_n$(TypeOf(_lhs)) + A_n$(TypeOf(_rhs)), A_T$(TypeOf(_lhs))))(_lhs, _rhs))
#define catA A_cat

/*========== Macros and Definitions =========================================*/

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_Arr__included */
