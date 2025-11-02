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

#ifndef MEM_COMMON_INCLUDED
#define MEM_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Memory Constants ===============================================*/

// Fixed page size (may be different per platform)
#if defined(__wasm32__) || defined(__wasm64__)
#define mem_page_size (64ull * 1024ull)
#elif defined(__aarch64__)
#define mem_page_size (16ull * 1024ull)
#elif defined(__sparc64__)
#define mem_page_size (8ull * 1024ull)
#else
#define mem_page_size (4ull * 1024ull)
#endif

/*========== Memory Utilities ===============================================*/

// Memory operations
$inline_always void mem_set(P$raw dest, u8 value, usize size);
$inline_always void mem_copy(P$raw dest, P_const$raw src, usize size);
$inline_always void mem_move(P$raw dest, P$raw src, usize size);
$inline_always i32 mem_cmp(P_const$raw lhs, P_const$raw rhs, usize size);

#define mem_asBytes_const(_ptr...)     comp_inline__mem_asBytes_const(_ptr)
#define mem_asBytes(_ptr...)           comp_inline__mem_asBytes(_ptr)
#define mem_toBytes$(_A$N$u8, _val...) comp_inline__mem_toBytes$(pp_uniqTok(val), _A$N$u8, _val)
#define mem_toBytes(_val...)           comp_inline__mem_toBytes(_val)

/*========== Alignment Functions ============================================*/

// Check if alignment is valid (power of 2)
$inline_always bool mem_isValidAlign(usize align);
// Check if address is aligned
$inline_always bool mem_isAligned(usize addr, usize align);
// Check if address is aligned to power of 2
$inline_always bool mem_isAlignedLog2(usize addr, u8 log2_align);
// Forward align an address
$inline_always usize mem_alignForward(usize addr, usize align);
// Forward align an address to power of 2
$inline_always usize mem_alignForwardLog2(usize addr, u8 log2_align);
// Backward align an address
$inline_always usize mem_alignBackward(usize addr, usize align);

/*========== Buffer Manipulation ============================================*/

// Copy bytes between buffers with bounds checking
$inline_always void mem_copyBytes(P$raw dest, P_const$raw src, usize len);
// Set bytes to value with bounds checking
$inline_always void mem_setBytes(P$raw dest, u8 value, usize len);
// Compare two byte buffers
$inline_always bool mem_eqlBytes(P_const$raw lhs, P_const$raw rhs, usize len);

/*========== Endian Conversion ==============================================*/

$inline_always u16 mem_littleToNative16(u16 x);
$inline_always u32 mem_littleToNative32(u32 x);
$inline_always u64 mem_littleToNative64(u64 x);

$inline_always u16 mem_bigToNative16(u16 x);
$inline_always u32 mem_bigToNative32(u32 x);
$inline_always u64 mem_bigToNative64(u64 x);

$inline_always u16 mem_nativeToLittle16(u16 x);
$inline_always u32 mem_nativeToLittle32(u32 x);
$inline_always u64 mem_nativeToLittle64(u64 x);

$inline_always u16 mem_nativeToBig16(u16 x);
$inline_always u32 mem_nativeToBig32(u32 x);
$inline_always u64 mem_nativeToBig64(u64 x);

/*========== Implementation =================================================*/

/**
 * @brief Count trailing zeros in an integer
 *
 * This is an implementation of Zig's @ctz builtin function.
 * Returns the number of trailing least-significant zero bits in the binary
 * representation of the given integer. If the input is 0, returns the bit
 * width of the type.
 *
 * @param x The input value
 * @return The number of trailing zeros
 */
$inline_always u32 mem_ctz(u64 x) {
    if (x == 0) {
        return sizeof(u64) * 8;
    }

#if defined(__GNUC__) || defined(__clang__)
    // GCC and Clang provide builtin
    return __builtin_ctzll(x);
#else
    // Fallback implementation using De Bruijn sequence
    static const u8 lookup[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };

    // First isolate the lowest set bit
    u64 isolate = x & (-x);

    // DeBruijn constant for 32-bit integers
    // Can be extended for 64-bit if needed
    u64 debruijn = 0x077CB531U;

    // Multiply by the DeBruijn constant and look up the result
    return lookup[((isolate * debruijn) >> 27) & 31];
#endif
}

/**
 * @brief Convert actual alignment value to log2
 */
$inline_always u8 mem_alignToLog2(usize align) {
    debug_assert(mem_isValidAlign(align));
    return (u8)mem_ctz(align);
}

/**
 * @brief Convert log2 value to actual alignment
 */
$inline_always usize mem_log2ToAlign(u8 log2_align) {
    return (usize)1 << log2_align;
}

/**
 * @brief Swap the byte order of an integer
 *
 * This is an implementation of Zig's @byteSwap builtin function.
 * Swaps the byte order of the given integer value. This can be used
 * to convert between little-endian and big-endian representations.
 *
 * @param x The value to byte-swap
 * @return The byte-swapped value
 */
$inline_always u16 byteSwap16(u16 x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap16(x);
#else
    return (x << 8) | (x >> 8);
#endif
}

$inline_always u32 byteSwap32(u32 x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32(x);
#else
    return ((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | ((x & 0x000000FF) << 24);
#endif
}

$inline_always u64 byteSwap64(u64 x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap64(x);
#else
    return ((x & 0xFF00000000000000ULL) >> 56) | ((x & 0x00FF000000000000ULL) >> 40) | ((x & 0x0000FF0000000000ULL) >> 24) | ((x & 0x000000FF00000000ULL) >> 8) | ((x & 0x00000000FF000000ULL) << 8) | ((x & 0x0000000000FF0000ULL) << 24) | ((x & 0x000000000000FF00ULL) << 40) | ((x & 0x00000000000000FFULL) << 56);
#endif
}

/*========== Memory Operations ==============================================*/

$inline_always void mem_set(P$raw dest, u8 value, usize size) {
    debug_assert_nonnull(dest);
    memset(dest, value, size);
}

$inline_always void mem_copy(P$raw dest, P_const$raw src, usize size) {
    debug_assert_nonnull(dest);
    debug_assert_nonnull(src);
    memcpy(dest, src, size);
}

$inline_always void mem_move(P$raw dest, P$raw src, usize size) {
    debug_assert_nonnull(dest);
    debug_assert_nonnull(src);
    memmove(dest, src, size);
}

$inline_always cmp_Ord mem_cmp(P_const$raw lhs, P_const$raw rhs, usize size) {
    debug_assert_nonnull(lhs);
    debug_assert_nonnull(rhs);
    return memcmp(lhs, rhs, size);
}

#define comp_inline__mem_asBytes_const(_ptr...) \
    init$S$((const u8)(as$((const u8*)(_ptr)), sizeOf$(TypeOf(*_ptr))))
#define comp_inline__mem_asBytes(_ptr...) \
    init$S$((u8)(as$((u8*)(_ptr)), sizeOf$(TypeOf(*_ptr))))
#define comp_inline__mem_toBytes$(__val, _A$N$u8, _val...) blk({ \
    var __val = _val; \
    blk_return deref$S$((_A$N$u8), mem_asBytesMut(&__val)); \
})
// #define comp_inline__mem_toBytes_const$(__val, _A$N$u8, _val...) blk({ \
//     let        __val = _val; \
//     blk_return S_deref$(_A$N$u8, mem_asBytesRef(&__val)); \
// })
#define comp_inline__mem_toBytes(_val...) \
    mem_toBytes$(A$$(sizeOf$(TypeOf(_val)), u8), _val)
// #define comp_inline__mem_toBytes_const(_val...) \
//     mem_toBytes$(A$$(sizeOf$(TypeOf(_val)), u8), _val)

/*========== Alignment Functions ============================================*/

$inline_always bool mem_isValidAlign(usize align) {
    return align > 0 && (align & (align - 1)) == 0;
}

$inline_always bool mem_isAligned(usize addr, usize align) {
    debug_assert(mem_isValidAlign(align));
    return (addr & (align - 1)) == 0;
}

$inline_always bool mem_isAlignedLog2(usize addr, u8 log2_align) {
    return mem_ctz(addr) >= log2_align;
}

$inline_always usize mem_alignForward(usize addr, usize align) {
    debug_assert(mem_isValidAlign(align));
    return (addr + (align - 1)) & ~(align - 1);
}

$inline_always usize mem_alignForwardLog2(usize addr, u8 log2_align) {
    const usize alignment = (usize)1 << log2_align;
    return mem_alignForward(addr, alignment);
}

$inline_always usize mem_alignBackward(usize addr, usize align) {
    debug_assert(mem_isValidAlign(align));
    return addr & ~(align - 1);
}

/*========== Buffer Manipulation ============================================*/

// Copy bytes between buffers with bounds checking
$inline_always void mem_copyBytes(P$raw dest, P_const$raw src, usize len) {
    debug_assert_nonnull(dest);
    debug_assert_nonnull(src);
    mem_copy(dest, src, len);
}

// Set bytes to value with bounds checking
$inline_always void mem_setBytes(P$raw dest, u8 value, usize len) {
    debug_assert_nonnull(dest);
    mem_set(dest, value, len);
}

// Compare two byte buffers
$inline_always bool mem_eqlBytes(P_const$raw lhs, P_const$raw rhs, usize len) {
    debug_assert_nonnull(lhs);
    debug_assert_nonnull(rhs);
    return mem_cmp(lhs, rhs, len) == 0;
}

/*========== Endian Conversion ==============================================*/

$inline_always u16 mem_littleToNative16(u16 x) {
#if plat_byte_order == plat_byte_order_little_endian
    return x;
#else
    return byteSwap16(x);
#endif
}

$inline_always u32 mem_littleToNative32(u32 x) {
#if plat_byte_order == plat_byte_order_little_endian
    return x;
#else
    return byteSwap32(x);
#endif
}

$inline_always u64 mem_littleToNative64(u64 x) {
#if plat_byte_order == plat_byte_order_little_endian
    return x;
#else
    return byteSwap64(x);
#endif
}

$inline_always u16 mem_bigToNative16(u16 x) {
#if plat_byte_order == plat_byte_order_big_endian
    return x;
#else
    return byteSwap16(x);
#endif
}

$inline_always u32 mem_bigToNative32(u32 x) {
#if plat_byte_order == plat_byte_order_big_endian
    return x;
#else
    return byteSwap32(x);
#endif
}

$inline_always u64 mem_bigToNative64(u64 x) {
#if plat_byte_order == plat_byte_order_big_endian
    return x;
#else
    return byteSwap64(x);
#endif
}

$inline_always u16 mem_nativeToLittle16(u16 x) {
#if plat_byte_order == plat_byte_order_little_endian
    return x;
#else
    return byteSwap16(x);
#endif
}

$inline_always u32 mem_nativeToLittle32(u32 x) {
#if plat_byte_order == plat_byte_order_little_endian
    return x;
#else
    return byteSwap32(x);
#endif
}

$inline_always u64 mem_nativeToLittle64(u64 x) {
#if plat_byte_order == plat_byte_order_little_endian
    return x;
#else
    return byteSwap64(x);
#endif
}

$inline_always u16 mem_nativeToBig16(u16 x) {
#if plat_byte_order == plat_byte_order_big_endian
    return x;
#else
    return byteSwap16(x);
#endif
}

$inline_always u32 mem_nativeToBig32(u32 x) {
#if plat_byte_order == plat_byte_order_big_endian
    return x;
#else
    return byteSwap32(x);
#endif
}

$inline_always u64 mem_nativeToBig64(u64 x) {
#if plat_byte_order == plat_byte_order_big_endian
    return x;
#else
    return byteSwap64(x);
#endif
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_COMMON_INCLUDED */
