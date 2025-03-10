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
/// Get type information for meta
#define typeInfo$(T) FUNC__typeInfo$(T)
#if !COMP_TIME
/// Compare equality of type information
force_inline bool TypeInfo_eq(TypeInfo, TypeInfo);
#else
#define TypeInfo_eq(val_lhs, val_rhs) comp_inline__TypeInfo_eq(val_lhs, val_rhs)
#endif

/// For explicit materialization type representation of abstract generic types
#define type$(TDest, val_generic_src)              FUNC__type$(TDest, val_generic_src)
#define typeAsg(var_addr_dest, val_generic_src...) comp_op__typeAsg(var_addr_dest, val_generic_src)

/*========== Macros and Implementations =====================================*/

#define FUNC__typeInfo$(T)                                  ((TypeInfo){ .size = sizeOf$(T), .align = alignOf$(T) })
#define comp_op__typeAsg(var_addr_dest, val_generic_src...) eval({   \
    let __addr_dest = var_addr_dest;                                 \
    *(__addr_dest)  = type$(TypeOf(*(__addr_dest)), val_generic_src); \
    eval_return __addr_dest;                                         \
})

#if COMP_TIME
#define comp_inline__TypeInfo_eq(val_lhs, val_rhs) eval({    \
    let _lhs = val_lhs;                                      \
    let _rhs = val_rhs;                                      \
    eval_return memcmp(&_lhs, &_rhs, sizeOf(TypeInfo)) == 0; \
})
#endif

#define FUNC__type$(TDest, val_generic_src) eval({                                                                                   \
    var _src = val_generic_src;                                                                                                      \
    claim_assert_static_msg(!isSameType(TypeOf(_src), meta_Ptr), "`meta_Ptr` is not compatible with `type$`. Use `meta_castPtr$`."); \
    claim_assert_static_msg(!isSameType(TypeOf(_src), meta_Sli), "`meta_Sli` is not compatible with `type$`. Use `meta_castSli$`."); \
    eval_return(*((TDest*)&_src));                                                                                                   \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_TYPE_INFO_INCLUDED */
