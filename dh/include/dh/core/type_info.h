/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    type_info.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2026-07-30 (date of last update)
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
#pragma once
#ifndef core_type_info__included
#define core_type_info__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pri.h"

/*========== Macros and Declarations ========================================*/

#define TypeInfo_size_bits_on_16bit 13
#define TypeInfo_align_bits_on_16bit 3
claim_assert_static((TypeInfo_size_bits_on_16bit + TypeInfo_align_bits_on_16bit) == abi_bits_16);

#define TypeInfo_size_bits_on_64bit 58
#define TypeInfo_align_bits_on_64bit 6
claim_assert_static((TypeInfo_size_bits_on_64bit + TypeInfo_align_bits_on_64bit) == abi_bits_64);
#define TypeInfo_size_bits_on_32bit 28
#define TypeInfo_align_bits_on_32bit 4
claim_assert_static((TypeInfo_size_bits_on_32bit + TypeInfo_align_bits_on_32bit) == abi_bits_32);

/// Core type information packed in the native size integer domain.
typedef usize TypeInfoPacked;

/// Get type information for meta
#define typeInfo$(_$T... /*(TypeInfo)*/) \
    ____typeInfo$(_$T)
/// Convert TypeInfo struct to packed usize
#define TypeInfo_pack(_$type_info /*: TypeInfo*/... /*(TypeInfoPacked)*/) \
    ____TypeInfo_pack(_$type_info)
/// Convert packed usize to TypeInfo struct
#define TypeInfo_unpack(_$packed /*: TypeInfoPacked*/... /*(TypeInfo)*/) \
    ____TypeInfo_unpack(_$packed)
/// Extract size from TypeInfo
#define TypeInfo_size(_$type_info /*: TypeInfo*/... /*(usize)*/) \
    ____TypeInfo_size(_$type_info)
/// Extract alignment from TypeInfo
#define TypeInfo_align(_$type_info /*: TypeInfo*/... /*(usize)*/) \
    ____TypeInfo_align(_$type_info)
/// Extract alignment (log2) from TypeInfo
#define TypeInfo_log2_align(_$type_info /*: TypeInfo*/... /*(u8)*/) \
    ____TypeInfo_log2_align(_$type_info)

/// Get packed type information for meta
#define packTypeInfo$(_$T...) \
    ____packTypeInfo$(_$T)
/// Extract size from packed TypeInfo (usize)
#define TypeInfoPacked_size(_$packed /*: TypeInfoPacked*/... /*(usize)*/) \
    ____TypeInfoPacked_size(_$packed)
/// Extract alignment from packed TypeInfo
#define TypeInfoPacked_align(_$packed /*: TypeInfoPacked*/... /*(usize)*/) \
    ____TypeInfoPacked_align(_$packed)
/// Extract alignment (log2) from packed TypeInfo
#define TypeInfoPacked_log2_align(_$packed /*: TypeInfoPacked*/... /*(u8)*/) \
    ____TypeInfoPacked_log2_align(_$packed)

/// Compare equality of type information
#define TypeInfo_eql(_$lhs /*: TypeInfo*/, _$rhs /*: TypeInfo*/... /*(bool)*/) \
    ____TypeInfo_eql(_$lhs, _$rhs)
#define TypeInfo_neq(_$lhs /*: TypeInfo*/, _$rhs /*: TypeInfo*/... /*(bool)*/) \
    ____TypeInfo_neq(_$lhs, _$rhs)
/// Compare equality of packed type information
#define TypeInfoPacked_eql(_$lhs /*: TypeInfoPacked*/, _$rhs /*: TypeInfoPacked*/... /*(bool)*/) \
    ____TypeInfoPacked_eql(_$lhs, _$rhs)
#define TypeInfoPacked_neq(_$lhs /*: TypeInfoPacked*/, _$rhs /*: TypeInfoPacked*/... /*(bool)*/) \
    ____TypeInfoPacked_neq(_$lhs, _$rhs)

/*========== Macros and Definitions =========================================*/

#define TypeInfo_size_bits pp_expand( \
    pp_switch_ pp_begin(abi_size_unit)( \
        pp_case_((abi_bits_unit_16bit)(TypeInfo_size_bits_on_16bit)), \
        pp_case_((abi_bits_unit_32bit)(TypeInfo_size_bits_on_32bit)), \
        pp_case_((abi_bits_unit_64bit)(TypeInfo_size_bits_on_64bit)), \
        pp_default_(0) \
    ) pp_end \
)
#define TypeInfo_align_bits pp_expand( \
    pp_switch_ pp_begin(abi_size_unit)( \
        pp_case_((abi_bits_unit_16bit)(TypeInfo_align_bits_on_16bit)), \
        pp_case_((abi_bits_unit_32bit)(TypeInfo_align_bits_on_32bit)), \
        pp_case_((abi_bits_unit_64bit)(TypeInfo_align_bits_on_64bit)), \
        pp_default_(0) \
    ) pp_end \
)
claim_assert_static_msg(
    abi_size_is_16bit || abi_size_is_32bit || abi_size_is_64bit,
    "TypeInfoPacked requires a supported native pointer-size domain"
);
bitfield_((TypeInfo)(TypeInfoPacked)(
    (size, TypeInfoPacked, TypeInfo_size_bits),
    (log2_align, TypeInfoPacked, TypeInfo_align_bits)
));
claim_assert_static(bitfield_bits$(TypeInfo) == abi_size_bits);
claim_assert_static(int_bits$(TypeInfoPacked) == bitfield_bits$(TypeInfo));
claim_assert_static(int_bytes$(TypeInfoPacked) == int_bits$(TypeInfoPacked) / arch_bits_per_byte);
claim_assert_static(sizeOf$(TypeInfoPacked) == int_bytes$(TypeInfoPacked));

#define ____typeInfo$(_$T...) l$((TypeInfo){ .size = sizeOf$(_$T), .log2_align = alignOfLog2$(_$T) })
#define ____TypeInfo_pack(_$type_info...) ((_$type_info).packed)
#define ____TypeInfo_unpack(_$type_info...) l$((TypeInfo){ .packed = (_$type_info) })
#define ____TypeInfo_size(_$type_info...) (as$(usize)((_$type_info).size))
#define ____TypeInfo_align(_$type_info...) log2ToAlign_static(TypeInfo_log2_align(_$type_info))
#define ____TypeInfo_log2_align(_$type_info...) (as$(u8)((_$type_info).log2_align))

#define ____packTypeInfo$(_$T...) ( \
    (as$(TypeInfoPacked)(sizeOf$(_$T)) & int_maskLo_static$((TypeInfoPacked)(TypeInfo_size_bits))) \
    | (as$(TypeInfoPacked)(alignOfLog2$(_$T)) << TypeInfo_size_bits) \
)
#define ____TypeInfoPacked_size(_$packed...) \
    ((_$packed) & int_maskLo_static$((TypeInfoPacked)(TypeInfo_size_bits)))
#define ____TypeInfoPacked_align(_$packed...) \
    (as$(u32)((_$packed) >> TypeInfo_size_bits))

#define ____TypeInfo_eql(_$lhs, _$rhs...) TypeInfoPacked_eql((_$lhs).packed, (_$rhs).packed)
#define ____TypeInfo_neq(_$lhs, _$rhs...) TypeInfoPacked_neq((_$lhs).packed, (_$rhs).packed)
#define ____TypeInfoPacked_eql(_$lhs, _$rhs...) pri_eql(_$lhs, _$rhs)
#define ____TypeInfoPacked_neq(_$lhs, _$rhs...) pri_neq(_$lhs, _$rhs)

claim_assert_static(packTypeInfo$(TypeInfo) == packTypeInfo$(TypeInfoPacked));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_type_info__included */
