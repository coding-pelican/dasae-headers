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

#define UnusedValue(_param) ((void)(_param))
#define IgnoreReturn(_func) (void)_func

#define Swap(_Type, _a, _b)     \
    do {                        \
        Ptr(_Type) __a = &(_a); \
        Ptr(_Type) __b = &(_b); \
        _Type __t      = *__a;  \
        *__a           = *__b;  \
        *__b           = __t;   \
    } while (false)

#define Min(_Type, _x, _y) (_Type##_lt((_x), (_y)) ? (_x) : (_y))
#define Max(_Type, _x, _y) (_Type##_gt((_x), (_y)) ? (_x) : (_y))

/* x in [low, high] */
#define Clamp(_Type, _x, _low, _high) Max(_Type, _low, Min(_Type, _x, _high))
/* x in [low, high] */
#define Wrap(_Type, _x, _low, _high) (                                                                                     \
    _Type##_ge(_low, _high)                                                                                                \
        ? (_low)                                                                                                           \
    : _Type##_lt(_x, _low)                                                                                                 \
        ? _Type##_sub(_Type##_sub(_high, _Type##_mod(_Type##_sub(_Type##_sub(_low, _x), 1), _Type##_sub(_high, _low))), 1) \
        : _Type##_add(_low, _Type##_mod(_Type##_sub(_x, _low), _Type##_sub(_high, _low)))                                  \
)


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* COMMON_INCLUDED */
