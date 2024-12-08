// /**
//  * @file heap/GeneralPurpose.h
//  * @brief General Purpose Allocator implementation
//  * @copyright Copyright 2024. All rights reserved.
//  */

// #ifndef HEAP_GENERAL_PURPOSE_INCLUDED
// #define HEAP_GENERAL_PURPOSE_INCLUDED (1)
// #if defined(__cplusplus)
// extern "C" {
// #endif /* defined(__cplusplus) */

// #include "dh/core/prim.h"
// #include "dh/core/ptr.h"
// #include "dh/core/Opt.h"
// #include "dh/core/Res.h"
// #include "dh/core/Err.h"
// #include "dh/debug/assert.h"
// #include "dh/mem.h"
// #include "dh/mem/Allocator.h"

// // Constants
// #define HEAP_GENERAL_PURPOSE_SMALLEST_BUCKET_SIZE_LOG2 3
// #define HEAP_GENERAL_PURPOSE_LARGEST_BUCKET_SIZE_LOG2  (math_log2_int(sizeof(usize), mem_page_size) - 1)

// // Bucket types
// typedef struct heap_GeneralPurpose_Bucket {
//     u8*   page;         // Page of memory for allocations
//     usize alloc_cursor; // Current allocation position
//     usize used_count;   // Number of used slots
// } heap_GeneralPurpose_Bucket;

// typedef struct heap_GeneralPurpose_BucketGroup {
//     heap_GeneralPurpose_Bucket* current;      // Current bucket
//     heap_GeneralPurpose_Bucket* buckets;      // Array of all buckets
//     usize                       bucket_count; // Number of buckets
// } heap_GeneralPurpose_BucketGroup;

// // Main allocator type
// typedef struct heap_GeneralPurpose {
//     mem_Allocator                    backing_allocator;      // Backing allocator instance
//     heap_GeneralPurpose_BucketGroup* groups;                 // Array of bucket groups per size class
//     usize                            group_count;            // Number of bucket groups
//     bool                             retain_metadata;        // Keep metadata for freed memory
//     bool                             never_unmap;            // Keep freed memory mapped
//     usize                            requested_memory_limit; // Limit on total memory allocations
// } heap_GeneralPurpose;

// // Get allocator interface
// mem_Allocator heap_GeneralPurpose_allocator(heap_GeneralPurpose* self);

// // Initialization and finalization
// Res_Void heap_GeneralPurpose_init(heap_GeneralPurpose* self, mem_Allocator backing_allocator, bool retain_metadata, bool never_unmap);
// void     heap_GeneralPurpose_fini(heap_GeneralPurpose* self);

// // Core allocation functions
// Res_Mptr_u8 heap_GeneralPurpose_alloc(heap_GeneralPurpose* self, usize size, usize alignment);
// bool        heap_GeneralPurpose_resize(heap_GeneralPurpose* self, Slice_u8 buf, usize log2_buf_align, usize new_size);
// void        heap_GeneralPurpose_free(heap_GeneralPurpose* self, Slice_u8 buf, usize log2_buf_align);

// // Helper functions
// void heap_GeneralPurpose_reset(heap_GeneralPurpose* self);
// void heap_GeneralPurpose_setRequestedMemoryLimit(heap_GeneralPurpose* self, usize limit);
// bool heap_GeneralPurpose_detectLeaks(heap_GeneralPurpose* self);

// #if defined(__cplusplus)
// } /* extern "C" */
// #endif /* defined(__cplusplus) */
// #endif /* HEAP_GENERAL_PURPOSE_INCLUDED */
