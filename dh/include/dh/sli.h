/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    sli.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-05 (date of creation)
 * @updated 2025-03-12 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Generic slice operations
 * @details Provides generic slice types that can be used with any element type:
 *          - SliZ: Zero(Null)-terminated slice (like C strings)
 *          - SliS: Sentinel-terminated slice (with custom sentinel value)
 *          - Sli: Regular slice with ptr and len
 *          with safe bounds checking and flexible range operations.
 */

#ifndef SLI_INCLUDED
#define SLI_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/Range.h"

/*========== Macros and Definitions =========================================*/

/* Slice ====================================================================*/

#define Sli_const$(T)                             comp_type_unnamed__Sli_const$(T)
#define Sli$(T)                                   comp_type_unnamed__Sli$(T)
#define Sli_asNamed$(T_NamedSli, var_unnamed_sli) comp_op__Sli_asNamed$(pp_uniqTok(__unnamed_sli), T_NamedSli, var_unnamed_sli)

#define use_Sli$(T)  comp_gen__use_Sli$(T)
#define decl_Sli$(T) comp_gen__decl_Sli$(T)
#define impl_Sli$(T) comp_gen__impl_Sli$(T)

/* Core Slice Operations */

#define Sli_from(val_ptr, val_len...)         comp_op__Sli_from(val_ptr, val_len)
#define Sli_from$(T_Sli, val_ptr, val_len...) comp_op__Sli_from$(T_Sli, val_ptr, val_len)
#define Sli_asg(var_addr_sli, val_sli...)     comp_op__Sli_asg(var_addr_sli, val_sli)

#define Sli_len(var_self...)                          comp_op__Sli_len(var_self)
#define Sli_at(var_self, usize_index...)              comp_op__Sli_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Sli_getAt(var_self, usize_index...)           comp_op__Sli_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Sli_setAt(var_self, usize_index, val_item...) comp_op__Sli_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

/* Range-based Slice Operations */

#define Sli_slice(var_self, range_index_begin_end...) /* sli[begin..end] */ \
    comp_op__Sli_slice(pp_uniqTok(self), pp_uniqTok(range), var_self, range_index_begin_end)
#define Sli_prefix(var_self, usize_index_end...) /* sli[0..index] */ \
    comp_op__Sli_prefix(pp_uniqTok(self), pp_uniqTok(end), var_self, usize_index_end)
#define Sli_prefixZ(var_self...) /* sli[0..*:null] */ \
    comp_op__Sli_prefixZ(pp_uniqTok(self), var_self)
#define Sli_prefixS(var_self, val_sentinel...) /* sli[0..*:sentinel] */ \
    comp_op__Sli_prefixS(pp_uniqTok(self), pp_uniqTok(sentinel), var_self, val_sentinel)
#define Sli_suffix(var_self, usize_index_begin...) /* sli[index..len] */ \
    comp_op__Sli_suffix(pp_uniqTok(self), pp_uniqTok(begin), var_self, usize_index_begin)
#define Sli_suffixZ(var_self, usize_index_begin...) /* sli[index..*:null] */ \
    comp_op__Sli_suffixZ(pp_uniqTok(self), pp_uniqTok(begin), var_self, usize_index_begin)
#define Sli_suffixS(var_self, usize_index_begin, val_sentinel...) /* sli[index..*:sentinel] */ \
    comp_op__Sli_suffixS(pp_uniqTok(self), pp_uniqTok(begin), pp_uniqTok(sentinel), var_self, usize_index_begin, val_sentinel)

/* Null-terminated slice (similar to C strings) =============================*/

/**
 * @brief Null-terminated slice (similar to C strings)
 * @tparam T Element type
 */
#define SliZ_const$(T)                                comp_type_unnamed__SliZ_const$(T)
#define SliZ$(T)                                      comp_type_unnamed__SliZ$(T)
#define SliZ_asNamed$(T_NamedSliZ, var_unnamed_sli_z) comp_op__SliZ_asNamed$(T_NamedSliZ, var_unnamed_sli_z)

#define SliZ_from(val_ptr...)              comp_op__SliZ_from(val_ptr)
#define SliZ_from$(T_SliZ, val_ptr...)     comp_op__SliZ_from$(T_SliZ, val_ptr)
#define SliZ_asg(var_addr_sli, val_sli...) comp_op__SliZ_asg(var_addr_sli, val_sli)

#define SliZ_at(var_self, usize_index...)              comp_op__SliZ_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define SliZ_getAt(var_self, usize_index...)           comp_op__SliZ_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define SliZ_setAt(var_self, usize_index, val_item...) comp_op__SliZ_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

/* Sentinel-terminated slice (ends with a specific sentinel value) ==========*/

/**
 * @brief Sentinel-terminated slice (ends with a specific sentinel value)
 * @tparam T Element type
 */
#define SliS_const$(T)                                comp_type_unnamed__SliS_const$(T)
#define SliS$(T)                                      comp_type_unnamed__SliS$(T)
#define SliS_asNamed$(T_NamedSliS, var_unnamed_sli_s) comp_op__SliS_asNamed$(T_NamedSliS, var_unnamed_sli_s)

#define SliS_from(val_ptr, val_sentinel...)          comp_op__SliS_from(val_ptr, val_sentinel)
#define SliS_from$(T_SliS, val_ptr, val_sentinel...) comp_op__SliS_from$(T_SliS, val_ptr, val_sentinel)
#define SliS_asg(var_addr_sli, val_sli...)           comp_op__SliS_asg(var_addr_sli, val_sli)

#define SliS_at(var_self, usize_index...)              comp_op__SliS_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define SliS_getAt(var_self, usize_index...)           comp_op__SliS_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define SliS_setAt(var_self, usize_index, val_item...) comp_op__SliS_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

/*========== Macros and Implementations =====================================*/

#define comp_type_unnamed__Sli_const$(T)   \
    union {                                \
        SliZ_const$(T) as_zero_terminated; \
        struct {                           \
            rawptr_const$(T) ptr;          \
            usize len;                     \
        };                                 \
    }
#define comp_type_unnamed__Sli$(T)   \
    union {                          \
        SliZ$(T) as_zero_terminated; \
        struct {                     \
            rawptr$(T) ptr;          \
            usize len;               \
        };                           \
        Sli_const$(T) as_const;      \
    }
#define comp_op__Sli_asNamed$(__unnamed_sli, T_NamedSli, var_unnamed_sli) eval({                             \
    let __unnamed_sli = var_unnamed_sli;                                                                     \
    claim_assert_static(sizeOf(T_NamedSli) == sizeOf(TypeOf(__unnamed_sli)));                                \
    claim_assert_static(alignOf(T_NamedSli) == alignOf(TypeOf(__unnamed_sli)));                              \
    claim_assert_static(hasField(TypeOf(__unnamed_sli), len));                                               \
    claim_assert_static(isSameType$(FieldTypeOf(T_NamedSli, len), FieldTypeOf(TypeOf(__unnamed_sli), len))); \
    claim_assert_static(hasField(TypeOf(__unnamed_sli), ptr));                                               \
    claim_assert_static(isSameType$(FieldTypeOf(T_NamedSli, ptr), FieldTypeOf(TypeOf(__unnamed_sli), ptr))); \
    eval_return(*(T_NamedSli*)&(__unnamed_sli));                                                             \
})

#define comp_gen__use_Sli$(T) \
    decl_Sli$(T);             \
    impl_Sli$(T)
#define comp_gen__decl_Sli$(T)                                          \
    typedef struct pp_join($, SliZ_const, T) pp_join($, SliZ_const, T); \
    typedef union pp_join($, SliZ, T) pp_join($, SliZ, T);              \
    typedef union pp_join($, SliS_const, T) pp_join($, SliS_const, T);  \
    typedef union pp_join($, SliS, T) pp_join($, SliS, T);              \
    typedef union pp_join($, Sli_const, T) pp_join($, Sli_const, T);    \
    typedef union pp_join($, Sli, T) pp_join($, Sli, T)
#define comp_gen__impl_Sli$(T)                                      \
    struct pp_join($, SliZ_const, T) {                              \
        rawptr_const$(T) ptr;                                       \
        union {                                                     \
            rawptr$(pp_join($, SliS_const, T)) sentinel_terminated; \
            rawptr$(pp_join($, Sli_const, T)) regular;              \
        } __association_type_hints[0];                              \
    };                                                              \
    union pp_join($, SliZ, T) {                                     \
        struct {                                                    \
            rawptr$(T) ptr;                                         \
        };                                                          \
        pp_join($, SliZ_const, T) as_const;                         \
        union {                                                     \
            rawptr$(pp_join($, SliS, T)) sentinel_terminated;       \
            rawptr$(pp_join($, Sli, T)) regular;                    \
        } __association_type_hints[0];                              \
    };                                                              \
    union pp_join($, SliS_const, T) {                               \
        pp_join($, SliZ_const, T) as_zero_terminated;               \
        struct {                                                    \
            rawptr_const$(T) ptr;                                   \
            T sentinel;                                             \
        };                                                          \
        union {                                                     \
            rawptr$(pp_join($, SliZ_const, T)) zero_terminated;     \
            rawptr$(pp_join($, Sli_const, T)) regular;              \
        } __association_type_hints[0];                              \
    };                                                              \
    union pp_join($, SliS, T) {                                     \
        pp_join($, SliZ, T) as_zero_terminated;                     \
        struct {                                                    \
            rawptr$(T) ptr;                                         \
            T sentinel;                                             \
        };                                                          \
        pp_join($, SliS_const, T) as_const;                         \
        union {                                                     \
            rawptr$(pp_join($, SliZ, T)) zero_terminated;           \
            rawptr$(pp_join($, Sli, T)) regular;                    \
        } __association_type_hints[0];                              \
    };                                                              \
    union pp_join($, Sli_const, T) {                                \
        pp_join($, SliZ_const, T) as_zero_terminated;               \
        struct {                                                    \
            rawptr_const$(T) ptr;                                   \
            usize len;                                              \
        };                                                          \
        union {                                                     \
            rawptr$(pp_join($, SliZ_const, T)) zero_terminated;     \
            rawptr$(pp_join($, SliS_const, T)) sentinel_terminated; \
        } __association_type_hints[0];                              \
    };                                                              \
    union pp_join($, Sli, T) {                                      \
        pp_join($, SliZ, T) as_zero_terminated;                     \
        struct {                                                    \
            rawptr$(T) ptr;                                         \
            usize len;                                              \
        };                                                          \
        pp_join($, Sli_const, T) as_const;                          \
        union {                                                     \
            rawptr$(pp_join($, SliZ, T)) zero_terminated;           \
            rawptr$(pp_join($, SliS, T)) sentinel_terminated;       \
        } __association_type_hints[0];                              \
    }


#define comp_op__Sli_from(val_ptr, val_len...)                 { .ptr = ensureNonnull(val_ptr), .len = val_len }
#define comp_op__Sli_from$(T_Sli, val_ptr, val_len...)         ((T_Sli)Sli_from(val_ptr, val_len))
#define comp_op__Sli_asg(__addr_sli, var_addr_sli, val_sli...) eval({ \
    let __addr_sli = var_addr_sli;                                    \
    debug_assert_nonnull(__addr_sli);                                 \
    *__addr_sli = TypeOf(*__addr_sli) val_sli;                        \
    eval_return __addr_sli;                                           \
})

#define comp_op__Sli_len(var_self...)                              (var_self.len)
#define comp_op__Sli_at(__self, __index, var_self, usize_index...) eval({ \
    let         __self  = var_self;                                       \
    const usize __index = usize_index;                                    \
    debug_assert_nonnull(__self.ptr);                                     \
    debug_assert_fmt(                                                     \
        __index < __self.len,                                             \
        "Index out of bounds: %zu >= %zu",                                \
        __index,                                                          \
        __self.len                                                        \
    );                                                                    \
    eval_return(                                                          \
        &__self.ptr[__index]                                              \
    );                                                                    \
})
#define comp_op__Sli_getAt(__self, __index, var_self, usize_index...) eval({ \
    let         __self  = var_self;                                          \
    const usize __index = usize_index;                                       \
    debug_assert_nonnull(__self.ptr);                                        \
    debug_assert_fmt(                                                        \
        __index < __self.len,                                                \
        "Index out of bounds: %zu >= %zu",                                   \
        __index,                                                             \
        __self.len                                                           \
    );                                                                       \
    eval_return(                                                             \
        __self.ptr[__index]                                                  \
    );                                                                       \
})
#define comp_op__Sli_setAt(__self, __index, var_self, usize_index, val_item...) eval({ \
    let         __self  = var_self;                                                    \
    const usize __index = usize_index;                                                 \
    debug_assert_nonnull(__self.ptr);                                                  \
    debug_assert_fmt(                                                                  \
        __index < __self.len,                                                          \
        "Index out of bounds: %zu >= %zu",                                             \
        __index,                                                                       \
        __self.len                                                                     \
    );                                                                                 \
    __self.ptr[__index] = val_item;                                                    \
    eval_return __self;                                                                \
})

#define comp_op__Sli_slice(__self, __range, var_self, range_index_begin_end...) eval({ \
    let         __self  = var_self;                                                    \
    const Range __range = Range_from range_index_begin_end;                            \
    debug_assert_fmt(                                                                  \
        __range.begin < __range.end,                                                   \
        "Invalid slice range: begin(%zu) >= end(%zu)",                                 \
        __range.begin,                                                                 \
        __range.end                                                                    \
    );                                                                                 \
    debug_assert_fmt(                                                                  \
        __range.end <= __self.len,                                                     \
        "Index out of bounds: end(%zu) > len(%zu)",                                    \
        __range.end,                                                                   \
        __self.len                                                                     \
    );                                                                                 \
    eval_return make$(                                                                 \
        TypeOf(__self),                                                                \
        .ptr = __self.ptr + __range.begin,                                             \
        .len = Range_len(__range)                                                      \
    );                                                                                 \
})
#define comp_op__Sli_prefix(__self, __end, var_self, usize_index_end...) eval({ \
    let         __self = var_self;                                              \
    const usize __end  = usize_index_end;                                       \
    debug_assert_fmt(                                                           \
        __end <= __self.len,                                                    \
        "Index out of bounds: %zu > %zu",                                       \
        __end,                                                                  \
        __self.len                                                              \
    );                                                                          \
    eval_return make$(                                                          \
        TypeOf(__self),                                                         \
        .ptr = __self.ptr,                                                      \
        .len = __end                                                            \
    );                                                                          \
})
#define comp_op__Sli_prefixZ(__self, var_self...) eval({               \
    let __self = var_self;                                             \
    eval_return make$(                                                 \
        TypeOf(__self.__association_type_hints[0].zero_terminated[0]), \
        .ptr = __self.ptr                                              \
    );                                                                 \
})
#define comp_op__Sli_prefixS(__self, __sentinel, var_self, val_sentinel...) eval({ \
    let __self     = var_self;                                                     \
    let __sentinel = val_sentinel;                                                 \
    eval_return make$(                                                             \
        TypeOf(__self.__association_type_hints[0].sentinel_terminated[0]),         \
        .ptr      = __self.ptr,                                                    \
        .sentinel = __sentinel                                                     \
    );                                                                             \
})
#define comp_op__Sli_suffix(__self, __begin, var_self, usize_index_begin...) eval({ \
    let         __self  = var_self;                                                 \
    const usize __begin = usize_index_begin;                                        \
    debug_assert_fmt(                                                               \
        __begin < __self.len,                                                       \
        "Index out of bounds: %zu > %zu",                                           \
        __begin,                                                                    \
        __self.len                                                                  \
    );                                                                              \
    eval_return make$(                                                              \
        TypeOf(__self),                                                             \
        .ptr = __self.ptr + __begin,                                                \
        .len = __self.len - __begin                                                 \
    );                                                                              \
})
#define comp_op__Sli_suffixZ(__self, __begin, var_self, usize_index_begin...) eval({ \
    let         __self  = var_self;                                                  \
    const usize __begin = usize_index_begin;                                         \
    debug_assert_fmt(                                                                \
        __begin < __self.len,                                                        \
        "Index out of bounds: %zu > %zu",                                            \
        __begin,                                                                     \
        __self.len                                                                   \
    );                                                                               \
    eval_return make$(                                                               \
        TypeOf(__self.__association_type_hints[0].zero_terminated[0]),               \
        .ptr = __self.ptr + __begin                                                  \
    );                                                                               \
})
#define comp_op__Sli_suffixS(__self, __begin, __sentinel, var_self, usize_index_begin, val_sentinel...) eval({ \
    let         __self     = var_self;                                                                         \
    const usize __begin    = usize_index_begin;                                                                \
    let         __sentinel = val_sentinel;                                                                     \
    debug_assert_fmt(                                                                                          \
        __begin < __self.len,                                                                                  \
        "Index out of bounds: %zu > %zu",                                                                      \
        __begin,                                                                                               \
        __self.len                                                                                             \
    );                                                                                                         \
    eval_return make$(                                                                                         \
        TypeOf(__self.__association_type_hints[0].sentinel_terminated[0]),                                     \
        .ptr      = __self.ptr + __begin,                                                                      \
        .sentinel = __sentinel                                                                                 \
    );                                                                                                         \
})


#define comp_type_unnamed__SliZ_const$(T) \
    struct {                              \
        rawptr_const$(T) ptr;             \
    }
#define comp_type_unnamed__SliZ$(T) \
    union {                         \
        struct {                    \
            rawptr$(T) ptr;         \
        };                          \
        SliZ_const$(T) as_const;    \
    }
#define comp_op__SliZ_asNamed$(T_NamedSliZ, var_unnamed_sli_z)

#define comp_op__SliZ_from(val_ptr...)                          { .ptr = ensureNonnull(val_ptr) }
#define comp_op__SliZ_from$(T_SliZ, val_ptr...)                 ((T_SliZ)SliZ_from(val_ptr))
#define comp_op__SliZ_asg(__addr_sli, var_addr_sli, val_sli...) eval({ \
    let __addr_sli = var_addr_sli;                                     \
    debug_assert_nonnull(__addr_sli);                                  \
    *__addr_sli = TypeOf(*__addr_sli) val_sli;                         \
    eval_return __addr_sli;                                            \
})

#define comp_op__SliZ_at(__self, __index, var_self, usize_index...) eval({ \
    let         __self  = var_self;                                        \
    const usize __index = usize_index;                                     \
    debug_assert_nonnull(__self.ptr);                                      \
    eval_return(                                                           \
        &__self.ptr[__index]                                               \
    );                                                                     \
})
#define comp_op__SliZ_getAt(__self, __index, var_self, usize_index...) eval({ \
    let         __self  = var_self;                                           \
    const usize __index = usize_index;                                        \
    debug_assert_nonnull(__self.ptr);                                         \
    eval_return(                                                              \
        __self.ptr[__index]                                                   \
    );                                                                        \
})
#define comp_op__SliZ_setAt(__self, __index, var_self, usize_index, val_item...) eval({ \
    let         __self  = var_self;                                                     \
    const usize __index = usize_index;                                                  \
    debug_assert_nonnull(__self.ptr);                                                   \
    __self.ptr[__index] = val_item;                                                     \
    eval_return __self;                                                                 \
})


#define comp_type_unnamed__SliS_const$(T)  \
    union {                                \
        SliZ_const$(T) as_zero_terminated; \
        struct {                           \
            rawptr_const$(T) ptr;          \
            T sentinel;                    \
        };                                 \
    }
#define comp_type_unnamed__SliS$(T)  \
    union {                          \
        SliZ$(T) as_zero_terminated; \
        struct {                     \
            rawptr$(T) ptr;          \
            T sentinel;              \
        };                           \
        SliS_const$(T) as_const;     \
    }
#define comp_op__SliS_asNamed$(T_NamedSliS, var_unnamed_sli_s)

#define comp_op__SliS_from(val_ptr, val_sentinel...)            { .ptr = ensureNonnull(val_ptr), .sentinel = val_sentinel }
#define comp_op__SliS_from$(T_SliS, val_ptr, val_sentinel...)   ((T_SliS)SliS_from(val_ptr, val_sentinel))
#define comp_op__SliS_asg(__addr_sli, var_addr_sli, val_sli...) eval({ \
    let __addr_sli = var_addr_sli;                                     \
    debug_assert_nonnull(__addr_sli);                                  \
    *__addr_sli = TypeOf(*__addr_sli) val_sli;                         \
    eval_return __addr_sli;                                            \
})

#define comp_op__SliS_at(__self, __index, var_self, usize_index...) eval({ \
    let         __self  = var_self;                                        \
    const usize __index = usize_index;                                     \
    debug_assert_nonnull(__self.ptr);                                      \
    eval_return(                                                           \
        &__self.ptr[__index]                                               \
    );                                                                     \
})
#define comp_op__SliS_getAt(__self, __index, var_self, usize_index...) eval({ \
    let         __self  = var_self;                                           \
    const usize __index = usize_index;                                        \
    debug_assert_nonnull(__self.ptr);                                         \
    eval_return(                                                              \
        __self.ptr[__index]                                                   \
    );                                                                        \
})
#define comp_op__SliS_setAt(__self, __index, var_self, usize_index, val_item...) eval({ \
    let         __self  = var_self;                                                     \
    const usize __index = usize_index;                                                  \
    debug_assert_nonnull(__self.ptr);                                                   \
    __self.ptr[__index] = val_item;                                                     \
    eval_return __self;                                                                 \
})

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
use_Sli$(u8);
use_Sli$(i32);

void slice_example(void) {
    // Create a slice from an array
    i32 numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    let slice     = Sli_from$(Sli$i32, numbers, 10);

    // Basic operations
    let third_ptr = Sli_at(slice, 2);    // Get pointer to the third element
    let value     = Sli_getAt(slice, 3); // Get value of the fourth element
    Sli_setAt(slice, 4, 50);             // Set the fifth element to 50
    let length = Sli_len(slice);         // Get the length (10)

    // Range-based operations
    let middle      = Sli_slice(slice, (2, 7)); // Elements 3 through 7
    let first_half  = Sli_prefix(slice, 5);     // First 5 elements
    let second_half = Sli_suffix(slice, 5);     // Last 5 elements

    let all           = Sli_prefixZ(slice);
    let terminated_10 = Sli_prefixS(slice, 10);
    for (var iter = SliS_at(terminated_10, 0); deref(iter) != terminated_10.sentinel; ++iter) {
        printf("%d ", deref(iter));
    }

    // Create a null-terminated string
    u8 str[] = "Hello, world";

    // Working with null-terminated slices (SliZ)
    let str_z = SliZ_from$(SliZ$u8, str);
    let ch    = SliZ_getAt(str_z, 1); // 'e'
    // let hello_sli = SliZ_toSli$(Sli$u8, str_z); // Convert to regular slice

    // Working with sentinel-terminated slices (SliS)
    i32 numbers_with_sentinel[] = { 1, 2, 3, 4, 5, -1, 6, 7 }; // -1 is sentinel
    let nums_s                  = SliS_from$(SliS$i32, numbers_with_sentinel, -1);
    // let s_len                   = SliS_len(nums_s);             // 5 (up to but not including -1)
    let num                     = SliS_getAt(nums_s, 2); // 3
    // let nums_sli                = SliS_toSli$(Sli$i32, nums_s); // Convert to regular slice

    // Type casting example
    // Sli$u32 bytes = Sli_castS$(Sli$u8, slice);  // View as bytes (i32 -> u8)
    // Sli$i32 ints  = Sli_castL$(Sli$i32, bytes); // Convert back to ints
}
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SLI_INCLUDED */
