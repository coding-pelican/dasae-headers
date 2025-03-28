/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Tracker.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-09 (date of creation)
 * @updated 2025-03-22 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem_Tracker
 *
 * @brief   Debug memory tracking system
 * @details Tracks memory allocations, frees, remaps, and detects issues like
 *          memory leaks, double frees, and invalid frees. Provides detailed
 *          reports with allocation source locations and timestamps.
 */

#ifndef MEM_TRACKER_INCLUDED
#define MEM_TRACKER_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/Str.h"
#include "dh/err_res.h"
#include "dh/core/src_loc.h"

/* TODO: Add option 'trace alloc and free no disable release'  */
#if defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !debug_comp_enabled
#else

/*========== Memory Tracking Types =========================================*/

typedef struct mem_Tracker {
    struct mem_Allocation* allocations;     /* Linked list of allocations */
    FILE*                  log_file;        /* Log file handle */
    usize                  total_allocated; /* Total bytes allocated */
    usize                  active_allocs;   /* Number of active allocations */
} mem_Tracker;

/*========== Memory Tracker Interface ======================================*/

/// Initialize memory tracker with custom log path
extern fn_(mem_Tracker_initWithPath(Str_const log_path), $must_check Err$void);
/// Generate final report and cleanup
extern fn_(mem_Tracker_finiAndGenerateReport(void), void);

/// Register allocation (e.g., alloc, create)
extern fn_(mem_Tracker_registerAlloc(
    anyptr      ptr,
    usize       size,
    SrcLoc      src_loc
), void);
/// Register memory remapping (e.g., resize/realloc)
extern fn_(mem_Tracker_registerRemap(
    anyptr      old_ptr,
    anyptr      new_ptr,
    usize       new_size,
    SrcLoc      src_loc
), void);
/// Register deallocation (e.g., free, destroy)
extern fn_(mem_Tracker_registerFree(
    anyptr      ptr,
    SrcLoc      src_loc
), bool);

/// Get singleton instance
extern fn_(mem_Tracker_instance(void), mem_Tracker*);

#endif /* defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !debug_comp_enabled */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_TRACKER_INCLUDED */
