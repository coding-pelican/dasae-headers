#include "dh/mem/Tracker.h"
#include <stdlib.h>
#include <string.h>

#if defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !DEBUG_ENABLED
#else

static const char* const mem_Tracker_default_log_file = "memory_trace.log";
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

    // Log allocation
    // clang-format off
    ignore fprintf(mem_Tracker_s_instance.log_file,
        "ALLOC: %p (%zu bytes) at %s:%d in %s\n",
        ptr, size, file, line, func
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

    // Update stats
    mem_Tracker_s_instance.total_allocated -= (*curr)->size;
    mem_Tracker_s_instance.active_allocs--;

    // Log deallocation
    // clang-format off
    ignore fprintf(mem_Tracker_s_instance.log_file,
        "FREE: %p at %s:%d in %s\n",
        ptr, file, line, func
    );
    // clang-format on

    // Remove from list
    mem_Allocation* to_free = *curr;
    *curr                   = to_free->next;
    free(to_free);
}

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

        while (curr) {
            ignore fprintf(mem_Tracker_s_instance.log_file, "Leak #%zu:\n", ++leak_count);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Address: %p\n", curr->ptr);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Size: %zu bytes\n", curr->size);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Location: %s:%d\n", curr->file, curr->line);
            ignore fprintf(mem_Tracker_s_instance.log_file, "  Function: %s\n", curr->func);

            total_leaked += curr->size;
            curr = curr->next;
        }

        ignore fprintf(mem_Tracker_s_instance.log_file, "\nTotal leaked memory: %zu bytes\n", total_leaked);
    }

    // Cleanup allocations
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

#endif /* defined(MEM_NO_TRACE_ALLOC_AND_FREE) || !DEBUG_ENABLED */
