/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    mem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-12 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  (none)
 */
#pragma once
#ifndef builtin_mem__included
#define builtin_mem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp.h"

/*========== Macros and Declarations ========================================*/

#define mem_bits_per_bit __comp_int__mem_bits_per_bit
#define mem_bits_per_octet __comp_int__mem_bits_per_octet
#define mem_bits_per_hextet __comp_int__mem_bits_per_hextet
#define mem_bits_per_quadlet __comp_int__mem_bits_per_quadlet
#define mem_bits_per_octlet __comp_int__mem_bits_per_octlet
#define mem_bits_per_hexlet __comp_int__mem_bits_per_hexlet

#define mem_bits_per_word __comp_int__mem_bits_per_word
#define mem_bits_per_byte __comp_int__mem_bits_per_byte
#define mem_bits_per_nibble __comp_int__mem_bits_per_nibble
#define mem_bits_per_crumb __comp_int__mem_bits_per_crumb

#define raw_countOnesSize(_$x...) ____raw_countOnesSize(_$x)
#define raw_countOnes64(_$x...) ____raw_countOnes64(_$x)
#define raw_countOnesLong(_$x...) ____raw_countOnesLong(_$x)
#define raw_countOnes32(_$x...) ____raw_countOnes32(_$x)
#define raw_countOnes16(_$x...) ____raw_countOnes16(_$x)
#define raw_countOnes8(_$x...) ____raw_countOnes8(_$x)

#define raw_leadingRedundantSgnBitsSize(_$x...) ____raw_leadingRedundantSgnBitsSize(_$x)
#define raw_leadingRedundantSgnBits64(_$x...) ____raw_leadingRedundantSgnBits64(_$x)
#define raw_leadingRedundantSgnBitsLong(_$x...) ____raw_leadingRedundantSgnBitsLong(_$x)
#define raw_leadingRedundantSgnBits32(_$x...) ____raw_leadingRedundantSgnBits32(_$x)
#define raw_leadingRedundantSgnBits16(_$x...) ____raw_leadingRedundantSgnBits16(_$x)
#define raw_leadingRedundantSgnBits8(_$x...) ____raw_leadingRedundantSgnBits8(_$x)

#define raw_leadingZerosSize(_$x...) ____raw_leadingZerosSize(_$x)
#define raw_leadingZeros64(_$x...) ____raw_leadingZeros64(_$x)
#define raw_leadingZerosLong(_$x...) ____raw_leadingZerosLong(_$x)
#define raw_leadingZeros32(_$x...) ____raw_leadingZeros32(_$x)
#define raw_leadingZeros16(_$x...) ____raw_leadingZeros16(_$x)
#define raw_leadingZeros8(_$x...) ____raw_leadingZeros8(_$x)

#define raw_trailingZerosSize(_$x...) ____raw_trailingZerosSize(_$x)
#define raw_trailingZeros64(_$x...) ____raw_trailingZeros64(_$x)
#define raw_trailingZerosLong(_$x...) ____raw_trailingZerosLong(_$x)
#define raw_trailingZeros32(_$x...) ____raw_trailingZeros32(_$x)
#define raw_trailingZeros16(_$x...) ____raw_trailingZeros16(_$x)
#define raw_trailingZeros8(_$x...) ____raw_trailingZeros8(_$x)

#define raw_firstSetBitSize(_$x...) ____raw_firstSetBitSize(_$x)
#define raw_firstSetBit64(_$x...) ____raw_firstSetBit64(_$x)
#define raw_firstSetBitLong(_$x...) ____raw_firstSetBitLong(_$x)
#define raw_firstSetBit32(_$x...) ____raw_firstSetBit32(_$x)
#define raw_firstSetBit16(_$x...) ____raw_firstSetBit16(_$x)
#define raw_firstSetBit8(_$x...) ____raw_firstSetBit8(_$x)

#define raw_paritySize(_$x...) ____raw_paritySize(_$x)
#define raw_parity64(_$x...) ____raw_parity64(_$x)
#define raw_parityLong(_$x...) ____raw_parityLong(_$x)
#define raw_parity32(_$x...) ____raw_parity32(_$x)
#define raw_parity16(_$x...) ____raw_parity16(_$x)
#define raw_parity8(_$x...) ____raw_parity8(_$x)

#define raw_swapBytesSize(_$x...) ____raw_swapBytesSize(_$x)
#define raw_swapBytes64(_$x...) ____raw_swapBytes64(_$x)
#define raw_swapBytesLong(_$x...) ____raw_swapBytesLong(_$x)
#define raw_swapBytes32(_$x...) ____raw_swapBytes32(_$x)
#define raw_swapBytes16(_$x...) ____raw_swapBytes16(_$x)

#define raw_rotateLeftSize(_$x, _$y...) ____raw_rotateLeftSize(_$x, _$y)
#define raw_rotateLeft64(_$x, _$y...) ____raw_rotateLeft64(_$x, _$y)
#define raw_rotateLeftLong(_$x, _$y...) ____raw_rotateLeftLong(_$x, _$y)
#define raw_rotateLeft32(_$x, _$y...) ____raw_rotateLeft32(_$x, _$y)
#define raw_rotateLeft16(_$x, _$y...) ____raw_rotateLeft16(_$x, _$y)
#define raw_rotateLeft8(_$x, _$y...) ____raw_rotateLeft8(_$x, _$y)

#define raw_rotateRightSize(_$x, _$y...) ____raw_rotateRightSize(_$x, _$y)
#define raw_rotateRight64(_$x, _$y...) ____raw_rotateRight64(_$x, _$y)
#define raw_rotateRightLong(_$x, _$y...) ____raw_rotateRightLong(_$x, _$y)
#define raw_rotateRight32(_$x, _$y...) ____raw_rotateRight32(_$x, _$y)
#define raw_rotateRight16(_$x, _$y...) ____raw_rotateRight16(_$x, _$y)
#define raw_rotateRight8(_$x, _$y...) ____raw_rotateRight8(_$x, _$y)

#define raw_reverseBitsSize(_$x...) ____raw_reverseBitsSize(_$x)
#define raw_reverseBits64(_$x...) ____raw_reverseBits64(_$x)
#define raw_reverseBitsLong(_$x...) ____raw_reverseBitsLong(_$x)
#define raw_reverseBits32(_$x...) ____raw_reverseBits32(_$x)
#define raw_reverseBits16(_$x...) ____raw_reverseBits16(_$x)
#define raw_reverseBits8(_$x...) ____raw_reverseBits8(_$x)

#undef memset
#undef memcpy
#undef memmove
#undef memcmp
#undef alloca

#define raw_memset0(_$p_dst, _$len...) ____raw_memset0(_$p_dst, _$len)
#define raw_memset(_$p_dst, _$b_src, _$len...) ____raw_memset(_$p_dst, _$b_src, _$len)
#define raw_memcpy(_$p_dst, _$p_src, _$len...) ____raw_memcpy(_$p_dst, _$p_src, _$len)
#define raw_memmove(_$p_dst, _$p_src, _$len...) ____raw_memmove(_$p_dst, _$p_src, _$len)
#define raw_memeql(_$p_lhs, _$p_rhs, _$len...) ____raw_memeql(_$p_lhs, _$p_rhs, _$len)
#define raw_memord(_$p_lhs, _$p_rhs, _$len...) __step__raw_memord(_$p_lhs, _$p_rhs, _$len)
#define raw_alloca(_$len_bytes...) ____raw_alloca(_$len_bytes)
#define raw_allocaAlign(_$len_bytes, _$log2_align...) ____raw_allocaAlign(_$len_bytes, _$log2_align)

/*========== Macros and Definitions =========================================*/

#define __comp_int__mem_bits_per_bit 1
#define __comp_int__mem_bits_per_octet 8
#define __comp_int__mem_bits_per_hextet 16
#define __comp_int__mem_bits_per_quadlet 32
#define __comp_int__mem_bits_per_octlet 64
#define __comp_int__mem_bits_per_hexlet 128

#define __comp_int__mem_bits_per_word arch_bits_per_word
#define __comp_int__mem_bits_per_byte arch_bits_per_byte
#define __comp_int__mem_bits_per_nibble arch_bits_per_nibble
#define __comp_int__mem_bits_per_crumb arch_bits_per_crumb

#define __comp_mem_sizeSelect(_$on16, _$on32, _$on64) pp_expand( \
    pp_switch_ pp_begin(abi_size_unit)( \
        pp_case_((abi_bits_unit_16bit)(_$on16)), \
        pp_case_((abi_bits_unit_32bit)(_$on32)), \
        pp_case_((abi_bits_unit_64bit)(_$on64)) \
    ) pp_end \
)

#define ____raw_countOnesSize(_$x...) \
    __comp_mem_sizeSelect(raw_countOnes16(_$x), raw_countOnes32(_$x), raw_countOnes64(_$x))
#define ____raw_countOnes64(_$x...) (as$(u32)(__builtin_popcountll(as$(u64)(_$x))))
#define ____raw_countOnesLong(_$x...) (as$(u32)(__builtin_popcountl(as$(ulong)(_$x))))
#if abi_int_bits >= 32
#define ____raw_countOnes32(_$x...) (as$(u32)(__builtin_popcount(as$(u32)(_$x))))
#else
#define ____raw_countOnes32(_$x...) (as$(u32)(__builtin_popcountl(as$(ulong)(as$(u32)(_$x)))))
#endif
#define ____raw_countOnes16(_$x...) (as$(u32)(__builtin_popcount(as$(u16)(_$x))))
#define ____raw_countOnes8(_$x...) (as$(u32)(__builtin_popcount(as$(u8)(_$x))))

#define ____raw_leadingRedundantSgnBitsSize(_$x...) \
    __comp_mem_sizeSelect( \
        raw_leadingRedundantSgnBits16(_$x), \
        raw_leadingRedundantSgnBits32(_$x), \
        raw_leadingRedundantSgnBits64(_$x) \
    )
#define ____raw_leadingRedundantSgnBits64(_$x...) (as$(u32)(__builtin_clrsbll(as$(i64)(_$x))))
#define ____raw_leadingRedundantSgnBitsLong(_$x...) (as$(u32)(__builtin_clrsbl(as$(ilong)(_$x))))
#if abi_int_bits >= 32
#define ____raw_leadingRedundantSgnBits32(_$x...) ( \
    as$(u32)(__builtin_clrsb(as$(i32)(_$x))) - (u32_(abi_int_bits) - u32_(32)) \
)
#else
#define ____raw_leadingRedundantSgnBits32(_$x...) ( \
    as$(u32)(__builtin_clrsbl(as$(ilong)(as$(i32)(_$x)))) - (u32_(abi_long_bits) - u32_(32)) \
)
#endif
#define ____raw_leadingRedundantSgnBits16(_$x...) ( \
    as$(u32)(__builtin_clrsb(as$(i16)(_$x))) - (u32_(abi_int_bits) - u32_(16)) \
)
#define ____raw_leadingRedundantSgnBits8(_$x...) ( \
    as$(u32)(__builtin_clrsb(as$(i8)(_$x))) - (u32_(abi_int_bits) - u32_(8)) \
)

#define ____raw_leadingZerosSize(_$x...) \
    __comp_mem_sizeSelect(raw_leadingZeros16(_$x), raw_leadingZeros32(_$x), raw_leadingZeros64(_$x))
#define ____raw_leadingZeros64(_$x...) (as$(u32)(__builtin_clzll(as$(u64)(_$x))))
#define ____raw_leadingZerosLong(_$x...) (as$(u32)(__builtin_clzl(as$(ulong)(_$x))))
#if abi_int_bits >= 32
#define ____raw_leadingZeros32(_$x...) ( \
    as$(u32)(__builtin_clz(as$(u32)(_$x))) - (u32_(abi_int_bits) - u32_(32)) \
)
#else
#define ____raw_leadingZeros32(_$x...) ( \
    as$(u32)(__builtin_clzl(as$(ulong)(as$(u32)(_$x)))) - (u32_(abi_long_bits) - u32_(32)) \
)
#endif
#define ____raw_leadingZeros16(_$x...) ( \
    as$(u32)(__builtin_clz(as$(u16)(_$x))) - (u32_(abi_int_bits) - u32_(16)) \
)
#define ____raw_leadingZeros8(_$x...) ( \
    as$(u32)(__builtin_clz(as$(u8)(_$x))) - (u32_(abi_int_bits) - u32_(8)) \
)

#define ____raw_trailingZerosSize(_$x...) \
    __comp_mem_sizeSelect(raw_trailingZeros16(_$x), raw_trailingZeros32(_$x), raw_trailingZeros64(_$x))
#define ____raw_trailingZeros64(_$x...) (as$(u32)(__builtin_ctzll(as$(u64)(_$x))))
#define ____raw_trailingZerosLong(_$x...) (as$(u32)(__builtin_ctzl(as$(ulong)(_$x))))
#if abi_int_bits >= 32
#define ____raw_trailingZeros32(_$x...) (as$(u32)(__builtin_ctz(as$(u32)(_$x))))
#else
#define ____raw_trailingZeros32(_$x...) (as$(u32)(__builtin_ctzl(as$(ulong)(as$(u32)(_$x)))))
#endif
#define ____raw_trailingZeros16(_$x...) (as$(u32)(__builtin_ctz(as$(u16)(_$x))))
#define ____raw_trailingZeros8(_$x...) (as$(u32)(__builtin_ctz(as$(u8)(_$x))))

#define ____raw_firstSetBitSize(_$x...) \
    __comp_mem_sizeSelect(raw_firstSetBit16(_$x), raw_firstSetBit32(_$x), raw_firstSetBit64(_$x))
#define ____raw_firstSetBit64(_$x...) (as$(u32)(__builtin_ffsll(as$(i64)(_$x))))
#define ____raw_firstSetBitLong(_$x...) (as$(u32)(__builtin_ffsl(as$(ilong)(_$x))))
#if abi_int_bits >= 32
#define ____raw_firstSetBit32(_$x...) (as$(u32)(__builtin_ffs(as$(i32)(_$x))))
#else
#define ____raw_firstSetBit32(_$x...) (as$(u32)(__builtin_ffsl(as$(ilong)(as$(i32)(_$x)))))
#endif
#define ____raw_firstSetBit16(_$x...) (as$(u32)(__builtin_ffs(as$(i16)(_$x))))
#define ____raw_firstSetBit8(_$x...) (as$(u32)(__builtin_ffs(as$(i8)(_$x))))

#define ____raw_paritySize(_$x...) \
    __comp_mem_sizeSelect(raw_parity16(_$x), raw_parity32(_$x), raw_parity64(_$x))
#define ____raw_parity64(_$x...) (as$(u32)(__builtin_parityll(as$(u64)(_$x))))
#define ____raw_parityLong(_$x...) (as$(u32)(__builtin_parityl(as$(ulong)(_$x))))
#if abi_int_bits >= 32
#define ____raw_parity32(_$x...) (as$(u32)(__builtin_parity(as$(u32)(_$x))))
#else
#define ____raw_parity32(_$x...) (as$(u32)(__builtin_parityl(as$(ulong)(as$(u32)(_$x)))))
#endif
#define ____raw_parity16(_$x...) (as$(u32)(__builtin_parity(as$(u16)(_$x))))
#define ____raw_parity8(_$x...) (as$(u32)(__builtin_parity(as$(u8)(_$x))))

#define ____raw_swapBytesSize(_$x...) \
    __comp_mem_sizeSelect(raw_swapBytes16(_$x), raw_swapBytes32(_$x), raw_swapBytes64(_$x))
#define ____raw_swapBytes64(_$x...) (as$(u64)(__builtin_bswap64(as$(u64)(_$x))))
#define ____raw_swapBytesLong(_$x...) pp_if_(abi_long_is_64bit)( \
    pp_then_(raw_swapBytes64(_$x)), \
    pp_else_(raw_swapBytes32(_$x)) \
)
#define ____raw_swapBytes32(_$x...) (as$(u32)(__builtin_bswap32(as$(u32)(_$x))))
#define ____raw_swapBytes16(_$x...) (as$(u16)(__builtin_bswap16(as$(u16)(_$x))))

/* GCC does not provide Clang's bitreverse/rotate builtin names.
 * Keep the public raw_* surface unchanged and provide constant-expression
 * fallbacks for constant operands, with a single-evaluation runtime path. */
#define __comp_raw_rotl8_const(_$x, _$n) \
    (as$(u8)((as$(u8)(_$x) << (as$(u32)(_$n) & 7u)) | (as$(u8)(_$x) >> ((0u - as$(u32)(_$n)) & 7u))))
#define __comp_raw_rotl16_const(_$x, _$n) \
    (as$(u16)((as$(u16)(_$x) << (as$(u32)(_$n) & 15u)) | (as$(u16)(_$x) >> ((0u - as$(u32)(_$n)) & 15u))))
#define __comp_raw_rotl32_const(_$x, _$n) \
    (as$(u32)((as$(u32)(_$x) << (as$(u32)(_$n) & 31u)) | (as$(u32)(_$x) >> ((0u - as$(u32)(_$n)) & 31u))))
#define __comp_raw_rotl64_const(_$x, _$n) \
    (as$(u64)((as$(u64)(_$x) << (as$(u64)(_$n) & 63ull)) | (as$(u64)(_$x) >> ((0ull - as$(u64)(_$n)) & 63ull))))

#define __comp_raw_rotr8_const(_$x, _$n) __comp_raw_rotl8_const((_$x), (0u - as$(u32)(_$n)))
#define __comp_raw_rotr16_const(_$x, _$n) __comp_raw_rotl16_const((_$x), (0u - as$(u32)(_$n)))
#define __comp_raw_rotr32_const(_$x, _$n) __comp_raw_rotl32_const((_$x), (0u - as$(u32)(_$n)))
#define __comp_raw_rotr64_const(_$x, _$n) __comp_raw_rotl64_const((_$x), (0ull - as$(u64)(_$n)))

#define __comp_raw_brev8_const(_$x) \
    (as$(u8)((((as$(u8)(_$x) & 0x55u) << 1) | ((as$(u8)(_$x) >> 1) & 0x55u))))
#define __comp_raw_brev8_const2(_$x) \
    (as$(u8)((((__comp_raw_brev8_const(_$x) & 0x33u) << 2) | ((__comp_raw_brev8_const(_$x) >> 2) & 0x33u))))
#define __comp_raw_brev8(_$x) \
    (as$(u8)((((__comp_raw_brev8_const2(_$x) & 0x0fu) << 4) | ((__comp_raw_brev8_const2(_$x) >> 4) & 0x0fu))))
#define __comp_raw_brev16_const(_$x) \
    (as$(u16)((as$(u16)(__comp_raw_brev8(as$(u8)(_$x))) << 8) | as$(u16)(__comp_raw_brev8(as$(u8)(as$(u16)(_$x) >> 8)))))
#define __comp_raw_brev32_const(_$x) \
    (as$(u32)((as$(u32)(__comp_raw_brev16_const(as$(u16)(_$x))) << 16) | as$(u32)(__comp_raw_brev16_const(as$(u16)(as$(u32)(_$x) >> 16)))))
#define __comp_raw_brev64_const(_$x) \
    (as$(u64)((as$(u64)(__comp_raw_brev32_const(as$(u32)(_$x))) << 32) | as$(u64)(__comp_raw_brev32_const(as$(u32)(as$(u64)(_$x) >> 32)))))

static inline __UINT8_TYPE__ __comp_raw_rotl8_runtime(__UINT8_TYPE__ x, __UINT32_TYPE__ n) {
    return (__UINT8_TYPE__)((__UINT8_TYPE__)(x << (n & 7u)) | (__UINT8_TYPE__)(x >> ((0u - n) & 7u)));
}
static inline __UINT16_TYPE__ __comp_raw_rotl16_runtime(__UINT16_TYPE__ x, __UINT32_TYPE__ n) {
    return (__UINT16_TYPE__)((__UINT16_TYPE__)(x << (n & 15u)) | (__UINT16_TYPE__)(x >> ((0u - n) & 15u)));
}
static inline __UINT32_TYPE__ __comp_raw_rotl32_runtime(__UINT32_TYPE__ x, __UINT32_TYPE__ n) {
    return (__UINT32_TYPE__)((__UINT32_TYPE__)(x << (n & 31u)) | (__UINT32_TYPE__)(x >> ((0u - n) & 31u)));
}
static inline __UINT64_TYPE__ __comp_raw_rotl64_runtime(__UINT64_TYPE__ x, __UINT64_TYPE__ n) {
    return (__UINT64_TYPE__)((__UINT64_TYPE__)(x << (n & 63ull)) | (__UINT64_TYPE__)(x >> ((0ull - n) & 63ull)));
}
static inline __UINT8_TYPE__ __comp_raw_rotr8_runtime(__UINT8_TYPE__ x, __UINT32_TYPE__ n) {
    return __comp_raw_rotl8_runtime(x, 0u - n);
}
static inline __UINT16_TYPE__ __comp_raw_rotr16_runtime(__UINT16_TYPE__ x, __UINT32_TYPE__ n) {
    return __comp_raw_rotl16_runtime(x, 0u - n);
}
static inline __UINT32_TYPE__ __comp_raw_rotr32_runtime(__UINT32_TYPE__ x, __UINT32_TYPE__ n) {
    return __comp_raw_rotl32_runtime(x, 0u - n);
}
static inline __UINT64_TYPE__ __comp_raw_rotr64_runtime(__UINT64_TYPE__ x, __UINT64_TYPE__ n) {
    return __comp_raw_rotl64_runtime(x, 0ull - n);
}
static inline __UINT8_TYPE__ __comp_raw_brev8_runtime(__UINT8_TYPE__ x) {
    x = (__UINT8_TYPE__)(((x & 0x55u) << 1) | ((x >> 1) & 0x55u));
    x = (__UINT8_TYPE__)(((x & 0x33u) << 2) | ((x >> 2) & 0x33u));
    return (__UINT8_TYPE__)(((x & 0x0fu) << 4) | ((x >> 4) & 0x0fu));
}
static inline __UINT16_TYPE__ __comp_raw_brev16_runtime(__UINT16_TYPE__ x) {
    return (__UINT16_TYPE__)(((__UINT16_TYPE__)__comp_raw_brev8_runtime((__UINT8_TYPE__)x) << 8)
                             | (__UINT16_TYPE__)__comp_raw_brev8_runtime((__UINT8_TYPE__)(x >> 8)));
}
static inline __UINT32_TYPE__ __comp_raw_brev32_runtime(__UINT32_TYPE__ x) {
    return ((__UINT32_TYPE__)__comp_raw_brev16_runtime((__UINT16_TYPE__)x) << 16)
         | (__UINT32_TYPE__)__comp_raw_brev16_runtime((__UINT16_TYPE__)(x >> 16));
}
static inline __UINT64_TYPE__ __comp_raw_brev64_runtime(__UINT64_TYPE__ x) {
    return ((__UINT64_TYPE__)__comp_raw_brev32_runtime((__UINT32_TYPE__)x) << 32)
         | (__UINT64_TYPE__)__comp_raw_brev32_runtime((__UINT32_TYPE__)(x >> 32));
}

#define ____raw_rotateLeftSize(_$x, _$y...) \
    __comp_mem_sizeSelect( \
        raw_rotateLeft16(_$x, _$y), \
        raw_rotateLeft32(_$x, _$y), \
        raw_rotateLeft64(_$x, _$y) \
    )
#if comp_type == comp_type_clang
#define ____raw_rotateLeft64(_$x, _$y...) (as$(u64)(__builtin_rotateleft64(as$(u64)(_$x), as$(u64)(_$y))))
#else
#define ____raw_rotateLeft64(_$x, _$y...) __builtin_choose_expr( \
    __builtin_constant_p(_$x) && __builtin_constant_p(_$y), \
    __comp_raw_rotl64_const(_$x, _$y), \
    __comp_raw_rotl64_runtime(as$(u64)(_$x), as$(u64)(_$y)) \
)
#endif
#define ____raw_rotateLeftLong(_$x, _$y...) pp_if_(abi_long_is_64bit)( \
    pp_then_(raw_rotateLeft64(_$x, _$y)), \
    pp_else_(raw_rotateLeft32(_$x, _$y)) \
)
#if comp_type == comp_type_clang
#define ____raw_rotateLeft32(_$x, _$y...) (as$(u32)(__builtin_rotateleft32(as$(u32)(_$x), as$(u32)(_$y))))
#else
#define ____raw_rotateLeft32(_$x, _$y...) __builtin_choose_expr( \
    __builtin_constant_p(_$x) && __builtin_constant_p(_$y), \
    __comp_raw_rotl32_const(_$x, _$y), \
    __comp_raw_rotl32_runtime(as$(u32)(_$x), as$(u32)(_$y)) \
)
#endif
#if comp_type == comp_type_clang
#define ____raw_rotateLeft16(_$x, _$y...) (as$(u16)(__builtin_rotateleft16(as$(u16)(_$x), as$(u16)(_$y))))
#else
#define ____raw_rotateLeft16(_$x, _$y...) __builtin_choose_expr( \
    __builtin_constant_p(_$x) && __builtin_constant_p(_$y), \
    __comp_raw_rotl16_const(_$x, _$y), \
    __comp_raw_rotl16_runtime(as$(u16)(_$x), as$(u32)(_$y)) \
)
#endif
#if comp_type == comp_type_clang
#define ____raw_rotateLeft8(_$x, _$y...) (as$(u8)(__builtin_rotateleft8(as$(u8)(_$x), as$(u8)(_$y))))
#else
#define ____raw_rotateLeft8(_$x, _$y...) __builtin_choose_expr( \
    __builtin_constant_p(_$x) && __builtin_constant_p(_$y), \
    __comp_raw_rotl8_const(_$x, _$y), \
    __comp_raw_rotl8_runtime(as$(u8)(_$x), as$(u32)(_$y)) \
)
#endif

#define ____raw_rotateRightSize(_$x, _$y...) \
    __comp_mem_sizeSelect( \
        raw_rotateRight16(_$x, _$y), \
        raw_rotateRight32(_$x, _$y), \
        raw_rotateRight64(_$x, _$y) \
    )
#if comp_type == comp_type_clang
#define ____raw_rotateRight64(_$x, _$y...) (as$(u64)(__builtin_rotateright64(as$(u64)(_$x), as$(u64)(_$y))))
#else
#define ____raw_rotateRight64(_$x, _$y...) __builtin_choose_expr( \
    __builtin_constant_p(_$x) && __builtin_constant_p(_$y), \
    __comp_raw_rotr64_const(_$x, _$y), \
    __comp_raw_rotr64_runtime(as$(u64)(_$x), as$(u64)(_$y)) \
)
#endif
#define ____raw_rotateRightLong(_$x, _$y...) pp_if_(abi_long_is_64bit)( \
    pp_then_(raw_rotateRight64(_$x, _$y)), \
    pp_else_(raw_rotateRight32(_$x, _$y)) \
)
#if comp_type == comp_type_clang
#define ____raw_rotateRight32(_$x, _$y...) (as$(u32)(__builtin_rotateright32(as$(u32)(_$x), as$(u32)(_$y))))
#else
#define ____raw_rotateRight32(_$x, _$y...) __builtin_choose_expr( \
    __builtin_constant_p(_$x) && __builtin_constant_p(_$y), \
    __comp_raw_rotr32_const(_$x, _$y), \
    __comp_raw_rotr32_runtime(as$(u32)(_$x), as$(u32)(_$y)) \
)
#endif
#if comp_type == comp_type_clang
#define ____raw_rotateRight16(_$x, _$y...) (as$(u16)(__builtin_rotateright16(as$(u16)(_$x), as$(u16)(_$y))))
#else
#define ____raw_rotateRight16(_$x, _$y...) __builtin_choose_expr( \
    __builtin_constant_p(_$x) && __builtin_constant_p(_$y), \
    __comp_raw_rotr16_const(_$x, _$y), \
    __comp_raw_rotr16_runtime(as$(u16)(_$x), as$(u32)(_$y)) \
)
#endif
#if comp_type == comp_type_clang
#define ____raw_rotateRight8(_$x, _$y...) (as$(u8)(__builtin_rotateright8(as$(u8)(_$x), as$(u8)(_$y))))
#else
#define ____raw_rotateRight8(_$x, _$y...) __builtin_choose_expr( \
    __builtin_constant_p(_$x) && __builtin_constant_p(_$y), \
    __comp_raw_rotr8_const(_$x, _$y), \
    __comp_raw_rotr8_runtime(as$(u8)(_$x), as$(u32)(_$y)) \
)
#endif

#define ____raw_reverseBitsSize(_$x...) \
    __comp_mem_sizeSelect(raw_reverseBits16(_$x), raw_reverseBits32(_$x), raw_reverseBits64(_$x))
#if comp_type == comp_type_clang
#define ____raw_reverseBits64(_$x...) (as$(u64)(__builtin_bitreverse64(as$(u64)(_$x))))
#else
#define ____raw_reverseBits64(_$x...) __builtin_choose_expr( \
    __builtin_constant_p(_$x), \
    __comp_raw_brev64_const(_$x), \
    __comp_raw_brev64_runtime(as$(u64)(_$x)) \
)
#endif
#define ____raw_reverseBitsLong(_$x...) pp_if_(abi_long_is_64bit)( \
    pp_then_(raw_reverseBits64(_$x)), \
    pp_else_(raw_reverseBits32(_$x)) \
)
#if comp_type == comp_type_clang
#define ____raw_reverseBits32(_$x...) (as$(u32)(__builtin_bitreverse32(as$(u32)(_$x))))
#else
#define ____raw_reverseBits32(_$x...) __builtin_choose_expr( \
    __builtin_constant_p(_$x), \
    __comp_raw_brev32_const(_$x), \
    __comp_raw_brev32_runtime(as$(u32)(_$x)) \
)
#endif
#if comp_type == comp_type_clang
#define ____raw_reverseBits16(_$x...) (as$(u16)(__builtin_bitreverse16(as$(u16)(_$x))))
#else
#define ____raw_reverseBits16(_$x...) __builtin_choose_expr( \
    __builtin_constant_p(_$x), \
    __comp_raw_brev16_const(_$x), \
    __comp_raw_brev16_runtime(as$(u16)(_$x)) \
)
#endif
#if comp_type == comp_type_clang
#define ____raw_reverseBits8(_$x...) (as$(u8)(__builtin_bitreverse8(as$(u8)(_$x))))
#else
#define ____raw_reverseBits8(_$x...) __builtin_choose_expr( \
    __builtin_constant_p(_$x), \
    __comp_raw_brev8(_$x), \
    __comp_raw_brev8_runtime(as$(u8)(_$x)) \
)
#endif

#define ____raw_memset0(_$p_dst, _$len...) __builtin_memset( \
    as$(u8*)(_$p_dst), as$(u32)(as$(u8)(0x00)), as$(usize)(_$len) \
)
#define ____raw_memset(_$p_dst, _$b_src, _$len...) __builtin_memset( \
    as$(u8*)(_$p_dst), as$(u32)(as$(u8)(_$b_src)), as$(usize)(_$len) \
)
#define ____raw_memcpy(_$p_dst, _$p_src, _$len...) __builtin_memcpy( \
    as$(u8*)(_$p_dst), as$(const u8*)(_$p_src), as$(usize)(_$len) \
)
#define ____raw_memmove(_$p_dst, _$p_src, _$len...) __builtin_memmove( \
    as$(u8*)(_$p_dst), as$(const u8*)(_$p_src), as$(usize)(_$len) \
)
#define ____raw_memeql(_$p_lhs, _$p_rhs, _$len...) (as$(bool)(!__builtin_memcmp( \
    as$(const u8*)(_$p_lhs), as$(const u8*)(_$p_rhs), as$(usize)(_$len) \
)))
#define __step__raw_memord(_$p_lhs, _$p_rhs, _$len...) ____raw_memord( \
    pp_uniqTok(ord), _$p_lhs, _$p_rhs, _$len \
)
#define ____raw_memord(__ord, _$p_lhs, _$p_rhs, _$len...) local_({ \
    let __ord = __builtin_memcmp( \
        as$(const u8*)(_$p_lhs), as$(const u8*)(_$p_rhs), as$(usize)(_$len) \
    ); \
    local_return_(as$(cmp_Ord)((__ord > 0) - (__ord < 0))); \
})
#define ____raw_alloca(_$len_bytes...) __builtin_alloca( \
    as$(usize)(_$len_bytes) \
)
#define ____raw_allocaAlign(_$len_bytes, _$log2_align...) __builtin_alloca_with_align( \
    as$(usize)(_$len_bytes), as$(usize)(arch_bits_per_byte) << as$(u8)(_$log2_align) \
)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_mem__included */
