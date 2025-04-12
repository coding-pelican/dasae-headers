/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    union.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-24 (date of creation)
 * @updated 2024-11-24 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Union type utilities
 * @details Provides union type utilities and functions for:
 *          - Union type definition
 *          - Union type usage
 *          - Union type operations
 *          - Union type utilities
 */

#ifndef CORE_PRIM_UNION_INCLUDED
#define CORE_PRIM_UNION_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

#define typedef_union(Alias)   \
    typedef union Alias Alias; \
    union Alias

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_UNION_INCLUDED */
