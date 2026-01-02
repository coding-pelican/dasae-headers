/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    type_info.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2025-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core
 * @prefix  (none)
 *
 * @brief   Type information for type-safe programming
 * @details Provides type information for type-safe programming:
 *          - Type size and alignment
 *          - Type comparison
 *          - Type casting
 *          - Type conversion
 *          - Type utility functions
 */
#ifndef core_type_info__included
#define core_type_info__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim.h"

/*========== Macros and Declarations ========================================*/

#define TypeInfo_size_bits_on_64bit 58
#define TypeInfo_align_bits_on_64bit 6
claim_assert_static((TypeInfo_size_bits_on_64bit + TypeInfo_align_bits_on_64bit) == arch_bits_wide_64bit);
#define TypeInfo_size_bits_on_32bit 28
#define TypeInfo_align_bits_on_32bit 4
claim_assert_static((TypeInfo_size_bits_on_32bit + TypeInfo_align_bits_on_32bit) == arch_bits_wide_32bit);

#define TypeInfo_size_bits __comp_int__TypeInfo_size_bits
#define TypeInfo_align_bits __comp_int__TypeInfo_align_bits
#define TypeInfo_bits __comp_const__TypeInfo_bits

/// Core type information (kept minimal size)
typedef union TypeInfo TypeInfo;
typedef usize TypeInfoPacked;

/// Get type information for meta
#define typeInfo$(_T... /*(TypeInfo)*/) \
    ____typeInfo$(_T)
/// Convert TypeInfo struct to packed usize
#define TypeInfo_pack(_type_info /*: TypeInfo*/... /*(TypeInfoPacked)*/) \
    ____TypeInfo_pack(_type_info)
/// Convert packed usize to TypeInfo struct
#define TypeInfo_unpack(_packed /*: TypeInfoPacked*/... /*(TypeInfo)*/) \
    ____TypeInfo_unpack(_packed)
#define TypeInfo_size(_type_info /*: TypeInfo*/... /*(usize)*/) \
    ____TypeInfo_size(_type_info)
#define TypeInfo_align(_type_info /*: TypeInfo*/... /*(mem_Log2Align)*/) \
    ____TypeInfo_align(_type_info)

/// Get packed type information for meta
#define packTypeInfo$(_T...) \
    ____packTypeInfo$(_T)
/// Extract size from packed TypeInfo (usize)
#define TypeInfoPacked_size(_packed /*: TypeInfoPacked*/... /*(usize)*/) \
    ____TypeInfoPacked_size(_packed)
/// Extract align (log2) from packed TypeInfo (usize)
#define TypeInfoPacked_align(_packed /*: TypeInfoPacked*/... /*(mem_Log2Align)*/) \
    ____TypeInfoPacked_align(_packed)

/// Compare equality of type information
#define TypeInfo_eq(_lhs /*: TypeInfo*/, _rhs /*: TypeInfo*/... /*(bool)*/) \
    ____TypeInfo_eq(_lhs, _rhs)
/// Compare equality of packed type information
#define TypeInfoPacked_eq(_lhs /*: TypeInfoPacked*/, _rhs /*: TypeInfoPacked*/... /*(bool)*/) \
    ____TypeInfoPacked_eq(_lhs, _rhs)

/*========== Macros and Definitions =========================================*/

#define __comp_int__TypeInfo_size_bits pp_if_(arch_bits_is_64bit)( \
    pp_then_(TypeInfo_size_bits_on_64bit), \
    pp_else_(TypeInfo_size_bits_on_32bit) \
)
#define __comp_int__TypeInfo_align_bits pp_if_(arch_bits_is_64bit)( \
    pp_then_(TypeInfo_align_bits_on_64bit), \
    pp_else_(TypeInfo_align_bits_on_32bit) \
)
#define __comp_const__TypeInfo_bits (TypeInfo_size_bits + TypeInfo_align_bits)

claim_assert_static(TypeInfo_bits == arch_bits_wide);
claim_assert_static(int_bits$(TypeInfoPacked) == TypeInfo_bits);
claim_assert_static(int_bytes$(TypeInfoPacked) == int_bits$(TypeInfoPacked) / arch_bits_per_byte);
claim_assert_static(sizeOf$(TypeInfoPacked) == int_bytes$(TypeInfoPacked));

union TypeInfo {
    struct {
        TypeInfoPacked size  : TypeInfo_size_bits;
        TypeInfoPacked align : TypeInfo_align_bits; /* same as `mem_Log2Align`,  */
    };
    TypeInfoPacked packed;
};

#define ____typeInfo$(_T...) lit$((TypeInfo){ .size = sizeOf$(_T), .align = alignOf$(_T) })
#define ____TypeInfo_pack(_type_info...) ((_type_info).packed)
#define ____TypeInfo_unpack(_type_info...) ((TypeInfo){ .packed = (_type_info) })
#define ____TypeInfo_size(_type_info...) (as$(usize)((_type_info).size))
#define ____TypeInfo_align(_type_info...) (as$(u8)((_type_info).align))

#define ____packTypeInfo$(_T...) ( \
    (as$(TypeInfoPacked)(sizeOf$(_T)) & prim_maskLo_static$((TypeInfoPacked)(TypeInfo_size_bits))) \
    | (as$(TypeInfoPacked)(alignOf$(_T)) << TypeInfo_size_bits) \
)
#define ____TypeInfoPacked_size(_packed...) \
    ((_packed) & prim_maskLo_static$((TypeInfoPacked)(TypeInfo_size_bits)))
#define ____TypeInfoPacked_align(_packed...) \
    (as$(u32)((_packed) >> TypeInfo_size_bits))

#define ____TypeInfo_eq(_lhs, _rhs...) \
    ((_lhs).packed == (_rhs).packed)
#define ____TypeInfoPacked_eq(_lhs, _rhs...) \
    ((_lhs) == (_rhs))

claim_assert_static(packTypeInfo$(TypeInfo) == packTypeInfo$(TypeInfoPacked));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_type_info__included */
