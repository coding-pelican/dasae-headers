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
 * @brief   SmpAlctr: A generic multi-threaded allocator with SMP optimization
 * @details This allocator performs high-performance SMP (multi-threaded, cache-friendly)
 *          algorithms regardless of the memory source. It completely eliminates OS dependencies
 *          by accepting a parent allocator at initialization time.
 */
#pragma once
#ifndef heap_Smp__included
#define heap_Smp__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "../thrd/Mtx.h"

/*========== Macros and Declarations ========================================*/

#define heap_Smp_max_thrd_count \
    usize_(heap_Smp__max_thrd_count)
#define heap_Smp_slab_len \
    usize_(heap_Smp__slab_len)
#define heap_Smp_min_size_class /* Because of storing free list pointers, the minimum size class is 3 */ \
    usize_(heap_Smp__min_size_class)
#define heap_Smp_size_class_count \
    usize_(heap_Smp__size_class_count)
#define heap_Smp_max_alloc_search /* Before mapping a fresh page, `alloc` will rotate this many times */ \
    usize_(heap_Smp__max_alloc_search)
enum {
    heap_Smp__max_thrd_count = usize_(128),
    heap_Smp__slab_len = pp_if_(abi_size_is_16bit)(
        pp_then_(u32_(pri_max_static(heap_page_size, u32_(32) * 1024))),
        pp_else_(usize_(pri_max_static(heap_page_size, u32_(64) * 1024)))),
    heap_Smp__min_size_class = usize_(uint_log2_static(sizeOf$(usize))),
    heap_Smp__size_class_count = usize_(uint_log2_static(heap_Smp_slab_len) - heap_Smp_min_size_class),
    heap_Smp__max_alloc_search = usize_(1),
};

typedef struct heap_Smp_ThrdMeta {
    var_(_avoid_false_sharing, Void) $align(arch_cache_line_bytes);
    /// Protects the state in this struct (per-thread state).
    /// Threads lock this before accessing their own state in order
    /// to support freelist reclamation.
    var_(mtx, thrd_Mtx);
    /// For each size class, tracks the next address to be returned from
    /// `alloc` when the freelist is empty.
    var_(next_addrs, A$$(heap_Smp_size_class_count, usize));
    /// For each size class, points to the freed pointer.
    var_(frees, A$$(heap_Smp_size_class_count, usize));
} heap_Smp_ThrdMeta;
T_use_prl$(heap_Smp_ThrdMeta);

typedef struct heap_Smp {
    /// Parent allocator that provides backing memory
    /// Can be any allocator: PageAlctr, SbrkAlctr, FixedAlctr, etc.
    var_(backing_alctr, mem_Alctr);
    /// Per-thread metadata array
    var_(thrd_metas, S$heap_Smp_ThrdMeta);
    /// CPU count (cached for performance)
    var_(cpu_count, u32);
} heap_Smp;
T_use_P$(heap_Smp);
T_use_E$($set(mem_E)(P$heap_Smp));
$extern let_(heap_Smp_vtbl, mem_Alctr_VTbl);
$extern fn_((heap_Smp_alctr(heap_Smp* self))(mem_Alctr));

$extern fn_((heap_Smp_from(mem_Alctr backing_alctr, S$heap_Smp_ThrdMeta thrd_metas))(heap_Smp));
$attr($must_check)
$extern fn_((heap_Smp_createOnHeap(mem_Alctr backing_alctr, usize thrd_meta_count))(mem_E$P$heap_Smp));
$extern fn_((heap_Smp_destroyOnHeap(P$heap_Smp* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_Smp__included */
