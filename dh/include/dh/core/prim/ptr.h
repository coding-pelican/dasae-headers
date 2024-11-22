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

/*========== Macros and Definitions =========================================*/

#define rawptr(_T)             \
    /**                        \
     * @brief Type raw pointer \
     */                        \
    IMPL_rawptr(_T)

#define rawaddr(_var)              \
    /**                            \
     * @brief Variable raw address \
     */                            \
    IMPL_rawaddr(_var)

#define anyptr                 \
    /**                        \
     * @brief Any type pointer \
     */                        \
    IMPL_anyptr

#define castPtr(_T, _raw)                          \
    /**                                            \
     * @brief Convert anyptr to pointer of type _T \
     */                                            \
    IMPL_castPtr(_T, _raw)

#define accessPtr(_raw)                                \
    /**                                                \
     * @brief Access the value pointed to by a pointer \
     */                                                \
    IMPL_accessPtr(_raw)

#define accessCastPtr(_T, _raw)                                   \
    /**                                                           \
     * @brief Access the value pointed to by a pointer of type _T \
     */                                                           \
    IMPL_accessCastPtr(_T, _raw)

/*========== Macros Implementation ==========================================*/

#define IMPL_rawptr(_T)              _T*
#define IMPL_rawaddr(_var...)        (&(_var))
#define IMPL_anyptr                  void*
#define IMPL_castPtr(_T, _raw)       (_T*)(_raw)
#define IMPL_accessPtr(_raw)         (*(_raw))
#define IMPL_accessCastPtr(_T, _raw) (*((_T*)(_raw)))

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
