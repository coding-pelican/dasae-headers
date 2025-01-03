/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-29 (date of creation)
 * @updated 2025-01-02 (date of last update)
 * @version v0.1
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
#define PtrConst$(T)  IMPL_PtrConst$(T)
#define Ptr$(T)       IMPL_Ptr$(T)
#define using_Ptr$(T) IMPL_using_Ptr$(T)
#define decl_Ptr$(T)  IMPL_decl_Ptr$(T)
#define impl_Ptr$(T)  IMPL_impl_Ptr$(T)

#define SliConst$(T)  IMPL_SliConst$(T)
#define Sli$(T)       IMPL_Sli$(T)
#define using_Sli$(T) IMPL_using_Sli$(T)
#define decl_Sli$(T)  IMPL_decl_Sli$(T)
#define impl_Sli$(T)  IMPL_impl_Sli$(T)

typedef const anyptr PtrConst;
typedef anyptr       Ptr;
extern Ptr           Ptr_constCast(PtrConst);

typedef struct SliConst SliConst;
typedef union Sli       Sli;
extern Sli              Sli_constCast(SliConst);
extern const anyptr     Sli_rawAt(TypeInfo, const anyptr, usize, usize);
extern anyptr           Sli_rawAt_mut(TypeInfo, anyptr, usize, usize);
extern const anyptr     Sli_rawSlice(TypeInfo, const anyptr, usize, usize, usize);
extern anyptr           Sli_rawSlice_mut(TypeInfo, anyptr, usize, usize, usize);

#define Sli_from(var_ptr, val_len)                 IMPL_Sli_from(var_ptr, val_len)
#define Sli_from$(T, var_ptr, val_len)             IMPL_Sli_from$(T, var_ptr, val_len)
#define Sli_range(var_ptr, val_begin, val_end)     IMPL_Sli_range(var_ptr, val_begin, val_end)
#define Sli_range$(T, var_ptr, val_begin, val_end) IMPL_Sli_range$(T, var_ptr, val_begin, val_end)
#define Sli_arr(var_arr...)                        IMPL_Sli_arr(var_arr)
#define Sli_arr$(T, var_arr...)                    IMPL_Sli_arr$(T, var_arr)
#define Sli_at(var_sli, val_index)                 IMPL_Sli_at(var_sli, val_index)
#define Sli_slice(var_sli, val_begin, val_end)     IMPL_Sli_slice(var_sli, val_begin, val_end)
#define Sli_prefix(var_sli, val_end)               IMPL_Sli_prefix(var_sli, val_end)
#define Sli_suffix(var_sli, val_begin)             IMPL_Sli_suffix(var_sli, val_begin)

/* Iterator support with scope (similar to Zig's for loops over slices) */
#define for_slice(var_sli, var_item)                    IMPL_for_slice(var_sli, var_item)
#define for_slice_indexed(var_sli, var_item, var_index) IMPL_for_slice_indexed(var_sli, var_item, var_index)

/* Any type */
typedef struct AnyType AnyType;
#define AnyType(var_ptr...) IMPL_AnyType(var_ptr)
#define AnyPtr(var_ptr...)  IMPL_AnyPtr(var_ptr)
#define AnySli(var_sli...)  IMPL_AnySli(var_sli)

/* Meta types */
typedef struct meta_PtrConst meta_PtrConst;
typedef union meta_Ptr       meta_Ptr;
typedef struct meta_SliConst meta_SliConst;
typedef union meta_Sli       meta_Sli;
extern meta_Ptr              meta_Ptr_constCast(meta_PtrConst);
extern meta_Sli              meta_Sli_constCast(meta_SliConst);
#define meta_ptr(var_ptr...)    IMPL_meta_ptr(var_ptr)
#define meta_sli(var_ptr...)    IMPL_meta_sli(var_ptr)
#define meta_castPtr(T, ptr...) IMPL_meta_castPtr(T, ptr)
#define meta_castSli(T, sli...) IMPL_meta_castSli(T, sli)

/* Implementation generic type */

#define IMPL_PtrConst$(T) const T*
#define IMPL_Ptr$(T)      T*

#define IMPL_using_Ptr$(T)                        \
    typedef PtrConst$(T) pp_join($, PtrConst, T); \
    typedef Ptr$(T) pp_join($, Ptr, T)

#define IMPL_SliConst$(T) \
    struct {              \
        PtrConst$(T) ptr; \
        usize len;        \
    }
#define IMPL_Sli$(T)           \
    union {                    \
        SliConst$(T) as_const; \
        struct {               \
            Ptr$(T) ptr;       \
            usize len;         \
        };                     \
    }

#define IMPL_using_Sli$(T) \
    decl_Sli$(T);          \
    impl_Sli$(T)

#define IMPL_decl_Sli$(T)                                           \
    typedef struct pp_join($, SliConst, T) pp_join($, SliConst, T); \
    typedef union pp_join($, Sli, T) pp_join($, Sli, T)

#define IMPL_impl_Sli$(T)                 \
    struct pp_join($, SliConst, T) {      \
        PtrConst$(T) ptr;                 \
        usize len;                        \
    };                                    \
    union pp_join($, Sli, T) {            \
        pp_join($, SliConst, T) as_const; \
        struct {                          \
            Ptr$(T) ptr;                  \
            usize len;                    \
        };                                \
    }

struct SliConst {
    PtrConst ptr;
    usize    len;
};

union Sli {
    SliConst as_const;
    struct {
        Ptr   ptr;
        usize len;
    };
};

#define IMPL_Sli_from(var_ptr, val_len)     { .ptr = (var_ptr), .len = (val_len) }
#define IMPL_Sli_from$(T, var_ptr, val_len) ({ \
    let IMPL__ptr = var_ptr;                   \
    claim_assert_nonnull(_ptr);                \
    (T){                                       \
        .ptr = _ptr,                           \
        .len = (val_len)                       \
    };                                         \
})

#define IMPL_Sli_range(var_ptr, val_begin, val_end)     { .ptr = (var_ptr) + (val_begin), .len = (val_end) - (val_begin) }
#define IMPL_Sli_range$(T, var_ptr, val_begin, val_end) ({                                 \
    let   _ptr   = var_ptr;                                                                \
    usize _begin = val_begin;                                                              \
    usize _end   = val_end;                                                                \
    claim_assert_nonnull(_ptr);                                                            \
    claim_assert_fmt(_begin < _end, "Invalid range (begin: %zu, end: %zu)", _begin, _end); \
    (T){                                                                                   \
        .ptr = _ptr + _begin,                                                              \
        .len = _end - _begin                                                               \
    };                                                                                     \
})
#define IMPL_Sli_arr(var_arr...)     { .ptr = (var_arr), .len = countOf(var_arr) }
#define IMPL_Sli_arr$(T, var_arr...) ({ \
    let _arr = (var_arr);               \
    claim_assert_nonnull(_arr);         \
    (pp_join($, Sli, T)){               \
        .ptr = _arr,                    \
        .len = countOf(_arr)            \
    };                                  \
})

#define IMPL_Sli_at(var_sli, val_index) ({                                                      \
    let _sli = var_sli;                                                                         \
    (TypeOf(_sli.ptr)) Sli_rawAt(typeInfo(TypeOf(*(_sli.ptr))), _sli.ptr, _sli.len, val_index); \
})

#define IMPL_Sli_slice(var_sli, val_begin, val_end) ({                                        \
    let   _sli   = var_sli;                                                                   \
    usize _begin = val_begin;                                                                 \
    usize _end   = val_end;                                                                   \
    (TypeOf(_sli)){                                                                           \
        .ptr = Sli_rawSlice(typeInfo(TypeOf(*(_sli.ptr))), _sli.ptr, _sli.len, _begin, _end), \
        .len = _end - _begin                                                                  \
    };                                                                                        \
})

#define IMPL_Sli_prefix(var_sli, val_end) ({ \
    let _sli = var_sli;                      \
    Sli_slice(_sli, 0, val_end);             \
})

#define IMPL_Sli_suffix(var_sli, val_begin) ({ \
    let _sli = var_sli;                        \
    Sli_slice(_sli, val_begin, _sli.len);      \
})

#define IMPL_for_slice(var_sli, var_item)        \
    for (usize _i = 0; _i < (var_sli).len; ++_i) \
    scope_with(let var_item = Sli_at(var_sli, _i))

#define IMPL_for_slice_indexed(var_sli, var_item, var_index)              \
    for (usize var_index = 0; (var_index) < (var_sli).len; ++(var_index)) \
    scope_with(let var_item = Sli_at(var_sli, var_index))

using_Opt$(PtrConst);
using_Opt$(Ptr);
using_Err$(PtrConst);
using_Err$(Ptr);

using_Opt$(SliConst);
using_Opt$(Sli);
using_Err$(SliConst);
using_Err$(Sli);


/* Implementation any type */
struct AnyType {
    anyptr   ctx;
    TypeInfo type;
};

#define IMPL_AnyType(var_ptr...) ({          \
    let _var_ptr = var_ptr;                  \
    claim_assert_nonnull(_var_ptr);          \
    (AnyType){                               \
        .ctx  = _var_ptr,                    \
        .type = typeInfo(TypeOf(*_var_ptr)), \
    };                                       \
})

#define IMPL_AnyPtr(var_ptr...) \
    ((AnyType){ .ctx = &(var_ptr), .type = typeInfo(TypeOf(*(var_ptr))) })

#define IMPL_AnySli(var_sli...) \
    ((AnyType){ .ctx = &(var_sli), .type = typeInfo(TypeOf(*((var_sli).ptr))) })

/* Implementation meta types */
struct meta_PtrConst {
    const anyptr addr;
    TypeInfo     type;
};

union meta_Ptr {
    meta_PtrConst as_const;
    struct {
        anyptr   addr;
        TypeInfo type;
    };
};

struct meta_SliConst {
    union {
        meta_PtrConst ptr;
        struct {
            const anyptr addr;
            TypeInfo     type;
        };
    };
    usize len;
};

union meta_Sli {
    meta_SliConst as_const;
    struct {
        union {
            meta_Ptr ptr;
            struct {
                anyptr   addr;
                TypeInfo type;
            };
        };
        usize len;
    };
};

#define IMPL_meta_ptr(var_ptr...) ({         \
    let _var_ptr = var_ptr;                  \
    (meta_Ptr){                              \
        .addr = _var_ptr,                    \
        .type = typeInfo(TypeOf(*_var_ptr)), \
    };                                       \
})

#define IMPL_meta_sli(var_sli...) ({                 \
    let _var_sli = var_sli;                          \
    (meta_Sli){                                      \
        .ptr = {                                     \
            .addr = _var_sli.ptr,                    \
            .type = typeInfo(TypeOf(*_var_sli.ptr)), \
        },                                           \
        .len = _var_sli.len,                         \
    };                                               \
})

#define IMPL_meta_castPtr(T, ptr...) ({ \
    let _ptr = ptr;                     \
    (T)((_ptr).addr);                   \
})

#define IMPL_meta_castSli(T, sli...) ({           \
    let _sli = sli;                               \
    (T){ .ptr = (_sli).addr, .len = (_sli).len }; \
})

using_Opt$(meta_PtrConst);
using_Opt$(meta_Ptr);
using_Opt$(meta_SliConst);
using_Opt$(meta_Sli);

using_Err$(meta_PtrConst);
using_Err$(meta_Ptr);
using_Err$(meta_SliConst);
using_Err$(meta_Sli);

/* builtin types */
using_Ptr$(u8);
using_Ptr$(u16);
using_Ptr$(u32);
using_Ptr$(u64);
using_Ptr$(usize);

using_Ptr$(i8);
using_Ptr$(i16);
using_Ptr$(i32);
using_Ptr$(i64);
using_Ptr$(isize);

using_Ptr$(f32);
using_Ptr$(f64);

using_Ptr$(bool);
using_Ptr$(char);

using_Sli$(u8);
using_Sli$(u16);
using_Sli$(u32);
using_Sli$(u64);
using_Sli$(usize);

using_Sli$(i8);
using_Sli$(i16);
using_Sli$(i32);
using_Sli$(i64);
using_Sli$(isize);

using_Sli$(f32);
using_Sli$(f64);

using_Sli$(bool);
using_Sli$(char);

using_Opt$(u8);
using_Opt$(u16);
using_Opt$(u32);
using_Opt$(u64);
using_Opt$(usize);

using_Opt$(i8);
using_Opt$(i16);
using_Opt$(i32);
using_Opt$(i64);
using_Opt$(isize);

using_Opt$(f32);
using_Opt$(f64);

using_Opt$(bool);
using_Opt$(char);

using_Err$(u8);
using_Err$(u16);
using_Err$(u32);
using_Err$(u64);
using_Err$(usize);

using_Err$(i8);
using_Err$(i16);
using_Err$(i32);
using_Err$(i64);
using_Err$(isize);

using_Err$(f32);
using_Err$(f64);

using_Err$(bool);
using_Err$(char);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* META_COMMON_INCLUDED */
