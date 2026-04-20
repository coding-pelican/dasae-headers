#include "dh/prl.h"

T_alias$((P_const$raw)(const void*));
T_alias$((P$raw)(void*));

T_alias$((P_const$T)(P_const$raw));
T_alias$((P$T)(P$raw));
T_alias$((S_const$T)(S_const$raw));
T_alias$((S$T)(S$raw));

T_alias$((u_Inner)(TypeOf(u8 $flexible)));
T_use_prl$(u_Inner);

/*========== Macros and Declarations ========================================*/

/* --- Literal Value --- */
#define u_T$$(_T$raw...) __type__u_T$$(_T$raw)
#define u_T$(_T$raw...) __alias__u_T$(_T$raw)
#define T_decl_u_T$(_T$raw...) __stmt__T_decl_u_T$(_T$raw)
#define T_impl_u_T$(_T$raw...) __stmt__T_impl_u_T$(_T$raw)
#define T_use_u_T$(_T$raw...) __stmt__T_use_u_T$(_T$raw)
T_alias$((u_T)(struct u_T {
    var_(inner, P$u_Inner);
    var_(type, TypeInfo);
}));

/* --- Literal Array --- */
#define u_NT$$(_NT$raw...) __type__u_NT$$(_NT$raw)
#define u_NT$(_NT$raw...) __alias__u_NT$(_NT$raw)
#define T_decl_u_NT$(_NT$raw...) __stmt__T_decl_u_NT$(_NT$raw)
#define T_impl_u_NT$(_NT$raw...) __stmt__T_impl_u_NT$(_NT$raw)
#define T_use_u_NT$(_NT$raw...) __stmt__T_use_u_NT$(_NT$raw)
T_alias$((u_NT)(struct u_NT {
    var_(inner, S$u_Inner);
    var_(type, TypeInfo);
}));

/* --- Reference Value --- */
T_alias$((u_V_const$T)(struct u_V_const$T {
    var_(inner, P_const$u_Inner);
    var_(type, TypeInfo);
}));
T_alias$((u_V_mut$T)(struct u_V_mut$T {
    var_(inner, P$u_Inner);
    var_(type, TypeInfo);
}));
T_alias$((u_V$T)(union u_V$T {
    T_embed$(u_V_mut$T);
    var_(as_const, u_V_const$T);
}));

/* --- Reference Array --- */
T_alias$((u_A_const$T)(struct u_A_const$T {
    var_(inner, S_const$u_Inner);
    var_(type, TypeInfo);
}));
T_alias$((u_A_mut$T)(struct u_A_mut$T {
    var_(inner, S$u_Inner);
    var_(type, TypeInfo);
}));
T_alias$((u_A$T)(union u_A$T {
    T_embed$(u_A_mut$T);
    var_(as_const, u_A_const$T);
}));

/* --- Pointer --- */
T_alias$((u_P_const$T)(struct u_P_const$T {
    T_embed$(union {
        var_(inner, P_const$u_Inner) $like_ref;
        var_(raw, P_const$raw);
    });
    var_(type, TypeInfo);
}));
T_alias$((u_P_mut$T)(struct u_P_mut$T {
    T_embed$(union {
        var_(inner, P$u_Inner) $like_ref;
        var_(raw, P$raw);
    });
    var_(type, TypeInfo);
}));
T_alias$((u_P$T)(union u_P$T {
    T_embed$(u_P_mut$T);
    var_(as_const, u_P_const$T);
}));

/* --- Slice --- */
T_alias$((u_S_const$T)(struct u_S_const$T {
    T_embed$(union {
        var_(inner, S_const$u_Inner) $like_ref;
        var_(raw, S_const$raw);
        struct {
            var_(ptr, P_const$raw);
            var_(len, usize);
        };
    });
    var_(type, TypeInfo);
}));
T_alias$((u_S_mut$T)(struct u_S_mut$T {
    T_embed$(union {
        var_(inner, S$u_Inner) $like_ref;
        var_(raw, S$raw);
        struct {
            var_(ptr, P$raw);
            var_(len, usize);
        };
    });
    var_(type, TypeInfo);
}));
T_alias$((u_S$T)(union u_S$T {
    T_embed$(u_S_mut$T);
    var_(as_const, u_S_const$T);
}));

/*========== Macros and Definitions =========================================*/

#define __type__u_T$$(_T$raw...) \
    union { \
        T_embed$(struct { \
            var_(inner, P$$(_T$raw)); \
            var_(type, TypeInfo); \
        }); \
        var_(as_raw, u_T) $like_ref; \
    }
#define __alias__u_T$(_T$raw...) tpl_id$T(u_T, _T$raw)
#define __stmt__T_decl_u_T$(_T$raw...) \
    T_alias$((u_T$(_T$raw))(union u_T$(_T$raw)))
#define __stmt__T_impl_u_T$(_T$raw...) \
    union u_T$(_T$raw) { \
        T_embed$(struct { \
            var_(inner, P$(_T$raw)); \
            var_(type, TypeInfo); \
        }); \
        var_(as_raw, u_T) $like_ref; \
    }
#define __stmt__T_use_u_T$(_T$raw...) \
    T_decl_u_T$(_T$raw); \
    T_impl_u_T$(_T$raw)

#define __type__u_NT$$(_NT$raw...) \
    union { \
        T_embed$(struct { \
            var_(inner, S$$(_NT$raw)); \
            var_(type, TypeInfo); \
        }); \
        var_(as_raw, u_NT) $like_ref; \
    }
#define __alias__u_NT$(_NT$raw...) tpl_id$T(u_NT, _NT$raw)
#define __stmt__T_decl_u_NT$(_NT$raw...) \
    T_alias$((u_NT$(_NT$raw))(union u_NT$(_NT$raw)))
#define __stmt__T_impl_u_NT$(_NT$raw...) \
    union u_NT$(_NT$raw) { \
        T_embed$(struct { \
            var_(inner, S$(_NT$raw)); \
            var_(type, TypeInfo); \
        }); \
    }
#define __stmt__T_use_u_NT$(_NT$raw...) \
    T_decl_u_NT$(_NT$raw); \
    T_impl_u_NT$(_NT$raw)

#undef u_allocT
#define u_allocT(_type...) __expr__u_allocT(_type)
#define __expr__u_allocT(_type...) ____u_allocT(pp_uniqTok(inner), pp_uniqTok(type), _type)
#define ____u_allocT(__inner, __type, _type...) local_({ \
    let_(__type, TypeInfo) = _type; \
    let_(__inner, P$u_Inner) = ptrCast$((P$u_Inner)(raw_alloca(TypeInfo_size(__type)))); \
    local_return_(l$((u_T){ \
        .inner = raw_memset0(__inner, TypeInfo_size(__type)), \
        .type = __type, \
    })); \
})
#undef u_allocNT
#define u_allocNT(_len, _type...) __expr__u_allocNT(_len, _type)
#define __expr__u_allocNT(_len, _type...) ____u_allocNT(pp_uniqTok(inner), pp_uniqTok(len), _len, pp_uniqTok(type), _type)
#define ____u_allocNT(__inner, __len, _len, __type, _type...) local_({ \
    let_(__type, TypeInfo) = _type; \
    let_(__len, usize) = _len; \
    let_(__inner, P$u_Inner) = ptrCast$((P$u_Inner)(raw_alloca(__len * TypeInfo_size(__type)))); \
    local_return_(l$((u_NT){ \
        .inner = { \
            .ptr = raw_memset0(__inner, __len * TypeInfo_size(__type)), \
            .len = __len, \
        }, \
        .type = __type, \
    })); \
})

#undef V_meta
#undef ____V_meta
#undef P_meta
#undef ____P_meta
#undef S_meta
#undef ____S_meta

#undef u_refT
#undef u_derefT
#undef u_storeT
#undef u_loadT

#undef u_V_store
#undef u_V_load

#undef at$u_S
#define at$u_S u_S_at



#define P_raw(_p /*: u_P(T)*/... /*(P(raw))*/) (_p.raw)
#define P_meta(/*(_type: TypeInfo)(_raw: P(raw))*/... /*(u_P(T))*/) \
    __step__P_meta(__step__P_meta__parse __VA_ARGS__)
#define __step__P_meta__parse(_type...) _type,
#define __step__P_meta(...) ____P_meta(__VA_ARGS__)
#define ____P_meta(_type, _raw...) T_switch$((TypeOf(_raw))( \
    T_case$((P_const$raw)(l$((u_P_const$T){ .raw = ptrQualCast$((P_const$u_Inner)(_raw)), .type = _type }))), \
    T_case$((P$raw)(l$((u_P$T){ .raw = ptrQualCast$((P$u_Inner)(_raw)), .type = _type }))) \
))

#define S_raw(_s /*: u_S(T)*/... /*(S(raw))*/) (_s.raw)
#define S_meta(/*(_type: TypeInfo)(_raw: S(raw))*/... /*(u_S(T))*/) \
    __step__S_meta(__step__S_meta__parse __VA_ARGS__)
#define __step__S_meta__parse(_type...) _type,
#define __step__S_meta(...) ____S_meta(__VA_ARGS__)
#define ____S_meta(_type, _raw...) T_switch$((TypeOf(_raw))( \
    T_case$((S_const$raw)(l$((u_S_const$T){ .raw = *ptrQualCast$((S_const$u_Inner*)(&copy(_raw))), .type = _type }))), \
    T_case$((S$raw)(l$((u_S$T){ .raw = *ptrQualCast$((S$u_Inner*)(&copy(_raw))), .type = _type }))) \
))


#define u_refT(_b /*: u_T|u_V(T)*/... /*(u_P(T))*/) T_switch$((TypeOf(_b))( \
    T_case$((u_T)(*ptrQualCast$((u_P$T*)(&(_b).inner)))), \
    T_case$((u_V_const$T)(*ptrQualCast$((u_P_const$T*)(&(_b).inner)))), \
    T_case$((u_V$T)(*ptrQualCast$((u_P$T*)(&(_b).inner)))) \
))
#define u_derefT(_p /*: u_P(T)*/... /*(u_V(T))*/) T_switch$((TypeOf(&*(_p).raw))( \
    T_case$((P_const$T)(*ptrQualCast$((u_V_const$T*)((_p).inner)))), \
    T_case$((P$T)(*ptrQualCast$((u_V$T*)((_p).inner)))) \
))

#define u_V_store(_v /*: u_V(T)*/, _b /*: u_T */... /*(u_V(T))*/) \
    u_derefT(u_storeT(u_refT(_v), _b))
#define u_V_load(_v /*: u_V(T)*/... /*(u_T)*/) local_({ \
    let_(__v, TypeOf(_v)) = _v; \
    let_(__b, u_T) = u_allocT(__v.type); \
    raw_memcpy(__b.inner, __v.inner, TypeInfo_size(__v.type)); \
    local_return_(__b); \
})

#define u_storeT(_p /*: u_P(T) */, _b /*: u_T */... /*(u_P(T))*/) local_({ \
    let_(__p, u_P$T) = _p; \
    let_(__b, u_T) = _b; \
    claim_assert(TypeInfo_eql(__p.type, __b.type)); \
    raw_memcpy(*__p.inner, __b.inner, TypeInfo_size(__b.type)); \
    local_return_(__p); \
})
#define u_loadT(_p /*: u_P(T)*/... /*(u_T)*/) u_V_load(u_derefT(_p))


#define u_refNT(_b /*: u_NT|u_A(T)*/... /*(u_S(T)*/) T_switch$((TypeOf(_b))( \
    T_case$((u_NT)(*ptrQualCast$((u_S$T*)(&(_b).inner)))), \
    T_case$((u_A_const$T)(*ptrQualCast$((u_S_const$T*)(&(_b).inner)))), \
    T_case$((u_A$T)(*ptrQualCast$((u_S$T*)(&(_b).inner)))) \
))
#define u_derefNT(_p /*: u_S(T)*/... /*(u_A(T))*/) T_switch$((TypeOf(&*(_p).raw))( \
    T_case$((S_const$T)(*ptrQualCast$((u_A_const$T*)((_p).inner)))), \
    T_case$((S$T)(*ptrQualCast$((u_A$T*)((_p).inner)))) \
))

#define u_A_store(_a /*: u_A(T)*/, _b /*: u_NT */... /*(u_A(T))*/) \
    u_derefNT(u_storeNT(u_refNT(_a), _b))
#define u_A_load(_a /*: u_A(T)*/... /*(u_NT)*/) ({ \
    let_(__a, TypeOf(_a)) = _a; \
    let_(__b, u_NT) = u_allocNT(__a.len, __a.type); \
    raw_memcpy(__b.inner.ptr, __a.inner.ptr, __a.len * TypeInfo_size(__a.type)); \
    local_return_(__b); \
})

#define u_storeNT(_s /*: u_S(T) */, _b /*: u_NT */... /*(u_S(T))*/) ({ \
    let_(__s, u_S$T) = _s; \
    let_(__b, u_NT) = _b; \
    claim_assert((*__s.inner).len == __b.inner.len); \
    claim_assert(TypeInfo_eql(__s.type, __b.type)); \
    raw_memcpy((*__s.inner).ptr, __b.inner.ptr, __b.inner.len * TypeInfo_size(__b.type)); \
    local_return_(__s); \
})
#define u_loadNT(_s /*: u_S(T)*/... /*(u_NT)*/) u_A_load(u_derefNT(_s))



#define u_copyT(_src /*: u_T|u_V(T)*/... /*(u_T)*/) local_({ \
    let_(__v, TypeOf(_src)) = _src; \
    let_(__b, u_T) = u_allocT(__v.type); \
    raw_memcpy(__b.inner, __v.inner, TypeInfo_size(__v.type)); \
    local_return_(__b); \
})
#define u_moveT(_src /*: u_T|u_V(T)*/... /*(u_T)*/) local_({ \
    let_(__v, TypeOf(_src)) = _src; \
    let_(__b, u_T) = u_allocT(__v.type); \
    raw_memcpy(__b.inner, __v.inner, TypeInfo_size(__v.type)); \
    raw_memset0(__v.inner, TypeInfo_size(__v.type)); \
    local_return_(__b); \
})

#define u_copyNT(_src /*: u_NT|u_A(T)*/... /*(u_NT)*/) local_({ \
    let_(__a, TypeOf(_src)) = _src; \
    let_(__b, u_NT) = u_allocNT(__a.len, __a.type); \
    raw_memcpy(__b.inner.ptr, __a.inner.ptr, __a.len * TypeInfo_size(__a.type)); \
    local_return_(__b); \
})
#define u_moveNT(_src /*: u_NT|u_A(T)*/... /*(u_NT)*/) local_({ \
    let_(__a, TypeOf(_src)) = _src; \
    let_(__b, u_NT) = u_allocNT(__a.len, __a.type); \
    raw_memcpy(__b.inner.ptr, __a.inner.ptr, __a.len * TypeInfo_size(__a.type)); \
    raw_memset0(__a.inner.ptr, __a.len * TypeInfo_size(__a.type)); \
    local_return_(__b); \
})


#define u_stride_static(_type...) ____u_stride_static(_type)
#define ____u_stride_static(_type...) \
    (((_type).size + ((1ull << (_type).log2_align) - 1)) & ~((1ull << (_type).log2_align) - 1))
#define u_stride(_type...) __step__u_stride(_type)
#define __step__u_stride(_type...) ____u_stride(pp_uniqTok(type), _type)
#define ____u_stride(__type, _type...) ({ \
    let_(__type, usize) = _type; \
    u_stride_static(__type); \
})
#define u_S_at(_s, _idx...) __u_S_at(pp_uniqTok(s), pp_uniqTok(idx), pp_uniqTok(stride), _s, _idx)
#define __u_S_at(__s, __idx, __stride, _s, _idx...) $supress_cast_qual(({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__idx, usize) = _idx; \
    claim_assert_fmt(__idx < u_lenS(__s), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, u_lenS(__s)); \
    let_(__stride, usize) = u_stride_static(__s.type); \
    T_switch$((TypeOf(__s))( \
        T_case$((u_S_const$T)(l$((u_P_const$T){ \
            .raw = as$(const u8*)(__s.ptr) + (__idx * __stride), \
            .type = __s.type, \
        }))), \
        T_case$((u_S$T)(l$((u_P$T){ \
            .raw = as$(u8*)(__s.ptr) + (__idx * __stride), \
            .type = __s.type, \
        }))) \
    )); \
}))


#if UNUSED_CODE
fn_((old_cmp_lessFn(u_V$T lhs, u_V$T rhs))(cmp_Ord)) {
    let l = u_castV$((i32)(lhs));
    let r = u_castV$((i32)(rhs));
    return pri_ord(&l, &r);
}
typedef TypeOf(old_cmp_lessFn) old_cmp_LessFn;

fn_((max(u_S_const$raw items, old_cmp_LessFn cmpFn, u_V$T ret))(u_V$T)) {
    var best = u_atS(items, 0);
    for_(($us(items))(item) {
        if (cmpFn(u_castV$((u_V$T)(item)), u_castV$((u_V$T)(best))) == cmp_Ord_gt) {
            best = item;
        }
    });
    return u_memcpy(ret.ref, best), ret;
}

fn_((max$i32(S_const$i32 items, old_cmp_LessFn cmpFn))(i32) $scope) {
    return u_castV$((ReturnType)(max(u_anyS(items), cmpFn, u_retV$(ReturnType))));
} $unscoped(fn);
#endif /* UNUSED_CODE */

#define u_castV$(/*(_T)(_Expr...)*/... /*(_T)*/) \
    __step_inline__u_castV$(pp_defer(__emit_inline__u_castV$)(__param_parse__u_castV$ __VA_ARGS__))
#define __step_inline__u_castV$(...) __VA_ARGS__
#define __param_parse__u_castV$(...) __VA_ARGS__,
#undef __emit_inline__u_castV$
#define __emit_inline__u_castV$(_T, _meta...) (*ptrAlignCast$((_T*)(_meta.inner)))

#define u_castA$(/*(_T)(_Expr...)*/... /*(_T)*/) \
    __step_inline__u_castA$(pp_defer(__emit_inline__u_castA$)(__param_parse__u_castA$ __VA_ARGS__))
#define __step_inline__u_castA$(...) __VA_ARGS__
#define __param_parse__u_castA$(...) __VA_ARGS__,
#undef __emit_inline__u_castA$
#define __emit_inline__u_castA$(_T, _meta...) S_deref$((_T)(_meta.inner.as_raw))

$attr($maybe_unused)
$static fn_((cmp_lessFn(u_T lhs, u_T rhs))(cmp_Ord)) {
    let l = u_castV$((i32)(lhs));
    let r = u_castV$((i32)(rhs));
    return pri_ord(l, r);
};
T_alias$((cmp_LessFn)(TypeOf(cmp_lessFn)));

$attr($maybe_unused)
$static fn_((min(u_S_const$T items, cmp_LessFn cmpFn, u_V$T ret))(u_V$T)) {
    var best = u_loadT(u_S_at(items, 0));
    for_(($us(items))(item) {
        if (cmpFn(u_copyT(best), u_loadT(item)) == cmp_Ord_lt) {
            u_storeT(u_refT(best), u_loadT(item));
        }
    });
    return u_V_store(ret, best);
};
