/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    mem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-12 (date of creation)
 * @updated 2025-02-12 (date of last update)
 * @version v0.1-alpha
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

#define raw_popcntSize(_x...) ____raw_popcntSize(_x)
#define raw_popcnt64(_x...) ____raw_popcnt64(_x)
#define raw_popcnt32(_x...) ____raw_popcnt32(_x)
#define raw_popcnt16(_x...) ____raw_popcnt16(_x)
#define raw_popcnt8(_x...) ____raw_popcnt8(_x)

#define raw_clzSize(_x...) ____raw_clzSize(_x)
#define raw_clz64(_x...) ____raw_clz64(_x)
#define raw_clz32(_x...) ____raw_clz32(_x)
#define raw_clz16(_x...) ____raw_clz16(_x)
#define raw_clz8(_x...) ____raw_clz8(_x)

#define raw_ctzSize(_x...) ____raw_ctzSize(_x)
#define raw_ctz64(_x...) ____raw_ctz64(_x)
#define raw_ctz32(_x...) ____raw_ctz32(_x)
#define raw_ctz16(_x...) ____raw_ctz16(_x)
#define raw_ctz8(_x...) ____raw_ctz8(_x)

#undef memcpy
#define memcpy(_p_dst, _p_src, _len...) __op__memcpy(_p_dst, _p_src, _len)
#define raw_memcpy(_p_dst, _p_src, _len...) ____raw_memcpy(_p_dst, _p_src, _len)
#undef memmove
#define memmove(_p_dst, _p_src, _len...) __op__memmove(_p_dst, _p_src, _len)
#define raw_memmove(_p_dst, _p_src, _len...) ____raw_memmove(_p_dst, _p_src, _len)
#undef memset
#define memset(_p_dst, _byte_src, _len...) __op__memset(_p_dst, _byte_src, _len)
#define raw_memset(_p_dst, _byte_src, _len...) ____raw_memset(_p_dst, _byte_src, _len)
#define memset0(_p_dst, _len...) __op__memset0(_p_dst, _len)
#define raw_memset0(_p_dst, _len...) ____raw_memset0(_p_dst, _len)
#undef memcmp
#define memcmp(_p_lhs, _p_rhs, _len...) __op__memcmp(_p_lhs, _p_rhs, _len)
#define raw_memcmp(_p_lhs, _p_rhs, _len...) ____raw_memcmp(_p_lhs, _p_rhs, _len)
#undef alloca
#define alloca(_len_bytes...) ____alloca(_len_bytes)

/*========== Macros and Definitions =========================================*/

#define __comp_int__bits_per_bit 1
#define __comp_int__bits_per_octet 8
#define __comp_int__bits_per_hextet 16
#define __comp_int__bits_per_quadlet 32
#define __comp_int__bits_per_octlet 64
#define __comp_int__bits_per_hexlet 128

#define ____raw_popcntSize(_x...) pp_if_(arch_bits_is_64bit)( \
    pp_then_(raw_popcnt64(_x)), \
    pp_else_(raw_popcnt32(_x)) \
)
#define ____raw_popcnt64(_x...) (as$(u32)(__builtin_popcountll(as$(u64)(_x))))
#define ____raw_popcnt32(_x...) (as$(u32)(__builtin_popcount(as$(u32)(_x))))
#define ____raw_popcnt16(_x...) (as$(u32)(__builtin_popcount(as$(u32)(as$(u16)(_x)))))
#define ____raw_popcnt8(_x...) (as$(u32)(__builtin_popcount(as$(u32)(as$(u8)(_x)))))

#define ____raw_clzSize(_x...) pp_if_(arch_bits_is_64bit)( \
    pp_then_(raw_clz64(_x)), \
    pp_else_(raw_clz32(_x)) \
)
#define ____raw_clz64(_x...) (as$(u32)(__builtin_clzll(as$(u64)(_x))))
#define ____raw_clz32(_x...) (as$(u32)(__builtin_clz(as$(u32)(_x))))
#define ____raw_clz16(_x...) (as$(u32)(__builtin_clz(as$(u32)(as$(u16)(_x)))) - 16)
#define ____raw_clz8(_x...) (as$(u32)(__builtin_clz(as$(u32)(as$(u8)(_x)))) - 24)

#define ____raw_ctzSize(_x...) pp_if_(arch_bits_is_64bit)( \
    pp_then_(raw_ctz64(_x)), \
    pp_else_(raw_ctz32(_x)) \
)
#define ____raw_ctz64(_x...) (as$(u32)(__builtin_ctzll(as$(u64)(_x))))
#define ____raw_ctz32(_x...) (as$(u32)(__builtin_ctz(as$(u32)(_x))))
#define ____raw_ctz16(_x...) (as$(u32)(__builtin_ctz(as$(u32)(as$(u16)(_x)))))
#define ____raw_ctz8(_x...) (as$(u32)(__builtin_ctz(as$(u32)(as$(u8)(_x)))))

#define __op__memcpy(_p_dst, _p_src, _len...) __builtin_memcpy(_p_dst, _p_src, _len)
#define ____raw_memcpy(_p_dst, _p_src, _len...) __builtin_memcpy(_p_dst, _p_src, _len)
#define __op__memmove(_p_dst, _p_src, _len...) __builtin_memmove(_p_dst, _p_src, _len)
#define ____raw_memmove(_p_dst, _p_src, _len...) __builtin_memmove(_p_dst, _p_src, _len)
#define __op__memset(_p_dst, _byte_src, _len...) __builtin_memset(_p_dst, _byte_src, _len)
#define ____raw_memset(_p_dst, _byte_src, _len...) __builtin_memset(_p_dst, _byte_src, _len)
#define __op__memset0(_p_dst, _len...) __builtin_memset(_p_dst, 0x00, _len)
#define ____raw_memset0(_p_dst, _len...) __builtin_memset(_p_dst, 0x00, _len)
#define __op__memcmp(_p_lhs, _p_rhs, _len...) __builtin_memcmp(_p_lhs, _p_rhs, _len)
#define ____raw_memcmp(_p_lhs, _p_rhs, _len...) __builtin_memcmp(_p_lhs, _p_rhs, _len)
#define ____alloca(_len_bytes...) __builtin_alloca(_len_bytes)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_mem__included */
