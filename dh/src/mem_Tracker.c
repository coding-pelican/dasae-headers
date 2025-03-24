/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    mem_Tracker.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-09 (date of creation)
 * @updated 2025-03-22 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem_Tracker
 *
 * @brief   Debug memory tracking system
 * @details Implementation of memory allocation tracking functionality
 */

#include "dh/mem/Tracker.h"
#include "dh/mem/common.h"
#include "dh/heap/Classic.h"
#include "dh/ArrList.h"
#include "dh/err_res.h"
#include "dh/core/src_loc.h"
#include "dh/fs/dir.h"
#include "dh/io.h"
#include "dh/Str.h"
#include "dh/Arr.h"
#include "dh/time.h"

#if defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !debug_comp_enabled
#else

/*========== Constants and Default Configuration ===========================*/

static const Str_const mem_Tracker_default_log_file = Str_l("log/mem.log");

/*========== Tracker Leak Report Types =====================================*/

/// Individual allocation record
typedef struct mem_Allocation {
    anyptr                 ptr;       /* Allocated pointer */
    usize                  size;      /* Allocation size */
    SrcLoc                 src_loc;   /* Source location*/
    time_Instant           timestamp; /* Allocation time */
    struct mem_Allocation* next;
} mem_Allocation;

/// Leak site for aggregated reporting
typedef struct LeakSite {
    SrcLoc src_loc;
    usize  count;
    usize  total_bytes;
} LeakSite;

/*========== Singleton Instance ============================================*/

static mem_Tracker mem_Tracker_s_instance = cleared();

/// Automatic initialization at program start
static $on_load fn_(mem_Tracker_init(void), void) {
    catch_from(mem_Tracker_initWithPath(mem_Tracker_default_log_file), err, eval({
         /* If initialization fails, try to log to stderr */
        printf("ERROR: Failed to initialize memory tracker: [%s] %s\n",
            Err_domainToCStr(err), Err_codeToCStr(err)
        );
        ErrTrace_print();
    }));
    ignore atexit(mem_Tracker_finiAndGenerateReport);
}

/// Automatic finalization at program exit (will call atexit handler)
static $on_exit fn_(mem_Tracker_fini(void), void) {
    mem_Tracker_finiAndGenerateReport();
}

/*========== Implementation ================================================*/

fn_ext_scope(mem_Tracker_initWithPath(Str_const log_path), Err$void) {
    // Create directory if needed
    let dir_path = Str_l("log");
    try_(fs_dir_create(dir_path));

    // Open log file
    Arr$$(256, u8) path_buf = Arr_zero();
    mem_copy(path_buf.items, log_path.ptr, log_path.len);
    Arr_setAt(path_buf, log_path.len, '\0');

    let log_file = fopen(as$(const char*, path_buf.items), "w");
    if (!log_file) { return_err(io_FileErr_OpenFailed()); }
    errdefer_(ignore fclose(log_file));

    // Close previous log file if it exists
    if (mem_Tracker_s_instance.log_file) {
        ignore fclose(mem_Tracker_s_instance.log_file);
    }

    // Set up the tracker instance
    mem_Tracker_s_instance.log_file        = log_file;
    mem_Tracker_s_instance.allocations     = null;
    mem_Tracker_s_instance.total_allocated = 0;
    mem_Tracker_s_instance.active_allocs   = 0;

    // clang-format off
    // Write header
    ignore fprintf(mem_Tracker_s_instance.log_file, "Memory Tracker Initialized at %f\n",
        time_Duration_asSecs_f64(time_Instant_elapsed(time_Instant_now()))
    );
    ignore fprintf(mem_Tracker_s_instance.log_file, "================================\n");
    // clang-format on
    return_void();
} ext_unscoped;

fn_ext_scope(mem_Tracker_finiAndGenerateReport(void), void) {
    if (!mem_Tracker_s_instance.log_file) { return; }

    // clang-format off
    ignore fprintf(mem_Tracker_s_instance.log_file, "\nMemory Leak Report\n");
    ignore fprintf(mem_Tracker_s_instance.log_file, "=====================================\n");
    ignore fprintf(mem_Tracker_s_instance.log_file, "Total allocations: %zu bytes\n",
        mem_Tracker_s_instance.total_allocated
    );
    ignore fprintf(mem_Tracker_s_instance.log_file, "Active allocations: %zu\n",
        mem_Tracker_s_instance.active_allocs
    );
    // clang-format on

    if (mem_Tracker_s_instance.active_allocs > 0) {
        ignore fprintf(mem_Tracker_s_instance.log_file, "\nDetected Memory Leaks:\n");
        ignore fprintf(mem_Tracker_s_instance.log_file, "=====================================\n");

        // Get current time for age calculations
        time_Instant now = time_Instant_now();

        // Track all leaks
        mem_Allocation* curr         = mem_Tracker_s_instance.allocations;
        usize           leak_count   = 0;
        usize           total_leaked = 0;

        // Create a dynamic array for leak sites
        var sites = type$(ArrList$$(LeakSite), ArrList_init(typeInfo$(LeakSite), heap_Classic_allocator(create$(heap_Classic))));
        defer_(ArrList_fini(sites.base));

        // Process each leak
        while (curr) {
            leak_count++;
            total_leaked += curr->size;

            // Calculate age of leak
            time_Duration age      = time_Instant_durationSince(now, curr->timestamp);
            f64           age_secs = time_Duration_asSecs_f64(age);

            // Log individual leak
            ignore fprintf(mem_Tracker_s_instance.log_file, "Leak #%zu:\n", leak_count);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Address: %p\n", curr->ptr);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Size: %zu bytes\n", curr->size);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Location: %s:%d\n", curr->src_loc.file_name, curr->src_loc.line);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Function: %s\n", curr->src_loc.fn_name);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Age: %.2f seconds\n", age_secs);

            // Find or add to leak sites
            bool found = false;
            for_slice_indexed (sites.items, site_ptr, i) {
                LeakSite* site = site_ptr;

                // Check if it's the same location
                if (Str_eql(Str_viewZ(as$(u8*, site->src_loc.file_name)), Str_viewZ(as$(u8*, curr->src_loc.file_name)))
                    && site->src_loc.line == curr->src_loc.line
                    && Str_eql(Str_viewZ(as$(u8*, site->src_loc.fn_name)), Str_viewZ(as$(u8*, curr->src_loc.fn_name)))) {

                    site->count++;
                    site->total_bytes += curr->size;
                    found = true;
                    break;
                }
            }

            if (!found) {
                // Add new leak site
                catch_(ArrList_addBackOne(sites.base), ({ ignore fprintf(mem_Tracker_s_instance.log_file, "ERROR: Failed to track leak site\n");
                }));

                if_some (ArrList_popOrNull(sites.base), site_ptr) {
                    let site          = meta_cast$(LeakSite*, site_ptr);
                    site->src_loc     = curr->src_loc;
                    site->count       = 1;
                    site->total_bytes = curr->size;
                }
            }

            curr = curr->next;
        }

        // Print leak summary by allocation site
        ignore fprintf(mem_Tracker_s_instance.log_file, "\nLeak Summary by Location:\n");
        ignore fprintf(mem_Tracker_s_instance.log_file, "=====================================\n");

        for_slice (sites.items, site) {
            ignore fprintf(
                mem_Tracker_s_instance.log_file,
                "Location: %s:%d in %s\n"
                "  Count: %zu leaks\n"
                "  Total bytes: %zu\n\n",
                site->src_loc.file_name,
                site->src_loc.line,
                site->src_loc.fn_name,
                site->count,
                site->total_bytes
            );
        }

        ignore fprintf(mem_Tracker_s_instance.log_file, "\nTotal leaked memory: %zu bytes\n", total_leaked);
    }

    // Cleanup the tracker's linked list of allocations
    mem_Allocation* curr = mem_Tracker_s_instance.allocations;
    while (curr) {
        mem_Allocation* next = curr->next;
        free(curr);
        curr = next;
    }

    ignore fclose(mem_Tracker_s_instance.log_file);
    mem_Tracker_s_instance.log_file    = null;
    mem_Tracker_s_instance.allocations = null;
} ext_unscoped;

fn_(mem_Tracker_registerAlloc(anyptr ptr, usize size, SrcLoc src_loc), void) {
    if (!ptr || !mem_Tracker_s_instance.log_file) { return; }

    // Create new allocation record
    let alloc = as$(mem_Allocation*, malloc(sizeof(mem_Allocation)));
    if (!alloc) {
        // clang-format off
        ignore fprintf(mem_Tracker_s_instance.log_file, "Failed to allocate memory for tracker at %s:%d\n",
            src_loc.file_name, src_loc.line
        );
        // clang-format on
        return;
    }

    // Fill allocation info
    alloc->ptr       = ptr;
    alloc->size      = size;
    alloc->src_loc   = src_loc;
    alloc->timestamp = time_Instant_now();

    // Add to linked list
    alloc->next                        = mem_Tracker_s_instance.allocations;
    mem_Tracker_s_instance.allocations = alloc;

    // Update stats
    mem_Tracker_s_instance.total_allocated += size;
    mem_Tracker_s_instance.active_allocs++;

    // clang-format off
    // Log allocation with total bytes
    ignore fprintf(mem_Tracker_s_instance.log_file, "ALLOC: %p (%zu bytes) at %s:%d in %s (Total: %zu bytes)\n",
        ptr, size, src_loc.file_name, src_loc.line, src_loc.fn_name, mem_Tracker_s_instance.total_allocated
    );
    // clang-format on
}

fn_(mem_Tracker_registerRemap(anyptr old_ptr, anyptr new_ptr, usize new_size, SrcLoc src_loc), void) {
    if (!mem_Tracker_s_instance.log_file) { return; }

    // First, find the old allocation
    usize old_size = 0;
    if (old_ptr) {
        mem_Allocation* curr = mem_Tracker_s_instance.allocations;
        while (curr) {
            if (curr->ptr == old_ptr) {
                old_size = curr->size;
                break;
            }
            curr = curr->next;
        }
    }

    // clang-format off
    // Log the remap operation
    ignore fprintf(mem_Tracker_s_instance.log_file, "REMAP: %p (%zu bytes) -> %p (%zu bytes) at %s:%d in %s\n",
        old_ptr, old_size, new_ptr, new_size, src_loc.file_name, src_loc.line, src_loc.fn_name
    );
    // clang-format on

    // If we found the old allocation, update it
    if (0 < old_size) {
        // For remap, remove the old allocation and add a new one
        mem_Tracker_registerFree(old_ptr, src_loc);
    }

    // Register the new allocation if it's valid
    if (new_ptr) {
        mem_Tracker_registerAlloc(new_ptr, new_size, src_loc);
    }
}

fn_(mem_Tracker_registerFree(anyptr ptr, SrcLoc src_loc), bool) {
    if (!ptr || !mem_Tracker_s_instance.log_file) { return false; }

    mem_Allocation** curr = &mem_Tracker_s_instance.allocations;

    // Search for the allocation
    while (*curr && (*curr)->ptr != ptr) {
        curr = &(*curr)->next;
    }

    if (!*curr) {
        // clang-format off
        // Double free or invalid free detected
        ignore fprintf(mem_Tracker_s_instance.log_file, "ERROR: DOUBLE FREE or INVALID FREE of %p at %s:%d in %s\n",
            ptr, src_loc.file_name, src_loc.line, src_loc.fn_name
        );
        // clang-format on
        return false;
    }

    // Calculate elapsed time since allocation
    let elapsed     = time_Instant_durationSince(time_Instant_now(), (*curr)->timestamp);
    let elapsed_sec = time_Duration_asSecs_f64(elapsed);

    // Get the original allocation info
    let orig_size    = (*curr)->size;
    let orig_src_loc = (*curr)->src_loc;

    // Update stats
    mem_Tracker_s_instance.total_allocated -= orig_size;
    mem_Tracker_s_instance.active_allocs--;

    // clang-format off
    // Log deallocation with details
    ignore fprintf(mem_Tracker_s_instance.log_file,
        "FREE: %p (%zu bytes) at %s:%d in %s\n"
        "      Originally allocated at %s:%d in %s (%.2f seconds ago)\n"
        "      (Total remaining: %zu bytes)\n",
        ptr, orig_size, src_loc.file_name, src_loc.line, src_loc.fn_name,
        orig_src_loc.file_name, orig_src_loc.line, orig_src_loc.fn_name, elapsed_sec,
        mem_Tracker_s_instance.total_allocated
    );
    // clang-format on

    // Remove from list
    mem_Allocation* to_free = *curr;
    *curr                   = to_free->next;
    free(to_free);
    return true;
}

fn_(mem_Tracker_instance(void), mem_Tracker*) {
    return &mem_Tracker_s_instance;
}

#endif /* defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !debug_comp_enabled */
