#ifndef prl_tpl_Ptr__included
#define prl_tpl_Ptr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

/* Pointer Anonymous */
#define P_const$$(_T...) __P_const$$(_T)
#define P$$(_T...) pp_Tok_if_(Tok_isConst$(_T))( \
    pp_Tok_then_(__P_const$$(Tok_removeConst$(_T))), \
    pp_Tok_else_(__P$$(_T)) \
)
#define __P_const$$(_T...) $P_const$(_T)
#define __P$$(_T...) $P$(_T)

/* Pointer Alias */
#define P_const$(_T...) __P_const$(_T)
#define P$(_T...) pp_Tok_if_(Tok_isConst$(_T))( \
    pp_Tok_then_(__P_const$(Tok_removeConst$(_T))), \
    pp_Tok_else_(__P$(_T)) \
)
#define __P_const$(_T...) tpl$(P_const, _T)
#define __P$(_T...) tpl$(P, _T)

/* Pointer Template */
#define T_decl_P$(_T...) \
    $maybe_unused typedef $P$(const _T) P$(const _T); \
    $maybe_unused typedef $P$(_T) P$(_T)
#define T_impl_P$(_T...) \
    $maybe_unused typedef $P$(const _T) P$(const _T); \
    $maybe_unused typedef $P$(_T) P$(_T)
#define T_use_P$(_T...) \
    T_impl_P$(_T)

#define P_T$(_T...) TypeOf(*as$(_T)(null))
#define P_TUnqual$(_T...) TypeOfUnqual(*as$(_T)(null))
#define P_isConst$(_T... /*(bool)*/) eqlType$(P_DerefT$(_T)*, const P_DerefTUnqual$(_T)*)

#define P_DerefT$(_T...) TypeOf(*as$(_T)(null))
#define P_DerefTUnqual$(_T...) TypeOfUnqual(*as$(_T)(null))
#define P_InnerT$(_T...) TypeOf(*as$(_T)(null))
#define P_InnerTUnqual$(_T...) TypeOfUnqual(*as$(_T)(null))

/* Pointer Operations */
#define isNullP isNull
#define P_isNull isNullP
#define isNonnullP isNonnull
#define P_isNonnull isNonnullP
#define ensureNonnullP ensureNonnull
#define P_ensureNonnull ensureNonnullP

#define P_deref deref
#define derefP P_deref
#define P_mutCast mutCast
#define mutCastP P_mutCast
#define P_constCast constCast
#define constCastP P_constCast

#define P_at(/*(_p: P(_T))[_idx: usize]*/... /*(P(_T))*/) (&__VA_ARGS__)
#define atP P_at

#define P_slice$(/*(_ST: S(_T))((_p: P(_T))(_range: R))*/... /*(_ST)*/) \
    __op__P_slice$(__op__P_slice$__parseST __VA_ARGS__)
#define __op__P_slice$(...) __op__P_slice$__emit(__VA_ARGS__)
#define __op__P_slice$__parseST(_ST...) _ST, __op__P_slice$__parseP
#define __op__P_slice$__parseP(_p...) pp_uniqTok(p), _p, pp_uniqTok(range),
#define __op__P_slice$__emit(_ST, __p, _p, __range, _range...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__range, R) = _range; \
    claim_assert_fmt(R_isValid(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    /* claim_assert_fmt(__range.end <= 1, "Invalid slice range: end({:uz}) > 1", __range.end); */ \
    l$((_ST){ .ptr = &__p[__range.begin], .len = R_len(__range) }); \
})
#define sliceP$ P_slice$
#define P_slice(/*(_p: P(_T))(_range: R)*/... /*(S(_T))*/) \
    __op__P_slice(__op__A_slice__parse __VA_ARGS__)
#define __op__P_slice(...) __op__P_slice__emit(__VA_ARGS__)
#define __op__P_slice__parse(_p...) pp_uniqTok(p), _p, pp_uniqTok(range),
#define __op__P_slice__emit(__p, _p, __range, _range...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__range, R) = _range; \
    claim_assert_fmt(R_isValid(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    /* claim_assert_fmt(__range.end <= 1, "Invalid slice range: end({:uz}) > 1", __range.end); */ \
    T_switch$((TypeOf(*__p))( \
        T_qual$((const TypeOfUnqual(*__p))( \
            l$((S_const$$(P_InnerT$(TypeOf(__p)))){ .ptr = &__p[__range.begin], .len = R_len(__range) }) \
        )), \
        T_qual$((TypeOfUnqual(*__p))( \
            l$((S$$(P_InnerT$(TypeOf(__p)))){ .ptr = &__p[__range.begin], .len = R_len(__range) }) \
        )) \
    )); \
})
#define sliceP P_slice

#define P_prefix$(/*(_ST: S(_T))(_p: P(_T))(_end: usize)*/... /*(_ST)*/) \
    __op__P_prefix$(__op__P_prefix$__parseST __VA_ARGS__)
#define __op__P_prefix$(...) __op__P_prefix$__emit(__VA_ARGS__)
#define __op__P_prefix$__parseST(_ST...) _ST, __op__P_prefix$__parseA
#define __op__P_prefix$__parseA(_p...) pp_uniqTok(p), _p, pp_uniqTok(end),
#define __op__P_prefix$__emit(_ST, __p, _p, __end, _end...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__end, usize) = _end; \
    /* claim_assert_fmt(__end <= 1, "Invalid slice range: end({:uz}) > 1", __end); */ \
    l$((_ST){ .ptr = __p, .len = __end }); \
})
#define prefixP$ P_prefix$
#define P_prefix(/*(_p: P(_T))(_end: usize)*/... /*(S(_T))*/) \
    __op__P_prefix(__op__P_prefix__parse __VA_ARGS__)
#define __op__P_prefix(...) __op__P_prefix__emit(__VA_ARGS__)
#define __op__P_prefix__parse(_p...) pp_uniqTok(p), _p, pp_uniqTok(end),
#define __op__P_prefix__emit(__p, _p, __end, _end...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__end, usize) = _end; \
    /* claim_assert_fmt(__end <= 1, "Invalid slice range: end({:uz}) > 1", __end); */ \
    T_switch$((TypeOf(*__p))( \
        T_qual$((const TypeOfUnqual(*__p))( \
            l$((S_const$$(P_InnerT$(TypeOf(__p)))){ .ptr = __p, .len = __end }) \
        )), \
        T_qual$((TypeOfUnqual(*__p))( \
            l$((S$$(P_InnerT$(TypeOf(__p)))){ .ptr = __p, .len = __end }) \
        )) \
    )); \
})
#define prefixP P_prefix

#define P_suffix$(/*(_ST: S(_T))((_p: P(_T))(_begin: usize))*/... /*(_ST)*/) \
    __op__P_suffix$(__op__P_suffix$__parseST __VA_ARGS__)
#define __op__P_suffix$(...) __op__P_suffix$__emit(__VA_ARGS__)
#define __op__P_suffix$__parseST(_ST...) _ST, __op__P_suffix$__parseA
#define __op__P_suffix$__parseA(_p...) pp_uniqTok(p), _p, pp_uniqTok(begin),
#define __op__P_suffix$__emit(_ST, __p, _p, __begin, _begin...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__begin, usize) = _begin; \
    /* claim_assert_fmt(__begin <= 1, "Invalid slice range: begin({:uz}) > 1", __begin); */ \
    l$((_ST){ .ptr = &__p[__begin], .len = usize_limit_max - __begin }); \
})
#define suffixP$ P_suffix$
#define P_suffix(/*(_p: P(_T))(_begin: usize)*/... /*(S(_T))*/) \
    __op__P_suffix(__op__P_suffix__parse __VA_ARGS__)
#define __op__P_suffix(...) __op__P_suffix__emit(__VA_ARGS__)
#define __op__P_suffix__parse(_p...) pp_uniqTok(p), _p, pp_uniqTok(begin),
#define __op__P_suffix__emit(__p, _p, __begin, _begin...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__begin, usize) = _begin; \
    /* claim_assert_fmt(__begin <= 1, "Invalid slice range: begin({:uz}) > 1", __begin); */ \
    T_switch$((TypeOf(*__a))( \
        T_qual$((const TypeOfUnqual(*__p))( \
            l$((S_const$$(P_InnerT$(TypeOf(*__p)))){ .ptr = &__p[__begin], .len = usize_limit_max - __begin }) \
        )), \
        T_qual$((TypeOfUnqual(*__p))( \
            l$((S$$(P_InnerT$(TypeOf(*__p)))){ .ptr = &__p[__begin], .len = usize_limit_max - __begin }) \
        )) \
    )); \
})
#define suffixP P_suffix

/*========== Macros and Definitions =========================================*/

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_Ptr__included */
