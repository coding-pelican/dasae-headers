/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    sli.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-05 (date of creation)
 * @updated 2025-03-13 (date of last update)
 * @version v0.1-alpha.3
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

/*========== Macros and Declarations ========================================*/

typedef struct SliZ_const SliZ_const;
typedef union SliZ        SliZ;

typedef union SliS_const SliS_const;
typedef union SliS       SliS;

typedef union Sli_const Sli_const;
typedef union Sli       Sli;

/* Slice ====================================================================*/

#define use_Sli$(T)  comp_type_gen__use_Sli$(T)
#define decl_Sli$(T) comp_type_gen__decl_Sli$(T)
#define impl_Sli$(T) comp_type_gen__impl_Sli$(T)

#define Sli_const$(T)                     comp_type_alias__Sli_const$(T)
#define Sli$(T)                           comp_type_alias__Sli$(T)
#define Sli_const$$(T)                    comp_type_anon__Sli_const$$(T)
#define Sli$$(T)                          comp_type_anon__Sli$$(T)
#define Sli_anonCast$(T_Sli, var_anon...) comp_op__Sli_anonCast$(pp_uniqTok(__anon), T_Sli, var_anon)

/* Core Slice Operations */

#define Sli_from(val_ptr, val_len...)         comp_op__Sli_from(val_ptr, val_len)
#define Sli_from$(T_Sli, val_ptr, val_len...) comp_op__Sli_from$(T_Sli, val_ptr, val_len)
#define Sli_arr(val_arr...)                   comp_op__Sli_arr(pp_uniqTok(arr), val_arr)
#define Sli_arr$(T_Sli, val_arr...)           comp_op__Sli_arr$(pp_uniqTok(arr), T_Sli, val_arr)
#define Sli_asg(var_addr_sli, val_sli...)     comp_op__Sli_asg(var_addr_sli, val_sli)

#define Sli_len(var_self...)                          comp_op__Sli_len(var_self)
#define Sli_at(var_self, usize_index...)              comp_op__Sli_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Sli_getAt(var_self, usize_index...)           comp_op__Sli_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Sli_setAt(var_self, usize_index, val_item...) comp_op__Sli_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

/* Range-based Slice Operations */

#define Sli_slice(var_self, range_index_begin_end...) /* sli[begin..end] */ \
    comp_op__Sli_slice(pp_uniqTok(self), pp_uniqTok(range), var_self, range_index_begin_end)
#define Sli_sliceZ(var_self, range_index_begin_end...) /* sli[begin..end] */ \
    comp_op__Sli_sliceZ(pp_uniqTok(self), pp_uniqTok(range), var_self, range_index_begin_end)
#define Sli_sliceS(var_self, range_index_begin_end, val_sentinel...) /* sli[begin..end] */ \
    comp_op__Sli_sliceS(pp_uniqTok(self), pp_uniqTok(range), pp_uniqTok(sentinel), var_self, range_index_begin_end, val_sentinel)
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

/* Iterator support with scope (similar to Zig's for loops over slices) */

#define for_slice(var_sli, _Iter_item)                          comp_syn__for_slice(pp_uniqTok(sli), pp_uniqTok(i), var_sli, _Iter_item)
#define for_slice_indexed(var_sli, _Iter_item, _Iter_index)     comp_syn__for_slice_indexed(pp_uniqTok(sli), var_sli, _Iter_item, _Iter_index)
#define for_slice_rev(var_sli, _Iter_item)                      comp_syn__for_slice_rev(pp_uniqTok(sli), pp_uniqTok(i), var_sli, _Iter_item)
#define for_slice_rev_indexed(var_sli, _Iter_item, _Iter_index) comp_syn__for_slice_rev_indexed(pp_uniqTok(sli), var_sli, _Iter_item, _Iter_index)

/* Null-terminated slice (similar to C strings) =============================*/

/**
 * @brief Null-terminated slice (similar to C strings)
 * @tparam T Element type
 */
#define SliZ_const$(T)                              comp_type_anon__SliZ_const$(T)
#define SliZ$(T)                                    comp_type_anon__SliZ$(T)
#define SliZ_anonCast$(T_NamedSliZ, var_anon_sli_z) comp_op__SliZ_anonCast$(T_NamedSliZ, var_anon_sli_z)

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
#define SliS_const$(T)                              comp_type_anon__SliS_const$(T)
#define SliS$(T)                                    comp_type_anon__SliS$(T)
#define SliS_anonCast$(T_NamedSliS, var_anon_sli_s) comp_op__SliS_anonCast$(T_NamedSliS, var_anon_sli_s)

#define SliS_from(val_ptr, val_sentinel...)          comp_op__SliS_from(val_ptr, val_sentinel)
#define SliS_from$(T_SliS, val_ptr, val_sentinel...) comp_op__SliS_from$(T_SliS, val_ptr, val_sentinel)
#define SliS_asg(var_addr_sli, val_sli...)           comp_op__SliS_asg(var_addr_sli, val_sli)

#define SliS_at(var_self, usize_index...)              comp_op__SliS_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define SliS_getAt(var_self, usize_index...)           comp_op__SliS_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define SliS_setAt(var_self, usize_index, val_item...) comp_op__SliS_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

/*========== Macros and Definitions =========================================*/

typedef struct __AssociationTypes_Sli_const
    __AssociationTypes_Sli_const;
typedef union __AssociationTypes_Sli
    __AssociationTypes_Sli;

struct SliZ_const {
    anyptr_const ptr;
    const __AssociationTypes_Sli_const*
        __association_types_hints[0];
};
union SliZ {
    struct {
        anyptr ptr;
    };
    SliZ_const as_const;
    const __AssociationTypes_Sli*
        __association_types_hints[0];
};
union SliS_const {
    SliZ_const as_zero_terminated;
    struct {
        anyptr_const ptr;
        anyptr       sentinel;
    };
    const __AssociationTypes_Sli_const*
        __association_types_hints[0];
};
union SliS {
    SliZ as_zero_terminated;
    struct {
        anyptr ptr;
        anyptr sentinel;
    };
    SliS_const as_const;
    const __AssociationTypes_Sli*
        __association_types_hints[0];
};
union Sli_const {
    SliZ_const as_zero_terminated;
    struct {
        anyptr_const ptr;
        usize        len;
    };
    const __AssociationTypes_Sli_const*
        __association_types_hints[0];
};
union Sli {
    SliZ as_zero_terminated;
    struct {
        anyptr ptr;
        usize  len;
    };
    Sli_const as_const;
    const __AssociationTypes_Sli*
        __association_types_hints[0];
};

struct __AssociationTypes_Sli_const {
    SliZ_const* zero_terminated;
    SliS_const* sentinel_terminated;
    Sli_const*  regular;
};
union __AssociationTypes_Sli {
    struct {
        SliZ* zero_terminated;
        SliS* sentinel_terminated;
        Sli*  regular;
    };
    __AssociationTypes_Sli_const as_const;
};

#define comp_type_gen__use_Sli$(T) \
    decl_Sli$(T);                  \
    impl_Sli$(T)
#define comp_type_gen__decl_Sli$(T)                                                   \
    $maybe_unused typedef struct pp_join($, SliZ_const, T) pp_join($, SliZ_const, T); \
    $maybe_unused typedef union pp_join($, SliZ, T) pp_join($, SliZ, T);              \
    $maybe_unused typedef union pp_join($, SliS_const, T) pp_join($, SliS_const, T);  \
    $maybe_unused typedef union pp_join($, SliS, T) pp_join($, SliS, T);              \
    $maybe_unused typedef union Sli_const$(T) Sli_const$(T);                          \
    $maybe_unused typedef union Sli$(T) Sli$(T)
#define comp_type_gen__impl_Sli$(T)                                                             \
    typedef struct pp_join($, __AssociationTypes_Sli_const, T)                                  \
        pp_join($, __AssociationTypes_Sli_const, T);                                            \
    typedef union pp_join($, __AssociationTypes_Sli, T)                                         \
        pp_join($, __AssociationTypes_Sli, T);                                                  \
    struct pp_join($, SliZ_const, T) {                                                          \
        rawptr_const$(T) ptr;                                                                   \
        rawptr_const$(SliZ_const) __base_type_hint[0];                                          \
        rawptr_const$(pp_join($, __AssociationTypes_Sli_const, T)) __association_types_hint[0]; \
    };                                                                                          \
    union pp_join($, SliZ, T) {                                                                 \
        struct {                                                                                \
            rawptr$(T) ptr;                                                                     \
        };                                                                                      \
        pp_join($, SliZ_const, T) as_const;                                                     \
        rawptr_const$(SliZ) __base_type_hint[0];                                                \
        rawptr_const$(pp_join($, __AssociationTypes_Sli, T)) __association_types_hint[0];       \
    };                                                                                          \
    union pp_join($, SliS_const, T) {                                                           \
        pp_join($, SliZ_const, T) as_zero_terminated;                                           \
        struct {                                                                                \
            rawptr_const$(T) ptr;                                                               \
            T sentinel;                                                                         \
        };                                                                                      \
        rawptr_const$(SliS_const) __base_type_hint[0];                                          \
        rawptr_const$(pp_join($, __AssociationTypes_Sli_const, T)) __association_types_hint[0]; \
    };                                                                                          \
    union pp_join($, SliS, T) {                                                                 \
        pp_join($, SliZ, T) as_zero_terminated;                                                 \
        struct {                                                                                \
            rawptr$(T) ptr;                                                                     \
            T sentinel;                                                                         \
        };                                                                                      \
        pp_join($, SliS_const, T) as_const;                                                     \
        rawptr_const$(SliS) __base_type_hint[0];                                                \
        rawptr_const$(pp_join($, __AssociationTypes_Sli, T)) __association_types_hint[0];       \
    };                                                                                          \
    union pp_join($, Sli_const, T) {                                                            \
        pp_join($, SliZ_const, T) as_zero_terminated;                                           \
        struct {                                                                                \
            rawptr_const$(T) ptr;                                                               \
            usize len;                                                                          \
        };                                                                                      \
        rawptr_const$(Sli_const) __base_type_hint[0];                                           \
        rawptr_const$(pp_join($, __AssociationTypes_Sli_const, T)) __association_types_hint[0]; \
    };                                                                                          \
    union Sli$(T) {                                                                             \
        pp_join($, SliZ, T) as_zero_terminated;                                                 \
        struct {                                                                                \
            rawptr$(T) ptr;                                                                     \
            usize len;                                                                          \
        };                                                                                      \
        pp_join($, Sli_const, T) as_const;                                                      \
        rawptr_const$(Sli) __base_type_hint[0];                                                 \
        rawptr_const$(pp_join($, __AssociationTypes_Sli, T)) __association_types_hint[0];       \
    };                                                                                          \
    struct pp_join($, __AssociationTypes_Sli_const, T) {                                        \
        rawptr$(pp_join($, SliZ_const, T)) zero_terminated;                                     \
        rawptr$(pp_join($, SliS_const, T)) sentinel_terminated;                                 \
        rawptr$(pp_join($, Sli_const, T)) regular;                                              \
    };                                                                                          \
    union pp_join($, __AssociationTypes_Sli, T) {                                               \
        struct {                                                                                \
            rawptr$(pp_join($, SliZ, T)) zero_terminated;                                       \
            rawptr$(pp_join($, SliS, T)) sentinel_terminated;                                   \
            rawptr$(pp_join($, Sli, T)) regular;                                                \
        };                                                                                      \
        pp_join($, __AssociationTypes_Sli_const, T) as_const;                                   \
    }

#define comp_type_alias__Sli_const$(T) \
    pp_join($, Sli_const, T)
#define comp_type_alias__Sli$(T) \
    pp_join($, Sli, T)
#define comp_type_anon__Sli_const$$(T)  \
    struct {                            \
        rawptr_const$(T) ptr;           \
        usize      len;                 \
        Sli_const* __base_type_hint[0]; \
    }
#define comp_type_anon__Sli$$(T)  \
    union {                       \
        struct {                  \
            rawptr$(T) ptr;       \
            usize len;            \
        };                        \
        Sli_const$$(T) as_const;  \
        Sli* __base_type_hint[0]; \
    }
#define comp_op__Sli_anonCast$(__anon, T_Sli, var_anon...) eval({                       \
    let_(__anon, TypeOf(&var_anon)) = &var_anon;                                        \
    claim_assert_static(sizeOf(TypeOf(__anon)) == sizeOf(T_Sli));                       \
    claim_assert_static(alignOf(TypeOf(__anon)) == alignOf(T_Sli));                     \
    claim_assert_static(validateField(TypeOf(__anon), ptr, FieldType$(T_Sli, ptr)));    \
    claim_assert_static(validateField(TypeOf(__anon), len, FieldType$(T_Sli, len)));    \
    claim_assert_static(fieldPadding(TypeOf(__anon), ptr) == fieldPadding(T_Sli, ptr)); \
    claim_assert_static(fieldPadding(TypeOf(__anon), len) == fieldPadding(T_Sli, len)); \
    eval_return_(*as$(rawptr$(T_Sli), &__anon));                                        \
})

#define comp_op__Sli_from(val_ptr, val_len...)         { .ptr = ensureNonnull(val_ptr), .len = val_len }
#define comp_op__Sli_from$(T_Sli, val_ptr, val_len...) ((T_Sli)Sli_from(val_ptr, val_len))
#define comp_op__Sli_arr(__arr, val_arr...)            eval({ \
    let_(__arr, TypeOf(&val_arr)) = &val_arr;                 \
    eval_return Sli_from$(                                    \
        Sli$$(TypeOf(__arr->buf[0])),                         \
        __arr->buf,                                           \
        Arr_len(*__arr)                                       \
    );                                                        \
})
#define comp_op__Sli_arr$(__arr, T_Sli, val_arr...) eval({ \
    let_(__arr, TypeOf(&val_arr)) = &val_arr;              \
    eval_return Sli_from$(                                 \
        T_Sli,                                             \
        __arr->buf,                                        \
        Arr_len(*__arr)                                    \
    );                                                     \
})
#define comp_op__Sli_asg(__addr_sli, var_addr_sli, val_sli...) eval({      \
    let_(__addr_sli, TypeOf(&var_addr_sli)) = &var_addr_sli;               \
    deref(__addr_sli)                       = TypeOf(*__addr_sli) val_sli; \
    eval_return __addr_sli;                                                \
})

#define comp_op__Sli_len(var_self...)                              ((var_self).len)
#define comp_op__Sli_at(__self, __index, var_self, usize_index...) eval({ \
    let_(__self, TypeOf(var_self)) = var_self;                            \
    let_(__index, usize)           = usize_index;                         \
    debug_assert_nonnull(__self.ptr);                                     \
    debug_assert_fmt(                                                     \
        __index < __self.len,                                             \
        "Index out of bounds: %zu >= %zu",                                \
        __index,                                                          \
        __self.len                                                        \
    );                                                                    \
    eval_return_(&__self.ptr[__index]);                                   \
})
#define comp_op__Sli_getAt(__self, __index, var_self, usize_index...) eval({ \
    let_(__self, TypeOf(var_self)) = var_self;                               \
    let_(__index, usize)           = usize_index;                            \
    debug_assert_nonnull(__self.ptr);                                        \
    debug_assert_fmt(                                                        \
        __index < __self.len,                                                \
        "Index out of bounds: %zu >= %zu",                                   \
        __index,                                                             \
        __self.len                                                           \
    );                                                                       \
    eval_return __self.ptr[__index];                                         \
})
#define comp_op__Sli_setAt(__self, __index, var_self, usize_index, val_item...) eval({ \
    let_(__self, TypeOf(var_self)) = var_self;                                         \
    let_(__index, usize)           = usize_index;                                      \
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
    let_(__self, TypeOf(var_self)) = var_self;                                         \
    let_(__range, Range)           = Range_from range_index_begin_end;                 \
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
#define comp_op__Sli_sliceZ(__self, __range, var_self, range_index_begin_end...) eval({ \
    let_(__self, TypeOf(var_self)) = var_self;                                          \
    let_(__range, Range)           = Range_from range_index_begin_end;                  \
    debug_assert_fmt(                                                                   \
        __range.begin < __range.end,                                                    \
        "Invalid slice range: begin(%zu) >= end(%zu)",                                  \
        __range.begin,                                                                  \
        __range.end                                                                     \
    );                                                                                  \
    debug_assert_fmt(                                                                   \
        __range.end <= __self.len,                                                      \
        "Index out of bounds: end(%zu) > len(%zu)",                                     \
        __range.end,                                                                    \
        __self.len                                                                      \
    );                                                                                  \
    debug_assert_fmt(                                                                   \
        bti_memcmp(                                                                     \
            as$(u8*, &__self.ptr[__range.end]),                                         \
            as$(u8*, &make$(TypeOf(*__self.ptr))),                                      \
            sizeOf$(TypeOf(*__self.ptr))                                                \
        ) == 0,                                                                         \
        "The slice is not zero-terminated"                                              \
    );                                                                                  \
    eval_return make$(                                                                  \
        TypeOf(__self.__association_types_hint[0][0].zero_terminated[0]),               \
        .ptr = __self.ptr + __range.begin                                               \
    );                                                                                  \
})
#define comp_op__Sli_sliceS(__self, __range, __sentinel, var_self, range_index_begin_end, val_sentinel...) eval({ \
    let_(__self, TypeOf(var_self))         = var_self;                                                            \
    let_(__range, Range)                   = Range_from range_index_begin_end;                                    \
    let_(__sentinel, TypeOf(val_sentinel)) = val_sentinel;                                                        \
    debug_assert_fmt(                                                                                             \
        __range.begin < __range.end,                                                                              \
        "Invalid slice range: begin(%zu) >= end(%zu)",                                                            \
        __range.begin,                                                                                            \
        __range.end                                                                                               \
    );                                                                                                            \
    debug_assert_fmt(                                                                                             \
        __range.end <= __self.len,                                                                                \
        "Index out of bounds: end(%zu) > len(%zu)",                                                               \
        __range.end,                                                                                              \
        __self.len                                                                                                \
    );                                                                                                            \
    debug_assert_fmt(                                                                                             \
        bti_memcmp(                                                                                               \
            as$(u8*, &__self.ptr[__range.end]),                                                                   \
            as$(u8*, &__sentinel),                                                                                \
            sizeOf$(TypeOf(*__self.ptr))                                                                          \
        ) == 0,                                                                                                   \
        "The slice is not sentinel-terminated"                                                                    \
    );                                                                                                            \
    eval_return make$(                                                                                            \
        TypeOf(__self.__association_types_hint[0][0].sentinel_terminated[0]),                                     \
        .ptr      = __self.ptr + __range.begin,                                                                   \
        .sentinel = __sentinel                                                                                    \
    );                                                                                                            \
})
#define comp_op__Sli_prefix(__self, __end, var_self, usize_index_end...) eval({ \
    let_(__self, TypeOf(var_self)) = var_self;                                  \
    let_(__end, usize)             = usize_index_end;                           \
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
#define comp_op__Sli_prefixZ(__self, var_self...) eval({                  \
    let_(__self, TypeOf(var_self)) = var_self;                            \
    debug_assert_fmt(                                                     \
        bti_memcmp(                                                       \
            as$(u8*, &__self.ptr[__self.len]),                            \
            as$(u8*, &make$(TypeOf(*__self.ptr))),                        \
            sizeOf$(TypeOf(*__self.ptr))                                  \
        ) == 0,                                                           \
        "The slice is not zero-terminated"                                \
    );                                                                    \
    eval_return make$(                                                    \
        TypeOf(__self.__association_types_hint[0][0].zero_terminated[0]), \
        .ptr = __self.ptr                                                 \
    );                                                                    \
})
#define comp_op__Sli_prefixS(__self, __sentinel, var_self, val_sentinel...) eval({ \
    let_(__self, TypeOf(var_self))         = var_self;                             \
    let_(__sentinel, TypeOf(val_sentinel)) = val_sentinel;                         \
    debug_assert_fmt(                                                              \
        bti_memcmp(                                                                \
            as$(u8*, &__self.ptr[__self.len]),                                     \
            as$(u8*, &__sentinel),                                                 \
            sizeOf$(TypeOf(*__self.ptr))                                           \
        ) == 0,                                                                    \
        "The slice is not sentinel-terminated"                                     \
    );                                                                             \
    eval_return make$(                                                             \
        TypeOf(__self.__association_types_hint[0][0].sentinel_terminated[0]),      \
        .ptr      = __self.ptr,                                                    \
        .sentinel = __sentinel                                                     \
    );                                                                             \
})
#define comp_op__Sli_suffix(__self, __begin, var_self, usize_index_begin...) eval({ \
    let_(__self, TypeOf(var_self)) = var_self;                                      \
    let_(__begin, usize)           = usize_index_begin;                             \
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
    let_(__self, TypeOf(var_self)) = var_self;                                       \
    let_(__begin, usize)           = usize_index_begin;                              \
    debug_assert_fmt(                                                                \
        __begin < __self.len,                                                        \
        "Index out of bounds: %zu > %zu",                                            \
        __begin,                                                                     \
        __self.len                                                                   \
    );                                                                               \
    debug_assert_fmt(                                                                \
        bti_memcmp(                                                                  \
            as$(u8*, &__self.ptr[__self.len]),                                       \
            as$(u8*, &make$(TypeOf(*__self.ptr))),                                   \
            sizeOf$(TypeOf(*__self.ptr))                                             \
        ) == 0,                                                                      \
        "The slice is not zero-terminated"                                           \
    );                                                                               \
    eval_return make$(                                                               \
        TypeOf(__self.__association_types_hint[0][0].zero_terminated[0]),            \
        .ptr = __self.ptr + __begin                                                  \
    );                                                                               \
})
#define comp_op__Sli_suffixS(__self, __begin, __sentinel, var_self, usize_index_begin, val_sentinel...) eval({ \
    let_(__self, TypeOf(var_self))         = var_self;                                                         \
    let_(__begin, usize)                   = usize_index_begin;                                                \
    let_(__sentinel, TypeOf(val_sentinel)) = val_sentinel;                                                     \
    debug_assert_fmt(                                                                                          \
        __begin < __self.len,                                                                                  \
        "Index out of bounds: %zu > %zu",                                                                      \
        __begin,                                                                                               \
        __self.len                                                                                             \
    );                                                                                                         \
    debug_assert_fmt(                                                                                          \
        bti_memcmp(                                                                                            \
            as$(u8*, &__self.ptr[__self.len]),                                                                 \
            as$(u8*, &__sentinel),                                                                             \
            sizeOf$(TypeOf(*__self.ptr))                                                                       \
        ) == 0,                                                                                                \
        "The slice is not sentinel-terminated"                                                                 \
    );                                                                                                         \
    eval_return make$(                                                                                         \
        TypeOf(__self.__association_types_hint[0][0].sentinel_terminated[0]),                                  \
        .ptr      = __self.ptr + __begin,                                                                      \
        .sentinel = __sentinel                                                                                 \
    );                                                                                                         \
})

#define comp_syn__for_slice(__sli, __i, var_sli, _Iter_item) \
    with_(let __sli = (var_sli)) for (usize __i = 0; (__i) < (__sli).len; ++(__i)) with_(var _Iter_item = Sli_at(__sli, __i))
#define comp_syn__for_slice_indexed(__sli, var_sli, _Iter_item, _Iter_index) \
    with_(let __sli = (var_sli)) for (usize _Iter_index = 0; (_Iter_index) < (__sli).len; ++(_Iter_index)) with_(let _Iter_item = Sli_at(__sli, _Iter_index))
#define comp_syn__for_slice_rev(__sli, __i, var_sli, _Iter_item) \
    with_(let __sli = (var_sli)) for (usize __i = (__sli).len; (__i)-- > 0;) with_(let _Iter_item = Sli_at(__sli, __i))
#define comp_syn__for_slice_rev_indexed(__sli, var_sli, _Iter_item, _Iter_index) \
    with_(let __sli = (var_sli)) for (usize _Iter_index = (__sli).len; (_Iter_index)-- > 0;) with_(let _Iter_item = Sli_at(__sli, _Iter_index))


#define comp_type_anon__SliZ_const$(T)   \
    struct {                             \
        rawptr_const$(T) ptr;            \
        SliZ_const* __base_type_hint[0]; \
    }
#define comp_type_anon__SliZ$(T)   \
    struct {                       \
        rawptr$(T) ptr;            \
        SliZ* __base_type_hint[0]; \
    }
#define comp_op__SliZ_anonCast$(T_NamedSliZ, var_anon_sli_z)

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
    eval_return_(&__self.ptr[__index]);                                    \
})
#define comp_op__SliZ_getAt(__self, __index, var_self, usize_index...) eval({ \
    let         __self  = var_self;                                           \
    const usize __index = usize_index;                                        \
    debug_assert_nonnull(__self.ptr);                                         \
    eval_return __self.ptr[__index];                                          \
})
#define comp_op__SliZ_setAt(__self, __index, var_self, usize_index, val_item...) eval({ \
    let         __self  = var_self;                                                     \
    const usize __index = usize_index;                                                  \
    debug_assert_nonnull(__self.ptr);                                                   \
    __self.ptr[__index] = val_item;                                                     \
    eval_return __self;                                                                 \
})


#define comp_type_anon__SliS_const$(T)   \
    struct {                             \
        rawptr_const$(T) ptr;            \
        T           sentinel;            \
        SliS_const* __base_type_hint[0]; \
    }
#define comp_type_anon__SliS$(T)   \
    struct {                       \
        rawptr$(T) ptr;            \
        T     sentinel;            \
        SliS* __base_type_hint[0]; \
    }
#define comp_op__SliS_anonCast$(T_NamedSliS, var_anon_sli_s)

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
    eval_return __self.ptr + __index;                                      \
})
#define comp_op__SliS_getAt(__self, __index, var_self, usize_index...) eval({ \
    let         __self  = var_self;                                           \
    const usize __index = usize_index;                                        \
    debug_assert_nonnull(__self.ptr);                                         \
    eval_return __self.ptr[__index];                                          \
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
#include "dh/Arr.h"
use_Sli$(u8);
use_Sli$(i32);

void slice_example(void) {
    // Create a slice from an array
    Arr$$(10, i32) c = Arr_init({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    let slice        = Sli_arr$(Sli$i32, c);

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
