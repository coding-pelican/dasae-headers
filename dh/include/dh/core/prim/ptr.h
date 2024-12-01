/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-11-29 (date of last update)
 * @version v1.0.0
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

#define anyptr                            \
    /**                                   \
     * @brief Pointer to any type (void*) \
     */                                   \
    IMPL_anyptr

#define rawptr(TSrc)                  \
    /**                               \
     * @brief Pointer to type (TSrc*) \
     */                               \
    IMPL_rawptr(TSrc)

typedef ptrdiff_t ptrdiff;

#define ref(var)                 \
    /**                          \
     * @brief Reference variable \
     */                          \
    IMPL_ref(var)

#define deref(TSelf, var)          \
    /**                            \
     * @brief Dereference variable \
     */                            \
    IMPL_deref(TSelf, var)

#define rawptrCast(TDestPtr, var)                        \
    /**                                                  \
     * @brief Convert anyptr to pointer of type TDestPtr \
     */                                                  \
    IMPL_rawptrCast(TDestPtr, var)

#define rawptrToInt(raw)                  \
    /**                                   \
     * @brief Convert rawptr to int(uptr) \
     */                                   \
    IMPL_rawptrToInt(raw)

#define intToRawptr(TDestRaw, val)  \
    /**                             \
     * @brief Convert int to rawptr \
     */                             \
    IMPL_intToRawptr(TDestRaw, val)

#define rawptrIsNull(var)              \
    /**                                \
     * @brief Check if pointer is null \
     */                                \
    IMPL_rawptrIsNull(var)

#define rawptrIsNonnull(_ptr)              \
    /**                                    \
     * @brief Check if pointer is non-null \
     */                                    \
    IMPL_rawptrIsNonnull(_ptr)

/*========== Macros Implementation ==========================================*/

#define IMPL_anyptr void*

#define IMPL_rawptr(TSrc) TSrc*

#define IMPL_ref(var)          (&(var))
#define IMPL_deref(TSelf, var) (*(TSelf*)(var))

#define IMPL_rawptrCast(TDestPtr, var)  ((TDestPtr)(var))
#define IMPL_rawptrToInt(raw)           ((uptr)(raw))     // NOLINT
#define IMPL_intToRawptr(TDestRaw, val) ((TDestRaw)(val)) // NOLINT

#define IMPL_rawptrIsNull(var)    ((var) == null)
#define IMPL_rawptrIsNonnull(var) ((var) != null)

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
