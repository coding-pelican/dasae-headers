/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    int.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-02 (date of creation)
 * @updated 2025-03-02 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Integer type overflow checking utilities
 * @details Provides utilities for checking integer overflows.
 */

#ifndef INT_INCLUDED
#define INT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/opt.h"
#include "dh/meta/common.h"

/*========== Macros and Definitions =========================================*/

/// Signed integer overflow checking functions

force_inline Opt$i8 i8_chkdAdd(i8 lhs, i8 rhs);
force_inline Opt$i8 i8_chkdSub(i8 lhs, i8 rhs);
force_inline Opt$i8 i8_chkdMul(i8 lhs, i8 rhs);
force_inline Opt$i8 i8_chkdDiv(i8 lhs, i8 rhs);
force_inline Opt$i8 i8_chkdMod(i8 lhs, i8 rhs);
// force_inline Opt$i8 i8_chkdInc(i8 x);
// force_inline Opt$i8 i8_chkdDec(i8 x);

force_inline Opt$i16 i16_chkdAdd(i16 lhs, i16 rhs);
force_inline Opt$i16 i16_chkdSub(i16 lhs, i16 rhs);
force_inline Opt$i16 i16_chkdMul(i16 lhs, i16 rhs);
force_inline Opt$i16 i16_chkdDiv(i16 lhs, i16 rhs);
force_inline Opt$i16 i16_chkdMod(i16 lhs, i16 rhs);
// force_inline Opt$i16 i16_chkdInc(i16 x);
// force_inline Opt$i16 i16_chkdDec(i16 x);

force_inline Opt$i32 i32_chkdAdd(i32 lhs, i32 rhs);
force_inline Opt$i32 i32_chkdSub(i32 lhs, i32 rhs);
force_inline Opt$i32 i32_chkdMul(i32 lhs, i32 rhs);
force_inline Opt$i32 i32_chkdDiv(i32 lhs, i32 rhs);
force_inline Opt$i32 i32_chkdMod(i32 lhs, i32 rhs);
// force_inline Opt$i32 i32_chkdInc(i32 x);
// force_inline Opt$i32 i32_chkdDec(i32 x);

force_inline Opt$i64 i64_chkdAdd(i64 lhs, i64 rhs);
force_inline Opt$i64 i64_chkdSub(i64 lhs, i64 rhs);
force_inline Opt$i64 i64_chkdMul(i64 lhs, i64 rhs);
force_inline Opt$i64 i64_chkdDiv(i64 lhs, i64 rhs);
force_inline Opt$i64 i64_chkdMod(i64 lhs, i64 rhs);
// force_inline Opt$i64 i64_chkdInc(i64 x);
// force_inline Opt$i64 i64_chkdDec(i64 x);

force_inline Opt$isize isize_chkdAdd(isize lhs, isize rhs);
force_inline Opt$isize isize_chkdSub(isize lhs, isize rhs);
force_inline Opt$isize isize_chkdMul(isize lhs, isize rhs);
force_inline Opt$isize isize_chkdDiv(isize lhs, isize rhs);
force_inline Opt$isize isize_chkdMod(isize lhs, isize rhs);
// force_inline Opt$isize isize_chkdInc(isize x);
// force_inline Opt$isize isize_chkdDec(isize x);

/// Unsigned integer overflow checking functions

force_inline Opt$u8 u8_chkdAdd(u8 lhs, u8 rhs);
force_inline Opt$u8 u8_chkdSub(u8 lhs, u8 rhs);
force_inline Opt$u8 u8_chkdMul(u8 lhs, u8 rhs);
force_inline Opt$u8 u8_chkdDiv(u8 lhs, u8 rhs);
force_inline Opt$u8 u8_chkdMod(u8 lhs, u8 rhs);
// force_inline Opt$u8 u8_chkdInc(u8 x);
// force_inline Opt$u8 u8_chkdDec(u8 x);

force_inline Opt$u16 u16_chkdAdd(u16 lhs, u16 rhs);
force_inline Opt$u16 u16_chkdSub(u16 lhs, u16 rhs);
force_inline Opt$u16 u16_chkdMul(u16 lhs, u16 rhs);
force_inline Opt$u16 u16_chkdDiv(u16 lhs, u16 rhs);
force_inline Opt$u16 u16_chkdMod(u16 lhs, u16 rhs);
// force_inline Opt$u16 u16_chkdInc(u16 x);
// force_inline Opt$u16 u16_chkdDec(u16 x);

force_inline Opt$u32 u32_chkdAdd(u32 lhs, u32 rhs);
force_inline Opt$u32 u32_chkdSub(u32 lhs, u32 rhs);
force_inline Opt$u32 u32_chkdMul(u32 lhs, u32 rhs);
force_inline Opt$u32 u32_chkdDiv(u32 lhs, u32 rhs);
force_inline Opt$u32 u32_chkdMod(u32 lhs, u32 rhs);
// force_inline Opt$u32 u32_chkdInc(u32 x);
// force_inline Opt$u32 u32_chkdDec(u32 x);

force_inline Opt$u64 u64_chkdAdd(u64 lhs, u64 rhs);
force_inline Opt$u64 u64_chkdSub(u64 lhs, u64 rhs);
force_inline Opt$u64 u64_chkdMul(u64 lhs, u64 rhs);
force_inline Opt$u64 u64_chkdDiv(u64 lhs, u64 rhs);
force_inline Opt$u64 u64_chkdMod(u64 lhs, u64 rhs);
// force_inline Opt$u64 u64_chkdInc(u64 x);
// force_inline Opt$u64 u64_chkdDec(u64 x);

force_inline Opt$usize usize_chkdAdd(usize lhs, usize rhs);
force_inline Opt$usize usize_chkdSub(usize lhs, usize rhs);
force_inline Opt$usize usize_chkdMul(usize lhs, usize rhs);
force_inline Opt$usize usize_chkdDiv(usize lhs, usize rhs);
force_inline Opt$usize usize_chkdMod(usize lhs, usize rhs);
// force_inline Opt$usize usize_chkdInc(usize x);
// force_inline Opt$usize usize_chkdDec(usize x);

/*========== Macros and Implementations =====================================*/

/// Signed integer implementations

force_inline Opt$i8 i8_chkdAdd(i8 lhs, i8 rhs) {
    i8 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$i8); }
    return some$(Opt$i8, result);
}
force_inline Opt$i8 i8_chkdSub(i8 lhs, i8 rhs) {
    i8 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$i8); }
    return some$(Opt$i8, result);
}
force_inline Opt$i8 i8_chkdMul(i8 lhs, i8 rhs) {
    i8 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$i8); }
    return some$(Opt$i8, result);
}
force_inline Opt$i8 i8_chkdDiv(i8 lhs, i8 rhs) {
    if (rhs == 0 || (lhs == i8_limit_min && rhs == -1)) { return none$(Opt$i8); }
    return some$(Opt$i8, lhs / rhs);
}
force_inline Opt$i8 i8_chkdMod(i8 lhs, i8 rhs) {
    if (rhs == 0 || (lhs == i8_limit_min && rhs == -1)) { return none$(Opt$i8); }
    return some$(Opt$i8, lhs % rhs);
}
// force_inline Opt$i8 i8_chkdInc(i8 x) { return i8_chkdAdd(x, 1); }
// force_inline Opt$i8 i8_chkdDec(i8 x) { return i8_chkdSub(x, 1); }

force_inline Opt$i16 i16_chkdAdd(i16 lhs, i16 rhs) {
    i16 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$i16); }
    return some$(Opt$i16, result);
}
force_inline Opt$i16 i16_chkdSub(i16 lhs, i16 rhs) {
    i16 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$i16); }
    return some$(Opt$i16, result);
}
force_inline Opt$i16 i16_chkdMul(i16 lhs, i16 rhs) {
    i16 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$i16); }
    return some$(Opt$i16, result);
}
force_inline Opt$i16 i16_chkdDiv(i16 lhs, i16 rhs) {
    if (rhs == 0 || (lhs == i16_limit_min && rhs == -1)) { return none$(Opt$i16); }
    return some$(Opt$i16, lhs / rhs);
}
force_inline Opt$i16 i16_chkdMod(i16 lhs, i16 rhs) {
    if (rhs == 0 || (lhs == i16_limit_min && rhs == -1)) { return none$(Opt$i16); }
    return some$(Opt$i16, lhs % rhs);
}
// force_inline Opt$i16 i16_chkdInc(i16 x) { return i16_chkdAdd(x, 1); }
// force_inline Opt$i16 i16_chkdDec(i16 x) { return i16_chkdSub(x, 1); }

force_inline Opt$i32 i32_chkdAdd(i32 lhs, i32 rhs) {
    i32 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$i32); }
    return some$(Opt$i32, result);
}
force_inline Opt$i32 i32_chkdSub(i32 lhs, i32 rhs) {
    i32 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$i32); }
    return some$(Opt$i32, result);
}
force_inline Opt$i32 i32_chkdMul(i32 lhs, i32 rhs) {
    i32 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$i32); }
    return some$(Opt$i32, result);
}
force_inline Opt$i32 i32_chkdDiv(i32 lhs, i32 rhs) {
    if (rhs == 0 || (lhs == i32_limit_min && rhs == -1)) { return none$(Opt$i32); }
    return some$(Opt$i32, lhs / rhs);
}
force_inline Opt$i32 i32_chkdMod(i32 lhs, i32 rhs) {
    if (rhs == 0 || (lhs == i32_limit_min && rhs == -1)) { return none$(Opt$i32); }
    return some$(Opt$i32, lhs % rhs);
}
// force_inline Opt$i32 i32_chkdInc(i32 x) { return i32_chkdAdd(x, 1); }
// force_inline Opt$i32 i32_chkdDec(i32 x) { return i32_chkdSub(x, 1); }

force_inline Opt$i64 i64_chkdAdd(i64 lhs, i64 rhs) {
    i64 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$i64); }
    return some$(Opt$i64, result);
}
force_inline Opt$i64 i64_chkdSub(i64 lhs, i64 rhs) {
    i64 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$i64); }
    return some$(Opt$i64, result);
}
force_inline Opt$i64 i64_chkdMul(i64 lhs, i64 rhs) {
    i64 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$i64); }
    return some$(Opt$i64, result);
}
force_inline Opt$i64 i64_chkdDiv(i64 lhs, i64 rhs) {
    if (rhs == 0 || (lhs == i64_limit_min && rhs == -1)) { return none$(Opt$i64); }
    return some$(Opt$i64, lhs / rhs);
}
force_inline Opt$i64 i64_chkdMod(i64 lhs, i64 rhs) {
    if (rhs == 0 || (lhs == i64_limit_min && rhs == -1)) { return none$(Opt$i64); }
    return some$(Opt$i64, lhs % rhs);
}
// force_inline Opt$i64 i64_chkdInc(i64 x) { return i64_chkdAdd(x, 1); }
// force_inline Opt$i64 i64_chkdDec(i64 x) { return i64_chkdSub(x, 1); }

force_inline Opt$isize isize_chkdAdd(isize lhs, isize rhs) {
    isize result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$isize); }
    return some$(Opt$isize, result);
}
force_inline Opt$isize isize_chkdSub(isize lhs, isize rhs) {
    isize result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$isize); }
    return some$(Opt$isize, result);
}
force_inline Opt$isize isize_chkdMul(isize lhs, isize rhs) {
    isize result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$isize); }
    return some$(Opt$isize, result);
}
force_inline Opt$isize isize_chkdDiv(isize lhs, isize rhs) {
    if (rhs == 0 || (lhs == isize_limit_min && rhs == -1)) { return none$(Opt$isize); }
    return some$(Opt$isize, lhs / rhs);
}
force_inline Opt$isize isize_chkdMod(isize lhs, isize rhs) {
    if (rhs == 0 || (lhs == isize_limit_min && rhs == -1)) { return none$(Opt$isize); }
    return some$(Opt$isize, lhs % rhs);
}
// force_inline Opt$isize isize_chkdInc(isize x) { return isize_chkdAdd(x, 1); }
// force_inline Opt$isize isize_chkdDec(isize x) { return isize_chkdSub(x, 1); }

/// Unsigned integer implementations

force_inline Opt$u8 u8_chkdAdd(u8 lhs, u8 rhs) {
    u8 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$u8); }
    return some$(Opt$u8, result);
}
force_inline Opt$u8 u8_chkdSub(u8 lhs, u8 rhs) {
    u8 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$u8); }
    return some$(Opt$u8, result);
}
force_inline Opt$u8 u8_chkdMul(u8 lhs, u8 rhs) {
    u8 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$u8); }
    return some$(Opt$u8, result);
}
force_inline Opt$u8 u8_chkdDiv(u8 lhs, u8 rhs) {
    if (rhs == 0) { return none$(Opt$u8); }
    return some$(Opt$u8, lhs / rhs);
}
force_inline Opt$u8 u8_chkdMod(u8 lhs, u8 rhs) {
    if (rhs == 0) { return none$(Opt$u8); }
    return some$(Opt$u8, lhs % rhs);
}
// force_inline Opt$u8 u8_chkdInc(u8 x) { return u8_chkdAdd(x, 1); }
// force_inline Opt$u8 u8_chkdDec(u8 x) { return u8_chkdSub(x, 1); }

force_inline Opt$u16 u16_chkdAdd(u16 lhs, u16 rhs) {
    u16 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$u16); }
    return some$(Opt$u16, result);
}
force_inline Opt$u16 u16_chkdSub(u16 lhs, u16 rhs) {
    u16 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$u16); }
    return some$(Opt$u16, result);
}
force_inline Opt$u16 u16_chkdMul(u16 lhs, u16 rhs) {
    u16 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$u16); }
    return some$(Opt$u16, result);
}
force_inline Opt$u16 u16_chkdDiv(u16 lhs, u16 rhs) {
    if (rhs == 0) { return none$(Opt$u16); }
    return some$(Opt$u16, lhs / rhs);
}
force_inline Opt$u16 u16_chkdMod(u16 lhs, u16 rhs) {
    if (rhs == 0) { return none$(Opt$u16); }
    return some$(Opt$u16, lhs % rhs);
}
// force_inline Opt$u16 u16_chkdInc(u16 x) { return u16_chkdAdd(x, 1); }
// force_inline Opt$u16 u16_chkdDec(u16 x) { return u16_chkdSub(x, 1); }

force_inline Opt$u32 u32_chkdAdd(u32 lhs, u32 rhs) {
    u32 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$u32); }
    return some$(Opt$u32, result);
}
force_inline Opt$u32 u32_chkdSub(u32 lhs, u32 rhs) {
    u32 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$u32); }
    return some$(Opt$u32, result);
}
force_inline Opt$u32 u32_chkdMul(u32 lhs, u32 rhs) {
    u32 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$u32); }
    return some$(Opt$u32, result);
}
force_inline Opt$u32 u32_chkdDiv(u32 lhs, u32 rhs) {
    if (rhs == 0) { return none$(Opt$u32); }
    return some$(Opt$u32, lhs / rhs);
}
force_inline Opt$u32 u32_chkdMod(u32 lhs, u32 rhs) {
    if (rhs == 0) { return none$(Opt$u32); }
    return some$(Opt$u32, lhs % rhs);
}
// force_inline Opt$u32 u32_chkdInc(u32 x) { return u32_chkdAdd(x, 1); }
// force_inline Opt$u32 u32_chkdDec(u32 x) { return u32_chkdSub(x, 1); }

force_inline Opt$u64 u64_chkdAdd(u64 lhs, u64 rhs) {
    u64 result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$u64); }
    return some$(Opt$u64, result);
}
force_inline Opt$u64 u64_chkdSub(u64 lhs, u64 rhs) {
    u64 result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$u64); }
    return some$(Opt$u64, result);
}
force_inline Opt$u64 u64_chkdMul(u64 lhs, u64 rhs) {
    u64 result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$u64); }
    return some$(Opt$u64, result);
}
force_inline Opt$u64 u64_chkdDiv(u64 lhs, u64 rhs) {
    if (rhs == 0) { return none$(Opt$u64); }
    return some$(Opt$u64, lhs / rhs);
}
force_inline Opt$u64 u64_chkdMod(u64 lhs, u64 rhs) {
    if (rhs == 0) { return none$(Opt$u64); }
    return some$(Opt$u64, lhs % rhs);
}
// force_inline Opt$u64 u64_chkdInc(u64 x) { return u64_chkdAdd(x, 1); }
// force_inline Opt$u64 u64_chkdDec(u64 x) { return u64_chkdSub(x, 1); }

force_inline Opt$usize usize_chkdAdd(usize lhs, usize rhs) {
    usize result = 0;
    if (__builtin_add_overflow(lhs, rhs, &result)) { return none$(Opt$usize); }
    return some$(Opt$usize, result);
}
force_inline Opt$usize usize_chkdSub(usize lhs, usize rhs) {
    usize result = 0;
    if (__builtin_sub_overflow(lhs, rhs, &result)) { return none$(Opt$usize); }
    return some$(Opt$usize, result);
}
force_inline Opt$usize usize_chkdMul(usize lhs, usize rhs) {
    usize result = 0;
    if (__builtin_mul_overflow(lhs, rhs, &result)) { return none$(Opt$usize); }
    return some$(Opt$usize, result);
}
force_inline Opt$usize usize_chkdDiv(usize lhs, usize rhs) {
    if (rhs == 0) { return none$(Opt$usize); }
    return some$(Opt$usize, lhs / rhs);
}
force_inline Opt$usize usize_chkdMod(usize lhs, usize rhs) {
    if (rhs == 0) { return none$(Opt$usize); }
    return some$(Opt$usize, lhs % rhs);
}
// force_inline Opt$usize usize_chkdInc(usize x) { return usize_chkdAdd(x, 1); }
// force_inline Opt$usize usize_chkdDec(usize x) { return usize_chkdSub(x, 1); }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* INT_INCLUDED */
