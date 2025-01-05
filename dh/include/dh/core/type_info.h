/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    type_info.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1
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
    usize size  : 52;
    usize align : 12;
} TypeInfo;
#define typeInfo(T) (                                    \
    (TypeInfo){ .size = sizeof(T), .align = alignof(T) } \
)
#if COMP_TIME
#define TypeInfo_eq(val_lhs, val_rhs) eval(                \
    let         lhs = val_lhs;                             \
    let         rhs = val_rhs;                             \
    eval_return memcmp(&lhs, &rhs, sizeof(TypeInfo)) == 0; \
)
#else
force_inline bool TypeInfo_eq(TypeInfo, TypeInfo);
#endif

// For explicit materialization type representation of abstract generic types
#define typed(TDest, val_src) eval( \
    var src = val_src;              \
    eval_return(*((TDest*)&src));   \
)

typedef struct Void {
    u8 unused_[0];
} Void;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_TYPE_INFO_INCLUDED */
