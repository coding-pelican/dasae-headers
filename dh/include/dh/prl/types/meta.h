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
#include "ErrRes.h"
#include "Err.h"

// ============================================================================
// Generic Meta Types - The Foundation
// ============================================================================

typedef struct u_P_const$raw {
    union {
        P_const$raw inner;
        P_const$raw raw;
    };
    TypeInfo type;
} u_P_const$raw;
typedef union u_P$raw {
    struct {
        union {
            P$raw inner;
            P$raw raw;
        };
        TypeInfo type;
    };
    u_P_const$raw as_const;
} u_P$raw;

typedef union u_V$raw {
    struct {
        TypeInfo inner_type;
        P$raw inner;
    };
    u_P$raw ref;
} u_V$raw;

typedef struct u_S_const$raw {
    union {
        struct {
            P_const$raw ptr;
            usize len;
        };
        S_const$raw inner;
        S_const$raw raw;
    };
    TypeInfo type;
    struct {
        u_P_const$raw ptr[0];
    } __type_hint[0];
} u_S_const$raw;
typedef union u_S$raw {
    struct {
        union {
            struct {
                P$raw ptr;
                usize len;
            };
            S$raw inner;
            S$raw raw;
        };
        TypeInfo type;
    };
    u_S_const$raw as_const;
    struct {
        u_P$raw ptr[0];
    } __type_hint[0];
} u_S$raw;

typedef union u_A$raw {
    struct {
        S$raw inner;
        TypeInfo inner_type;
    };
    u_S$raw ref;
} u_A$raw;

typedef struct u_O$raw {
    bool is_some;
    union {
        union {
            Void none;
            u_V$raw some;
        } payload;
        u_V$raw inner;
    };
} u_O$raw;

typedef struct u_E$raw {
    bool is_ok;
    union {
        union {
            ErrCode err;
            u_V$raw ok;
        } payload;
        u_V$raw inner;
    };
} u_E$raw;

#define u_allocV(_type...) ({ \
    const TypeInfo __type = _type; \
    const P$raw __ptr = alloca(__type.size); \
    memset(__ptr, 0, __type.size); \
    lit$((u_V$raw){ .inner_type = __type, .inner = __ptr }); \
})

#define u_allocA(_len, _type...) ({ \
    const TypeInfo __type = _type; \
    const usize __len = _len; \
    const P$raw __ptr = alloca(__len * __type.size); \
    memset(__ptr, 0, __len * __type.size); \
    lit$((u_A$raw){ .inner_type = __type, .inner = { .ptr = __ptr, .len = __len } }); \
})

#define u_make(_type...) ({ \
    const TypeInfo __type = _type; \
    const P$raw __ptr = alloca(__type.size); \
    memset(__ptr, 0, __type.size); \
    lit$((u_V$raw){ .inner_type = __type, .inner = __ptr }); \
})
#define u_create(_type...) ({ \
    const TypeInfo __type = _type; \
    const P$raw __ptr = alloca(__type.size); \
    memset(__ptr, 0, __type.size); \
    lit$((u_P$raw){ .type = __type, .inner = __ptr }); \
})

#define u_from$S(/*(_type: TypeInfo)(_s: S$raw)*/... /*(u_S$raw)*/) __u_from$S__step(pp_defer(__u_from$S__emit)(__u_from$S__parseType __VA_ARGS__))
#define __u_from$S__step(...)                                       __VA_ARGS__
#define __u_from$S__parseType(_type...) \
    pp_Tok_if_(Tok_isConst$(_type))( \
        pp_Tok_then_(u_S_const$raw, Tok_removeConst$(_type)), \
        pp_Tok_else_(u_S$raw, _type) \
    ), \
        __u_from$S__parseS
#define __u_from$S__parseS(_s...)                _s
#define __u_from$S__emit(...)                    __u_from$S__emitNext(__VA_ARGS__)
#define __u_from$S__emitNext(_T_u, _type, _s...) lit$((_T_u){ .type = _type, .raw = _s })

#define u_init$S(/*(_type: TypeInfo)(_ptr: P$$(_T), _len: usize)*/... /*(u_S$raw)*/) __u_init$S__step(pp_defer(__u_init$S__emit)(__u_init$S__parseType __VA_ARGS__))
#define __u_init$S__step(...)                                                        __VA_ARGS__
#define __u_init$S__parseType(_type...)                                              _type, __u_init$S__parsePtrLen
#define __u_init$S__parsePtrLen(_ptr, _len...)                                       _ptr, _len
#define __u_init$S__emit(_type, _ptr, _len...) \
    pp_Tok_if_(Tok_isConst$(_type))( \
        pp_then_(__u_init$S__emitRef(Tok_removeConst$(_type), _ptr, _len)), \
        pp_else_(__u_init$S__emitRefMut(_type, _ptr, _len)) \
    )
#define __u_init$S__emitRef(_type, _ptr, _len...)    lit$((u_S_const$raw){ .type = _type, .ptr = _ptr, .len = _len })
#define __u_init$S__emitRefMut(_type, _ptr, _len...) lit$((u_S$raw){ .type = _type, .ptr = _ptr, .len = _len })

#define u_at$S(_s, _idx...)               __u_at$S(pp_uniqTok(s), pp_uniqTok(idx), _s, _idx)
#define __u_at$S(__s, __idx, _s, _idx...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__idx, usize) = _idx; \
    claim_assert_fmt(__idx < len$S(__s), "Index out of bounds: idx(%zu) >= len(%zu)", __idx, len$S(__s)); \
    lit$((u_TypeHintOf(TypeOf(__s), ptr)){ .type = __s.type, .inner = ptr$S(__s) + (__idx * __s.type.size) }); \
})

#define u_slice$S(_s, _range)                    __u_slice$S(pp_uniqTok(s), pp_uniqTok(range), _s, _range)
#define __u_slice$S(__s, __range, _s, _range...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__range, R) = _range; \
    claim_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$S(__s)); \
    lit$((TypeOf(__s)){ .type = __s.type, .ptr = ptr$S(__s) + (__range.begin * __s.type.size), .len = len$R(__range) }); \
})
#define u_prefix$S(_s, _end)                  __u_prefix$S(pp_uniqTok(s), pp_uniqTok(end), _s, _end)
#define __u_prefix$S(__s, __end, _s, _end...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__end, usize) = _end; \
    claim_assert_fmt(__end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __end, len$S(__s)); \
    lit$((TypeOf(__s)){ .type = __s.type, .ptr = ptr$S(__s), .len = __end }); \
})
#define u_suffix$S(_s, _begin)                    __u_suffix$S(pp_uniqTok(s), pp_uniqTok(begin), _s, _begin)
#define __u_suffix$S(__s, __begin, _s, _begin...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__begin, usize) = _begin; \
    claim_assert_fmt(__begin <= len$S(__s), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$S(__s)); \
    lit$((TypeOf(__s)){ .type = __s.type, .ptr = ptr$S(__s) + (__begin * __s.type.size), .len = len$S(__s) - __begin }); \
})

#define u_TypeHintOf(_TBase, _hint_field...) FieldTypeOf(_TBase, __type_hint->_hint_field[0])

$inline_always
$static fn_((u_copy$P(u_P$raw dst, u_P_const$raw src))(u_P$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    memcpy(dst.inner, src.inner, src.type.size);
    return dst;
}

$inline_always
$static fn_((u_move$P(u_P$raw dst, u_P$raw src))(u_P$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    memmove(dst.inner, src.inner, src.type.size);
    return dst;
}

$inline_always
$static fn_((u_set$P(u_P$raw dst, u_V$raw src))(u_P$raw)) {
    claim_assert_fmt(dst.type.size == src.inner_type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.inner_type.size);
    claim_assert_fmt(dst.type.align == src.inner_type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.inner_type.align);
    memcpy(dst.inner, src.inner, src.inner_type.size);
    return dst;
}

$inline_always
$static fn_((u_copy$S(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    claim_assert_fmt(dst.len == src.len, "Length mismatch: dst(%zu) != src(%zu)", dst.len, src.len);
    memcpy(dst.ptr, src.ptr, src.len * src.type.size);
    return dst;
}

$inline_always
$static fn_((u_move$S(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    claim_assert_fmt(dst.len == src.len, "Length mismatch: dst(%zu) != src(%zu)", dst.len, src.len);
    memmove(dst.ptr, src.ptr, src.len * src.type.size);
    return dst;
}

$inline_always
$static fn_((u_set$S(u_S$raw dst, u_V$raw src))(u_S$raw)) {
    claim_assert_fmt(dst.type.size == src.inner_type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.inner_type.size);
    claim_assert_fmt(dst.type.align == src.inner_type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.inner_type.align);
    for_(($r(0, dst.len))(i) {
        memcpy(u_at$S(dst, i).inner, src.inner, src.inner_type.size);
    });
    return dst;
}

// ============================================================================
// Conversion Patterns - The Core Innovation
// ============================================================================

#define u_ret$ u_ret$V
#define u_ret$V(_T)     ((u_V$raw){ .inner_type = typeInfo$(_T), .inner = &((_T){}) })
#define u_ret$A(_N, _T) ((u_A$raw){ .inner_type = typeInfo$(FieldTypeOf(A$$(_N, _T), val[0])), .inner = ref$A((A$$(_N, _T)){}) })
#define u_ret$O(_T)     ((u_O$raw){ .inner_type = typeInfo$(FieldTypeOf(O$$(_T), payload->some)), .inner = ((O$$(_T)){}).ref_raw })
#define u_ret$E(_T)     ((u_E$raw){ .inner_type = typeInfo$(FieldTypeOf(E$$(_T), payload->ok)), .inner = ((E$$(_T)){}).ref_raw })

#define u_any$P_const(_p...) ((u_P_const$raw){ .type = typeInfo$(TypeOf(*_p)), .raw = _p })
#define u_any$P(_p...)       ((u_P$raw){ .type = typeInfo$(TypeOf(*_p)), .raw = _p })
#define u_any$S_const(_s...) ((u_S_const$raw){ .type = typeInfo$(TypeOf(*_s.ptr)), .raw = _s.as_raw })
#define u_any$S(_s...)       ((u_S$raw){ .type = typeInfo$(TypeOf(*_s.ptr)), .raw = _s.as_raw })

#define u_any$V(_v...) ({ \
    let __p_v = &copy(_v); \
    lit$((u_V$raw){ .inner_type = typeInfo$(TypeOf(*__p_v)), .inner = __p_v }); \
})
#define u_any$A(_a...) ({ \
    let __p_a = &copy(_a); \
    lit$((u_A$raw){ .inner_type = typeInfo$(TypeOf(*__p_a->val)), .inner = ref$A(*__p_a).as_raw }); \
})
#define u_any$O(_o...) ({ \
    let __p_o = &copy(_o); \
    __p_o->is_some \
        ? (u_O$raw)some(u_any$V(__p_o->payload.some)) \
        : (u_O$raw)none(); \
})
#define u_any$E(_e...) ({ \
    let __p_e = &copy(_e); \
    __p_e->is_ok \
        ? (u_E$raw)ok(u_any$V(__p_e->payload.ok)) \
        : (u_E$raw)err(__p_e->payload.err); \
})

#define u_cast u_castV

#define u_castP(/*(_T)(_Expr...)*/... /*(P_const$(_T))*/) \
    __step_inline__uCast$P$(pp_defer(__emit_inline__uCast$P$)(__param_parse__uCast$P$ __VA_ARGS__))
#define __step_inline__uCast$P$(...)       __VA_ARGS__
#define __param_parse__uCast$P$(...)          __VA_ARGS__, __param_next__uCast$P$
#define __param_next__uCast$P$(...)           __VA_ARGS__
#define __emit_inline__uCast$P$(_P_T, _meta...) (*as$((_P_T*)(_meta.inner)))

#define u_castV(/*(_T)(_Expr...)*/... /*(_T)*/) \
    __step_inline__uCast$V$(pp_defer(__emit_inline__uCast$V$)(__param_parse__uCast$V$ __VA_ARGS__))
#define __step_inline__uCast$V$(...)          __VA_ARGS__
#define __param_parse__uCast$V$(...)          __VA_ARGS__, __param_next__uCast$V$
#define __param_next__uCast$V$(...)           __VA_ARGS__
#define __emit_inline__uCast$V$(_T, _meta...) (*as$((_T*)(_meta.inner)))

#define u_castS(/*(_T)(_Expr...)*/... /*(S_const$(_T))*/) \
    __step_inline__uCast$S$(pp_defer(__emit_inline__uCast$S$)(__param_parse__uCast$S$ __VA_ARGS__))
#define __step_inline__uCast$S$(...)          __VA_ARGS__
#define __param_parse__uCast$S$(...)          __VA_ARGS__, __param_next__uCast$S$
#define __param_next__uCast$S$(...)        __VA_ARGS__
#define __emit_inline__uCast$S$(_S_T, _meta...) (*as$((_S_T*)(&_meta.inner)))

#define u_castA(/*(_N,_T)(_Expr...)*/... /*(A$(_N,_T))*/) \
    __step_inline__uCast$A$(pp_defer(__emit_inline__uCast$A$)(__param_parse__uCast$A$ __VA_ARGS__))
#define __step_inline__uCast$A$(...)              __VA_ARGS__
#define __param_parse__uCast$A$(...)              __VA_ARGS__, __param_next__uCast$A$
#define __param_next__uCast$A$(...)               __VA_ARGS__
#define __emit_inline__uCast$A$(_A_N_T, _meta...) (*as$((_A_N_T*)(_meta.inner)))

#define u_castO(/*(_O_T)(_Expr...)*/... /*(_O_T)*/) \
    __step__u_castO__expand(pp_defer(__step__u_castO)(__step__u_castO__parseOT __VA_ARGS__))
#define __step__u_castO__expand(...) __VA_ARGS__
#define __step__u_castO__parseOT(_O_T...)                   pp_uniqTok(ty_hint), pp_uniqTok(meta), _O_T, __step__u_castO__parseExpr
#define __step__u_castO__parseExpr(_Expr...)                _Expr
#define __step__u_castO(__ty_hint, __meta, _O_T, _Expr...) ({ \
    let __ty_hint = lit$((_O_T[0]){}); \
    let __meta = _Expr; \
    __meta.is_some \
        ? lit$((TypeOf(__ty_hint[0]))some(*as$((FieldTypeOf(TypeOf(__ty_hint[0]), payload.some)*)(&__meta.payload.some.inner)))) \
        : lit$((TypeOf(__ty_hint[0]))none()); \
})

#define u_castE(/*(_E_T)(_Expr...)*/... /*(_E_T)*/) \
    __step__u_castE__expand(pp_defer(__step__u_castE)(__step__u_castE__parseET __VA_ARGS__))
#define __step__u_castE__expand(...) __VA_ARGS__
#define __step__u_castE__parseET(_E_T...)                   pp_uniqTok(ty_hint), pp_uniqTok(meta), _E_T, __step__u_castE__parseExpr
#define __step__u_castE__parseExpr(_Expr...)                _Expr
#define __step__u_castE(__ty_hint, __meta, _E_T, _Expr...) ({ \
    let __ty_hint = lit$((_E_T[0]){}); \
    let __meta = _Expr; \
    __meta.is_ok \
        ? lit$((TypeOf(__ty_hint[0]))ok(*as$((FieldTypeOf(TypeOf(__ty_hint[0]), payload.ok)*)(&__meta.payload.ok.inner)))) \
        : lit$((TypeOf(__ty_hint[0]))err(__meta.payload.err)); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* types_meta__included */
