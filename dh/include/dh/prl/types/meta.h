#ifndef prl_types_meta__included
#define prl_types_meta__included 1
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
        P_const$raw raw;
        P_const$raw inner $like_ref;
    };
    TypeInfo type;
} u_P_const$raw;
typedef union u_P$raw {
    struct {
        union {
            P$raw raw;
            P$raw inner $like_ref;
        };
        TypeInfo type;
    };
    u_P_const$raw as_const;
} u_P$raw;

typedef union u_V$raw {
    struct {
        union {
            V$raw raw;
            P$raw inner;
        };
        union {
            TypeInfo inner_type;
            TypeInfo type;
        };
    };
    u_P$raw ref;
} u_V$raw;

typedef struct u_S_const$raw {
    union {
        struct {
            P_const$raw ptr;
            usize len;
        };
        S_const$raw raw;
        S_const$raw inner $like_ref;
    };
    TypeInfo type;
    struct {
        u_P_const$raw ptr $zero_sized;
    } __type_hint $zero_sized;
} u_S_const$raw;
typedef union u_S$raw {
    struct {
        union {
            struct {
                P$raw ptr;
                usize len;
            };
            S$raw raw;
            S$raw inner $like_ref;
        };
        TypeInfo type;
    };
    u_S_const$raw as_const;
    struct {
        u_P$raw ptr $zero_sized;
    } __type_hint $zero_sized;
} u_S$raw;

typedef union u_A$raw {
    struct {
        S$raw inner;
        union {
            TypeInfo inner_type;
            TypeInfo type;
        };
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
#define __u_from$S__step(...) __VA_ARGS__
#define __u_from$S__parseType(_type...) \
    pp_Tok_if_(Tok_isConst$(_type))( \
        pp_Tok_then_(u_S_const$raw, Tok_removeConst$(_type)), \
        pp_Tok_else_(u_S$raw, _type) \
    ), \
        __u_from$S__parseS
#define __u_from$S__parseS(_s...) _s
#define __u_from$S__emit(...) __u_from$S__emitNext(__VA_ARGS__)
#define __u_from$S__emitNext(_T_u, _type, _s...) lit$((_T_u){ .type = _type, .raw = _s })

#define u_init$S(/*(_type: TypeInfo)(_ptr: P$$(_T), _len: usize)*/... /*(u_S$raw)*/) __u_init$S__step(pp_defer(__u_init$S__emit)(__u_init$S__parseType __VA_ARGS__))
#define __u_init$S__step(...) __VA_ARGS__
#define __u_init$S__parseType(_type...) _type, __u_init$S__parsePtrLen
#define __u_init$S__parsePtrLen(_ptr, _len...) _ptr, _len
#define __u_init$S__emit(_type, _ptr, _len...) \
    pp_Tok_if_(Tok_isConst$(_type))( \
        pp_then_(__u_init$S__emitRef(Tok_removeConst$(_type), _ptr, _len)), \
        pp_else_(__u_init$S__emitRefMut(_type, _ptr, _len)) \
    )
#define __u_init$S__emitRef(_type, _ptr, _len...) lit$((u_S_const$raw){ .type = _type, .ptr = _ptr, .len = _len })
#define __u_init$S__emitRefMut(_type, _ptr, _len...) lit$((u_S$raw){ .type = _type, .ptr = _ptr, .len = _len })

#define len$u_S(_s...) len$S(_s)

#define at$u_S(_s, _idx...) __u_atS(pp_uniqTok(s), pp_uniqTok(idx), _s, _idx)
#define u_atS(_s, _idx...) __u_atS(pp_uniqTok(s), pp_uniqTok(idx), _s, _idx)
#define __u_atS(__s, __idx, _s, _idx...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__idx, usize) = _idx; \
    claim_assert_fmt(__idx < len$S(__s), "Index out of bounds: idx(%zu) >= len(%zu)", __idx, len$S(__s)); \
    lit$((u_TypeHintOf(TypeOf(__s), ptr)){ .type = __s.type, .raw = ptr$S(__s) + (__idx * __s.type.size) }); \
})

#define u_deref(_p...) (*as$(u_V$raw*)(_p.inner))
#define u_load(_v /*: u_V_const$T|u_V$T*/... /*(u_T)*/) ({ \
    const TypeOf(_v) __v = _v; \
    const u_V$raw __b = u_allocV(__v.inner_type); \
    prim_memcpy(__b.inner, __v.inner, __v.inner_type.size); \
    __b; \
})

#define u_sliceP(_p, _range) __u_sliceP(pp_uniqTok(p), pp_uniqTok(range), _p, _range)
#define __u_sliceP(__p, __range, _p, _range...) blk({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__range, R) = _range; \
    claim_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    T_switch$((TypeOf(__p))( \
        T_case$((u_P_const$raw)(lit$((u_S_const$raw){ \
            .type = __p.type, \
            .ptr = as$(const u8*)(__p.raw) + (__range.begin * __p.type.size), \
            .len = len$R(__range), \
        }))), \
        T_case$((u_P$raw)(lit$((u_S$raw){ \
            .type = __p.type, \
            .ptr = as$(u8*)(__p.raw) + (__range.begin * __p.type.size), \
            .len = len$R(__range), \
        }))) \
    )); \
})
#define u_prefixP(_p, _end) __u_prefixP(pp_uniqTok(p), pp_uniqTok(end), _p, _end)
#define __u_prefixP(__p, __end, _p, _end...) blk({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__end, usize) = _end; \
    T_switch$((TypeOf(__p))( \
        T_case$((u_P_const$raw)(lit$((u_S_const$raw){ \
            .type = __p.type, \
            .ptr = as$(const u8*)(__p.raw), \
            .len = __end, \
        }))), \
        T_case$((u_P$raw)(lit$((u_S$raw){ \
            .type = __p.type, \
            .ptr = as$(u8*)(__p.raw), \
            .len = __end, \
        }))) \
    )); \
})
#define u_suffixP(_p, _begin) __u_suffixP(pp_uniqTok(p), pp_uniqTok(begin), _p, _begin)
#define __u_suffixP(__p, __begin, _p, _begin...) blk({ \
    let_(__p, TypeOf(_p)) = _p; \
    let_(__begin, usize) = _begin; \
    T_switch$((TypeOf(__p))( \
        T_case$((u_P_const$raw)(lit$((u_S_const$raw){ \
            .type = __p.type, \
            .ptr = as$(const u8*)(__p.raw) + (__begin * __p.type.size), \
            .len = usize_limit_max - __begin, \
        }))), \
        T_case$((u_P$raw)(lit$((u_S$raw){ \
            .type = __p.type, \
            .ptr = as$(u8*)(__p.raw) + (__begin * __p.type.size), \
            .len = usize_limit_max - __begin, \
        }))) \
    )); \
})

#define u_sliceS(_s, _range) __u_sliceS(pp_uniqTok(s), pp_uniqTok(range), _s, _range)
#define __u_sliceS(__s, __range, _s, _range...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__range, R) = _range; \
    claim_assert_fmt(isValid$R(__range), "Invalid range: begin(%zu) > end(%zu)", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __range.end, len$S(__s)); \
    lit$((TypeOf(__s)){ .type = __s.type, .ptr = ptr$S(__s) + (__range.begin * __s.type.size), .len = len$R(__range) }); \
})
#define u_prefixS(_s, _end) __u_prefixS(pp_uniqTok(s), pp_uniqTok(end), _s, _end)
#define __u_prefixS(__s, __end, _s, _end...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__end, usize) = _end; \
    claim_assert_fmt(__end <= len$S(__s), "Invalid slice range: end(%zu) > len(%zu)", __end, len$S(__s)); \
    lit$((TypeOf(__s)){ .type = __s.type, .ptr = ptr$S(__s), .len = __end }); \
})
#define u_suffixS(_s, _begin) __u_suffixS(pp_uniqTok(s), pp_uniqTok(begin), _s, _begin)
#define __u_suffixS(__s, __begin, _s, _begin...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__begin, usize) = _begin; \
    claim_assert_fmt(__begin <= len$S(__s), "Invalid slice range: begin(%zu) > len(%zu)", __begin, len$S(__s)); \
    lit$((TypeOf(__s)){ .type = __s.type, .ptr = ptr$S(__s) + (__begin * __s.type.size), .len = len$S(__s) - __begin }); \
})

#define u_TypeHintOf(_TBase, _hint_field...) FieldType$(_TBase, __type_hint->_hint_field[0])

$inline_always
$static fn_((u_memcpy(u_P$raw dst, u_P_const$raw src))(u_P$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    claim_assert_fmt(dst.raw != src.raw, "Pointers must not be the same: dst(%p) != src(%p)", dst.raw, src.raw);
    return memcpy(dst.raw, src.raw, src.type.size), dst;
};
$inline_always
$static fn_((u_memmove(u_P$raw dst, u_P_const$raw src))(u_P$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    return memmove(dst.raw, src.raw, src.type.size), dst;
};
$inline_always
$static fn_((u_memset(u_P$raw dst, u_V$raw src))(u_P$raw)) {
    claim_assert_fmt(dst.type.size == src.inner_type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.inner_type.size);
    claim_assert_fmt(dst.type.align == src.inner_type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.inner_type.align);
    claim_assert_fmt(dst.raw != src.inner, "Pointers must not be the same: dst(%p) != src(%p)", dst.raw, src.inner);
    return memcpy(dst.raw, src.inner, src.inner_type.size), dst;
};
$inline_always
$static fn_((u_memeql(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) {
    claim_assert_fmt(lhs.type.size == rhs.type.size, "Type size mismatch: lhs(%zu) != rhs(%zu)", lhs.type.size, rhs.type.size);
    claim_assert_fmt(lhs.type.align == rhs.type.align, "Type alignment mismatch: lhs(%zu) != rhs(%zu)", lhs.type.align, rhs.type.align);
    return memcmp(lhs.raw, rhs.raw, rhs.type.size) == 0;
};
$inline_always
$static fn_((u_memord(u_P_const$raw lhs, u_P_const$raw rhs))(cmp_Ord)) {
    claim_assert_fmt(lhs.type.size == rhs.type.size, "Type size mismatch: lhs(%zu) != rhs(%zu)", lhs.type.size, rhs.type.size);
    claim_assert_fmt(lhs.type.align == rhs.type.align, "Type alignment mismatch: lhs(%zu) != rhs(%zu)", lhs.type.align, rhs.type.align);
    let result = memcmp(lhs.raw, rhs.raw, rhs.type.size);
    return result < 0 ? cmp_Ord_lt : (0 < result ? cmp_Ord_gt : cmp_Ord_eq);
};

$inline_always
$static fn_((u_memcpyS(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    claim_assert_fmt(dst.len == src.len, "Length mismatch: dst(%zu) != src(%zu)", dst.len, src.len);
    claim_assert_fmt(dst.ptr != src.ptr, "Pointers must not be the same: dst(%p) != src(%p)", dst.ptr, src.ptr);
    return memcpy(dst.ptr, src.ptr, src.len * src.type.size), dst;
};
$inline_always
$static fn_((u_memmoveS(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    claim_assert_fmt(dst.len == src.len, "Length mismatch: dst(%zu) != src(%zu)", dst.len, src.len);
    return memmove(dst.ptr, src.ptr, src.len * src.type.size), dst;
};
$inline_always
$static fn_((u_memsetS(u_S$raw dst, u_V$raw src))(u_S$raw)) {
    claim_assert_fmt(dst.type.size == src.inner_type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.inner_type.size);
    claim_assert_fmt(dst.type.align == src.inner_type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.inner_type.align);
    claim_assert_fmt(dst.ptr != src.inner, "Pointers must not be the same: dst(%p) != src(%p)", dst.ptr, src.inner);
    for_(($r(0, dst.len))(i) { u_memcpy(u_atS(dst, i), src.ref.as_const); });
    return dst;
};
$inline_always
$static fn_((u_memeqlS(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) {
    claim_assert_fmt(lhs.type.size == rhs.type.size, "Type size mismatch: lhs(%zu) != rhs(%zu)", lhs.type.size, rhs.type.size);
    claim_assert_fmt(lhs.type.align == rhs.type.align, "Type alignment mismatch: lhs(%zu) != rhs(%zu)", lhs.type.align, rhs.type.align);
    claim_assert_fmt(lhs.len == rhs.len, "Length mismatch: lhs(%zu) != rhs(%zu)", lhs.len, rhs.len);
    return memcmp(lhs.ptr, rhs.ptr, rhs.len * rhs.type.size) == 0;
};
$inline_always
$static fn_((u_memordS(u_S_const$raw lhs, u_S_const$raw rhs))(cmp_Ord)) {
    claim_assert_fmt(lhs.type.size == rhs.type.size, "Type size mismatch: lhs(%zu) != rhs(%zu)", lhs.type.size, rhs.type.size);
    claim_assert_fmt(lhs.type.align == rhs.type.align, "Type alignment mismatch: lhs(%zu) != rhs(%zu)", lhs.type.align, rhs.type.align);
    claim_assert_fmt(lhs.len == rhs.len, "Length mismatch: lhs(%zu) != rhs(%zu)", lhs.len, rhs.len);
    let result = memcmp(lhs.ptr, rhs.ptr, rhs.len * rhs.type.size);
    return (result < 0) ? cmp_Ord_lt : (0 < result) ? cmp_Ord_gt
                                                    : cmp_Ord_eq;
};

typedef fn_(((*)(u_V$raw lhs, u_V$raw rhs))(bool) $T) u_EqlFn;
$inline_always
$static fn_((u_eql(u_V$raw lhs, u_V$raw rhs))(bool)) {
    claim_assert_fmt(lhs.inner_type.size == rhs.inner_type.size, "Type size mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.size, rhs.inner_type.size);
    claim_assert_fmt(lhs.inner_type.align == rhs.inner_type.align, "Type alignment mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.align, rhs.inner_type.align);
    let l = u_allocV(lhs.inner_type);
    u_memcpy(l.ref, lhs.ref.as_const);
    let r = u_allocV(rhs.inner_type);
    u_memcpy(r.ref, rhs.ref.as_const);
    return u_memeql(lhs.ref.as_const, rhs.ref.as_const);
}
$inline_always
$static fn_((u_neq(u_V$raw lhs, u_V$raw rhs))(bool)) { return !u_eql(lhs, rhs); };
$inline_always
$static fn_((u_eqlBy(u_V$raw lhs, u_V$raw rhs, u_EqlFn eqlFn))(bool)) {
    claim_assert_fmt(lhs.inner_type.size == rhs.inner_type.size, "Type size mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.size, rhs.inner_type.size);
    claim_assert_fmt(lhs.inner_type.align == rhs.inner_type.align, "Type alignment mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.align, rhs.inner_type.align);
    let l = u_allocV(lhs.inner_type);
    u_memcpy(l.ref, lhs.ref.as_const);
    let r = u_allocV(rhs.inner_type);
    u_memcpy(r.ref, rhs.ref.as_const);
    return eqlFn(l, r);
};
$inline_always
$static fn_((u_neqBy(u_V$raw lhs, u_V$raw rhs, u_EqlFn eqlFn))(bool)) { return !u_eqlBy(lhs, rhs, eqlFn); };
typedef fn_(((*)(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool) $T) u_EqlCtxFn;
$inline_always
$static fn_((u_eqlCtx(u_V$raw lhs, u_V$raw rhs, u_EqlCtxFn eqlFn, u_V$raw ctx))(bool)) {
    claim_assert_fmt(lhs.inner_type.size == rhs.inner_type.size, "Type size mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.size, rhs.inner_type.size);
    claim_assert_fmt(lhs.inner_type.align == rhs.inner_type.align, "Type alignment mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.align, rhs.inner_type.align);
    let l = u_allocV(lhs.inner_type);
    u_memcpy(l.ref, lhs.ref.as_const);
    let r = u_allocV(rhs.inner_type);
    u_memcpy(r.ref, rhs.ref.as_const);
    let c = u_allocV(ctx.inner_type);
    u_memcpy(c.ref, ctx.ref.as_const);
    return eqlFn(l, r, ctx);
};
$inline_always
$static fn_((u_neqCtx(u_V$raw lhs, u_V$raw rhs, u_EqlCtxFn eqlFn, u_V$raw ctx))(bool)) { return !u_eqlCtx(lhs, rhs, eqlFn, ctx); };

typedef fn_(((*)(u_V$raw lhs, u_V$raw rhs))(cmp_Ord) $T) u_OrdFn;
$inline_always
$static fn_((u_ord(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) {
    claim_assert_fmt(lhs.inner_type.size == rhs.inner_type.size, "Type size mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.size, rhs.inner_type.size);
    claim_assert_fmt(lhs.inner_type.align == rhs.inner_type.align, "Type alignment mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.align, rhs.inner_type.align);
    let l = u_allocV(lhs.inner_type);
    u_memcpy(l.ref, lhs.ref.as_const);
    let r = u_allocV(rhs.inner_type);
    u_memcpy(r.ref, rhs.ref.as_const);
    return u_memord(l.ref.as_const, r.ref.as_const);
};
$inline_always
$static fn_((u_ordBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(cmp_Ord)) {
    claim_assert_fmt(lhs.inner_type.size == rhs.inner_type.size, "Type size mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.size, rhs.inner_type.size);
    claim_assert_fmt(lhs.inner_type.align == rhs.inner_type.align, "Type alignment mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.align, rhs.inner_type.align);
    let l = u_allocV(lhs.inner_type);
    u_memcpy(l.ref, lhs.ref.as_const);
    let r = u_allocV(rhs.inner_type);
    u_memcpy(r.ref, rhs.ref.as_const);
    return ordFn(l, r);
};
typedef fn_(((*)(u_V$raw lhs, u_V$raw rhs, u_V$raw context))(cmp_Ord) $T) u_OrdCtxFn;
$inline_always
$static fn_((u_ordCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(cmp_Ord)) {
    claim_assert_fmt(lhs.inner_type.size == rhs.inner_type.size, "Type size mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.size, rhs.inner_type.size);
    claim_assert_fmt(lhs.inner_type.align == rhs.inner_type.align, "Type alignment mismatch: lhs(%zu) != rhs(%zu)", lhs.inner_type.align, rhs.inner_type.align);
    let l = u_allocV(lhs.inner_type);
    u_memcpy(l.ref, lhs.ref.as_const);
    let r = u_allocV(rhs.inner_type);
    u_memcpy(r.ref, rhs.ref.as_const);
    let c = u_allocV(ctx.inner_type);
    u_memcpy(c.ref, ctx.ref.as_const);
    return ordFn(l, r, ctx);
};

$inline_always
$static fn_((u_eq(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) == cmp_Ord_eq; };
$inline_always
$static fn_((u_ne(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) != cmp_Ord_eq; };
$inline_always
$static fn_((u_lt(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) == cmp_Ord_lt; };
$inline_always
$static fn_((u_gt(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) == cmp_Ord_gt; };
$inline_always
$static fn_((u_le(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) <= cmp_Ord_eq; };
$inline_always
$static fn_((u_ge(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) >= cmp_Ord_eq; };
$inline_always
$static fn_((u_eqBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) == cmp_Ord_eq; };
$inline_always
$static fn_((u_neBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) != cmp_Ord_eq; };
$inline_always
$static fn_((u_ltBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) == cmp_Ord_lt; };
$inline_always
$static fn_((u_gtBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) == cmp_Ord_gt; };
$inline_always
$static fn_((u_leBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) <= cmp_Ord_eq; };
$inline_always
$static fn_((u_geBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) >= cmp_Ord_eq; };
$inline_always
$static fn_((u_eqCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) == cmp_Ord_eq; };
$inline_always
$static fn_((u_neCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) != cmp_Ord_eq; };
$inline_always
$static fn_((u_ltCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) == cmp_Ord_lt; };
$inline_always
$static fn_((u_gtCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) == cmp_Ord_gt; };
$inline_always
$static fn_((u_leCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) <= cmp_Ord_eq; };
$inline_always
$static fn_((u_geCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) >= cmp_Ord_eq; };

// ============================================================================
// Conversion Patterns - The Core Innovation
// ============================================================================

#define u_ret$ u_retV$
#define u_retV$(_T) ((u_V$raw){ .inner_type = typeInfo$(_T), .inner = &lit0$((_T)) })
#define u_retA$(_N, _T) ((u_A$raw){ .inner_type = typeInfo$(FieldType$(A$$(_N, _T), val[0])), .inner = ref$A((A$$(_N, _T)){}) })
#define u_retO$(_T) ((u_O$raw){ .inner_type = typeInfo$(FieldType$(O$$(_T), payload->some)), .inner = ((O$$(_T)){}).ref_raw })
#define u_retE$(_T) ((u_E$raw){ .inner_type = typeInfo$(FieldType$(E$$(_T), payload->ok)), .inner = ((E$$(_T)){}).ref_raw })

// #define u_anyP_const(_p...) ((u_P_const$raw){ .type = typeInfo$(TypeOf(*_p)), .raw = _p })
// #define u_anyP(_p...) ((u_P$raw){ .type = typeInfo$(TypeOf(*_p)), .raw = _p })
#define u_anyP(_p...) _Generic( \
    TypeOf(*_p), \
    const TypeOfUnqual(*_p): ((u_P_const$raw){ .raw = (P_const$raw)(_p), .type = typeInfo$(TypeOf(*_p)) }), \
    TypeOfUnqual(*_p): ((u_P$raw){ .raw = (P$raw)(_p), .type = typeInfo$(TypeOf(*_p)) }) \
)
// #define u_anyS_const(_s...) ((u_S_const$raw){ .type = typeInfo$(TypeOf(*_s.ptr)), .raw = _s.as_raw })
// #define u_anyS(_s...)       ((u_S$raw){ .type = typeInfo$(TypeOf(*_s.ptr)), .raw = _s.as_raw })
#define u_anyS(_s...) _Generic( \
    TypeOf(&*_s.ptr), \
    const TypeOfUnqual(*_s.ptr)*: ((u_S_const$raw){ .raw = *(S_const$raw*)(_s.ref_raw), .type = typeInfo$(TypeOf(*_s.ptr)) }), \
    TypeOfUnqual(*_s.ptr)*: ((u_S$raw){ .raw = *(S$raw*)(_s.ref_raw), .type = typeInfo$(TypeOf(*_s.ptr)) }) \
)

#define u_anyV(_v...) ({ \
    let_(__p_v, TypeOfUnqual(_v)*) = &copy(_v); \
    lit$((u_V$raw){ .inner_type = typeInfo$(TypeOf(*__p_v)), .inner = __p_v }); \
})
#define u_anyA(_a...) ({ \
    let_(__p_a, TypeOfUnqual(_a)*) = &copy(_a); \
    lit$((u_A$raw){ .inner_type = typeInfo$(TypeOf(*__p_a->val)), .inner = ref$A(*__p_a).as_raw }); \
})
#define u_anyO(_o...) ({ \
    let_(__p_o, TypeOfUnqual(_o)*) = &copy(_o); \
    __p_o->is_some \
        ? (u_O$raw)some(u_anyV(__p_o->payload.some)) \
        : (u_O$raw)none(); \
})
#define u_anyE(_e...) ({ \
    let_(__p_e, TypeOfUnqual(_e)*) = &copy(_e); \
    __p_e->is_ok \
        ? (u_E$raw)ok(u_anyV(__p_e->payload.ok)) \
        : (u_E$raw)err(__p_e->payload.err); \
})

#define u_cast u_castV$

#define u_castP$(/*(_T)(_Expr...)*/... /*(P_const$(_T))*/) \
    __step_inline__uCast$P$(pp_defer(__emit_inline__uCast$P$)(__param_parse__uCast$P$ __VA_ARGS__))
#define __step_inline__uCast$P$(...) __VA_ARGS__
#define __param_parse__uCast$P$(...) __VA_ARGS__, __param_next__uCast$P$
#define __param_next__uCast$P$(...) __VA_ARGS__
#if UNUSED_CODE
#define __emit_inline__uCast$P$(_PT, _meta...) (*as$(_PT*)(_meta.inner))
#endif /* UNUSED_CODE */
#define __emit_inline__uCast$P$(_PT, _meta...) ({ \
    typedef _PT CastType; \
    $maybe_unused typedef DerefType$(CastType) DerefType; \
    $maybe_unused typedef DerefType InnerType; \
    claim_assert(T_switch$((TypeOf(_meta.raw))( \
        T_qual$((P_const$raw)(P_isConst$(CastType))), \
        T_default_(true) \
    ))); \
    *as$(CastType*)(_meta.inner); \
})

#define u_castV$(/*(_T)(_Expr...)*/... /*(_T)*/) \
    __step_inline__uCast$V$(pp_defer(__emit_inline__uCast$V$)(__param_parse__uCast$V$ __VA_ARGS__))
#define __step_inline__uCast$V$(...) __VA_ARGS__
#define __param_parse__uCast$V$(...) __VA_ARGS__, __param_next__uCast$V$
#define __param_next__uCast$V$(...) __VA_ARGS__
#define __emit_inline__uCast$V$(_T, _meta...) (*as$(_T*)(_meta.inner))

#define u_castS$(/*(_T)(_Expr...)*/... /*(S_const$(_T))*/) \
    __step_inline__uCast$S$(pp_defer(__emit_inline__uCast$S$)(__param_parse__uCast$S$ __VA_ARGS__))
#define __step_inline__uCast$S$(...) __VA_ARGS__
#define __param_parse__uCast$S$(...) __VA_ARGS__, __param_next__uCast$S$
#define __param_next__uCast$S$(...) __VA_ARGS__
#if UNUSED_CODE
#define __emit_inline__uCast$S$(_ST, _meta...) (*as$(_ST*)(_meta.inner))
#endif /* UNUSED_CODE */
#define __emit_inline__uCast$S$(_ST, _meta...) ({ \
    typedef _ST CastType; \
    $maybe_unused typedef FieldType$(CastType, ptr) PtrType; \
    $maybe_unused typedef DerefType$(PtrType) DerefType; \
    $maybe_unused typedef DerefType InnerType; \
    claim_assert(T_switch$((TypeOf(_meta.raw.ptr))( \
        T_qual$((P_const$raw)(P_isConst$(PtrType))), \
        T_default_(true) \
    ))); \
    *as$(CastType*)(_meta.inner); \
})

#define u_castA$(/*(_N,_T)(_Expr...)*/... /*(A$(_N,_T))*/) \
    __step_inline__uCast$A$(pp_defer(__emit_inline__uCast$A$)(__param_parse__uCast$A$ __VA_ARGS__))
#define __step_inline__uCast$A$(...) __VA_ARGS__
#define __param_parse__uCast$A$(...) __VA_ARGS__, __param_next__uCast$A$
#define __param_next__uCast$A$(...) __VA_ARGS__
#define __emit_inline__uCast$A$(_ANT, _meta...) (*as$(_ANT*)(_meta.inner))

#define u_castO$(/*(_OT)(_Expr...)*/... /*(_OT)*/) \
    __step__u_castO$__expand(pp_defer(__step__u_castO$)(__step__u_castO$__parseOT __VA_ARGS__))
#define __step__u_castO$__expand(...) __VA_ARGS__
#define __step__u_castO$__parseOT(_OT...) pp_uniqTok(meta), _OT, __step__u_castO$__parseExpr
#define __step__u_castO$__parseExpr(_Expr...) _Expr
#define __step__u_castO$(__meta, _OT, _Expr...) ({ \
    typedef _OT O$Ret$u_castO$; \
    $maybe_unused typedef O$Ret$u_castO$ CastType; \
    $maybe_unused typedef FieldType$(CastType, payload.some) SomeType; \
    $maybe_unused typedef SomeType InnerType; \
    let_(__meta, TypeOf(_Expr)) = _Expr; \
    __meta.is_some \
        ? lit$((O$Ret$u_castO$)some(*as$(FieldType$(O$Ret$u_castO$, payload.some)*)(__meta.payload.some.inner))) \
        : lit$((O$Ret$u_castO$)none()); \
})

#define u_castE$(/*(_ET)(_Expr...)*/... /*(_ET)*/) \
    __step__u_castE$__expand(pp_defer(__step__u_castE$)(__step__u_castE$__parseET __VA_ARGS__))
#define __step__u_castE$__expand(...) __VA_ARGS__
#define __step__u_castE$__parseET(_ET...) pp_uniqTok(meta), _ET, __step__u_castE$__parseExpr
#define __step__u_castE$__parseExpr(_Expr...) _Expr
#define __step__u_castE$(__meta, _ET, _Expr...) ({ \
    typedef _ET E$Ret$u_castE$; \
    $maybe_unused typedef E$Ret$u_castE$ CastType; \
    $maybe_unused typedef FieldType$(CastType, payload.err) ErrType; \
    $maybe_unused typedef FieldType$(CastType, payload.ok) OkType; \
    $maybe_unused typedef OkType InnerType; \
    let_(__meta, TypeOf(_Expr)) = _Expr; \
    __meta.is_ok \
        ? lit$((E$Ret$u_castE$)ok(*as$(FieldType$(E$Ret$u_castE$, payload.ok)*)(__meta.payload.ok.inner))) \
        : lit$((E$Ret$u_castE$)err(__meta.payload.err)); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_types_meta__included */
