#ifndef prl_types_Ptr__included
#define prl_types_Ptr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "raw.h"

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
#define __P_const$(_T...) pp_join($, P_const, _T)
#define __P$(_T...) pp_join($, P, _T)

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
#define P_isConst$(_T... /*(bool)*/) Type_eq$(P_DerefT$(_T)*, const P_DerefTUnqual$(_T)*)

#define P_DerefT$(_T...) TypeOf(*as$(_T)(null))
#define P_DerefTUnqual$(_T...) TypeOfUnqual(*as$(_T)(null))
#define P_InnerT$(_T...) TypeOf(*as$(_T)(null))
#define P_InnerTUnqual$(_T...) TypeOfUnqual(*as$(_T)(null))

/* Pointer Operations */
#define deref$P(_p /*: P$$(_T)*/... /*(_T)*/) deref(_p)

#define mutCast$P(_p /*: P$$(_T)*/... /*(P_const $$(_T))*/) (as$(const TypeOfUnqual(*_p)*)(_p))
#define constCast$P(_p /*: P_const$$(_T)*/... /*(P$$(_T))*/) /* TODO: Implement */

#define P_slice$(/*(_ST: S(_T))((_p: P(_T))(_range: R))*/... /*(_ST)*/) \
    __op__P_slice$(__op__P_slice$__parseST __VA_ARGS__)
#define __op__P_slice$(...) __op__P_slice$__emit(__VA_ARGS__)
#define __op__P_slice$__parseST(_ST...) _ST, __op__P_slice$__parseP
#define __op__P_slice$__parseP(_p...) pp_uniqTok(p), _p, pp_uniqTok(range),
#define __op__P_slice$__emit(_ST, __p, _p, __range, _range...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__range, R) = _range; \
    claim_assert_fmt(isValid$R(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    /* claim_assert_fmt(__range.end <= 1, "Invalid slice range: end({:uz}) > 1", __range.end); */ \
    lit$((_ST){ .ptr = &__p[__range.begin], .len = len$R(__range) }); \
})
#define P_slice(/*(_p: P(_T))(_range: R)*/... /*(S(_T))*/) \
    __op__P_slice(__op__A_slice__parse __VA_ARGS__)
#define __op__P_slice(...) __op__P_slice__emit(__VA_ARGS__)
#define __op__P_slice__parse(_p...) pp_uniqTok(p), _p, pp_uniqTok(range),
#define __op__P_slice__emit(__p, _p, __range, _range...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__range, R) = _range; \
    claim_assert_fmt(isValid$R(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    /* claim_assert_fmt(__range.end <= 1, "Invalid slice range: end({:uz}) > 1", __range.end); */ \
    T_switch$((TypeOf(*__p))( \
        T_qual$((const TypeOfUnqual(*__p))( \
            lit$((S_const$$(P_InnerT$(TypeOf(__p)))){ .ptr = &__p[__range.begin], .len = len$R(__range) }) \
        )), \
        T_qual$((TypeOfUnqual(*__p))( \
            lit$((S$$(P_InnerT$(TypeOf(__p)))){ .ptr = &__p[__range.begin], .len = len$R(__range) }) \
        )) \
    )); \
})

#define P_prefix$(/*(_ST: S(_T))(_p: P(_T))(_end: usize)*/... /*(_ST)*/) \
    __op__P_prefix$(__op__P_prefix$__parseST __VA_ARGS__)
#define __op__P_prefix$(...) __op__P_prefix$__emit(__VA_ARGS__)
#define __op__P_prefix$__parseST(_ST...) _ST, __op__P_prefix$__parseA
#define __op__P_prefix$__parseA(_p...) pp_uniqTok(p), _p, pp_uniqTok(end),
#define __op__P_prefix$__emit(_ST, __p, _p, __end, _end...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__end, usize) = _end; \
    /* claim_assert_fmt(__end <= 1, "Invalid slice range: end({:uz}) > 1", __end); */ \
    lit$((_ST){ .ptr = __p, .len = __end }); \
})
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
            lit$((S_const$$(P_InnerT$(TypeOf(__p)))){ .ptr = __p, .len = __end }) \
        )), \
        T_qual$((TypeOfUnqual(*__p))( \
            lit$((S$$(P_InnerT$(TypeOf(__p)))){ .ptr = __p, .len = __end }) \
        )) \
    )); \
})

#define P_suffix$(/*(_ST: S(_T))((_p: P(_T))(_begin: usize))*/... /*(_ST)*/) \
    __op__P_suffix$(__op__P_suffix$__parseST __VA_ARGS__)
#define __op__P_suffix$(...) __op__P_suffix$__emit(__VA_ARGS__)
#define __op__P_suffix$__parseST(_ST...) _ST, __op__P_suffix$__parseA
#define __op__P_suffix$__parseA(_p...) pp_uniqTok(p), _p, pp_uniqTok(begin),
#define __op__P_suffix$__emit(_ST, __p, _p, __begin, _begin...) ({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__begin, usize) = _begin; \
    /* claim_assert_fmt(__begin <= 1, "Invalid slice range: begin({:uz}) > 1", __begin); */ \
    lit$((_ST){ .ptr = __p + __begin, .len = usize_limit_max - __begin }); \
})
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
            lit$((S_const$$(P_InnerT$(TypeOf(*__p)))){ .ptr = __p + __begin, .len = usize_limit_max - __begin }) \
        )), \
        T_qual$((TypeOfUnqual(*__p))( \
            lit$((S$$(P_InnerT$(TypeOf(*__p)))){ .ptr = __p + __begin, .len = usize_limit_max - __begin }) \
        )) \
    )); \
})

#define slice$P(/*_p: P$$(_T), $r(_begin:0, _end:1): R*/... /*(S_const$$(_T))*/) __param_expand__slice$P(__VA_ARGS__)
#define slice$P$(/*(_T)(_p: P$$(_T), $r(_begin:0, _end:1): R)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__slice$P$)(__param_expand__slice$P$ __VA_ARGS__))
#define prefix$P(/*_p: P$$(_T), _end: usize*/... /*(S_const$$(_T))*/) __param_expand__prefix$P(__VA_ARGS__)
#define prefix$P$(/*(_T)(_p: P$$(_T), _end: usize)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__prefix$P$)(__param_expand__prefix$P$ __VA_ARGS__))
#define suffix$P(/*_p: P$$(_T), _begin: usize*/... /*(S_const$$(_T))*/) __param_expand__suffix$P(__VA_ARGS__)
#define suffix$P$(/*(_T)(_p: P$$(_T), _begin: usize)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__suffix$P$)(__param_expand__suffix$P$ __VA_ARGS__))

/*========== Macros and Definitions =========================================*/

#define __param_expand__slice$P(_p, _range...) __block_inline__slice$P(pp_uniqTok(p), pp_uniqTok(range), _p, _range)
#define __block_inline__slice$P(__p, __range, _p, _range...) \
    ({ \
        let_(__p, TypeOf(_p)) = _p; \
        let_(__range, R) = _range; \
        claim_assert_fmt(isValid$R(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
        /* claim_assert_fmt(__range.end <= 1, "Invalid slice range: end({:uz}) > 1", __range.end); */ \
        init$S$$((TypeOf(*__p))(__p, len$R(__range))); \
    })

#define __param_expand__slice$P$(...) __VA_ARGS__, pp_uniqTok(p), pp_uniqTok(range), __param_next__slice$P$
#define __param_next__slice$P$(...) __VA_ARGS__
#define __block_inline__slice$P$(_T, __p, __range, _p, _range...) \
    ({ \
        let_(__p, TypeOf(_p)) = _p; \
        let_(__range, R) = _range; \
        claim_assert_fmt(isValid$R(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
        /* claim_assert_fmt(__range.end <= 1, "Invalid slice range: end({:uz}) > 1", __range.end); */ \
        init$S$((_T)(__p, len$R(__range))); \
    })

#define __param_expand__prefix$P(_p, _end...) __block_inline__prefix$P(pp_uniqTok(p), pp_uniqTok(end), _p, _end)
#define __block_inline__prefix$P(__p, __end, _p, _end...) \
    ({ \
        let_(__p, TypeOf(_p)) = _p; \
        let_(__end, usize) = _end; \
        /* claim_assert_fmt(__end <= 1, "Invalid slice range: end({:uz}) > 1", __end); */ \
        init$S$$((TypeOf(*__p))(__p, __end)); \
    })

#define __param_expand__prefix$P$(...) __VA_ARGS__, pp_uniqTok(p), pp_uniqTok(end), __param_next__prefix$P$
#define __param_next__prefix$P$(...) __VA_ARGS__
#define __block_inline__prefix$P$(_T, __p, __end, _p, _end...) \
    ({ \
        let_(__p, TypeOf(_p)) = _p; \
        let_(__end, usize) = _end; \
        /* claim_assert_fmt(__end <= 1, "Invalid slice range: end({:uz}) > 1", __end); */ \
        init$S$((_T)(__p, __end)); \
    })

#define __param_expand__suffix$P(_p, _begin...) __block_inline__suffix$P(pp_uniqTok(p), pp_uniqTok(begin), _p, _begin)
#define __block_inline__suffix$P(__p, __begin, _p, _begin...) \
    ({ \
        let_(__p, TypeOf(_p)) = _p; \
        let_(__begin, usize) = _begin; \
        /* claim_assert_fmt(__begin <= 1, "Invalid slice range: begin({:uz}) > 1", __begin); */ \
        init$S$$((TypeOf(*__p))(__p, usize_limit_max - __begin)); \
    })

#define __param_expand__suffix$P$(...) __VA_ARGS__, pp_uniqTok(p), pp_uniqTok(begin), __param_next__suffix$P$
#define __param_next__suffix$P$(...) __VA_ARGS__
#define __block_inline__suffix$P$(_T, __p, __begin, _p, _begin...) \
    ({ \
        let_(__p, TypeOf(_p)) = _p; \
        let_(__begin, usize) = _begin; \
        /* claim_assert_fmt(__begin <= 1, "Invalid slice range: begin({:uz}) > 1", __begin); */ \
        init$S$((_T)(__p, usize_limit_max - __begin)); \
    })

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_types_Ptr__included */
