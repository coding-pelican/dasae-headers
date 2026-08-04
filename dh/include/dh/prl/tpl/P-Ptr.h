#pragma once
#ifndef prl_tpl_P_Ptr__included
#define prl_tpl_P_Ptr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

/* Pointer Anonymous */
#define P_const$$(_$T...) __P_const$$(_$T)
#define P$$(_$T...) pp_Tok_if_(Tok_isConst$(_$T))( \
    pp_Tok_then_(__P_const$$(Tok_removeConst$(_$T))), \
    pp_Tok_else_(__P$$(_$T)) \
)
#define __P_const$$(_$T...) $P_const$(_$T)
#define __P$$(_$T...) $P$(_$T)

/* Pointer Alias */
#define P_const$(_$T...) __P_const$(_$T)
#define P$(_$T...) pp_Tok_if_(Tok_isConst$(_$T))( \
    pp_Tok_then_(__P_const$(Tok_removeConst$(_$T))), \
    pp_Tok_else_(__P$(_$T)) \
)
#define __P_const$(_$T...) tpl$(P_const, _$T)
#define __P$(_$T...) tpl$(P, _$T)

/* Pointer Template */
#define T_decl_P$(_$T...) \
    $maybe_unused typedef $P$(const _$T) P$(const _$T); \
    $maybe_unused typedef $P$(_$T) P$(_$T)
#define T_impl_P$(_$T...) \
    $maybe_unused typedef $P$(const _$T) P$(const _$T); \
    $maybe_unused typedef $P$(_$T) P$(_$T)
#define T_use_P$(_$T...) \
    T_impl_P$(_$T)

#define P_T$(_$T...) TypeOf(*as$(_$T)(null))
#define P_TUnqual$(_$T...) TypeOfUnqual(*as$(_$T)(null))
#define P_isConst$(_$T... /*(bool)*/) eqlType$(_$T, const P_DerefT$(_$T)*)

#define P_DerefT$(_$T...) TypeOf(*as$(_$T)(null))
#define P_DerefTUnqual$(_$T...) TypeOfUnqual(*as$(_$T)(null))
#define P_InnerT$(_$T...) TypeOf(*as$(_$T)(null))
#define P_InnerTUnqual$(_$T...) TypeOfUnqual(*as$(_$T)(null))

#define P_isNull isNull
#define isNullP P_isNull
#define P_isNonnull isNonnull
#define isNonnullP P_isNonnull
#define P_ensureNonnull ensureNonnull
#define ensureNonnullP P_ensureNonnull

/* Pointer Operations */

#define P_deref deref
#define derefP P_deref
#define P_mutCast mutCast
#define mutCastP P_mutCast
#define P_constCast constCast
#define constCastP P_constCast

#define P_at(/*(_$p: P(_$T))[_$idx: usize]*/... /*(P(_$T))*/) (&__VA_ARGS__)
#define atP P_at

#define P_slice$(/*(_$ST: S(_$T))((_$p: P(_$T))(_$range: R))*/... /*(_$ST)*/) \
    __op__P_slice$(__op__P_slice$__parseST __VA_ARGS__)
#define __op__P_slice$(...) __op__P_slice$__emit(__VA_ARGS__)
#define __op__P_slice$__parseST(_$ST...) _$ST, __op__P_slice$__parseP
#define __op__P_slice$__parseP(_$p...) pp_uniqTok(p), _$p, pp_uniqTok(range),
#define __op__P_slice$__emit(_$ST, __p, _$p, __range, _$range...) ({ \
    let_(__p, TypeOf(_$p)) = _$p; \
    let_(__range, R) = _$range; \
    claim_assert_fmt(R_isValid(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    /* claim_assert_fmt(__range.end <= 1, "Invalid slice range: end({:uz}) > 1", __range.end); */ \
    l$((_$ST){ .ptr = &__p[__range.begin], .len = R_len(__range) }); \
})
#define sliceP$ P_slice$
#define P_slice(/*(_$p: P(_$T))(_$range: R)*/... /*(S(_$T))*/) \
    __op__P_slice(__op__A_slice__parse __VA_ARGS__)
#define __op__P_slice(...) __op__P_slice__emit(__VA_ARGS__)
#define __op__P_slice__parse(_$p...) pp_uniqTok(p), _$p, pp_uniqTok(range),
#define __op__P_slice__emit(__p, _$p, __range, _$range...) ({ \
    let_(__p, TypeOf(_$p)) = _$p; \
    let_(__range, R) = _$range; \
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

#define P_prefix$(/*(_$ST: S(_$T))(_$p: P(_$T))(_$end: usize)*/... /*(_$ST)*/) \
    __op__P_prefix$(__op__P_prefix$__parseST __VA_ARGS__)
#define __op__P_prefix$(...) __op__P_prefix$__emit(__VA_ARGS__)
#define __op__P_prefix$__parseST(_$ST...) _$ST, __op__P_prefix$__parseA
#define __op__P_prefix$__parseA(_$p...) pp_uniqTok(p), _$p, pp_uniqTok(end),
#define __op__P_prefix$__emit(_$ST, __p, _$p, __end, _$end...) ({ \
    let_(__p, TypeOf(_$p)) = _$p; \
    let_(__end, usize) = _$end; \
    /* claim_assert_fmt(__end <= 1, "Invalid slice range: end({:uz}) > 1", __end); */ \
    l$((_$ST){ .ptr = __p, .len = __end }); \
})
#define prefixP$ P_prefix$
#define P_prefix(/*(_$p: P(_$T))(_$end: usize)*/... /*(S(_$T))*/) \
    __op__P_prefix(__op__P_prefix__parse __VA_ARGS__)
#define __op__P_prefix(...) __op__P_prefix__emit(__VA_ARGS__)
#define __op__P_prefix__parse(_$p...) pp_uniqTok(p), _$p, pp_uniqTok(end),
#define __op__P_prefix__emit(__p, _$p, __end, _$end...) ({ \
    let_(__p, TypeOf(_$p)) = _$p; \
    let_(__end, usize) = _$end; \
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

#define P_suffix$(/*(_$ST: S(_$T))((_$p: P(_$T))(_$begin: usize))*/... /*(_$ST)*/) \
    __op__P_suffix$(__op__P_suffix$__parseST __VA_ARGS__)
#define __op__P_suffix$(...) __op__P_suffix$__emit(__VA_ARGS__)
#define __op__P_suffix$__parseST(_$ST...) _$ST, __op__P_suffix$__parseA
#define __op__P_suffix$__parseA(_$p...) pp_uniqTok(p), _$p, pp_uniqTok(begin),
#define __op__P_suffix$__emit(_$ST, __p, _$p, __begin, _$begin...) ({ \
    let_(__p, TypeOf(_$p)) = _$p; \
    let_(__begin, usize) = _$begin; \
    /* claim_assert_fmt(__begin <= 1, "Invalid slice range: begin({:uz}) > 1", __begin); */ \
    l$((_$ST){ .ptr = &__p[__begin], .len = usize_limit_max - __begin }); \
})
#define suffixP$ P_suffix$
#define P_suffix(/*(_$p: P(_$T))(_$begin: usize)*/... /*(S(_$T))*/) \
    __op__P_suffix(__op__P_suffix__parse __VA_ARGS__)
#define __op__P_suffix(...) __op__P_suffix__emit(__VA_ARGS__)
#define __op__P_suffix__parse(_$p...) pp_uniqTok(p), _$p, pp_uniqTok(begin),
#define __op__P_suffix__emit(__p, _$p, __begin, _$begin...) ({ \
    let_(__p, TypeOf(_$p)) = _$p; \
    let_(__begin, usize) = _$begin; \
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
#endif /* prl_tpl_P_Ptr__included */
