/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    interp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-08 (date of creation)
 * @updated 2025-01-08 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/math
 * @prefix  math_interp
 *
 * @brief   Mathematical interpolation functions
 * @details Provides a set of interpolation functions
 */

/* Interpolation functions */
// /* Linear interpolation */
// #define math_lerp(val_x, val_min, val_max)                                              FUNC_math_lerp(val_x, val_min, val_max)
// #define math_lerpClamped(val_x, val_min, val_max)                                       FUNC_math_lerpClamped(val_x, val_min, val_max)
// #define math_invLerp(val_x, val_min, val_max)                                           FUNC_math_invLerp(val_x, val_min, val_max)
// #define math_invLerpClamped(val_x, val_min, val_max)                                    FUNC_math_invLerpClamped(val_x, val_min, val_max)
// #define math_remap(val_x, val_srcMin, val_srcMax, val_dstMin, val_dstMax)               FUNC_math_remap(val_x, val_srcMin, val_srcMax, val_dstMin, val_dstMax)
// #define math_remapClamped(val_x, val_srcMin, val_srcMax, val_dstMin, val_dstMax)        FUNC_math_remapClamped(val_x, val_srcMin, val_srcMax, val_dstMin, val_dstMax)
// /* Bezier interpolation */
// #define math_bezier(val_x, val_min, val_max)                                            FUNC_math_bezier(val_x, val_min, val_max)
// #define math_bezierClamped(val_x, val_min, val_max)                                     FUNC_math_bezierClamped(val_x, val_min, val_max)
// #define math_bezierQuadratic(val_x, val_min, val_control, val_max)                      FUNC_math_bezierQuadratic(val_x, val_min, val_control, val_max)
// #define math_bezierQuadraticClamped(val_x, val_min, val_control, val_max)               FUNC_math_bezierQuadraticClamped(val_x, val_min, val_control, val_max)
// #define math_bezierCubic(val_x, val_min, val_control1, val_control2, val_max)           FUNC_math_bezierCubic(val_x, val_min, val_control1, val_control2, val_max)
// #define math_bezierCubicClamped(val_x, val_min, val_control1, val_control2, val_max)    FUNC_math_bezierCubicClamped(val_x, val_min, val_control1, val_control2, val_max)
// /* Smoothstep interpolation */
// #define math_smoothstep(val_x, val_min, val_max)                                        FUNC_math_smoothstep(val_x, val_min, val_max)
// #define math_smoothstepClamped(val_x, val_min, val_max)                                 FUNC_math_smoothstepClamped(val_x, val_min, val_max)
// #define math_smootherstep(val_x, val_min, val_max)                                      FUNC_math_smootherstep(val_x, val_min, val_max)
// #define math_smootherstepClamped(val_x, val_min, val_max)                               FUNC_math_smootherstepClamped(val_x, val_min, val_max)
// /* Hermite interpolation */
// #define math_hermite(val_x, val_min, val_max, val_tension, val_bias)                    FUNC_math_hermite(val_x, val_min, val_max, val_tension, val_bias)
// #define math_hermiteClamped(val_x, val_min, val_max, val_tension, val_bias)             FUNC_math_hermiteClamped(val_x, val_min, val_max, val_tension, val_bias)
// /* Catmull-Rom interpolation */
// #define math_catmullRom(val_x, val_prev, val_start, val_end, val_next)                  FUNC_math_catmullRom(val_x, val_prev, val_start, val_end, val_next)
// #define math_catmullRomClamped(val_x, val_prev, val_start, val_end, val_next)           FUNC_math_catmullRomClamped(val_x, val_prev, val_start, val_end, val_next)
// /* Spring interpolation */
// #define math_spring(val_x, val_velocity, val_target, val_damping, val_stiffness)        FUNC_math_spring(val_x, val_velocity, val_target, val_damping, val_stiffness)
// #define math_springClamped(val_x, val_velocity, val_target, val_damping, val_stiffness) FUNC_math_springClamped(val_x, val_velocity, val_target, val_damping, val_stiffness)
// /* Elastic interpolation */
// #define math_elastic(val_x, val_min, val_max, val_bounciness, val_frequency)            FUNC_math_elastic(val_x, val_min, val_max, val_bounciness, val_frequency)
// #define math_elasticClamped(val_x, val_min, val_max, val_bounciness, val_frequency)     FUNC_math_elasticClamped(val_x, val_min, val_max, val_bounciness, val_frequency)
// /* Bounce interpolation */
// #define math_bounce(val_x, val_min, val_max, val_bounces)                               FUNC_math_bounce(val_x, val_min, val_max, val_bounces)
// #define math_bounceClamped(val_x, val_min, val_max, val_bounces)                        FUNC_math_bounceClamped(val_x, val_min, val_max, val_bounces)

/* Interpolation functions */
// #define FUNC_math_lerp(val_a, val_b, val_t) eval({\
//     let _a = (val_a);                             \
//     let _b = (val_b);                             \
//     let _t = (val_t);                             \
//     if (_t < 0 || _t > 1) {                       \
//         eval_return makeCleared(TypeOf(_a));      \
//     };                                            \
//     eval_return _a + (_b - _a) * _t;              \
//})
// #define FUNC_math_lerpSafe(val_a, val_b, val_t) eval({\
//     let _a = (val_a);                                      \
//     let _b = (val_b);                                      \
//     let _t = (val_t);                                      \
//     if (_t < 0 || _t > 1) {                                \
//         /* Interpolation factor out of range */            \
//         return_err(math_Err_err(math_ErrCode_OutOfRange)); \
//     };                                                     \
//     return_ok(_a + (_b - _a) * _t);                        \
//})
// #define FUNC_math_smoothstep(val_x, val_edge0, val_edge1) eval({\
//     let _x     = (val_x);                                         \
//     let _edge0 = (val_edge0);                                     \
//     let _edge1 = (val_edge1);                                     \
//     if (_edge0 >= _edge1) {                                       \
//         eval_return makeCleared(TypeOf(_x));                      \
//     };                                                            \
//     let _t = math_clamp((_x - _edge0) / (_edge1 - _edge0), 0, 1); \
//     eval_return _t * _t * (3 - 2 * _t);                           \
//})
// #define FUNC_math_smoothstepSafe(val_x, val_edge0, val_edge1) eval({\
//     let _x     = (val_x);                                           \
//     let _edge0 = (val_edge0);                                       \
//     let _edge1 = (val_edge1);                                       \
//     if (_edge0 >= _edge1) {                                         \
//         /* Edge values are equal */                                 \
//         return_err(math_Err_err(math_ErrCode_OutOfRange));          \
//     };                                                              \
//     let _t = math_clamp((_x - _edge0) / (_edge1 - _edge0), 0, 1);   \
//     eval_return _t * _t * (3 - 2 * _t);                             \
//})
