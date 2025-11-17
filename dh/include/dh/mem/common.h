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
#include "dh/prl.h"

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

$inline_always
$static fn_((mem_lenZ0$u8(const u8* p))(usize));
$inline_always
$static fn_((mem_idxZ$u8(u8 sentinel, const u8* p))(usize));

#define mem_asBytes(_ptr.../*S|S_const(u8)*/)       __step__mem_asBytes(_ptr)
#define mem_bytesAs$(/*(_P_T)_bytes*/.../*(_P_T)*/) __step__mem_bytesAs$(__VA_ARGS__)

#define mem_toBytes$(_A$N$u8, _val...)          __step__mem_toBytes$(pp_uniqTok(val), _A$N$u8, _val)
#define mem_toBytes(_val...)                    __step__mem_toBytes(_val)
#define mem_bytesTo$(/*(_T)_bytes*/.../*(_T)*/) __step__mem_bytesTo$(__VA_ARGS__)

#define mem_sliceAsBytes(_sli.../*(S(u8))*/) __step__mem_sliceAsBytes(__VA_ARGS__)
#define mem_bytesAsSlice$/*(_S_T)_bytes*/.../*(_S_T)*/ __step__mem_bytesAsS$(__VA_ARGS__)

$extern fn_((mem_copyFwds(u_S$raw dst, u_S_const$raw src))(u_S$raw));
#define T_use_mem_copyFwds$(_T...) \
    $inline_always $static fn_((tpl_id(mem_copyFwds, _T)(S$(_T) dst, S$(const _T) src))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_copyFwds(u_anyS(dst), u_anyS_const(src)))); \
    }
$extern fn_((mem_copyBwds(u_S$raw dst, u_S_const$raw src))(u_S$raw));
#define T_use_mem_copyBwds$(_T...) \
    $inline_always $static fn_((tpl_id(mem_copyBwds, _T)(S$(_T) dst, S$(const _T) src))(S$(_T))) { \
        return u_castS$((S$(_T))(mem_copyBwds(u_anyS(dst), u_anyS_const(src)))); \
    }

$extern fn_((mem_eqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));
$extern fn_((mem_nqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool));

$extern fn_((mem_eq(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_eq$(_T...) \
    $inline_always $static fn_((tpl_id(mem_eq, _T)(S$(const _T) lhs, S$(const _T) rhs))(bool)) { \
        return mem_eq(u_anyS_const(lhs), u_anyS_const(rhs)); \
    }
$extern fn_((mem_ne(u_S_const$raw lhs, u_S_const$raw rhs))(bool));
#define T_use_mem_ne$(_T...) \
    $inline_always $static fn_((tpl_id(mem_ne, _T)(S$(const _T) lhs, S$(const _T) rhs))(bool)) { \
        return mem_ne(u_anyS_const(lhs), u_anyS_const(rhs)); \
    }

$extern fn_((mem_startsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool));
#define T_use_mem_startsWith$(_T...) \
    $inline_always $static fn_((tpl_id(mem_startsWith, _T)(S$(const _T) haystack, S$(const _T) needle))(bool)) { \
        return mem_startsWith(u_anyS_const(haystack), u_anyS_const(needle)); \
    }
$extern fn_((mem_endsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool));
#define T_use_mem_endsWith$(_T...) \
    $inline_always $static fn_((tpl_id(mem_endsWith, _T)(S$(const _T) haystack, S$(const _T) needle))(bool)) { \
        return mem_endsWith(u_anyS_const(haystack), u_anyS_const(needle)); \
    }

typedef enum mem_DelimType: u8 {
    mem_delimType_value = 0,
    mem_delimType_pattern = 1,
    mem_delimType_choice = 2,
} mem_DelimType;

#define mem_TokenIter$(_T...) pp_join($, mem_TokenIter, _T)
typedef struct mem_TokenIter {
    S_const$raw buf;
    usize idx;
    mem_DelimType delim_tag;
    union {
        V$raw value;
        S_const$raw pattern;
        S_const$raw choice;
    } delim;
    debug_only(TypeInfo type;)
} mem_TokenIter;
T_use_P$(mem_TokenIter);

#define T_use_mem_TokenIter$(_T...)\
    typedef union mem_TokenIter$(_T) { \
        struct { \
            S$(const _T) buf; \
            usize idx; \
            mem_DelimType delim_tag; \
            union { \
                _T value; \
                S$(const _T) pattern; \
                S$(const _T) choice; \
            } delim; \
            debug_only(TypeInfo type;) \
        }; \
        mem_TokenIter as_raw $like_ref; \
    } mem_TokenIter$(_T)
$extern fn_((mem_tokenizeValue(u_S_const$raw buf, u_V$raw value, P$mem_TokenIter ret_mem))(P$mem_TokenIter));
$extern fn_((mem_tokenizePattern(u_S_const$raw buf, u_S_const$raw pattern, P$mem_TokenIter ret_mem))(P$mem_TokenIter));
$extern fn_((mem_tokenizeChoice(u_S_const$raw buf, u_S_const$raw choice, P$mem_TokenIter ret_mem))(P$mem_TokenIter));
$extern fn_((mem_TokenIter_reset(mem_TokenIter* self))(void));
$extern fn_((mem_TokenIter_next(mem_TokenIter* self, TypeInfo type))(O$u_S_const$raw));
$extern fn_((mem_TokenIter_peek(mem_TokenIter* self, TypeInfo type))(O$u_S_const$raw));
$extern fn_((mem_TokenIter_rest(mem_TokenIter* self, TypeInfo type))(O$u_S_const$raw));

#define T_use_mem_tokenizeValue$(_T...) \
    $inline_always $static fn_((tpl_id(mem_tokenizeValue, _T)(S$(const _T) buf, _T value))(mem_TokenIter$(_T))) { \
        return *as$(mem_TokenIter$(_T)*)(mem_tokenizeValue(u_anyS_const(buf), u_anyV(value), lit$((mem_TokenIter$(_T)){}).as_raw)); \
    }
#define T_use_mem_tokenizePattern$(_T...) \
    $inline_always $static fn_((tpl_id(mem_tokenizePattern, _T)(S$(const _T) buf, S$(const _T) pattern))(mem_TokenIter$(_T))) { \
        return *as$(mem_TokenIter$(_T)*)(mem_tokenizePattern(u_anyS_const(buf), u_anyS_const(pattern), lit$((mem_TokenIter$(_T)){}).as_raw)); \
    }
#define T_use_mem_tokenizeChoice$(_T...) \
    $inline_always $static fn_((tpl_id(mem_tokenizeChoice, _T)(S$(const _T) buf, S$(const _T) choice))(mem_TokenIter$(_T))) { \
        return *as$(mem_TokenIter$(_T)*)(mem_tokenizeChoice(u_anyS_const(buf), u_anyS_const(choice), lit$((mem_TokenIter$(_T)){}).as_raw)); \
    }

#define T_use_mem_TokenIter_reset$(_T...) \
    $inline_always $static fn_((tpl_id(mem_TokenIter_reset, _T)(mem_TokenIter$(_T)* self))(void)) { \
        return mem_TokenIter_reset(self->as_raw); \
    }
#define T_use_mem_TokenIter_next$(_T...) \
    $inline_always $static fn_((tpl_id(mem_TokenIter_next, _T)(mem_TokenIter$(_T)* self))(O$(S$(const _T))) $scope) { \
        return_(u_castO$((ReturnType)(mem_TokenIter_next(self->as_raw, typeInfo$(_T))))); \
    } $unscoped_(fn)
#define T_use_mem_TokenIter_peek$(_T...) \
    $inline_always $static fn_((tpl_id(mem_TokenIter_peek, _T)(mem_TokenIter$(_T)* self))(O$(S$(const _T))) $scope) { \
        return_(u_castO$((ReturnType)(mem_TokenIter_peek(self->as_raw, typeInfo$(_T))))); \
    } $unscoped_(fn)
#define T_use_mem_TokenIter_rest$(_T...) \
    $inline_always $static fn_((tpl_id(mem_TokenIter_rest, _T)(mem_TokenIter$(_T)* self))(O$(S$(const _T))) $scope) { \
        return_(u_castO$((ReturnType)(mem_TokenIter_rest(self->as_raw, typeInfo$(_T))))); \
    } $unscoped_(fn)

// Memory operations
$inline_always P$raw mem_set(P$raw dest, u8 value, usize size);
$inline_always P$raw mem_copy(P$raw dest, P_const$raw src, usize size);
$inline_always P$raw mem_move(P$raw dest, P_const$raw src, usize size);
$inline_always cmp_Ord mem_cmp(P_const$raw lhs, P_const$raw rhs, usize size);

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
$inline_always P$raw mem_copyBytes(P$raw dest, P_const$raw src, usize len);
// Set bytes to value with bounds checking
$inline_always P$raw mem_setBytes(P$raw dest, u8 value, usize len);
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

$inline_always P$raw mem_set(P$raw dest, u8 value, usize size) {
    claim_assert_nonnull(dest);
    return prim_memset(dest, value, size);
}

$inline_always P$raw mem_copy(P$raw dest, P_const$raw src, usize size) {
    claim_assert_nonnull(dest);
    claim_assert_nonnull(src);
    return prim_memcpy(dest, src, size);
}

$inline_always P$raw mem_move(P$raw dest, P_const$raw src, usize size) {
    claim_assert_nonnull(dest);
    claim_assert_nonnull(src);
    return prim_memmove(dest, src, size);
}

$inline_always cmp_Ord mem_cmp(P_const$raw lhs, P_const$raw rhs, usize size) {
    claim_assert_nonnull(lhs);
    claim_assert_nonnull(rhs);
    return as$(cmp_Ord)(prim_memcmp(lhs, rhs, size));
}

$inline_always S_const$u8 mem_spanZ0_const$u8(const u8* p) {
    claim_assert_nonnull(p);
    return (S_const$u8){ .ptr = p, .len = mem_lenZ0$u8(p) };
}

$inline_always S$u8 mem_spanZ0$u8(u8* p) {
    claim_assert_nonnull(p);
    return (S$u8){ .ptr = p, .len = mem_lenZ0$u8(p) };
}

$inline_always usize mem_lenZ0$u8(const u8* p) {
    claim_assert_nonnull(p);
    return mem_idxZ$u8(u8_c('\0'), p);
}

$inline_always usize mem_idxZ$u8(u8 sentinel, const u8* p) {
    claim_assert_nonnull(p);
    usize idx = 0;
    while (p[idx] != sentinel) { idx++; }
    return idx;
}

#define __step__mem_asBytes(_ptr...) _Generic(\
    TypeOf(_ptr),\
    const TypeOfUnqual(*_ptr)*: \
        init$S$((const u8)(as$(const u8*)(_ptr), sizeOf$(TypeOf(*_ptr)))), \
    TypeOfUnqual(*_ptr)*: \
        init$S$((u8)(as$(u8*)(_ptr), sizeOf$(TypeOf(*_ptr)))) \
)
#define __step__mem_bytesAs$(_P_T, _bytes...) _Generic(\
    TypeOf(_bytes.ptr), \
    const u8*: _Generic(\
        _P_T, \
        const TypeOfUnqual(*as$(_P_T)(null))*: as$(_P_T)(_bytes.ptr), \
        TypeOfUnqual(*as$(_P_T)(null))*: claim_unreachable \
    ), \
    u8*: as$(_P_T)(_bytes.ptr) \
)

#define __step__mem_toBytes$(__val, _A$N$u8, _val...) blk({ \
    var __val = _val; \
    blk_return deref$S$((_A$N$u8), mem_asBytes(&__val)); \
})
#define __step__mem_toBytes(_val...) \
    mem_toBytes$(A$$(sizeOf$(TypeOf(_val)), u8), _val)
#define __step__mem_bytesTo$(_T, _bytes...) _Generic(\
    _bytes.ptr, \
    const u8*: *as$(const _T*)(_bytes.ptr), \
    u8*: *as$(_T*)(_bytes.ptr) \
)

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
$inline_always P$raw mem_copyBytes(P$raw dest, P_const$raw src, usize len) {
    claim_assert_nonnull(dest);
    claim_assert_nonnull(src);
    return mem_copy(dest, src, len);
}

// Set bytes to value with bounds checking
$inline_always P$raw mem_setBytes(P$raw dest, u8 value, usize len) {
    claim_assert_nonnull(dest);
    return mem_set(dest, value, len);
}

// Compare two byte buffers
$inline_always bool mem_eqlBytes(P_const$raw lhs, P_const$raw rhs, usize len) {
    claim_assert_nonnull(lhs);
    claim_assert_nonnull(rhs);
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
