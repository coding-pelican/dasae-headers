/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    type_info.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core
 * @prefix  NONE
 *
 * @brief   Type information for type-safe programming
 * @details Some detailed explanation
 */

#ifndef CORE_TYPE_INFO_INCLUDED
#define CORE_TYPE_INFO_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

/* Core type information (kept minimal - 8 bytes) */
typedef struct TypeInfo {
    u32 size;
    u32 align;
} TypeInfo;
#define typeInfo$(T) typeInfo(T)
#define typeInfo(T)  ((TypeInfo){ .size = sizeOf$(T), .align = alignOf$(T) })
#if !COMP_TIME
force_inline bool TypeInfo_eq(TypeInfo, TypeInfo);
#else
#define TypeInfo_eq(val_lhs, val_rhs) eval({                 \
    let         _lhs = val_lhs;                              \
    let         _rhs = val_rhs;                              \
    eval_return memcmp(&_lhs, &_rhs, sizeOf(TypeInfo)) == 0; \
})
#endif

// For explicit materialization type representation of abstract generic types
#define type$(TDest, val_generic_src) typed(TDest, val_generic_src)
#define typed(TDest, val_generic_src) eval({                                                                                         \
    var _src = val_generic_src;                                                                                                      \
    claim_assert_static_msg(!isSameType(TypeOf(_src), meta_Ptr), "`meta_Ptr` is not compatible with `typed`. Use `meta_castPtr$`."); \
    claim_assert_static_msg(!isSameType(TypeOf(_src), meta_Sli), "`meta_Sli` is not compatible with `typed`. Use `meta_castSli$`."); \
    eval_return(*((TDest*)&_src));                                                                                                   \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_TYPE_INFO_INCLUDED */
