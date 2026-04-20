/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    mem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-12 (date of creation)
 * @updated 2026-03-12 (date of last update)
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  (none)
 */
#ifndef builtin_mem__included
#define builtin_mem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Declarations ========================================*/

#define bits_per_bit __comp_int__bits_per_bit
#define bits_per_octet __comp_int__bits_per_octet
#define bits_per_hextet __comp_int__bits_per_hextet
#define bits_per_quadlet __comp_int__bits_per_quadlet
#define bits_per_octlet __comp_int__bits_per_octlet
#define bits_per_hexlet __comp_int__bits_per_hexlet

#define raw_countOnesSize(_x...) ____raw_countOnesSize(_x)
#define raw_countOnes64(_x...) ____raw_countOnes64(_x)
#define raw_countOnes32(_x...) ____raw_countOnes32(_x)
#define raw_countOnes16(_x...) ____raw_countOnes16(_x)
#define raw_countOnes8(_x...) ____raw_countOnes8(_x)

#define raw_leadingZerosSize(_x...) ____raw_leadingZerosSize(_x)
#define raw_leadingZeros64(_x...) ____raw_leadingZeros64(_x)
#define raw_leadingZeros32(_x...) ____raw_leadingZeros32(_x)
#define raw_leadingZeros16(_x...) ____raw_leadingZeros16(_x)
#define raw_leadingZeros8(_x...) ____raw_leadingZeros8(_x)

#define raw_trailingZerosSize(_x...) ____raw_trailingZerosSize(_x)
#define raw_trailingZeros64(_x...) ____raw_trailingZeros64(_x)
#define raw_trailingZeros32(_x...) ____raw_trailingZeros32(_x)
#define raw_trailingZeros16(_x...) ____raw_trailingZeros16(_x)
#define raw_trailingZeros8(_x...) ____raw_trailingZeros8(_x)

#define raw_rotateLeftSize(_x, y...) ____raw_rotateLeftSize(_x, y)
#define raw_rotateLeft64(_x, y...) ____raw_rotateLeft64(_x, y)
#define raw_rotateLeft32(_x, y...) ____raw_rotateLeft32(_x, y)
#define raw_rotateLeft16(_x, y...) ____raw_rotateLeft16(_x, y)
#define raw_rotateLeft8(_x, y...) ____raw_rotateLeft8(_x, y)

#define raw_rotateRightSize(_x, y...) ____raw_rotateRightSize(_x, y)
#define raw_rotateRight64(_x, y...) ____raw_rotateRight64(_x, y)
#define raw_rotateRight32(_x, y...) ____raw_rotateRight32(_x, y)
#define raw_rotateRight16(_x, y...) ____raw_rotateRight16(_x, y)
#define raw_rotateRight8(_x, y...) ____raw_rotateRight8(_x, y)

#define raw_swapBytesSize(_x...) ____raw_swapBytesSize(_x)
#define raw_swapBytes64(_x...) ____raw_swapBytes64(_x)
#define raw_swapBytes32(_x...) ____raw_swapBytes32(_x)
#define raw_swapBytes16(_x...) ____raw_swapBytes16(_x)

#define raw_reverseBitsSize(_x...) ____raw_reverseBitsSize(_x)
#define raw_reverseBits64(_x...) ____raw_reverseBits64(_x)
#define raw_reverseBits32(_x...) ____raw_reverseBits32(_x)
#define raw_reverseBits16(_x...) ____raw_reverseBits16(_x)
#define raw_reverseBits8(_x...) ____raw_reverseBits8(_x)

#undef memset
#undef memcpy
#undef memmove
#undef memcmp
#undef alloca

#define raw_memset(_p_dst, _b_src, _len...) ____raw_memset(_p_dst, _b_src, _len)
#define raw_memset0(_p_dst, _len...) ____raw_memset0(_p_dst, _len)
#define raw_memcpy(_p_dst, _p_src, _len...) ____raw_memcpy(_p_dst, _p_src, _len)
#define raw_memmove(_p_dst, _p_src, _len...) ____raw_memmove(_p_dst, _p_src, _len)
#define raw_memeql(_p_lhs, _p_rhs, _len...) ____raw_memeql(_p_lhs, _p_rhs, _len)
#define raw_memord(_p_lhs, _p_rhs, _len...) ____raw_memord(_p_lhs, _p_rhs, _len)
#define raw_alloca(_len_bytes...) ____raw_alloca(_len_bytes)
#define raw_allocaAlign(_len_bytes, _log2_align...) ____raw_allocaAlign(_len_bytes, _log2_align)

/*========== Macros and Definitions =========================================*/

#define __comp_int__bits_per_bit 1
#define __comp_int__bits_per_octet 8
#define __comp_int__bits_per_hextet 16
#define __comp_int__bits_per_quadlet 32
#define __comp_int__bits_per_octlet 64
#define __comp_int__bits_per_hexlet 128

#define ____raw_countOnesSize(_x...) pp_if_(arch_bits_is_64bit)( \
    pp_then_(raw_countOnes64(_x)), \
    pp_else_(raw_countOnes32(_x)) \
)
#define ____raw_countOnes64(_x...) (as$(u32)(__builtin_popcountll(as$(u64)(_x))))
#define ____raw_countOnes32(_x...) (as$(u32)(__builtin_popcount(as$(u32)(_x))))
#define ____raw_countOnes16(_x...) (as$(u32)(__builtin_popcount(as$(u32)(as$(u16)(_x)))))
#define ____raw_countOnes8(_x...) (as$(u32)(__builtin_popcount(as$(u32)(as$(u8)(_x)))))

#define ____raw_leadingZerosSize(_x...) pp_if_(arch_bits_is_64bit)( \
    pp_then_(raw_leadingZeros64(_x)), \
    pp_else_(raw_leadingZeros32(_x)) \
)
#define ____raw_leadingZeros64(_x...) (as$(u32)(__builtin_clzll(as$(u64)(_x))))
#define ____raw_leadingZeros32(_x...) (as$(u32)(__builtin_clz(as$(u32)(_x))))
#define ____raw_leadingZeros16(_x...) (as$(u32)(__builtin_clz(as$(u32)(as$(u16)(_x)))) - 16)
#define ____raw_leadingZeros8(_x...) (as$(u32)(__builtin_clz(as$(u32)(as$(u8)(_x)))) - 24)

#define ____raw_trailingZerosSize(_x...) pp_if_(arch_bits_is_64bit)( \
    pp_then_(raw_trailingZeros64(_x)), \
    pp_else_(raw_trailingZeros32(_x)) \
)
#define ____raw_trailingZeros64(_x...) (as$(u32)(__builtin_ctzll(as$(u64)(_x))))
#define ____raw_trailingZeros32(_x...) (as$(u32)(__builtin_ctz(as$(u32)(_x))))
#define ____raw_trailingZeros16(_x...) (as$(u32)(__builtin_ctz(as$(u32)(as$(u16)(_x)))))
#define ____raw_trailingZeros8(_x...) (as$(u32)(__builtin_ctz(as$(u32)(as$(u8)(_x)))))

#define ____raw_rotateLeftSize(_x, y...) pp_if_(arch_bits_is_64bit)( \
    pp_then_(raw_rotateLeft64(_x, y)), \
    pp_else_(raw_rotateLeft32(_x, y)) \
)
#define ____raw_rotateLeft64(_x, y...) (as$(u64)(__builtin_rotateleft64(as$(u64)(_x), as$(u64)(y))))
#define ____raw_rotateLeft32(_x, y...) (as$(u32)(__builtin_rotateleft32(as$(u32)(_x), as$(u32)(y))))
#define ____raw_rotateLeft16(_x, y...) (as$(u16)(__builtin_rotateleft16(as$(u16)(_x), as$(u16)(y))))
#define ____raw_rotateLeft8(_x, y...) (as$(u8)(__builtin_rotateleft8(as$(u8)(_x), as$(u8)(y))))

#define ____raw_rotateRightSize(_x, y...) pp_if_(arch_bits_is_64bit)( \
    pp_then_(raw_rotateRight64(_x, y)), \
    pp_else_(raw_rotateRight32(_x, y)) \
)
#define ____raw_rotateRight64(_x, y...) (as$(u64)(__builtin_rotateright64(as$(u64)(_x), as$(u64)(y))))
#define ____raw_rotateRight32(_x, y...) (as$(u32)(__builtin_rotateright32(as$(u32)(_x), as$(u32)(y))))
#define ____raw_rotateRight16(_x, y...) (as$(u16)(__builtin_rotateright16(as$(u16)(_x), as$(u16)(y))))
#define ____raw_rotateRight8(_x, y...) (as$(u8)(__builtin_rotateright8(as$(u8)(_x), as$(u8)(y))))

#define ____raw_swapBytesSize(_x...) pp_if_(arch_bits_is_64bit)( \
    pp_then_(raw_swapBytes64(_x)), \
    pp_else_(raw_swapBytes32(_x)) \
)
#define ____raw_swapBytes64(_x...) (as$(u64)(__builtin_bswap64(as$(u64)(_x))))
#define ____raw_swapBytes32(_x...) (as$(u32)(__builtin_bswap32(as$(u32)(_x))))
#define ____raw_swapBytes16(_x...) (as$(u16)(__builtin_bswap16(as$(u16)(_x))))

#define ____raw_reverseBitsSize(_x...) pp_if_(arch_bits_is_64bit)( \
    pp_then_(raw_reverseBits64(_x)), \
    pp_else_(raw_reverseBits32(_x)) \
)
#define ____raw_reverseBits64(_x...) (as$(u64)(__builtin_bitreverse64(as$(u64)(_x))))
#define ____raw_reverseBits32(_x...) (as$(u32)(__builtin_bitreverse32(as$(u32)(_x))))
#define ____raw_reverseBits16(_x...) (as$(u16)(__builtin_bitreverse16(as$(u16)(_x))))
#define ____raw_reverseBits8(_x...) (as$(u8)(__builtin_bitreverse8(as$(u8)(_x))))

#define ____raw_memset(_p_dst, _b_src, _len...) __builtin_memset( \
    as$(u8*)(_p_dst), as$(u32)(as$(u8)(_b_src)), as$(usize)(_len) \
)
#define ____raw_memset0(_p_dst, _len...) __builtin_memset( \
    as$(u8*)(_p_dst), as$(u32)(as$(u8)(0x00)), as$(usize)(_len) \
)
#define ____raw_memcpy(_p_dst, _p_src, _len...) __builtin_memcpy( \
    as$(u8*)(_p_dst), as$(const u8*)(_p_src), as$(usize)(_len) \
)
#define ____raw_memmove(_p_dst, _p_src, _len...) __builtin_memmove( \
    as$(u8*)(_p_dst), as$(const u8*)(_p_src), as$(usize)(_len) \
)
#define ____raw_memeql(_p_lhs, _p_rhs, _len...) (as$(bool)(!__builtin_memcmp( \
    as$(const u8*)(_p_lhs), as$(const u8*)(_p_rhs), as$(usize)(_len) \
)))
#define ____raw_memord(_p_lhs, _p_rhs, _len...) (as$(cmp_Ord)(__builtin_memcmp( \
    as$(const u8*)(_p_lhs), as$(const u8*)(_p_rhs), as$(usize)(_len) \
)))
#define ____raw_alloca(_len_bytes...) __builtin_alloca( \
    as$(usize)(_len_bytes) \
)
#define ____raw_allocaAlign(_len_bytes, _log2_align...) __builtin_alloca_with_align( \
    as$(usize)(_len_bytes), as$(usize)(1) << as$(u8)(_log2_align) \
)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_mem__included */
