#ifndef prl_types_Sli__included
#define prl_types_Sli__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "raw.h"

/*========== Macros and Declarations ========================================*/

/* Slice Anonymous */
#define S_const$$(_T...) __S_const$$(_T)
#define S$$(_T...) pp_Tok_if_(Tok_isConst$(_T))( \
    pp_Tok_then_(__S_const$$(Tok_removeConst$(_T))), \
    pp_Tok_else_(__S$$(_T)) \
)
#define __S_const$$(_T...) TypeOf(union { \
    struct { \
        var_(ptr, $P$(const _T)); \
        var_(len, usize); \
    }; \
    var_(as_raw, S_const$raw); \
    var_(ref_raw $like_ref, S_const$raw); \
})
#define __S$$(_T...) TypeOf(union { \
    struct { \
        var_(ptr, $P$(_T)); \
        var_(len, usize); \
    }; \
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
#define __S_const$(_T...) pp_join($, S_const, _T)
#define __S$(_T...) pp_join($, S, _T)

/* Slice Template */
#define T_decl_S$(_T...) \
    $maybe_unused typedef union S$(const _T) S$(const _T); \
    $maybe_unused typedef union S$(_T) S$(_T)
#define T_impl_S$(_T...) \
    union S$(const _T) { \
        struct { \
            var_(ptr, $P$(const _T)); \
            var_(len, usize); \
        }; \
        var_(as_raw, S$(const raw)); \
        var_(ref_raw $like_ref, S$(const raw)); \
    }; \
    union S$(_T) { \
        struct { \
            var_(ptr, $P$(_T)); \
            var_(len, usize); \
        }; \
        var_(as_raw, S$raw); \
        var_(ref_raw $like_ref, S$raw); \
        var_(as_const, S$(const _T)); \
    }
#define T_use_S$(_T...) \
    T_decl_S$(_T); \
    T_impl_S$(_T)

#define S_InnerT$(_T...) TypeOf(*(as$(_T*)(null))->ptr)
#define S_InnerTUnqual$(_T...) TypeOfUnqual(*(as$(_T*)(null))->ptr)
#define S_isConst$(_T...) Type_eq$(S_InnerT$(_T)*, const S_InnerTUnqual$(_T)*)

/* Slice Operations */
#define zeroS zero$S
#define zero$S() init$S(null, 0)
#define zeroS$ zero$S$
#define zero$S$(/*(_T)*/... /*(S$(_T))*/) init$S$(__VA_ARGS__(null, 0))
#define zero$S$$(/*(_T)*/... /*(S$$(_T))*/) init$S$$(__VA_ARGS__(null, 0))

#define initS init$S
#define init$S(_ptr, _len...) { .ptr = _ptr, .len = _len }
#define initS$ init$S$
#define init$S$(/*(_T)(_ptr: P$$(_T), _len: usize)*/... /*(S$(_T))*/) \
    __lit_init$S__step(pp_defer(__lit_init$S__emit)(S$, __lit_init$S__parseT __VA_ARGS__))
#define init$S$$(/*(_T)(_ptr: P$$(_T), _len: usize)*/... /*(S$$(_T))*/) \
    __lit_init$S__step(pp_defer(__lit_init$S__emit)(S$$, __lit_init$S__parseT __VA_ARGS__))

#define asgS(_p_s, _v_s...) asgS1(_p_s, _v_s)
#define asgS1(_p_s, _v_s...) asg(_p_s, _v_s, (ptr->ptr))
#define asgS2(_p_s, _v_s...) asg(_p_s, _v_s, (ptr->ptr->ptr))
#define asgS3(_p_s, _v_s...) asg(_p_s, _v_s, (ptr->ptr->ptr->ptr))
#define asgS4(_p_s, _v_s...) asg(_p_s, _v_s, (ptr->ptr->ptr->ptr->ptr))

#define derefS$ deref$S$
#define deref$S$(/*(_N, _T)(_s: S$$(_T))*/... /*(A$(_N,_T))*/) \
    __step_inline__deref$S$(pp_defer(__emit_inline__deref$S$)(__param_parse__deref$S$ __VA_ARGS__))
#define deref$S$$(/*(_N, _T)(_s: S$$(_T))*/... /*(A$$(_N,_T))*/) \
    __step_inline__deref$S$$(pp_defer(__emit_inline__deref$S$$)(__param_parse__deref$S$$ __VA_ARGS__))

#define mutCastS mutCast$S
#define mutCast$S(_s /*: S$$(_T)*/... /*(S_const$$(_T))*/) (_s.as_const)
#define constCastS constCast$S
#define constCast$S(_s /*: S_const$$(_T)*/... /*(S$$(_T))*/) /* TODO: Implement */

#define S_ptr ptr$S
#define S_len len$S
#define S_len$ len$S$

#define ptrS ptr$S
#define ptr$S(_s /*: S$$(_T)*/... /*(P$$(_T))*/) ((_s).ptr)
#define lenS len$S
#define len$S(_s /*: S$$(_T)*/... /*(usize)*/) ((_s).len)

#define atS at$S
#define at$S(_s /*: S$$(_T)*/, _idx /*: usize*/... /*(P$$(_T))*/) \
    pp_expand(pp_defer(block_inline__at$S)(param_expand__at$S(_s, _idx)))

#define S_deref$(/*(_ANT: A(_N,_T))(_self: S(_T))*/... /*(_ANT)*/) __op__S_deref$(__op__S_deref$__parseANT __VA_ARGS__)
#define __op__S_deref$(...) __op__S_deref$__emit(__VA_ARGS__)
#define __op__S_deref$__parseANT(_ANT...) _ANT,
#define __op__S_deref$__emit(_ANT, _s...) (*blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    claim_assert_fmt(S_len(__s) == A_innerN$(_ANT), "length mismatch: len(%zu) != N(%zu)", S_len(__s), A_innerN$(_ANT)); \
    as$(_ANT*)(ensureNonnull(S_ptr(__s))); \
}))

#define S_at(/*(_self: S(_T))[_idx]*/... /*(P(_T))*/) \
    __op__S_at(__op__S_at__parse __VA_ARGS__)
#define __op__S_at__parse(_s...) pp_uniqTok(s), pp_uniqTok(idx), _s,
#define __op__S_at(...) __op__S_at__emit(__VA_ARGS__)
#define __op__S_at__emit(__s, __idx, _s, _idx...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _idx)); \
    claim_assert_fmt(__idx < S_len(__s), "Index out of bounds: idx(%zu) >= len(%zu)", __idx, S_len(__s)); \
    S_ptr(__s) + __idx; \
})
#if UNUSED_CODE
#define __op__S_at__emit(__s, __idx, _s, _idx...) ({ \
    let_(__s, TypeOf(&(_s))) = &(_s); \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _idx)); \
    claim_assert_fmt(__idx < S_len(*__s), "Index out of bounds: idx(%zu) >= len(%zu)", __idx, S_len(*__s)); \
    S_ptr(*__s) + __idx; \
})
#endif /* UNUSED_CODE */
#define S_slice(/*(_self: S(_T))_range*/... /*(S(_T))*/) \
    __op__S_slice(__op__S_slice__parse __VA_ARGS__)
#define __op__S_slice__parse(_s...) pp_uniqTok(s), pp_uniqTok(range), _s,
#define __op__S_slice(...) __op__S_slice__emit(__VA_ARGS__)
#define __op__S_slice__emit(__s, __range, _s, _range...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__range, R) = _range; \
    claim_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= S_len(__s), "Invalid slice range: end(%zu) > len(%zu)", __range.end, S_len(__s)); \
    lit$((TypeOf(__s)){ .ptr = &S_ptr(__s)[__range.begin], .len = len$R(__range) }); \
})
#define S_prefix(/*(_self: S(_T))_end*/... /*(S(_T))*/) \
    __op__S_prefix(__op__S_prefix__parse __VA_ARGS__)
#define __op__S_prefix__parse(_s...) pp_uniqTok(s), pp_uniqTok(end), _s,
#define __op__S_prefix(...) __op__S_prefix__emit(__VA_ARGS__)
#define __op__S_prefix__emit(__s, __end, _s, _end...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__end, usize) = _end; \
    claim_assert_fmt(__end <= S_len(__s), "Invalid slice range: end(%zu) > len(%zu)", __end, S_len(__s)); \
    lit$((TypeOf(__s)){ .ptr = S_ptr(__s), .len = __end }); \
})
#define S_suffix(/*(_self: S(_T))_begin*/... /*(S(_T))*/) \
    __op__S_suffix(__op__S_suffix__parse __VA_ARGS__)
#define __op__S_suffix__parse(_s...) pp_uniqTok(s), pp_uniqTok(begin), _s,
#define __op__S_suffix(...) __op__S_suffix__emit(__VA_ARGS__)
#define __op__S_suffix__emit(__s, __begin, _s, _begin...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__begin, usize) = _begin; \
    claim_assert_fmt(__begin <= S_len(__s), "Invalid slice range: begin(%zu) > len(%zu)", __begin, S_len(__s)); \
    lit$((TypeOf(__s)){ .ptr = S_ptr(__s) + __begin, .len = S_len(__s) - __begin }); \
})

#define sliceS slice$S
#define slice$S(/*_s: S$$(_T), $r(_begin, _end): R*/... /*(S$$(_T))*/) __param_expand__slice$S(__VA_ARGS__)
#define slice$S$(/*(_T)(_s: S$$(_T), $r(_begin, _end): R)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__slice$S$)(__param_expand__slice$S$ __VA_ARGS__))
#define prefixS prefix$S
#define prefix$S(/*_s: S$$(_T), _end: usize*/... /*(S$$(_T))*/) __param_expand__prefix$S(__VA_ARGS__)
#define prefix$S$(/*(_T)(_s: S$$(_T), _end: usize)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__prefix$S$)(__param_expand__prefix$S$ __VA_ARGS__))
#define suffixS suffix$S
#define suffix$S(/*_s: S$$(_T), _begin: usize*/... /*(S$$(_T))*/) __param_expand__suffix$S(__VA_ARGS__)
#define suffix$S$(/*(_T)(_s: S$$(_T), _begin: usize)*/... /*(S_const$(_T))*/) pp_expand(pp_defer(__block_inline__suffix$S$)(__param_expand__suffix$S$ __VA_ARGS__))

#define u8_c(_literal...) comp_syn__u8_c(_literal)
#define u16_c(_literal...) comp_syn__u16_c(_literal)
#define u32_c(_literal...) comp_syn__u32_c(_literal)
#define u8_a(_literal...) comp_syn__u8_a(_literal)
#define u8_s(_literal...) comp_syn__u8_s(_literal)
#define u8_l(_literal...) comp_syn__u8_l(_literal)
#define u8_az(_literal...) comp_syn__u8_az(_literal)
#define u8_sz(_literal...) comp_syn__u8_sz(_literal)
#define u8_lz(_literal...) comp_syn__u8_lz(_literal)
#define u8z_a(_literal...) comp_syn__u8z_a(_literal)
#define u8z_s(_literal...) comp_syn__u8z_s(_literal)
#define u8z_l(_literal...) comp_syn__u8z_l(_literal)

#define u8_s0(_literal...) comp_syn__u8_s0(_literal)
#define u8_l0(_literal...) comp_syn__u8_l0(_literal)

/*========== Macros and Definitions =========================================*/

#define __lit_init$S__step(...) __VA_ARGS__
#define __lit_init$S__parseT(_T...) _T, __lit_init$S__parsePtrLen
#define __lit_init$S__parsePtrLen(_ptr, _len...) _ptr, _len
#define __lit_init$S__emit(_AliasFn, _T, _ptr, _len...) \
    lit$((_AliasFn(_T))init$S(_ptr, _len))

#define __step_inline__deref$S$(...) __VA_ARGS__
#define __param_parse__deref$S$(...) __VA_ARGS__, __param_next__deref$S$
#define __param_next__deref$S$(...) pp_uniqTok(s), __VA_ARGS__
#define __emit_inline__deref$S$(_N, _T, __s, _s...) (*({ \
    let_(__s, TypeOf(_s)) = _s; \
    claim_assert_fmt(len$S(__s) == _N, "length mismatch: len(%zu) != N(%zu)", len$S(__s), _N); \
    as$(A$(_N, _T)*)(ensureNonnull(ptr$S(__s))); \
}))

#define __step_inline__deref$S$$(...) __VA_ARGS__
#define __param_parse__deref$S$$(...) __VA_ARGS__, __param_next__deref$S$$
#define __param_next__deref$S$$(...) pp_uniqTok(s), __VA_ARGS__
#define __emit_inline__deref$S$$(_N, _T, __s, _s...) (*({ \
    let_(__s, TypeOf(_s)) = _s; \
    claim_assert_fmt(len$S(__s) == _N, "length mismatch: len(%zu) != N(%zu)", len$S(__s), _N); \
    as$(A$$(_N, _T)*)(ensureNonnull(ptr$S(__s))); \
}))

#define param_expand__at$S(_s, _idx...) pp_uniqTok(s), pp_uniqTok(idx), _s, _idx
#define block_inline__at$S(__s, __idx, _s, _idx...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__idx, usize) = _idx; \
    claim_assert_fmt(__idx < len$S(__s), "Index out of bounds: idx(%zu) >= len(%zu)", __idx, len$S(__s)); \
    &ptr$S(__s)[__idx]; \
})

#define __param_expand__slice$S(_s, _range...) __block_inline__slice$S(pp_uniqTok(s), pp_uniqTok(range), _s, _range)
#define __block_inline__slice$S(__s, __range, _s, _range...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__range, R) = _range; \
    claim_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$S(__s)); \
    lit$((TypeOf(__s))init$S(&ptr$S(__s)[__range.begin], len$R(__range))); \
})

#define __param_expand__slice$S$(...) __VA_ARGS__, pp_uniqTok(s), pp_uniqTok(range), __param_next__slice$S$
#define __param_next__slice$S$(...) __VA_ARGS__
#define __block_inline__slice$S$(_T, __s, __range, _s, _range...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__range, R) = _range; \
    claim_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$S(__s)); \
    init$S$((_T)(&ptr$S(__s)[__range.begin], len$R(__range))); \
})

#define __param_expand__prefix$S(_s, _end...) __block_inline__prefix$S(pp_uniqTok(s), pp_uniqTok(end), _s, _end)
#define __block_inline__prefix$S(__s, __end, _s, _end...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__end, usize) = _end; \
    claim_assert_fmt(__end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __end, len$S(__s)); \
    lit$((TypeOf(__s))init$S(ptr$S(__s), __end)); \
})

#define __param_expand__prefix$S$(...) __VA_ARGS__, pp_uniqTok(s), pp_uniqTok(end), __param_next__prefix$S$
#define __param_next__prefix$S$(...) __VA_ARGS__
#define __block_inline__prefix$S$(_T, __s, __end, _s, _end...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__end, usize) = _end; \
    claim_assert_fmt(__end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __end, len$S(__s)); \
    init$S$((_T)(ptr$S(__s), __end)); \
})

#define __param_expand__suffix$S(_s, _begin...) __block_inline__suffix$S(pp_uniqTok(s), pp_uniqTok(begin), _s, _begin)
#define __block_inline__suffix$S(__s, __begin, _s, _begin...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__begin, usize) = _begin; \
    claim_assert_fmt(__begin <= len$S(__s), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$S(__s)); \
    lit$((TypeOf(__s))init$S(ptr$S(__s) + __begin, len$S(__s) - __begin)); \
})

#define __param_expand__suffix$S$(...) __VA_ARGS__, pp_uniqTok(s), pp_uniqTok(begin), __param_next__suffix$S$
#define __param_next__suffix$S$(...) __VA_ARGS__
#define __block_inline__suffix$S$(_T, __s, __begin, _s, _begin...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__begin, usize) = _begin; \
    claim_assert_fmt(__begin <= len$S(__s), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$S(__s)); \
    init$S$((_T)(ptr$S(__s) + __begin, len$S(__s) - __begin)); \
})

#define comp_syn__u8_c(_literal...) lit$((u8){ _literal })
#define comp_syn__u16_c(_literal...) lit$((u16){ u##_literal })
#define comp_syn__u32_c(_literal...) lit$((u32){ U##_literal })

#define comp_syn__u8_a(_literal...) pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wunterminated-string-initialization\"", \
    "clang diagnostic pop", \
    lit$((A$$(sizeOf$(_literal) - 1, u8)){ .val = { _literal } }) \
)
#define comp_syn__u8_s(_literal...) lit$((S$u8){ .ptr = lit$((u8[]){ "" _literal }), .len = sizeOf$(TypeOf(_literal)) - 1 })
#define comp_syn__u8_l(_literal...) lit$((S_const$u8){ .ptr = as$(const u8*)("" _literal), .len = sizeOf$(TypeOf(_literal)) - 1 })

#define comp_syn__u8_az(_literal...) lit$((A$$(sizeOf$(_literal), u8)){ .val = { _literal } })
#define comp_syn__u8_sz(_literal...) lit$((S$u8){ .ptr = lit$((u8[]){ "" _literal }), .len = sizeOf$(TypeOf(_literal)) })
#define comp_syn__u8_lz(_literal...) lit$((S_const$u8){ .ptr = as$(const u8*)("" _literal), .len = sizeOf$(TypeOf(_literal)) })

#define comp_syn__u8z_a(_literal...) lit$((AZ$$(sizeOf$(_literal) - 1, u8)){ .val = { _literal } })
#define comp_syn__u8z_s(_literal...) lit$((SZ$u8){ .ptr = lit$((u8[]){ "" _literal }), .len = sizeOf$(TypeOf(_literal)) - 1 })
#define comp_syn__u8z_l(_literal...) lit$((SZ_const$u8){ .ptr = as$(const u8*)("" _literal), .len = sizeOf$(TypeOf(_literal)) - 1 })

#define comp_syn__u8_s0(_literal...) lit$((u8[]){ _literal })
#define comp_syn__u8_l0(_literal...) as$(const u8*)(_literal)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_types_Sli__included */
