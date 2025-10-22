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
 * @prefix  NONE
 *
 * @brief   Type information for type-safe programming
 * @details Provides type information for type-safe programming:
 *          - Type size and alignment
 *          - Type comparison
 *          - Type casting
 *          - Type conversion
 *          - Type utility functions
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
#define typeInfo$(T /* TypeInfo */) comp_op__typeInfo$(T)
/// Compare equality of type information
static bool TypeInfo_eq(TypeInfo, TypeInfo);

/// For explicit materialization type representation of abstract generic types
#define type$(T_Dest, val_src... /* T_Dest */)                 comp_op__type$(pp_uniqTok(src), T_Dest, val_src)
#define typeAsg(var_addr_dest, val_src... /* var_addr_dest */) comp_op__typeAsg(pp_uniqTok(addr_dest), var_addr_dest, val_src)

#define typeCast$(TDest_w_ValSrc...) \
    pp_expand(pp_defer(type$)(pp_Tuple_unwrapSufCommaExpand TDest_w_ValSrc))

/*========== Macros and Implementations =====================================*/

#define comp_op__typeInfo$(T) ((TypeInfo){ .size = sizeOf$(T), .align = alignOf$(T) })
#if COMP_TIME
$inline_always bool TypeInfo_eq(TypeInfo lhs, TypeInfo rhs) {
    return bti_memcmp(&lhs, &rhs, sizeOf(TypeInfo)) == 0;
}
#endif /* COMP_TIME */

#define comp_op__type$(__src, T_Dest, val_src...) eval({ \
    var __src = val_src; \
    claim_assert_static_msg( \
        !isSameType(TypeOf(__src), meta_Ptr), \
        "`meta_Ptr` is not compatible with `type$`. Use `meta_castPtr$`." \
    ); \
    claim_assert_static_msg( \
        !isSameType(TypeOf(__src), meta_Sli), \
        "`meta_Sli` is not compatible with `type$`. Use `meta_castSli$`." \
    ); \
    eval_return(*((T_Dest*)&(__src))); \
})
#define comp_op__typeAsg(__addr_dest, var_addr_dest, val_src...) eval({ \
    let __addr_dest = var_addr_dest; \
    *(__addr_dest)  = type$(TypeOf(*(__addr_dest)), val_src); \
    eval_return __addr_dest; \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_TYPE_INFO_INCLUDED */
