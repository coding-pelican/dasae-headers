#ifndef types_Arr__included
#define types_Arr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "raw.h"

/*========== Macros and Declarations ========================================*/

/* Array Anonymous */
#define A$$(_N, _T...) \
    union { \
        var_(val, $A$(_N, _T)); \
        var_(as_raw, A$raw); \
        var_(ref_raw $like_ref, A$raw); \
    }
/* Array Alias */
#define A$(_N, _T...) pp_join3($, A, _N, _T)
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

/* Array Operations */
#define zero$A()                                  init$A({})
#define zero$A$(/*(_N,_T)*/... /*(A$(_N,_T))*/)   init$A$(__VA_ARGS__{})
#define zero$A$$(/*(_N,_T)*/... /*(A$$(_N,_T))*/) init$A$$(__VA_ARGS__{})

#define asgA(_p_a, _v_a...)  asgA1(_p_a, _v_a)
#define asgA1(_p_a, _v_a...) asg(_p_a, _v_a, (val))
#define asgA2(_p_a, _v_a...) asg(_p_a, _v_a, (val.val))
#define asgA3(_p_a, _v_a...) asg(_p_a, _v_a, (val.val.val))
#define asgA4(_p_a, _v_a...) asg(_p_a, _v_a, (val.val.val.val))

#define init$A(_initial...) { .val = _initial }
#define init$A$(/*(_N, _T){_initial...}*/... /*(A$(_N,_T))*/) \
    __lit_init$A__step(pp_defer(__lit_init$A__emit)(A$, __lit_init$A__parseT __VA_ARGS__))
#define init$A$$(/*(_N, _T){_initial...}*/... /*(A$$(_N,_T))*/) \
    __lit_init$A__step(pp_defer(__lit_init$A__emit)(A$$, __lit_init$A__parseT __VA_ARGS__))

#define ref$A(_a /*: A$$(_N,_T)*/... /*(S$(_T))*/) \
    init$S$$((TypeOf(*ptr$A(_a)))(ptr$A(_a), len$A(_a)))
#define ref$A$(/*((_T))(a: A$$(_N,_T))*/... /*((_T))*/) \
    __ref$A__step(pp_defer(__ref$A__emit)(__ref$A__parseT __VA_ARGS__))

#define val$A(_a /*: A$$(_N,_T)*/... /*($A$(_N,_T))*/) ((_a).val)
#define ptr$A(_a /*: A$$(_N,_T)*/... /*(P$$(_T))*/)    (&*val$A(_a))
#define lenA                                           len$A
#define len$A(_a /*: A$$(_N,_T)*/... /*(usize)*/)      countOf$(TypeOf((_a).val))
#define len$A$(_T...)                                  len$A(*as$((_T*)(0)))

#define atA                                                          at$A
#define at$A(_a /*: A$$(_N,_T)*/, _idx /*: usize*/... /*(P$$(_T))*/) pp_expand(pp_defer(block_inline__at$A)(param_expand__at$A(_a, _idx)))

#define slice$A(/*_a: A$$(_N,_T), $r(_begin, _end): R*/... /*(S_const$$(_T))*/)       __param_expand__slice$A(__VA_ARGS__)
#define slice$A$(/*(_T)(_a: A$$(_N,_T), $r(_begin, _end): R)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__slice$A$)(__param_expand__slice$A$ __VA_ARGS__))
#define prefix$A(/*_a: A$$(_N,_T), _end: usize*/... /*(S_const$$(_T))*/)              __param_expand__prefix$A(__VA_ARGS__)
#define prefix$A$(/*(_T)(_a: A$$(_N,_T), _end: usize)*/... /*(S_const$(_T))*/)        pp_expand(pp_defer(__block_inline__prefix$A$)(__param_expand__prefix$A$ __VA_ARGS__))
#define suffix$A(/*_a: A$$(_N,_T), _begin: usize*/... /*(S_const$$(_T))*/)            __param_expand__suffix$A(__VA_ARGS__)
#define suffix$A$(/*(_T)(_a: A$$(_N,_T), _begin: usize)*/... /*(S_const$(_T))*/)      pp_expand(pp_defer(__block_inline__suffix$A$)(__param_expand__suffix$A$ __VA_ARGS__))

/*========== Macros and Definitions =========================================*/

#define A_from$(/*(_T){_initial...}*/...) __A_from__step(pp_defer(__A_from__emit)(__A_from__parseT __VA_ARGS__))
#define __A_from__step(...)               __VA_ARGS__
#define __A_from__parseT(_T...)           _T,
#define __A_from__emit(_T, _a...) \
    lit$((A$$((sizeOf$(TypeOf((_T[])_a)) / sizeOf$(_T)), _T)){ .val = _a })

#define __lit_init$A__step(...)         __VA_ARGS__
#define __lit_init$A__parseT(_N, _T...) _N, _T,
#define __lit_init$A__emit(_AliasFn, _N, _T, _initial...) \
    lit$((_AliasFn(_N, _T))init$A(_initial))

#define __ref$A__step(...)     __VA_ARGS__
#define __ref$A__parseT(_T...) _T, __ref$A__parseA
#define __ref$A__parseA(_a...) _a
#define __ref$A__emit(_T, _a...) \
    init$S$((_T)(ptr$A(_a), len$A(_a)))

#define param_expand__at$A(_a, _idx...)             pp_uniqTok(a), pp_uniqTok(idx), _a, _idx
#define block_inline__at$A(__a, __idx, _a, _idx...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__idx, usize) = _idx; \
    claim_assert_static_msg(__builtin_constant_p(__idx) ? (__idx < len$A(*__a)) : true, "index out of bounds"); \
    claim_assert_fmt(__idx < len$A(*__a), "Index out of bounds: idx(%zu) >= len(%zu)", __idx, len$A(*__a)); \
    &val$A(*__a)[__idx]; \
})

#define __param_expand__slice$A(_a, _range...)               __block_inline__slice$A(pp_uniqTok(a), pp_uniqTok(range), _a, _range)
#define __block_inline__slice$A(__a, __range, _a, _range...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__range, R) = _range; \
    claim_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$A(*__a)); \
    init$S$$((TypeOf(*ptr$A(*__a)))(&val$A(*__a)[__range.begin], len$R(__range))); \
})

#define __param_expand__slice$A$(...)                             __VA_ARGS__, pp_uniqTok(a), pp_uniqTok(range), __param_next__slice$A$
#define __param_next__slice$A$(...)                               __VA_ARGS__
#define __block_inline__slice$A$(_T, __a, __range, _a, _range...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__range, R) = _range; \
    claim_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$A(*__a)); \
    init$S$((_T)(&val$A(*__a)[__range.begin], len$R(__range))); \
})

#define __param_expand__prefix$A(_a, _end...)             __block_inline__prefix$A(pp_uniqTok(a), pp_uniqTok(end), _a, _end)
#define __block_inline__prefix$A(__a, __end, _a, _end...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__end, usize) = _end; \
    claim_assert_fmt(__end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __end, len$A(*__a)); \
    init$S$$((TypeOf(*ptr$A(*__a)))(ptr$A(*__a), __end)); \
})

#define __param_expand__prefix$A$(...)                         __VA_ARGS__, pp_uniqTok(a), pp_uniqTok(end), __param_next__prefix$A$
#define __param_next__prefix$A$(...)                           __VA_ARGS__
#define __block_inline__prefix$A$(_T, __a, __end, _a, _end...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__end, usize) = _end; \
    claim_assert_fmt(__end <= len$A(*__a), "Invalid slice range: end(%zu) > len(%zu)", __end, len$A(*__a)); \
    init$S$((_T)(ptr$A(*__a), __end)); \
})

#define __param_expand__suffix$A(_a, _begin...)               __block_inline__suffix$A(pp_uniqTok(a), pp_uniqTok(begin), _a, _begin)
#define __block_inline__suffix$A(__a, __begin, _a, _begin...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__begin, usize) = _begin; \
    claim_assert_fmt(__begin <= len$A(*__a), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$A(*__a)); \
    init$S$$((TypeOf(*ptr$A(*__a)))(ptr$A(*__a) + __begin, len$A(*__a) - __begin)); \
})

#define __param_expand__suffix$A$(...)                             __VA_ARGS__, pp_uniqTok(a), pp_uniqTok(begin), __param_next__suffix$A$
#define __param_next__suffix$A$(...)                               __VA_ARGS__
#define __block_inline__suffix$A$(_T, __a, __begin, _a, _begin...) ({ \
    let_(__a, TypeOf(&(_a))) = &(_a); \
    let_(__begin, usize) = _begin; \
    claim_assert_fmt(__begin <= len$A(*__a), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$A(*__a)); \
    init$S$((_T)(ptr$A(*__a) + __begin, len$A(*__a) - __begin)); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* types_Arr__included */
