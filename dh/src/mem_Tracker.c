#include "dh/mem/Tracker.h"
#if defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !debug_comp_enabled
#else
#include "dh/ArrList.h"
#include "dh/heap/Classic.h"
#include "dh/mem/common.h"
#include "dh/fs/Dir.h"
#include "dh/io.h"
#include "dh/Str.h"
#include "dh/time.h"
#include <stdio.h> /* TODO: Remove this dependency */
#include <stdlib.h> /* TODO: Remove this dependency */

/*========== Constants and Default Configuration ===========================*/

static const S_const$u8 mem_Tracker_default_log_file = u8_l(".log/mem.log");

/*========== Tracker Leak Report Types =====================================*/

/// Individual allocation record
typedef struct mem_Allocation {
    P$raw ptr; /* Allocated pointer */
    usize size; /* Allocation size */
    SrcLoc src_loc; /* Source location*/
    time_Instant timestamp; /* Allocation time */
    struct mem_Allocation* next;
} mem_Allocation;

/// Leak site for aggregated reporting
typedef struct LeakSite {
    SrcLoc src_loc;
    usize count;
    usize total_bytes;
} LeakSite;

/*========== Singleton Instance ============================================*/

static mem_Tracker mem_Tracker_s_instance = cleared();

/// Automatic initialization at program start
$attr($on_load)
$static fn_((mem_Tracker__init(void))(void)) {
    catch_((mem_Tracker_initWithPath(mem_Tracker_default_log_file))(err, {
        /* If initialization fails, try to log to stderr */
        printf("ERROR: Failed to initialize memory tracker: [%s] %s\n", Err_domainToCStr(err), Err_codeToCStr(err));
        ErrTrace_print();
    }));
    let_ignore = atexit(mem_Tracker_finiAndGenerateReport);
}

/// Automatic finalization at program exit (will call atexit handler)
$attr($on_exit)
$static fn_((mem_Tracker__fini(void))(void)) {
    mem_Tracker_finiAndGenerateReport();
}

/*========== Implementation ================================================*/

fn_((mem_Tracker_initWithPath(S_const$u8 log_path))(E$void) $guard) {
    // Create directory if needed
    let dir_path = u8_l(".log");
    try_(fs_Dir_create(dir_path));

    // Open log file
    A$$(256, u8) path_str = zero$A();
    mem_copyBytes(A_ref$((S$u8)path_str), log_path);
    *at$A(path_str, log_path.len) = '\0';

    let log_file = fopen(as$(const char*)(path_str.val), "w");
    if (!log_file) { return_err(fs_File_Err_OpenFailed()); }
    errdefer_($ignore, let_ignore = fclose(log_file));

    // Close previous log file if it exists
    if (mem_Tracker_s_instance.log_file) {
        let_ignore = fclose(mem_Tracker_s_instance.log_file);
    }

    // Set up the tracker instance
    mem_Tracker_s_instance.log_file = log_file;
    mem_Tracker_s_instance.allocations = null;
    mem_Tracker_s_instance.total_allocated = 0;
    mem_Tracker_s_instance.active_allocs = 0;

    static heap_Classic s_heap_ctx = cleared();
    mem_Tracker_s_instance.gpa = heap_Classic_allocator(&s_heap_ctx);

    // clang-format off
    // Write header
    let_ignore = fprintf(mem_Tracker_s_instance.log_file, "Memory Tracker Initialized at %f\n",
        time_Duration_asSecs$f64(time_Instant_elapsed(time_Instant_now()))
    );
    let_ignore = fprintf(mem_Tracker_s_instance.log_file, "================================\n");
    // clang-format on
    return_ok({});
} $unguarded_(fn);

T_use$((LeakSite)(P, S, O));
T_use_E$($set(mem_Err)(P$LeakSite));
T_use$((LeakSite)(ArrList, ArrList_init, ArrList_fini, ArrList_addBack));

fn_((mem_Tracker_finiAndGenerateReport(void))(void) $guard) {
    if (!mem_Tracker_s_instance.log_file) { return; }

    // clang-format off
    let_ignore = fprintf(mem_Tracker_s_instance.log_file, "\nMemory Leak Report\n");
    let_ignore = fprintf(mem_Tracker_s_instance.log_file, "=====================================\n");
    let_ignore = fprintf(mem_Tracker_s_instance.log_file, "Total allocations: %zu bytes\n",
        mem_Tracker_s_instance.total_allocated
    );
    let_ignore = fprintf(mem_Tracker_s_instance.log_file, "Active allocations: %zu\n",
        mem_Tracker_s_instance.active_allocs
    );
    // clang-format on

    if (mem_Tracker_s_instance.active_allocs > 0) {
        let_ignore = fprintf(mem_Tracker_s_instance.log_file, "\nDetected Memory Leaks:\n");
        let_ignore = fprintf(mem_Tracker_s_instance.log_file, "=====================================\n");

        // Get current time for age calculations
        time_Instant now = time_Instant_now();

        // Track all leaks
        mem_Allocation* curr = mem_Tracker_s_instance.allocations;
        usize leak_count = 0;
        usize total_leaked = 0;

        // Create a dynamic array for leak sites
        var sites = catch_((ArrList_init$LeakSite(mem_Tracker_s_instance.gpa, 0))($ignore, claim_unreachable));
        defer_(ArrList_fini$LeakSite(&sites, mem_Tracker_s_instance.gpa));

        // Process each leak
        while (curr) {
            leak_count++;
            total_leaked += curr->size;

            // Calculate age of leak
            time_Duration age = time_Instant_durationSince(now, curr->timestamp);
            f64 age_secs = time_Duration_asSecs$f64(age);

            // Log individual leak
            let_ignore = fprintf(mem_Tracker_s_instance.log_file, "Leak #%zu:\n", leak_count);
            let_ignore = fprintf(mem_Tracker_s_instance.log_file, "  Address: %p\n", curr->ptr);
            let_ignore = fprintf(mem_Tracker_s_instance.log_file, "  Size: %zu bytes\n", curr->size);
            let_ignore = fprintf(mem_Tracker_s_instance.log_file, "  Location: %s:%d\n", curr->src_loc.file_name, curr->src_loc.line);
            let_ignore = fprintf(mem_Tracker_s_instance.log_file, "  Function: %s\n", curr->src_loc.fn_name);
            let_ignore = fprintf(mem_Tracker_s_instance.log_file, "  Age: %.2f seconds\n", age_secs);

            // Find or add to leak sites
            eval_(void $scope)(for_(($s(sites.items))(site) {
                // Check if it's the same location
                if (Str_eql(Str_viewZ(as$(u8*)(site->src_loc.file_name)), Str_viewZ(as$(u8*)(curr->src_loc.file_name)))
                    && site->src_loc.line == curr->src_loc.line
                    && Str_eql(Str_viewZ(as$(u8*)(site->src_loc.fn_name)), Str_viewZ(as$(u8*)(curr->src_loc.fn_name)))) {

                    site->count++;
                    site->total_bytes += curr->size;
                    $break_void();
                }
            })) eval_(else)({
                // Add new leak site
                let site = catch_((ArrList_addBack$LeakSite(&sites, mem_Tracker_s_instance.gpa))($ignore, claim_unreachable));
                site->src_loc = curr->src_loc;
                site->count = 1;
                site->total_bytes = curr->size;
            }) $unscoped_(eval);
            curr = curr->next;
        }

        // Print leak summary by allocation site
        let_ignore = fprintf(mem_Tracker_s_instance.log_file, "\nLeak Summary by Location:\n");
        let_ignore = fprintf(mem_Tracker_s_instance.log_file, "=====================================\n");

        for_(($s(sites.items))(site) {
            let_ignore = fprintf(
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
        });

        let_ignore = fprintf(mem_Tracker_s_instance.log_file, "\nTotal leaked memory: %zu bytes\n", total_leaked);
    }

    // Cleanup the tracker's linked list of allocations
    mem_Allocation* curr = mem_Tracker_s_instance.allocations;
    while (curr) {
        mem_Allocation* next = curr->next;
        free(curr);
        curr = next;
    }

    let_ignore = fclose(mem_Tracker_s_instance.log_file);
    mem_Tracker_s_instance.log_file = null;
    mem_Tracker_s_instance.allocations = null;
    return_void();
} $unguarded_(fn);

fn_((mem_Tracker_registerAlloc(P$raw ptr, usize size, SrcLoc src_loc))(void)) {
    if (!ptr || !mem_Tracker_s_instance.log_file) { return; }

    // Create new allocation record
    let alloc = as$(mem_Allocation*)((malloc(sizeof(mem_Allocation))));
    if (!alloc) {
        // clang-format off
        let_ignore = fprintf(mem_Tracker_s_instance.log_file, "Failed to allocate memory for tracker at %s:%d\n",
            src_loc.file_name, src_loc.line
        );
        // clang-format on
        return;
    }

    // Fill allocation info
    alloc->ptr = ptr;
    alloc->size = size;
    alloc->src_loc = src_loc;
    alloc->timestamp = time_Instant_now();

    // Add to linked list
    alloc->next = mem_Tracker_s_instance.allocations;
    mem_Tracker_s_instance.allocations = alloc;

    // Update stats
    mem_Tracker_s_instance.total_allocated += size;
    mem_Tracker_s_instance.active_allocs++;

    // clang-format off
    // Log allocation with total bytes
    let_ignore = fprintf(mem_Tracker_s_instance.log_file, "ALLOC: %p (%zu bytes) at %s:%d in %s (Total: %zu bytes)\n",
        ptr, size, src_loc.file_name, src_loc.line, src_loc.fn_name, mem_Tracker_s_instance.total_allocated
    );
    // clang-format on
}

fn_((mem_Tracker_registerRemap(P$raw old_ptr, P$raw new_ptr, usize new_size, SrcLoc src_loc))(void)) {
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
    let_ignore = fprintf(mem_Tracker_s_instance.log_file, "REMAP: %p (%zu bytes) -> %p (%zu bytes) at %s:%d in %s\n",
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

fn_((mem_Tracker_registerFree(P$raw ptr, SrcLoc src_loc))(bool)) {
    if (!ptr || !mem_Tracker_s_instance.log_file) { return false; }

    mem_Allocation** curr = &mem_Tracker_s_instance.allocations;

    // Search for the allocation
    while (*curr && (*curr)->ptr != ptr) {
        curr = &(*curr)->next;
    }

    if (!*curr) {
        // clang-format off
        // Double free or invalid free detected
        let_ignore = fprintf(mem_Tracker_s_instance.log_file, "ERROR: DOUBLE FREE or INVALID FREE of %p at %s:%d in %s\n",
            ptr, src_loc.file_name, src_loc.line, src_loc.fn_name
        );
        // clang-format on
        return false;
    }

    // Calculate elapsed time since allocation
    let elapsed = time_Instant_durationSince(time_Instant_now(), (*curr)->timestamp);
    let elapsed_sec = time_Duration_asSecs$f64(elapsed);

    // Get the original allocation info
    let orig_size = (*curr)->size;
    let orig_src_loc = (*curr)->src_loc;

    // Update stats
    mem_Tracker_s_instance.total_allocated -= orig_size;
    mem_Tracker_s_instance.active_allocs--;

    // clang-format off
    // Log deallocation with details
    let_ignore = fprintf(mem_Tracker_s_instance.log_file,
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
    *curr = to_free->next;
    free(to_free);
    return true;
}

fn_((mem_Tracker_instance(void))(mem_Tracker*)) {
    return &mem_Tracker_s_instance;
}

#endif /* defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !debug_comp_enabled */
