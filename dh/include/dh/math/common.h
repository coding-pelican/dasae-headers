/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-08 (date of creation)
 * @updated 2025-01-13 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/math
 * @prefix  math
 *
 * @brief   Mathematical common scalar operations with safety guarantees
 * @details Provides a set of common mathematical operations and functions
 */

#ifndef MATH_COMMON_INCLUDED
#define MATH_COMMON_INCLUDED (1)

#include "dh/core.h"
#include "dh/err_res.h"

// TODO: Provide specialized inline functions per type.
// `math_f64_pi` vs `math_pi_f64`

/* Common error types */
use_Err(
    math_Err,
    Overflow,
    Underflow,
    OutOfRange,
    InvalidInput,
    InvalidDomain,
    DivisionByZero
);

/* Common constants */
#define math_pi           VAL_math_pi
#define math_tau          VAL_math_tau
#define math_e            VAL_math_e
#define math_golden_ratio VAL_math_golden_ratio
#define math_sqrt_2       VAL_math_sqrt_2
#define math_sqrt_3       VAL_math_sqrt_3
#define math_eps          VAL_math_eps
#define math_nan          VAL_math_nan
#define math_inf          VAL_math_inf

#define math_limit_min$(TScalar) VAL_math_limit_min$(TScalar)
#define math_limit_max$(TScalar) VAL_math_limit_max$(TScalar)

#define math_f32_pi           VAL_math_f32_pi
#define math_f32_tau          VAL_math_f32_tau
#define math_f32_e            VAL_math_f32_e
#define math_f32_golden_ratio VAL_math_f32_golden_ratio
#define math_f32_sqrt_2       VAL_math_f32_sqrt_2
#define math_f32_sqrt_3       VAL_math_f32_sqrt_3
#define math_f32_eps          VAL_math_f32_eps
#define math_f32_nan          VAL_math_f32_nan
#define math_f32_inf          VAL_math_f32_inf

#define math_f64_pi           VAL_math_f64_pi
#define math_f64_tau          VAL_math_f64_tau
#define math_f64_e            VAL_math_f64_e
#define math_f64_golden_ratio VAL_math_f64_golden_ratio
#define math_f64_sqrt_2       VAL_math_f64_sqrt_2
#define math_f64_sqrt_3       VAL_math_f64_sqrt_3
#define math_f64_eps          VAL_math_f64_eps
#define math_f64_nan          VAL_math_f64_nan
#define math_f64_inf          VAL_math_f64_inf

/* Comparison operations */
#define math_cmp(val_lhs, val_rhs) OP_math_cmp(pp_uniqueToken(cmp_lhs), pp_uniqueToken(cmp_rhs), val_lhs, val_rhs) /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_eq(val_lhs, val_rhs)  OP_math_eq(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_ne(val_lhs, val_rhs)  OP_math_ne(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_lt(val_lhs, val_rhs)  OP_math_lt(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_gt(val_lhs, val_rhs)  OP_math_gt(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_le(val_lhs, val_rhs)  OP_math_le(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_ge(val_lhs, val_rhs)  OP_math_ge(val_lhs, val_rhs)                                                    /* NOLINT(bugprone-assignment-in-if-condition) */

/* Arithmetic operations */
#define math_neg(val_x)                OP_math_neg(val_x)
#define math_add(val_lhs, val_rhs)     OP_math_add(val_lhs, val_rhs)
#define math_sub(val_lhs, val_rhs)     OP_math_sub(val_lhs, val_rhs)
#define math_mul(val_lhs, val_rhs)     OP_math_mul(val_lhs, val_rhs)
#define math_div(val_lhs, val_rhs)     OP_math_div(pp_uniqueToken(div_lhs), pp_uniqueToken(div_rhs), val_lhs, val_rhs)
#define math_divSafe(val_lhs, val_rhs) OP_math_divSafe(pp_uniqueToken(divSafe_lhs), pp_uniqueToken(divSafe_rhs), val_lhs, val_rhs)
#define math_mod(val_lhs, val_rhs)     OP_math_mod(pp_uniqueToken(mod_lhs), pp_uniqueToken(mod_rhs), val_lhs, val_rhs)
#define math_modSafe(val_lhs, val_rhs) OP_math_modSafe(pp_uniqueToken(modSafe_lhs), pp_uniqueToken(modSafe_rhs), val_lhs, val_rhs)

/* Basic functions */
#define math_abs(val_x)                         FUNC_math_abs(pp_uniqueToken(abs_x), val_x)
#define math_sign(val_x)                        FUNC_math_sign(pp_uniqueToken(sign_x), val_x)
#define math_min(val_lhs, val_rhs)              FUNC_math_min(val_lhs, val_rhs)
#define math_max(val_lhs, val_rhs)              FUNC_math_max(val_lhs, val_rhs)
#define math_clamp(val_x, val_min, val_max)     FUNC_math_clamp(val_x, val_min, val_max)
// #define math_clamp01(val_x)                     FUNC_math_clamp01(val_x)
#define math_clampSafe(val_x, val_min, val_max) FUNC_math_clampSafe(val_x, val_min, val_max) /* TODO: make safety */
#define math_wrap(val_x, val_min, val_max)      FUNC_math_wrap(val_x, val_min, val_max)
// #define math_wrap01(val_x)                      FUNC_math_wrap01(val_x)
#define math_wrapSafe(val_x, val_min, val_max)  FUNC_math_wrapSafe(val_x, val_min, val_max) /* TODO: make safety */

/* Rounding functions */
#define math_floor(val_x) FUNC_math_floor(val_x)
#define math_ceil(val_x)  FUNC_math_ceil(val_x)
#define math_round(val_x) FUNC_math_round(val_x)
// TODO: add more rounding functions
#define math_trunc(val_x) FUNC_math_trunc(val_x)

/* Exponential/logarithmic functions */
#define math_sqrt(val_x)                FUNC_math_sqrt(val_x)
#define math_sqrtSafe(val_x)            FUNC_math_sqrtSafe(val_x)
#define math_pow(val_base, val_exp)     FUNC_math_pow(val_base, val_exp)
#define math_powSafe(val_base, val_exp) FUNC_math_powSafe(val_base, val_exp)
#define math_rsqrt(val_x)               FUNC_math_rsqrt(val_x)
#define math_rsqrtSafe(val_x)           FUNC_math_rsqrtSafe(val_x)
// #define math_exp(val_x)                 FUNC_math_exp(val_x)
// #define math_exp2(val_x)                FUNC_math_exp2(val_x)
// #define math_exp10(val_x)               FUNC_math_exp10(val_x)
// #define math_log(val_base, val_antilog) FUNC_math_log(val_base, val_antilog)
// #define math_log2(val_antilog)          FUNC_math_log2(val_antilog)
// #define math_log10(val_antilog)         FUNC_math_log10(val_antilog)
// #define math_cube(val_x)                FUNC_math_cube(val_x)
// #define math_cbrt(val_x)                FUNC_math_cbrt(val_x)
// #define math_rcbrt(val_x)               FUNC_math_rcbrt(val_x)
// #define math_quad(val_x)                FUNC_math_quad(val_x)
// #define math_qdrt(val_x)                FUNC_math_qdrt(val_x)
// #define math_rqdrt(val_x)               FUNC_math_rqdrt(val_x)
// #define math_hypot(val_x, val_y)        FUNC_math_hypot(val_x, val_y)

/* Trigonometric functions */
#define math_sin(val_x)              FUNC_math_sin(val_x)
#define math_cos(val_x)              FUNC_math_cos(val_x)
#define math_tan(val_x)              FUNC_math_tan(val_x)
#define math_asin(val_x)             FUNC_math_asin(val_x)
#define math_asinSafe(val_x)         FUNC_math_asinSafe(val_x)
#define math_acos(val_x)             FUNC_math_acos(val_x)
#define math_acosSafe(val_x)         FUNC_math_acosSafe(val_x)
#define math_atan(val_x)             FUNC_math_atan(val_x)
#define math_atan2(val_y, val_x)     FUNC_math_atan2(val_y, val_x)
#define math_atan2Safe(val_y, val_x) FUNC_math_atan2Safe(val_y, val_x)

/* Hyperbolic functions */
// #define math_sinh(val_x)      FUNC_math_sinh(val_x)
// #define math_cosh(val_x)      FUNC_math_cosh(val_x)
// #define math_tanh(val_x)      FUNC_math_tanh(val_x)
// #define math_asinh(val_x)     FUNC_math_asinh(val_x)
// #define math_asinhSafe(val_x) FUNC_math_asinhSafe(val_x)
// #define math_acosh(val_x)     FUNC_math_acosh(val_x)
// #define math_acoshSafe(val_x) FUNC_math_acoshSafe(val_x)
// #define math_atanh(val_x)     FUNC_math_atanh(val_x)
// #define math_atanhSafe(val_x) FUNC_math_atanhSafe(val_x)

/*========== Implementation =================================================*/

/* Common constants */
#define VAL_math_pi           math_f64_pi
#define VAL_math_tau          math_f64_tau
#define VAL_math_e            math_f64_e
#define VAL_math_golden_ratio math_f64_golden_ratio
#define VAL_math_sqrt_2       math_f64_sqrt_2
#define VAL_math_sqrt_3       math_f64_sqrt_3
#define VAL_math_eps          math_f64_eps
#define VAL_math_nan          math_f64_nan
#define VAL_math_inf          math_f64_inf

#define VAL_math_limit_min$(TScalar) _Generic( \
    (TScalar)0,                                \
    u8: u8_limit_min,                          \
    u16: u16_limit_min,                        \
    u32: u32_limit_min,                        \
    u64: u64_limit_min,                        \
    i8: i8_limit_min,                          \
    i16: i16_limit_min,                        \
    i32: i32_limit_min,                        \
    i64: i64_limit_min,                        \
    f32: f32_limit_min,                        \
    f64: f64_limit_min,                        \
    default: 0                                 \
)
#define VAL_math_limit_max$(TScalar) _Generic( \
    (TScalar)0,                                \
    u8: u8_limit_max,                          \
    u16: u16_limit_max,                        \
    u32: u32_limit_max,                        \
    u64: u64_limit_max,                        \
    i8: i8_limit_max,                          \
    i16: i16_limit_max,                        \
    i32: i32_limit_max,                        \
    i64: i64_limit_max,                        \
    f32: f32_limit_max,                        \
    f64: f64_limit_max,                        \
    default: 0                                 \
)

#define VAL_math_f32_pi           as$(f32, math_pi)
#define VAL_math_f32_tau          as$(f32, math_tau)
#define VAL_math_f32_e            as$(f32, math_e)
#define VAL_math_f32_golden_ratio as$(f32, math_golden_ratio)
#define VAL_math_f32_sqrt_2       as$(f32, math_sqrt_2)
#define VAL_math_f32_sqrt_3       as$(f32, math_sqrt_3)
#define VAL_math_f32_eps          f32_eps
#define VAL_math_f32_nan          f32_nan
#define VAL_math_f32_inf          f32_inf

#define VAL_math_f64_pi           (3.14159265358979323846)
#define VAL_math_f64_tau          (2.0 * math_f64_pi)
#define VAL_math_f64_e            (2.71828182845904523536)
#define VAL_math_f64_golden_ratio (1.61803398874989484820)
#define VAL_math_f64_sqrt_2       (1.41421356237309504880)
#define VAL_math_f64_sqrt_3       (1.73205080756887729352)
#define VAL_math_f64_nan          f64_nan
#define VAL_math_f64_eps          f64_eps
#define VAL_math_f64_inf          f64_inf

/* Comparison operations */
#define OP_math_cmp(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let _lhs = (val_lhs);                               \
    let _rhs = (val_rhs);                               \
    var _ret = cmp_Ord_equal;                           \
    if (isFlt(TypeOf(_lhs))) {                          \
        if (math_abs((_lhs) - (_rhs)) <= math_eps) {    \
            _ret = cmp_Ord_equal;                       \
        } else if ((_lhs) < (_rhs)) {                   \
            _ret = cmp_Ord_less;                        \
        } else {                                        \
            _ret = cmp_Ord_greater;                     \
        }                                               \
    } else {                                            \
        if ((_lhs) < (_rhs)) {                          \
            _ret = cmp_Ord_less;                        \
        } else if ((_lhs) > (_rhs)) {                   \
            _ret = cmp_Ord_greater;                     \
        } else {                                        \
            _ret = cmp_Ord_equal;                       \
        }                                               \
    };                                                  \
    eval_return _ret;                                   \
)
#define OP_math_eq(val_lhs, val_rhs) (math_cmp(val_lhs, val_rhs) == cmp_Ord_equal)
#define OP_math_ne(val_lhs, val_rhs) (math_cmp(val_lhs, val_rhs) != cmp_Ord_equal)
#define OP_math_lt(val_lhs, val_rhs) (math_cmp(val_lhs, val_rhs) == cmp_Ord_less)
#define OP_math_gt(val_lhs, val_rhs) (math_cmp(val_lhs, val_rhs) == cmp_Ord_greater)
#define OP_math_le(val_lhs, val_rhs) (!math_gt(val_lhs, val_rhs))
#define OP_math_ge(val_lhs, val_rhs) (!math_lt(val_lhs, val_rhs))

/* Arithmetic operations */
#define OP_math_neg(val_x)                        (-(_val_x))
#define OP_math_add(val_lhs, val_rhs)             ((val_lhs) + (val_rhs))
#define OP_math_sub(val_lhs, val_rhs)             ((val_lhs) - (val_rhs))
#define OP_math_mul(val_lhs, val_rhs)             ((val_lhs) * (val_rhs))
#define OP_math_div(_lhs, _rhs, val_lhs, val_rhs) eval( \
    let _lhs = (val_lhs);                               \
    let _rhs = (val_rhs);                               \
    var _ret = makeCleared(TypeOf(_lhs));               \
    if ((_rhs) != 0) {                                  \
        _ret = (_lhs) / (_rhs);                         \
    };                                                  \
    eval_return _ret;                                   \
)
#define OP_math_divSafe(_lhs, _rhs, val_lhs, val_rhs) eval(                                \
    let   _lhs = (val_lhs);                                                                \
    let   _rhs = (val_rhs);                                                                \
    void* _ret = null;                                                                     \
    if ((_rhs) == 0) {                                                                     \
        _ret = (void*)&(Err$(TypeOf(_lhs)))err(math_Err_err(math_ErrType_DivisionByZero)); \
    } else {                                                                               \
        _ret = (void*)&(Err$(TypeOf(_lhs)))ok((_lhs) / (_rhs));                            \
    };                                                                                     \
    eval_return(*(Err$(TypeOf(_lhs))*)_ret);                                               \
)
#define OP_math_mod(_lhs, _rhs, val_lhs, val_rhs) eval(                     \
    let _lhs = (val_lhs);                                                   \
    let _rhs = (val_rhs);                                                   \
    var _ret = makeCleared(TypeOf(_lhs));                                   \
    if ((_rhs) != 0) {                                                      \
        if (isFlt(TypeOf(_ret))) {                                          \
            _ret = as$(TypeOf(_ret), fmod(as$(f64, _lhs), as$(f64, _rhs))); \
        } else {                                                            \
            _ret = as$(TypeOf(_ret), as$(i64, _lhs) % as$(i64, _rhs));      \
        }                                                                   \
    };                                                                      \
    eval_return _ret;                                                       \
)
#define OP_math_modSafe(_lhs, _rhs, val_lhs, val_rhs) eval(                                \
    let   _lhs = (val_lhs);                                                                \
    let   _rhs = (val_rhs);                                                                \
    void* _ret = null;                                                                     \
    if ((_rhs) == 0) {                                                                     \
        _ret = (void*)&(Err$(TypeOf(_lhs)))err(math_Err_err(math_ErrType_DivisionByZero)); \
    } else {                                                                               \
        if (isFlt(TypeOf(_ret))) {                                                         \
            _ret = (void*)&(Err$(TypeOf(_lhs)))ok(fmod(as$(f64, _lhs), as$(f64, _rhs)));   \
        } else {                                                                           \
            _ret = (void*)&(Err$(TypeOf(_lhs)))ok(as$(i64, _lhs) % as$(i64, _rhs));        \
        }                                                                                  \
    };                                                                                     \
    eval_return(*(Err$(TypeOf(_lhs))*)_ret);                                               \
)

/* Basic functions */
#define FUNC_math_abs(_x, val_x) eval(              \
    var _x = (val_x);                               \
    if (isFlt(TypeOf(_x))) {                        \
        (_x) = as$(TypeOf(_x), fabs(as$(f64, _x))); \
    } else {                                        \
        (_x) = (_x) < 0 ? -(_x) : (_x);             \
    };                                              \
    eval_return _x;                                 \
)
#define FUNC_math_sign(_x, val_x) eval(           \
    let _x = (val_x);                             \
    eval_return _x < 0 ? -1 : ((_x) > 0 ? 1 : 0); \
)
#define FUNC_math_min(val_lhs, val_rhs) eval( \
    let _lhs = (val_lhs);                     \
    let _rhs = (val_rhs);                     \
    eval_return _lhs < _rhs ? _lhs : _rhs;    \
)
#define FUNC_math_max(val_lhs, val_rhs) eval( \
    let _lhs = (val_lhs);                     \
    let _rhs = (val_rhs);                     \
    eval_return _lhs > _rhs ? _lhs : _rhs;    \
)
#define FUNC_math_clamp(val_x, val_min, val_max) eval( \
    let         _x   = (val_x);                        \
    let         _min = (val_min);                      \
    let         _max = (val_max);                      \
    eval_return math_min(math_max(_min, _x), _max);    \
)
#define FUNC_math_clampSafe(val_x, val_min, val_max) eval(         \
    let _x   = val_x;                                              \
    let _min = val_min;                                            \
    let _max = val_max;                                            \
    eval_return((Err$(TypeOf(_x)))ok(math_clamp(_x, _min, _max))); \
)
#define FUNC_math_wrap(val_x, val_min, val_max) eval(    \
    let         _x   = (val_x);                          \
    let         _min = (val_min);                        \
    let         _max = (val_max);                        \
    eval_return _min + math_mod(_x - _min, _max - _min); \
)
#define FUNC_math_wrapSafe(val_x, val_min, val_max) eval(         \
    let _x   = val_x;                                             \
    let _min = val_min;                                           \
    let _max = val_max;                                           \
    eval_return((Err$(TypeOf(_x)))ok(math_wrap(_x, _min, _max))); \
)

/* Rounding functions */
#define FUNC_math_floor(val_x) eval(                  \
    let         _x = (val_x);                         \
    eval_return as$(TypeOf(_x), floor(as$(f64, _x))); \
)
#define FUNC_math_ceil(val_x) eval(                  \
    let         _x = (val_x);                        \
    eval_return as$(TypeOf(_x), ceil(as$(f64, _x))); \
)
#define FUNC_math_round(val_x) eval(                  \
    let         _x = (val_x);                         \
    eval_return as$(TypeOf(_x), round(as$(f64, _x))); \
)
#define FUNC_math_trunc(val_x) eval(                  \
    let         _x = (val_x);                         \
    eval_return as$(TypeOf(_x), trunc(as$(f64, _x))); \
)

/* Exponential/logarithmic functions */
#define FUNC_math_sqrt(val_x) eval(                   \
    let _x   = (val_x);                               \
    var _ret = makeCleared(TypeOf(_x));               \
    if (0 <= _x) {                                    \
        _ret = as$(TypeOf(_ret), sqrt(as$(f64, _x))); \
    };                                                \
    eval_return _ret;                                 \
)
#define FUNC_math_sqrtSafe(val_x) eval(                                                 \
    let   _x   = (val_x);                                                               \
    void* _ret = null;                                                                  \
    if (_x < 0) {                                                                       \
        /* Square root of negative number */                                            \
        _ret = (void*)&(Err$(TypeOf(_x)))err(math_Err_err(math_ErrType_InvalidDomain)); \
    } else {                                                                            \
        _ret = (void*)&(Err$(TypeOf(_x)))ok(as$(TypeOf(_ret), sqrt(as$(f64, _x))));     \
    };                                                                                  \
    eval_return(*(Err$(TypeOf(_x))*)_ret);                                              \
)
#define FUNC_math_pow(val_base, val_exp) eval(                          \
    let _base = (val_base);                                             \
    let _exp  = (val_exp);                                              \
    var _ret  = makeCleared(TypeOf(_base));                             \
    if (_base != 0 || 0 < _exp) {                                       \
        _ret = as$(TypeOf(_ret), pow(as$(f64, _base), as$(f64, _exp))); \
    };                                                                  \
    eval_return _ret;                                                   \
)
#define FUNC_math_powSafe(val_base, val_exp) eval(                                                       \
    let   _base = (val_base);                                                                            \
    let   _exp  = (val_exp);                                                                             \
    void* _ret  = null;                                                                                  \
    if (_base == 0 && _exp <= 0) {                                                                       \
        /* Invalid base/exponent combination */                                                          \
        _ret = (void*)&(Err$(TypeOf(_base)))err(math_Err_err(math_ErrType_InvalidDomain));               \
    } else {                                                                                             \
        _ret = (void*)&(Err$(TypeOf(_base)))ok(as$(TypeOf(_ret), pow(as$(f64, _base), as$(f64, _exp)))); \
    };                                                                                                   \
    eval_return(*(Err$(TypeOf(_x))*)_ret);                                                               \
)
#define FUNC_math_rsqrt(val_x) eval(                        \
    let _x   = (val_x);                                     \
    var _ret = makeCleared(TypeOf(_x));                     \
    if (0 < _x) {                                           \
        _ret = as$(TypeOf(_ret), 1.0 / sqrt(as$(f64, _x))); \
    };                                                      \
    eval_return _ret;                                       \
)
#define FUNC_math_rsqrtSafe(val_x) eval(                                                  \
    let   _x   = (val_x);                                                                 \
    void* _ret = null;                                                                    \
    if (_x <= 0) {                                                                        \
        /* Invalid input for inverse square root */                                       \
        _ret = (void*)&(Err$(TypeOf(_x)))err(math_Err_err(math_ErrType_InvalidDomain));   \
    } else {                                                                              \
        _ret = (void*)&(Err$(TypeOf(_x)))ok(as$(TypeOf(_ret), 1.0 / sqrt(as$(f64, _x)))); \
    };                                                                                    \
    eval_return(*(Err$(TypeOf(_x))*)_ret);                                                \
)

/* Trigonometric functions */
#define FUNC_math_sin(val_x) eval(                  \
    let         _x = (val_x);                       \
    eval_return as$(TypeOf(_x), sin(as$(f64, _x))); \
)
#define FUNC_math_cos(val_x) eval(                  \
    let         _x = (val_x);                       \
    eval_return as$(TypeOf(_x), cos(as$(f64, _x))); \
)
#define FUNC_math_tan(val_x) eval(                  \
    let         _x = (val_x);                       \
    eval_return as$(TypeOf(_x), tan(as$(f64, _x))); \
)
#define FUNC_math_asin(val_x) eval(     \
    let _x   = (val_x);                 \
    var _ret = makeCleared(TypeOf(_x)); \
    if (-1 <= _x && _x <= 1) {          \
        _ret = asin(_x);                \
    };                                  \
    eval_return _ret;                   \
)
#define FUNC_math_asinSafe(val_x) eval(                                                 \
    let   _x   = (val_x);                                                               \
    void* _ret = null;                                                                  \
    if (_x < -1 || 1 < _x) {                                                            \
        /* Inverse sine domain error */                                                 \
        _ret = (void*)&(Err$(TypeOf(_x)))err(math_Err_err(math_ErrType_InvalidDomain)); \
    } else {                                                                            \
        _ret = (void*)&(Err$(TypeOf(_x)))ok(asin(_x));                                  \
    };                                                                                  \
    eval_return(*(Err$(TypeOf(_x))*)_ret);                                              \
)
#define FUNC_math_acos(val_x) eval(     \
    let _x   = (val_x);                 \
    var _ret = makeCleared(TypeOf(_x)); \
    if (-1 <= _x && _x <= 1) {          \
        _ret = acos(_x);                \
    };                                  \
    eval_return _ret;                   \
)
#define FUNC_math_acosSafe(val_x) eval(                                                 \
    let   _x   = (val_x);                                                               \
    void* _ret = null;                                                                  \
    if (_x < -1 || 1 < _x) {                                                            \
        /* Inverse cosine domain error */                                               \
        _ret = (void*)&(Err$(TypeOf(_x)))err(math_Err_err(math_ErrType_InvalidDomain)); \
    } else {                                                                            \
        _ret = (void*)&(Err$(TypeOf(_x)))ok(acos(_x));                                  \
    };                                                                                  \
    eval_return(*(Err$(TypeOf(_x))*)_ret);                                              \
)
#define FUNC_math_atan(val_x) eval( \
    let         _x = (val_x);       \
    eval_return atan(_x);           \
)
#define FUNC_math_atan2(val_y, val_x) eval( \
    let _y   = (val_y);                     \
    let _x   = (val_x);                     \
    var _ret = makeCleared(TypeOf(_y));     \
    if (_y != 0 || _x != 0) {               \
        _ret = atan2(_y, _x);               \
    };                                      \
    eval_return _ret;                       \
)
#define FUNC_math_atan2Safe(val_y, val_x) eval(                                         \
    let   _y   = (val_y);                                                               \
    let   _x   = (val_x);                                                               \
    void* _ret = null;                                                                  \
    if (_x == 0 && _y == 0) {                                                           \
        /* atan2(0, 0) is undefined */                                                  \
        _ret = (void*)&(Err$(TypeOf(_y)))err(math_Err_err(math_ErrType_InvalidDomain)); \
    } else {                                                                            \
        _ret = (void*)&(Err$(TypeOf(_y)))ok(atan2(_y, _x));                             \
    };                                                                                  \
    eval_return(*(Err$(TypeOf(_y))*)_ret);                                              \
)

#endif /* MATH_COMMON_INCLUDED */
