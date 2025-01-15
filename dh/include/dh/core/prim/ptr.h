/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
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

#define anyptr_const                            \
    /**                                         \
     * @brief Pointer to any type (const void*) \
     */                                         \
    TYPE_anyptr_const

#define anyptr                            \
    /**                                   \
     * @brief Pointer to any type (void*) \
     */                                   \
    TYPE_anyptr

#define rawptr_const$(TSrc)                 \
    /**                                     \
     * @brief Pointer to type (const TSrc*) \
     */                                     \
    TYPE_rawptr_const$(TSrc)

#define rawptr$(TSrc)                 \
    /**                               \
     * @brief Pointer to type (TSrc*) \
     */                               \
    TYPE_rawptr$(TSrc)

#define rawptrCast$(TDestRawptr, var_rawptr)                \
    /**                                                     \
     * @brief Convert anyptr to pointer of type TDestRawptr \
     */                                                     \
    FUNC_rawptrCast$(TDestRawptr, var_rawptr)

#define rawptrToInt(val_rawptr)            \
    /**                                    \
     * @brief Convert rawptr to int(usize) \
     */                                    \
    FUNC_rawptrToInt(val_rawptr)

#define intToRawptr$(TDestRawptr, val_int) \
    /**                                    \
     * @brief Convert int to rawptr        \
     */                                    \
    FUNC_intToRawptr$(TDestRawptr, val_int)

#define rawptrIsNull(var_rawptr)       \
    /**                                \
     * @brief Check if pointer is null \
     */                                \
    FUNC_rawptrIsNull(var_rawptr)

#define rawptrIsNonnull(var_rawptr)        \
    /**                                    \
     * @brief Check if pointer is non-null \
     */                                    \
    FUNC_rawptrIsNonnull(var_rawptr)

/*========== Macros Implementation ==========================================*/

#define TYPE_anyptr_const        const void*
#define TYPE_anyptr              void*
#define TYPE_rawptr_const$(TSrc) const TSrc*
#define TYPE_rawptr$(TSrc)       TSrc*

#define FUNC_rawptrCast$(TDestRawptr, var_rawptr) ((TDestRawptr)(var_rawptr))
#define FUNC_rawptrToInt(val_rawptr)              ((usize)(val_rawptr))    // NOLINT
#define FUNC_intToRawptr$(TDestRawptr, val_int)   ((TDestRawptr)(val_int)) // NOLINT

#define FUNC_rawptrIsNull(var_rawptr)    ((var_rawptr) == null)
#define FUNC_rawptrIsNonnull(var_rawptr) ((var_rawptr) != null)

/*========== Validation Checks ==============================================*/

#if BUILTIN_PLTF_64BIT
/* 64-bit */
claim_assert_static_msg(sizeof(anyptr) == 8, "pointer must be 8 bytes on 64-bit");
#elif BUILTIN_PLTF_32BIT
/* 32-bit */
claim_assert_static_msg(sizeof(anyptr) == 4, "pointer must be 4 bytes on 32-bit");
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_PTR_INCLUDED */
