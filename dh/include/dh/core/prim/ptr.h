/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
 *
 * @brief   Pointer utilities
 * @details Provides pointer utilities and functions for:
 *          - Pointer definition
 *          - Pointer usage
 *          - Pointer operations
 *          - Pointer utilities
 */

#ifndef CORE_PRIM_PTR_INCLUDED
#define CORE_PRIM_PTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "null.h"

#include <stddef.h>

/*========== Macros and Definitions =========================================*/

typedef const void* voidptr_const;
typedef void*       voidptr;
typedef ptrdiff_t   ptrdiff;

#define anyptr_const \
    /** \
     * @brief Pointer to any type (const void*) \
     */ \
    comp_prim_type__anyptr_const

#define anyptr \
    /** \
     * @brief Pointer to any type (void*) \
     */ \
    comp_prim_type__anyptr

#define rawptr_const$(TSrc) \
    /** \
     * @brief Pointer to type (const TSrc*) \
     */ \
    comp_prim_type__rawptr_const$(TSrc)

#define rawptr$(TSrc) \
    /** \
     * @brief Pointer to type (TSrc*) \
     */ \
    comp_prim_type__rawptr$(TSrc)

#define rawptrCast$(TDestRawptr, var_rawptr) \
    /** \
     * @brief Convert anyptr to pointer of type TDestRawptr \
     */ \
    comp_op__rawptrCast$(TDestRawptr, var_rawptr)

#define intFromRawptr rawptrToInt
#define rawptrToInt(val_rawptr) \
    /** \
     * @brief Convert rawptr to int(usize) \
     */ \
    comp_op__rawptrToInt(val_rawptr)

#define rawptrFromInt$ intToRawptr
#define intToRawptr$(TDestRawptr, val_int) \
    /** \
     * @brief Convert int to rawptr \
     */ \
    comp_op__intToRawptr$(TDestRawptr, val_int)

#define rawptrIsNull(var_rawptr) \
    /** \
     * @brief Check if pointer is null \
     */ \
    comp_op__rawptrIsNull(var_rawptr)

#define rawptrIsNonnull(var_rawptr) \
    /** \
     * @brief Check if pointer is non-null \
     */ \
    comp_op__rawptrIsNonnull(var_rawptr)

#define ensureNonnull(val_ptr...) comp_op__ensureNonnull(pp_uniqTok(ptr), val_ptr)

#define rawref(var_src...)   comp_op__rawref(var_src)
#define rawderef(val_ptr...) comp_op__rawderef(val_ptr)

#define ref(val_src...)   comp_op__ref(val_src)
#define deref(val_ptr...) comp_op__deref(val_ptr)

/*========== Macros Implementation ==========================================*/

#define comp_prim_type__anyptr_const        const void*
#define comp_prim_type__anyptr              void*
#define comp_prim_type__rawptr_const$(TSrc) const TSrc*
#define comp_prim_type__rawptr$(TSrc)       TSrc*

#define comp_op__rawptrCast$(TDestRawptr, var_rawptr) ((TDestRawptr)(var_rawptr))
#define comp_op__rawptrToInt(val_rawptr)              ((usize)(val_rawptr))    // NOLINT
#define comp_op__intToRawptr$(TDestRawptr, val_int)   ((TDestRawptr)(val_int)) // NOLINT

#define comp_op__rawptrIsNull(var_rawptr)    ((var_rawptr) == null)
#define comp_op__rawptrIsNonnull(var_rawptr) ((var_rawptr) != null)

#define comp_op__ensureNonnull(__ptr, val_ptr...) eval({ \
    TypeOf(*val_ptr)* __ptr = val_ptr; \
    eval_return debug_assert_nonnull(__ptr), __ptr; \
})

#define comp_op__rawref(var_src...)   (&(var_src))
#define comp_op__rawderef(val_ptr...) (*(val_ptr))

#define comp_op__ref(val_src...)   (&(val_src))
#define comp_op__deref(val_ptr...) (*ensureNonnull(val_ptr))

/*========== Validation Checks ==============================================*/

#if bti_plat_64bit
/* 64-bit */
claim_assert_static_msg(sizeof(anyptr) == 8, "pointer must be 8 bytes on 64-bit");
#elif bti_plat_32bit
/* 32-bit */
claim_assert_static_msg(sizeof(anyptr) == 4, "pointer must be 4 bytes on 32-bit");
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_PTR_INCLUDED */
