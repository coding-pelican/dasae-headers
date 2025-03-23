/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Arena.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-15 (date of creation)
 * @updated 2025-01-15 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Arena
 *
 * @brief   Arena allocator that allows batch deallocation
 * @details Takes an existing allocator, wraps it, and provides an interface
 *          where you can allocate without freeing, and then free it all together.
 */

/* TODO: Implement this header interface */

#ifndef HEAP_ARENA_INCLUDED
#define HEAP_ARENA_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Buffer List Node =============================================*/

/* Node for the linked list of memory buffers */
typedef struct heap_Arena_BufNode heap_Arena_BufNode;
use_Ptr(heap_Arena_BufNode);
use_Opt$(Ptr$heap_Arena_BufNode);

struct heap_Arena_BufNode {
    Opt$Ptr$heap_Arena_BufNode next;
    usize                      len; // Total buffer length including node
};

/*========== Arena State ================================================*/

/* Inner state of ArenaAllocator. Can be stored separately as an optimization */
typedef struct heap_Arena_State {
    Opt$Ptr$heap_Arena_BufNode first;     // First buffer in list
    usize                      end_index; // Current allocation position
} heap_Arena_State;
/* Inner state promoter function */
extern struct heap_Arena heap_Arena_State_promote(heap_Arena_State* self, mem_Allocator child);

/*========== Arena Allocator ============================================*/

typedef struct heap_Arena {
    mem_Allocator    child; // Underlying allocator
    heap_Arena_State state; // Arena state
} heap_Arena;
extern mem_Allocator heap_Arena_allocator(heap_Arena* self);

// Lifecycle
extern heap_Arena heap_Arena_init(mem_Allocator child);
extern void       heap_Arena_fini(heap_Arena* self);

/*========== Arena Operations ===========================================*/

/* Modes for resetting the arena */
enum {
    heap_Arena_ResetMode_free_all     = 0, // Free all memory
    heap_Arena_ResetMode_retain_cap   = 1, // Keep memory for future use
    heap_Arena_ResetMode_retain_limit = 2, // Keep memory up to limit
};
typedef union heap_Arena_ResetMode {
    struct {
        i32 tag;
    };
    struct {
        i32   tag;
        usize limit;
    } retain_limit;
} heap_Arena_ResetMode;

/* Query current arena capacity */
extern usize heap_Arena_queryCap(const heap_Arena* self);

/* Reset arena and optionally retain capacity */
extern bool heap_Arena_reset(heap_Arena* self, heap_Arena_ResetMode mode);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HEAP_ARENA_INCLUDED */
