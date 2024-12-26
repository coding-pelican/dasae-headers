// /**
//  * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
//  *
//  * @file    heap.h
//  * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
//  * @date    2024-12-07 (date of creation)
//  * @updated 2024-12-07 (date of last update)
//  * @version v0.1
//  * @ingroup dasae-headers(dh)
//  * @prefix  heap
//  *
//  * @brief   Heap management interfaces and utilities
//  * @details
//  */

// #ifndef HEAP_INCLUDED
// #define HEAP_INCLUDED (1)
// #if defined(__cplusplus)
// extern "C" {
// #endif /* defined(__cplusplus) */

// /*========== Includes =======================================================*/

// #include "dh/core/prim.h"
// #include "dh/core/ptr.h"
// #include "dh/core/Opt.h"
// #include "dh/core/Res.h"
// #include "dh/core/Err.h"
// #include "dh/debug/assert.h"
// #include "dh/mem/Allocator.h"
// #include "dh/heap/Classic.h"
// #include "dh/heap/Page.h"
// #include "dh/heap/Arena.h"
// #include "dh/heap/FixedBuffer.h"
// #include "dh/heap/GeneralPurpose.h"
// #include "dh/heap/ThrdSafe.h"

// /*========== Heap Interface =================================================*/

// // Forward declarations
// // typedef struct heap_Page           heap_Page;
// // typedef struct heap_Arena          heap_Arena;
// // typedef struct heap_FixedBuffer    heap_FixedBuffer;
// // typedef struct heap_GeneralPurpose heap_GeneralPurpose;
// // typedef struct heap_ThrdSafe       heap_ThrdSafe;

// // // Page allocator interface
// // mem_Allocator heap_Page_allocator(heap_Page* self);
// // Res_Void      heap_Page_init(heap_Page* self);
// // void          heap_Page_fini(heap_Page* self);

// // // Arena allocator interface
// // typedef struct heap_Arena {
// //     mem_Allocator child_allocator;
// //     anyptr        buffer_list;
// //     usize         total_allocated;
// // } heap_Arena;

// // mem_Allocator heap_Arena_allocator(heap_Arena* self);
// // Res_Void      heap_Arena_init(heap_Arena* self, mem_Allocator child_allocator);
// // void          heap_Arena_fini(heap_Arena* self);
// // void          heap_Arena_reset(heap_Arena* self);

// // // Fixed buffer allocator interface
// // typedef struct heap_FixedBuffer {
// //     u8*   buffer;
// //     usize buffer_size;
// //     usize end_index;
// // } heap_FixedBuffer;

// // mem_Allocator heap_FixedBuffer_allocator(heap_FixedBuffer* self);
// // Res_Void      heap_FixedBuffer_init(heap_FixedBuffer* self, u8* buffer, usize size);
// // void          heap_FixedBuffer_reset(heap_FixedBuffer* self);

// // // General Purpose allocator options/config
// // typedef struct heap_GeneralPurposeConfig {
// //     usize stack_trace_frames;
// //     bool  enable_memory_limit;
// //     bool  safety;
// //     bool  thread_safe;
// //     bool  never_unmap;
// //     bool  retain_metadata;
// //     bool  verbose_log;
// // } heap_GeneralPurposeConfig;

// // #define heap_config_default ((heap_GeneralPurposeConfig){ \
// //     .stack_trace_frames  = 6,                             \
// //     .enable_memory_limit = false,                         \
// //     .safety              = true,                          \
// //     .thread_safe         = true,                          \
// //     .never_unmap         = false,                         \
// //     .retain_metadata     = false,                         \
// //     .verbose_log         = false })

// // // Global allocator instances
// // extern mem_Allocator heap_classic_allocator;
// // extern mem_Allocator heap_page_allocator;

// #if defined(__cplusplus)
// } /* extern "C" */
// #endif /* defined(__cplusplus) */
// #endif /* HEAP_INCLUDED */
