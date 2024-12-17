/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    TypeInfo.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2024-12-17 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header/Source/Implementation of some software
 * @details Some detailed explanation
 */

#ifndef TYPE_INFO_INCLUDED
#define TYPE_INFO_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core/prim.h"

#include <stdalign.h>

/*========== Macros and Definitions =========================================*/

/* Minimal type information */
typedef struct TypeInfo {
    usize size  : 52;
    usize align : 12;
} TypeInfo;
#define typeInfo(T) (                                    \
    (TypeInfo){ .size = sizeof(T), .align = alignof(T) } \
)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TYPE_INFO_INCLUDED */
