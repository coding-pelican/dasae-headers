/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    flt.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-10-25 (date of creation)
 * @updated 2025-10-25 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 */

#ifndef flt__included
#define flt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== includes =======================================================*/

#include "common.h"

/*========== Macros and Definitions =========================================*/

$inline_always bool f32_isNan(f32 x) {
    return flt_isNan(x);
#if UNUSED_CODE
    // IEEE 754 비트 레벨 구현
    union {
        f32 f;
        u32 u;
    } conv = {.f = x};

    u32 exponent = (conv.u >> 23) & 0xFF;
    u32 mantissa = conv.u & 0x7FFFFF;

    // NaN: exponent = 0xFF, mantissa != 0
    return (exponent == 0xFF) && (mantissa != 0);
#endif /* UNUSED_CODE */
}

$inline_always bool f32_isInf(f32 x) {
    return flt_isInf(x);
#if UNUSED_CODE
    // IEEE 754 비트 레벨 구현
    union {
        f32 f;
        u32 u;
    } conv = {.f = x};

    u32 exponent = (conv.u >> 23) & 0xFF;
    u32 mantissa = conv.u & 0x7FFFFF;

    // Inf: exponent = 0xFF, mantissa = 0
    return (exponent == 0xFF) && (mantissa == 0);
#endif /* UNUSED_CODE */
}

$inline_always bool f64_isNan(f64 x) {
    return flt_isNan(x);
#if UNUSED_CODE
    // IEEE 754 비트 레벨 구현
    union {
        f64 f;
        u64 u;
    } conv = {.f = x};

    u64 exponent = (conv.u >> 52) & 0x7FF;
    u64 mantissa = conv.u & 0xFFFFFFFFFFFFFULL;

    // NaN: exponent = 0x7FF, mantissa != 0
    return (exponent == 0x7FF) && (mantissa != 0);
#endif /* UNUSED_CODE */
}

$inline_always i32 f64_isInf(f64 x) {
    return flt_isInf(x);
#if UNUSED_CODE
    // IEEE 754 비트 레벨 구현
    union {
        f64 f;
        u64 u;
    } conv = {.f = x};

    u64 exponent = (conv.u >> 52) & 0x7FF;
    u64 mantissa = conv.u & 0xFFFFFFFFFFFFFULL;

    // Inf: exponent = 0x7FF, mantissa = 0
    return (exponent == 0x7FF) && (mantissa == 0);
#endif /* UNUSED_CODE */
}

$inline_always bool f32_isFinite(f32 x) {
    return flt_isFinite(x);
}

$inline_always bool f64_isFinite(f64 x) {
    return flt_isFinite(x);
}

$inline_always bool f32_isNormal(f32 x) {
    return flt_isNormal(x);
#if UNUSED_CODE
    union {
        f32 f;
        u32 u;
    } conv = {.f = x};

    u32 exponent = (conv.u >> 23) & 0xFF;
    return (exponent != 0) && (exponent != 0xFF);
#endif /* UNUSED_CODE */
}

$inline_always bool f64_isNormal(f64 x) {
    return flt_isNormal(x);
#if UNUSED_CODE
    union {
        f64 f;
        u64 u;
    } conv = {.f = x};

    u64 exponent = (conv.u >> 52) & 0x7FF;
    return (exponent != 0) && (exponent != 0x7FF);
#endif /* UNUSED_CODE */
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* flt__included */
