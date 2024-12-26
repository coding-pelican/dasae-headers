// /**
//  * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
//  *
//  * @file    Allocator.h
//  * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
//  * @date    2024-12-07 (date of creation)
//  * @updated 2024-12-10 (date of last update)
//  * @version v0.2
//  * @ingroup dasae-headers(dh)/mem
//  * @prefix  mem_Allocator
//  *
//  * @brief   Memory allocator interface using smart pointers
//  */

// #ifndef MEM_ALLOCATOR_INCLUDED
// #define MEM_ALLOCATOR_INCLUDED (1)
// #if defined(__cplusplus)
// extern "C" {
// #endif /* defined(__cplusplus) */

// /*========== Includes =======================================================*/

// #include "cfg.h"
// #include "dh/ext_types.h"

// /*========== Allocator Interface ============================================*/

// typedef struct mem_Allocator        mem_Allocator;
// typedef struct mem_Allocator_VTable mem_Allocator_VTable;
// decl_ExtType(mem_Allocator);
// decl_ExtType(mem_Allocator_VTable);

// // Allocator vtable
// struct mem_Allocator_VTable {
//     // Allocate memory with size/alignment from PtrBase
//     // Returns Opt_Ptr_u8 for allocation result
//     Opt_Ptr_u8 (*alloc)(anyptr ctx, usize len, usize ptr_align);

//     // Try to resize existing allocation - returns success/failure
//     bool (*resize)(anyptr ctx, Sli_u8 buf, usize buf_align, usize new_len);

//     // Free allocation using pointer's metadata
//     void (*free)(anyptr ctx, Sli_u8 buf, usize buf_align);
// };
// impl_ExtType(mem_Allocator_VTable);

// // Allocator instance
// struct mem_Allocator {
//     anyptr                        ctx; // Context pointer with type info
//     PtrConst_mem_Allocator_VTable vt;  // Virtual table
// };
// impl_ExtType(mem_Allocator);

// /*========== Core Allocator Functions =======================================*/

// /* Raw allocation */
// Opt_Ptr_u8 mem_Allocator_rawAlloc(mem_Allocator self, usize len, usize ptr_align);
// /* Try to resize in-place */
// bool       mem_Allocator_rawResize(mem_Allocator self, Sli_u8 buf, usize buf_align, usize new_len);
// /* Free memory */
// void       mem_Allocator_rawFree(mem_Allocator self, Sli_u8 buf, usize buf_align);

// /*========== High-level Allocator Functions =================================*/

// /* Single-item allocation */
// extern ResErr_Ptr mem_Allocator_create(mem_Allocator self, TypeInfo type);
// /* Free single-item */
// extern void       mem_Allocator_destroy(mem_Allocator self, Ptr ptr);
// /* Sli allocation */
// extern ResErr_Sli mem_Allocator_alloc(mem_Allocator self, TypeInfo type, usize count);
// /* Try to resize slice in-place */
// extern bool       mem_Allocator_resize(mem_Allocator self, Ptr_Sli sli, usize new_len);
// /* Reallocate slice with new size */
// extern Opt_Sli    mem_Allocator_realloc(mem_Allocator self, Sli sli, usize new_len);
// /* Free slice */
// extern void       mem_Allocator_free(mem_Allocator self, Sli sli);

// #if defined(__cplusplus)
// } /* extern "C" */
// #endif /* defined(__cplusplus) */
// #endif /* MEM_ALLOCATOR_INCLUDED */
