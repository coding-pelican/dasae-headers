/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
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

#include "compat/compat_primitive.h"
#include "preprocessor.h"


#define cmp_eq(_T, _x, _y) (_T##_eq(_x, _y))
#define cmp_ne(_T, _x, _y) (_T##_ne(_x, _y))
#define cmp_lt(_T, _x, _y) (_T##_lt(_x, _y))
#define cmp_le(_T, _x, _y) (_T##_le(_x, _y))
#define cmp_gt(_T, _x, _y) (_T##_gt(_x, _y))
#define cmp_ge(_T, _x, _y) (_T##_ge(_x, _y))

#define pp_neg(_T, _x) (_T##_neg(_x))
#define pp_abs(_T, _x) (_T##_abs(_x))

#define pp_add(_T, _x, _y) (_T##_add(_x, _y))
#define pp_sub(_T, _x, _y) (_T##_sub(_x, _y))
#define pp_mul(_T, _x, _y) (_T##_mul(_x, _y))
#define pp_div(_T, _x, _y) (_T##_div(_x, _y))
#define pp_mod(_T, _x, _y) (_T##_mod(_x, _y))

#define pp_min(_T, _x, _y)            (_T##_min(_x, _y))
#define pp_max(_T, _x, _y)            (_T##_max(_x, _y))
/* _x in [low, high] */
#define pp_clamp(_T, _x, _low, _high) (_T##_clamp(_x, _low, _high))
/* _x in [low, high] */
#define pp_wrap(_T, _x, _low, _high)  (_T##_wrap(_x, _low, _high))

#define pp_swap(_T, _a, _b) pp_func( \
    Ptr(_T) __a = &(_a);             \
    Ptr(_T) __b = &(_b);             \
    _T __t      = *__a;              \
    *__a        = *__b;              \
    *__b        = __t;               \
)


#define pp_abs_(_T, _x) (_T##_ge((_x), (0)) ? (_x) : (_T##_neg(_x)))

#define pp_min_(_T, _x, _y)            (_T##_lt((_x), (_y)) ? (_x) : (_y))
#define pp_max_(_T, _x, _y)            (_T##_gt((_x), (_y)) ? (_x) : (_y))
/* _x in [low, high] */
#define pp_clamp_(_T, _x, _low, _high) pp_max_(_T, _low, pp_min_(_T, _x, _high))
// clang-format off
/**
 * pp_wrap - Generic wrap macro using Type_func operations
 * Assumes the following operations are defined for the type _T:
 * _T##_eq(_x, _y), _T##_ne(_x, _y), _T##_lt(_x, _y), _T##_le(_x, _y), _T##_gt(_x, _y), _T##_ge(_x, _y)
 * _T##_neg(_x), _T##_abs(_x), _T##_add(_x, _y), _T##_sub(_x, _y), _T##_mul(_x, _y), _T##_div(_x, _y), _T##_mod(_x, _y)
 */
/* _x in [low, high] */
#  define pp_wrap_(_T, _x, _low, _high) ( \
    _T##_le(_high, _low) ? (_low) : \
    _T##_add(_low, \
        _T##_mod( \
            _T##_add( \
                _T##_mod(_T##_sub(_x, _low), _T##_add(_T##_sub(_high, _low), 1)), \
                _T##_add(_T##_sub(_high, _low), 1) \
            ), \
            _T##_add(_T##_sub(_high, _low), 1) \
        ) \
    ) \
)
// clang-format on


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* COMMON_INCLUDED */
