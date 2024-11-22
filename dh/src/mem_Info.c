// #include "dh/mem/Info.h"
// #include "dh/mem/cfg.h"
// #include "dh/debug/cfg.h"
// #include "dh/debug/assert.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>


// #if defined(MEM_TRACE_ENABLED) && MEM_TRACE_ENABLED


// // Global memory tracking list
// static mem_Info* mem_s_info_list = null;


// // Register atexit handler for memory leak detection
// static void __attribute__((constructor)) mem_initInfoList(void);
// static void __attribute__((destructor))  mem_finiInfoList(void);
// // Memory tracking functions
// static void                              mem_addInfo(anyptr allocated, usize size, const char* func, const char* file, i32 line);
// static void                              mem_removeInfo(anyptr target);
// static void                              mem_printInfoMemoryLeakTrace(void);


// // Register atexit handler for memory leak detection
// static void __attribute__((constructor)) mem_initInfoList(void) {
//     ignore atexit(mem_printInfoMemoryLeakTrace);
// }

// static void __attribute__((destructor)) mem_finiInfoList(void) {
//     mem_printInfoMemoryLeakTrace();

//     mem_Info* current = mem_s_info_list;
//     mem_Info* next    = null;

//     while (current) {
//         next = current->next;
//         free(current);
//         current = next;
//     }

//     mem_s_info_list = null;
// }

// // Memory tracking functions
// static void mem_addInfo(anyptr allocated, usize size, const char* func, const char* file, i32 line) {
//     mem_Info* info     = malloc(sizeof(mem_Info));
//     info->ptr          = allocated;
//     info->size         = size;
//     info->func         = func;
//     info->file         = file;
//     info->line         = line;
//     info->alloc_time   = time(null);
//     info->dealloc_time = 0;
//     info->ref_count    = 1;
//     info->next         = mem_s_info_list;
//     mem_s_info_list    = info;
// }

// static void mem_removeInfo(anyptr target) {
//     mem_Info** current = &mem_s_info_list;
//     while (*current) {
//         if ((*current)->ptr == target) {
//             (*current)->dealloc_time = time(null);
//             (*current)->ref_count--;

//             // Remove if no references remain
//             if ((*current)->ref_count <= 0) {
//                 mem_Info* to_remove = *current;
//                 *current            = to_remove->next;
//                 free(to_remove);
//             }
//             return;
//         }
//         current = &(*current)->next;
//     }
// }

// static void mem_printInfoMemoryLeakTrace() {
//     mem_Info* current = mem_s_info_list;
//     while (current) {
//         if (0 < current->ref_count) {
//             ignore fprintf(stderr, "Memory Leak Detected!\n");
//             ignore fprintf(stderr, "Allocated in %s at %s:%d\n", current->func, current->file, current->line);
//             ignore fprintf(stderr, "Size: %zu bytes\n", current->size);
//             ignore fprintf(stderr, "Allocation time: %s", ctime(&current->alloc_time));
//             ignore fprintf(stderr, "Reference count: %d\n\n", current->ref_count);
//         }
//         current = current->next;
//     }
// }

// // Debug memory management functions
// anyptr mem__allocate(usize size, const char* func, const char* file, i32 line) {
//     anyptr allocated = malloc(size);
//     debug_assert_nonnull_fmt(allocated, "Memory allocation(malloc) failed in %s at %s:%d\n", func, file, line);
//     mem_addInfo(allocated, size, func, file, line);
//     return allocated;
// }

// anyptr mem__allocateCleared(usize size, usize count, const char* func, const char* file, i32 line) {
//     anyptr allocated = calloc(count, size);
//     debug_assert_nonnull_fmt(allocated, "Memory allocation(calloc) failed in %s at %s:%d\n", func, file, line);
//     mem_addInfo(allocated, count * size, func, file, line);
//     return allocated;
// }

// anyptr mem__allocateWith(usize size, anyptr src, const char* func, const char* file, i32 line) {
//     debug_assert_nonnull_fmt(src, "null in allocation(memcpy) from %s at %s:%d\n", func, file, line);
//     anyptr dest = mem__allocate(size, func, file, line);
//     debug_assert_nonnull_fmt(dest, "Memory allocation(malloc) failed in %s at %s:%d\n", func, file, line);
//     anyptr result = mem__copy(dest, src, size, func, file, line);
//     debug_assert_nonnull_fmt(result, "Memory copy(memcpy) failed in %s at %s:%d\n", func, file, line);
//     return result;
// }

// anyptr mem__reallocate(anyptr ptr, usize size, const char* func, const char* file, i32 line) {
//     debug_assert_nonnull_fmt(ptr, "null in reallocation(realloc) from %s at %s:%d\n", func, file, line);
//     if (!ptr) {
//         return mem__allocate(size, func, file, line);
//     }

//     anyptr reallocated = realloc(ptr, size);
//     debug_assert_nonnull_fmt(reallocated, "Memory reallocation failed in %s at %s:%d\n", func, file, line);

//     // Update tracking info
//     mem_Info* current = mem_s_info_list;
//     while (current) {
//         if (current->ptr == ptr) {
//             current->ptr  = reallocated;
//             current->size = size;
//             break;
//         }
//         current = current->next;
//     }

//     return reallocated;
// }

// void mem__deallocate(anyptr* ptr_addr, const char* func, const char* file, i32 line) {
//     debug_assert_nonnull_fmt(ptr_addr, "null in deallocation(free) from %s at %s:%d\n", func, file, line);
//     debug_assert_nonnull_fmt(*ptr_addr, "null in deallocation(free) from %s at %s:%d\n", func, file, line);
//     mem_removeInfo(*ptr_addr);
//     free(*ptr_addr);
//     *ptr_addr = null;
// }

// anyptr mem__set(anyptr dest, i32 val, usize size, const char* func, const char* file, i32 line) {
//     debug_assert_nonnull_fmt(dest, "null in set(memset) from %s at %s:%d\n", func, file, line);
//     anyptr result = memset(dest, val, size);
//     debug_assert_nonnull_fmt(result, "Memory set(memset) failed in %s at %s:%d\n", func, file, line);
//     return result;
// }

// // Only update reference counts for tracked memory
// anyptr mem__copy(anyptr dest, const anyptr src, usize size, const char* func, const char* file, i32 line) {
//     debug_assert_nonnull_fmt(dest, "null in copy(memcpy) from %s at %s:%d\n", func, file, line);
//     debug_assert_nonnull_fmt(src, "null in copy(memcpy) from %s at %s:%d\n", func, file, line);
//     anyptr result = memcpy(dest, src, size);
//     debug_assert_nonnull_fmt(result, "Memory copy(memcpy) failed in %s at %s:%d\n", func, file, line);
//     return result;
// }

// // Don't create new tracking entry, just update existing ones
// anyptr mem__move(anyptr dest, anyptr src, usize size, const char* func, const char* file, i32 line) {
//     debug_assert_nonnull_fmt(dest, "null in move(memmove) from %s at %s:%d\n", func, file, line);
//     debug_assert_nonnull_fmt(src, "null in move(memmove) from %s at %s:%d\n", func, file, line);
//     anyptr result = memmove(dest, src, size);
//     debug_assert_nonnull_fmt(result, "Memory move(memmove) failed in %s at %s:%d\n", func, file, line);
//     return result;
// }

// #else

// anyptr mem__allocate(usize size) {
//     return malloc(size);
// }

// anyptr mem__allocateCleared(usize count, usize size) {
//     return calloc(count, size);
// }

// anyptr mem__allocateWith(usize size, anyptr src) {
//     return mem__copy(mem__allocate(size), src, size);
// }

// anyptr mem__reallocate(anyptr ptr, usize size) {
//     return realloc(ptr, size);
// }

// void mem__deallocate(anyptr* ptr_addr) {
//     free(*ptr_addr);
//     *ptr_addr = null;
// }

// anyptr mem__set(anyptr dest, i32 val, usize size) {
//     return memset(dest, val, size);
// }

// anyptr mem__copy(anyptr dest, const anyptr src, usize size) {
//     return memcpy(dest, src, size);
// }

// anyptr mem__move(anyptr dest, anyptr src, usize size) {
//     return memmove(dest, src, size);
// }

// #endif
