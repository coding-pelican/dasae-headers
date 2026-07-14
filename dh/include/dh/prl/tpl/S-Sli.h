#pragma once
#ifndef prl_tpl_S_Sli__included
#define prl_tpl_S_Sli__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

/* Slice Anonymous */
#define S_const$$(_$T...) __S_const$$(_$T)
#define S$$(_$T...) pp_Tok_if_(Tok_isConst$(_$T))( \
    pp_Tok_then_(__S_const$$(Tok_removeConst$(_$T))), \
    pp_Tok_else_(__S$$(_$T)) \
)
#define __S_const$$(_$T...) TypeOf(union { \
    T_embed$(struct { \
        var_(ptr, $P$(const _$T)); \
        var_(len, usize); \
    }); \
    var_(as_raw, S_const$raw); \
    var_(ref_raw $like_ref, S_const$raw); \
})
#define __S$$(_$T...) TypeOf(union { \
    T_embed$(struct { \
        var_(ptr, $P$(_$T)); \
        var_(len, usize); \
    }); \
    var_(as_raw, S$raw); \
    var_(ref_raw $like_ref, S$raw); \
    var_(as_const, __S_const$$(_$T)); \
})

/* Slice Alias */
#define S_const$(_$T...) __S_const$(_$T)
#define S$(_$T...) pp_Tok_if_(Tok_isConst$(_$T))( \
    pp_Tok_then_(__S_const$(Tok_removeConst$(_$T))), \
    pp_Tok_else_(__S$(_$T)) \
)
#define __S_const$(_$T...) tpl$(S_const, _$T)
#define __S$(_$T...) tpl$(S, _$T)

/* Slice Template */
#define T_decl_S$(_$T...) \
    $maybe_unused typedef union S$(const _$T) S$(const _$T); \
    $maybe_unused typedef union S$(_$T) S$(_$T)
#define T_impl_S$(_$T...) \
    union S$(const _$T) { \
        T_embed$(struct { \
            var_(ptr, $P$(const _$T)); \
            var_(len, usize); \
        }); \
        var_(as_raw, S$(const raw)); \
        var_(ref_raw $like_ref, S$(const raw)); \
    }; \
    union S$(_$T) { \
        T_embed$(struct { \
            var_(ptr, $P$(_$T)); \
            var_(len, usize); \
        }); \
        var_(as_raw, S$raw); \
        var_(ref_raw $like_ref, S$raw); \
        var_(as_const, S$(const _$T)); \
    }
#define T_use_S$(_$T...) \
    T_decl_S$(_$T); \
    T_impl_S$(_$T)

#define S_T$(_$T...) TypeOf(*(as$(_$T*)(null))->ptr)
#define S_TUnqual$(_$T...) TypeOfUnqual(*(as$(_$T*)(null))->ptr)
#define S_isConst$(_$T...) eqlType$(S_InnerT$(_$T)*, const S_InnerTUnqual$(_$T)*)

#define S_InnerT$(_$T...) TypeOf(*(as$(_$T*)(null))->ptr)
#define S_InnerTUnqual$(_$T...) TypeOfUnqual(*(as$(_$T*)(null))->ptr)

#define isNullS(_$s /*: S(_$T)*/... /*(bool)*/) ____isNullS(_$s)
#define ____isNullS(_$s...) (as$(bool)((_$s).ptr == null))
#define S_isNull isNullS
#define isNonnullS(_$s /*: S(_$T)*/... /*(bool)*/) ____isNonnullS(_$s)
#define ____isNonnullS(_$s...) (as$(bool)((_$s).ptr != null))
#define S_isNonnull isNonnullS
#define ensureNonnullS(_$s /*: S(_$T)*/... /*(S(_$T))*/) __step__ensureNonnullS(_$s)
#define __step__ensureNonnullS(_$s...) ____ensureNonnullS(pp_uniqTok(s), _$s)
#define ____ensureNonnullS(__s, _$s...) local_({ \
    let_(__s, TypeOf(_$s)) = _$s; \
    local_return_(claim_assert_nonnullS(__s), __s); \
})
#define S_ensureNonnull ensureNonnullS

#define mutCastS(_$s /*: S(_$T)*/... /*(S_const(_$T))*/) ____mutCastS(_$s)
#define ____mutCastS(_$s...) ((_$s).as_const)
#define S_mutCast mutCast
#define constCastS$(/*(_$S_T: S(T))(_$s: S_const(T))*/... /*(_$S_T)*/) __step__constCastS$(__VA_ARGS__)
#define __step__constCastS$(...) __step__constCastS$__emit(__step__constCastS$__parse __VA_ARGS__)
#define __step__constCastS$__parse(_$S_T...) _$S_T, pp_uniqTok(s),
#define __step__constCastS$__emit(...) ____constCastS$(__VA_ARGS__)
#define ____constCastS$(_$S_T, __s, _$s...) local_({ \
    let_(__s, TypeOf(_$s)) = _$s; \
    local_return_(l$((_$S_T){ .ptr = constCast(__s.ptr), .len = __s.len })); \
})
#define S_constCast$ constCastS$
#define constCastS(_$s /*: S_const(_$T)*/... /*(S(_$T))*/) ____constCastS(_$s)
#define ____constCastS(_$s...) constCastS$((S$$(S_TUnqual$(TypeOf(_$s))))(_$s))
#define S_constCast constCastS

/* Slice Operations */
#define asgS(_$p_s, _$v_s...) asgS1(_$p_s, _$v_s)
#define asgS1(_$p_s, _$v_s...) asg(_$p_s, _$v_s, (ptr->ptr))
#define asgS2(_$p_s, _$v_s...) asg(_$p_s, _$v_s, (ptr->ptr->ptr))
#define asgS3(_$p_s, _$v_s...) asg(_$p_s, _$v_s, (ptr->ptr->ptr->ptr))
#define asgS4(_$p_s, _$v_s...) asg(_$p_s, _$v_s, (ptr->ptr->ptr->ptr->ptr))

#define S_ptr(_$s /*: S$$(_$T)*/... /*(P$$(_$T))*/) ((_$s).ptr)
#define ptrS S_ptr
#define S_len(_$s /*: S$$(_$T)*/... /*(usize)*/) ((_$s).len)
#define lenS S_len

#define S_deref$(/*(_$ANT: A(_$N,_$T))(_$s: S(_$T))*/... /*(_$ANT)*/) \
    __op__S_deref$(__op__S_deref$__parseANT __VA_ARGS__)
#define __op__S_deref$(...) __op__S_deref$__emit(__VA_ARGS__)
#define __op__S_deref$__parseANT(_$ANT...) _$ANT,
#define __op__S_deref$__emit(_$ANT, _$s...) (*local_({ \
    let_(__s, TypeOf(_$s)) = _$s; \
    claim_assert_fmt(S_len(__s) == A_n$(_$ANT), "length mismatch: len({:uz}) != N({:uz})", S_len(__s), A_n$(_$ANT)); \
    as$(_$ANT*)(ensureNonnull(S_ptr(__s))); \
}))
#define derefS$ S_deref$

#define S_at(/*(_$s: S(_$T))[_$idx: usize]*/... /*(P(_$T))*/) \
    __op__S_at(__op__S_at__parse __VA_ARGS__)
#define __op__S_at__parse(_$s...) pp_uniqTok(s), pp_uniqTok(idx), _$s,
#define __op__S_at(...) __op__S_at__emit(__VA_ARGS__)
#define __op__S_at__emit(__s, __idx, _$s, _$idx...) ({ \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _$idx)); \
    let_(__s, TypeOf(_$s)) = _$s; \
    claim_assert_fmt(__idx < S_len(__s), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, S_len(__s)); \
    &S_ptr(__s)[__idx]; \
})
#define atS S_at
#if UNUSED_CODE
#define __op__S_at__emit(__s, __idx, _$s, _$idx...) ({ \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _$idx)); \
    let_(__s, TypeOf(&(_$s))) = &(_$s); \
    claim_assert_fmt(__idx < S_len(*__s), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, S_len(*__s)); \
    S_ptr(*__s) + __idx; \
})
#endif /* UNUSED_CODE */
#define S_slice$(/*(_$ST: S(_$T))(_$s: S(_$T))(_$range: R)*/... /*(_$ST)*/) __op__S_slice$(__op__S_slice$__parseST __VA_ARGS__)
#define __op__S_slice$(...) __op__S_slice$__emit(__VA_ARGS__)
#define __op__S_slice$__parseST(_$ST...) _$ST, __op__S_slice$__parseS
#define __op__S_slice$__parseS(_$s...) pp_uniqTok(s), _$s, pp_uniqTok(range),
#define __op__S_slice$__emit(_$ST, __s, _$s, __range, _$range...) ({ \
    let_(__range, R) = _$range; \
    let_(__s, TypeOf(_$s)) = _$s; \
    claim_assert_fmt(R_isValid(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= S_len(__s), "Invalid slice range: end({:uz}) > len({:uz})", __range.end, S_len(__s)); \
    l$((_$ST){ .ptr = &S_ptr(__s)[__range.begin], .len = R_len(__range) }); \
})
#define sliceS$ S_slice$
#define S_slice(/*(_$s: S(_$T))(_$range: R)*/... /*(S(_$T))*/) \
    __op__S_slice(__op__S_slice__parse __VA_ARGS__)
#define __op__S_slice__parse(_$s...) pp_uniqTok(s), _$s, pp_uniqTok(range),
#define __op__S_slice(...) __op__S_slice__emit(__VA_ARGS__)
#define __op__S_slice__emit(__s, _$s, __range, _$range...) ({ \
    let_(__range, R) = _$range; \
    let_(__s, TypeOf(_$s)) = _$s; \
    claim_assert_fmt(R_isValid(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= S_len(__s), "Invalid slice range: end({:uz}) > len({:uz})", __range.end, S_len(__s)); \
    l$((TypeOf(__s)){ .ptr = &S_ptr(__s)[__range.begin], .len = R_len(__range) }); \
})
#define sliceS S_slice

#define S_prefix$(/*(_$ST: S(_$T))(_$s: S(_$T))(_$end: usize)*/... /*(_$ST)*/) \
    __op__S_prefix$(__op__S_prefix$__parseST __VA_ARGS__)
#define __op__S_prefix$(...) __op__S_prefix$__emit(__VA_ARGS__)
#define __op__S_prefix$__parseST(_$ST...) _$ST, __op__S_prefix$__parseS
#define __op__S_prefix$__parseS(_$s...) pp_uniqTok(s), _$s, pp_uniqTok(end),
#define __op__S_prefix$__emit(_$ST, __s, _$s, __end, _$end...) ({ \
    let_(__end, usize) = _$end; \
    let_(__s, TypeOf(_$s)) = _$s; \
    claim_assert_fmt(__end <= S_len(__s), "Invalid slice range: end({:uz}) > len({:uz})", __end, S_len(__s)); \
    l$((_$ST){ .ptr = S_ptr(__s), .len = __end }); \
})
#define prefixS$ S_prefix$
#define S_prefix(/*(_$s: S(_$T))(_$end: usize)*/... /*(S(_$T))*/) \
    __op__S_prefix(__op__S_prefix__parse __VA_ARGS__)
#define __op__S_prefix__parse(_$s...) pp_uniqTok(s), _$s, pp_uniqTok(end),
#define __op__S_prefix(...) __op__S_prefix__emit(__VA_ARGS__)
#define __op__S_prefix__emit(__s, _$s, __end, _$end...) ({ \
    let_(__end, usize) = _$end; \
    let_(__s, TypeOf(_$s)) = _$s; \
    claim_assert_fmt(__end <= S_len(__s), "Invalid slice range: end({:uz}) > len({:uz})", __end, S_len(__s)); \
    l$((TypeOf(__s)){ .ptr = S_ptr(__s), .len = __end }); \
})
#define prefixS S_prefix

#define S_suffix$(/*(_$ST: S(_$T))(_$s: S(_$T))(_$begin: usize)*/... /*(_$ST)*/) \
    __op__S_suffix$(__op__S_suffix$__parseST __VA_ARGS__)
#define __op__S_suffix$(...) __op__S_suffix$__emit(__VA_ARGS__)
#define __op__S_suffix$__parseST(_$ST...) _$ST, __op__S_suffix$__parseS
#define __op__S_suffix$__parseS(_$s...) pp_uniqTok(s), _$s, pp_uniqTok(begin),
#define __op__S_suffix$__emit(_$ST, __s, _$s, __begin, _$begin...) ({ \
    let_(__begin, usize) = _$begin; \
    let_(__s, TypeOf(_$s)) = _$s; \
    claim_assert_fmt(__begin <= S_len(__s), "Invalid slice range: begin({:uz}) > len({:uz})", __begin, S_len(__s)); \
    l$((_$ST){ .ptr = &S_ptr(__s)[__begin], .len = S_len(__s) - __begin }); \
})
#define suffixS$ S_suffix$
#define S_suffix(/*(_$s: S(_$T))(_$begin: usize)*/... /*(S(_$T))*/) \
    __op__S_suffix(__op__S_suffix__parse __VA_ARGS__)
#define __op__S_suffix__parse(_$s...) pp_uniqTok(s), _$s, pp_uniqTok(begin),
#define __op__S_suffix(...) __op__S_suffix__emit(__VA_ARGS__)
#define __op__S_suffix__emit(__s, _$s, __begin, _$begin...) ({ \
    let_(__begin, usize) = _$begin; \
    let_(__s, TypeOf(_$s)) = _$s; \
    claim_assert_fmt(__begin <= S_len(__s), "Invalid slice range: begin({:uz}) > len({:uz})", __begin, S_len(__s)); \
    l$((TypeOf(__s)){ .ptr = &S_ptr(__s)[__begin], .len = S_len(__s) - __begin }); \
})
#define suffixS S_suffix

/*========== Macros and Definitions =========================================*/

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_S_Sli__included */
