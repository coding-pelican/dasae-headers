/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    type_info.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2024-12-23 (date of last update)
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

/* TODO: supports specialized 32-bit platforms */
/* Core type information (kept minimal - 8 bytes) */
typedef u64 TypeInfoPacked;
typedef union TypeInfo {
    struct {
        TypeInfoPacked size  : 58;
        TypeInfoPacked align : 6;
    };
    TypeInfoPacked packed;
} TypeInfo;

/// Get type information for meta
#define typeInfo$(_T... /*(TypeInfo)*/) \
    ____typeInfo$(_T)
/// Convert TypeInfo struct to packed u64
#define TypeInfo_pack(_type_info...) \
    ____TypeInfo_pack(_type_info)
/// Convert packed u64 to TypeInfo struct
#define TypeInfo_unpack(_packed...) \
    ____TypeInfo_unpack(_packed)

/// Get packed type information for meta
#define packTypeInfo$(_T...) \
    ____packTypeInfo$(_T)
/// Extract size from packed TypeInfo (u64)
#define TypeInfoPacked_size(_packed...) \
    ____TypeInfoPacked_size(_packed)
/// Extract align (log2) from packed TypeInfo (u64)
#define TypeInfoPacked_align(_packed...) \
    ____TypeInfoPacked_align(_packed)

/// Compare equality of type information
#define TypeInfo_eq(_lhs /*: TypeInfo*/, _rhs /*: TypeInfo*/... /*(bool)*/) \
    ____TypeInfo_eq(_lhs, _rhs)
/// Compare equality of packed type information
#define TypeInfoPacked_eq(_lhs /*: TypeInfoPacked*/, _rhs /*: TypeInfoPacked*/... /*(bool)*/) \
    ____TypeInfoPacked_eq(_lhs, _rhs)

/// For explicit materialization type representation of abstract generic types
#define typeAsg(var_addr_dest, val_src... /* var_addr_dest */) comp_op__typeAsg(pp_uniqTok(addr_dest), var_addr_dest, val_src)
#define typeCast$(TDest_w_ValSrc...) pp_expand(pp_defer(type$)(pp_Tuple_unwrapSufCommaExpand TDest_w_ValSrc))

/*========== Macros and Definitions =========================================*/

#define ____typeInfo$(_T...) lit$((TypeInfo){ .size = sizeOf$(_T), .align = alignOf$(_T) })
#define ____TypeInfo_pack(_type_info...) ((_type_info).packed)
#define ____TypeInfo_unpack(_type_info...) ((TypeInfo){ .packed = (_type_info) })

#define ____packTypeInfo$(_T...) ( \
    (as$(TypeInfoPacked)(sizeOf$(_T)) & 0x03FFFFFFFFFFFFFFull) \
    | (as$(TypeInfoPacked)(alignOf$(_T)) << 58ull) \
)
#define ____TypeInfoPacked_size(_packed...) \
    ((_packed) & 0x03FFFFFFFFFFFFFFull)
#define ____TypeInfoPacked_align(_packed...) \
    ((u32)((_packed) >> 58))

#define ____TypeInfo_eq(_lhs, _rhs...) \
    ((_lhs).packed == (_rhs).packed)
#define ____TypeInfoPacked_eq(_lhs, _rhs...) \
    ((_lhs) == (_rhs))

#if UNUSED_CODE
#define comp_op__type$(__src, T_Dest, val_src...) \
    blk({ \
        var __src = val_src; \
        claim_assert_static_msg(!Type_eq$(TypeOf(__src), meta_P$raw), "`meta_P$raw` is not compatible with `type$`. Use `meta_castP$`."); \
        claim_assert_static_msg(!Type_eq$(TypeOf(__src), meta_S$raw), "`meta_S$raw` is not compatible with `type$`. Use `meta_castS$`."); \
        blk_return(*((T_Dest*)&(__src))); \
    })
#endif /* UNUSED_CODE */

#define comp_op__typeAsg(__addr_dest, var_addr_dest, val_src...) \
    blk({ \
        let __addr_dest = var_addr_dest; \
        *(__addr_dest) = type$(TypeOf(*(__addr_dest)), val_src); \
        blk_return __addr_dest; \
    })

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_type_info__included */
