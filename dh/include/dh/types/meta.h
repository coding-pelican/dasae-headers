#ifndef types_meta__included
#define types_meta__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "raw.h"
#include "Val.h"
#include "Ptr.h"
#include "Arr.h"
#include "Sli.h"
#include "Opt.h"
#include "Res.h"
#include "Err.h"

// ============================================================================
// Generic Meta Types - The Foundation
// ============================================================================

typedef struct meta_P_const$raw {
    TypeInfo type;
    union {
        P_const$raw inner;
        P_const$raw raw;
    };
} meta_P_const$raw;
typedef union meta_P$raw {
    struct {
        TypeInfo type;
        union {
            P$raw inner;
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
        S_const$raw inner;
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
            S$raw inner;
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
        } payload;
        meta_V$raw inner;
    };
} meta_O$raw;

typedef struct meta_E$raw {
    bool is_ok;
    union {
        union {
            ErrCode err;
            meta_V$raw ok;
        } payload;
        meta_V$raw inner;
    };
} meta_E$raw;

#define meta_make(_type...) ({ \
    const TypeInfo __type = _type; \
    const P$raw __ptr = bti_alloca(__type.size); \
    bti_memset(__ptr, 0, __type.size); \
    lit$((meta_V$raw){ .inner_type = __type, .inner = __ptr }); \
})
#define meta_create(_type...) ({ \
    const TypeInfo __type = _type; \
    const P$raw __ptr = bti_alloca(__type.size); \
    bti_memset(__ptr, 0, __type.size); \
    lit$((meta_P$raw){ .type = __type, .inner = __ptr }); \
})
#define meta_alloc(_type, _len...) ({ \
    const TypeInfo __type = _type; \
    const usize __len = _len; \
    const P$raw __ptr = bti_alloca(__type.size * __len); \
    bti_memset(__ptr, 0, __type.size * __len); \
    lit$((meta_S$raw){ .type = __type, .ptr = __ptr, .len = __len }); \
})

#define meta_init$S(/*(_type: TypeInfo)(_ptr: P$$(_T), _len: usize)*/... /*(meta_S$raw)*/) __meta_init$S__step(pp_defer(__meta_init$S__emit)(__meta_init$S__parseType __VA_ARGS__))
#define __meta_init$S__step(...)                                                           __VA_ARGS__
#define __meta_init$S__parseType(_type...)                                                 _type, __meta_init$S__parsePtrLen
#define __meta_init$S__parsePtrLen(_ptr, _len...)                                          _ptr, _len
#define __meta_init$S__emit(_type, _ptr, _len...) \
    pp_Tok_if_(Tok_isConst$(_type))( \
        pp_then_(__meta_init$S__emitRef(Tok_removeConst$(_type), _ptr, _len)), \
        pp_else_(__meta_init$S__emitRefMut(_type, _ptr, _len)) \
    )
#define __meta_init$S__emitRef(_type, _ptr, _len...)    lit$((meta_S_const$raw){ .type = _type, .ptr = _ptr, .len = _len })
#define __meta_init$S__emitRefMut(_type, _ptr, _len...) lit$((meta_S$raw){ .type = _type, .ptr = _ptr, .len = _len })

#define meta_at$S(_s, _idx...)               __meta_at$S(pp_uniqTok(s), pp_uniqTok(idx), _s, _idx)
#define __meta_at$S(__s, __idx, _s, _idx...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__idx, usize) = _idx; \
    debug_assert_fmt(__idx < len$S(__s), "Index out of bounds: idx(%zu) >= len(%zu)", __idx, len$S(__s)); \
    lit$((meta_P$raw){ .type = __s.type, .inner = &ptr$S(__s)[__idx * __s.type.size] }); \
})

#define meta_slice$S(_s, _range)                    __meta_slice$S(pp_uniqTok(s), pp_uniqTok(range), _s, _range)
#define __meta_slice$S(__s, __range, _s, _range...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__range, R) = _range; \
    debug_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    debug_assert_fmt(__range.end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$S(__s)); \
    lit$((TypeOf(__s))meta_init$S((__s.type)(&ptr$S(__s)[__range.begin * __s.type.size], len$R(__range)))); \
})
#define meta_prefix$S(_s, _end)                  __meta_prefix$S(pp_uniqTok(s), pp_uniqTok(end), _s, _end)
#define __meta_prefix$S(__s, __end, _s, _end...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__end, usize) = _end; \
    debug_assert_fmt(__end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __end, len$S(__s)); \
    lit$((TypeOf(__s))meta_init$S((__s.type)(ptr$S(__s), __end))); \
})
#define meta_suffix$S(_s, _begin)                    __meta_suffix$S(pp_uniqTok(s), pp_uniqTok(begin), _s, _begin)
#define __meta_suffix$S(__s, __begin, _s, _begin...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__begin, usize) = _begin; \
    debug_assert_fmt(__begin <= len$S(__s), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$S(__s)); \
    lit$((TypeOf(__s))meta_init$S((__s.type)(ptr$S(__s) + __begin * __s.type.size, len$S(__s) - __begin))); \
})

$inline_always
$static fn_((meta_copy$P(meta_P$raw dst, meta_P_const$raw src))(meta_P$raw)) {
    debug_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    debug_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    bti_memcpy(dst.inner, src.inner, src.type.size);
    return dst;
}

$inline_always
$static fn_((meta_move$P(meta_P$raw dst, meta_P$raw src))(meta_P$raw)) {
    debug_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    debug_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    bti_memmove(dst.inner, src.inner, src.type.size);
    return dst;
}

$inline_always
$static fn_((meta_set$P(meta_P$raw dst, meta_V$raw src))(meta_P$raw)) {
    debug_assert_fmt(dst.type.size == src.inner_type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.inner_type.size);
    debug_assert_fmt(dst.type.align == src.inner_type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.inner_type.align);
    bti_memcpy(dst.inner, src.inner, src.inner_type.size);
    return dst;
}

$inline_always
$static fn_((meta_copy$S(meta_S$raw dst, meta_S_const$raw src))(meta_S$raw)) {
    debug_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    debug_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    debug_assert_fmt(dst.len == src.len, "Length mismatch: dst(%zu) != src(%zu)", dst.len, src.len);
    bti_memcpy(dst.ptr, src.ptr, src.len * src.type.size);
    return dst;
}

$inline_always
$static fn_((meta_move$S(meta_S$raw dst, meta_S$raw src))(meta_S$raw)) {
    debug_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    debug_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    debug_assert_fmt(dst.len == src.len, "Length mismatch: dst(%zu) != src(%zu)", dst.len, src.len);
    bti_memmove(dst.ptr, src.ptr, src.len * src.type.size);
    return dst;
}

$inline_always
$static fn_((meta_set$S(meta_S$raw dst, meta_V$raw src))(meta_S$raw)) {
    debug_assert_fmt(dst.type.size == src.inner_type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.inner_type.size);
    debug_assert_fmt(dst.type.align == src.inner_type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.inner_type.align);
    for_(($r(0, dst.len))(i) {
        bti_memcpy(meta_at$S(dst, i).inner, src.inner, src.inner_type.size);
    });
    return dst;
}

// ============================================================================
// Conversion Patterns - The Core Innovation
// ============================================================================

#define meta_ret$V(_T)     ((meta_V$raw){ .inner_type = typeInfo$(_T), .inner = &((_T){}) })
#define meta_ret$A(_N, _T) ((meta_A$raw){ .inner_type = typeInfo$(FieldTypeOf(A$$(_N, _T), val[0])), .inner = ref$A((A$$(_N, _T)){}) })
#define meta_ret$O(_T)     ((meta_O$raw){ .inner_type = typeInfo$(FieldTypeOf(O$$(_T), payload->some)), .inner = ((O$$(_T)){}).ref_raw })
#define meta_ret$E(_T)     ((meta_E$raw){ .inner_type = typeInfo$(FieldTypeOf(E$$(_T), payload->ok)), .inner = ((E$$(_T)){}).ref_raw })

#define any$P_const(_p...) ((meta_P_const$raw){ .type = typeInfo$(TypeOf(*_p)), .raw = _p })
#define any$P(_p...)       ((meta_P$raw){ .type = typeInfo$(TypeOf(*_p)), .raw = _p })
#define any$S_const(_s...) ((meta_S_const$raw){ .type = typeInfo$(TypeOf(*_s.ptr)), .raw = _s.as_raw })
#define any$S(_s...)       ((meta_S$raw){ .type = typeInfo$(TypeOf(*_s.ptr)), .raw = _s.as_raw })

#define any$V(_v...) ({ \
    let __p_v = &copy(_v); \
    lit$((meta_V$raw){ .inner_type = typeInfo$(TypeOf(*__p_v)), .inner = __p_v }); \
})
#define any$A(_a...) ({ \
    let __p_a = &copy(_a); \
    lit$((meta_A$raw){ .inner_type = typeInfo$(TypeOf(*__p_a->val)), .inner = ref$A(*__p_a).as_raw }); \
})
#define any$O(_o...) ({ \
    let __p_o = &copy(_o); \
    __p_o->is_some \
        ? (meta_O$raw)some(any$V(__p_o->payload.some)) \
        : (meta_O$raw)none(); \
})
#define any$E(_e...) ({ \
    let __p_e = &copy(_e); \
    __p_e->is_ok \
        ? (meta_E$raw)ok(any$V(__p_e->payload.ok)) \
        : (meta_E$raw)err(__p_e->payload.err); \
})

#define meta$P$(/*(_T)(_Expr...)*/... /*(P_const$(_T))*/) \
    __step_inline__meta$P$(pp_defer(__emit_inline__meta$P$)(__param_parse__meta$P$ __VA_ARGS__))
#define __step_inline__meta_pRef$(...)       __VA_ARGS__
#define __param_parse__meta$P$(...)          __VA_ARGS__, __param_next__meta$P$
#define __param_next__meta$P$(...)           __VA_ARGS__
#define __emit_inline__meta$P$(_T, _meta...) (*as$((P$(_T)*)(&__meta.raw)))
#define meta$P$$(/*(_T)(_Expr...)*/... /*(P_const$$(_T))*/) \
    __step_inline__meta$P$$(pp_defer(__emit_inline__meta$P$$)(__param_parse__meta$P$$ __VA_ARGS__))
#define __step_inline__meta$P$$(...)          __VA_ARGS__
#define __param_parse__meta$P$$(...)          __VA_ARGS__, __param_next__meta$P$$
#define __param_next__meta$P$$(...)           __VA_ARGS__
#define __emit_inline__meta$P$$(_T, _meta...) (*as$((P$$(_T)*)(&__meta.raw)))

#define meta$V$(/*(_T)(_Expr...)*/... /*(_T)*/) \
    __step_inline__meta$V$(pp_defer(__emit_inline__meta$V$)(__param_parse__meta$V$ __VA_ARGS__))
#define __step_inline__meta$V$(...)          __VA_ARGS__
#define __param_parse__meta$V$(...)          __VA_ARGS__, __param_next__meta$V$
#define __param_next__meta$V$(...)           __VA_ARGS__
#define __emit_inline__meta$V$(_T, _meta...) (*as$((_T*)(_meta.inner)))

#define meta$S$(/*(_T)(_Expr...)*/... /*(S_const$(_T))*/) \
    __step_inline__meta$S$(pp_defer(__emit_inline__meta$S$)(__param_parse__meta$S$ __VA_ARGS__))
#define __step_inline__meta$S$(...)          __VA_ARGS__
#define __param_parse__meta$S$(...)          __VA_ARGS__, __param_next__meta$S$
#define __param_next__meta_sRef$(...)        __VA_ARGS__
#define __emit_inline__meta$S$(_T, _meta...) (*as$((S$(_T)*)(&__meta.raw)))
#define meta$S$$(/*(_T)(_Expr...)*/... /*(S_const$$(_T))*/) \
    __step_inline__meta$S$$(pp_defer(__emit_inline__meta$S$$)(__param_parse__meta$S$$ __VA_ARGS__))
#define __step_inline__meta$S$$(...)          __VA_ARGS__
#define __param_parse__meta$S$$(...)          __VA_ARGS__, __param_next__meta$S$$
#define __param_next__meta$S$$(...)           __VA_ARGS__
#define __emit_inline__meta$S$$(_T, _meta...) (*as$((S$$(_T)*)(&__meta.raw)))

#define meta$A$(/*(_N,_T)(_Expr...)*/... /*(A$(_N,_T))*/) \
    __step_inline__meta$A$(pp_defer(__emit_inline__meta$A$)(__param_parse__meta$A$ __VA_ARGS__))
#define __step_inline__meta$A$(...)              __VA_ARGS__
#define __param_parse__meta$A$(...)              __VA_ARGS__, __param_next__meta$A$
#define __param_next__meta$A$(...)               __VA_ARGS__
#define __emit_inline__meta$A$(_N, _T, _meta...) (*as$((A$(_N, _T)*)(__meta.inner)))
#define meta$A$$(/*(_N,_T)(_Expr...)*/... /*(A$$(_N,_T))*/) \
    __step_inline__meta$A$$(pp_defer(__emit_inline__meta$A$$)(__param_parse__meta$A$$ __VA_ARGS__))
#define __step_inline__meta$A$$(...)              __VA_ARGS__
#define __param_parse__meta$A$$(...)              __VA_ARGS__, __param_next__meta$A$$
#define __param_next__meta$A$$(...)               __VA_ARGS__
#define __emit_inline__meta$A$$(_N, _T, _meta...) (*as$((A$$(_N, _T)*)(__meta.inner)))

#define meta$O$(/*(_T)(_Expr...)*/... /*(O$(_T))*/) \
    pp_expand(pp_defer(__block_inline__meta_O$)(__param_expand__meta_O$ __VA_ARGS__))
#define __param_expand__meta_O$(...)                  pp_uniqTok(meta), __VA_ARGS__, __param_expand1__meta_O$
#define __param_expand1__meta_O$(...)                 __VA_ARGS__
#define __block_inline__meta_O$(__meta, _T, _meta...) ({ \
    let __meta = _meta; \
    __meta.is_some \
        ? ((O$(_T))some(*as$((_T*)(__meta.payload.some.inner)))) \
        : ((O$(_T))none()); \
})
#define meta$O$$(/*(_T)(_Expr...)*/... /*(O$$(_T))*/) \
    pp_expand(pp_defer(__block_inline__meta_O$$)(__param_expand__meta_O$$ __VA_ARGS__))
#define __param_expand__meta_O$$(...)                  pp_uniqTok(meta), __VA_ARGS__, __param_expand1__meta_O$$
#define __param_expand1__meta_O$$(...)                 __VA_ARGS__
#define __block_inline__meta_O$$(__meta, _T, _meta...) ({ \
    let __meta = _meta; \
    __meta.is_some \
        ? ((O$$(_T))some(*as$((_T*)(__meta.payload.some.inner)))) \
        : ((O$$(_T))none()); \
})

#define meta$E$(/*(_T)(_Expr...)*/... /*(E$(_T))*/) \
    pp_expand(pp_defer(__block_inline__meta_E$)(__param_expand__meta_E$ __VA_ARGS__))
#define __param_expand__meta_E$(...)                  pp_uniqTok(meta), __VA_ARGS__, __param_expand1__meta_E$
#define __param_expand1__meta_E$(...)                 __VA_ARGS__
#define __block_inline__meta_E$(__meta, _T, _meta...) ({ \
    let __meta = _meta; \
    __meta.is_ok \
        ? ((E$(_T))ok(*as$((_T*)(&__meta.payload.ok.inner)))) \
        : ((E$(_T))err(__meta.payload.err)); \
})
#define meta$E$$(/*(_T)(_Expr...)*/... /*(E$$(_T))*/) \
    pp_expand(pp_defer(__block_inline__meta_E$$)(__param_expand__meta_E$$ __VA_ARGS__))
#define __param_expand__meta_E$$(...)                  pp_uniqTok(meta), __VA_ARGS__, __param_expand1__meta_E$$
#define __param_expand1__meta_E$$(...)                 __VA_ARGS__
#define __block_inline__meta_E$$(__meta, _T, _meta...) ({ \
    let __meta = _meta; \
    __meta.is_ok \
        ? ((E$$(_T))ok(*as$((_T*)(&__meta.payload.ok.inner)))) \
        : ((E$$(_T))err(__meta.payload.err)); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* types_meta__included */
