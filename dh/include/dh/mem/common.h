/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2025-01-22 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem
 *
 * @brief   Memory management with type-safe operations
 * @details Provides type-safe memory allocation and management utilities:
 *          - Memory allocation and deallocation
 *          - Memory resizing and remapping
 *          - Memory tracing and debugging
 *          - Memory alignment and padding utilities
 *          - Memory comparison and equality checks
 *          - Memory conversion and type-safe operations
 */

#ifndef mem_common__included
#define mem_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

/* --- Memory Constants --- */

// Fixed page size (may be different per platform)
#define mem_page_size __comp_const__mem_page_size

/* --- Integer Bit Operations --- */

$inline_always
$static fn_((mem_trailingZerosSize(usize x))(u32));
$inline_always
$static fn_((mem_trailingZeros64(u64 x))(u32));
$inline_always
$static fn_((mem_trailingZeros32(u32 x))(u32));
$inline_always
$static fn_((mem_trailingZeros16(u16 x))(u32));
$inline_always
$static fn_((mem_trailingZeros8(u8 x))(u32));

$inline_always
$static fn_((mem_leadingZerosSize(usize x))(u32));
$inline_always
$static fn_((mem_leadingZeros64(u64 x))(u32));
$inline_always
$static fn_((mem_leadingZeros32(u32 x))(u32));
$inline_always
$static fn_((mem_leadingZeros16(u16 x))(u32));
$inline_always
$static fn_((mem_leadingZeros8(u8 x))(u32));

/* --- Byte Swap Functions --- */

$inline_always
$static fn_((mem_byteSwapSize(usize x))(usize));
$inline_always
$static fn_((mem_byteSwap64(u64 x))(u64));
$inline_always
$static fn_((mem_byteSwap32(u32 x))(u32));
$inline_always
$static fn_((mem_byteSwap16(u16 x))(u16));

/* --- Endian Conversion --- */

$inline_always
$static fn_((mem_littleToNativeSize(usize x))(usize));
$inline_always
$static fn_((mem_littleToNative64(u64 x))(u64));
$inline_always
$static fn_((mem_littleToNative32(u32 x))(u32));
$inline_always
$static fn_((mem_littleToNative16(u16 x))(u16));

$inline_always
$static fn_((mem_bigToNativeSize(usize x))(usize));
$inline_always
$static fn_((mem_bigToNative64(u64 x))(u64));
$inline_always
$static fn_((mem_bigToNative32(u32 x))(u32));
$inline_always
$static fn_((mem_bigToNative16(u16 x))(u16));

$inline_always
$static fn_((mem_nativeToLittleSize(usize x))(usize));
$inline_always
$static fn_((mem_nativeToLittle64(u64 x))(u64));
$inline_always
$static fn_((mem_nativeToLittle32(u32 x))(u32));
$inline_always
$static fn_((mem_nativeToLittle16(u16 x))(u16));

$inline_always
$static fn_((mem_nativeToBigSize(usize x))(usize));
$inline_always
$static fn_((mem_nativeToBig64(u64 x))(u64));
$inline_always
$static fn_((mem_nativeToBig32(u32 x))(u32));
$inline_always
$static fn_((mem_nativeToBig16(u16 x))(u16));

/* --- Alignment Functions --- */

typedef u8 mem_Log2Align;
typedef mem_Log2Align mem_Align;

/// Check if alignment is valid (power of 2)
$inline_always
$static fn_((mem_isValidAlign(usize align))(bool));
/// Check if address is aligned
$inline_always
$static fn_((mem_isAligned(usize addr, usize align))(bool));
/// Check if address is aligned to power of 2
$inline_always
$static fn_((mem_isAlignedLog2(usize addr, mem_Align log2_align))(bool));
/// Forward align an address
$inline_always
$static fn_((mem_alignFwd(usize addr, usize align))(usize));
/// Forward align an address to power of 2
$inline_always
$static fn_((mem_alignFwdLog2(usize addr, mem_Align log2_align))(usize));
/// Backward align an address
$inline_always
$static fn_((mem_alignBwd(usize addr, usize align))(usize));
/// Backward align an address to power of 2
$inline_always
$static fn_((mem_alignBwdLog2(usize addr, mem_Align log2_align))(usize));
/// Convert actual alignment value to log2
$inline_always
$static fn_((mem_alignToLog2(usize align))(mem_Align));
/// Convert log2 value to actual alignment
$inline_always
$static fn_((mem_log2ToAlign(mem_Align log2_align))(usize));

/* --- Memory Utilities --- */

$inline_always
$static fn_((mem_idxZ$u8(u8 sentinel, const u8* p))(usize));
$inline_always
$static fn_((mem_lenZ0$u8(const u8* p))(usize));

$inline_always
$static fn_((mem_spanZ0$u8(const u8* p))(S_const$u8));
$inline_always
$static fn_((mem_spanZ0Mut$u8(u8* p))(S$u8));

#define mem_asBytes(_ptr... /*S|S_const(u8)*/) __step__mem_asBytes(_ptr)
#define mem_bytesAs$(/*(_P_T)_bytes*/... /*(_P_T)*/) __step__mem_bytesAs$(__VA_ARGS__)

#define mem_toBytes$(_A$N$u8, _val...) __step__mem_toBytes$(pp_uniqTok(val), _A$N$u8, _val)
#define mem_toBytes(_val...) __step__mem_toBytes(_val)
#define mem_bytesTo$(/*(_T)_bytes*/... /*(_T)*/) __step__mem_bytesTo$(__VA_ARGS__)

#define mem_sliceAsBytes(_sli... /*(S(u8))*/) __step__mem_sliceAsBytes(pp_uniqTok(sli), _sli)
#define mem_bytesAsSlice$(/*(_S_T)_bytes*/... /*(_S_T)*/) __step__mem_bytesAsSlice$(pp_uniqTok(bytes), __VA_ARGS__)

$extern fn_((mem_copyBytes(S$u8 dst, S_const$u8 src))(S$u8));
$extern fn_((mem_copy(u_S$raw dst, u_S_const$raw src))(u_S$raw));
#define T_use_mem_copy$(_T...) \
    $inline_always $static fn_((tpl_id(mem_copy, _T)(S$(_T) dst, S_const$(_T) src))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_copy(u_anyS(dst), u_anyS(src)))); \
    }
$extern fn_((mem_moveBytes(S$u8 dst, S_const$u8 src))(S$u8));
$extern fn_((mem_move(u_S$raw dst, u_S_const$raw src))(u_S$raw));
#define T_use_mem_move$(_T...) \
    $inline_always $static fn_((tpl_id(mem_move, _T)(S$(_T) dst, S_const$(_T) src))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_move(u_anyS(dst), u_anyS(src)))); \
    }
$extern fn_((mem_setBytes(S$u8 dst, u8 val))(S$u8));
$extern fn_((mem_set(u_S$raw dst, u_V$raw val))(u_S$raw));
#define T_use_mem_set$(_T...) \
    $inline_always $static fn_((tpl_id(mem_set, _T)(S$(_T) dst, _T val))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_set(u_anyS(dst), u_anyV(val)))); \
    }
$extern fn_((mem_setBytes0(S$u8 dst))(S$u8));
$extern fn_((mem_set0(u_S$raw dst))(u_S$raw));
#define T_use_mem_set0$(_T...) \
    $inline_always $static fn_((tpl_id(mem_set0, _T)(S$(_T) dst))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_set0(u_anyS(dst)))); \
    }
$extern fn_((mem_eqlBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_eql(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_eql$(_T...) \
    $inline_always $static fn_((tpl_id(mem_eql, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_eql(u_anyS(lhs), u_anyS(rhs)); \
    }
$extern fn_((mem_neqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_neq(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_neq$(_T...) \
    $inline_always $static fn_((tpl_id(mem_neq, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_neq(u_anyS(lhs), u_anyS(rhs)); \
    }
$extern fn_((mem_ordBytes(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord));
$extern fn_((mem_ord(u_S_const$raw lhs, u_S_const$raw rhs))(cmp_Ord));
#define T_use_mem_ord$(_T...) \
    $inline_always $static fn_((tpl_id(mem_ord, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(cmp_Ord)) { \
        return mem_ord(u_anyS(lhs), u_anyS(rhs)); \
    }
$extern fn_((mem_eqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_eq(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_eq$(_T...) \
    $inline_always $static fn_((tpl_id(mem_eq, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_eq(u_anyS(lhs), u_anyS(rhs)); \
    }
$extern fn_((mem_neBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_ne(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_ne$(_T...) \
    $inline_always $static fn_((tpl_id(mem_ne, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_ne(u_anyS(lhs), u_anyS(rhs)); \
    }
$extern fn_((mem_ltBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_lt(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_lt$(_T...) \
    $inline_always $static fn_((tpl_id(mem_lt, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_lt(u_anyS(lhs), u_anyS(rhs)); \
    }
$extern fn_((mem_gtBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_gt(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_gt$(_T...) \
    $inline_always $static fn_((tpl_id(mem_gt, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_gt(u_anyS(lhs), u_anyS(rhs)); \
    }
$extern fn_((mem_leBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_le(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_le$(_T...) \
    $inline_always $static fn_((tpl_id(mem_le, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_le(u_anyS(lhs), u_anyS(rhs)); \
    }
$extern fn_((mem_geBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_ge(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_ge$(_T...) \
    $inline_always $static fn_((tpl_id(mem_ge, _T)(S_const$(_T) lhs, S_const$(_T) rhs))(bool)) { \
        return mem_ge(u_anyS(lhs), u_anyS(rhs)); \
    }

$extern fn_((mem_swapBytes(S$u8 lhs, S$u8 rhs))(void));
$extern fn_((mem_swap(u_S$raw lhs, u_S$raw rhs))(void));
#define T_use_mem_swap$(_T...) \
    $inline_always $static fn_((tpl_id(mem_swap, _T)(S$(_T) lhs, S$(_T) rhs))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_swap(u_anyS(lhs), u_anyS(rhs)))); \
    }
$extern fn_((mem_swapBytesTmp(S$u8 lhs, S$u8 rhs, S$u8 tmp))(S$u8));
$extern fn_((mem_swapTmp(u_S$raw lhs, u_S$raw rhs, u_S$raw tmp))(u_S$raw));
#define T_use_mem_swapTmp$(_T...) \
    $inline_always $static fn_((tpl_id(mem_swapTmp, _T)(S$(_T) lhs, S$(_T) rhs))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_swapTmp(u_anyS(lhs), u_anyS(rhs), u_anyS(tmp)))); \
    }

$extern fn_((mem_startsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool));
#define T_use_mem_startsWith$(_T...) \
    $inline_always $static fn_((tpl_id(mem_startsWith, _T)(S_const$(_T) haystack, S_const$(_T) needle))(bool)) { \
        return mem_startsWith(u_anyS(haystack), u_anyS(needle)); \
    }
$extern fn_((mem_endsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool));
#define T_use_mem_endsWith$(_T...) \
    $inline_always $static fn_((tpl_id(mem_endsWith, _T)(S_const$(_T) haystack, S_const$(_T) needle))(bool)) { \
        return mem_endsWith(u_anyS(haystack), u_anyS(needle)); \
    }

typedef enum_(mem_DelimType $bits(8)) {
    mem_delimType_value = 0,
    mem_delimType_pattern = 1,
    mem_delimType_choice = 2,
} mem_DelimType;

#define mem_TokenIter$(_T...) tpl_id(mem_TokenIter, _T)
typedef struct mem_TokenIter$raw {
    var_(buf, S_const$raw);
    var_(idx, usize);
    var_(delim_type, mem_DelimType);
    union {
        var_(value, V$raw);
        var_(pattern, S_const$raw);
        var_(choice, S_const$raw);
    } delim;
    debug_only(var_(type, TypeInfo);)
} mem_TokenIter$raw;
T_use_P$(mem_TokenIter$raw);
typedef P$mem_TokenIter$raw V$mem_TokenIter$raw;

#define T_use_mem_TokenIter$(_T...) \
    typedef union mem_TokenIter$(_T) { \
        struct { \
            var_(buf, S_const$(_T)); \
            var_(idx, usize); \
            var_(delim_type, mem_DelimType); \
            union { \
                var_(value, _T); \
                var_(pattern, S_const$(_T)); \
                var_(choice, S_const$(_T)); \
            } delim; \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, mem_TokenIter$raw) $like_ref; \
    } mem_TokenIter$(_T)
$extern fn_((mem_tokenizeValue(u_S_const$raw buf, u_V$raw value, V$mem_TokenIter$raw ret_mem))(V$mem_TokenIter$raw));
$extern fn_((mem_tokenizePattern(u_S_const$raw buf, u_S_const$raw pattern, V$mem_TokenIter$raw ret_mem))(V$mem_TokenIter$raw));
$extern fn_((mem_tokenizeChoice(u_S_const$raw buf, u_S_const$raw choice, V$mem_TokenIter$raw ret_mem))(V$mem_TokenIter$raw));
$extern fn_((mem_TokenIter_reset(mem_TokenIter$raw* self))(void));
$extern fn_((mem_TokenIter_next(mem_TokenIter$raw* self, TypeInfo type))(O$u_S_const$raw));
$extern fn_((mem_TokenIter_peek(mem_TokenIter$raw* self, TypeInfo type))(O$u_S_const$raw));
$extern fn_((mem_TokenIter_rest(mem_TokenIter$raw* self, TypeInfo type))(O$u_S_const$raw));
/* clang-format off */
#define T_use_mem_tokenizeValue$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(mem_tokenizeValue, _T)(S_const$(_T) buf, _T value))(mem_TokenIter$(_T))) { \
        return *as$(mem_TokenIter$(_T)*)(mem_tokenizeValue(u_anyS(buf), u_anyV(value), lit0$((mem_TokenIter$(_T))).as_raw)); \
    }
#define T_use_mem_tokenizePattern$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(mem_tokenizePattern, _T)(S_const$(_T) buf, S_const$(_T) pattern))(mem_TokenIter$(_T))) { \
        return *as$(mem_TokenIter$(_T)*)(mem_tokenizePattern(u_anyS(buf), u_anyS(pattern), lit0$((mem_TokenIter$(_T))).as_raw)); \
    }
#define T_use_mem_tokenizeChoice$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(mem_tokenizeChoice, _T)(S_const$(_T) buf, S_const$(_T) choice))(mem_TokenIter$(_T))) { \
        return *as$(mem_TokenIter$(_T)*)(mem_tokenizeChoice(u_anyS(buf), u_anyS(choice), lit0$((mem_TokenIter$(_T))).as_raw)); \
    }
#define T_use_mem_TokenIter_reset$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(mem_TokenIter_reset, _T)($P$(mem_TokenIter$(_T)) self))(void)) { \
        return mem_TokenIter_reset(self->as_raw); \
    }
#define T_use_mem_TokenIter_next$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(mem_TokenIter_next, _T)($P$(mem_TokenIter$(_T)) self))(O$(S_const$(_T)))$scope) { \
        return_(u_castO$((ReturnType)(mem_TokenIter_next(self->as_raw, typeInfo$(_T))))); \
    } $unscoped_(fn)
#define T_use_mem_TokenIter_peek$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(mem_TokenIter_peek, _T)($P$(mem_TokenIter$(_T)) self))(O$(S_const$(_T)))$scope) { \
        return_(u_castO$((ReturnType)(mem_TokenIter_peek(self->as_raw, typeInfo$(_T))))); \
    } $unscoped_(fn)
#define T_use_mem_TokenIter_rest$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl_id(mem_TokenIter_rest, _T)($P$(mem_TokenIter$(_T)) self))(O$(S_const$(_T)))$scope) { \
        return_(u_castO$((ReturnType)(mem_TokenIter_rest(self->as_raw, typeInfo$(_T))))); \
    } $unscoped_(fn)
/* clang-format on */

/*========== Macros and Definitions =========================================*/

/* --- Memory Constants --- */

#define __comp_const__mem_page_size (pp_expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_wasm64)(64ull * 1024ull)), \
        pp_case_((arch_type_wasm32)(64ull * 1024ull)), \
        pp_case_((arch_type_aarch64)(16ull * 1024ull)), \
        pp_default_(4ull * 1024ull) \
    ) pp_end \
))

/* --- Integer Bit Operations --- */

$inline_always
$static fn_((mem_trailingZerosSize(usize x))(u32)) {
    if (x == 0) { return sizeOf$(usize) * 8; }
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_trailingZeros64(x)),
        pp_else_(mem_countTrailingZeros32(x)));
}
$inline_always
$static fn_((mem_trailingZeros64(u64 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_trailingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u64) * 8; }
    // 64-bit De Bruijn Sequence
    static const u8 lookup[64] = {
        0, 1, 56, 2, 57, 49, 28, 3, 61, 58, 42, 50, 38, 29, 17, 4, 62, 47, 59, 36, 45, 43, 51, 22, 53, 39, 33, 30, 24, 18, 12, 5, 63, 55, 48, 27, 60, 41, 37, 16, 46, 35, 44, 21, 52, 32, 23, 11, 54, 26, 40, 15, 34, 20, 31, 10, 25, 14, 19, 9, 13, 8, 7, 6
    };
    const u64 debruijn = 0x03F79D71B4CA8B09ull;
    // (x & -x) isolates the lowest set bit
    return lookup[((x & (0 - x)) * debruijn) >> 58];
#endif
}
$inline_always
$static fn_((mem_trailingZeros32(u32 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_trailingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u32) * 8; }
    // 32-bit De Bruijn Sequence (moved from your 64-bit impl)
    static const u8 lookup[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    const u32 debruijn = 0x077CB531u;
    return lookup[((x & (0 - x)) * debruijn) >> 27];
#endif
}
$inline_always
$static fn_((mem_trailingZeros16(u16 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_trailingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u16) * 8; }
    return mem_countTrailingZeros32(as$(u32)(x));
#endif
}
$inline_always
$static fn_((mem_trailingZeros8(u8 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_trailingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u8) * 8; }
    return mem_countTrailingZeros32(as$(u32)(x));
#endif
}

$inline_always
$static fn_((mem_leadingZerosSize(usize x))(u32)) {
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_leadingZeros64(x)),
        pp_else_(mem_leadingZeros32(x)));
};
$inline_always
$static fn_((mem_leadingZeros64(u64 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_leadingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u64) * 8; }
    // Fallback: Smear bits to right to fill with 1s from MSB
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    // Count ones (population count) is easier,
    // but reusing CTZ logic by isolating MSB is also valid.
    // Here we use a standard 'count set bits' (Hamming weight) for the smeared value.
    // leading_zeros = 64 - popcount(smeared_x)

    // Implementation of popcount64 for fallback (SWAR algorithm)
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return 64 - (u32)((x * 0x0101010101010101ULL) >> 56);
#endif
}
$inline_always
$static fn_((mem_leadingZeros32(u32 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_leadingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u32) * 8; }
    // Fallback: Smear and Popcount
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;

    // Popcount32 (SWAR)
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return 32 - (x & 0x3F);
#endif
}
$inline_always
$static fn_((mem_leadingZeros16(u16 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_leadingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u16) * 8; }
    // __builtin_clz works on 32-bit (usually).
    // clz(0x0000FFFF) is 16. We want 0. So subtract 16.
    // clz(0x00000001) is 31. We want 15. So subtract 16.
    return mem_leadingZeros32(as$(u32)(x)) - 16;
#endif
}
$inline_always
$static fn_((mem_leadingZeros8(u8 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return int_leadingZeros_static(x);
#else
    if (x == 0) { return sizeOf$(u8) * 8; }
    // __builtin_clz works on 32-bit (usually).
    // clz(0x0000FFFF) is 16. We want 0. So subtract 24.
    // clz(0x00000001) is 31. We want 7. So subtract 24.
    return mem_leadingZeros32(as$(u32)(x)) - 24;
#endif
}

/* --- Byte Swap Functions --- */

$inline_always
$static fn_((mem_byteSwapSize(usize x))(usize)) {
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_byteSwap64(x)),
        pp_else_(mem_byteSwap32(x)));
}
$inline_always
$static fn_((mem_byteSwap64(u64 x))(u64)) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap64(x);
#else
    return ((x & 0xFF00000000000000ULL) >> 56)
         | ((x & 0x00FF000000000000ULL) >> 40)
         | ((x & 0x0000FF0000000000ULL) >> 24)
         | ((x & 0x000000FF00000000ULL) >> 8)
         | ((x & 0x00000000FF000000ULL) << 8)
         | ((x & 0x0000000000FF0000ULL) << 24)
         | ((x & 0x000000000000FF00ULL) << 40)
         | ((x & 0x00000000000000FFULL) << 56);
#endif
}
$inline_always
$static fn_((mem_byteSwap32(u32 x))(u32)) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32(x);
#else
    return ((x & 0xFF000000) >> 24)
         | ((x & 0x00FF0000) >> 8)
         | ((x & 0x0000FF00) << 8)
         | ((x & 0x000000FF) << 24);
#endif
}
$inline_always
$static fn_((mem_byteSwap16(u16 x))(u16)) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap16(x);
#else
    return (x << 8) | (x >> 8);
#endif
}

/* --- Endian Conversion --- */

$inline_always
$static fn_((mem_littleToNativeSize(usize x))(usize)) {
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_littleToNative64(x)),
        pp_else_(mem_littleToNative32(x)));
}
$inline_always
$static fn_((mem_littleToNative64(u64 x))(u64)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap64(x)));
}
$inline_always
$static fn_((mem_littleToNative32(u32 x))(u32)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap32(x)));
}
$inline_always
$static fn_((mem_littleToNative16(u16 x))(u16)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap16(x)));
}

$inline_always
$static fn_((mem_bigToNativeSize(usize x))(usize)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwapSize(x)));
}
$inline_always
$static fn_((mem_bigToNative64(u64 x))(u64)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap64(x)));
}
$inline_always
$static fn_((mem_bigToNative32(u32 x))(u32)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap32(x)));
}
$inline_always
$static fn_((mem_bigToNative16(u16 x))(u16)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap16(x)));
}

$inline_always
$static fn_((mem_nativeToLittleSize(usize x))(usize)) {
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_nativeToLittle64(x)),
        pp_else_(mem_nativeToLittle32(x)));
}
$inline_always
$static fn_((mem_nativeToLittle64(u64 x))(u64)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap64(x)));
}
$inline_always
$static fn_((mem_nativeToLittle32(u32 x))(u32)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap32(x)));
}
$inline_always
$static fn_((mem_nativeToLittle16(u16 x))(u16)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap16(x)));
}

$inline_always
$static fn_((mem_nativeToBigSize(usize x))(usize)) {
    return pp_if_(arch_bits_is_64bit)(
        pp_then_(mem_nativeToBig64(x)),
        pp_else_(mem_nativeToBig32(x)));
}
$inline_always
$static fn_((mem_nativeToBig64(u64 x))(u64)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap64(x)));
}
$inline_always
$static fn_((mem_nativeToBig32(u32 x))(u32)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap32(x)));
}

$inline_always
$static fn_((mem_nativeToBig16(u16 x))(u16)) {
    return pp_if_(arch_byte_order_is_big_endian)(
        pp_then_(x),
        pp_else_(mem_byteSwap16(x)));
}

/* --- Alignment Functions --- */

$inline_always
$static fn_((mem_isValidAlign(usize align))(bool)) {
    return 0 < align && (align & (align - 1)) == 0;
}
$inline_always
$static fn_((mem_isAligned(usize addr, usize align))(bool)) {
    return claim_assert(mem_isValidAlign(align)), (addr & (align - 1)) == 0;
}
$inline_always
$static fn_((mem_isAlignedLog2(usize addr, mem_Align log2_align))(bool)) {
    return mem_trailingZerosSize(addr) >= log2_align;
}
$inline_always
$static fn_((mem_alignFwd(usize addr, usize align))(usize)) {
    return claim_assert(mem_isValidAlign(align)), (addr + (align - 1)) & ~(align - 1);
}
$inline_always
$static fn_((mem_alignFwdLog2(usize addr, mem_Align log2_align))(usize)) {
    return mem_alignFwd(addr, mem_log2ToAlign(log2_align));
}
$inline_always
$static fn_((mem_alignBwd(usize addr, usize align))(usize)) {
    return claim_assert(mem_isValidAlign(align)), addr & ~(align - 1);
}
$inline_always
$static fn_((mem_alignBwdLog2(usize addr, mem_Align log2_align))(usize)) {
    return mem_alignBwd(addr, mem_log2ToAlign(log2_align));
}
$inline_always
$static fn_((mem_alignToLog2(usize align))(mem_Align)) {
    return claim_assert(mem_isValidAlign(align)),
           intCast$((mem_Align)(usize_bits - 1u)) - intCast$((mem_Align)(mem_trailingZerosSize(align)));
}
$inline_always
$static fn_((mem_log2ToAlign(mem_Align log2_align))(usize)) {
    return as$(usize)(1) << log2_align;
}

/* --- Memory Utilities --- */

$inline_always
$static fn_((mem_idxZ$u8(u8 sentinel, const u8* p))(usize)) {
    claim_assert_nonnull(p);
    usize idx = 0;
    while (p[idx] != sentinel) { ++idx; }
    return idx;
}
$inline_always
$static fn_((mem_lenZ0$u8(const u8* p))(usize)) {
    claim_assert_nonnull(p);
    return mem_idxZ$u8(u8_c('\0'), p);
}

$inline_always
$static fn_((mem_spanZ0$u8(const u8* p))(S_const$u8)) {
    claim_assert_nonnull(p);
    return (S_const$u8){ .ptr = p, .len = mem_lenZ0$u8(p) };
}
$inline_always
$static fn_((mem_spanZ0Mut$u8(u8* p))(S$u8)) {
    claim_assert_nonnull(p);
    return (S$u8){ .ptr = p, .len = mem_lenZ0$u8(p) };
}

#define __step__mem_asBytes(_ptr...) _Generic( \
    TypeOf(&*_ptr), \
    const TypeOfUnqual(*_ptr)*: init$S$((const u8)(as$(const u8*)(_ptr), sizeOf$(TypeOf(*_ptr)))), \
    TypeOfUnqual(*_ptr)*: init$S$((u8)(as$(u8*)(_ptr), sizeOf$(TypeOf(*_ptr)))) \
)
#define __step__mem_bytesAs$(_P_T, _bytes...) _Generic( \
    TypeOf(&*_bytes.ptr), \
    const u8*: _Generic( \
        _P_T, \
        const TypeOfUnqual(*as$(_P_T)(null))*: as$(_P_T)(_bytes.ptr), \
        TypeOfUnqual(*as$(_P_T)(null))*: claim_unreachable \
    ), \
    u8*: as$(_P_T)(_bytes.ptr) \
)

#define __step__mem_toBytes$(__val, _A$N$u8, _val...) blk({ \
    var __val = _val; \
    blk_return_(deref$S$((_A$N$u8), mem_asBytes(&__val))); \
})
#define __step__mem_toBytes(_val...) \
    mem_toBytes$(A$$(sizeOf$(TypeOf(_val)), u8), _val)
#define __step__mem_bytesTo$(_T, _bytes...) _Generic( \
    &*_bytes.ptr, \
    const u8*: *as$(const _T*)(_bytes.ptr), \
    u8*: *as$(_T*)(_bytes.ptr) \
)

#define __step__mem_sliceAsBytes(__sli, _sli...) blk({ \
    let __sli = &copy(_sli); \
    typedef TypeOfUnqual(*__sli) Sli; \
    _Generic( \
        S_InnerT$(Sli), \
        const S_InnerTUnqual$(Sli): (S_const$u8){ .ptr = as$(const u8*)(__sli->ptr), .len = __sli->len * sizeOf$(S_InnerT$(Sli)) }, \
        S_InnerTUnqual$(Sli): (S$u8){ .ptr = as$(u8*)(__sli->ptr), .len = __sli->len * sizeOf$(S_InnerT$(Sli)) } \
    ); \
})
#define __step__mem_bytesAsSlice$(__bytes, _S_T, _bytes...) blk({ \
    let __bytes = &copy(_bytes); \
    _Generic( \
        TypeOf(&*_bytes.ptr), \
        const u8*: _Generic( \
            S_InnerT$(_S_T), \
            const S_InnerTUnqual$(_S_T): (_S_T){ .ptr = as$(S_InnerT$(_S_T)*)(__bytes->ptr), .len = __bytes->len / sizeOf$(S_InnerT$(_S_T)) }, \
            S_InnerTUnqual$(_S_T): claim_unreachable \
        ), \
        u8*: (_S_T){ .ptr = as$(S_InnerT$(_S_T)*)(__bytes->ptr), .len = __bytes->len / sizeOf$(S_InnerT$(_S_T)) } \
    ); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_common__included */
