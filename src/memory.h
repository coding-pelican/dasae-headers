// TODO(dev-dasae): Separate implementation from header
// TODO(dev-dasae): Use member `deallocTime` to track memory leaks
// TODO(dev-dasae): Document annotations with heading


#ifndef MEMORY_INCLUDED
#define MEMORY_INCLUDED (1)


#include "assert.h"
#include "primitive_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct MemoryInfo MemoryInfo;


// Memory tracking structure
struct MemoryInfo {
    void*       ptr; // Pointer to allocated memory
    size_t      size; // Size of allocation
    const char* func; // Function name
    const char* file; // Source file
    int         line; // Line number
    time_t      allocTime; // Allocation timestamp
    time_t      deallocTime; // Deallocation timestamp
    int         refCount; // Reference count
    MemoryInfo* next; // Next in linked list
};


// Global memory tracking list
static MemoryInfo* Memory__s_infoList = NULL;


// Debug macro definitions
#ifdef NDEBUG
#  define Memory_Allocate(size)             malloc(size)
#  define Memory_AllocateCleared(num, size) calloc(num, size)
#  define Memory_Deallocate(ptr)            free(ptr)
#  define Memory_Set(dest, val, size)       memset(dest, val, size)
#  define Memory_Copy(dest, src, size)      memcpy(dest, src, size)
#  define Memory_Move(dest, src, size)      memmove(dest, src, size)
#  define Memory_CopyBlock(dest, src, size) memcpy(dest, src, size)
#else
#  define Memory_Allocate(size)             Memory_Allocate_(size, __func__, __FILE__, __LINE__)
#  define Memory_AllocateCleared(num, size) Memory_AllocateCleared_(num, size, __func__, __FILE__, __LINE__)
#  define Memory_Deallocate(ptr)            Memory_Deallocate_(ptr, __func__, __FILE__, __LINE__)
#  define Memory_Set(dest, val, size)       Memory_Set_(dest, val, size, __func__, __FILE__, __LINE__)
#  define Memory_Copy(dest, src, size)      Memory_Copy_(dest, src, size, __func__, __FILE__, __LINE__)
#  define Memory_Move(dest, src, size)      Memory_Move_(dest, src, size, __func__, __FILE__, __LINE__)
#  define Memory_CopyBlock(dest, src, size) Memory_CopyBlock_(dest, src, size, __func__, __FILE__, __LINE__)
#endif


// Memory tracking functions
static void Memory__AddInfo(void* ptr, size_t size, const char* func, const char* file, int line) {
    MemoryInfo* info   = malloc(sizeof(MemoryInfo));
    info->ptr          = ptr;
    info->size         = size;
    info->func         = func;
    info->file         = file;
    info->line         = line;
    info->allocTime    = time(NULL);
    info->deallocTime  = 0;
    info->refCount     = 1;
    info->next         = Memory__s_infoList;
    Memory__s_infoList = info;
}

static void Memory__RemoveInfo(void* ptr) {
    MemoryInfo** current = &Memory__s_infoList;
    while (*current) {
        if ((*current)->ptr == ptr) {
            (*current)->deallocTime = time(NULL);
            (*current)->refCount--;

            // Remove if no references remain
            if ((*current)->refCount <= 0) {
                MemoryInfo* to_remove = *current;
                *current              = to_remove->next;
                free(to_remove);
            }
            return;
        }
        current = &(*current)->next;
    }
}

static void Memory__PrintInfoMemoryLeakTrace() {
    MemoryInfo* current = Memory__s_infoList;
    while (current) {
        if (current->refCount > 0) {
            (void)fprintf(stderr, "Memory Leak Detected!\n");
            (void)fprintf(stderr, "Allocated in %s at %s:%d\n", current->func, current->file, current->line);
            (void)fprintf(stderr, "Size: %zu bytes\n", current->size);
            (void)fprintf(stderr, "Allocation time: %s", ctime(&current->allocTime));
            (void)fprintf(stderr, "Reference count: %d\n\n", current->refCount);
        }
        current = current->next;
    }
}

// Debug memory management functions
void* Memory_Allocate_(size_t size, const char* func, const char* file, int line) {
    void* ptr = malloc(size);
    AssertNotNullFmt(ptr, "Memory allocation failed in %s at %s:%d\n", func, file, line);
    Memory__AddInfo(ptr, size, func, file, line);
    return ptr;
}

void* Memory_AllocateCleared_(size_t num, size_t size, const char* func, const char* file, int line) {
    void* ptr = calloc(num, size);
    AssertNotNullFmt(ptr, "Memory allocation failed in %s at %s:%d\n", func, file, line);
    Memory__AddInfo(ptr, num * size, func, file, line);
    return ptr;
}

void Memory_Deallocate_(void* ptr, const char* func, const char* file, int line) {
    AssertNotNullFmt(ptr, "NULL pointer in free from %s at %s:%d\n", func, file, line);
    void** ptrTarget = (void**)ptr;
    Memory__RemoveInfo(*ptrTarget);
    free(*ptrTarget);
    *ptrTarget = nullptr;
}

void* Memory_Set_(void* dest, int val, size_t size, const char* func, const char* file, int line) {
    AssertNotNullFmt(dest, "NULL pointer in memset from %s at %s:%d\n", func, file, line);
    return memset(dest, val, size);
}

void* Memory_Copy_(void* dest, const void* src, size_t size, const char* func, const char* file, int line) {
    AssertNotNullFmt(dest, "NULL pointer in memcpy from %s at %s:%d\n", func, file, line);
    AssertNotNullFmt(src, "NULL pointer in memcpy from %s at %s:%d\n", func, file, line);
    return memcpy(dest, src, size);
}

void* Memory_Move_(void* dest, void* src, size_t size, const char* func, const char* file, int line) {
    AssertNotNullFmt(dest, "NULL pointer in move from %s at %s:%d\n", func, file, line);
    AssertNotNullFmt(src, "NULL pointer in move from %s at %s:%d\n", func, file, line);
    void* result = memmove(dest, src, size);
    // Don't create new tracking entry, just update existing ones
    return result;
}

void* Memory_CopyBlock_(void* dest, const void* src, size_t size, const char* func, const char* file, int line) {
    AssertNotNullFmt(dest, "NULL pointer in copy from %s at %s:%d\n", func, file, line);
    void* result = memcpy(dest, src, size);
    // Only update reference counts for tracked memory
    return result;
}

// Register atexit handler for memory leak detection
static void __attribute__((constructor)) Memory__InitInfoList() {
    (void)atexit(Memory__PrintInfoMemoryLeakTrace);
}


#endif /* MEMORY_INCLUDED */
