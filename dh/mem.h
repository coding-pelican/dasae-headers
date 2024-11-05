/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    mem.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-11-03 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */
// TODO(dev-dasae): Separate implementation from header
// TODO(dev-dasae): Use member `deallocTime` to track memory leaks
// TODO(dev-dasae): Document annotations with heading


#ifndef MEM_INCLUDED
#define MEM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


/*========== Includes =======================================================*/

#include "debug/debug_cfg.h"
#include "types.h"

/*========== Macros and Definitions =========================================*/

#define mem_create(_T)                  RETURN_mem_create(_T)
#define mem_createCleared(_T)           RETURN_mem_createCleared(_T)
#define mem_createWith(_T, _initial...) RETURN_mem_createWith(_T, _initial)
#define mem_destroy(_ptr)               RETURN_mem_destroy(_ptr)

#define mem_new(_T, _count)                  RETURN_mem_new(_T, _count)
#define mem_newCleared(_T, _count)           RETURN_mem_newCleared(_T, _count)
#define mem_newWith(_T, _count, _initial...) RETURN_mem_newWith(_T, _count, _initial)
#define mem_delete(_ptr)                     RETURN_mem_delete(_ptr)

// Debug macro definitions
#if defined(DEBUG_ENABLED) && DEBUG_ENABLED
#define mem_allocate(_size)                mem__allocate(_size, __func__, __FILE__, __LINE__)
#define mem_allocateCleared(_size, _count) mem__allocateCleared(_size, _count, __func__, __FILE__, __LINE__)
#define mem_allocateWith(_size, _src)      mem__allocateWith(_size, _src, __func__, __FILE__, __LINE__)
#define mem_reallocate(_ptr, _size)        mem__reallocate(_ptr, _size, __func__, __FILE__, __LINE__)
#define mem_deallocate(_ptr)               mem__deallocate(_ptr, __func__, __FILE__, __LINE__)
#define mem_set(_dest, _val, _size)        mem__set(_dest, _val, _size, __func__, __FILE__, __LINE__)
#define mem_copy(_dest, _src, _size)       mem__copy(_dest, _src, _size, __func__, __FILE__, __LINE__)
#define mem_move(_dest, _src, _size)       mem__move(_dest, _src, _size, __func__, __FILE__, __LINE__)
#else
#define mem_allocate(_size)                mem__allocate(_size)
#define mem_allocateCleared(_size, _count) mem__allocateCleared(_size, _count)
#define mem_allocateWith(_size, _src)      mem__allocateWith(_size, _src)
#define mem_reallocate(_ptr, _size)        mem__reallocate(_ptr, _size)
#define mem_deallocate(_ptr)               mem__deallocate(_ptr)
#define mem_set(_dest, _val, _size)        mem__set(_dest, _val, _size)
#define mem_copy(_dest, _src, _size)       mem__copy(_dest, _src, _size)
#define mem_move(_dest, _src, _size)       mem__move(_dest, _src, _size)
#endif

#if defined(DEBUG_ENABLED) && DEBUG_ENABLED
// Memory tracking structure
typedef struct memInfo memInfo;
struct memInfo {
    anyptr      ptr;          // Pointer to allocated memory
    usize       size;         // Size of allocation
    const char* func;         // Function name
    const char* file;         // Source file
    i32         line;         // Line number
    time_t      alloc_time;   // Allocation timestamp
    time_t      dealloc_time; // Deallocation timestamp
    i32         ref_count;    // Reference count
    memInfo*    next;         // Next in linked list
};
// Global memory tracking list
extern memInfo*                          mem__info_list;
// Register atexit handler for memory leak detection
static void __attribute__((constructor)) mem__initInfoList(void);
static void __attribute__((destructor))  mem__finiInfoList(void);
// Memory tracking functions
static void                              mem__addInfo(anyptr allocated, usize size, const char* func, const char* file, i32 line);
static void                              mem__removeInfo(anyptr target);
static void                              mem__printInfoMemoryLeakTrace(void);
#endif /* defined(DEBUG_ENABLED) && DEBUG_ENABLED */


#define RETURN_mem_create(_T)          (Ptr(_T)) mem_allocate(sizeof(_T))
#define RETURN_mem_createCleared(_T)   (Ptr(_T)) mem_allocateCleared(sizeof(_T), 0)
#define RETURN_mem_createWith(_T, ...) (Ptr(_T)) mem_allocateWith(sizeof(_T), &makeWith(_T, __VA_ARGS__))
#define RETURN_mem_destroy(_ptr)       mem_deallocate(_ptr)

#define RETURN_mem_new(_T, _count)          (Ptr(_T)) mem_allocate(sizeof(_T) * (_count))
#define RETURN_mem_newCleared(_T, _count)   (Ptr(_T)) mem_allocateCleared(sizeof(_T), _count)
#define RETURN_mem_newWith(_T, _count, ...) (Ptr(_T)) mem_allocateWith(sizeof(_T) * _count, &nArrayWith(_count, _T, __VA_ARGS__))
#define RETURN_mem_delete(_ptr)             mem_deallocate(_ptr)

// Debug memory management functions
#if defined(DEBUG_ENABLED) && DEBUG_ENABLED
anyptr mem__allocate(usize size, const char* func, const char* file, i32 line);
anyptr mem__allocateCleared(usize size, usize count, const char* func, const char* file, i32 line);
anyptr mem__allocateWith(usize size, anyptr src, const char* func, const char* file, i32 line);
anyptr mem__reallocate(anyptr ptr, usize size, const char* func, const char* file, i32 line);
void   mem__deallocate(anyptr ptr, const char* func, const char* file, i32 line);
anyptr mem__set(anyptr dest, i32 val, usize size, const char* func, const char* file, i32 line);
anyptr mem__copy(anyptr dest, const anyptr src, usize size, const char* func, const char* file, i32 line);
anyptr mem__move(anyptr dest, anyptr src, usize size, const char* func, const char* file, i32 line);
#else
anyptr mem__allocate(usize size);
anyptr mem__allocateCleared(usize size, usize count);
anyptr mem__allocateWith(usize size, anyptr src);
anyptr mem__reallocate(anyptr ptr, usize size);
void   mem__deallocate(anyptr ptr);
anyptr mem__set(anyptr dest, i32 val, usize size);
anyptr mem__copy(anyptr dest, const anyptr src, usize size);
anyptr mem__move(anyptr dest, anyptr src, usize size);
#endif


#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* MEM_INCLUDED */
