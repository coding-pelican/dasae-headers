/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
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

#include "dh/prl.h"

// TODO: Provide specialized inline functions per type.
// `math_f64_pi` vs `math_pi_f64`

/* Common error types */
errset_((math_Err)(
    Overflow,
    Underflow,
    OutOfRange,
    InvalidInput,
    InvalidDomain,
    DivisionByZero
));

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
#define math_cmp(val_lhs, val_rhs) OP_math_cmp(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), val_lhs, val_rhs) /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_eq(val_lhs, val_rhs)  OP_math_eq(val_lhs, val_rhs)                                                     /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_ne(val_lhs, val_rhs)  OP_math_ne(val_lhs, val_rhs)                                                     /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_lt(val_lhs, val_rhs)  OP_math_lt(val_lhs, val_rhs)                                                     /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_gt(val_lhs, val_rhs)  OP_math_gt(val_lhs, val_rhs)                                                     /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_le(val_lhs, val_rhs)  OP_math_le(val_lhs, val_rhs)                                                     /* NOLINT(bugprone-assignment-in-if-condition) */
#define math_ge(val_lhs, val_rhs)  OP_math_ge(val_lhs, val_rhs)                                                     /* NOLINT(bugprone-assignment-in-if-condition) */

/* Arithmetic operations */
#define math_neg(val_x)                OP_math_neg(val_x)
#define math_add(val_lhs, val_rhs)     OP_math_add(val_lhs, val_rhs)
#define math_sub(val_lhs, val_rhs)     OP_math_sub(val_lhs, val_rhs)
#define math_mul(val_lhs, val_rhs)     OP_math_mul(val_lhs, val_rhs)
#define math_div(val_lhs, val_rhs)     OP_math_div(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), val_lhs, val_rhs)
#define math_divSafe(val_lhs, val_rhs) OP_math_divSafe(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), val_lhs, val_rhs)
#define math_mod(val_lhs, val_rhs)     OP_math_mod(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), val_lhs, val_rhs)
#define math_modSafe(val_lhs, val_rhs) OP_math_modSafe(pp_uniqTok(lhs), pp_uniqTok(rhs), pp_uniqTok(ret), val_lhs, val_rhs)

/* Basic functions */
#define math_abs(val_x)                         FUNC_math_abs(pp_uniqTok(abs_x), val_x)
#define math_sign(val_x)                        FUNC_math_sign(pp_uniqTok(sign_x), val_x)
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
    (TScalar)0, \
    u8: u8_limit_min, \
    u16: u16_limit_min, \
    u32: u32_limit_min, \
    u64: u64_limit_min, \
    i8: i8_limit_min, \
    i16: i16_limit_min, \
    i32: i32_limit_min, \
    i64: i64_limit_min, \
    f32: f32_limit_min, \
    f64: f64_limit_min, \
    default: 0 \
)
#define VAL_math_limit_max$(TScalar) _Generic( \
    (TScalar)0, \
    u8: u8_limit_max, \
    u16: u16_limit_max, \
    u32: u32_limit_max, \
    u64: u64_limit_max, \
    i8: i8_limit_max, \
    i16: i16_limit_max, \
    i32: i32_limit_max, \
    i64: i64_limit_max, \
    f32: f32_limit_max, \
    f64: f64_limit_max, \
    default: 0 \
)

#define VAL_math_f32_pi           as$(f32)(math_pi)
#define VAL_math_f32_tau          as$(f32)(math_tau)
#define VAL_math_f32_e            as$(f32)(math_e)
#define VAL_math_f32_golden_ratio as$(f32)(math_golden_ratio)
#define VAL_math_f32_sqrt_2       as$(f32)(math_sqrt_2)
#define VAL_math_f32_sqrt_3       as$(f32)(math_sqrt_3)
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
#define OP_math_cmp(__lhs, __rhs, __ret, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    var __ret = cmp_Ord_eq; \
    if (isFlt(TypeOf(__lhs))) { \
        if (math_abs((__lhs) - (__rhs)) <= math_eps) { \
            (__ret) = cmp_Ord_eq; \
        } else if ((__lhs) < (__rhs)) { \
            (__ret) = cmp_Ord_lt; \
        } else { \
            (__ret) = cmp_Ord_gt; \
        } \
    } else { \
        if ((__lhs) < (__rhs)) { \
            (__ret) = cmp_Ord_lt; \
        } else if ((__lhs) > (__rhs)) { \
            (__ret) = cmp_Ord_gt; \
        } else { \
            (__ret) = cmp_Ord_eq; \
        } \
    }; \
    blk_return __ret; \
})
#define OP_math_eq(val_lhs, val_rhs) (math_cmp(val_lhs, val_rhs) == cmp_Ord_eq)
#define OP_math_ne(val_lhs, val_rhs) (math_cmp(val_lhs, val_rhs) != cmp_Ord_eq)
#define OP_math_lt(val_lhs, val_rhs) (math_cmp(val_lhs, val_rhs) == cmp_Ord_lt)
#define OP_math_gt(val_lhs, val_rhs) (math_cmp(val_lhs, val_rhs) == cmp_Ord_gt)
#define OP_math_le(val_lhs, val_rhs) (!math_gt(val_lhs, val_rhs))
#define OP_math_ge(val_lhs, val_rhs) (!math_lt(val_lhs, val_rhs))

/* Arithmetic operations */
#define OP_math_neg(val_x)                                 (-(_val_x))
#define OP_math_add(val_lhs, val_rhs)                      ((val_lhs) + (val_rhs))
#define OP_math_sub(val_lhs, val_rhs)                      ((val_lhs) - (val_rhs))
#define OP_math_mul(val_lhs, val_rhs)                      ((val_lhs) * (val_rhs))
#define OP_math_div(__lhs, __rhs, __ret, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    var __ret = make$((TypeOf(__lhs)){}); \
    if ((__rhs) != 0) { \
        (__ret) = (__lhs) / (__rhs); \
    } \
    blk_return __ret; \
})
#define OP_math_divSafe(__lhs, __rhs, __ret, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    P$raw __ret = null; \
    if ((__rhs) == 0) { \
        (__ret) = as$(P$raw)(&(E$(TypeOf(__lhs)) err(math_Err_err(math_ErrCode_DivisionByZero)))); \
    } else { \
        (__ret) = as$(P$raw)(&(E$(TypeOf(__lhs)) ok((__lhs) / (__rhs)))); \
    } \
    blk_return( \
        *as$(E$(TypeOf(__lhs))*, __ret) \
    ); \
})
#define OP_math_mod(__lhs, __rhs, __ret, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    var __ret = make$((TypeOf(__lhs)){}); \
    if ((__rhs) != 0) { \
        if (isFlt(TypeOf(__ret))) { \
            (__ret) = as$(TypeOf(__ret))(flt_rem(as$(f64)(__lhs), as$(f64)(__rhs))); \
        } else { \
            (__ret) = as$(TypeOf(__ret))(as$(i64)(__lhs) % as$(i64)(__rhs)); \
        } \
    } \
    blk_return __ret; \
})
#define OP_math_modSafe(__lhs, __rhs, __ret, val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    P$raw __ret = null; \
    if ((__rhs) == 0) { \
        (__ret) = as$(P$raw)(&(E$(TypeOf(__lhs)) err(math_Err_err(math_ErrCode_DivisionByZero)))); \
    } else { \
        if (isFlt(TypeOf(__ret))) { \
            (__ret) = as$(P$raw)(&(E$(TypeOf(__lhs)) ok(flt_rem(as$(f64)(__lhs), as$(f64)(__rhs))))); \
        } else { \
            (__ret) = as$(P$raw)(&(E$(TypeOf(__lhs)) ok(as$(i64)(__lhs) % as$(i64)(__rhs)))); \
        } \
    } \
    blk_return( \
        *as$(E$(TypeOf(__lhs))*, __ret) \
    ); \
})

/* Basic functions */
#define FUNC_math_abs(_x, val_x) blk({ \
    var _x = (val_x); \
    if (isFlt(TypeOf(_x))) { \
        (_x) = as$(TypeOf(_x))(flt_abs(as$(f64)(_x))); \
    } else { \
        (_x) = (_x) < 0 ? -(_x) : (_x); \
    } \
    blk_return _x; \
})
#define FUNC_math_sign(_x, val_x) blk({ \
    let _x = (val_x); \
    blk_return _x < 0 ? -1 : ((_x) > 0 ? 1 : 0); \
})
#define FUNC_math_min(val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return __lhs < __rhs ? __lhs : __rhs; \
})
#define FUNC_math_max(val_lhs, val_rhs) blk({ \
    let __lhs = (val_lhs); \
    let __rhs = (val_rhs); \
    blk_return __lhs > __rhs ? __lhs : __rhs; \
})
#define FUNC_math_clamp(val_x, val_min, val_max) blk({ \
    let _x = (val_x); \
    let _min = (val_min); \
    let _max = (val_max); \
    blk_return math_min(math_max(_min, _x), _max); \
})
#define FUNC_math_clampSafe(val_x, val_min, val_max) blk({ \
    let _x = val_x; \
    let _min = val_min; \
    let _max = val_max; \
    blk_return((E$(TypeOf(_x)))ok(math_clamp(_x, _min, _max))); \
})
#define FUNC_math_wrap(val_x, val_min, val_max) blk({ \
    let _x = (val_x); \
    let _min = (val_min); \
    let _max = (val_max); \
    blk_return _min + math_mod(_x - _min, _max - _min); \
})
#define FUNC_math_wrapSafe(val_x, val_min, val_max) blk({ \
    let _x = val_x; \
    let _min = val_min; \
    let _max = val_max; \
    blk_return((E$(TypeOf(_x)))ok(math_wrap(_x, _min, _max))); \
})

/* Rounding functions */
#define FUNC_math_floor(val_x) blk({ \
    let _x = (val_x); \
    blk_return as$(TypeOf(_x))(flt_floor(as$(f64)(_x))); \
})
#define FUNC_math_ceil(val_x) blk({ \
    let _x = (val_x); \
    blk_return as$(TypeOf(_x))(flt_ceil(as$(f64)(_x))); \
})
#define FUNC_math_round(val_x) blk({ \
    let _x = (val_x); \
    blk_return as$(TypeOf(_x))(flt_round(as$(f64)(_x))); \
})
#define FUNC_math_trunc(val_x) blk({ \
    let _x = (val_x); \
    blk_return as$(TypeOf(_x))(flt_trunc(as$(f64)(_x))); \
})

/* Exponential/logarithmic functions */
#define FUNC_math_sqrt(val_x) blk({ \
    let _x = (val_x); \
    var __ret = make$((TypeOf(_x)){}); \
    if (0 <= _x) { \
        __ret = as$(TypeOf(__ret))(flt_sqrt(as$(f64)(_x))); \
    } \
    blk_return __ret; \
})
#define FUNC_math_sqrtSafe(val_x) blk({ \
    let _x = (val_x); \
    P$raw __ret = null; \
    if (_x < 0) { \
        /* Square root of negative number */ \
        __ret = (P$raw)&(E$(TypeOf(_x)))err(math_Err_err(math_ErrCode_InvalidDomain)); \
    } else { \
        __ret = (P$raw)&(E$(TypeOf(_x)))ok(as$(TypeOf(__ret))(flt_sqrt(as$(f64)(_x)))); \
    } \
    blk_return(*(E$(TypeOf(_x))*)__ret); \
})
#define FUNC_math_pow(val_base, val_exp) blk({ \
    let _base = (val_base); \
    let _exp = (val_exp); \
    var __ret = make$((TypeOf(_base)){}); \
    if (_base != 0 || 0 < _exp) { \
        __ret = as$(TypeOf(__ret))(flt_powF(as$(f64)(_base), as$(f64)(_exp))); \
    } \
    blk_return __ret; \
})
#define FUNC_math_powSafe(val_base, val_exp) blk({ \
    let _base = (val_base); \
    let _exp = (val_exp); \
    P$raw __ret = null; \
    if (_base == 0 && _exp <= 0) { \
        /* Invalid base/exponent combination */ \
        __ret = (P$raw)&(E$(TypeOf(_base)))err(math_Err_err(math_ErrCode_InvalidDomain)); \
    } else { \
        __ret = (P$raw)&(E$(TypeOf(_base)))ok(as$(TypeOf(__ret))(flt_powF(as$(f64)(_base), as$(f64)(_exp)))); \
    } \
    blk_return(*(E$(TypeOf(_x))*)__ret); \
})
#define FUNC_math_rsqrt(val_x) blk({ \
    let _x = (val_x); \
    var __ret = make$((TypeOf(_x)){}); \
    if (0 < _x) { \
        __ret = as$(TypeOf(__ret))(1.0 / flt_sqrt(as$(f64)(_x))); \
    } \
    blk_return __ret; \
})
#define FUNC_math_rsqrtSafe(val_x) blk({ \
    let _x = (val_x); \
    P$raw __ret = null; \
    if (_x <= 0) { \
        /* Invalid input for inverse square root */ \
        __ret = (P$raw)&(E$(TypeOf(_x)))err(math_Err_err(math_ErrCode_InvalidDomain)); \
    } else { \
        __ret = (P$raw)&(E$(TypeOf(_x)))ok(as$(TypeOf(__ret))(1.0 / flt_sqrt(as$(f64)(_x)))); \
    } \
    blk_return(*(E$(TypeOf(_x))*)__ret); \
})

/* Trigonometric functions */
#define FUNC_math_sin(val_x) blk({ \
    let _x = (val_x); \
    blk_return as$(TypeOf(_x))(flt_sin(as$(f64)(_x))); \
})
#define FUNC_math_cos(val_x) blk({ \
    let _x = (val_x); \
    blk_return as$(TypeOf(_x))(flt_cos(as$(f64)(_x))); \
})
#define FUNC_math_tan(val_x) blk({ \
    let _x = (val_x); \
    blk_return as$(TypeOf(_x))(flt_tan(as$(f64)(_x))); \
})
#define FUNC_math_asin(val_x) blk({ \
    let _x = (val_x); \
    var __ret = make$((TypeOf(_x)){}); \
    if (-1 <= _x && _x <= 1) { \
        __ret = flt_asin(_x); \
    } \
    blk_return __ret; \
})
#define FUNC_math_asinSafe(val_x) blk({ \
    let _x = (val_x); \
    P$raw __ret = null; \
    if (_x < -1 || 1 < _x) { \
        /* Inverse sine domain error */ \
        __ret = (P$raw)&(E$(TypeOf(_x)))err(math_Err_err(math_ErrCode_InvalidDomain)); \
    } else { \
        __ret = (P$raw)&(E$(TypeOf(_x)))ok(flt_asin(_x)); \
    } \
    blk_return(*(E$(TypeOf(_x))*)__ret); \
})
#define FUNC_math_acos(val_x) blk({ \
    let _x = (val_x); \
    var __ret = make$((TypeOf(_x)){}); \
    if (-1 <= _x && _x <= 1) { \
        __ret = flt_acos(_x); \
    } \
    blk_return __ret; \
})
#define FUNC_math_acosSafe(val_x) blk({ \
    let _x = (val_x); \
    P$raw __ret = null; \
    if (_x < -1 || 1 < _x) { \
        /* Inverse cosine domain error */ \
        __ret = (P$raw)&(E$(TypeOf(_x)))err(math_Err_err(math_ErrCode_InvalidDomain)); \
    } else { \
        __ret = (P$raw)&(E$(TypeOf(_x)))ok(flt_acos(_x)); \
    } \
    blk_return(*(E$(TypeOf(_x))*)__ret); \
})
#define FUNC_math_atan(val_x) blk({ \
    let _x = (val_x); \
    blk_return flt_atan(_x); \
})
#define FUNC_math_atan2(val_y, val_x) blk({ \
    let _y = (val_y); \
    let _x = (val_x); \
    var __ret = make$((TypeOf(_y)){}); \
    if (_y != 0 || _x != 0) { \
        __ret = flt_atan2(_y, _x); \
    } \
    blk_return __ret; \
})
#define FUNC_math_atan2Safe(val_y, val_x) blk({ \
    let _y = (val_y); \
    let _x = (val_x); \
    P$raw __ret = null; \
    if (_x == 0 && _y == 0) { \
        /* flt_atan2(0, 0) is undefined */ \
        __ret = (P$raw)&(E$(TypeOf(_y)))err(math_Err_err(math_ErrCode_InvalidDomain)); \
    } else { \
        __ret = (P$raw)&(E$(TypeOf(_y)))ok(flt_atan2(_y, _x)); \
    } \
    blk_return(*(E$(TypeOf(_y))*)__ret); \
})

#endif /* MATH_COMMON_INCLUDED */
