/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1
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

#define rawptr_const(TSrc)                  \
    /**                                     \
     * @brief Pointer to type (const TSrc*) \
     */                                     \
    TYPE_rawptr_const(TSrc)

#define rawptr(TSrc)                  \
    /**                               \
     * @brief Pointer to type (TSrc*) \
     */                               \
    TYPE_rawptr(TSrc)

#define rawptrCast(TDestPtr, var)                        \
    /**                                                  \
     * @brief Convert anyptr to pointer of type TDestPtr \
     */                                                  \
    FUNC_rawptrCast(TDestPtr, var)

#define rawptrToInt(raw)                   \
    /**                                    \
     * @brief Convert rawptr to int(usize) \
     */                                    \
    FUNC_rawptrToInt(raw)

#define intToRawptr(TDestRaw, val)  \
    /**                             \
     * @brief Convert int to rawptr \
     */                             \
    FUNC_intToRawptr(TDestRaw, val)

#define rawptrIsNull(var)              \
    /**                                \
     * @brief Check if pointer is null \
     */                                \
    FUNC_rawptrIsNull(var)

#define rawptrIsNonnull(ptr)               \
    /**                                    \
     * @brief Check if pointer is non-null \
     */                                    \
    FUNC_rawptrIsNonnull(ptr)

/*========== Macros Implementation ==========================================*/

#define TYPE_anyptr_const       const void*
#define TYPE_anyptr             void*
#define TYPE_rawptr_const(TSrc) const TSrc*
#define TYPE_rawptr(TSrc)       TSrc*

#define FUNC_rawptrCast(TDestPtr, var)  ((TDestPtr)(var))
#define FUNC_rawptrToInt(raw)           ((usize)(raw))    // NOLINT
#define FUNC_intToRawptr(TDestRaw, val) ((TDestRaw)(val)) // NOLINT

#define FUNC_rawptrIsNull(var)    ((var) == null)
#define FUNC_rawptrIsNonnull(var) ((var) != null)

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
