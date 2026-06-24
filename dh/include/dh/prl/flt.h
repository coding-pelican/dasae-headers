/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    flt.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-10-25 (date of creation)
 * @updated 2026-06-03 (date of last update)
 * @ingroup dasae-headers(dh)/prl
 * @prefix  (none)
 */
#pragma once
#ifndef prl_flt__included
#define prl_flt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

$attr($inline_always)
$static fn_((f32_sgnBit(f32 x))(bool));
$attr($inline_always)
$static fn_((f32_isNaN(f32 x))(bool));
$attr($inline_always)
$static fn_((f32_isInf(f32 x))(bool));
$attr($inline_always)
$static fn_((f32_isFinite(f32 x))(bool));
$attr($inline_always)
$static fn_((f32_isNormal(f32 x))(bool));
$attr($inline_always)
$static fn_((f32_isSubnormal(f32 x))(bool));
$attr($inline_always)
$static fn_((f32_isZero(f32 x))(bool));
$attr($inline_always)
$static fn_((f32_isNonzero(f32 x))(bool));

$attr($inline_always)
$static fn_((f64_sgnBit(f64 x))(bool));
$attr($inline_always)
$static fn_((f64_isNaN(f64 x))(bool));
$attr($inline_always)
$static fn_((f64_isInf(f64 x))(bool));
$attr($inline_always)
$static fn_((f64_isFinite(f64 x))(bool));
$attr($inline_always)
$static fn_((f64_isNormal(f64 x))(bool));
$attr($inline_always)
$static fn_((f64_isSubnormal(f64 x))(bool));
$attr($inline_always)
$static fn_((f64_isZero(f64 x))(bool));
$attr($inline_always)
$static fn_((f64_isNonzero(f64 x))(bool));

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((f32_sgnBit(f32 x))(bool)) {
    return flt_sgnBit(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f32 f;
        u32 u;
    } conv = { .f = x };
    // Sign bit: check the 32nd bit (MSB)
    return (conv.u >> 31) != 0;
#endif /* UNUSED_CODE */
};
fn_((f32_isNaN(f32 x))(bool)) {
    return flt_isNaN(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f32 f;
        u32 u;
    } conv = { .f = x };
    u32 exponent = (conv.u >> 23) & 0xFF;
    u32 mantissa = conv.u & 0x7FFFFF;
    // NaN: exponent = 0xFF, mantissa != 0
    return (exponent == 0xFF) && (mantissa != 0);
#endif /* UNUSED_CODE */
};
fn_((f32_isInf(f32 x))(bool)) {
    return flt_isInf(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f32 f;
        u32 u;
    } conv = { .f = x };
    u32 exponent = (conv.u >> 23) & 0xFF;
    u32 mantissa = conv.u & 0x7FFFFF;
    // Inf: exponent = 0xFF, mantissa = 0
    return (exponent == 0xFF) && (mantissa == 0);
#endif /* UNUSED_CODE */
};
fn_((f32_isFinite(f32 x))(bool)) {
    return flt_isFinite(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f32 f;
        u32 u;
    } conv = { .f = x };
    u32 exponent = (conv.u >> 23) & 0xFF;
    return (exponent != 0xFF);
#endif /* UNUSED_CODE */
};
fn_((f32_isNormal(f32 x))(bool)) {
    return flt_isNormal(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f32 f;
        u32 u;
    } conv = { .f = x };
    u32 exponent = (conv.u >> 23) & 0xFF;
    return (exponent != 0) && (exponent != 0xFF);
#endif /* UNUSED_CODE */
};
fn_((f32_isSubnormal(f32 x))(bool)) {
#if UNUSED_CODE
    return flt_isSubnormal(x);
#endif /* UNUSED_CODE */
    // IEEE 754 bit-level implementation
    union {
        f32 f;
        u32 u;
    } conv = { .f = x };
    u32 exponent = (conv.u >> 23) & 0xFF;
    u32 mantissa = conv.u & 0x7FFFFF;
    // Subnormal: exponent = 0, mantissa != 0
    return (exponent == 0) && (mantissa != 0);
};
fn_((f32_isZero(f32 x))(bool)) {
    return flt_isZero(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f32 f;
        u32 u;
    } conv = { .f = x };
    // Zero: all bits except the sign bit are 0 (+0.0 or -0.0)
    return (conv.u & 0x7FFFFFFF) == 0;
#endif /* UNUSED_CODE */
};
fn_((f32_isNonzero(f32 x))(bool)) {
    return !f32_isZero(x);
};
fn_((f64_sgnBit(f64 x))(bool)) {
    return flt_sgnBit(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f64 f;
        u64 u;
    } conv = { .f = x };
    // Sign bit: check the 64th bit (MSB)
    return (conv.u >> 63) != 0;
#endif /* UNUSED_CODE */
};
fn_((f64_isNaN(f64 x))(bool)) {
    return flt_isNaN(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f64 f;
        u64 u;
    } conv = { .f = x };
    u64 exponent = (conv.u >> 52) & 0x7FF;
    u64 mantissa = conv.u & 0xFFFFFFFFFFFFFULL;
    // NaN: exponent = 0x7FF, mantissa != 0
    return (exponent == 0x7FF) && (mantissa != 0);
#endif /* UNUSED_CODE */
};
fn_((f64_isInf(f64 x))(bool)) {
    return flt_isInf(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f64 f;
        u64 u;
    } conv = { .f = x };
    u64 exponent = (conv.u >> 52) & 0x7FF;
    u64 mantissa = conv.u & 0xFFFFFFFFFFFFFULL;
    // Inf: exponent = 0x7FF, mantissa = 0
    return (exponent == 0x7FF) && (mantissa == 0);
#endif /* UNUSED_CODE */
};
fn_((f64_isFinite(f64 x))(bool)) {
    return flt_isFinite(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f64 f;
        u64 u;
    } conv = { .f = x };
    u64 exponent = (conv.u >> 52) & 0x7FF;
    return (exponent != 0x7FF);
#endif /* UNUSED_CODE */
};
fn_((f64_isNormal(f64 x))(bool)) {
    return flt_isNormal(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f64 f;
        u64 u;
    } conv = { .f = x };
    u64 exponent = (conv.u >> 52) & 0x7FF;
    return (exponent != 0) && (exponent != 0x7FF);
#endif /* UNUSED_CODE */
};
fn_((f64_isSubnormal(f64 x))(bool)) {
#if UNUSED_CODE
    return flt_isSubnormal(x);
#endif /* UNUSED_CODE */
    // IEEE 754 bit-level implementation
    union {
        f64 f;
        u64 u;
    } conv = { .f = x };
    u64 exponent = (conv.u >> 52) & 0x7FF;
    u64 mantissa = conv.u & 0xFFFFFFFFFFFFFULL;
    // Subnormal: exponent = 0, mantissa != 0
    return (exponent == 0) && (mantissa != 0);
};
fn_((f64_isZero(f64 x))(bool)) {
    return flt_isZero(x);
#if UNUSED_CODE
    // IEEE 754 bit-level implementation
    union {
        f64 f;
        u64 u;
    } conv = { .f = x };
    // Zero: all bits except the sign bit are 0 (+0.0 or -0.0)
    return (conv.u & 0x7FFFFFFFFFFFFFFFULL) == 0;
#endif /* UNUSED_CODE */
};
fn_((f64_isNonzero(f64 x))(bool)) {
    return !f64_isZero(x);
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_flt__included */
