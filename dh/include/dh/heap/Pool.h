/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Pool.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-23 (date of creation)
 * @updated 2026-04-23 (date of last update)
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Pool
 */
#ifndef heap_Pool__included
#define heap_Pool__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Arena.h"

/*========== Macros and Declarations ========================================*/

T_alias$((heap_Pool_Opts)(struct heap_Pool_Opts {
    var_(growable, bool);
}));

/*========== Macros and Definitions =========================================*/

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_Pool__included */
