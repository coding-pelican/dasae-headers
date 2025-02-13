#include "dh/mem/Tracker.h"

#include <stdlib.h>
#include <string.h>

#if defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !debug_comp_enabled
#else

static const char* const mem_Tracker_default_log_file = "mem_trace.log";
// static FILE*             mem_Tracker_fallbackLogFile(void) { return stderr; }

static void __attribute__((constructor)) mem_Tracker_init(void) {
    mem_Tracker_initWithFile(mem_Tracker_default_log_file);
}
static void __attribute__((destructor)) mem_Tracker_fini(void) {
    mem_Tracker_generateReportAndCleanup();
}

static mem_Tracker mem_Tracker_s_instance = cleared();

void mem_Tracker_initWithFile(const char* log_path) {
    mem_Tracker_s_instance.log_file = fopen(log_path, "w");
    if (!mem_Tracker_s_instance.log_file) {
        ignore fprintf(stderr, "Failed to open memory tracker log file: %s\n", log_path);
        exit(1);
    }
    ignore fprintf(mem_Tracker_s_instance.log_file, "Memory Tracker Initialized\n");
    ignore fprintf(mem_Tracker_s_instance.log_file, "================================\n");
}

void mem_Tracker_registerAlloc(void* ptr, usize size, const char* file, int line, const char* func) {
    if (!ptr) { return; }

    // Create new allocation record
    mem_Allocation* alloc = (mem_Allocation*)malloc(sizeof(mem_Allocation));
    if (!alloc) {
        ignore fprintf(mem_Tracker_s_instance.log_file, "Failed to allocate memory for tracker at %s:%d\n", file, line);
        return;
    }

    // Fill allocation info
    alloc->ptr       = ptr;
    alloc->size      = size;
    alloc->file      = file;
    alloc->line      = line;
    alloc->func      = func;
    alloc->timestamp = time(null);

    // Add to linked list
    alloc->next                        = mem_Tracker_s_instance.allocations;
    mem_Tracker_s_instance.allocations = alloc;

    // Update stats
    mem_Tracker_s_instance.total_allocated += size;
    mem_Tracker_s_instance.active_allocs++;

    // Log allocation with total bytes
    // clang-format off
    ignore fprintf(mem_Tracker_s_instance.log_file,
        "ALLOC: %p (%zu bytes) at %s:%d in %s (Total bytes allocated: %zu)\n",
        ptr, size, file, line, func, mem_Tracker_s_instance.total_allocated
    );
    // clang-format on
}

void mem_Tracker_registerFree(void* ptr, const char* file, int line, const char* func) {
    if (!ptr) { return; }

    mem_Allocation** curr = &mem_Tracker_s_instance.allocations;

    // Search for the allocation
    while (*curr && (*curr)->ptr != ptr) {
        curr = &(*curr)->next;
    }

    if (!*curr) {
        // Double free or invalid free detected
        // clang-format off
        ignore fprintf(mem_Tracker_s_instance.log_file,
            "ERROR: Invalid/Double free of %p at %s:%d in %s\n",
            ptr, file, line, func
        );
        // clang-format on
        return;
    }

    // Get the original allocation size
    usize orig_size = (*curr)->size;

    // Update stats
    mem_Tracker_s_instance.total_allocated -= orig_size;
    mem_Tracker_s_instance.active_allocs--;

    // Log deallocation with total bytes
    // clang-format off
    ignore fprintf(mem_Tracker_s_instance.log_file,
        "FREE: %p (%zu bytes) at %s:%d in %s (Total bytes remaining: %zu)\n",
        ptr, orig_size, file, line, func, mem_Tracker_s_instance.total_allocated
    );
    // clang-format on

    // Remove from list
    mem_Allocation* to_free = *curr;
    *curr                   = to_free->next;
    free(to_free);
}

typedef struct LeakSite {
    const char* file;
    i32         line;
    const char* func;
    usize       count;
    usize       total_bytes;
} LeakSite;
void mem_Tracker_generateReportAndCleanup(void) {
    if (!mem_Tracker_s_instance.log_file) { return; }

    ignore fprintf(mem_Tracker_s_instance.log_file, "\nMemory Leak Report\n");
    ignore fprintf(mem_Tracker_s_instance.log_file, "=====================================\n");
    ignore fprintf(mem_Tracker_s_instance.log_file, "Total allocations: %zu bytes\n", mem_Tracker_s_instance.total_allocated);
    ignore fprintf(mem_Tracker_s_instance.log_file, "Active allocations: %zu\n", mem_Tracker_s_instance.active_allocs);

    if (0 < mem_Tracker_s_instance.active_allocs) {
        ignore fprintf(mem_Tracker_s_instance.log_file, "\nDetected Memory Leaks:\n");
        ignore fprintf(mem_Tracker_s_instance.log_file, "=====================================\n");

        mem_Allocation* curr         = mem_Tracker_s_instance.allocations;
        usize           leak_count   = 0;
        usize           total_leaked = 0;

        // Group leaks by allocation site
        LeakSite* sites         = null;
        usize     site_count    = 0;
        usize     site_capacity = 16;
        sites                   = malloc(sizeof(LeakSite) * site_capacity);

        while (curr) {
            ignore fprintf(mem_Tracker_s_instance.log_file, "Leak #%zu:\n", ++leak_count);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Address: %p\n", curr->ptr);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Size: %zu bytes\n", curr->size);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Location: %s:%d\n", curr->file, curr->line);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Function: %s\n", curr->func);

            // Track leak sites
            bool found = false;
            for (usize i = 0; i < site_count; i++) {
                if (sites[i].file == curr->file && sites[i].line == curr->line && sites[i].func == curr->func) {
                    sites[i].count++;
                    sites[i].total_bytes += curr->size;
                    found = true;
                    break;
                }
            }

            if (!found) {
                if (site_count == site_capacity) {
                    site_capacity *= 2;
                    LeakSite* new_sites = realloc(sites, sizeof(LeakSite) * site_capacity);
                    if (new_sites == null) {
                        // Handle realloc failure
                        free(sites);
                        claim_unreachable_msg("Failed to reallocate memory for leak sites");
                        // TODO: Perhaps add some cleanup-related logic
                        return;
                    }
                    sites = new_sites;
                }
                sites[site_count] = (LeakSite){
                    .file        = curr->file,
                    .line        = curr->line,
                    .func        = curr->func,
                    .count       = 1,
                    .total_bytes = curr->size
                };
                site_count++;
            }

            total_leaked += curr->size;
            curr = curr->next;
        }

        // Print leak summary by allocation site
        ignore fprintf(mem_Tracker_s_instance.log_file, "\nLeak Summary by Location:\n");
        ignore fprintf(mem_Tracker_s_instance.log_file, "=====================================\n");
        for (usize i = 0; i < site_count; i++) {
            // clang-format off
            ignore fprintf(mem_Tracker_s_instance.log_file,
                "Location: %s:%d in %s\n"
                    "  Count: %zu leaks\n"
                    "  Total bytes: %zu\n\n",
                sites[i].file, sites[i].line, sites[i].func,
                sites[i].count,
                sites[i].total_bytes
            ); // clang-format on
        }

        free(sites);
        ignore fprintf(mem_Tracker_s_instance.log_file, "\nTotal leaked memory: %zu bytes\n", total_leaked);
    }

    // Only clean up the tracker's own allocations
    mem_Allocation* curr = mem_Tracker_s_instance.allocations;
    while (curr) {
        mem_Allocation* next = curr->next;
        /* FIXME: When exiting the program, a memory leak was detected and during the cleanup process, the program was crashed in the code below. */
        // free(curr->ptr); // Fix the leak
        free(curr);
        curr = next;
    }

    ignore fclose(mem_Tracker_s_instance.log_file);
    mem_Tracker_s_instance.log_file    = null;
    mem_Tracker_s_instance.allocations = null;
}

mem_Tracker* mem_Tracker_instance(void) {
    return &mem_Tracker_s_instance;
}

#endif /* defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !debug_comp_enabled */
