/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2024-12-17 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem
 *
 * @brief   Memory management with type-safe operations
 * @details
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
#define mem_page_size (64 * 1024)
#elif defined(__aarch64__)
#define mem_page_size (16 * 1024)
#elif defined(__sparc64__)
#define mem_page_size (8 * 1024)
#else
#define mem_page_size (4 * 1024)
#endif

/*========== Memory Utilities ===============================================*/

// Memory operations
force_inline void mem_set(anyptr dest, u8 value, usize size);
force_inline void mem_copy(anyptr dest, const anyptr src, usize size);
force_inline void mem_move(anyptr dest, anyptr src, usize size);
force_inline i32  mem_cmp(const anyptr lhs, const anyptr rhs, usize size);

/*========== Alignment Functions ============================================*/

// Check if alignment is valid (power of 2)
force_inline bool  mem_isValidAlign(usize align);
// Check if address is aligned
force_inline bool  mem_isAligned(usize addr, usize align);
// Check if address is aligned to power of 2
force_inline bool  mem_isAlignedLog2(usize addr, u8 log2_align);
// Forward align an address
force_inline usize mem_alignForward(usize addr, usize align);
// Forward align an address to power of 2
force_inline usize mem_alignForwardLog2(usize addr, u8 log2_align);
// Backward align an address
force_inline usize mem_alignBackward(usize addr, usize align);

/*========== Buffer Manipulation ============================================*/

// Copy bytes between buffers with bounds checking
force_inline void mem_copyBytes(u8* dest, const u8* src, usize len);
// Set bytes to value with bounds checking
force_inline void mem_setBytes(u8* dest, u8 value, usize len);
// Compare two byte buffers
force_inline bool mem_eqlBytes(const u8* lhs, const u8* rhs, usize len);

/*========== Endian Conversion ==============================================*/

force_inline u16 mem_littleToNative16(u16 x);
force_inline u32 mem_littleToNative32(u32 x);
force_inline u64 mem_littleToNative64(u64 x);

force_inline u16 mem_bigToNative16(u16 x);
force_inline u32 mem_bigToNative32(u32 x);
force_inline u64 mem_bigToNative64(u64 x);

force_inline u16 mem_nativeToLittle16(u16 x);
force_inline u32 mem_nativeToLittle32(u32 x);
force_inline u64 mem_nativeToLittle64(u64 x);

force_inline u16 mem_nativeToBig16(u16 x);
force_inline u32 mem_nativeToBig32(u32 x);
force_inline u64 mem_nativeToBig64(u64 x);

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
force_inline u32 ctz(u64 x) {
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
force_inline u8 mem_alignToLog2(usize align) {
    debug_assert(mem_isValidAlign(align));
    return (u8)ctz(align);
}

/**
 * @brief Convert log2 value to actual alignment
 */
force_inline usize mem_log2ToAlign(u8 log2_align) {
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
force_inline u16 byteSwap16(u16 x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap16(x);
#else
    return (x << 8) | (x >> 8);
#endif
}

force_inline u32 byteSwap32(u32 x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32(x);
#else
    return ((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | ((x & 0x000000FF) << 24);
#endif
}

force_inline u64 byteSwap64(u64 x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap64(x);
#else
    return ((x & 0xFF00000000000000ULL) >> 56) | ((x & 0x00FF000000000000ULL) >> 40) | ((x & 0x0000FF0000000000ULL) >> 24) | ((x & 0x000000FF00000000ULL) >> 8) | ((x & 0x00000000FF000000ULL) << 8) | ((x & 0x0000000000FF0000ULL) << 24) | ((x & 0x000000000000FF00ULL) << 40) | ((x & 0x00000000000000FFULL) << 56);
#endif
}

/*========== Memory Operations ==============================================*/

force_inline void mem_set(anyptr dest, u8 value, usize size) {
    debug_assert_nonnull(dest);
    memset(dest, value, size);
}

force_inline void mem_copy(anyptr dest, const anyptr src, usize size) {
    debug_assert_nonnull(dest);
    debug_assert_nonnull(src);
    memcpy(dest, src, size);
}

force_inline void mem_move(anyptr dest, anyptr src, usize size) {
    debug_assert_nonnull(dest);
    debug_assert_nonnull(src);
    memmove(dest, src, size);
}

force_inline i32 mem_cmp(const anyptr lhs, const anyptr rhs, usize size) {
    debug_assert_nonnull(lhs);
    debug_assert_nonnull(rhs);
    return memcmp(lhs, rhs, size);
}

/*========== Alignment Functions ============================================*/

force_inline bool mem_isValidAlign(usize align) {
    return align > 0 && (align & (align - 1)) == 0;
}

force_inline bool mem_isAligned(usize addr, usize align) {
    debug_assert(mem_isValidAlign(align));
    return (addr & (align - 1)) == 0;
}

force_inline bool mem_isAlignedLog2(usize addr, u8 log2_align) {
    return ctz(addr) >= log2_align;
}

force_inline usize mem_alignForward(usize addr, usize align) {
    debug_assert(mem_isValidAlign(align));
    return (addr + (align - 1)) & ~(align - 1);
}

force_inline usize mem_alignForwardLog2(usize addr, u8 log2_align) {
    const usize alignment = (usize)1 << log2_align;
    return mem_alignForward(addr, alignment);
}

force_inline usize mem_alignBackward(usize addr, usize align) {
    debug_assert(mem_isValidAlign(align));
    return addr & ~(align - 1);
}

/*========== Buffer Manipulation ============================================*/

// Copy bytes between buffers with bounds checking
force_inline void mem_copyBytes(u8* dest, const u8* src, usize len) {
    debug_assert_nonnull(dest);
    debug_assert_nonnull(src);
    mem_copy(dest, src, len);
}

// Set bytes to value with bounds checking
force_inline void mem_setBytes(u8* dest, u8 value, usize len) {
    debug_assert_nonnull(dest);
    mem_set(dest, value, len);
}

// Compare two byte buffers
force_inline bool mem_eqlBytes(const u8* lhs, const u8* rhs, usize len) {
    debug_assert_nonnull(lhs);
    debug_assert_nonnull(rhs);
    return mem_cmp(lhs, rhs, len) == 0;
}

/*========== Endian Conversion ==============================================*/

force_inline u16 mem_littleToNative16(u16 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_LITTLE_ENDIAN
    return x;
#else
    return byteSwap16(x);
#endif
}

force_inline u32 mem_littleToNative32(u32 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_LITTLE_ENDIAN
    return x;
#else
    return byteSwap32(x);
#endif
}

force_inline u64 mem_littleToNative64(u64 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_LITTLE_ENDIAN
    return x;
#else
    return byteSwap64(x);
#endif
}

force_inline u16 mem_bigToNative16(u16 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_BIG_ENDIAN
    return x;
#else
    return byteSwap16(x);
#endif
}

force_inline u32 mem_bigToNative32(u32 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_BIG_ENDIAN
    return x;
#else
    return byteSwap32(x);
#endif
}

force_inline u64 mem_bigToNative64(u64 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_BIG_ENDIAN
    return x;
#else
    return byteSwap64(x);
#endif
}

force_inline u16 mem_nativeToLittle16(u16 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_LITTLE_ENDIAN
    return x;
#else
    return byteSwap16(x);
#endif
}

force_inline u32 mem_nativeToLittle32(u32 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_LITTLE_ENDIAN
    return x;
#else
    return byteSwap32(x);
#endif
}

force_inline u64 mem_nativeToLittle64(u64 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_LITTLE_ENDIAN
    return x;
#else
    return byteSwap64(x);
#endif
}

force_inline u16 mem_nativeToBig16(u16 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_BIG_ENDIAN
    return x;
#else
    return byteSwap16(x);
#endif
}

force_inline u32 mem_nativeToBig32(u32 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_BIG_ENDIAN
    return x;
#else
    return byteSwap32(x);
#endif
}

force_inline u64 mem_nativeToBig64(u64 x) {
#if BUILTIN_PLTF_BYTE_ORDER == BUILTIN_PLTF_BYTE_ORDER_BIG_ENDIAN
    return x;
#else
    return byteSwap64(x);
#endif
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_COMMON_INCLUDED */
