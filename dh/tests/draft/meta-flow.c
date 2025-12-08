#include "dh/prl.h"

typedef TypeOf(const void*) P_const$raw;
typedef TypeOf(void*) P$raw;

typedef P_const$raw P_const$T;
typedef P$raw P$T;
typedef S_const$raw S_const$T;
typedef S$raw S$T;

typedef TypeOf(u8[0]) u_Inner;
T_use_S$(u_Inner);

#define u_T$$(_T$raw...) __comp_anon__u_T$$(_T$raw)
#define u_T$(_T$raw...) __comp_alias__u_T$(_T$raw)
#define T_decl_u_T$(_T$raw...) __comp_gen__T_decl_u_T$(_T$raw)
#define T_impl_u_T$(_T$raw...) __comp_gen__T_impl_u_T$(_T$raw)
#define T_use_u_T$(_T$raw...) __comp_gen__T_use_u_T$(_T$raw)

typedef struct u_T {
    var_(inner, u_Inner*);
    var_(type, TypeInfo);
} u_T;

#define __comp_anon__u_T$$(_T$raw...) \
    union { \
        struct { \
            var_(inner, _T$raw*); \
            var_(type, TypeInfo); \
        }; \
        var_(as_raw, u_T) $like_ref; \
    }
#define __comp_alias__u_T$(_T$raw...) tpl_id$T(u_T, _T$raw)
#define __comp_gen__T_decl_u_T$(_T$raw...) \
    typedef union u_T$(_T$raw) u_T$(_T$raw)
#define __comp_gen__T_impl_u_T$(_T$raw...) \
    union u_T$(_T$raw) { \
        struct { \
            var_(inner, _T$raw*); \
            var_(type, TypeInfo); \
        }; \
        var_(as_raw, u_T) $like_ref; \
    }
#define __comp_gen__T_use_u_T$(_T$raw...) \
    T_decl_u_T$(_T$raw); \
    T_impl_u_T$(_T$raw)

typedef struct u_V_const$T {
    var_(inner, const u_Inner*);
    var_(type, TypeInfo);
} u_V_const$T;
typedef union u_V$T {
    struct {
        var_(inner, u_Inner*);
        var_(type, TypeInfo);
    };
    var_(as_const, u_V_const$T);
} u_V$T;

typedef struct u_P_const$T {
    union {
        var_(inner, const u_Inner*) $like_ref;
        var_(raw, P_const$T);
    };
    var_(type, TypeInfo);
} u_P_const$T;
typedef union u_P$T {
    struct {
        union {
            var_(inner, u_Inner*) $like_ref;
            var_(raw, P$T);
        };
        var_(type, TypeInfo);
    };
    var_(as_const, u_P_const$T);
} u_P$T;

typedef struct u_S_const$T {
    union {
        var_(inner, S_const$u_Inner) $like_ref;
        var_(raw, S_const$T);
        struct {
            var_(ptr, P_const$T);
            var_(len, usize);
        };
    };
    var_(type, TypeInfo);
} u_S_const$T;
typedef union u_S$T {
    struct {
        union {
            var_(inner, S$u_Inner) $like_ref;
            var_(raw, S$T);
            struct {
                var_(ptr, P$T);
                var_(len, usize);
            };
        };
        var_(type, TypeInfo);
    };
    var_(as_const, u_S_const$T);
} u_S$T;

#undef u_allocV
#define u_allocV(_type...) ({ \
    const TypeInfo __type = _type; \
    const P$T __inner = alloca(__type.size); \
    memset(__inner, 0, __type.size); \
    lit$((u_T){ .inner = __inner, .type = __type }); \
})

#define V_meta(/*(_type: TypeInfo)(_raw: u_Inner*)*/... /*(u_V_const$T|u_V$T)*/) \
    __step__V_meta(__step__V_meta__parse __VA_ARGS__)
#define __step__V_meta__parse(_type...) _type,
#define __step__V_meta(...) ____V_meta(__VA_ARGS__)
#define ____V_meta(_type, _raw...) /* TODO: Implement */
#define P_meta(/*(_type: TypeInfo)(_raw: P_const$T|P$T)*/... /*(u_P_const$T|u_P$T)*/) \
    __step__P_meta(__step__P_meta__parse __VA_ARGS__)
#define __step__P_meta__parse(_type...) _type,
#define __step__P_meta(...) ____P_meta(__VA_ARGS__)
#define ____P_meta(_type, _raw...) /* TODO: Implement */
#define S_meta(/*(_type: TypeInfo)(_raw: S_const$T|S$T)*/... /*(u_S_const$T|u_S$T)*/) \
    __step__S_meta(__step__S_meta__parse __VA_ARGS__)
#define __step__S_meta__parse(_type...) _type,
#define __step__S_meta(...) ____S_meta(__VA_ARGS__)
#define ____S_meta(_type, _raw...) /* TODO: Implement */

#define V_raw(_v /*: u_V_const$T|u_V$T*/... /*(u_Inner*)*/) (_v.inner)
#define P_raw(_p /*: u_P_const$T|u_P$T*/... /*(P_const$T|P$T)*/) (_p.raw)
#define S_raw(_s /*: u_S_const$T|u_S$T*/... /*(S_const$T|S$T)*/) (_s.raw)

#define u_ref(_b /*: u_T|u_V_const$T|u_V$T*/... /*(u_P_const$T|u_P$T)*/) ({ \
    let_(__b, TypeOf(_b)) = _b; \
    T_switch$((TypeOf(_b))( \
        T_qual$((const u_T)((u_P_const$T){ .inner[0] = __b.inner, .type = __b.type })), \
        T_qual$((u_T)((u_P$T){ .inner[0] = __b.inner, .type = __b.type })), \
        T_case$((u_V_const$T)((u_P_const$T){ .inner[0] = __b.inner, .type = __b.type })), \
        T_case$((u_V$T)((u_P$T){ .inner[0] = __b.inner, .type = __b.type })) \
    )); \
})
#undef u_deref
#define u_deref(_p /*: u_P_const$T|u_P$T*/... /*(u_V_const$T|u_V$T)*/) T_switch$((TypeOf(&*_p.raw))( \
    T_case$((P_const$T)(*as$(u_V_const$T*)(_p.inner))), \
    T_case$((P$T)(*as$(u_V$T*)(_p.inner))) \
))

#undef u_store
#define u_store(_p /*: u_P$T */, _b... /*(u_P$T)*/) ({ \
    const u_P$T __p = _p; \
    const u_T __b = _b; \
    claim_assert(TypeInfo_eq(__p.type, __b.type)); \
    prim_memcpy(*__p.inner, __b.inner, __b.type.size); \
    __p; \
})
#undef u_load
#define u_load(_v /*: u_V_const$T|u_V$T*/... /*(u_T)*/) ({ \
    const TypeOf(_v) __v = _v; \
    const u_T __b = u_allocV(__v.type); \
    prim_memcpy(__b.inner, __v.inner, __v.type.size); \
    __b; \
})

#define u_copy(_src... /*(u_T)*/) ({ \
    const TypeOf(_src) __v = _src; \
    const u_T __b = u_allocV(__v.type); \
    prim_memcpy(__b.inner, __v.inner, __v.type.size); \
    __b; \
})
#define u_move(_src... /*(u_T)*/) ({ \
    const TypeOf(_src) __v = _src; \
    const u_T __b = u_allocV(__v.type); \
    prim_memcpy(__b.inner, __v.inner, __v.type.size); \
    prim_memset(__v.inner, 0, __v.type.size); \
    __b; \
})

#define u_at(_s, _idx...) __u_at(pp_uniqTok(s), pp_uniqTok(idx), _s, _idx)
#define __u_at(__s, __idx, _s, _idx...) ({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__idx, usize) = _idx; \
    claim_assert_fmt(__idx < __s.len, "Index out of bounds: idx(%zu) >= len(%zu)", __idx, __s.len); \
    _Generic( \
        TypeOf(&*__s.ptr), \
        P_const$T: (u_P_const$T){ .inner[0] = (const u_Inner*)(__s.ptr + (__idx * __s.type.size)), .type = __s.type }, \
        P$T: (u_P$T){ .inner[0] = (u_Inner*)(__s.ptr + (__idx * __s.type.size)), .type = __s.type } \
    ); \
})

#if UNUSED_CODE
fn_((old_cmp_lessFn(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) {
    let l = u_castV$((i32)(lhs));
    let r = u_castV$((i32)(rhs));
    return prim_ord(&l, &r);
}
typedef TypeOf(old_cmp_lessFn) old_cmp_LessFn;

fn_((max(u_S_const$raw items, old_cmp_LessFn cmpFn, u_V$raw ret))(u_V$raw)) {
    var best = u_atS(items, 0);
    for_(($us(items))(item) {
        if (cmpFn(u_castV$((u_V$raw)(item)), u_castV$((u_V$raw)(best))) == cmp_Ord_gt) {
            best = item;
        }
    });
    return u_memcpy(ret.ref, best), ret;
}

fn_((max$i32(S_const$i32 items, old_cmp_LessFn cmpFn))(i32) $scope) {
    return u_castV$((ReturnType)(max(u_anyS(items), cmpFn, u_retV$(ReturnType))));
} $unscoped_(fn);
#endif /* UNUSED_CODE */

fn_((cmp_lessFn(u_T lhs, u_T rhs))(cmp_Ord)) {
    let l = u_castV$((i32)(lhs));
    let r = u_castV$((i32)(rhs));
    return prim_ord(l, r);
}
typedef TypeOf(cmp_lessFn) cmp_LessFn;

#undef at$u_S
#define at$u_S u_at
fn_((min(u_S_const$T items, cmp_LessFn cmpFn, u_V$T ret))(u_V$T)) {
    var best = u_load(u_deref(u_at(items, 0)));
    for_(($us(items))(item) {
        if (cmpFn(u_copy(best), u_load(u_deref(item))) == cmp_Ord_lt) {
            u_store(u_ref(best), u_load(u_deref(item)));
        }
    });
    return u_deref(u_store(u_ref(ret), best));
}
