/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    type_info.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-23 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef BUILTIN_TYPE_INFO_INCLUDED
#define BUILTIN_TYPE_INFO_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <stdalign.h>

/*========== Macros and Definitions =========================================*/

#define sizeOf(T_or_val) \
    IMPL_sizeOf(T_or_val)

#define alignAs(val_align) \
    IMPL_alignAs(val_align)

#define alignOf(T) \
    IMPL_alignOf(T)

#define countOf(var_arr) \
    IMPL_countOf(var_arr)

#define TypeOf(val) \
    IMPL_TypeOf(val)

/*========== Macros Implementation ==========================================*/

#define IMPL_sizeOf(T_or_val) \
    sizeof(T_or_val)

#define IMPL_alignAs(val_align) \
    alignas(val_align)

#define IMPL_alignOf(T) \
    alignof(T)

// NOLINTBEGIN
#define IMPL_countOf(var_arr) \
    (sizeof(var_arr) / sizeof(var_arr[0]))
// NOLINTEND

#define IMPL_TypeOf(val) \
    __typeof__(val)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_TYPE_INFO_INCLUDED */
