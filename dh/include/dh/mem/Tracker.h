/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Tracker.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-09 (date of creation)
 * @updated 2025-01-09 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem_Tracker
 *
 * @brief   Debug memory tracking system
 */

#ifndef MEM_TRACKER_INCLUDED
#define MEM_TRACKER_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include <stdio.h>
#include <time.h>

#include "cfg.h"

/* TODO: Add option 'trace alloc and free no disable release'  */

#if defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !DEBUG_ENABLED
#else

/*========== Memory Tracking Types =========================================*/

typedef struct mem_Allocation mem_Allocation;
typedef struct mem_Tracker    mem_Tracker;

struct mem_Allocation {
    anyptr          ptr;       /* Allocated pointer */
    usize           size;      /* Allocation size */
    const char*     file;      /* Source file */
    i32             line;      /* Source line */
    const char*     func;      /* Function name */
    time_t          timestamp; /* Allocation time */
    mem_Allocation* next;
};

struct mem_Tracker {
    mem_Allocation* allocations;     /* Linked list of allocations */
    FILE*           log_file;        /* Log file handle */
    usize           total_allocated; /* Total bytes allocated */
    usize           active_allocs;   /* Number of active allocations */
};

/*========== Memory Tracker Interface ======================================*/

// Initialize memory tracker
extern void mem_Tracker_initWithFile(const char* log_path);

// Register allocation
extern void mem_Tracker_registerAlloc(
    anyptr      ptr,
    usize       size,
    const char* file,
    i32         line,
    const char* func
);

// Register deallocation
extern void mem_Tracker_registerFree(
    anyptr      ptr,
    const char* file,
    i32         line,
    const char* func
);

// Generate final report and cleanup
extern void mem_Tracker_generateReportAndCleanup(void);

// Get singleton instance
extern mem_Tracker* mem_Tracker_instance(void);

#endif /* defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !DEBUG_ENABLED */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_TRACKER_INCLUDED */
