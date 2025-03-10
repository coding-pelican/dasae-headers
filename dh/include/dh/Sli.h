/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    sli.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-05 (date of creation)
 * @updated 2025-03-10 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Generic slice operations
 * @details Provides generic slice types that can be used with any element type:
 *          - Sli: Regular slice with ptr and len
 *          - SliZ: Null-terminated slice (like C strings)
 *          - SliS: Sentinel-terminated slice (with custom sentinel value)
 *          All with safe bounds checking and flexible range operations.
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
#define use_Sli$(T)                               comp_gen__use_Sli$(T)
#define decl_Sli$(T)                              comp_gen__decl_Sli$(T)
#define impl_Sli$(T)                              comp_gen__impl_Sli$(T)
#define Sli_asNamed$(T_NamedSli, var_unnamed_sli) comp_op__Sli_asNamed$(pp_uniqTok(__unnamed_sli), T_NamedSli, var_unnamed_sli)

#define Sli_from(val_ptr, val_len...)         comp_op__Sli_from(val_ptr, val_len)
#define Sli_from$(T_Sli, val_ptr, val_len...) comp_op__Sli_from$(T_Sli, val_ptr, val_len)
#define Sli_asg(var_addr_sli, val_sli...)     comp_op__Sli_asg(var_addr_sli, val_sli)

/* Core Slice Operations */

#define Sli_len(var_self...)                          comp_op__Sli_len(var_self)
#define Sli_at(var_self, usize_index...)              comp_op__Sli_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Sli_getAt(var_self, usize_index...)           comp_op__Sli_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Sli_setAt(var_self, usize_index, val_item...) comp_op__Sli_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

/* Range-based Slice Operations */

#define Sli_slice(var_self, range_index_begin_end...) /* sli[begin..end] */ \
    comp_op__Sli_slice(var_self, range_index_begin_end)
#define Sli_prefix(var_self, usize_index_end...) /* sli[0..index] */ \
    comp_op__Sli_prefix(var_self, usize_index_end)
#define Sli_prefixZ(var_self...) /* sli[0..*:null] */ \
    comp_op__Sli_prefixZ(var_self)
#define Sli_prefixS(var_self, val_sentinel...) /* sli[0..*:sentinel] */ \
    comp_op__Sli_prefixS(var_self, val_sentinel)
#define Sli_suffix(var_self, usize_index_begin...) /* sli[index..len] */ \
    comp_op__Sli_suffix(var_self, usize_index_begin)
#define Sli_suffixZ(var_self, usize_index_begin...) /* sli[index..*:null] */ \
    comp_op__Sli_suffixZ(var_self, usize_index_begin)
#define Sli_suffixS(var_self, usize_index_begin, val_sentinel...) /* sli[index..*:sentinel] */ \
    comp_op__Sli_suffixS(var_self, usize_index_begin, val_sentinel)

/* Null-terminated slice (similar to C strings) =============================*/

/**
 * @brief Null-terminated slice (similar to C strings)
 * @tparam T Element type
 */
#define SliZ_const$(T)                                comp_type_unnamed__SliZ_const$(T)
#define SliZ$(T)                                      comp_type_unnamed__SliZ$(T)
#define use_SliZ$(T)                                  comp_gen__use_SliZ$(T)
#define decl_SliZ$(T)                                 comp_gen__decl_SliZ$(T)
#define impl_SliZ$(T)                                 comp_gen__impl_SliZ$(T)
#define SliZ_asNamed$(T_NamedSliZ, var_unnamed_sli_z) comp_op__SliZ_asNamed$(T_NamedSliZ, var_unnamed_sli_z)

/* Sentinel-terminated slice (ends with a specific sentinel value) ==========*/

/**
 * @brief Sentinel-terminated slice (ends with a specific sentinel value)
 * @tparam T Element type
 */
#define SliS_const$(T)                                comp_type_unnamed__SliS_const$(T)
#define SliS$(T)                                      comp_type_unnamed__SliS$(T)
#define use_SliS$(T)                                  comp_gen__use_SliS$(T)
#define decl_SliS$(T)                                 comp_gen__decl_SliS$(T)
#define impl_SliS$(T)                                 comp_gen__impl_SliS$(T)
#define SliS_asNamed$(T_NamedSliS, var_unnamed_sli_s) comp_op__SliS_asNamed$(T_NamedSliS, var_unnamed_sli_s)

/*========== Macros and Implementations =====================================*/

#define comp_type_unnamed__Sli_const$(T) \
    struct {                             \
        rawptr_const$(T) ptr;            \
        usize len;                       \
    }
#define comp_type_unnamed__Sli$(T) \
    union {                        \
        Sli_const$(T) as_const;    \
        struct {                   \
            rawptr$(T) ptr;        \
            usize len;             \
        };                         \
    }
#define comp_gen__use_Sli$(T) \
    decl_Sli$(T);             \
    impl_Sli$(T)
#define comp_gen__decl_Sli$(T)                                        \
    typedef struct pp_join($, Sli_const, T) pp_join($, Sli_const, T); \
    typedef union pp_join($, Sli, T) pp_join($, Sli, T)
#define comp_gen__impl_Sli$(T)             \
    struct pp_join($, Sli_const, T) {      \
        rawptr_const$(T) ptr;              \
        usize len;                         \
    };                                     \
    union pp_join($, Sli, T) {             \
        pp_join($, Sli_const, T) as_const; \
        struct {                           \
            rawptr$(T) ptr;                \
            usize len;                     \
        };                                 \
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

#define comp_op__Sli_from(val_ptr, val_len...)                 { .ptr = val_ptr, .len = val_len }
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
    debug_assert_nonnull((__self).ptr);                                   \
    debug_assert_fmt(                                                     \
        (__index) < (__self).len,                                         \
        "Index out of bounds: %zu >= %zu",                                \
        __index,                                                          \
        (__self).len                                                      \
    );                                                                    \
    eval_return(                                                          \
        &(__self).ptr[__index]                                            \
    );                                                                    \
})
#define comp_op__Sli_getAt(__self, __index, var_self, usize_index...) eval({ \
    let         __self  = var_self;                                          \
    const usize __index = usize_index;                                       \
    debug_assert_nonnull((__self).ptr);                                      \
    debug_assert_fmt(                                                        \
        (__index) < (__self).len,                                            \
        "Index out of bounds: %zu >= %zu",                                   \
        __index,                                                             \
        (__self).len                                                         \
    );                                                                       \
    eval_return(                                                             \
        (__self).ptr[__index]                                                \
    );                                                                       \
})
#define comp_op__Sli_setAt(__self, __index, var_self, usize_index, val_item...) eval({ \
    let         __self  = var_self;                                                    \
    const usize __index = usize_index;                                                 \
    debug_assert_nonnull((__self).ptr);                                                \
    debug_assert_fmt(                                                                  \
        (__index) < (__self).len,                                                      \
        "Index out of bounds: %zu >= %zu",                                             \
        __index,                                                                       \
        (__self).len                                                                   \
    );                                                                                 \
    (__self).ptr[__index] = val_item;                                                  \
    eval_return __self;                                                                \
})

#define comp_op__Sli_slice(__self, __range, var_self, range_index_begin_end...) eval({ \
    let         __self  = var_self;                                                    \
    const Range __range = Range_from range_index_begin_end;                            \
    debug_assert_fmt(                                                                  \
        (__range).begin < (__range).end,                                               \
        "Invalid slice range: begin(%zu) >= end(%zu)",                                 \
        (__range).begin,                                                               \
        (__range).end                                                                  \
    );                                                                                 \
    debug_assert_fmt(                                                                  \
        (__range).end <= (__self).len,                                                 \
        "Index out of bounds: end(%zu) > len(%zu)",                                    \
        (__range).end,                                                                 \
        (__self).len                                                                   \
    );                                                                                 \
    eval_return make$(                                                                 \
        TypeOf(__self),                                                                \
        .ptr = (__self).ptr + (__range).begin,                                         \
        .len = Range_len(__range)                                                      \
    );                                                                                 \
})
#define comp_op__Sli_prefix(__self, __end, var_self, usize_index_end...) eval({ \
    let         __self = var_self;                                              \
    const usize __end  = usize_index_end;                                       \
    debug_assert_fmt(                                                           \
        (__end) <= (__self).len,                                                \
        "Index out of bounds: %zu > %zu",                                       \
        __end,                                                                  \
        (__self)->len                                                           \
    );                                                                          \
    eval_return make$(                                                          \
        TypeOf(__self),                                                         \
        .ptr = (__self).ptr,                                                    \
        .len = (__end)                                                          \
    );                                                                          \
})
#define comp_op__Sli_prefixZ(__self, var_self...) eval({ \
    let __self = var_self;                               \
    eval_return make$(                                   \
        SliZ$(TypeOf(*(__self).ptr)),                    \
        .ptr = (__self).ptr                              \
    );                                                   \
})
#define comp_op__Sli_prefixS(__self, __sentinel, var_self, val_sentinel...) eval({ \
    let __self     = var_self;                                                     \
    let __sentinel = val_sentinel;                                                 \
    eval_return make$(                                                             \
        SliS$(TypeOf(*(__self).ptr)),                                              \
        .ptr      = (__self).ptr,                                                  \
        .sentinel = (__sentinel)                                                   \
    );                                                                             \
})
#define comp_op__Sli_suffix(__self, __begin, var_self, usize_index_begin) eval({ \
    let         __self  = var_self;                                              \
    const usize __begin = usize_index_begin;                                     \
    debug_assert_fmt(                                                            \
        (__begin) < (__self).len,                                                \
        "Index out of bounds: %zu > %zu",                                        \
        __begin,                                                                 \
        (__self).len                                                             \
    );                                                                           \
    eval_return make$(                                                           \
        TypeOf(__self),                                                          \
        .ptr = (__self).ptr + (__begin),                                         \
        .len = (__self).len - (__begin)                                          \
    );                                                                           \
})
#define comp_op__Sli_suffixZ(__self, __begin, var_self, usize_index_begin) eval({ \
    let         __self  = var_self;                                               \
    const usize __begin = usize_index_begin;                                      \
    debug_assert_fmt(                                                             \
        (__begin) < (__self).len,                                                 \
        "Index out of bounds: %zu > %zu",                                         \
        __begin,                                                                  \
        (__self).len                                                              \
    );                                                                            \
    eval_return make$(                                                            \
        SliZ$(TypeOf(*(__self).ptr)),                                             \
        .ptr = (__self).ptr + (__begin)                                           \
    );                                                                            \
})
#define comp_op__Sli_suffixS(__self, __begin, __sentinel, var_self, usize_index_begin, val_sentinel) eval({ \
    let         __self     = var_self;                                                                      \
    const usize __begin    = usize_index_begin;                                                             \
    let         __sentinel = val_sentinel;                                                                  \
    debug_assert_fmt(                                                                                       \
        (__begin) < (__self).len,                                                                           \
        "Index out of bounds: %zu > %zu",                                                                   \
        __begin,                                                                                            \
        (__self).len                                                                                        \
    );                                                                                                      \
    eval_return make$(                                                                                      \
        SliS$(TypeOf(*(__self).ptr)),                                                                       \
        .ptr      = (__self).ptr + (__begin),                                                               \
        .sentinel = (__sentinel)                                                                            \
    );                                                                                                      \
})


#define comp_type_unnamed__SliZ_const$(T) \
    struct {                              \
        rawptr_const$(T) ptr;             \
    }
#define comp_type_unnamed__SliZ$(T) \
    union {                         \
        SliZ_const$(T) as_const;    \
        struct {                    \
            rawptr$(T) ptr;         \
        };                          \
    }
#define comp_gen__use_SliZ$(T) \
    decl_SliZ$(T);             \
    impl_SliZ$(T)
#define comp_gen__decl_SliZ$(T)                                         \
    typedef struct pp_join($, SliZ_const, T) pp_join($, SliZ_const, T); \
    typedef union pp_join($, SliZ, T) pp_join($, SliZ, T)
#define comp_gen__impl_SliZ$(T)             \
    struct pp_join($, SliZ_const, T) {      \
        rawptr_const$(T) ptr;               \
    };                                      \
    union pp_join($, SliZ, T) {             \
        pp_join($, SliZ_const, T) as_const; \
        struct {                            \
            rawptr$(T) ptr;                 \
        };                                  \
    }
#define comp_op__SliZ_asNamed$(T_NamedSliZ, var_unnamed_sli_z)


#define comp_type_unnamed__SliS_const$(T) \
    struct {                              \
        rawptr_const$(T) ptr;             \
        T sentinel;                       \
    }
#define comp_type_unnamed__SliS$(T) \
    union {                         \
        SliS_const$(T) as_const;    \
        struct {                    \
            rawptr$(T) ptr;         \
            T sentinel;             \
        };                          \
    }
#define comp_gen__use_SliS$(T) \
    decl_SliS$(T);             \
    impl_SliS$(T)
#define comp_gen__decl_SliS$(T)                                         \
    typedef struct pp_join($, SliS_const, T) pp_join($, SliS_const, T); \
    typedef union pp_join($, SliS, T) pp_join($, SliS, T)
#define comp_gen__impl_SliS$(T)             \
    struct pp_join($, SliS_const, T) {      \
        rawptr_const$(T) ptr;               \
        T sentinel;                         \
    };                                      \
    union pp_join($, SliS, T) {             \
        pp_join($, SliS_const, T) as_const; \
        struct {                            \
            rawptr$(T) ptr;                 \
            T sentinel;                     \
        };                                  \
    }
#define comp_op__SliS_asNamed$(T_NamedSliS, var_unnamed_sli_s)

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
void slice_example(void) {
    // Create a slice from an array
    int     numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    Sli$i32 slice     = Sli_from$(i32, numbers, 10);

    // Basic operations
    i32* third_ptr = Sli_at(slice, 2);    // Get pointer to the third element
    i32  value     = Sli_getAt(slice, 3); // Get value of the fourth element
    Sli_setAt(slice, 4, 50);              // Set the fifth element to 50
    usize length = Sli_len(slice);        // Get the length (10)

    // Range-based operations
    Sli$i32 middle      = Sli_slice(slice, 2, 7); // Elements 3 through 7
    Sli$i32 first_half  = Sli_prefix(slice, 5);   // First 5 elements
    Sli$i32 second_half = Sli_suffix(slice, 5);   // Last 5 elements

    // Range operations
    Range full_range   = Sli_range(slice);          // Range{0, 10}
    Range mid_range    = Sli_subrange(slice, 2, 7); // Range{2, 7}
    Range prefix_range = Sli_prefixRange(slice, 5); // Range{0, 5}
    Range suffix_range = Sli_suffixRange(slice, 5); // Range{5, 10}

    // Create a null-terminated string
    char str[] = "Hello, world";

    // Working with null-terminated slices (SliZ)
    use_SliZ$(char);
    SliZ$char str_z     = SliZ_from$(char, str);
    usize     z_len     = SliZ_len(str_z);              // 12 (not including null)
    char      ch        = SliZ_getAt(str_z, 1);         // 'e'
    Sli$char  hello_sli = SliZ_toSli$(Sli$char, str_z); // Convert to regular slice

    // Working with sentinel-terminated slices (SliS)
    use_SliS$(i32);
    int      numbers_with_sentinel[] = { 1, 2, 3, 4, 5, -1, 6, 7 }; // -1 is sentinel
    SliS$i32 nums_s                  = SliS_from$(i32, numbers_with_sentinel, -1);
    usize    s_len                   = SliS_len(nums_s);             // 5 (up to but not including -1)
    i32      num                     = SliS_getAt(nums_s, 2);        // 3
    Sli$i32  nums_sli                = SliS_toSli$(Sli$i32, nums_s); // Convert to regular slice

    // Type casting example
    Sli$u32 bytes = Sli_castS$(Sli$u8, slice);  // View as bytes (i32 -> u8)
    Sli$i32 ints  = Sli_castL$(Sli$i32, bytes); // Convert back to ints
}
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SLI_INCLUDED */
