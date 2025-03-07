/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-29 (date of creation)
 * @updated 2025-02-01 (date of last update)
 * @version v0.1-alpha.3
 * @ingroup dasae-headers(dh)/meta
 * @prefix
 *
 * @brief
 * @details
 */

#ifndef META_COMMON_INCLUDED
#define META_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"

/*========== Meta Utilities =================================================*/

/* Generic type */
typedef anyptr_const Ptr_const;
typedef anyptr       Ptr;
#define Ptr_const$(T) TYPE__Ptr_const$(T)
#define Ptr$(T)       TYPE__Ptr$(T)
#define use_Ptr$(T)   GEN__use_Ptr$(T)
extern Ptr Ptr_constCast(Ptr_const);

typedef struct Sli_const Sli_const;
typedef union Sli        Sli;
#define Sli_const$(T)                             TYPE_UNNAMED__Sli_const$(T)
#define Sli$(T)                                   TYPE_UNNAMED__Sli$(T)
#define use_Sli$(T)                               GEN__use_Sli$(T)
#define decl_Sli$(T)                              GEN__decl_Sli$(T)
#define impl_Sli$(T)                              GEN__impl_Sli$(T)
#define Sli_asNamed$(T_NamedSli, var_unnamed_sli) OP__Sli_asNamed$(T_NamedSli, var_unnamed_sli)
extern Sli          Sli_constCast(Sli_const);
extern anyptr_const Sli_rawAt(TypeInfo, anyptr_const, usize, usize);
extern anyptr       Sli_rawAt_mut(TypeInfo, anyptr, usize, usize);
extern anyptr_const Sli_rawSlice(TypeInfo, anyptr_const, usize, usize, usize);
extern anyptr       Sli_rawSlice_mut(TypeInfo, anyptr, usize, usize, usize);

#define Sli_from(var_ptr, val_len)                 OP__Sli_from(var_ptr, val_len)
#define Sli_from$(T, var_ptr, val_len...)          OP__Sli_from$(T, var_ptr, val_len)
#define Sli_range(var_ptr, val_begin, val_end)     OP__Sli_range(var_ptr, val_begin, val_end)
#define Sli_range$(T, var_ptr, val_begin, val_end) OP__Sli_range$(T, var_ptr, val_begin, val_end)
#define Sli_arr(var_arr...)                        OP__Sli_arr(var_arr)
#define Sli_arr$(T, var_arr...)                    OP__Sli_arr$(T, var_arr)

#define Sli_assign(var_dst_ptr, var_src) OP__Sli_assign(pp_uniqTok(dst_ptr), pp_uniqTok(src), var_dst_ptr, var_src)

#define Sli_at(var_self, usize_index)               OP__Sli_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Sli_slice(var_self, usize_begin, usize_end) OP__Sli_slice(pp_uniqTok(self), pp_uniqTok(begin), pp_uniqTok(end), var_self, usize_begin, usize_end)
#define Sli_prefix(var_self, usize_end)             OP__Sli_prefix(pp_uniqTok(self), var_self, usize_end)
#define Sli_suffix(var_self, usize_begin)           OP__Sli_suffix(pp_uniqTok(self), var_self, usize_begin)

/* Iterator support with scope (similar to Zig's for loops over slices) */
#define for_slice(var_sli, _Iter_item)                          SYN__for_slice(pp_uniqTok(sli), pp_uniqTok(i), var_sli, _Iter_item)
#define for_slice_indexed(var_sli, _Iter_item, _Iter_index)     SYN__for_slice_indexed(pp_uniqTok(sli), var_sli, _Iter_item, _Iter_index)
#define for_slice_rev(var_sli, _Iter_item)                      SYN__for_slice_rev(pp_uniqTok(sli), pp_uniqTok(i), var_sli, _Iter_item)
#define for_slice_rev_indexed(var_sli, _Iter_item, _Iter_index) SYN__for_slice_rev_indexed(pp_uniqTok(sli), var_sli, _Iter_item, _Iter_index)

/* Any type */
typedef struct AnyType AnyType;
// #define anyType(val_addr...) IMPL_anyType(val_addr)
#define anyPtr(var_ptr...) OP__anyPtr(var_ptr)
#define anySli(var_sli...) OP__anySli(var_sli)

/* Meta types */
typedef struct meta_Ptr_const meta_Ptr_const;
typedef union meta_Ptr        meta_Ptr;
typedef struct meta_Sli_const meta_Sli_const;
typedef union meta_Sli        meta_Sli;
extern meta_Ptr               meta_Ptr_constCast(meta_Ptr_const);
extern meta_Sli               meta_Sli_constCast(meta_Sli_const);
#define meta_refPtr(var_ptr...)               OP__meta_refPtr(var_ptr)
#define meta_refSli(var_ptr...)               OP__meta_refSli(var_ptr)
#define meta_refPtr_const(var_ptr...)         OP__meta_refPtr_const(var_ptr)
#define meta_refSli_const(var_ptr...)         OP__meta_refSli_const(var_ptr)
#define meta_cast$(TDest, var_meta...)        OP__meta_cast$(TDest, var_meta)
#define meta_castPtr$(TDest, var_meta_ptr...) OP__meta_castPtr$(TDest, var_meta_ptr)
#define meta_castSli$(TDest, var_meta_sli...) OP__meta_castSli$(TDest, var_meta_sli)
#define meta_ptrToAny(var_meta_ptr...)        OP__meta_ptrToAny(var_meta_ptr)
#define meta_sliToAny(var_meta_sli...)        OP__meta_sliToAny(var_meta_sli)

// extern void meta_setSli(meta_Sli mem, meta_Ptr val);
// extern void meta_copySli(meta_Sli dst, meta_Sli src);
// extern void meta_moveSli(meta_Sli dst, meta_Sli src);

/* Implementation generic type */

#define TYPE__Ptr_const$(T) const T*
#define TYPE__Ptr$(T)       T*
#define GEN__use_Ptr$(T)                            \
    typedef Ptr_const$(T) pp_join($, Ptr_const, T); \
    typedef Ptr$(T) pp_join($, Ptr, T)

struct Sli_const {
    Ptr_const ptr;
    usize     len;
};
union Sli {
    Sli_const as_const;
    struct {
        Ptr   ptr;
        usize len;
    };
};

#define TYPE_UNNAMED__Sli_const$(T) \
    struct {                        \
        Ptr_const$(T) ptr;          \
        usize len;                  \
    }
#define TYPE_UNNAMED__Sli$(T)   \
    union {                     \
        Sli_const$(T) as_const; \
        struct {                \
            Ptr$(T) ptr;        \
            usize len;          \
        };                      \
    }
#define GEN__use_Sli$(T) \
    decl_Sli$(T);        \
    impl_Sli$(T)
#define GEN__decl_Sli$(T)                                             \
    typedef struct pp_join($, Sli_const, T) pp_join($, Sli_const, T); \
    typedef union pp_join($, Sli, T) pp_join($, Sli, T)
#define GEN__impl_Sli$(T)                  \
    struct pp_join($, Sli_const, T) {      \
        Ptr_const$(T) ptr;                 \
        usize len;                         \
    };                                     \
    union pp_join($, Sli, T) {             \
        pp_join($, Sli_const, T) as_const; \
        struct {                           \
            Ptr$(T) ptr;                   \
            usize len;                     \
        };                                 \
    }
#define OP__Sli_asNamed$(T_NamedSli, var_unnamed_sli) eval({                                                \
    let _unnamed_sli = var_unnamed_sli;                                                                     \
    claim_assert_static(sizeOf(T_NamedSli) == sizeOf(TypeOf(_unnamed_sli)));                                \
    claim_assert_static(alignOf(T_NamedSli) == alignOf(TypeOf(_unnamed_sli)));                              \
    claim_assert_static(hasField(TypeOf(_unnamed_sli), len));                                               \
    claim_assert_static(isSameType$(FieldTypeOf(T_NamedSli, len), FieldTypeOf(TypeOf(_unnamed_sli), len))); \
    claim_assert_static(hasField(TypeOf(_unnamed_sli), ptr));                                               \
    claim_assert_static(isSameType$(FieldTypeOf(T_NamedSli, ptr), FieldTypeOf(TypeOf(_unnamed_sli), ptr))); \
    eval_return(*(T_NamedSli*)&_unnamed_sli);                                                               \
})

#define OP__Sli_from(var_ptr, val_len)        { .ptr = (var_ptr), .len = (val_len) }
#define OP__Sli_from$(T, var_ptr, val_len...) eval({ \
    let _ptr = var_ptr;                              \
    debug_assert_nonnull(_ptr);                      \
    eval_return make$(                               \
        T,                                           \
        .ptr = _ptr,                                 \
        .len = (val_len)                             \
    );                                               \
})

#define OP__Sli_range(var_ptr, val_begin, val_end)     { .ptr = (var_ptr) + (val_begin), .len = (val_end) - (val_begin) }
#define OP__Sli_range$(T, var_ptr, val_begin, val_end) eval({                              \
    let         _ptr   = var_ptr;                                                          \
    const usize _begin = val_begin;                                                        \
    const usize _end   = val_end;                                                          \
    debug_assert_nonnull(_ptr);                                                            \
    debug_assert_fmt(_begin < _end, "Invalid range (begin: %zu, end: %zu)", _begin, _end); \
    eval_return make$(                                                                     \
        T,                                                                                 \
        .ptr = _ptr + _begin,                                                              \
        .len = _end - _begin                                                               \
    );                                                                                     \
})
#define OP__Sli_arr(var_arr...)     { .ptr = (var_arr), .len = countOf(var_arr) }
#define OP__Sli_arr$(T, var_arr...) eval({ \
    let _arr = (var_arr);                  \
    debug_assert_nonnull(_arr);            \
    eval_return make$(pp_join($, Sli, T)){ \
        .ptr = _arr,                       \
        .len = countOf(_arr)               \
    };                                     \
})

#define OP__Sli_assign(__dst_ptr, __src, var_dst_ptr, var_src) eval({ \
    let __dst_ptr    = var_dst_ptr;                                   \
    let __src        = var_src;                                       \
    (__dst_ptr)->ptr = (__src)->ptr;                                  \
    (__dst_ptr)->len = (__src)->len;                                  \
    eval_return*(__dst_ptr);                                          \
})

#define OP__Sli_at(__self, __index, var_self, usize_index) eval({                     \
    let         __self  = var_self;                                                   \
    const usize __index = usize_index;                                                \
    debug_assert_fmt((__index) < (__self).len, "%llu < %llu", __index, (__self).len); \
    eval_return(&(__self).ptr[__index]);                                              \
})

#define OP__Sli_slice(__self, __begin, __end, var_self, usize_begin, usize_end) eval({                              \
    let   __self  = &(var_self);                                                                                    \
    usize __begin = usize_begin;                                                                                    \
    usize __end   = usize_end;                                                                                      \
    eval_return make$(                                                                                              \
        TypeOf(*_sli),                                                                                              \
        .ptr = Sli_rawSlice_mut(typeInfo$(TypeOf(*((__self)->ptr))), (__self)->ptr, (__self)->len, __begin, __end), \
        .len = _end - _begin                                                                                        \
    );                                                                                                              \
})

#define OP__Sli_prefix(__self, var_sli, usize_end) eval({ \
    let __self = var_sli;                                 \
    eval_return Sli_slice(__self, 0, usize_end);          \
})

#define OP__Sli_suffix(__self, var_sli, usize_begin) eval({   \
    let __self = var_sli;                                     \
    eval_return Sli_slice(__self, usize_begin, (__self).len); \
})

#define SYN__for_slice(__sli, __i, var_sli, _Iter_item)                                 \
    scope_with(let __sli = (var_sli)) for (usize __i = 0; (__i) < (__sli).len; ++(__i)) \
        scope_with(let _Iter_item = Sli_at(__sli, __i))

#define SYN__for_slice_indexed(__sli, var_sli, _Iter_item, _Iter_index)                                         \
    scope_with(let __sli = (var_sli)) for (usize _Iter_index = 0; (_Iter_index) < (__sli).len; ++(_Iter_index)) \
        scope_with(let _Iter_item = Sli_at(__sli, _Iter_index))

#define SYN__for_slice_rev(__sli, __i, var_sli, _Iter_item)                       \
    scope_with(let __sli = (var_sli)) for (usize __i = (__sli).len; (__i)-- > 0;) \
        scope_with(let _Iter_item = Sli_at(__sli, __i))

#define SYN__for_slice_rev_indexed(__sli, var_sli, _Iter_item, _Iter_index)                       \
    scope_with(let __sli = (var_sli)) for (usize _Iter_index = (__sli).len; (_Iter_index)-- > 0;) \
        scope_with(let _Iter_item = Sli_at(__sli, _Iter_index))

// clang-format off
use_Opt$(Ptr_const); use_Opt$(Ptr);
use_Err$(Ptr_const); use_Err$(Ptr);

use_Opt$(Sli_const); use_Opt$(Sli);
use_Err$(Sli_const); use_Err$(Sli);
// clang-format on

/* Implementation any type */

struct AnyType {
    TypeInfo type;
    anyptr   ctx;
    usize    len;
};

// #define IMPL_anyType(val_addr...) eval({\
//     var _addr = val_addr;                 \
//     debug_assert_nonnull(_addr);          \
//     eval_return(AnyType){                 \
//         .ctx  = (void*)_addr,             \
//         .len  = 1,                        \
//         .type = typeInfo$(TypeOf(*_addr)), \
//     };                                    \
//})

#define OP__anyPtr(var_ptr...) eval({                                                                                                  \
    var _ptr = var_ptr;                                                                                                                \
    claim_assert_static_msg(!isSameType$(TypeOf(_ptr), meta_Sli), "`meta_Sli` is not compatible with `anyPtr`. Use `meta_sliToAny`."); \
    claim_assert_static_msg(!isSameType$(TypeOf(_ptr), meta_Ptr), "`meta_Ptr` is not compatible with `anyPtr`. Use `meta_ptrToAny`."); \
    debug_assert_nonnull(_ptr);                                                                                                        \
    eval_return((AnyType){                                                                                                             \
        .type = typeInfo$(TypeOf(*_ptr)),                                                                                              \
        .ctx  = (void*)_ptr,                                                                                                           \
        .len  = 1,                                                                                                                     \
    });                                                                                                                                \
})

#define OP__anySli(var_sli...) eval({                                                                                                  \
    var _sli = var_sli;                                                                                                                \
    claim_assert_static_msg(!isSameType$(TypeOf(_sli), meta_Ptr), "`meta_Ptr` is not compatible with `anySli`. Use `meta_ptrToAny`."); \
    claim_assert_static_msg(!isSameType$(TypeOf(_sli), meta_Sli), "`meta_Sli` is not compatible with `anySli`. Use `meta_sliToAny`."); \
    debug_assert_nonnull(_sli.ptr);                                                                                                    \
    eval_return((AnyType){                                                                                                             \
        .type = typeInfo$(TypeOf(*_sli.ptr)),                                                                                          \
        .ctx  = (void*)_sli.ptr,                                                                                                       \
        .len  = _sli.len,                                                                                                              \
    });                                                                                                                                \
})

/* Implementation meta types */
struct meta_Ptr_const {
    TypeInfo     type; // Type info first
    anyptr_const addr; // Then address
};

union meta_Ptr {
    struct {
        TypeInfo type; // Type info first
        anyptr   addr; // Then address
    };
    meta_Ptr_const as_const;
};

struct meta_Sli_const {
    TypeInfo     type; // Type info first
    anyptr_const addr; // Then address
    usize        len;  // Then length
};

union meta_Sli {
    struct {
        TypeInfo type; // Type info first
        anyptr   addr; // Then address
        usize    len;  // Then length
    };
    meta_Sli_const as_const;
};

#define OP__meta_refPtr(var_ptr...) eval({ \
    let _ptr = var_ptr;                    \
    eval_return((meta_Ptr){                \
        .type = typeInfo$(TypeOf(*_ptr)),  \
        .addr = _ptr,                      \
    });                                    \
})

#define OP__meta_refSli(var_sli...) eval({    \
    let _sli = var_sli;                       \
    eval_return((meta_Sli){                   \
        .type = typeInfo$(TypeOf(*_sli.ptr)), \
        .addr = _sli.ptr,                     \
        .len  = _sli.len,                     \
    });                                       \
})

#define OP__meta_refPtr_const(var_ptr...) eval({ \
    let _ptr = var_ptr;                          \
    eval_return((meta_Ptr_const){                \
        .type = typeInfo$(TypeOf(*_ptr)),        \
        .addr = _ptr,                            \
    });                                          \
})

#define OP__meta_refSli_const(var_sli...) eval({  \
    let _sli = var_sli;                           \
    eval_return((meta_Sli_const){                 \
        .ptr = {                                  \
            .type = typeInfo$(TypeOf(*_sli.ptr)), \
            .addr = _sli.ptr,                     \
        },                                        \
        .len = _sli.len,                          \
    });                                           \
})

#define OP__meta_cast$(TDest, var_meta...) eval({                                                                               \
    var _meta = var_meta;                                                                                                       \
    claim_assert_static_msg(isSameType$(TypeOf(_meta), meta_Ptr) || isSameType$(TypeOf(_meta), meta_Sli), "Invalid meta type"); \
    eval_return(*((TDest*)&_meta.addr));                                                                                        \
})

#define OP__meta_castPtr$(TDest, var_meta_ptr...) eval({                               \
    let _ptr = var_meta_ptr;                                                           \
    claim_assert_static_msg(isSameType$(TypeOf(_ptr), meta_Ptr), "Invalid meta type"); \
    eval_return((TDest)_ptr.addr);                                                     \
})

#define OP__meta_castSli$(TDest, var_meta_sli...) eval({                               \
    let _sli = var_meta_sli;                                                           \
    claim_assert_static_msg(isSameType$(TypeOf(_sli), meta_Sli), "Invalid meta type"); \
    eval_return((TDest){ .ptr = _sli.addr, .len = _sli.len });                         \
})

#define OP__meta_ptrToAny(var_meta_ptr) eval({                                         \
    let _ptr = var_meta_ptr;                                                           \
    claim_assert_static_msg(isSameType$(TypeOf(_ptr), meta_Ptr), "Invalid meta type"); \
    eval_return(AnyType){ .type = _ptr.type, .ctx = _ptr.addr, .len = 1 };             \
})

#define OP__meta_sliToAny(var_meta_sli) eval({                                         \
    let _sli = var_meta_sli;                                                           \
    claim_assert_static_msg(isSameType$(TypeOf(_sli), meta_Sli), "Invalid meta type"); \
    eval_return(*(AnyType*)&_sli);                                                     \
})

// clang-format off
use_Opt$(meta_Ptr_const); use_Opt$(meta_Ptr);
use_Err$(meta_Ptr_const); use_Err$(meta_Ptr);

use_Opt$(meta_Sli_const); use_Opt$(meta_Sli);
use_Err$(meta_Sli_const); use_Err$(meta_Sli);

/* builtin types */
use_Ptr$(u8); use_Ptr$(u16); use_Ptr$(u32); use_Ptr$(u64); use_Ptr$(usize);
use_Sli$(u8); use_Sli$(u16); use_Sli$(u32); use_Sli$(u64); use_Sli$(usize);
use_Opt$(u8); use_Opt$(u16); use_Opt$(u32); use_Opt$(u64); use_Opt$(usize);
use_Err$(u8); use_Err$(u16); use_Err$(u32); use_Err$(u64); use_Err$(usize);

use_Ptr$(i8); use_Ptr$(i16); use_Ptr$(i32); use_Ptr$(i64); use_Ptr$(isize);
use_Sli$(i8); use_Sli$(i16); use_Sli$(i32); use_Sli$(i64); use_Sli$(isize);
use_Opt$(i8); use_Opt$(i16); use_Opt$(i32); use_Opt$(i64); use_Opt$(isize);
use_Err$(i8); use_Err$(i16); use_Err$(i32); use_Err$(i64); use_Err$(isize);

use_Ptr$(f32); use_Ptr$(f64);
use_Sli$(f32); use_Sli$(f64);
use_Opt$(f32); use_Opt$(f64);
use_Err$(f32); use_Err$(f64);

use_Ptr$(bool); use_Ptr$(char);
use_Sli$(bool); use_Sli$(char);
use_Opt$(bool); use_Opt$(char);
use_Err$(bool); use_Err$(char);
// clang-format on

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* META_COMMON_INCLUDED */
