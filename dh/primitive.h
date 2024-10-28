/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    primitive.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2024-10-28 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  prim
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef PRIMITIVE_INCLUDED
#define PRIMITIVE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "compat/compat_primitive.h"


#define prim_as(_T, ...)             ((_T)__VA_ARGS__)
#define prim_divisible(_prim_n, ...) ((__VA_ARGS__ == 0) ? 0 : ((_prim_n) % (__VA_ARGS__) == 0))


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PRIMITIVE_INCLUDED */
