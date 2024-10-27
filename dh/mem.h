// TODO(dev-dasae): Separate implementation from header
// TODO(dev-dasae): Use member `deallocTime` to track memory leaks
// TODO(dev-dasae): Document annotations with heading


#ifndef MEM_INCLUDED
#define MEM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


#include "types.h"
#include <time.h>


#ifndef NDEBUG
typedef struct mem_MemoryInfo mem_MemoryInfo;
#endif /* NDEBUG */


#ifndef NDEBUG
// Memory tracking structure
struct mem_MemoryInfo {
    anyopaque       ptr; // Pointer to allocated memory
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
static void mem__addInfo(anyopaque allocated, usize size, const char* func, const char* file, i32 line);
static void mem__removeInfo(anyopaque target);
static void mem__printInfoMemoryLeakTrace();
// Register atexit handler for memory leak detection
static void __attribute__((constructor)) mem__initInfoList();
#endif /* NDEBUG */


#define mem_create(_T) (              \
    (Ptr(_T))mem_allocate(sizeof(_T)) \
)
#define mem_createCleared(_T) (                 \
    (Ptr(_T))mem_allocateCleared(sizeof(_T), 0) \
)
#define mem_createWith(_T, ...) (                                     \
    (Ptr(_T))mem_allocateWith(sizeof(_T), &makeWith(_T, __VA_ARGS__)) \
)
#define mem_destroy(_ptr) \
    mem_deallocate(_ptr)


#define mem_new(_T, _count) (                    \
    (Ptr(_T))mem_allocate(sizeof(_T) * (_count)) \
)
#define mem_newCleared(_T, _count) (                 \
    (Ptr(_T))mem_allocateCleared(sizeof(_T), _count) \
)
#define mem_newWith(_T, _count, ...) (                                                 \
    (Ptr(_T))mem_allocateWith(sizeof(_T) * _count, &makeWith(_T[_count], __VA_ARGS__)) \
)
#define mem_delete(_ptr) \
    mem_deallocate(_ptr)


// #define mem_init(_T, self) // TODO(dev-dasae): Implement
// #define mem_fini(_T, self) // TODO(dev-dasae): Implement



// Debug macro definitions
#ifndef NDEBUG
#  define mem_allocate(_size)                mem_allocate_(_size, __func__, __FILE__, __LINE__)
#  define mem_allocateCleared(_count, _size) mem_allocateCleared_(_count, _size, __func__, __FILE__, __LINE__)
#  define mem_allocateWith(_size, _src)      mem_allocateWith_(_size, _src, __func__, __FILE__, __LINE__)
#  define mem_deallocate(_ptr)               mem_deallocate_(_ptr, __func__, __FILE__, __LINE__)
#  define mem_set(_dest, _val, _size)        mem_set_(_dest, _val, _size, __func__, __FILE__, __LINE__)
#  define mem_copy(_dest, _src, _size)       mem_copy_(_dest, _src, _size, __func__, __FILE__, __LINE__)
#  define mem_move(_dest, _src, _size)       mem_move_(_dest, _src, _size, __func__, __FILE__, __LINE__)
#else
#  define mem_allocate(_size)                mem_allocate_(_size)
#  define mem_allocateCleared(_count, _size) mem_allocateCleared_(_count, _size)
#  define mem_allocateWith(_size, _src)      mem_allocateWith_(_size, _src)
#  define mem_deallocate(_ptr)               mem_deallocate_(_ptr)
#  define mem_set(_dest, _val, _size)        mem_set_(_dest, _val, _size)
#  define mem_copy(_dest, _src, _size)       mem_copy_(_dest, _src, _size)
#  define mem_move(_dest, _src, _size)       mem_move_(_dest, _src, _size)
#endif /* NDEBUG */

// Debug memory management functions
#ifndef NDEBUG
anyopaque mem_allocate_(usize size, const char* func, const char* file, i32 line);
anyopaque mem_allocateCleared_(usize count, usize size, const char* func, const char* file, i32 line);
anyopaque mem_allocateWith_(usize size, anyopaque src, const char* func, const char* file, i32 line);
void      mem_deallocate_(anyopaque ptr, const char* func, const char* file, i32 line);
anyopaque mem_set_(anyopaque dest, i32 val, usize size, const char* func, const char* file, i32 line);
anyopaque mem_copy_(anyopaque dest, const anyopaque src, usize size, const char* func, const char* file, i32 line);
anyopaque mem_move_(anyopaque dest, anyopaque src, usize size, const char* func, const char* file, i32 line);
#else
anyopaque mem_allocate_(usize size);
anyopaque mem_allocateCleared_(usize count, usize size);
anyopaque mem_allocateWith_(usize size, anyopaque src);
void      mem_deallocate_(anyopaque ptr);
anyopaque mem_set_(anyopaque dest, i32 val, usize size);
anyopaque mem_copy_(anyopaque dest, const anyopaque src, usize size);
anyopaque mem_move_(anyopaque dest, anyopaque src, usize size);
#endif /* NDEBUG */


#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* MEM_INCLUDED */
