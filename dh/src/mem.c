#include "../include/dh/mem/mem.h"
#include "../include/dh/debug/debug_assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#if defined(debug_enabled) && debug_enabled

// Global memory tracking list
memInfo* mem__info_list = null;

// Register atexit handler for memory leak detection
static void __attribute__((constructor)) mem__initInfoList(void) {
    pp_ignore atexit(mem__printInfoMemoryLeakTrace);
}

static void __attribute__((destructor)) mem__finiInfoList(void) {
    mem__printInfoMemoryLeakTrace();

    memInfo* current = mem__info_list;
    memInfo* next    = null;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }

    mem__info_list = null;
}

// Memory tracking functions
static void mem__addInfo(anyptr allocated, usize size, const char* func, const char* file, i32 line) {
    memInfo* info      = malloc(sizeof(memInfo));
    info->ptr          = allocated;
    info->size         = size;
    info->func         = func;
    info->file         = file;
    info->line         = line;
    info->alloc_time   = time(null);
    info->dealloc_time = 0;
    info->ref_count    = 1;
    info->next         = mem__info_list;
    mem__info_list     = info;
}

static void mem__removeInfo(anyptr target) {
    memInfo** current = &mem__info_list;
    while (*current) {
        if ((*current)->ptr == target) {
            (*current)->dealloc_time = time(null);
            (*current)->ref_count--;

            // Remove if no references remain
            if ((*current)->ref_count <= 0) {
                memInfo* to_remove = *current;
                *current           = to_remove->next;
                free(to_remove);
            }
            return;
        }
        current = &(*current)->next;
    }
}

static void mem__printInfoMemoryLeakTrace() {
    memInfo* current = mem__info_list;
    while (current) {
        if (0 < current->ref_count) {
            pp_ignore fprintf(stderr, "Memory Leak Detected!\n");
            pp_ignore fprintf(stderr, "Allocated in %s at %s:%d\n", current->func, current->file, current->line);
            pp_ignore fprintf(stderr, "Size: %zu bytes\n", current->size);
            pp_ignore fprintf(stderr, "Allocation time: %s", ctime(&current->alloc_time));
            pp_ignore fprintf(stderr, "Reference count: %d\n\n", current->ref_count);
        }
        current = current->next;
    }
}

// Debug memory management functions
anyptr mem__allocate(usize size, const char* func, const char* file, i32 line) {
    anyptr allocated = malloc(size);
    debug_assertNotNullFmt(allocated, "Memory allocation(malloc) failed in %s at %s:%d\n", func, file, line);
    mem__addInfo(allocated, size, func, file, line);
    return allocated;
}

anyptr mem__allocateCleared(usize size, usize count, const char* func, const char* file, i32 line) {
    anyptr allocated = calloc(count, size);
    debug_assertNotNullFmt(allocated, "Memory allocation(calloc) failed in %s at %s:%d\n", func, file, line);
    mem__addInfo(allocated, count * size, func, file, line);
    return allocated;
}

anyptr mem__allocateWith(usize size, anyptr src, const char* func, const char* file, i32 line) {
    debug_assertNotNullFmt(src, "null in allocation(memcpy) from %s at %s:%d\n", func, file, line);
    anyptr dest = mem__allocate(size, func, file, line);
    debug_assertNotNullFmt(dest, "Memory allocation(malloc) failed in %s at %s:%d\n", func, file, line);
    anyptr result = mem__copy(dest, src, size, func, file, line);
    debug_assertNotNullFmt(result, "Memory copy(memcpy) failed in %s at %s:%d\n", func, file, line);
    return result;
}

anyptr mem__reallocate(anyptr ptr, usize size, const char* func, const char* file, i32 line) {
    debug_assertNotNullFmt(ptr, "null in reallocation(realloc) from %s at %s:%d\n", func, file, line);
    if (!ptr) {
        return mem__allocate(size, func, file, line);
    }

    anyptr reallocated = realloc(ptr, size);
    debug_assertNotNullFmt(reallocated, "Memory reallocation failed in %s at %s:%d\n", func, file, line);

    // Update tracking info
    memInfo* current = mem__info_list;
    while (current) {
        if (current->ptr == ptr) {
            current->ptr  = reallocated;
            current->size = size;
            break;
        }
        current = current->next;
    }

    return reallocated;
}

void mem__deallocate(anyptr ptr_addr, const char* func, const char* file, i32 line) {
    debug_assertNotNullFmt(ptr_addr, "null in deallocation(free) from %s at %s:%d\n", func, file, line);
    anyptr* target_ptr = (anyptr*)ptr_addr;
    mem__removeInfo(*target_ptr);
    free(*target_ptr);
    *target_ptr = null;
}

anyptr mem__set(anyptr dest, i32 val, usize size, const char* func, const char* file, i32 line) {
    debug_assertNotNullFmt(dest, "null in set(memset) from %s at %s:%d\n", func, file, line);
    anyptr result = memset(dest, val, size);
    debug_assertNotNullFmt(result, "Memory set(memset) failed in %s at %s:%d\n", func, file, line);
    return result;
}

// Only update reference counts for tracked memory
anyptr mem__copy(anyptr dest, const anyptr src, usize size, const char* func, const char* file, i32 line) {
    debug_assertNotNullFmt(dest, "null in copy(memcpy) from %s at %s:%d\n", func, file, line);
    debug_assertNotNullFmt(src, "null in copy(memcpy) from %s at %s:%d\n", func, file, line);
    anyptr result = memcpy(dest, src, size);
    debug_assertNotNullFmt(result, "Memory copy(memcpy) failed in %s at %s:%d\n", func, file, line);
    return result;
}

// Don't create new tracking entry, just update existing ones
anyptr mem__move(anyptr dest, anyptr src, usize size, const char* func, const char* file, i32 line) {
    debug_assertNotNullFmt(dest, "null in move(memmove) from %s at %s:%d\n", func, file, line);
    debug_assertNotNullFmt(src, "null in move(memmove) from %s at %s:%d\n", func, file, line);
    anyptr result = memmove(dest, src, size);
    debug_assertNotNullFmt(result, "Memory move(memmove) failed in %s at %s:%d\n", func, file, line);
    return result;
}

#else

anyptr mem__allocate(usize size) {
    return malloc(size);
}

anyptr mem__allocateCleared(usize count, usize size) {
    return calloc(count, size);
}

anyptr mem__allocateWith(usize size, anyptr src) {
    return mem__copy(mem__allocate(size), src, size);
}

anyptr mem__reallocate(anyptr ptr, usize size) {
    return realloc(ptr, size);
}

void mem__deallocate(anyptr ptr_addr) {
    anyptr* target_ptr = (anyptr*)ptr_addr;
    free(*target_ptr);
    *target_ptr = null;
}

anyptr mem__set(anyptr dest, i32 val, usize size) {
    return memset(dest, val, size);
}

anyptr mem__copy(anyptr dest, const anyptr src, usize size) {
    return memcpy(dest, src, size);
}

anyptr mem__move(anyptr dest, anyptr src, usize size) {
    return memmove(dest, src, size);
}

#endif
