/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-29 (date of creation)
 * @updated 2025-01-02 (date of last update)
 * @version v0.1-alpha
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
#define Sli_at(var_sli, usize_index)               IMPL_Sli_at(var_sli, usize_index)
#define Sli_slice(var_sli, usize_begin, usize_end) IMPL_Sli_slice(var_sli, usize_begin, usize_end)
#define Sli_prefix(var_sli, val_end)               IMPL_Sli_prefix(var_sli, val_end)
#define Sli_suffix(var_sli, val_begin)             IMPL_Sli_suffix(var_sli, val_begin)

/* Iterator support with scope (similar to Zig's for loops over slices) */
#define for_slice(var_sli, var_item)                    IMPL_for_slice(var_sli, var_item)
#define for_slice_indexed(var_sli, var_item, var_index) IMPL_for_slice_indexed(var_sli, var_item, var_index)

/* Any type */
typedef struct AnyType AnyType;
// #define anyType(val_addr...) IMPL_anyType(val_addr)
#define anyPtr(var_ptr...) IMPL_anyPtr(var_ptr)
#define anySli(var_sli...) IMPL_anySli(var_sli)

/* Meta types */
typedef struct meta_PtrConst meta_PtrConst;
typedef union meta_Ptr       meta_Ptr;
typedef struct meta_SliConst meta_SliConst;
typedef union meta_Sli       meta_Sli;
extern meta_Ptr              meta_Ptr_constCast(meta_PtrConst);
extern meta_Sli              meta_Sli_constCast(meta_SliConst);
#define meta_refPtr(var_ptr...)          IMPL_meta_refPtr(var_ptr)
#define meta_refSli(var_ptr...)          IMPL_meta_refSli(var_ptr)
#define meta_refPtrConst(var_ptr...)     IMPL_meta_refPtrConst(var_ptr)
#define meta_refSliConst(var_ptr...)     IMPL_meta_refSliConst(var_ptr)
#define meta_cast$(TDest, var_meta...)   IMPL_meta_cast$(TDest, var_meta)
#define meta_castPtr$(TDest, var_ptr...) IMPL_meta_castPtr$(TDest, var_ptr)
#define meta_castSli$(TDest, var_sli...) IMPL_meta_castSli$(TDest, var_sli)

#define meta_copy
#define meta_move

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
    let _ptr = var_ptr;                        \
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

#define IMPL_Sli_at(var_self, usize_index) eval( \
    let   _self  = var_self;                     \
    usize _index = usize_index;                  \
    debug_assert(_index < _self.len);            \
    eval_return(&_self.ptr[_index]);             \
)

#define IMPL_Sli_slice(var_self, usize_begin, usize_end) ({                                   \
    let   _sli   = var_self;                                                                  \
    usize _begin = usize_begin;                                                               \
    usize _end   = usize_end;                                                                 \
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
    TypeInfo type;
    anyptr   ctx;
    usize    len;
};

// #define IMPL_anyType(val_addr...) eval(   \
//     var _addr = val_addr;                 \
//     debug_assert_nonnull(_addr);          \
//     eval_return(AnyType){                 \
//         .ctx  = (void*)_addr,             \
//         .len  = 1,                        \
//         .type = typeInfo(TypeOf(*_addr)), \
//     };                                    \
// )

#define IMPL_anyPtr(var_ptr...) eval(    \
    var _ptr = var_ptr;                  \
    debug_assert_nonnull(_ptr);          \
    eval_return(AnyType){                \
        .type = typeInfo(TypeOf(*_ptr)), \
        .ctx  = (void*)_ptr,             \
        .len  = 1,                       \
    };                                   \
)

#define IMPL_anyPtr(var_ptr...) eval(    \
    var _ptr = var_ptr;                  \
    debug_assert_nonnull(_ptr);          \
    eval_return(AnyType){                \
        .type = typeInfo(TypeOf(*_ptr)), \
        .ctx  = (void*)_ptr,             \
        .len  = 1,                       \
    };                                   \
)

#define IMPL_anySli(var_sli...) eval(        \
    var _sli = var_sli;                      \
    debug_assert_nonnull(_sli.ptr);          \
    eval_return(AnyType){                    \
        .type = typeInfo(TypeOf(*_sli.ptr)), \
        .ctx  = (void*)_sli.ptr,             \
        .len  = _sli.len,                    \
    };                                       \
)

/* Implementation meta types */
struct meta_PtrConst {
    TypeInfo     type; // Type info first
    const anyptr addr; // Then address
};

union meta_Ptr {
    struct {
        TypeInfo type; // Type info first
        anyptr   addr; // Then address
    };
    meta_PtrConst as_const;
};

struct meta_SliConst {
    TypeInfo     type; // Type info first
    const anyptr addr; // Then address
    usize        len;  // Then length
};

union meta_Sli {
    struct {
        TypeInfo type; // Type info first
        anyptr   addr; // Then address
        usize    len;  // Then length
    };
    meta_SliConst as_const;
};

#define IMPL_meta_refPtr(var_ptr...) eval( \
    let         _ptr = var_ptr;            \
    eval_return make(                      \
        meta_Ptr,                          \
        .type = typeInfo(TypeOf(*_ptr)),   \
        .addr = _ptr,                      \
    );                                     \
)

#define IMPL_meta_refSli(var_sli...) eval(   \
    let         _sli = var_sli;              \
    eval_return make(                        \
        meta_Sli,                            \
        .type = typeInfo(TypeOf(*_sli.ptr)), \
        .addr = _sli.ptr,                    \
        .len  = _sli.len,                    \
    );                                       \
)

#define IMPL_meta_refPtrConst(var_ptr...) eval( \
    let         _ptr = var_ptr;                 \
    eval_return make(                           \
        meta_PtrConst,                          \
        .type = typeInfo(TypeOf(*_ptr)),        \
        .addr = _ptr,                           \
    );                                          \
)

#define IMPL_meta_refSliConst(var_sli...) eval(  \
    let         _sli = var_sli;                  \
    eval_return make(                            \
        meta_SliConst,                           \
        .ptr = {                                 \
            .type = typeInfo(TypeOf(*_sli.ptr)), \
            .addr = _sli.ptr,                    \
        },                                       \
        .len = _sli.len,                         \
    );                                           \
)

#define IMPL_meta_cast$(TDest, var_meta...) eval( \
    var _meta = var_meta;                         \
    eval_return(*((TDest*)&_meta.addr));          \
)

#define IMPL_meta_castPtr$(TDest, var_ptr...) eval( \
    let         _ptr = var_ptr;                     \
    eval_return as(TDest, _ptr.addr);               \
)

#define IMPL_meta_castSli$(TDest, var_sli...) eval(                 \
    let         _sli = var_sli;                                     \
    eval_return make(TDest, .ptr = (_sli).addr, .len = (_sli).len); \
)

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
