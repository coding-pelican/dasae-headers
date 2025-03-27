/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Arena.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-26 (date of creation)
 * @updated 2025-03-26 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Arena
 *
 * @brief   Arena allocator that wraps another allocator for bulk freeing
 * @details Takes an existing allocator, wraps it, and provides an interface
 *          where you can allocate without freeing, and then free it all together.
 */

#ifndef HEAP_ARENA_INCLUDED
#define HEAP_ARENA_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/mem/Allocator.h"
#include "dh/list.h"

/*========== Arena Allocator ===============================================*/

use_ListSgl$(usize);
typedef struct heap_Arena heap_Arena;
typedef struct heap_Arena_State {
    ListSgl$usize buffer_list;
    usize         end_index;
} heap_Arena_State;

/// Inner state of ArenaAllocator
pub fn_(heap_Arena_State_promote(heap_Arena_State* self, mem_Allocator child_allocator), heap_Arena);

struct heap_Arena {
    mem_Allocator    child_allocator;
    heap_Arena_State state;
};

/// Get allocator interface for instance
extern fn_(heap_Arena_allocator(heap_Arena* self), mem_Allocator);

/// Initialize with child allocator
extern fn_(heap_Arena_init(mem_Allocator child_allocator), heap_Arena);
/// Deinitialize and free all memory
extern fn_(heap_Arena_fini(heap_Arena self), void);

/// Reset mode for arena reset operation
config_UnionEnum(heap_Arena_ResetMode,
    (heap_Arena_ResetMode_free_all, Void),
    (heap_Arena_ResetMode_retain_capacity, Void),
    (heap_Arena_ResetMode_retain_with_limit, usize)
);

/// Query current memory capacity of arena
extern fn_(heap_Arena_queryCap(const heap_Arena* self), usize);
/// Reset arena with specified mode
extern fn_(heap_Arena_reset(heap_Arena* self, heap_Arena_ResetMode mode), bool);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HEAP_ARENA_INCLUDED */
