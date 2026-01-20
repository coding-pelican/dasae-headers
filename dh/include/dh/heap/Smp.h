/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Smp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-19 (date of creation)
 * @updated 2026-01-19 (date of last update)
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Smp
 *
 * @brief   SmpAllocator: A generic multi-threaded allocator with SMP optimization
 * @details This allocator performs high-performance SMP (multi-threaded, cache-friendly)
 *          algorithms regardless of the memory source. It completely eliminates OS dependencies
 *          by accepting a parent allocator at initialization time.
 */
#ifndef heap_Smp__included
#define heap_Smp__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/Thrd/Mtx.h"

/*========== Macros and Declarations ========================================*/

#define heap_Smp_max_thrd_count \
    128
#define heap_Smp_slab_len \
    prim_max_static(heap_page_size, 64 * 1024)
#define heap_Smp_min_class /* Because of storing free list pointers, the minimum size class is 3 */ \
    uint_log2_static(sizeOf$(usize))
#define heap_Smp_size_class_count \
    (uint_log2_static(heap_Smp_slab_len) - heap_Smp_min_class)
#define heap_Smp_max_alloc_search /* Before mapping a fresh page, `alloc` will rotate this many times */ \
    1

typedef struct heap_Smp_ThrdMeta {
    var_(_avoid_false_sharing, Void) $align(arch_cache_line_bytes);
    /// Protects the state in this struct (per-thread state).
    /// Threads lock this before accessing their own state in order
    /// to support freelist reclamation.
    var_(mtx, Thrd_Mtx);
    /// For each size class, tracks the next address to be returned from
    /// `alloc` when the freelist is empty.
    var_(next_addrs, A$$(heap_Smp_size_class_count, usize));
    /// For each size class, points to the freed pointer.
    var_(frees, A$$(heap_Smp_size_class_count, usize));
} heap_Smp_ThrdMeta;
T_use_prl$(heap_Smp_ThrdMeta);

typedef struct heap_Smp {
    /// Parent allocator that provides backing memory
    /// Can be any allocator: PageAllocator, SbrkAllocator, FixedAllocator, etc.
    var_(backing_allocator, mem_Allocator);
    /// Per-thread metadata array
    var_(thrd_metas, S$heap_Smp_ThrdMeta);
    /// CPU count (cached for performance)
    var_(cpu_count, u32);
} heap_Smp;
T_use_P$(heap_Smp);
T_use_E$($set(mem_Err)(P$heap_Smp));
$extern let_(heap_Smp_vt, mem_Allocator_VT);
$extern fn_((heap_Smp_allocator(heap_Smp* self))(mem_Allocator));
$extern fn_((heap_Smp_from(mem_Allocator backing_allocator, S$heap_Smp_ThrdMeta thrd_metas))(heap_Smp));
$attr($must_check)
$extern fn_((heap_Smp_createOnHeap(mem_Allocator backing_allocator, usize thrd_meta_count))(mem_Err$P$heap_Smp));
$extern fn_((heap_Smp_destroyOnHeap(P$heap_Smp* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_Smp__included */
