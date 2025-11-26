#include "dh/prl.h"

typedef TypeOf(const void*) P_const$raw;
typedef TypeOf(void*) P$raw;

typedef P_const$raw P_const$T;
typedef P$raw P$T;
typedef S_const$raw S_const$T;
typedef S$raw S$T;

typedef TypeOf(u8[0]) u_Inner;
T_use_S$(u_Inner);
typedef struct u_T {
    u_Inner* inner;
    TypeInfo type;
} u_T;
typedef struct u_V_const$T {
    const u_Inner* inner;
    TypeInfo type;
} u_V_const$T;
typedef union u_V$T {
    struct {
        u_Inner* inner;
        TypeInfo type;
    };
    u_V_const$T as_const;
} u_V$T;

typedef struct u_P_const$T {
    union {
        const u_Inner* inner[1];
        P_const$T raw;
    };
    TypeInfo type;
} u_P_const$T;
typedef union u_P$T {
    struct {
        union {
            u_Inner* inner[1];
            P$T raw;
        };
        TypeInfo type;
    };
    u_P_const$T as_const;
} u_P$T;

typedef struct u_S_const$T {
    union {
        S_const$u_Inner inner[1];
        S_const$T raw;
        struct {
            P_const$T ptr;
            usize len;
        };
    };
    TypeInfo type;
} u_S_const$T;
typedef union u_S$T {
    struct {
        union {
            S$u_Inner inner[1];
            S$T raw;
            struct {
                P$T ptr;
                usize len;
            };
        };
        TypeInfo type;
    };
    u_S_const$T as_const;
} u_S$T;

#undef u_allocV
#define u_allocV(_type...) ({ \
    const TypeInfo __type = _type; \
    const P$T __ptr = alloca(__type.size); \
    prim_memset(__ptr, 0, __type.size); \
    lit$((u_T){ .inner = __ptr, .type = __type }); \
})

#define u_ref(_b /*: u_T*/... /*(u_P_const$T|u_P$T)*/) ({ \
    const TypeOf(_b) __b = _b; \
    _Generic( \
        TypeOf(_b), \
        const u_T: (u_P_const$T){ .inner[0] = __b.inner, .type = __b.type }, \
        u_T: (u_P$T){ .inner[0] = __b.inner, .type = __b.type }, \
        u_V_const$T: (u_P_const$T){ .inner[0] = __b.inner, .type = __b.type }, \
        u_V$T: (u_P$T){ .inner[0] = __b.inner, .type = __b.type } \
    ); \
})
#define u_deref(_p /*: u_P_const$T|u_P$T*/... /*(u_V_const$T|u_V$T)*/) _Generic( \
    TypeOf(&*_p.raw), \
    P_const$T: (*(u_V_const$T*)(_p.inner)), \
    P$T: (*(u_V$T*)(_p.inner)) \
)
#define u_load(_v /*: u_V_const$T|u_V$T*/... /*(u_T)*/) ({ \
    const TypeOf(_v) __v = _v; \
    const u_T __b = u_allocV(__v.type); \
    prim_memcpy(__b.inner, __v.inner, __v.type.size); \
    __b; \
})
#define u_store(_p, _b...) ({ \
    const u_P$T __p = _p; \
    const u_T __b = _b; \
    claim_assert(TypeInfo_eq(__p.type, __b.type)); \
    prim_memcpy(*__p.inner, __b.inner, __b.type.size); \
    __p; \
})
#define u_copy(_src...) ({ \
    const TypeOf(_src) __v = _src; \
    const u_T __b = u_allocV(__v.type); \
    prim_memcpy(__b.inner, __v.inner, __v.type.size); \
    __b; \
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

#define u_at(_s, _idx...)               __u_at(pp_uniqTok(s), pp_uniqTok(idx), _s, _idx)
#define __u_at(__s, __idx, _s, _idx...) blk({ \
    let_(__s, TypeOf(_s)) = _s; \
    let_(__idx, usize) = _idx; \
    claim_assert_fmt(__idx < __s.len, "Index out of bounds: idx(%zu) >= len(%zu)", __idx, __s.len); \
    _Generic( \
        TypeOf(&*__s.ptr), \
        P_const$T: (u_P_const$T){ .inner[0] = (const u_Inner*)(__s.ptr + (__idx * __s.type.size)), .type = __s.type }, \
        P$T: (u_P$T){ .inner[0] = (u_Inner*)(__s.ptr + (__idx * __s.type.size)), .type = __s.type } \
    ); \
})

fn_((cmp_lessFn(u_T lhs, u_T rhs))(cmp_Ord)) {
    let l = u_castV$((i32)(lhs));
    let r = u_castV$((i32)(rhs));
    return prim_ord(&l, &r);
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
