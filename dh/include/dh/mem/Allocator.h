/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Allocator.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-07 (date of creation)
 * @updated 2024-12-10 (date of last update)
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
#include "dh/generic.h"

/*========== Allocator Interface ============================================*/

// typedef void* Ptr;
// typedef struct Sli {
//     Ptr   ptr;
//     usize len;
// } Sli;
using_Opt(Ptr);
using_Opt(Sli);

using_Opt(u8);
typedef union Sli$u8 {
    Sli base;
    struct {
        u8*   ptr;
        usize len;
    };
} Sli$u8;

typedef struct mem_Allocator   mem_Allocator;
typedef struct mem_AllocatorVT mem_AllocatorVT;

// Allocator vtable
struct mem_AllocatorVT {
    // Allocate memory with size/alignment from PtrBase
    // Returns Optptr$u8 for allocation result
    Optptr$u8 (*alloc)(anyptr ctx, usize len, usize ptr_align);

    // Try to resize existing allocation - returns success/failure
    bool (*resize)(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_len);

    // Free allocation using pointer's metadata
    void (*free)(anyptr ctx, Sli$u8 buf, usize buf_align);
};
// Allocator instance
struct mem_Allocator {
    anyptr                 ctx; // Context pointer with type info
    const mem_AllocatorVT* vt;  // Virtual table
};

impl_Err(
    mem_AllocErr,
    OutOfMemory
);

/*========== Core Allocator Functions =======================================*/

/* Raw allocation */
Optptr$u8 mem_Allocator_rawAlloc(mem_Allocator self, usize len, usize ptr_align);
/* Try to resize in-place */
bool      mem_Allocator_rawResize(mem_Allocator self, Sli$u8 buf, usize buf_align, usize new_len);
/* Free memory */
void      mem_Allocator_rawFree(mem_Allocator self, Sli$u8 buf, usize buf_align);

/*========== High-level Allocator Functions =================================*/

decl_Err$(Ptr);
decl_Err$(Sli);
/* Single-item allocation */
extern Err$Ptr mem_Allocator_create(mem_Allocator self, TypeInfo type);
/* Free single-item */
extern void    mem_Allocator_destroy(mem_Allocator self, Ptr ptr);
/* Sli allocation */
extern Err$Sli mem_Allocator_alloc(mem_Allocator self, TypeInfo type, usize count);
/* Try to resize slice in-place */
extern bool    mem_Allocator_resize(mem_Allocator self, Sli* sli, usize new_len);
/* Reallocate slice with new size */
extern Opt$Sli mem_Allocator_realloc(mem_Allocator self, Sli sli, usize new_len);
/* Free slice */
extern void    mem_Allocator_free(mem_Allocator self, Sli sli);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_ALLOCATOR_INCLUDED */
