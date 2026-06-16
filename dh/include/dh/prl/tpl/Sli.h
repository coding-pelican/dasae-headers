#pragma once
#ifndef prl_tpl_Sli__included
#define prl_tpl_Sli__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

/* Slice Anonymous */
#define S_const$$(_T...) __S_const$$(_T)
#define S$$(_T...) pp_Tok_if_(Tok_isConst$(_T))( \
    pp_Tok_then_(__S_const$$(Tok_removeConst$(_T))), \
    pp_Tok_else_(__S$$(_T)) \
)
#define __S_const$$(_T...) TypeOf(union { \
    T_embed$(struct { \
        var_(ptr, $P$(const _T)); \
        var_(len, usize); \
    }); \
    var_(as_raw, S_const$raw); \
    var_(ref_raw $like_ref, S_const$raw); \
})
#define __S$$(_T...) TypeOf(union { \
    T_embed$(struct { \
        var_(ptr, $P$(_T)); \
        var_(len, usize); \
    }); \
    var_(as_raw, S$raw); \
    var_(ref_raw $like_ref, S$raw); \
    var_(as_const, __S_const$$(_T)); \
})

/* Slice Alias */
#define S_const$(_T...) __S_const$(_T)
#define S$(_T...) pp_Tok_if_(Tok_isConst$(_T))( \
    pp_Tok_then_(__S_const$(Tok_removeConst$(_T))), \
    pp_Tok_else_(__S$(_T)) \
)
#define __S_const$(_T...) tpl$(S_const, _T)
#define __S$(_T...) tpl$(S, _T)

/* Slice Template */
#define T_decl_S$(_T...) \
    $maybe_unused typedef union S$(const _T) S$(const _T); \
    $maybe_unused typedef union S$(_T) S$(_T)
#define T_impl_S$(_T...) \
    union S$(const _T) { \
        T_embed$(struct { \
            var_(ptr, $P$(const _T)); \
            var_(len, usize); \
        }); \
        var_(as_raw, S$(const raw)); \
        var_(ref_raw $like_ref, S$(const raw)); \
    }; \
    union S$(_T) { \
        T_embed$(struct { \
            var_(ptr, $P$(_T)); \
            var_(len, usize); \
        }); \
        var_(as_raw, S$raw); \
        var_(ref_raw $like_ref, S$raw); \
        var_(as_const, S$(const _T)); \
    }
#define T_use_S$(_T...) \
    T_decl_S$(_T); \
    T_impl_S$(_T)

#define S_T$(_T...) TypeOf(*(as$(_T*)(null))->ptr)
#define S_TUnqual$(_T...) TypeOfUnqual(*(as$(_T*)(null))->ptr)
#define S_isConst$(_T...) eqlType$(S_InnerT$(_T)*, const S_InnerTUnqual$(_T)*)

#define S_InnerT$(_T...) TypeOf(*(as$(_T*)(null))->ptr)
#define S_InnerTUnqual$(_T...) TypeOfUnqual(*(as$(_T*)(null))->ptr)

#define isNullS(_s /*: S(_T)*/... /*(bool)*/) ____isNullS(_s)
#define ____isNullS(_s...) (as$(bool)((_s).ptr == null))
#define S_isNull isNullS
#define isNonnullS(_s /*: S(_T)*/... /*(bool)*/) ____isNonnullS(_s)
#define ____isNonnullS(_s...) (as$(bool)((_s).ptr != null))
#define S_isNonnull isNonnullS
#define ensureNonnullS(_s /*: S(_T)*/... /*(S(_T))*/) __step__ensureNonnullS(_s)
#define __step__ensureNonnullS(_s...) ____ensureNonnullS(pp_uniqTok(s), _s)
#define ____ensureNonnullS(__s, _s...) local_({ \
    let_(__s, TypeOf(_s)) = _s; \
    local_return_(claim_assert_nonnullS(__s), __s); \
})
#define S_ensureNonnull ensureNonnullS

#define mutCastS(_s /*: S(_T)*/... /*(S_const(_T))*/) ____mutCastS(_s)
#define ____mutCastS(_s...) ((_s).as_const)
#define S_mutCast mutCast
#define constCastS$(/*(_S_T: S(T))(_s: S_const(T))*/... /*(_S_T)*/) __step__constCastS$(__VA_ARGS__)
#define __step__constCastS$(...) __step__constCastS$__emit(__step__constCastS$__parse __VA_ARGS__)
#define __step__constCastS$__parse(_S_T...) _S_T, pp_uniqTok(s),
#define __step__constCastS$__emit(...) ____constCastS$(__VA_ARGS__)
#define ____constCastS$(_S_T, __s, _s...) local_({ \
    let_(__s, TypeOf(_s)) = _s; \
    local_return_(l$((_S_T){ .ptr = constCast(__s.ptr), .len = __s.len })); \
})
#define S_constCast$ constCastS$
#define constCastS(_s /*: S_const(_T)*/... /*(S(_T))*/) ____constCastS(_s)
#define ____constCastS(_s...) constCastS$((S$$(S_TUnqual$(TypeOf(_s))))(_s))
#define S_constCast constCastS

/* Slice Operations */
#define asgS(_p_s, _v_s...) asgS1(_p_s, _v_s)
#define asgS1(_p_s, _v_s...) asg(_p_s, _v_s, (ptr->ptr))
#define asgS2(_p_s, _v_s...) asg(_p_s, _v_s, (ptr->ptr->ptr))
#define asgS3(_p_s, _v_s...) asg(_p_s, _v_s, (ptr->ptr->ptr->ptr))
#define asgS4(_p_s, _v_s...) asg(_p_s, _v_s, (ptr->ptr->ptr->ptr->ptr))

#define S_ptr(_s /*: S$$(_T)*/... /*(P$$(_T))*/) ((_s).ptr)
#define ptrS S_ptr
#define S_len(_s /*: S$$(_T)*/... /*(usize)*/) ((_s).len)
#define lenS S_len

#define S_deref$(/*(_ANT: A(_N,_T))(_s: S(_T))*/... /*(_ANT)*/) \
    __op__S_deref$(__op__S_deref$__parseANT __VA_ARGS__)
#define __op__S_deref$(...) __op__S_deref$__emit(__VA_ARGS__)
#define __op__S_deref$__parseANT(_ANT...) _ANT,
#define __op__S_deref$__emit(_ANT, _s...) (*local_({ \
    let_(__s, TypeOf(_s)) = _s; \
    claim_assert_fmt(S_len(__s) == A_n$(_ANT), "length mismatch: len({:uz}) != N({:uz})", S_len(__s), A_n$(_ANT)); \
    as$(_ANT*)(ensureNonnull(S_ptr(__s))); \
}))
#define derefS$ S_deref$

#define S_at(/*(_s: S(_T))[_idx: usize]*/... /*(P(_T))*/) \
    __op__S_at(__op__S_at__parse __VA_ARGS__)
#define __op__S_at__parse(_s...) pp_uniqTok(s), pp_uniqTok(idx), _s,
#define __op__S_at(...) __op__S_at__emit(__VA_ARGS__)
#define __op__S_at__emit(__s, __idx, _s, _idx...) ({ \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _idx)); \
    let_(__s, TypeOf(_s)) = _s; \
    claim_assert_fmt(__idx < S_len(__s), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, S_len(__s)); \
    &S_ptr(__s)[__idx]; \
})
#define atS S_at
#if UNUSED_CODE
#define __op__S_at__emit(__s, __idx, _s, _idx...) ({ \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _idx)); \
    let_(__s, TypeOf(&(_s))) = &(_s); \
    claim_assert_fmt(__idx < S_len(*__s), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, S_len(*__s)); \
    S_ptr(*__s) + __idx; \
})
#endif /* UNUSED_CODE */
#define S_slice$(/*(_ST: S(_T))(_s: S(_T))(_range: R)*/... /*(_ST)*/) __op__S_slice$(__op__S_slice$__parseST __VA_ARGS__)
#define __op__S_slice$(...) __op__S_slice$__emit(__VA_ARGS__)
#define __op__S_slice$__parseST(_ST...) _ST, __op__S_slice$__parseS
#define __op__S_slice$__parseS(_s...) pp_uniqTok(s), _s, pp_uniqTok(range),
#define __op__S_slice$__emit(_ST, __s, _s, __range, _range...) ({ \
    let_(__range, R) = _range; \
    let_(__s, TypeOf(_s)) = _s; \
    claim_assert_fmt(R_isValid(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= S_len(__s), "Invalid slice range: end({:uz}) > len({:uz})", __range.end, S_len(__s)); \
    l$((_ST){ .ptr = &S_ptr(__s)[__range.begin], .len = R_len(__range) }); \
})
#define sliceS$ S_slice$
#define S_slice(/*(_s: S(_T))(_range: R)*/... /*(S(_T))*/) \
    __op__S_slice(__op__S_slice__parse __VA_ARGS__)
#define __op__S_slice__parse(_s...) pp_uniqTok(s), _s, pp_uniqTok(range),
#define __op__S_slice(...) __op__S_slice__emit(__VA_ARGS__)
#define __op__S_slice__emit(__s, _s, __range, _range...) ({ \
    let_(__range, R) = _range; \
    let_(__s, TypeOf(_s)) = _s; \
    claim_assert_fmt(R_isValid(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= S_len(__s), "Invalid slice range: end({:uz}) > len({:uz})", __range.end, S_len(__s)); \
    l$((TypeOf(__s)){ .ptr = &S_ptr(__s)[__range.begin], .len = R_len(__range) }); \
})
#define sliceS S_slice

#define S_prefix$(/*(_ST: S(_T))(_s: S(_T))(_end: usize)*/... /*(_ST)*/) \
    __op__S_prefix$(__op__S_prefix$__parseST __VA_ARGS__)
#define __op__S_prefix$(...) __op__S_prefix$__emit(__VA_ARGS__)
#define __op__S_prefix$__parseST(_ST...) _ST, __op__S_prefix$__parseS
#define __op__S_prefix$__parseS(_s...) pp_uniqTok(s), _s, pp_uniqTok(end),
#define __op__S_prefix$__emit(_ST, __s, _s, __end, _end...) ({ \
    let_(__end, usize) = _end; \
    let_(__s, TypeOf(_s)) = _s; \
    claim_assert_fmt(__end <= S_len(__s), "Invalid slice range: end({:uz}) > len({:uz})", __end, S_len(__s)); \
    l$((_ST){ .ptr = S_ptr(__s), .len = __end }); \
})
#define prefixS$ S_prefix$
#define S_prefix(/*(_s: S(_T))(_end: usize)*/... /*(S(_T))*/) \
    __op__S_prefix(__op__S_prefix__parse __VA_ARGS__)
#define __op__S_prefix__parse(_s...) pp_uniqTok(s), _s, pp_uniqTok(end),
#define __op__S_prefix(...) __op__S_prefix__emit(__VA_ARGS__)
#define __op__S_prefix__emit(__s, _s, __end, _end...) ({ \
    let_(__end, usize) = _end; \
    let_(__s, TypeOf(_s)) = _s; \
    claim_assert_fmt(__end <= S_len(__s), "Invalid slice range: end({:uz}) > len({:uz})", __end, S_len(__s)); \
    l$((TypeOf(__s)){ .ptr = S_ptr(__s), .len = __end }); \
})
#define prefixS S_prefix

#define S_suffix$(/*(_ST: S(_T))(_s: S(_T))(_begin: usize)*/... /*(_ST)*/) \
    __op__S_suffix$(__op__S_suffix$__parseST __VA_ARGS__)
#define __op__S_suffix$(...) __op__S_suffix$__emit(__VA_ARGS__)
#define __op__S_suffix$__parseST(_ST...) _ST, __op__S_suffix$__parseS
#define __op__S_suffix$__parseS(_s...) pp_uniqTok(s), _s, pp_uniqTok(begin),
#define __op__S_suffix$__emit(_ST, __s, _s, __begin, _begin...) ({ \
    let_(__begin, usize) = _begin; \
    let_(__s, TypeOf(_s)) = _s; \
    claim_assert_fmt(__begin <= S_len(__s), "Invalid slice range: begin({:uz}) > len({:uz})", __begin, S_len(__s)); \
    l$((_ST){ .ptr = &S_ptr(__s)[__begin], .len = S_len(__s) - __begin }); \
})
#define suffixS$ S_suffix$
#define S_suffix(/*(_s: S(_T))(_begin: usize)*/... /*(S(_T))*/) \
    __op__S_suffix(__op__S_suffix__parse __VA_ARGS__)
#define __op__S_suffix__parse(_s...) pp_uniqTok(s), _s, pp_uniqTok(begin),
#define __op__S_suffix(...) __op__S_suffix__emit(__VA_ARGS__)
#define __op__S_suffix__emit(__s, _s, __begin, _begin...) ({ \
    let_(__begin, usize) = _begin; \
    let_(__s, TypeOf(_s)) = _s; \
    claim_assert_fmt(__begin <= S_len(__s), "Invalid slice range: begin({:uz}) > len({:uz})", __begin, S_len(__s)); \
    l$((TypeOf(__s)){ .ptr = &S_ptr(__s)[__begin], .len = S_len(__s) - __begin }); \
})
#define suffixS S_suffix

/*========== Macros and Definitions =========================================*/

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_Sli__included */
