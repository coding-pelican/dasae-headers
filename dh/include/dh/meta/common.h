/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-29 (date of creation)
 * @updated 2024-12-29 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/meta
 * @prefix  meta
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
#define PtrConst$(T) const T*
#define Ptr$(T)      T*

#define using_Ptr$(T)                             \
    typedef PtrConst$(T) pp_join($, PtrConst, T); \
    typedef Ptr$(T) pp_join($, Ptr, T)

#define SliConst$(T)      \
    struct {              \
        PtrConst$(T) ptr; \
        usize len;        \
    }
#define Sli$(T)      \
    struct {         \
        Ptr$(T) ptr; \
        usize len;   \
    }

#define using_Sli$(T) \
    decl_Sli$(T);     \
    impl_Sli$(T)

#define decl_Sli$(T)                                                \
    typedef struct pp_join($, SliConst, T) pp_join($, SliConst, T); \
    typedef struct pp_join($, Sli, T) pp_join($, Sli, T)

#define impl_Sli$(T)                 \
    struct pp_join($, SliConst, T) { \
        PtrConst$(T) ptr;            \
        usize len;                   \
    };                               \
    struct pp_join($, Sli, T) {      \
        Ptr$(T) ptr;                 \
        usize len;                   \
    }

/* Any type */
typedef struct AnyType {
    anyptr   ctx;
    TypeInfo type;
} AnyType;

#define AnyPtr(var_ptr) \
    ((AnyType){ .ctx = &(var_ptr), .type = typeInfo(TypeOf(*(var_ptr))) })

#define AnySli(var_sli) \
    ((AnyType){ .ctx = &(var_sli), .type = typeInfo(TypeOf(*((var_sli).ptr))) })

/* Meta types */
typedef struct meta_PtrConst meta_PtrConst;
typedef union meta_Ptr       meta_Ptr;
typedef struct meta_SliConst meta_SliConst;
typedef union meta_Sli       meta_Sli;

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

using_Opt$(meta_PtrConst);
using_Opt$(meta_Ptr);
using_Opt$(meta_SliConst);
using_Opt$(meta_Sli);

using_Err$(meta_PtrConst);
using_Err$(meta_Ptr);
using_Err$(meta_SliConst);
using_Err$(meta_Sli);

#define meta_ptr(var_ptr...) ({      \
    let _var_ptr = var_ptr;          \
    (meta_Ptr){                      \
        .addr = _var_ptr,            \
        .type = typeInfo(*_var_ptr), \
    };                               \
})
#define meta_sli(var_sli...) ({              \
    let _var_sli = var_sli;                  \
    (meta_Sli){                              \
        .ptr = {                             \
            .addr = _var_sli.ptr,            \
            .type = typeInfo(*_var_sli.ptr), \
        },                                   \
        .len = _var_sli.len,                 \
    };                                       \
})
#define meta_castPtr(T, ptr...) ({ \
    let _ptr = ptr;                \
    (T)((_ptr).addr);              \
})
#define meta_castSli(T, sli...) ({                \
    let _sli = sli;                               \
    (T){ .ptr = (_sli).addr, .len = (_sli).len }; \
})

#define Sli_from(_ptr, _len)             { .ptr = (_ptr), .len = (_len) }
#define Sli_fromPtr(_ptr)                { .ptr = (_ptr), .len = 1 }
#define Sli_fromSli(_sli)                { .ptr = (_sli).ptr, .len = (_sli).len }
#define Sli_fromArr(_arr)                { .ptr = (_arr), .len = countOf(_arr) }
#define Sli_slice(_ptr, _begin, _end)    { .ptr = (_ptr) + (_begin), .len = (_end) - (_begin) }
#define Sli_subslice(_sli, _begin, _end) { .ptr = (_sli).ptr + (_begin), .len = (_end) - (_begin) }

/* Access operations with detailed assertions */
#define Sli_at(slice, index)                \
    ({                                      \
        var _slice = (slice);               \
        let _index = (index);               \
        debug_assert_nonnull((_slice).ptr); \
        debug_assert_fmt(                   \
            (_index) < (_slice).len,        \
            "Index out of bounds "          \
            "(index: %zu, len: %zu)",       \
            (usize)(_index),                \
            (_slice).len                    \
        );                                  \
        &(_slice).ptr[_index];              \
    })

/* Iterator support with scope (similar to Zig's for loops over slices) */
#define for_slice(slice, item)                 \
    for (usize _i = 0; _i < (slice).len; ++_i) \
    scope_with(let item = Sli_at(slice, _i))

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
