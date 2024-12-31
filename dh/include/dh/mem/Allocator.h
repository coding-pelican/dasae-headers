/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Allocator.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-07 (date of creation)
 * @updated 2024-12-31 (date of last update)
 * @version v0.2
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem_Allocator
 *
 * @brief   Memory allocator interface using smart pointers
 */

#ifndef MEM_ALLOCATOR_INCLUDED
#define MEM_ALLOCATOR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/meta/common.h"

/*========== Allocator Interface ============================================*/

/* Allocator vtable */
typedef struct mem_AllocatorVT mem_AllocatorVT;
struct mem_AllocatorVT {
    must_check Opt$Ptr$u8 (*alloc)(anyptr ctx, usize len, usize ptr_align);
    must_check bool (*resize)(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_len);
    void (*free)(anyptr ctx, Sli$u8 buf, usize buf_align);
};

/* Allocator instance */
typedef struct mem_Allocator mem_Allocator;
struct mem_Allocator {
    anyptr                 ctx; /* Context pointer with type info */
    const mem_AllocatorVT* vt;  /* Virtual table */
};

/*========== Core Allocator Functions =======================================*/

/* Raw allocation */
extern must_check Opt$Ptr$u8 mem_Allocator_rawAlloc(mem_Allocator self, usize len, usize ptr_align);
/* Try to resize in-place */
extern must_check bool       mem_Allocator_rawResize(mem_Allocator self, Sli$u8 buf, usize buf_align, usize new_len);
/* Free memory */
extern void                  mem_Allocator_rawFree(mem_Allocator self, Sli$u8 buf, usize buf_align);

/*========== High-level Allocator Functions =================================*/

/* Single-item allocation */
extern must_check Err$meta_Ptr mem_Allocator_create(mem_Allocator self, TypeInfo type);
/* Free single-item */
extern void                    mem_Allocator_destroy(mem_Allocator self, AnyType ptr);
/* Sli allocation */
extern must_check Err$meta_Sli mem_Allocator_alloc(mem_Allocator self, TypeInfo type, usize count);
/* Try to resize slice in-place */
extern must_check bool         mem_Allocator_resize(mem_Allocator self, AnyType old_mem, usize new_len);
/* Reallocate slice with new size */
extern must_check Opt$meta_Sli mem_Allocator_realloc(mem_Allocator self, AnyType old_mem, usize new_len);
/* Free slice */
extern void                    mem_Allocator_free(mem_Allocator self, AnyType memory);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_ALLOCATOR_INCLUDED */
