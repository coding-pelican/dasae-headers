// /**
//  * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
//  *
//  * @file    Classic.h
//  * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
//  * @date    2024-12-07 (date of creation)
//  * @updated 2024-12-23 (date of last update)
//  * @version v0.2
//  * @ingroup dasae-headers(dh)/heap
//  * @prefix  heap_Classic
//  *
//  * @brief   Classic C standard library allocator wrapper using smart pointers
//  */

// #ifndef HEAP_CLASSIC_INCLUDED
// #define HEAP_CLASSIC_INCLUDED (1)
// #if defined(__cplusplus)
// extern "C" {
// #endif /* defined(__cplusplus) */

// /*========== Includes =======================================================*/

// #include "cfg.h"
// #include <stdlib.h>

// /*========== Classic C Allocator ============================================*/

// // Classic allocator instance (minimal state)
// typedef struct heap_Classic {
//     Void unused_[1]; // Empty struct not allowed in C
// } heap_Classic;
// using_ExtTypes(heap_Classic);

// // Get allocator interface for instance
// extern mem_Allocator heap_Classic_allocator(void);

// // Lifecycle
// extern ResErr_Void heap_Classic_init(Ptr_mem_Allocator self);
// extern void        heap_Classic_fini(Ptr_mem_Allocator self);

// // VTable implementations
// extern Opt_Ptr_u8 heap_Classic_alloc(anyptr ctx, usize len, usize ptr_align);
// extern bool       heap_Classic_resize(anyptr ctx, Sli_u8 buf, usize buf_align, usize new_len);
// extern void       heap_Classic_free(anyptr ctx, Sli_u8 buf, usize buf_align);

// // VTable for Classic allocator
// static const PtrConst_mem_Allocator_VTable heap_Classic_vt = {
//     .addr = &(mem_Allocator_VTable){
//         .alloc  = heap_Classic_alloc,
//         .resize = heap_Classic_resize,
//         .free   = heap_Classic_free,
//     },
// };

// /*========== Implementation =================================================*/

// // Get underlying malloc_size if available
// #if defined(__GLIBC__) || defined(__APPLE__)
// #define heap_Classic_has_malloc_size (1)
// force_inline usize heap_Classic_mallocSize(anyptr ptr) {
// #if defined(__GLIBC__)
//     return malloc_usable_size(ptr);
// #else
//     return malloc_size(ptr);
// #endif
// }
// #else
// #define heap_Classic_has_malloc_size (0)
// #endif

// #if defined(__cplusplus)
// } /* extern "C" */
// #endif /* defined(__cplusplus) */
// #endif /* HEAP_CLASSIC_INCLUDED */
