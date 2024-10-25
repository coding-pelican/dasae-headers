// TODO(dev-dasae): Separate implementation from header
// TODO(dev-dasae): Use member `deallocTime` to track memory leaks
// TODO(dev-dasae): Document annotations with heading


#ifndef MEM_INCLUDED
#define MEM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


#include "primitive_types.h"
#include <time.h>


#ifndef NDEBUG
typedef struct mem_MemoryInfo mem_MemoryInfo;
#endif /* NDEBUG */


#ifndef NDEBUG
// Memory tracking structure
struct mem_MemoryInfo {
    type            ptr; // Pointer to allocated memory
    usize           size; // Size of allocation
    const char*     func; // Function name
    const char*     file; // Source file
    i32             line; // Line number
    time_t          alloc_time; // Allocation timestamp
    time_t          dealloc_time; // Deallocation timestamp
    i32             ref_count; // Reference count
    mem_MemoryInfo* next; // Next in linked list
};
// Global memory tracking list
extern mem_MemoryInfo* mem__info_list;
// Memory tracking functions
static void mem__addInfo(type allocated, usize size, const char* func, const char* file, i32 line);
static void mem__removeInfo(type target);
static void mem__printInfoMemoryLeakTrace();
// Register atexit handler for memory leak detection
static void __attribute__((constructor)) mem__initInfoList();
#endif /* NDEBUG */


#define mem_create(T) (             \
    (Ptr(T))mem_allocate(sizeof(T)) \
)
#define mem_createCleared(T) (                \
    (Ptr(T))mem_allocateCleared(sizeof(T), 0) \
)
#define mem_createWith(T, ...) (                               \
    (Ptr(T))mem_allocateWith(sizeof(T), &make(T, __VA_ARGS__)) \
)
#define mem_destroy(ptr) \
    mem_deallocate(ptr)


#define mem_new(T, size) (                   \
    (Ptr(T))mem_allocate(sizeof(T) * (size)) \
)
#define mem_newCleared(T, size) (                \
    (Ptr(T))mem_allocateCleared(sizeof(T), size) \
)
#define mem_delete(ptr) \
    mem_deallocate(ptr)


// #define mem_init(T, self) // TODO(dev-dasae): Implement
// #define mem_fini(T, self) // TODO(dev-dasae): Implement



// Debug macro definitions
#ifndef NDEBUG
#  define mem_allocate(size)             mem_allocate_(size, __func__, __FILE__, __LINE__)
#  define mem_allocateCleared(num, size) mem_allocateCleared_(num, size, __func__, __FILE__, __LINE__)
#  define mem_allocateWith(size, src)    mem_allocateWith_(size, src, __func__, __FILE__, __LINE__)
#  define mem_deallocate(target)         mem_deallocate_(target, __func__, __FILE__, __LINE__)
#  define mem_set(dest, val, size)       mem_set_(dest, val, size, __func__, __FILE__, __LINE__)
#  define mem_copy(dest, src, size)      mem_copy_(dest, src, size, __func__, __FILE__, __LINE__)
#  define mem_move(dest, src, size)      mem_move_(dest, src, size, __func__, __FILE__, __LINE__)
#  define mem_copyBlock(dest, src, size) mem_copyBlock_(dest, src, size, __func__, __FILE__, __LINE__)
#else
#  define mem_allocate(size)             mem_allocate_(size)
#  define mem_allocateCleared(num, size) mem_allocateCleared_(num, size)
#  define mem_allocateWith(size, src)    mem_allocateWith_(size, src)
#  define mem_deallocate(target)         mem_deallocate_(target)
#  define mem_set(dest, val, size)       mem_set_(dest, val, size)
#  define mem_copy(dest, src, size)      mem_copy_(dest, src, size)
#  define mem_move(dest, src, size)      mem_move_(dest, src, size)
#  define mem_copyBlock(dest, src, size) mem_copyBlock_(dest, src, size)
#endif /* NDEBUG */

// Debug memory management functions
#ifndef NDEBUG
type mem_allocate_(usize size, const char* func, const char* file, i32 line);
type mem_allocateCleared_(usize num, usize size, const char* func, const char* file, i32 line);
type mem_allocateWith_(usize size, type src, const char* func, const char* file, i32 line);
void mem_deallocate_(type target, const char* func, const char* file, i32 line);
type mem_set_(type dest, i32 val, usize size, const char* func, const char* file, i32 line);
type mem_copy_(type dest, const type src, usize size, const char* func, const char* file, i32 line);
type mem_move_(type dest, type src, usize size, const char* func, const char* file, i32 line);
type mem_copyBlock_(type dest, const type src, usize size, const char* func, const char* file, i32 line);
#else
type mem_allocate_(usize size);
type mem_allocateCleared_(usize num, usize size);
type mem_allocateWith_(usize size, type src);
void mem_deallocate_(type target);
type mem_set_(type dest, i32 val, usize size);
type mem_copy_(type dest, const type src, usize size);
type mem_move_(type dest, type src, usize size);
type mem_copyBlock_(type dest, const type src, usize size);
#endif /* NDEBUG */


#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* MEM_INCLUDED */
