/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    struct.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  (none)
 *
 * @brief   Struct type utilities
 * @details Provides struct type utilities and functions for:
 *          - Struct type definition
 *          - Struct type usage
 *          - Struct type operations
 *          - Struct type utilities
 */
#ifndef CORE_PRIM_STRUCT_INCLUDED
#define CORE_PRIM_STRUCT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "int.h"

/*========== Macros and Definitions =========================================*/

#define typedef_struct(Alias) \
    typedef struct Alias Alias; \
    struct Alias

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_STRUCT_INCLUDED */
