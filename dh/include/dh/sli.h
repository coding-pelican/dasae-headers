/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    sli.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-05 (date of creation)
 * @updated 2025-03-13 (date of last update)
 * @version v0.1-alpha.3
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Generic slice operations
 * @details Provides generic slice types that can be used with any element type:
 *          - SZ: Zero(Null)-terminated slice (like C strings)
 *          - SS: Sentinel-terminated slice (with custom sentinel value)
 *          - S: Regular slice with ptr and len
 *          with safe bounds checking and flexible range operations.
 */

#ifndef SLI_INCLUDED
#define SLI_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/types/Sli.h"

#if UNUSED_CODE
/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/Range.h"

/*========== Macros and Declarations ========================================*/

typedef struct S_const$raw {
    P_const$raw ptr;
    usize       len;
} S_const$raw;
typedef union S$raw {
    struct {
        P$raw ptr;
        usize len;
    };
    S_const$raw as_const;
} S$raw;

/* Slice ====================================================================*/

#define use_S$(_T)  comp_type_gen__use_S$(_T)
#define decl_S$(_T) comp_type_gen__decl_S$(_T)
#define impl_S$(_T) comp_type_gen__impl_S$(_T)

#define S_const$(_T)                   comp_type_alias__S_const$(_T)
#define S$(_T)                         comp_type_alias__S$(_T)
#define S_const$$(_T)                  comp_type_anon__S_const$$(_T)
#define S$$(_T)                        comp_type_anon__S$$(_T)
#define S_anonCast$(_T_S, var_anon...) comp_op__S_anonCast$(pp_uniqTok(__anon), _T_S, var_anon)

/* Core Slice Operations */

#define S_from(val_ptr, val_len...)       comp_op__S_from(val_ptr, val_len)
#define S_from$(T_S, val_ptr, val_len...) comp_op__S_from$(T_S, val_ptr, val_len)
#define S_arr(val_arr...)                 comp_op__S_arr(pp_uniqTok(arr), val_arr)
#define S_arr$(T_S, val_arr...)           comp_op__S_arr$(pp_uniqTok(arr), T_S, val_arr)
#define S_asg(var_addr_sli, val_sli...)   comp_op__S_asg(var_addr_sli, val_sli)

#define S_deref$(_A$N$T, _val_sli...) comp_op__S_deref$(_A$N$T, _val_sli)

#define ptr$S(var_self...)             comp_op__S_ptr(var_self)
#define len$S(var_self...)             comp_op__S_len(var_self)
#define at$S(var_self, usize_index...) S_at(var_self, usize_index)

#define S_ptr(var_self...)                          comp_op__S_ptr(var_self)
#define S_len(var_self...)                          comp_op__S_len(var_self)
#define S_at(var_self, usize_index...)              comp_op__S_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define S_getAt(var_self, usize_index...)           comp_op__S_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define S_setAt(var_self, usize_index, val_item...) comp_op__S_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

/* Range-based Slice Operations */

#define slice$S(var_self, range_index_begin_end...) /* sli[begin..end] */ \
    comp_op__S_slice(pp_uniqTok(self), pp_uniqTok(range), var_self, range_index_begin_end)
#define prefix$S(var_self, usize_index_end...) /* sli[0..index] */ \
    comp_op__S_prefix(pp_uniqTok(self), pp_uniqTok(end), var_self, usize_index_end)
#define suffix$S(var_self, usize_index_begin...) /* sli[index..len] */ \
    comp_op__S_suffix(pp_uniqTok(self), pp_uniqTok(begin), var_self, usize_index_begin)

#define S_slice(var_self, range_index_begin_end...) /* sli[begin..end] */ \
    comp_op__S_slice(pp_uniqTok(self), pp_uniqTok(range), var_self, range_index_begin_end)
#define S_sliceZ(var_self, range_index_begin_end...) /* sli[begin..end] */ \
    comp_op__S_sliceZ(pp_uniqTok(self), pp_uniqTok(range), var_self, range_index_begin_end)
#define S_sliceS(var_self, range_index_begin_end, val_sentinel...) /* sli[begin..end] */ \
    comp_op__S_sliceS(pp_uniqTok(self), pp_uniqTok(range), pp_uniqTok(sentinel), var_self, range_index_begin_end, val_sentinel)
#define S_prefix(var_self, usize_index_end...) /* sli[0..index] */ \
    comp_op__S_prefix(pp_uniqTok(self), pp_uniqTok(end), var_self, usize_index_end)
#define S_prefixZ(var_self...) /* sli[0..*:null] */ \
    comp_op__S_prefixZ(pp_uniqTok(self), var_self)
#define S_prefixS(var_self, val_sentinel...) /* sli[0..*:sentinel] */ \
    comp_op__S_prefixS(pp_uniqTok(self), pp_uniqTok(sentinel), var_self, val_sentinel)
#define S_suffix(var_self, usize_index_begin...) /* sli[index..len] */ \
    comp_op__S_suffix(pp_uniqTok(self), pp_uniqTok(begin), var_self, usize_index_begin)
#define S_suffixZ(var_self, usize_index_begin...) /* sli[index..*:null] */ \
    comp_op__S_suffixZ(pp_uniqTok(self), pp_uniqTok(begin), var_self, usize_index_begin)
#define S_suffixS(var_self, usize_index_begin, val_sentinel...) /* sli[index..*:sentinel] */ \
    comp_op__S_suffixS(pp_uniqTok(self), pp_uniqTok(begin), pp_uniqTok(sentinel), var_self, usize_index_begin, val_sentinel)

/* Null-terminated slice (similar to C strings) =============================*/

/**
 * @brief Null-terminated slice (similar to C strings)
 * @tparam T Element type
 */
#define SZ_const$(T)                            comp_type_anon__SZ_const$(T)
#define SZ$(T)                                  comp_type_anon__SZ$(T)
#define SZ_anonCast$(T_NamedSZ, var_anon_sli_z) comp_op__SZ_anonCast$(T_NamedSZ, var_anon_sli_z)

#define SZ_from(val_ptr...)              comp_op__SZ_from(val_ptr)
#define SZ_from$(T_SZ, val_ptr...)       comp_op__SZ_from$(T_SZ, val_ptr)
#define SZ_asg(var_addr_sli, val_sli...) comp_op__SZ_asg(var_addr_sli, val_sli)

#define SZ_at(var_self, usize_index...)              comp_op__SZ_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define SZ_getAt(var_self, usize_index...)           comp_op__SZ_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define SZ_setAt(var_self, usize_index, val_item...) comp_op__SZ_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

/* Sentinel-terminated slice (ends with a specific sentinel value) ==========*/

/**
 * @brief Sentinel-terminated slice (ends with a specific sentinel value)
 * @tparam T Element type
 */
#define SS_const$(T)                            comp_type_anon__SS_const$(T)
#define SS$(T)                                  comp_type_anon__SS$(T)
#define SS_anonCast$(T_NamedSS, var_anon_sli_s) comp_op__SS_anonCast$(T_NamedSS, var_anon_sli_s)

#define SS_from(val_ptr, val_sentinel...)        comp_op__SS_from(val_ptr, val_sentinel)
#define SS_from$(T_SS, val_ptr, val_sentinel...) comp_op__SS_from$(T_SS, val_ptr, val_sentinel)
#define SS_asg(var_addr_sli, val_sli...)         comp_op__SS_asg(var_addr_sli, val_sli)

#define SS_at(var_self, usize_index...)              comp_op__SS_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define SS_getAt(var_self, usize_index...)           comp_op__SS_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define SS_setAt(var_self, usize_index, val_item...) comp_op__SS_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__use_S$(_T) \
    decl_S$(_T); \
    impl_S$(_T)
#define comp_type_gen__decl_S$(_T) \
    $maybe_unused typedef struct S_const$(_T) S_const$(_T); \
    $maybe_unused typedef union S$(_T) S$(_T)
#define comp_type_gen__impl_S$(_T) \
    struct S_const$(_T) { \
        var_(ptr, $P_const$(_T)); \
        var_(len, usize); \
    }; \
    union S$(_T) { \
        var_(as_const, S_const$(_T)); \
        struct { \
            var_(ptr, $P$(_T)); \
            var_(len, usize); \
        }; \
    }

#define comp_type_alias__S_const$(_T) \
    pp_join($, S_const, _T)
#define comp_type_alias__S$(_T) \
    pp_join($, S, _T)
#define comp_type_anon__S_const$$(_T) \
    struct { \
        var_(ptr, $P_const$(_T)); \
        var_(len, usize); \
    }
#define comp_type_anon__S$$(_T) \
    union { \
        var_(as_const, S_const$$(_T)); \
        struct { \
            var_(ptr, $P$(_T)); \
            var_(len, usize); \
        }; \
    }
#define comp_op__S_anonCast$(__anon, T_S, var_anon...) blk({ \
    let_(__anon, TypeOf(&var_anon)) = &var_anon; \
    claim_assert_static(sizeOf(TypeOf(__anon)) == sizeOf(T_S)); \
    claim_assert_static(alignOf(TypeOf(__anon)) == alignOf(T_S)); \
    claim_assert_static(validateField(TypeOf(__anon), ptr, FieldTypeOf(T_S, ptr))); \
    claim_assert_static(validateField(TypeOf(__anon), len, FieldTypeOf(T_S, len))); \
    claim_assert_static(fieldPadding(TypeOf(__anon), ptr) == fieldPadding(T_S, ptr)); \
    claim_assert_static(fieldPadding(TypeOf(__anon), len) == fieldPadding(T_S, len)); \
    blk_return_(*as$(($P$(T_S))(&__anon))); \
})

#define comp_op__S_from(val_ptr, val_len...)       { .ptr = ensureNonnull(val_ptr), .len = val_len }
#define comp_op__S_from$(T_S, val_ptr, val_len...) ((T_S)S_from(val_ptr, val_len))
#define comp_op__S_arr(__arr, val_arr...)          blk({ \
    let_(__arr, TypeOf(&val_arr)) = &val_arr; \
    blk_return S_from$( \
        S$$(TypeOf(__arr->val[0])), \
        __arr->val, \
        A_len(*__arr) \
    ); \
})
#define comp_op__S_arr$(__arr, T_S, val_arr...) blk({ \
    let_(__arr, TypeOf(&val_arr)) = &val_arr; \
    blk_return S_from$( \
        T_S, \
        __arr->val, \
        A_len(*__arr) \
    ); \
})
#define comp_op__S_asg(__addr_sli, var_addr_sli, val_sli...) blk({ \
    let_(__addr_sli, TypeOf(&var_addr_sli)) = &var_addr_sli; \
    deref(__addr_sli)                       = TypeOf(*__addr_sli) val_sli; \
    blk_return __addr_sli; \
})

#define comp_op__S_deref$(_A$N$T, _val_sli...) (*blk({ \
    let __sli = _val_sli; \
    debug_assert_nonnull(__sli.ptr); \
    let __arr_ptr = as$((_A$N$T*)(__sli.ptr)); \
    debug_assert(__sli.len == A_len(*__arr_ptr)); \
    blk_return __arr_ptr; \
}))

#define comp_op__S_ptr(var_self...) \
    ((var_self).ptr)
#define comp_op__S_len(var_self...) \
    ((var_self).len)
#define comp_op__S_at(__self, __index, var_self, usize_index...) blk({ \
    let_(__self, TypeOf(var_self)) = var_self; \
    let_(__index, usize)           = usize_index; \
    debug_assert_nonnull(__self.ptr); \
    debug_assert_fmt( \
        __index < __self.len, \
        "Index out of bounds: %zu >= %zu", \
        __index, \
        __self.len \
    ); \
    blk_return_(&__self.ptr[__index]); \
})
#define comp_op__S_getAt(__self, __index, var_self, usize_index...) blk({ \
    let_(__self, TypeOf(var_self)) = var_self; \
    let_(__index, usize)           = usize_index; \
    debug_assert_nonnull(__self.ptr); \
    debug_assert_fmt( \
        __index < __self.len, \
        "Index out of bounds: %zu >= %zu", \
        __index, \
        __self.len \
    ); \
    blk_return __self.ptr[__index]; \
})
#define comp_op__S_setAt(__self, __index, var_self, usize_index, val_item...) blk({ \
    let_(__self, TypeOf(var_self)) = var_self; \
    let_(__index, usize)           = usize_index; \
    debug_assert_nonnull(__self.ptr); \
    debug_assert_fmt( \
        __index < __self.len, \
        "Index out of bounds: %zu >= %zu", \
        __index, \
        __self.len \
    ); \
    __self.ptr[__index] = val_item; \
    blk_return __self; \
})

#define comp_op__S_slice(__self, __range, var_self, range_index_begin_end...) blk({ \
    let_(__self, TypeOf(var_self)) = var_self; \
    let_(__range, R)               = range_index_begin_end; \
    debug_assert_fmt( \
        __range.begin <= __range.end, \
        "Invalid slice range: begin(%zu) > end(%zu)", \
        __range.begin, \
        __range.end \
    ); \
    debug_assert_fmt( \
        __range.end <= __self.len, \
        "Index out of bounds: end(%zu) > len(%zu)", \
        __range.end, \
        __self.len \
    ); \
    blk_return make$( \
        TypeOf(__self), \
        .ptr = __self.ptr + __range.begin, \
        .len = R_len(__range) \
    ); \
})
#define comp_op__S_sliceZ(__self, __range, var_self, range_index_begin_end...) blk({ \
    let_(__self, TypeOf(var_self)) = var_self; \
    let_(__range, R)               = range_index_begin_end; \
    debug_assert_fmt( \
        __range.begin <= __range.end, \
        "Invalid slice range: begin(%zu) > end(%zu)", \
        __range.begin, \
        __range.end \
    ); \
    debug_assert_fmt( \
        __range.end <= __self.len, \
        "Index out of bounds: end(%zu) > len(%zu)", \
        __range.end, \
        __self.len \
    ); \
    debug_assert_fmt( \
        bti_memcmp( \
            as$((u8*)(&__self.ptr[__range.end])), \
            as$((u8*)(&make$(TypeOf(*__self.ptr)))), \
            sizeOf$(TypeOf(*__self.ptr)) \
        ) == 0, \
        "The slice is not zero-terminated" \
    ); \
    blk_return make$( \
        TypeOf(__self.__association_types_hint[0][0].zero_terminated[0]), \
        .ptr = __self.ptr + __range.begin \
    ); \
})
#define comp_op__S_sliceS(__self, __range, __sentinel, var_self, range_index_begin_end, val_sentinel...) blk({ \
    let_(__self, TypeOf(var_self))         = var_self; \
    let_(__range, R)                       = range_index_begin_end; \
    let_(__sentinel, TypeOf(val_sentinel)) = val_sentinel; \
    debug_assert_fmt( \
        __range.begin <= __range.end, \
        "Invalid slice range: begin(%zu) > end(%zu)", \
        __range.begin, \
        __range.end \
    ); \
    debug_assert_fmt( \
        __range.end <= __self.len, \
        "Index out of bounds: end(%zu) > len(%zu)", \
        __range.end, \
        __self.len \
    ); \
    debug_assert_fmt( \
        bti_memcmp( \
            as$((u8*)(&__self.ptr[__range.end])), \
            as$((u8*)(&__sentinel)), \
            sizeOf$(TypeOf(*__self.ptr)) \
        ) == 0, \
        "The slice is not sentinel-terminated" \
    ); \
    blk_return make$( \
        TypeOf(__self.__association_types_hint[0][0].sentinel_terminated[0]), \
        .ptr      = __self.ptr + __range.begin, \
        .sentinel = __sentinel \
    ); \
})
#define comp_op__S_prefix(__self, __end, var_self, usize_index_end...) blk({ \
    let_(__self, TypeOf(var_self)) = var_self; \
    let_(__end, usize)             = usize_index_end; \
    debug_assert_fmt( \
        __end <= __self.len, \
        "Index out of bounds: %zu > %zu", \
        __end, \
        __self.len \
    ); \
    blk_return make$( \
        TypeOf(__self), \
        .ptr = __self.ptr, \
        .len = __end \
    ); \
})
#define comp_op__S_prefixZ(__self, var_self...) blk({ \
    let_(__self, TypeOf(var_self)) = var_self; \
    debug_assert_fmt( \
        bti_memcmp( \
            as$((u8*)(&__self.ptr[__self.len])), \
            as$((u8*)(&make$(TypeOf(*__self.ptr)))), \
            sizeOf$(TypeOf(*__self.ptr)) \
        ) == 0, \
        "The slice is not zero-terminated" \
    ); \
    blk_return make$( \
        TypeOf(__self.__association_types_hint[0][0].zero_terminated[0]), \
        .ptr = __self.ptr \
    ); \
})
#define comp_op__S_prefixS(__self, __sentinel, var_self, val_sentinel...) blk({ \
    let_(__self, TypeOf(var_self))         = var_self; \
    let_(__sentinel, TypeOf(val_sentinel)) = val_sentinel; \
    debug_assert_fmt( \
        bti_memcmp( \
            as$((u8*)(&__self.ptr[__self.len])), \
            as$((u8*)(&__sentinel)), \
            sizeOf$(TypeOf(*__self.ptr)) \
        ) == 0, \
        "The slice is not sentinel-terminated" \
    ); \
    blk_return make$( \
        TypeOf(__self.__association_types_hint[0][0].sentinel_terminated[0]), \
        .ptr      = __self.ptr, \
        .sentinel = __sentinel \
    ); \
})
#define comp_op__S_suffix(__self, __begin, var_self, usize_index_begin...) blk({ \
    let_(__self, TypeOf(var_self)) = var_self; \
    let_(__begin, usize)           = usize_index_begin; \
    debug_assert_fmt( \
        __begin <= __self.len, \
        "Index out of bounds: %zu > %zu", \
        __begin, \
        __self.len \
    ); \
    blk_return make$( \
        TypeOf(__self), \
        .ptr = __self.ptr + __begin, \
        .len = __self.len - __begin \
    ); \
})
#define comp_op__S_suffixZ(__self, __begin, var_self, usize_index_begin...) blk({ \
    let_(__self, TypeOf(var_self)) = var_self; \
    let_(__begin, usize)           = usize_index_begin; \
    debug_assert_fmt( \
        __begin <= __self.len, \
        "Index out of bounds: %zu > %zu", \
        __begin, \
        __self.len \
    ); \
    debug_assert_fmt( \
        bti_memcmp( \
            as$((u8*)(&__self.ptr[__self.len])), \
            as$((u8*)(&make$(TypeOf(*__self.ptr)))), \
            sizeOf$(TypeOf(*__self.ptr)) \
        ) == 0, \
        "The slice is not zero-terminated" \
    ); \
    blk_return make$( \
        TypeOf(__self.__association_types_hint[0][0].zero_terminated[0]), \
        .ptr = __self.ptr + __begin \
    ); \
})
#define comp_op__S_suffixS(__self, __begin, __sentinel, var_self, usize_index_begin, val_sentinel...) blk({ \
    let_(__self, TypeOf(var_self))         = var_self; \
    let_(__begin, usize)                   = usize_index_begin; \
    let_(__sentinel, TypeOf(val_sentinel)) = val_sentinel; \
    debug_assert_fmt( \
        __begin <= __self.len, \
        "Index out of bounds: %zu > %zu", \
        __begin, \
        __self.len \
    ); \
    debug_assert_fmt( \
        bti_memcmp( \
            as$((u8*)(&__self.ptr[__self.len])), \
            as$((u8*)(&__sentinel)), \
            sizeOf$(TypeOf(*__self.ptr)) \
        ) == 0, \
        "The slice is not sentinel-terminated" \
    ); \
    blk_return make$( \
        TypeOf(__self.__association_types_hint[0][0].sentinel_terminated[0]), \
        .ptr      = __self.ptr + __begin, \
        .sentinel = __sentinel \
    ); \
})

#define comp_syn__for_slice(__sli, __i, var_sli, _Iter_item) \
    with_(let __sli = (var_sli)) for (usize __i = 0; (__i) < (__sli).len; ++(__i)) with_(var _Iter_item = S_at(__sli, __i))
#define comp_syn__for_slice_indexed(__sli, var_sli, _Iter_item, _Iter_index) \
    with_(let __sli = (var_sli)) for (usize _Iter_index = 0; (_Iter_index) < (__sli).len; ++(_Iter_index)) with_(let _Iter_item = S_at(__sli, _Iter_index))
#define comp_syn__for_slice_rev(__sli, __i, var_sli, _Iter_item) \
    with_(let __sli = (var_sli)) for (usize __i = (__sli).len; (__i)-- > 0;) with_(let _Iter_item = S_at(__sli, __i))
#define comp_syn__for_slice_rev_indexed(__sli, var_sli, _Iter_item, _Iter_index) \
    with_(let __sli = (var_sli)) for (usize _Iter_index = (__sli).len; (_Iter_index)-- > 0;) with_(let _Iter_item = S_at(__sli, _Iter_index))


#define comp_type_anon__SZ_const$(T) \
    struct { \
        $P_const$(T) ptr; \
        SZ_const* __base_type_hint[0]; \
    }
#define comp_type_anon__SZ$(T) \
    struct { \
        $P$(T) \
        ptr; \
        SZ* __base_type_hint[0]; \
    }
#define comp_op__SZ_anonCast$(T_NamedSZ, var_anon_sli_z)

#define comp_op__SZ_from(val_ptr...)                          { .ptr = ensureNonnull(val_ptr) }
#define comp_op__SZ_from$(T_SZ, val_ptr...)                   ((T_SZ)SZ_from(val_ptr))
#define comp_op__SZ_asg(__addr_sli, var_addr_sli, val_sli...) blk({ \
    let __addr_sli = var_addr_sli; \
    debug_assert_nonnull(__addr_sli); \
    *__addr_sli = TypeOf(*__addr_sli) val_sli; \
    blk_return __addr_sli; \
})

#define comp_op__SZ_at(__self, __index, var_self, usize_index...) blk({ \
    let         __self  = var_self; \
    const usize __index = usize_index; \
    debug_assert_nonnull(__self.ptr); \
    blk_return_(&__self.ptr[__index]); \
})
#define comp_op__SZ_getAt(__self, __index, var_self, usize_index...) blk({ \
    let         __self  = var_self; \
    const usize __index = usize_index; \
    debug_assert_nonnull(__self.ptr); \
    blk_return __self.ptr[__index]; \
})
#define comp_op__SZ_setAt(__self, __index, var_self, usize_index, val_item...) blk({ \
    let         __self  = var_self; \
    const usize __index = usize_index; \
    debug_assert_nonnull(__self.ptr); \
    __self.ptr[__index] = val_item; \
    blk_return __self; \
})


#define comp_type_anon__SS_const$(T) \
    struct { \
        $P_const$(T) ptr; \
        T         sentinel; \
        SS_const* __base_type_hint[0]; \
    }
#define comp_type_anon__SS$(T) \
    struct { \
        $P$(T) \
        ptr; \
        T   sentinel; \
        SS* __base_type_hint[0]; \
    }
#define comp_op__SS_anonCast$(T_NamedSS, var_anon_sli_s)

#define comp_op__SS_from(val_ptr, val_sentinel...)            { .ptr = ensureNonnull(val_ptr), .sentinel = val_sentinel }
#define comp_op__SS_from$(T_SS, val_ptr, val_sentinel...)     ((T_SS)SS_from(val_ptr, val_sentinel))
#define comp_op__SS_asg(__addr_sli, var_addr_sli, val_sli...) blk({ \
    let __addr_sli = var_addr_sli; \
    debug_assert_nonnull(__addr_sli); \
    *__addr_sli = TypeOf(*__addr_sli) val_sli; \
    blk_return __addr_sli; \
})

#define comp_op__SS_at(__self, __index, var_self, usize_index...) blk({ \
    let         __self  = var_self; \
    const usize __index = usize_index; \
    debug_assert_nonnull(__self.ptr); \
    blk_return __self.ptr + __index; \
})
#define comp_op__SS_getAt(__self, __index, var_self, usize_index...) blk({ \
    let         __self  = var_self; \
    const usize __index = usize_index; \
    debug_assert_nonnull(__self.ptr); \
    blk_return __self.ptr[__index]; \
})
#define comp_op__SS_setAt(__self, __index, var_self, usize_index, val_item...) blk({ \
    let         __self  = var_self; \
    const usize __index = usize_index; \
    debug_assert_nonnull(__self.ptr); \
    __self.ptr[__index] = val_item; \
    blk_return __self; \
})
#endif /* UNUSED_CODE */

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
#include "dh/Arr.h"
use_S$(u8);
use_S$(i32);

void slice_example(void) {
    // Create a slice from an array
    A$$(10, i32) c     = A_init({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    let           slice = S_arr$(S$i32, c);

    // Basic operations
    let third_ptr = S_at(slice, 2);    // Get pointer to the third element
    let value     = S_getAt(slice, 3); // Get value of the fourth element
    S_setAt(slice, 4, 50);             // Set the fifth element to 50
    let length = S_len(slice);         // Get the length (10)

    // Range-based operations
    let middle      = S_slice(slice, (2, 7)); // Elements 3 through 7
    let first_half  = S_prefix(slice, 5);     // First 5 elements
    let second_half = S_suffix(slice, 5);     // Last 5 elements

    let all           = S_prefixZ(slice);
    let terminated_10 = S_prefixS(slice, 10);
    for (var iter = SS_at(terminated_10, 0); deref(iter) != terminated_10.sentinel; ++iter) {
        printf("%d ", deref(iter));
    }

    // Create a null-terminated string
    u8 str[] = "Hello, world";

    // Working with null-terminated slices (SZ)
    let str_z = SZ_from$(SZ$u8, str);
    let ch    = SZ_getAt(str_z, 1); // 'e'
    // let hello_sli = SZ_toS$(S$u8, str_z); // Convert to regular slice

    // Working with sentinel-terminated slices (SS)
    i32 numbers_with_sentinel[] = { 1, 2, 3, 4, 5, -1, 6, 7 }; // -1 is sentinel
    let nums_s                  = SS_from$(SS$i32, numbers_with_sentinel, -1);
    // let s_len                   = SS_len(nums_s);             // 5 (up to but not including -1)
    let num                     = SS_getAt(nums_s, 2); // 3
    // let nums_sli                = SS_toS$(S$i32, nums_s); // Convert to regular slice

    // Type casting example
    // S$u32 bytes = S_castS$(S$u8, slice);  // View as bytes (i32 -> u8)
    // S$i32 ints  = S_castL$(S$i32, bytes); // Convert back to ints
}
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SLI_INCLUDED */

#if SLI_MOCKUP_IMPROVEMENT

#ifndef SLI_INCLUDED
#define SLI_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/Range.h"

/*========== Macros and Declarations ========================================*/

#define use_S$(T)  comp_type_gen__use_S$(T)
#define decl_S$(T) comp_type_gen__decl_S$(T)
#define impl_S$(T) comp_type_gen__impl_S$(T)

#define S_c$(T) comp_type_alias__S_c$(T)
#define S$(T)   comp_type_alias__S$(T)

#define S_from(_ptr, _len...)         comp_op__S_from(_ptr, _len)
#define S_from$(T_S, _ptr, _len...)   comp_op__S_from$(T_S, _ptr, _len)
#define S_asg(_addr_lhs, _val_rhs...) comp_op__S_asg(pp_uniqTok(addr_lhs), _addr_lhs, _val_rhs)

#define S_ptr(_self...)                  comp_op__S_ptr(_self)
#define S_len(_self...)                  comp_op__S_len(_self)
#define S_at(_self, _index...)           comp_op__S_at(pp_uniqTok(self), pp_uniqTok(index), _self, _index)
#define S_getAt(_self, _index...)        comp_op__S_getAt(_self, _index)
#define S_setAt(_self, _index, _item...) comp_op__S_setAt(_self, _index, _item)

#define S_slice(_self, _begin, _end...)         comp_op__S_slice(pp_uniqTok(self), pp_uniqTok(begin), pp_uniqTok(end), _self, _begin, _end)
#define S_prefix(_self, _end...)                comp_op__S_prefix(pp_uniqTok(self), pp_uniqTok(end), _self, _end)
#define S_suffix(_self, _begin...)              comp_op__S_suffix(pp_uniqTok(self), pp_uniqTok(begin), _self, _begin)
#define S_sliceZ$(T_SZ, _self, _begin, _end...) comp_op__S_sliceZ(pp_uniqTok(self), pp_uniqTok(begin), T_SZ, pp_uniqTok(end), _self, _begin, _end)
#define S_prefixZ$(T_SZ, _self, _end...)        comp_op__S_prefixZ(pp_uniqTok(self), pp_uniqTok(end), T_SZ, _self, _end)
#define S_suffixZ$(T_SZ, _self, _begin...)      comp_op__S_suffixZ(pp_uniqTok(self), pp_uniqTok(begin), T_SZ, _self, _begin)

#define use_SZ$(T)  comp_type_gen__use_SZ$(T)
#define decl_SZ$(T) comp_type_gen__decl_SZ$(T)
#define impl_SZ$(T) comp_type_gen__impl_SZ$(T)

#define SZ_c$(T) comp_type_alias__SZ_c$(T)
#define SZ$(T)   comp_type_alias__SZ$(T)

#define SZ_from(_ptr, _len...)         comp_op__SZ_from(_ptr, _len)
#define SZ_from$(T_SZ, _ptr, _len...)  comp_op__SZ_from$(T_SZ, _ptr, _len)
#define SZ_asg(_addr_lhs, _val_rhs...) comp_op__SZ_asg(_addr_lhs, _val_rhs)

#define SZ_ptr(_self...)                  comp_op__SZ_ptr(_self)
#define SZ_len(_self...)                  comp_op__SZ_len(_self)
#define SZ_at(_self, _index...)           comp_op__SZ_at(pp_uniqTok(self), pp_uniqTok(index), _self, _index)
#define SZ_getAt(_self, _index...)        comp_op__SZ_getAt(pp_uniqTok(self), pp_uniqTok(index), _self, _index)
#define SZ_setAt(_self, _index, _item...) comp_op__SZ_setAt(pp_uniqTok(self), pp_uniqTok(index), _self, _index, _item)

#define SZ_slice$(T_S, _self, _begin, _end...) comp_op__SZ_slice(pp_uniqTok(self), pp_uniqTok(begin), pp_uniqTok(end), T_S, _self, _begin, _end)
#define SZ_prefix$(T_S, _self, _end...)        comp_op__SZ_prefix(pp_uniqTok(self), pp_uniqTok(end), T_S, _self, _end)
#define SZ_suffix$(T_S, _self, _begin...)      comp_op__SZ_suffix(pp_uniqTok(self), pp_uniqTok(begin), T_S, _self, _begin)
#define SZ_sliceZ(_self, _begin, _end...)      comp_op__SZ_sliceZ(pp_uniqTok(self), pp_uniqTok(begin), pp_uniqTok(end), _self, _begin, _end)
#define SZ_prefixZ(_self, _end...)             comp_op__SZ_prefixZ(pp_uniqTok(self), pp_uniqTok(end), _self, _end)
#define SZ_suffixZ(_self, _begin...)           comp_op__SZ_suffixZ(pp_uniqTok(self), pp_uniqTok(begin), _self, _begin)

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__use_S$(T) \
    comp_type_gen__decl_S$(T); \
    comp_type_gen__impl_S$(T)
#define comp_type_gen__decl_S$(T) \
    $maybe_unused typedef struct S_c$(T) S_c$(T); \
    $maybe_unused typedef union S$(T) S$(T)
#define comp_type_gen__impl_S$(T) \
    struct S_c$(T) { \
        $P_const$(T) ptr; \
        usize len; \
    }; \
    union S$(T) { \
        S_c$(T) as_const; \
        struct { \
            $P$(T) \
            ptr; \
            usize len; \
        }; \
    }

#define comp_type_alias__S_c$(T) pp_join($, S_c, T)
#define comp_type_alias__S$(T)   pp_join($, S, T)

#define comp_op__S_from(_ptr, _len...) \
    { .ptr = ensureNonnull(_ptr), .len = _len }
#define comp_op__S_from$(T_S, _ptr, _len...) \
    ((T_S)S_from(_ptr, _len))
#define comp_op__S_asg(__addr_lhs, _addr_lhs, _val_rhs...) blk({ \
    let __addr_lhs = _addr_lhs; \
    *__addr_lhs    = *(&*(TypeOf (*__addr_lhs)[1]){ [0] = _val_rhs }); \
    blk_return __addr_lhs; \
})

#define comp_op__S_ptr(_self...) \
    ((_self).ptr)
#define comp_op__S_len(_self...) \
    ((_self).len)
#define comp_op__S_at(__self, __index, _self, _index...) blk({ \
    let __self           = _self; \
    let_(__index, usize) = _index; \
    debug_assert_nonnull(__self.ptr); \
    debug_assert(__index < __self.len); \
    blk_return __self.ptr + __index; \
})
#define comp_op__S_getAt(_self, _index...) \
    (*S_at(_self, _index))
#define comp_op__S_setAt(_self, _index, _item...) \
    (S_getAt(_self, _index) = *(&*TypeOf(S_getAt(_self, _index)[1]){ [0] = _item }))

#define comp_op__S_slice(__self, __begin, __end, _self, _begin, _end...) blk({ \
    let __self           = _self; \
    let_(__begin, usize) = _begin; \
    let_(__end, usize)   = _end; \
    debug_assert_nonnull(__self.ptr); \
    debug_assert(__begin < __end); \
    debug_assert(__end <= __self.len); \
    blk_return S_from$(TypeOf(__self), __self.ptr + __begin, __end - __begin); \
})
#define comp_op__S_prefix(__self, __end, _self, _end...) blk({ \
    let __self         = _self; \
    let_(__end, usize) = _end; \
    blk_return S_slice(__self, 0, __end); \
})
#define comp_op__S_suffix(__self, __begin, _self, _begin...) blk({ \
    let __self           = _self; \
    let_(__begin, usize) = _begin; \
    blk_return S_slice(__self, __begin, __self.len); \
})
// #define S_sliceZ$(T_SZ, _self, _begin, _end...) comp_op__S_sliceZ(pp_uniqTok(self), pp_uniqTok(begin), T_SZ, pp_uniqTok(end), _self, _begin, _end)
// #define S_prefixZ$(T_SZ, _self, _end...)        comp_op__S_prefixZ(pp_uniqTok(self), pp_uniqTok(end), T_SZ, _self, _end)
// #define S_suffixZ$(T_SZ, _self, _begin...)      comp_op__S_suffixZ(pp_uniqTok(self), pp_uniqTok(begin), T_SZ, _self, _begin)

#define comp_type_gen__use_SZ$(T) \
    comp_type_gen__decl_SZ$(T); \
    comp_type_gen__impl_SZ$(T)
#define comp_type_gen__decl_SZ$(T) \
    $maybe_unused typedef struct SZ_c$(T) SZ_c$(T); \
    $maybe_unused typedef union SZ$(T) SZ$(T)
#define comp_type_gen__impl_SZ$(T) \
    struct SZ_c$(T) { \
        $P_const$(T) ptr; \
        usize len; \
    }; \
    union SZ$(T) { \
        SZ_c$(T) as_const; \
        struct { \
            $P$(T) \
            ptr; \
            usize len; \
        }; \
    }

#define comp_type_alias__SZ_c$(T) pp_join($, SZ_c, T)
#define comp_type_alias__SZ$(T)   pp_join($, SZ, T)

#define comp_op__SZ_from(_ptr, _len...) \
    { .ptr = ensureNonnull(_ptr), .len = _len }
#define comp_op__SZ_from$(T_SZ, _ptr, _len...) \
    ((T_SZ)SZ_from(_ptr, _len))
#define comp_op__SZ_asg(__addr_lhs, _addr_lhs, _val_rhs...) blk({ \
    let __addr_lhs = _addr_lhs; \
    *__addr_lhs    = *(&*(TypeOf (*__addr_lhs)[1]){ [0] = _val_rhs }); \
    blk_return __addr_lhs; \
})

#define comp_op__SZ_ptr(_self...) \
    ((_self).ptr)
#define comp_op__SZ_len(_self...) \
    ((_self).len)
#define comp_op__SZ_at(__self, __index, _self, _index...) blk({ \
    let __self           = _self; \
    let_(__index, usize) = _index; \
    debug_assert_nonnull(__self.ptr); \
    debug_assert(__index <= __self.len); \
    debug_assert_fmt( \
        bti_memcmp( \
            as$((u8*)(&(__self.ptr + __self.len))), \
            as$((u8*)(&(TypeOf(*__self.ptr)){})), \
            sizeOf$(TypeOf(*__self.ptr)) \
        ) == 0, \
        "The slice is not zero-terminated" \
    ); \
    blk_return __self.ptr + __index; \
})
#define comp_op__SZ_getAt(_self, _index...) \
    (*SZ_at(_self, _index))
#define comp_op__SZ_setAt(_self, _index, _item...) \
    (SZ_getAt(_self, _index) = *(&*TypeOf(SZ_getAt(_self, _index)[1]){ [0] = _item }))

// #define SZ_slice$(T_S, _self, _begin, _end...) comp_op__SZ_slice(pp_uniqTok(self), pp_uniqTok(begin), pp_uniqTok(end), T_S, _self, _begin, _end)
// #define SZ_prefix$(T_S, _self, _end...)        comp_op__SZ_prefix(pp_uniqTok(self), pp_uniqTok(end), T_S, _self, _end)
// #define SZ_suffix$(T_S, _self, _begin...)      comp_op__SZ_suffix(pp_uniqTok(self), pp_uniqTok(begin), T_S, _self, _begin)
#define comp_op__SZ_sliceZ(__self, __begin, __end, _self, _begin, _end...) blk({ \
    let __self           = _self; \
    let_(__begin, usize) = _begin; \
    let_(__end, usize)   = _end; \
    debug_assert_nonnull(__self.ptr); \
    debug_assert(__begin < __end); \
    debug_assert(__end <= __self.len); \
    debug_assert_fmt( \
        bti_memcmp( \
            as$((u8*)(&(__self.ptr + __end))), \
            as$((u8*)(&(TypeOf(*__self.ptr){}))), \
            sizeOf$(TypeOf(*__self.ptr)) \
        ) == 0, \
        "The slice is not zero-terminated" \
    ); \
    blk_return SZ_from$(TypeOf(__self), __self.ptr + __begin, __end - __begin); \
})
#define comp_op__SZ_prefixZ(__self, __end, _self, _end...) blk({ \
    let __self         = _self; \
    let_(__end, usize) = _end; \
    blk_return SZ_sliceZ(__self, 0, __end); \
})
#define comp_op__SZ_suffixZ(__self, __begin, _self, _begin...) blk({ \
    let __self           = _self; \
    let_(__begin, usize) = _begin; \
    blk_return SZ_sliceZ(__self, __begin, __self.len); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SLI_INCLUDED */

#endif /* SLI_MOCKUP_IMPROVEMENT */
