/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Tracker.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
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

#ifndef mem_Tracker__included
#define mem_Tracker__included 1
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
extern fn_((mem_Tracker_initWithPath(S_const$u8 log_path))(E$void)) $must_check;
/// Generate final report and cleanup
extern fn_((mem_Tracker_finiAndGenerateReport(void))(void));

/// Register allocation (e.g., alloc, create)
extern fn_((mem_Tracker_registerAlloc(P$raw ptr, usize size, SrcLoc src_loc))(void));
/// Register memory remapping (e.g., resize/realloc)
extern fn_((mem_Tracker_registerRemap(P$raw old_ptr, P$raw new_ptr, usize new_size, SrcLoc src_loc))(void));
/// Register deallocation (e.g., free, destroy)
extern fn_((mem_Tracker_registerFree(P$raw ptr, SrcLoc src_loc))(bool));

/// Get singleton instance
extern fn_((mem_Tracker_instance(void))(mem_Tracker*));

#endif /* defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !debug_comp_enabled */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_Tracker__included */
