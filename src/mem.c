#include "mem.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef NDEBUG

// Global memory tracking list
mem_MemoryInfo* mem__info_list = null;

// Memory tracking functions
static void mem__addInfo(type allocated, usize size, const char* func, const char* file, i32 line) {
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

static void mem__removeInfo(type target) {
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
            ignore_return fprintf(stderr, "Memory Leak Detected!\n");
            ignore_return fprintf(stderr, "Allocated in %s at %s:%d\n", current->func, current->file, current->line);
            ignore_return fprintf(stderr, "Size: %zu bytes\n", current->size);
            ignore_return fprintf(stderr, "Allocation time: %s", ctime(&current->alloc_time));
            ignore_return fprintf(stderr, "Reference count: %d\n\n", current->ref_count);
        }
        current = current->next;
    }
}

// Register atexit handler for memory leak detection
static void __attribute__((constructor)) mem__initInfoList() {
    ignore_return atexit(mem__printInfoMemoryLeakTrace);
}

// Debug memory management functions
type mem_allocate_(usize size, const char* func, const char* file, i32 line) {
    type allocated = malloc(size);
    assertNotNullFmt(allocated, "Memory allocation failed in %s at %s:%d\n", func, file, line);
    mem__addInfo(allocated, size, func, file, line);
    return allocated;
}

type mem_allocateCleared_(usize num, usize size, const char* func, const char* file, i32 line) {
    type allocated = calloc(num, size);
    assertNotNullFmt(allocated, "Memory allocation failed in %s at %s:%d\n", func, file, line);
    mem__addInfo(allocated, num * size, func, file, line);
    return allocated;
}

type mem_allocateWith_(usize size, type src, const char* func, const char* file, i32 line) {
    type dest = mem_allocate_(size, func, file, line);
    assertNotNullFmt(dest, "Memory allocation failed in %s at %s:%d\n", func, file, line);
    type result = mem_copy_(dest, src, size, func, file, line);
    assertNotNullFmt(result, "Memory copy failed in %s at %s:%d\n", func, file, line);
    return result;
}

void mem_deallocate_(type target, const char* func, const char* file, i32 line) {
    assertNotNullFmt(target, "null in free from %s at %s:%d\n", func, file, line);
    type* target_ptr = (type*)target;
    mem__removeInfo(*target_ptr);
    free(*target_ptr);
    *target_ptr = null;
}

type mem_set_(type dest, i32 val, usize size, const char* func, const char* file, i32 line) {
    assertNotNullFmt(dest, "null in memset from %s at %s:%d\n", func, file, line);
    type result = memset(dest, val, size);
    assertNotNullFmt(result, "memset failed in %s at %s:%d\n", func, file, line);
    return result;
}

type mem_copy_(type dest, const type src, usize size, const char* func, const char* file, i32 line) {
    assertNotNullFmt(dest, "null in memcpy from %s at %s:%d\n", func, file, line);
    assertNotNullFmt(src, "null in memcpy from %s at %s:%d\n", func, file, line);
    type result = memcpy(dest, src, size);
    assertNotNullFmt(result, "memcpy failed in %s at %s:%d\n", func, file, line);
    return result;
}

// Don't create new tracking entry, just update existing ones
type mem_move_(type dest, type src, usize size, const char* func, const char* file, i32 line) {
    assertNotNullFmt(dest, "null in move from %s at %s:%d\n", func, file, line);
    assertNotNullFmt(src, "null in move from %s at %s:%d\n", func, file, line);
    type result = memmove(dest, src, size);
    assertNotNullFmt(result, "memmove failed in %s at %s:%d\n", func, file, line);
    return result;
}

// Only update reference counts for tracked memory
type mem_copyBlock_(type dest, const type src, usize size, const char* func, const char* file, i32 line) {
    assertNotNullFmt(dest, "null in copy from %s at %s:%d\n", func, file, line);
    assertNotNullFmt(src, "null in copy from %s at %s:%d\n", func, file, line);
    type result = memcpy(dest, src, size);
    assertNotNullFmt(result, "memcpy failed in %s at %s:%d\n", func, file, line);
    return result;
}

#else

type mem_allocate_(usize size) {
    return malloc(size);
}

type mem_allocateCleared_(usize num, usize size) {
    return calloc(num, size);
}

type mem_allocateWith_(usize size, type src) {
    return mem_copy_(mem_allocate_(size), src, size);
}

void mem_deallocate_(type target) {
    type* target_ptr = (type*)target;
    free(*target_ptr);
    *target_ptr = null;
}

type mem_set_(type dest, i32 val, usize size) {
    return memset(dest, val, size);
}

type mem_copy_(type dest, const type src, usize size) {
    return memcpy(dest, src, size);
}

type mem_move_(type dest, type src, usize size) {
    return memmove(dest, src, size);
}

type mem_copyBlock_(type dest, const type src, usize size) {
    return memcpy(dest, src, size);
}

#endif /* NDEBUG */
