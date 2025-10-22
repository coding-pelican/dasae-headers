/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ThrdSafe.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-21 (date of creation)
 * @updated 2025-05-21 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_ThrdSafe
 */

#ifndef HEAP_THRD_SAFE_INCLUDED
#define HEAP_THRD_SAFE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/Thrd/Mtx.h"

/*========== Page Allocator =================================================*/

typedef struct heap_ThrdSafe {
    mem_Allocator child_allocator;
    Thrd_Mtx      mutex;
} heap_ThrdSafe;

/// Get allocator interface for instance
extern fn_((heap_ThrdSafe_allocator(heap_ThrdSafe* self))(mem_Allocator));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HEAP_THRD_SAFE_INCLUDED */
