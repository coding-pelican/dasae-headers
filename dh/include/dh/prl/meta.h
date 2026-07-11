#pragma once
#ifndef prl_tpl_meta__included
#define prl_tpl_meta__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "raw.h"
#include "tpl.h"

/*========== Macros and Declarations ========================================*/

// ============================================================================
// Generic Meta Types - The Foundation
// ============================================================================

typedef struct u_P_const$raw {
    T_embed$(union {
        P_const$raw raw;
        P_const$raw inner $like_ref;
    });
    TypeInfo type;
} u_P_const$raw;
typedef union u_P$raw {
    T_embed$(struct {
        T_embed$(union {
            P$raw raw;
            P$raw inner $like_ref;
        });
        TypeInfo type;
    });
    u_P_const$raw as_const;
} u_P$raw;

typedef union u_V$raw {
    T_embed$(struct {
        T_embed$(union {
            V$raw raw;
            P$raw inner;
        });
        T_embed$(union {
            TypeInfo inner_type;
            TypeInfo type;
        });
    });
    u_P$raw ref;
} u_V$raw;

// typedef struct u_FnCtx$raw {
//     FnCtx$raw fn;
//     u_V$raw inner;
// } u_FnCtx$raw;

typedef struct u_S_const$raw {
    T_embed$(union {
        T_embed$(struct {
            P_const$raw ptr;
            usize len;
        });
        S_const$raw raw;
        S_const$raw inner $like_ref;
    });
    TypeInfo type;
    struct {
        u_P_const$raw ptr $zero_sized;
    } __type_hint $zero_sized;
} u_S_const$raw;
typedef union u_S$raw {
    T_embed$(struct {
        T_embed$(union {
            T_embed$(struct {
                P$raw ptr;
                usize len;
            });
            S$raw raw;
            S$raw inner $like_ref;
        });
        TypeInfo type;
    });
    u_S_const$raw as_const;
    struct {
        u_P$raw ptr $zero_sized;
    } __type_hint $zero_sized;
} u_S$raw;

typedef union u_A$raw {
    T_embed$(struct {
        S$raw inner;
        T_embed$(union {
            TypeInfo inner_type;
            TypeInfo type;
        });
    });
    u_S$raw ref;
} u_A$raw;

/* Meta wrapper aliases */
#define u_V$(_$T...) __u_V$(_$T)
#define u_P_const$(_$T...) __u_P_const$(_$T)
#define u_P$(_$T...) pp_Tok_if_(Tok_isConst$(_$T))( \
    pp_Tok_then_(__u_P_const$(Tok_removeConst$(_$T))), \
    pp_Tok_else_(__u_P$(_$T)) \
)
#define u_S_const$(_$T...) __u_S_const$(_$T)
#define u_S$(_$T...) pp_Tok_if_(Tok_isConst$(_$T))( \
    pp_Tok_then_(__u_S_const$(Tok_removeConst$(_$T))), \
    pp_Tok_else_(__u_S$(_$T)) \
)
#define __u_V$(_$T...) tpl$(u_V, _$T)
#define __u_P_const$(_$T...) tpl$(u_P_const, _$T)
#define __u_P$(_$T...) tpl$(u_P, _$T)
#define __u_S_const$(_$T...) tpl$(u_S_const, _$T)
#define __u_S$(_$T...) tpl$(u_S, _$T)

/* Meta wrapper templates */
#define T_decl_u_V$(_$T...) \
    $maybe_unused typedef union u_V$(_$T) u_V$(_$T)
#define T_impl_u_V$(_$T...) \
    union u_V$(_$T) { \
        T_embed$(struct { \
            T_embed$(union { \
                $P$(_$T) \
                inner; \
                $P$(_$T) \
                raw $like_ref; \
            }); \
            T_embed$(union { \
                TypeInfo inner_type; \
                TypeInfo type; \
            }); \
        }); \
        u_V$raw as_raw; \
        u_P$raw ref; \
    }
#define T_use_u_V$(_$T...) \
    T_decl_u_V$(_$T); \
    T_impl_u_V$(_$T)

#define T_decl_u_P$(_$T...) \
    $maybe_unused typedef union u_P$(const _$T) u_P$(const _$T); \
    $maybe_unused typedef union u_P$(_$T) u_P$(_$T)
#define T_impl_u_P$(_$T...) \
    union u_P$(const _$T) { \
        T_embed$(struct { \
            T_embed$(union { \
                $P$(const _$T) \
                raw; \
                $P$(const _$T) \
                inner $like_ref; \
            }); \
            TypeInfo type; \
        }); \
        u_P_const$raw as_raw; \
    }; \
    union u_P$(_$T) { \
        T_embed$(struct { \
            T_embed$(union { \
                $P$(_$T) \
                raw; \
                $P$(_$T) \
                inner $like_ref; \
            }); \
            TypeInfo type; \
        }); \
        u_P$(const _$T) as_const; \
        u_P$raw as_raw; \
    }
#define T_use_u_P$(_$T...) \
    T_decl_u_P$(_$T); \
    T_impl_u_P$(_$T)

#define T_decl_u_S$(_$T...) \
    $maybe_unused typedef union u_S$(const _$T) u_S$(const _$T); \
    $maybe_unused typedef union u_S$(_$T) u_S$(_$T)
#define T_impl_u_S$(_$T...) \
    union u_S$(const _$T) { \
        T_embed$(struct { \
            T_embed$(union { \
                T_embed$(struct { \
                    $P$(const _$T) \
                    ptr; \
                    usize len; \
                }); \
                S_const$raw raw; \
                S_const$raw inner $like_ref; \
            }); \
            TypeInfo type; \
            struct { \
                u_P_const$raw ptr $zero_sized; \
            } __type_hint $zero_sized; \
        }); \
        u_S_const$raw as_raw; \
    }; \
    union u_S$(_$T) { \
        T_embed$(struct { \
            T_embed$(union { \
                T_embed$(struct { \
                    $P$(_$T) \
                    ptr; \
                    usize len; \
                }); \
                S$raw raw; \
                S$raw inner $like_ref; \
            }); \
            TypeInfo type; \
        }); \
        u_S$(const _$T) as_const; \
        u_S$raw as_raw; \
        struct { \
            u_P$raw ptr $zero_sized; \
        } __type_hint $zero_sized; \
    }
#define T_use_u_S$(_$T...) \
    T_decl_u_S$(_$T); \
    T_impl_u_S$(_$T)

typedef struct u_O$raw {
    bool is_some;
    T_embed$(union {
        union {
            Void none;
            u_V$raw some;
        } payload;
        u_V$raw inner;
    });
} u_O$raw;

typedef struct u_E$raw {
    bool is_ok;
    T_embed$(union {
        union {
            EAny err;
            u_V$raw ok;
        } payload;
        u_V$raw inner;
    });
} u_E$raw;

#define V_meta(/*(_$type: TypeInfo)(_$raw: u_Inner*)*/... /*(u_V_const$T|u_V$T)*/) \
    __step__V_meta(__step__V_meta__parse __VA_ARGS__)
#define __step__V_meta__parse(_$type...) _$type,
#define __step__V_meta(...) ____V_meta(__VA_ARGS__)
#define ____V_meta(_$type, _$raw...) T_switch$((TypeOf(*_$raw))( \
    T_case$((V$raw)(l$((u_V$raw){ .raw = as$(V$raw)(_$raw->inner), .type = _$type }))) \
))
#define V_meta$(/*(_$u_VT)(_$type: TypeInfo)(_$raw: u_Inner*)*/... /*(u_V_const$T|u_V$T)*/) \
    __step__V_meta$(__step__V_meta$__parse __VA_ARGS__)
#define __step__V_meta$__parse(_$u_VT...) _$u_VT, __step__V_meta__parse
#define __step__V_meta$(...) ____V_meta$(__VA_ARGS__)
#define ____V_meta$(_$u_VT, _$type, _$raw...) l$((_$u_VT){ \
    .inner = ptrCast$((FieldType$(_$u_VT, inner))((_$raw)->inner)), \
    .type = _$type, \
})
#define P_meta(/*(_$type: TypeInfo)(_$raw: P_const$T|P$T)*/... /*(u_P_const$T|u_P$T)*/) \
    __step__P_meta(__step__P_meta__parse __VA_ARGS__)
#define __step__P_meta__parse(_$type...) _$type,
#define __step__P_meta(...) ____P_meta(__VA_ARGS__)
#define ____P_meta(_$type, _$raw...) T_switch$((TypeOf(_$raw))( \
    T_case$((P_const$raw)(l$((u_P_const$raw){ .raw = ptrQualCast$((P_const$raw)(_$raw)), .type = _$type }))), \
    T_case$((P$raw)(l$((u_P$raw){ .raw = ptrQualCast$((P$raw)(_$raw)), .type = _$type }))) \
))
#define P_meta$(/*(_$u_PT)(_$type: TypeInfo)(_$raw: P_const$T|P$T)*/... /*(u_P_const$T|u_P$T)*/) \
    __step__P_meta$(__step__P_meta$__parse __VA_ARGS__)
#define __step__P_meta$__parse(_$u_PT...) _$u_PT, __step__P_meta__parse
#define __step__P_meta$(...) ____P_meta$(__VA_ARGS__)
#define ____P_meta$(_$u_PT, _$type, _$raw...) l$((_$u_PT){ \
    .raw = ptrQualCast$((FieldType$(_$u_PT, raw))(_$raw)), \
    .type = _$type, \
})
#define S_meta(/*(_$type: TypeInfo)(_$raw: S_const$T|S$T)*/... /*(u_S_const$T|u_S$T)*/) \
    __step__S_meta(__step__S_meta__parse __VA_ARGS__)
#define __step__S_meta__parse(_$type...) _$type,
#define __step__S_meta(...) ____S_meta(__VA_ARGS__)
#define ____S_meta(_$type, _$raw...) T_switch$((TypeOf(_$raw))( \
    T_case$((S_const$raw)(l$((u_S_const$raw){ .raw = *ptrQualCast$((S_const$raw*)(&copy(_$raw))), .type = _$type }))), \
    T_case$((S$raw)(l$((u_S$raw){ .raw = *ptrQualCast$((S$raw*)(&copy(_$raw))), .type = _$type }))) \
))
#define S_meta$(/*(_$u_ST)(_$type: TypeInfo)(_$raw: S_const$T|S$T)*/... /*(u_S_const$T|u_S$T)*/) \
    __step__S_meta$(__step__S_meta$__parse __VA_ARGS__)
#define __step__S_meta$__parse(_$u_ST...) _$u_ST, __step__S_meta__parse
#define __step__S_meta$(...) ____S_meta$(__VA_ARGS__)
#define ____S_meta$(_$u_ST, _$type, _$raw...) l$((_$u_ST){ \
    .raw = *ptrQualCast$((FieldType$(_$u_ST, raw)*)(&copy(_$raw))), \
    .type = _$type, \
})

#define V_raw(_$v /*: u_V_const$T|u_V$T*/... /*(u_Inner*)*/) (_$v.inner)
#define P_raw(_$p /*: u_P_const$T|u_P$T*/... /*(P_const$T|P$T)*/) (_$p.raw)
#define S_raw(_$s /*: u_S_const$T|u_S$T*/... /*(S_const$T|S$T)*/) (_$s.raw)

#define u_allocV(_$type...) ({ \
    const TypeInfo __type = _$type; \
    const usize __size = TypeInfo_size(__type); \
    const usize __align = TypeInfo_align(__type); \
    const P$raw __alloced = raw_alloca(__size + __align - 1); \
    const P$raw __aligned = intToPtr$((P$raw)((ptrToInt(__alloced) + (__align - 1)) & ~(__align - 1))); \
    $ignore_void raw_memset0(__aligned, __size); \
    l$((u_V$raw){ .inner = __aligned, .inner_type = __type }); \
})

#define u_allocA(_$len, _$type...) ({ \
    const TypeInfo __type = _$type; \
    const usize __len = _$len; \
    const usize __size = TypeInfo_size(__type); \
    const usize __align = TypeInfo_align(__type); \
    const P$raw __alloced = raw_alloca(__len * __size + __align - 1); \
    const P$raw __aligned = intToPtr$((P$raw)((ptrToInt(__alloced) + (__align - 1)) & ~(__align - 1))); \
    $ignore_void raw_memset0(__aligned, __len * __size); \
    l$((u_A$raw){ .inner = { .ptr = __aligned, .len = __len }, .inner_type = __type }); \
})

#define u_make u_allocV
#define u_create(_$type...) (u_make(_$type).ref)

#define u_deref(_$p...) $suppress_cast_qual(T_switch$((TypeOf(_$p))( \
    T_case$((u_P_const$raw)(*as$(u_V$raw*)((_$p).inner))), \
    T_case$((u_P$raw)(*as$(u_V$raw*)((_$p).inner))) \
)))
#define u_load(_$v /*: u_V_const$T|u_V$T*/... /*(u_T)*/) $suppress_cast_qual(({ \
    const u_V$raw __v = _$v; \
    const u_V$raw __b = u_allocV(__v.inner_type); \
    $ignore_void raw_memcpy(__b.inner, __v.inner, __v.inner_type.size); \
    __b; \
}))

#define u_TypeHintOf(_$T_Base, _$hint_field...) \
    FieldType$(_$T_Base, __type_hint->_$hint_field $like_deref)

#define u_from$S(/*(_$type: TypeInfo)(_$s: S$raw)*/... /*(u_S$raw)*/) \
    __u_from$S__step(pp_defer(__u_from$S__emit)(__u_from$S__parseType __VA_ARGS__))
#define __u_from$S__step(...) __VA_ARGS__
#define __u_from$S__parseType(_$type...) \
    pp_Tok_if_(Tok_isConst$(_$type))( \
        pp_Tok_then_(u_S_const$raw, Tok_removeConst$(_$type)), \
        pp_Tok_else_(u_S$raw, _$type) \
    ), \
        __u_from$S__parseS
#define __u_from$S__parseS(_$s...) _$s
#define __u_from$S__emit(...) __u_from$S__emitNext(__VA_ARGS__)
#define __u_from$S__emitNext(_$T_u, _$type, _$s...) l$((_$T_u){ .raw = _$s, .type = _$type })

#define u_init$S(/*(_$type: TypeInfo)(_$ptr: P$$(_$T), _$len: usize)*/... /*(u_S$raw)*/) \
    __u_init$S__step(pp_defer(__u_init$S__emit)(__u_init$S__parseType __VA_ARGS__))
#define __u_init$S__step(...) __VA_ARGS__
#define __u_init$S__parseType(_$type...) _$type, __u_init$S__parsePtrLen
#define __u_init$S__parsePtrLen(_$ptr, _$len...) _$ptr, _$len
#define __u_init$S__emit(_$type, _$ptr, _$len...) \
    pp_Tok_if_(Tok_isConst$(_$type))( \
        pp_then_(__u_init$S__emitRef(Tok_removeConst$(_$type), _$ptr, _$len)), \
        pp_else_(__u_init$S__emitRefMut(_$type, _$ptr, _$len)) \
    )
#define __u_init$S__emitRef(_$type, _$ptr, _$len...) l$((u_S_const$raw){ .ptr = _$ptr, .len = _$len, .type = _$type })
#define __u_init$S__emitRefMut(_$type, _$ptr, _$len...) l$((u_S$raw){ .ptr = _$ptr, .len = _$len, .type = _$type })

#define len$u_S u_lenS
#define u_lenS(_$s...) ____u_lenS(_$s)
#define ____u_lenS(_$s...) ((_$s).len)

#define ptr$u_S u_ptrS
#define u_ptrS(_$s...) __step__u_ptrS(_$s)
#define __step__u_ptrS(_$s...) ____u_ptrS(pp_uniqTok(s), _$s)
#define ____u_ptrS(__s, _$s...) $suppress_cast_qual(({ \
    let_(__s, TypeOf(_$s)) = _$s; \
    T_switch$((TypeOf(_$s))( \
        T_case$((u_S_const$raw)(l$((u_P_const$raw){ .raw = as$(const u8*)(__s.ptr), .type = __s.type }))), \
        T_case$((u_S$raw)(l$((u_P$raw){ .raw = as$(u8*)(__s.ptr), .type = __s.type }))) \
    )); \
}))

#define u_stride_static(_$type...) ____u_stride_static(_$type)
#define ____u_stride_static(_$type...) \
    (((_$type).size + ((usize_(1) << (_$type).log2_align) - 1)) & ~((usize_(1) << (_$type).log2_align) - 1))
#define u_stride(_$type...) __step__u_stride(_$type)
#define __step__u_stride(_$type...) ____u_stride(pp_uniqTok(type), _$type)
#define ____u_stride(__type, _$type...) ({ \
    let_(__type, usize) = _$type; \
    u_stride_static(__type); \
})

#define at$u_S u_atS
#define u_atS(_$s, _$idx...) __u_atS(pp_uniqTok(s), pp_uniqTok(idx), pp_uniqTok(stride), _$s, _$idx)
#define __u_atS(__s, __idx, __stride, _$s, _$idx...) $suppress_cast_qual(({ \
    let_(__s, TypeOf(_$s)) = _$s; \
    let_(__idx, usize) = _$idx; \
    claim_assert_fmt(__idx < u_lenS(__s), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, u_lenS(__s)); \
    let_(__stride, usize) = u_stride_static(__s.type); \
    T_switch$((TypeOf(__s))( \
        T_case$((u_S_const$raw)(l$((u_P_const$raw){ \
            .raw = as$(const u8*)(__s.ptr) + (__idx * __stride), \
            .type = __s.type, \
        }))), \
        T_case$((u_S$raw)(l$((u_P$raw){ \
            .raw = as$(u8*)(__s.ptr) + (__idx * __stride), \
            .type = __s.type, \
        }))) \
    )); \
}))
#if UNUSED_CODE
#define __u_atS(__s, __idx, __stride, _$s, _$idx...) $suppress_cast_qual({ \
    let_(__s, TypeOf(_$s)) = _$s; \
    let_(__idx, usize) = _$idx; \
    claim_assert_fmt(__idx < u_lenS(__s), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, u_lenS(__s)); \
    let_(__stride, usize) = u_stride_static(__s.type); \
    T_switch$((P$$(S_T$(TypeOf(__s))))( \
        T_case$((P_const$raw)(l$((u_P_const$raw){ \
            .raw = as$(const u8*)(__s.ptr) + (__idx * __stride), \
            .type = __s.type, \
        }))), \
        T_case$((P$raw)(l$((u_P$raw){ \
            .raw = as$(u8*)(__s.ptr) + (__idx * __stride), \
            .type = __s.type, \
        }))) \
    )); \
})
#endif /* UNUSED_CODE */

#define u_sliceP(_$p, _$range...) __u_sliceP(pp_uniqTok(p), pp_uniqTok(range), pp_uniqTok(stride), _$p, _$range)
#define __u_sliceP(__p, __range, __stride, _$p, _$range...) $suppress_cast_qual(({ \
    let_(__p, TypeOf(_$p)) = _$p; \
    let_(__range, R) = _$range; \
    claim_assert_fmt(isValidR(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    let_(__stride, usize) = u_stride_static(__p.type); \
    T_switch$((TypeOf(__p))( \
        T_case$((u_P_const$raw)(l$((u_S_const$raw){ \
            .ptr = as$(const u8*)(__p.raw) + (__range.begin * __stride), \
            .len = lenR(__range), \
            .type = __p.type, \
        }))), \
        T_case$((u_P$raw)(l$((u_S$raw){ \
            .ptr = as$(u8*)(__p.raw) + (__range.begin * __stride), \
            .len = lenR(__range), \
            .type = __p.type, \
        }))) \
    )); \
}))
#define u_prefixP(_$p, _$end...) __u_prefixP(pp_uniqTok(p), pp_uniqTok(end), _$p, _$end)
#define __u_prefixP(__p, __end, _$p, _$end...) $suppress_cast_qual(({ \
    let_(__p, TypeOf(_$p)) = _$p; \
    let_(__end, usize) = _$end; \
    T_switch$((TypeOf(__p))( \
        T_case$((u_P_const$raw)(l$((u_S_const$raw){ \
            .ptr = as$(const u8*)(__p.raw), \
            .len = __end, \
            .type = __p.type, \
        }))), \
        T_case$((u_P$raw)(l$((u_S$raw){ \
            .ptr = as$(u8*)(__p.raw), \
            .len = __end, \
            .type = __p.type, \
        }))) \
    )); \
}))
#define u_suffixP(_$p, _$begin...) __u_suffixP(pp_uniqTok(p), pp_uniqTok(begin), pp_uniqTok(stride), _$p, _$begin)
#define __u_suffixP(__p, __begin, __stride, _$p, _$begin...) $suppress_cast_qual(({ \
    let_(__p, TypeOf(_$p)) = _$p; \
    let_(__begin, usize) = _$begin; \
    let_(__stride, usize) = u_stride_static(__p.type); \
    T_switch$((TypeOf(__p))( \
        T_case$((u_P_const$raw)(l$((u_S_const$raw){ \
            .ptr = as$(const u8*)(__p.raw) + (__begin * __stride), \
            .len = usize_limit_max - __begin, \
            .type = __p.type, \
        }))), \
        T_case$((u_P$raw)(l$((u_S$raw){ \
            .ptr = as$(u8*)(__p.raw) + (__begin * __stride), \
            .len = usize_limit_max - __begin, \
            .type = __p.type, \
        }))) \
    )); \
}))

#define u_sliceS(_$s, _$range...) __u_sliceS(pp_uniqTok(s), pp_uniqTok(range), pp_uniqTok(stride), _$s, _$range)
#define __u_sliceS(__s, __range, __stride, _$s, _$range...) $suppress_cast_qual(({ \
    let_(__s, TypeOf(_$s)) = _$s; \
    let_(__range, R) = _$range; \
    claim_assert_fmt(isValidR(__range), "Invalid range: begin({:uz}) > end({:uz})", __range.begin, __range.end); \
    claim_assert_fmt(__range.end <= u_lenS(__s), "Invalid slice range: end({:uz}) > len({:uz})", __range.end, u_lenS(__s)); \
    let_(__stride, usize) = u_stride_static(__s.type); \
    l$((TypeOf(__s)){ \
        .ptr = T_switch$((P$$(S_T$(TypeOf(__s))))( \
                   T_case$((P_const$raw)(as$(const u8*)(ptrS(__s)))), \
                   T_case$((P$raw)(as$(u8*)(ptrS(__s)))) \
               )) \
             + (__range.begin * __stride), \
        .len = lenR(__range), \
        .type = __s.type, \
    }); \
}))
#define u_prefixS(_$s, _$end) __u_prefixS(pp_uniqTok(s), pp_uniqTok(end), _$s, _$end)
#define __u_prefixS(__s, __end, _$s, _$end...) $suppress_cast_qual(({ \
    let_(__s, TypeOf(_$s)) = _$s; \
    let_(__end, usize) = _$end; \
    claim_assert_fmt(__end <= u_lenS(__s), "Invalid slice range: end({:uz}) > len({:uz})", __end, u_lenS(__s)); \
    l$((TypeOf(__s)){ \
        .ptr = T_switch$((P$$(S_T$(TypeOf(__s))))( \
            T_case$((P_const$raw)(as$(const u8*)(ptrS(__s)))), \
            T_case$((P$raw)(as$(u8*)(ptrS(__s)))) \
        )), \
        .len = __end, \
        .type = __s.type, \
    }); \
}))
#define u_suffixS(_$s, _$begin) __u_suffixS(pp_uniqTok(s), pp_uniqTok(begin), pp_uniqTok(stride), _$s, _$begin)
#define __u_suffixS(__s, __begin, __stride, _$s, _$begin...) $suppress_cast_qual(({ \
    let_(__s, TypeOf(_$s)) = _$s; \
    let_(__begin, usize) = _$begin; \
    claim_assert_fmt(__begin <= u_lenS(__s), "Invalid slice range: begin({:uz}) > len({:uz})", __begin, u_lenS(__s)); \
    let_(__stride, usize) = u_stride_static(__s.type); \
    l$((TypeOf(__s)){ \
        .ptr = T_switch$((P$$(S_T$(TypeOf(__s))))( \
                   T_case$((P_const$raw)(as$(const u8*)(ptrS(__s)))), \
                   T_case$((P$raw)(as$(u8*)(ptrS(__s)))) \
               )) \
             + (__begin * __stride), \
        .len = u_lenS(__s) - __begin, \
        .type = __s.type, \
    }); \
}))

$attr($inline_always)
$static fn_((u_memset0(u_P$raw dst))(u_P$raw));
$attr($inline_always)
$static fn_((u_memset(u_P$raw dst, u_V$raw src))(u_P$raw));
$attr($inline_always)
$static fn_((u_memcpy(u_P$raw dst, u_P_const$raw src))(u_P$raw));
$attr($inline_always)
$static fn_((u_memmove(u_P$raw dst, u_P_const$raw src))(u_P$raw));
$attr($inline_always)
$static fn_((u_memeql(u_P_const$raw lhs, u_P_const$raw rhs))(bool));
$attr($inline_always)
$static fn_((u_memord(u_P_const$raw lhs, u_P_const$raw rhs))(cmp_Ord));

$attr($inline_always)
$static fn_((u_memset0S(u_S$raw dst))(u_S$raw));
$attr($inline_always)
$static fn_((u_memsetS(u_S$raw dst, u_V$raw src))(u_S$raw));
$attr($inline_always)
$static fn_((u_memcpyS(u_S$raw dst, u_S_const$raw src))(u_S$raw));
$attr($inline_always)
$static fn_((u_memmoveS(u_S$raw dst, u_S_const$raw src))(u_S$raw));
$attr($inline_always)
$static fn_((u_memeqlS(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
$attr($inline_always)
$static fn_((u_memordS(u_S_const$raw lhs, u_S_const$raw rhs))(cmp_Ord));

$attr($inline_always)
$static fn_((u_eql(u_V$raw lhs, u_V$raw rhs))(bool));
$attr($inline_always)
$static fn_((u_neq(u_V$raw lhs, u_V$raw rhs))(bool));

T_alias$((u_EqlFn)(fn_(((*)(u_V$raw lhs, u_V$raw rhs))(bool) $T)));
$attr($inline_always)
$static fn_((u_eqlBy(u_V$raw lhs, u_V$raw rhs, u_EqlFn eqlFn))(bool));
$attr($inline_always)
$static fn_((u_neqBy(u_V$raw lhs, u_V$raw rhs, u_EqlFn eqlFn))(bool));

T_alias$((u_EqlCtxFn)(fn_(((*)(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool) $T)));
$attr($inline_always)
$static fn_((u_eqlCtx(u_V$raw lhs, u_V$raw rhs, u_EqlCtxFn eqlFn, u_V$raw ctx))(bool));
$attr($inline_always)
$static fn_((u_neqCtx(u_V$raw lhs, u_V$raw rhs, u_EqlCtxFn eqlFn, u_V$raw ctx))(bool));

$attr($inline_always)
$static fn_((u_ord(u_V$raw lhs, u_V$raw rhs))(cmp_Ord));
$attr($inline_always)
$static fn_((u_eq(u_V$raw lhs, u_V$raw rhs))(bool));
$attr($inline_always)
$static fn_((u_ne(u_V$raw lhs, u_V$raw rhs))(bool));
$attr($inline_always)
$static fn_((u_lt(u_V$raw lhs, u_V$raw rhs))(bool));
$attr($inline_always)
$static fn_((u_gt(u_V$raw lhs, u_V$raw rhs))(bool));
$attr($inline_always)
$static fn_((u_le(u_V$raw lhs, u_V$raw rhs))(bool));
$attr($inline_always)
$static fn_((u_ge(u_V$raw lhs, u_V$raw rhs))(bool));

T_alias$((u_OrdFn)(fn_(((*)(u_V$raw lhs, u_V$raw rhs))(cmp_Ord) $T)));
$attr($inline_always)
$static fn_((u_ordBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(cmp_Ord));
$attr($inline_always)
$static fn_((u_eqBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool));
$attr($inline_always)
$static fn_((u_neBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool));
$attr($inline_always)
$static fn_((u_ltBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool));
$attr($inline_always)
$static fn_((u_gtBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool));
$attr($inline_always)
$static fn_((u_leBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool));
$attr($inline_always)
$static fn_((u_geBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool));

T_alias$((u_OrdCtxFn)(fn_(((*)(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord) $T)));
$attr($inline_always)
$static fn_((u_ordCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(cmp_Ord));
$attr($inline_always)
$static fn_((u_eqCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool));
$attr($inline_always)
$static fn_((u_neCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool));
$attr($inline_always)
$static fn_((u_ltCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool));
$attr($inline_always)
$static fn_((u_gtCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool));
$attr($inline_always)
$static fn_((u_leCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool));
$attr($inline_always)
$static fn_((u_geCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool));

T_alias$((u_OrdApxFn)(fn_(((*)(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode))(cmp_Ord) $T)));
$attr($inline_always)
$static fn_((u_ordApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(cmp_Ord));
$attr($inline_always)
$static fn_((u_eqApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_neApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_ltApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_gtApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_leApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_geApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool));

$attr($inline_always)
$static fn_((u_ordApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(cmp_Ord));
$attr($inline_always)
$static fn_((u_eqApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_neApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_ltApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_gtApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_leApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_geApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));

$attr($inline_always)
$static fn_((u_ordApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(cmp_Ord));
$attr($inline_always)
$static fn_((u_eqApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_neApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_ltApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_gtApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_leApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));
$attr($inline_always)
$static fn_((u_geApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool));

// ============================================================================
// Conversion Patterns - The Core Innovation
// ============================================================================

#define u_ret$ u_retV$
#define u_retV$(_$T) ((u_V$raw){ .inner = ptrCast$((P$raw) & l0$((_$T))), .inner_type = typeInfo$(_$T) })
#define u_retA$(_$N, _$T) ((u_A$raw){ .inner = A_ref(l0$((A$$(_$N, _$T)))).as_raw, .inner_type = typeInfo$(FieldType$(A$$(_$N, _$T), val[0])) })
#define u_retO$(_$T) ((u_O$raw){ .inner = l0$((O$$(_$T))).ref_raw, .inner_type = typeInfo$(FieldType$(O$$(_$T), payload->some)) })
#define u_retE$(_$T) ((u_E$raw){ .inner = l0$((E$$(_$T))).ref_raw, .inner_type = typeInfo$(FieldType$(E$$(_$T), payload->ok)) })

// #define u_anyP_const(_$p...) ((u_P_const$raw){ .type = typeInfo$(TypeOf(*_$p)), .raw = _$p })
// #define u_anyP(_$p...) ((u_P$raw){ .type = typeInfo$(TypeOf(*_$p)), .raw = _$p })
#define u_anyP(_$p...) T_switch$((TypeOf(*_$p))( \
    T_qual$((const TypeOfUnqual(*_$p))(l$((u_P_const$raw){ \
        .raw = ptrQualCast$((P_const$raw)(_$p)), \
        .type = typeInfo$(TypeOf(*_$p)), \
    }))), \
    T_qual$((TypeOfUnqual(*_$p))(l$((u_P$raw){ \
        .raw = ptrQualCast$((P$raw)(_$p)), \
        .type = typeInfo$(TypeOf(*_$p)), \
    }))) \
))
// #define u_anyS_const(_$s...) ((u_S_const$raw){ .type = typeInfo$(TypeOf(*_$s.ptr)), .raw = _$s.as_raw })
// #define u_anyS(_$s...)       ((u_S$raw){ .type = typeInfo$(TypeOf(*_$s.ptr)), .raw = _$s.as_raw })
#define u_anyS(_$s...) T_switch$((TypeOf(*(_$s).ptr))( \
    T_qual$((const TypeOfUnqual(*(_$s).ptr))(l$((u_S_const$raw){ \
        .raw = *ptrQualCast$((S_const$raw*)((_$s).ref_raw)), \
        .type = typeInfo$(TypeOf(*(_$s).ptr)), \
    }))), \
    T_qual$((TypeOfUnqual(*(_$s).ptr))(l$((u_S$raw){ \
        .raw = *ptrQualCast$((S$raw*)((_$s).ref_raw)), \
        .type = typeInfo$(TypeOf(*(_$s).ptr)), \
    }))) \
))

#define u_anyV(_$v...) $suppress_cast_qual(({ \
    let __p_v = &copy(_$v); \
    l$((u_V$raw){ .inner = ptrCast$((P$raw)(__p_v)), .inner_type = typeInfo$(TypeOf(*__p_v)) }); \
}))
#define u_anyP$(/*(_$u_PT)(_$p: P_const$T|P$T)*/... /*(_$u_PT)*/) \
    __step_inline__u_anyP$(pp_defer(__emit_inline__u_anyP$)(__param_parse__u_anyP$ __VA_ARGS__))
#define __step_inline__u_anyP$(...) __VA_ARGS__
#define __param_parse__u_anyP$(...) __VA_ARGS__,
#define __emit_inline__u_anyP$(_$u_PT, _$p...) $suppress_cast_qual(({ \
    typedef _$u_PT MetaType; \
    l$((MetaType){ \
        .raw = ptrQualCast$((FieldType$(MetaType, raw))(_$p)), \
        .type = typeInfo$(TypeOf(*(_$p))), \
    }); \
}))
#define u_anyS$(/*(_$u_ST)(_$s: S_const$T|S$T)*/... /*(_$u_ST)*/) \
    __step_inline__u_anyS$(pp_defer(__emit_inline__u_anyS$)(__param_parse__u_anyS$ __VA_ARGS__))
#define __step_inline__u_anyS$(...) __VA_ARGS__
#define __param_parse__u_anyS$(...) __VA_ARGS__,
#define __emit_inline__u_anyS$(_$u_ST, _$s...) $suppress_cast_qual(({ \
    typedef _$u_ST MetaType; \
    l$((MetaType){ \
        .raw = *ptrQualCast$((FieldType$(MetaType, raw)*)((_$s).ref_raw)), \
        .type = typeInfo$(TypeOf(*(_$s).ptr)), \
    }); \
}))
#define u_anyV$(/*(_$u_VT)(_$v: T)*/... /*(_$u_VT)*/) \
    __step_inline__u_anyV$(pp_defer(__emit_inline__u_anyV$)(__param_parse__u_anyV$ __VA_ARGS__))
#define __step_inline__u_anyV$(...) __VA_ARGS__
#define __param_parse__u_anyV$(...) __VA_ARGS__,
#define __emit_inline__u_anyV$(_$u_VT, _$v...) $suppress_cast_qual(({ \
    typedef _$u_VT MetaType; \
    let __p_v = &copy(_$v); \
    l$((MetaType){ \
        .inner = ptrCast$((FieldType$(MetaType, inner))(__p_v)), \
        .type = typeInfo$(TypeOf(*__p_v)), \
    }); \
}))
#define u_anyA(_$a...) $suppress_cast_qual(({ \
    let __p_a = &copy(_$a); \
    l$((u_A$raw){ .inner = A_ref(*__p_a).as_raw, .inner_type = typeInfo$(TypeOf(*__p_a->val)) }); \
}))
#define u_anyO(_$o...) $suppress_cast_qual(({ \
    let __p_o = &copy(_$o); \
    __p_o->is_some \
        ? (u_O$raw)some(u_anyV(__p_o->payload.some)) \
        : (u_O$raw)none(); \
}))
#define u_anyE(_$e...) $suppress_cast_qual(({ \
    let __p_e = &copy(_$e); \
    __p_e->is_ok \
        ? (u_E$raw)ok(u_anyV(__p_e->payload.ok)) \
        : (u_E$raw)err(__p_e->payload.err); \
}))

#define u_as$ u_asV$

#define u_asV$(/*(_$u_VT)(_$meta: u_V$raw|u_V$T)*/... /*(_$u_VT)*/) \
    __step_inline__u_asV$(pp_defer(__emit_inline__u_asV$)(__param_parse__u_asV$ __VA_ARGS__))
#define __step_inline__u_asV$(...) __VA_ARGS__
#define __param_parse__u_asV$(...) __VA_ARGS__,
#define __emit_inline__u_asV$(_$u_VT, _$meta...) $suppress_cast_qual(({ \
    typedef _$u_VT MetaType; \
    let __meta = _$meta; \
    l$((MetaType){ \
        .inner = ptrCast$((FieldType$(MetaType, inner))(__meta.inner)), \
        .type = __meta.type, \
    }); \
}))
#define u_asP$(/*(_$u_PT)(_$meta: u_P_const$raw|u_P$raw|u_P$T)*/... /*(_$u_PT)*/) \
    __step_inline__u_asP$(pp_defer(__emit_inline__u_asP$)(__param_parse__u_asP$ __VA_ARGS__))
#define __step_inline__u_asP$(...) __VA_ARGS__
#define __param_parse__u_asP$(...) __VA_ARGS__,
#define __emit_inline__u_asP$(_$u_PT, _$meta...) $suppress_cast_qual(({ \
    typedef _$u_PT MetaType; \
    $maybe_unused typedef FieldType$(MetaType, raw) RawType; \
    let __meta = _$meta; \
    claim_assert(!P_isConst$(TypeOf(__meta.raw)) || P_isConst$(RawType)); \
    l$((MetaType){ \
        .raw = ptrQualCast$((RawType)(__meta.raw)), \
        .type = __meta.type, \
    }); \
}))
#define u_asS$(/*(_$u_ST)(_$meta: u_S_const$raw|u_S$raw|u_S$T)*/... /*(_$u_ST)*/) \
    __step_inline__u_asS$(pp_defer(__emit_inline__u_asS$)(__param_parse__u_asS$ __VA_ARGS__))
#define __step_inline__u_asS$(...) __VA_ARGS__
#define __param_parse__u_asS$(...) __VA_ARGS__,
#define __emit_inline__u_asS$(_$u_ST, _$meta...) $suppress_cast_qual(({ \
    typedef _$u_ST MetaType; \
    $maybe_unused typedef FieldType$(MetaType, ptr) PtrType; \
    let __meta = _$meta; \
    claim_assert(!P_isConst$(TypeOf(__meta.ptr)) || P_isConst$(PtrType)); \
    l$((MetaType){ \
        .raw = *ptrQualCast$((FieldType$(MetaType, raw)*)(&__meta.raw)), \
        .type = __meta.type, \
    }); \
}))

#define u_cast$ u_castV$

#define u_castP$(/*(_$T)(_$Expr...)*/... /*(P_const$(_$T))*/) \
    __step_inline__u_castP$(pp_defer(__emit_inline__u_castP$)(__param_parse__u_castP$ __VA_ARGS__))
#define __step_inline__u_castP$(...) __VA_ARGS__
#define __param_parse__u_castP$(...) __VA_ARGS__,
#if UNUSED_CODE
#define __emit_inline__u_castP$(_$PT, _$meta...) $suppress_cast_qual((*as$(_$PT*)(_$meta.inner)))
#endif /* UNUSED_CODE */
#define __emit_inline__u_castP$(_$PT, _$meta...) $suppress_cast_qual(({ \
    typedef _$PT CastType; \
    $maybe_unused typedef DerefType$(CastType) DerefType; \
    $maybe_unused typedef DerefType InnerType; \
    claim_assert(T_switch$((TypeOf(_$meta.raw))( \
        T_qual$((P_const$raw)(P_isConst$(CastType))), \
        T_default_(true) \
    ))); \
    *as$(CastType*)(_$meta.inner); \
}))

#define u_castV$(/*(_$T)(_$Expr...)*/... /*(_$T)*/) \
    __step_inline__u_castV$(pp_defer(__emit_inline__u_castV$)(__param_parse__u_castV$ __VA_ARGS__))
#define __step_inline__u_castV$(...) __VA_ARGS__
#define __param_parse__u_castV$(...) __VA_ARGS__,
#define __emit_inline__u_castV$(_$T, _$meta...) $suppress_cast_qual((*as$(_$T*)(_$meta.inner)))

#define u_castS$(/*(_$T)(_$Expr...)*/... /*(S_const$(_$T))*/) \
    __step_inline__u_castS$(pp_defer(__emit_inline__u_castS$)(__param_parse__u_castS$ __VA_ARGS__))
#define __step_inline__u_castS$(...) __VA_ARGS__
#define __param_parse__u_castS$(...) __VA_ARGS__,
#if UNUSED_CODE
#define __emit_inline__u_castS$(_$ST, _$meta...) $suppress_cast_qual((*as$(_$ST*)(_$meta.inner)))
#endif /* UNUSED_CODE */
#define __emit_inline__u_castS$(_$ST, _$meta...) $suppress_cast_qual(({ \
    typedef _$ST CastType; \
    $maybe_unused typedef FieldType$(CastType, ptr) PtrType; \
    $maybe_unused typedef DerefType$(PtrType) DerefType; \
    $maybe_unused typedef DerefType InnerType; \
    claim_assert(T_switch$((TypeOf(_$meta.raw.ptr))( \
        T_qual$((P_const$raw)(P_isConst$(PtrType))), \
        T_default_(true) \
    ))); \
    *as$(CastType*)(_$meta.inner); \
}))

#define u_castA$(/*(_$N,_$T)(_$Expr...)*/... /*(A$(_$N,_$T))*/) \
    __step_inline__u_castA$(pp_defer(__emit_inline__u_castA$)(__param_parse__u_castA$ __VA_ARGS__))
#define __step_inline__u_castA$(...) __VA_ARGS__
#define __param_parse__u_castA$(...) __VA_ARGS__,
#define __emit_inline__u_castA$(_ANT, _$meta...) $suppress_cast_qual((*as$(_ANT*)(_$meta.inner)))

#define u_castO$(/*(_$OT)(_$Expr...)*/... /*(_$OT)*/) \
    __step__u_castO$__expand(pp_defer(__step__u_castO$)(__step__u_castO$__parseOT __VA_ARGS__))
#define __step__u_castO$__expand(...) __VA_ARGS__
#define __step__u_castO$__parseOT(_$OT...) pp_uniqTok(meta), _$OT,
#define __step__u_castO$(__meta, _$OT, _$Expr...) $suppress_cast_qual(({ \
    typedef _$OT O$Ret$u_castO$; \
    $maybe_unused typedef O$Ret$u_castO$ CastType; \
    $maybe_unused typedef FieldType$(CastType, payload.some) SomeType; \
    $maybe_unused typedef SomeType InnerType; \
    let_(__meta, TypeOf(_$Expr)) = _$Expr; \
    __meta.is_some \
        ? l$((O$Ret$u_castO$)some(*as$(FieldType$(O$Ret$u_castO$, payload.some)*)(__meta.payload.some.inner))) \
        : l$((O$Ret$u_castO$)none()); \
}))

#define u_castE$(/*(_$ET)(_$Expr...)*/... /*(_$ET)*/) \
    __step__u_castE$__expand(pp_defer(__step__u_castE$)(__step__u_castE$__parseET __VA_ARGS__))
#define __step__u_castE$__expand(...) __VA_ARGS__
#define __step__u_castE$__parseET(_$ET...) pp_uniqTok(meta), _$ET,
#define __step__u_castE$(__meta, _$ET, _$Expr...) $suppress_cast_qual(({ \
    typedef _$ET E$Ret$u_castE$; \
    $maybe_unused typedef E$Ret$u_castE$ CastType; \
    $maybe_unused typedef FieldType$(CastType, payload.err) ErrType; \
    $maybe_unused typedef FieldType$(CastType, payload.ok) OkType; \
    $maybe_unused typedef OkType InnerType; \
    let_(__meta, TypeOf(_$Expr)) = _$Expr; \
    __meta.is_ok \
        ? l$((E$Ret$u_castE$)ok(*as$(FieldType$(E$Ret$u_castE$, payload.ok)*)(__meta.payload.ok.inner))) \
        : l$((E$Ret$u_castE$)err(__expr__E__castForReturn(((E$Ret$u_castE$){}).payload.err, __meta.payload.err))); \
}))

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((u_memset0(u_P$raw dst))(u_P$raw)) {
    claim_assert_nonnull(dst.raw);
    return raw_memset0(dst.raw, dst.type.size), dst;
};
fn_((u_memset(u_P$raw dst, u_V$raw src))(u_P$raw)) {
    claim_assert_fmt(
        dst.type.size == src.inner_type.size,
        "Type size mismatch: dst({:uz}) != src({:uz})",
        dst.type.size, src.inner_type.size
    );
    claim_assert_fmt(
        dst.type.log2_align == src.inner_type.log2_align,
        "Type alignment mismatch: dst({:uz}) != src({:uz})",
        dst.type.log2_align, src.inner_type.log2_align
    );
    claim_assert_fmt(
        dst.raw != src.inner,
        "Pointers must not be the same: dst({:p}) != src({:p})",
        dst.raw, src.inner
    );
    return raw_memcpy(dst.raw, src.inner, src.inner_type.size), dst;
};
fn_((u_memcpy(u_P$raw dst, u_P_const$raw src))(u_P$raw)) {
    claim_assert_fmt(
        dst.type.size == src.type.size,
        "Type size mismatch: dst({:uz}) != src({:uz})",
        dst.type.size, src.type.size
    );
    claim_assert_fmt(
        dst.type.log2_align == src.type.log2_align,
        "Type alignment mismatch: dst({:uz}) != src({:uz})",
        dst.type.log2_align, src.type.log2_align
    );
    claim_assert_fmt(
        dst.raw != src.raw,
        "Pointers must not be the same: dst({:p}) != src({:p})",
        dst.raw, src.raw
    );
    return raw_memcpy(dst.raw, src.raw, src.type.size), dst;
};
fn_((u_memmove(u_P$raw dst, u_P_const$raw src))(u_P$raw)) {
    claim_assert_fmt(
        dst.type.size == src.type.size,
        "Type size mismatch: dst({:uz}) != src({:uz})",
        dst.type.size, src.type.size
    );
    claim_assert_fmt(
        dst.type.log2_align == src.type.log2_align,
        "Type alignment mismatch: dst({:uz}) != src({:uz})",
        dst.type.log2_align, src.type.log2_align
    );
    return raw_memmove(dst.raw, src.raw, src.type.size), dst;
};
fn_((u_memeql(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) {
    claim_assert_fmt(
        lhs.type.size == rhs.type.size,
        "Type size mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.type.size, rhs.type.size
    );
    claim_assert_fmt(
        lhs.type.log2_align == rhs.type.log2_align,
        "Type alignment mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.type.log2_align, rhs.type.log2_align
    );
    return raw_memeql(lhs.raw, rhs.raw, rhs.type.size);
};
fn_((u_memord(u_P_const$raw lhs, u_P_const$raw rhs))(cmp_Ord)) {
    claim_assert_fmt(
        lhs.type.size == rhs.type.size,
        "Type size mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.type.size, rhs.type.size
    );
    claim_assert_fmt(
        lhs.type.log2_align == rhs.type.log2_align,
        "Type alignment mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.type.log2_align, rhs.type.log2_align
    );
    return raw_memord(lhs.raw, rhs.raw, rhs.type.size);
};

fn_((u_memset0S(u_S$raw dst))(u_S$raw)) {
    claim_assert_nonnull(dst.ptr);
    return raw_memset0(dst.ptr, dst.len * dst.type.size), dst;
};
fn_((u_memsetS(u_S$raw dst, u_V$raw src))(u_S$raw)) {
    claim_assert_fmt(
        dst.type.size == src.inner_type.size,
        "Type size mismatch: dst({:uz}) != src({:uz})",
        dst.type.size, src.inner_type.size
    );
    claim_assert_fmt(
        dst.type.log2_align == src.inner_type.log2_align,
        "Type alignment mismatch: dst({:uz}) != src({:uz})",
        dst.type.log2_align, src.inner_type.log2_align
    );
    claim_assert_fmt(
        dst.ptr != src.inner,
        "Pointers must not be the same: dst({:p}) != src({:p})",
        dst.ptr, src.inner
    );
    for_(($r(0, dst.len))(i)) { u_memcpy(u_atS(dst, i), src.ref.as_const); } $end(for);
    return dst;
};
fn_((u_memcpyS(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_fmt(
        dst.type.size == src.type.size,
        "Type size mismatch: dst({:uz}) != src({:uz})",
        dst.type.size, src.type.size
    );
    claim_assert_fmt(
        dst.type.log2_align == src.type.log2_align,
        "Type alignment mismatch: dst({:uz}) != src({:uz})",
        dst.type.log2_align, src.type.log2_align
    );
    claim_assert_fmt(
        dst.len == src.len,
        "Length mismatch: dst({:uz}) != src({:uz})",
        dst.len, src.len
    );
    claim_assert_fmt(
        dst.ptr != src.ptr,
        "Pointers must not be the same: dst({:p}) != src({:p})",
        dst.ptr, src.ptr
    );
    return raw_memcpy(dst.ptr, src.ptr, src.len * src.type.size), dst;
};
fn_((u_memmoveS(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_fmt(
        dst.type.size == src.type.size,
        "Type size mismatch: dst({:uz}) != src({:uz})",
        dst.type.size, src.type.size
    );
    claim_assert_fmt(
        dst.type.log2_align == src.type.log2_align,
        "Type alignment mismatch: dst({:uz}) != src({:uz})",
        dst.type.log2_align, src.type.log2_align
    );
    claim_assert_fmt(
        dst.len == src.len,
        "Length mismatch: dst({:uz}) != src({:uz})",
        dst.len, src.len
    );
    return raw_memmove(dst.ptr, src.ptr, src.len * src.type.size), dst;
};
fn_((u_memeqlS(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) {
    claim_assert_fmt(
        lhs.type.size == rhs.type.size,
        "Type size mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.type.size, rhs.type.size
    );
    claim_assert_fmt(
        lhs.type.log2_align == rhs.type.log2_align,
        "Type alignment mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.type.log2_align, rhs.type.log2_align
    );
    claim_assert_fmt(
        lhs.len == rhs.len,
        "Length mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.len, rhs.len
    );
    return raw_memeql(lhs.ptr, rhs.ptr, rhs.len * rhs.type.size);
};
fn_((u_memordS(u_S_const$raw lhs, u_S_const$raw rhs))(cmp_Ord)) {
    claim_assert_fmt(
        lhs.type.size == rhs.type.size,
        "Type size mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.type.size, rhs.type.size
    );
    claim_assert_fmt(
        lhs.type.log2_align == rhs.type.log2_align,
        "Type alignment mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.type.log2_align, rhs.type.log2_align
    );
    claim_assert_fmt(
        lhs.len == rhs.len,
        "Length mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.len, rhs.len
    );
    return raw_memord(lhs.ptr, rhs.ptr, rhs.len * rhs.type.size);
};

fn_((u_eql(u_V$raw lhs, u_V$raw rhs))(bool)) {
    claim_assert_fmt(
        lhs.inner_type.size == rhs.inner_type.size,
        "Type size mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.size, rhs.inner_type.size
    );
    claim_assert_fmt(
        lhs.inner_type.log2_align == rhs.inner_type.log2_align,
        "Type alignment mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.log2_align, rhs.inner_type.log2_align
    );
    return u_memeql(u_load(lhs).ref.as_const, u_load(rhs).ref.as_const);
}
fn_((u_neq(u_V$raw lhs, u_V$raw rhs))(bool)) { return !u_eql(lhs, rhs); };

fn_((u_eqlBy(u_V$raw lhs, u_V$raw rhs, u_EqlFn eqlFn))(bool)) {
    claim_assert_fmt(
        lhs.inner_type.size == rhs.inner_type.size,
        "Type size mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.size, rhs.inner_type.size
    );
    claim_assert_fmt(
        lhs.inner_type.log2_align == rhs.inner_type.log2_align,
        "Type alignment mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.log2_align, rhs.inner_type.log2_align
    );
    return eqlFn(u_load(lhs), u_load(rhs));
};
fn_((u_neqBy(u_V$raw lhs, u_V$raw rhs, u_EqlFn eqlFn))(bool)) { return !u_eqlBy(lhs, rhs, eqlFn); };

fn_((u_eqlCtx(u_V$raw lhs, u_V$raw rhs, u_EqlCtxFn eqlFn, u_V$raw ctx))(bool)) {
    claim_assert_fmt(
        lhs.inner_type.size == rhs.inner_type.size,
        "Type size mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.size, rhs.inner_type.size
    );
    claim_assert_fmt(
        lhs.inner_type.log2_align == rhs.inner_type.log2_align,
        "Type alignment mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.log2_align, rhs.inner_type.log2_align
    );
    return eqlFn(u_load(lhs), u_load(rhs), u_load(ctx));
};
fn_((u_neqCtx(u_V$raw lhs, u_V$raw rhs, u_EqlCtxFn eqlFn, u_V$raw ctx))(bool)) { return !u_eqlCtx(lhs, rhs, eqlFn, ctx); };

fn_((u_ord(u_V$raw lhs, u_V$raw rhs))(cmp_Ord)) {
    claim_assert_fmt(
        lhs.inner_type.size == rhs.inner_type.size,
        "Type size mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.size, rhs.inner_type.size
    );
    claim_assert_fmt(
        lhs.inner_type.log2_align == rhs.inner_type.log2_align,
        "Type alignment mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.log2_align, rhs.inner_type.log2_align
    );
    return u_memord(u_load(lhs).ref.as_const, u_load(rhs).ref.as_const);
};
fn_((u_eq(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) == cmp_Ord_eq; };
fn_((u_ne(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) != cmp_Ord_eq; };
fn_((u_lt(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) == cmp_Ord_lt; };
fn_((u_gt(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) == cmp_Ord_gt; };
fn_((u_le(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) <= cmp_Ord_eq; };
fn_((u_ge(u_V$raw lhs, u_V$raw rhs))(bool)) { return u_ord(lhs, rhs) >= cmp_Ord_eq; };

fn_((u_ordBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(cmp_Ord)) {
    claim_assert_fmt(
        lhs.inner_type.size == rhs.inner_type.size,
        "Type size mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.size, rhs.inner_type.size
    );
    claim_assert_fmt(
        lhs.inner_type.log2_align == rhs.inner_type.log2_align,
        "Type alignment mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.log2_align, rhs.inner_type.log2_align
    );
    return ordFn(u_load(lhs), u_load(rhs));
};
fn_((u_eqBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) == cmp_Ord_eq; };
fn_((u_neBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) != cmp_Ord_eq; };
fn_((u_ltBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) == cmp_Ord_lt; };
fn_((u_gtBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) == cmp_Ord_gt; };
fn_((u_leBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) <= cmp_Ord_eq; };
fn_((u_geBy(u_V$raw lhs, u_V$raw rhs, u_OrdFn ordFn))(bool)) { return u_ordBy(lhs, rhs, ordFn) >= cmp_Ord_eq; };

fn_((u_ordCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(cmp_Ord)) {
    claim_assert_fmt(
        lhs.inner_type.size == rhs.inner_type.size,
        "Type size mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.size, rhs.inner_type.size
    );
    claim_assert_fmt(
        lhs.inner_type.log2_align == rhs.inner_type.log2_align,
        "Type alignment mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.log2_align, rhs.inner_type.log2_align
    );
    return ordFn(u_load(lhs), u_load(rhs), u_load(ctx));
};
fn_((u_eqCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) == cmp_Ord_eq; };
fn_((u_neCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) != cmp_Ord_eq; };
fn_((u_ltCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) == cmp_Ord_lt; };
fn_((u_gtCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) == cmp_Ord_gt; };
fn_((u_leCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) <= cmp_Ord_eq; };
fn_((u_geCtx(u_V$raw lhs, u_V$raw rhs, u_OrdCtxFn ordFn, u_V$raw ctx))(bool)) { return u_ordCtx(lhs, rhs, ordFn, ctx) >= cmp_Ord_eq; };

fn_((u_ordApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(cmp_Ord)) {
    claim_assert_fmt(
        lhs.inner_type.size == rhs.inner_type.size,
        "Type size mismatch: lhs({:uz}) != rhs({:uz})",
        lhs.inner_type.size, rhs.inner_type.size
    );
    return ordApxFn(u_load(lhs), u_load(rhs), u_load(threshold), mode);
};
fn_((u_eqApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxBy(lhs, rhs, threshold, mode, ordApxFn) == cmp_Ord_eq;
};
fn_((u_neApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxBy(lhs, rhs, threshold, mode, ordApxFn) != cmp_Ord_eq;
};
fn_((u_ltApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxBy(lhs, rhs, threshold, mode, ordApxFn) == cmp_Ord_lt;
};
fn_((u_gtApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxBy(lhs, rhs, threshold, mode, ordApxFn) == cmp_Ord_gt;
};
fn_((u_leApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxBy(lhs, rhs, threshold, mode, ordApxFn) <= cmp_Ord_eq;
};
fn_((u_geApxBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, cmp_ApxMode mode, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxBy(lhs, rhs, threshold, mode, ordApxFn) >= cmp_Ord_eq;
};

fn_((u_ordApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(cmp_Ord)) {
    return u_ordApxBy(lhs, rhs, threshold, cmp_ApxMode_abs, ordApxFn);
};
fn_((u_eqApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxAbsBy(lhs, rhs, threshold, ordApxFn) == cmp_Ord_eq;
};
fn_((u_neApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxAbsBy(lhs, rhs, threshold, ordApxFn) != cmp_Ord_eq;
};
fn_((u_ltApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxAbsBy(lhs, rhs, threshold, ordApxFn) == cmp_Ord_lt;
};
fn_((u_gtApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxAbsBy(lhs, rhs, threshold, ordApxFn) == cmp_Ord_gt;
};
fn_((u_leApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxAbsBy(lhs, rhs, threshold, ordApxFn) <= cmp_Ord_eq;
};
fn_((u_geApxAbsBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxAbsBy(lhs, rhs, threshold, ordApxFn) >= cmp_Ord_eq;
};

fn_((u_ordApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(cmp_Ord)) {
    return u_ordApxBy(lhs, rhs, threshold, cmp_ApxMode_rel, ordApxFn);
};
fn_((u_eqApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxRelBy(lhs, rhs, threshold, ordApxFn) == cmp_Ord_eq;
};
fn_((u_neApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxRelBy(lhs, rhs, threshold, ordApxFn) != cmp_Ord_eq;
};
fn_((u_ltApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxRelBy(lhs, rhs, threshold, ordApxFn) == cmp_Ord_lt;
};
fn_((u_gtApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxRelBy(lhs, rhs, threshold, ordApxFn) == cmp_Ord_gt;
};
fn_((u_leApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxRelBy(lhs, rhs, threshold, ordApxFn) <= cmp_Ord_eq;
};
fn_((u_geApxRelBy(u_V$raw lhs, u_V$raw rhs, u_V$raw threshold, u_OrdApxFn ordApxFn))(bool)) {
    return u_ordApxRelBy(lhs, rhs, threshold, ordApxFn) >= cmp_Ord_eq;
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_meta__included */
