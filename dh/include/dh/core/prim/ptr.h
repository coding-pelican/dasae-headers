/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-11-02 (date of last update)
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

/*========== Macros and Definitions =========================================*/

#define rawptr(_T)         \
    /**                    \
     * @brief Type pointer \
     */                    \
    IMPL_rawptr(_T)

#define rawaddr(_var)          \
    /**                        \
     * @brief Variable address \
     */                        \
    IMPL_rawaddr(_var)

#define rawref(_var)             \
    /**                          \
     * @brief Variable reference \
     */                          \
    IMPL_rawref(_var)

#define rawderef(_var)             \
    /**                            \
     * @brief Variable dereference \
     */                            \
    IMPL_rawderef(_var)

#define anyptr                 \
    /**                        \
     * @brief Any type pointer \
     */                        \
    IMPL_anyptr

#define rawCast(_T, _raw)                          \
    /**                                            \
     * @brief Convert anyptr to pointer of type _T \
     */                                            \
    IMPL_rawCast(_T, _raw)

#define rawDeref(_raw)                                    \
    /**                                                   \
     * @brief Reference the value pointed to by a pointer \
     */                                                   \
    IMPL_rawDeref(_raw)

#define rawCastDeref(_T, _raw)                                       \
    /**                                                              \
     * @brief Reference the value pointed to by a pointer of type _T \
     */                                                              \
    IMPL_rawCastDeref(_T, _raw)

#define intToRaw(TDest, val)                           \
    /**                                                \
     * @brief Convert integer to pointer of type TDest \
     */                                                \
    IMPL_intToRaw(TDest, val)

#define rawToInt(val)                                \
    /**                                              \
     * @brief Convert pointer of to unsigned integer \
     */                                              \
    IMPL_rawToInt(val)

#define rawIsNull(_ptr)                \
    /**                                \
     * @brief Check if pointer is null \
     */                                \
    IMPL_ptrIsNull(_ptr)

#define rawIsNonnull(_ptr)                 \
    /**                                    \
     * @brief Check if pointer is not null \
     */                                    \
    IMPL_ptrIsNonnull(_ptr)

/*========== Macros Implementation ==========================================*/

#define IMPL_rawptr(_T)             _T*
#define IMPL_rawaddr(_var...)       (&(_var))
#define IMPL_rawref(_var...)        (&(_var))
#define IMPL_rawderef(_var...)      (*(_var))
#define IMPL_anyptr                 void*
#define IMPL_rawCast(_T, _raw)      ((_T)(_raw))
#define IMPL_rawDeref(_raw)         (*(_raw))
#define IMPL_rawCastDeref(_T, _raw) (*((_T*)(_raw)))
#define IMPL_intToRaw(_T, val)      ((_T)(val))    // NOLINT(performance-no-int-to-ptr)
#define IMPL_rawToInt(val)          ((usize)(val)) // NOLINT(performance-no-int-to-ptr)
#define IMPL_ptrIsNull(_ptr)        ((_ptr) == null)
#define IMPL_ptrIsNonnull(_ptr)     ((_ptr) != null)

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
