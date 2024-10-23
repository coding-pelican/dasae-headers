/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-21 (date of last update)
 * @version v1.0.0
 * @ingroup SOME_GROUP
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef COMMON_INCLUDED
#define COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "primitive_types.h"


#define Swap(_Type, _a, _b)     \
    do {                        \
        Ptr(_Type) __a = &(_a); \
        Ptr(_Type) __b = &(_b); \
        _Type __t      = *__a;  \
        *__a           = *__b;  \
        *__b           = __t;   \
    } while (false)

#define UnusedValue(_param) ((void)(_param))
#define IgnoreReturn(_func) (void)_func


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* COMMON_INCLUDED */
