/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  (none)
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

typedef ptrdiff_t ptrdiff;

typedef TypeOf(const void*) P_const$raw;
typedef TypeOf(void*) P$raw;

#define $P_const$(_T...) TypeOf(const _T*)
#define $P$(_T...) TypeOf(_T*)

#define ptrCast$(TDestRawptr, var_rawptr) \
    /** \
     * @brief Convert P$raw to pointer of type TDestRawptr \
     */ \
    comp_op__ptrCast$(TDestRawptr, var_rawptr)

#define intFromPtr(val_rawptr) \
    /** \
     * @brief Convert rawptr to int(usize) \
     */ \
    comp_op__ptrToInt(val_rawptr)
#define ptrToInt(val_rawptr) \
    /** \
     * @brief Convert rawptr to int(usize) \
     */ \
    comp_op__ptrToInt(val_rawptr)
#define ptrFromInt$ intToPtr$
#define rawptrFromInt$ intToPtr$
#define intToPtr$(TDestRawptr, val_int) \
    /** \
     * @brief Convert int to rawptr \
     */ \
    comp_op__intToPtr$(TDestRawptr, val_int)

#define ptrIsNull(var_rawptr) \
    /** \
     * @brief Check if pointer is null \
     */ \
    comp_op__ptrIsNull(var_rawptr)

#define ptrIsNonnull(var_rawptr) \
    /** \
     * @brief Check if pointer is non-null \
     */ \
    comp_op__ptrIsNonnull(var_rawptr)

#define ensureNonnull(val_ptr...) comp_op__ensureNonnull(pp_uniqTok(ptr), val_ptr)

#define rawref(var_src...) comp_op__rawref(var_src)
#define rawderef(val_ptr...) comp_op__rawderef(val_ptr)

#define ref(val_src...) comp_op__ref(val_src)
#define deref(val_ptr...) comp_op__deref(val_ptr)

/*========== Macros Implementation ==========================================*/

#define comp_op__ptrCast$(TDestRawptr, var_rawptr) ((TDestRawptr)(var_rawptr))
#define comp_op__ptrToInt(val_rawptr) ((usize)(val_rawptr)) // NOLINT
#define comp_op__intToPtr$(TDestRawptr, val_int) ((TDestRawptr)(val_int)) // NOLINT

#define comp_op__ptrIsNull(var_rawptr) ((var_rawptr) == null)
#define comp_op__ptrIsNonnull(var_rawptr) ((var_rawptr) != null)

#define comp_op__ensureNonnull(__ptr, val_ptr...) blk({ \
    TypeOf(*val_ptr)* __ptr = val_ptr; \
    blk_return claim_assert_nonnull(__ptr), __ptr; \
})

#define comp_op__rawref(var_src...) (&(var_src))
#define comp_op__rawderef(val_ptr...) (*(val_ptr))

#define comp_op__ref(val_src...) (&(val_src))
#define comp_op__deref(val_ptr...) (*ensureNonnull(val_ptr))

/*========== Validation Checks ==============================================*/

#if arch_bits_is_64bit
/* 64-bit */
claim_assert_static_msg(sizeof(P$raw) == 8, "pointer must be 8 bytes on 64-bit");
#elif arch_bits_is_32bit
/* 32-bit */
claim_assert_static_msg(sizeof(P$raw) == 4, "pointer must be 4 bytes on 32-bit");
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_PTR_INCLUDED */
