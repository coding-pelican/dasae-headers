#include "mem.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef NDEBUG

// Global memory tracking list
mem_MemoryInfo* mem__info_list = null;

// Memory tracking functions
static void mem__addInfo(anyopaque allocated, usize size, const char* func, const char* file, i32 line) {
    mem_MemoryInfo* info = malloc(sizeof(mem_MemoryInfo));
    info->ptr            = allocated;
    info->size           = size;
    info->func           = func;
    info->file           = file;
    info->line           = line;
    info->alloc_time     = time(null);
    info->dealloc_time   = 0;
    info->ref_count      = 1;
    info->next           = mem__info_list;
    mem__info_list       = info;
}

static void mem__removeInfo(anyopaque target) {
    mem_MemoryInfo** current = &mem__info_list;
    while (*current) {
        if ((*current)->ptr == target) {
            (*current)->dealloc_time = time(null);
            (*current)->ref_count--;

            // Remove if no references remain
            if ((*current)->ref_count <= 0) {
                mem_MemoryInfo* to_remove = *current;
                *current                  = to_remove->next;
                free(to_remove);
            }
            return;
        }
        current = &(*current)->next;
    }
}

static void mem__printInfoMemoryLeakTrace() {
    mem_MemoryInfo* current = mem__info_list;
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

// Register atexit handler for memory leak detection
static void __attribute__((constructor)) mem__initInfoList() {
    pp_ignore atexit(mem__printInfoMemoryLeakTrace);
}

// Debug memory management functions
anyopaque mem_allocate_(usize size, const char* func, const char* file, i32 line) {
    anyopaque allocated = malloc(size);
    debug_assertNotNullFmt(allocated, "Memory allocation(malloc) failed in %s at %s:%d\n", func, file, line);
    mem__addInfo(allocated, size, func, file, line);
    return allocated;
}

anyopaque mem_allocateCleared_(usize count, usize size, const char* func, const char* file, i32 line) {
    anyopaque allocated = calloc(count, size);
    debug_assertNotNullFmt(allocated, "Memory allocation(calloc) failed in %s at %s:%d\n", func, file, line);
    mem__addInfo(allocated, count * size, func, file, line);
    return allocated;
}

anyopaque mem_allocateWith_(usize size, anyopaque src, const char* func, const char* file, i32 line) {
    debug_assertNotNullFmt(src, "null in allocation(memcpy) from %s at %s:%d\n", func, file, line);
    anyopaque dest = mem_allocate_(size, func, file, line);
    debug_assertNotNullFmt(dest, "Memory allocation(malloc) failed in %s at %s:%d\n", func, file, line);
    anyopaque result = mem_copy_(dest, src, size, func, file, line);
    debug_assertNotNullFmt(result, "Memory copy(memcpy) failed in %s at %s:%d\n", func, file, line);
    return result;
}

void mem_deallocate_(anyopaque ptr, const char* func, const char* file, i32 line) {
    debug_assertNotNullFmt(ptr, "null in deallocation(free) from %s at %s:%d\n", func, file, line);
    anyopaque* target_ptr = (anyopaque*)ptr;
    mem__removeInfo(*target_ptr);
    free(*target_ptr);
    *target_ptr = null;
}

anyopaque mem_set_(anyopaque dest, i32 val, usize size, const char* func, const char* file, i32 line) {
    debug_assertNotNullFmt(dest, "null in set(memset) from %s at %s:%d\n", func, file, line);
    anyopaque result = memset(dest, val, size);
    debug_assertNotNullFmt(result, "Memory set(memset) failed in %s at %s:%d\n", func, file, line);
    return result;
}

// Only update reference counts for tracked memory
anyopaque mem_copy_(anyopaque dest, const anyopaque src, usize size, const char* func, const char* file, i32 line) {
    debug_assertNotNullFmt(dest, "null in copy(memcpy) from %s at %s:%d\n", func, file, line);
    debug_assertNotNullFmt(src, "null in copy(memcpy) from %s at %s:%d\n", func, file, line);
    anyopaque result = memcpy(dest, src, size);
    debug_assertNotNullFmt(result, "Memory copy(memcpy) failed in %s at %s:%d\n", func, file, line);
    return result;
}

// Don't create new tracking entry, just update existing ones
anyopaque mem_move_(anyopaque dest, anyopaque src, usize size, const char* func, const char* file, i32 line) {
    debug_assertNotNullFmt(dest, "null in move(memmove) from %s at %s:%d\n", func, file, line);
    debug_assertNotNullFmt(src, "null in move(memmove) from %s at %s:%d\n", func, file, line);
    anyopaque result = memmove(dest, src, size);
    debug_assertNotNullFmt(result, "Memory move(memmove) failed in %s at %s:%d\n", func, file, line);
    return result;
}

#else

anyopaque mem_allocate_(usize size) {
    return malloc(size);
}

anyopaque mem_allocateCleared_(usize count, usize size) {
    return calloc(count, size);
}

anyopaque mem_allocateWith_(usize size, anyopaque src) {
    return mem_copy_(mem_allocate_(size), src, size);
}

void mem_deallocate_(anyopaque ptr) {
    anyopaque* target_ptr = (anyopaque*)ptr;
    free(*target_ptr);
    *target_ptr = null;
}

anyopaque mem_set_(anyopaque dest, i32 val, usize size) {
    return memset(dest, val, size);
}

anyopaque mem_copy_(anyopaque dest, const anyopaque src, usize size) {
    return memcpy(dest, src, size);
}

anyopaque mem_move_(anyopaque dest, anyopaque src, usize size) {
    return memmove(dest, src, size);
}

#endif /* NDEBUG */
