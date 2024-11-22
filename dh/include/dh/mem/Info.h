/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Info.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem_Info
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */
// TODO(dev-dasae): Move to 'mem/' and Rename to Info.h
// TODO(dev-dasae): Separate implementation from header
// TODO(dev-dasae): Use member `deallocTime` to track memory leaks
// TODO(dev-dasae): Document annotations with heading

#ifndef MEM_INFO_INCLUDED
#define MEM_INFO_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */


/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/debug/cfg.h"
#include "dh/core.h"

/*========== Macros and Definitions =========================================*/

#define mem_create(TYPE)                 IMPL_mem_create(TYPE)
#define mem_createCleared(TYPE)          IMPL_mem_createCleared(TYPE)
#define mem_createWith(TYPE, INITIAL...) IMPL_mem_createWith(TYPE, INITIAL)
#define mem_destroy(PTR_ADDR)            IMPL_mem_destroy(PTR_ADDR)

#define mem_alloc(TYPE, COUNT)                 IMPL_mem_alloc(TYPE, COUNT)
#define mem_allocCleared(TYPE, COUNT)          IMPL_mem_allocCleared(TYPE, COUNT)
#define mem_allocWith(TYPE, COUNT, INITIAL...) IMPL_mem_allocWith(TYPE, COUNT, INITIAL)
#define mem_free(PTR_ADDR)                     IMPL_mem_free(PTR_ADDR)

/* Debug macro definitions */
#if defined(MEM_TRACE_ENABLED) && MEM_TRACE_ENABLED
#define mem_allocate(SIZE)               mem__allocate(SIZE, __func__, __FILE__, __LINE__)
#define mem_allocateCleared(SIZE, COUNT) mem__allocateCleared(SIZE, COUNT, __func__, __FILE__, __LINE__)
#define mem_allocateWith(SIZE, SRC)      mem__allocateWith(SIZE, SRC, __func__, __FILE__, __LINE__)
#define mem_reallocate(PTR, SIZE)        mem__reallocate(PTR, SIZE, __func__, __FILE__, __LINE__)
#define mem_deallocate(PTR_ADDR)         mem__deallocate((anyptr*)(PTR_ADDR), __func__, __FILE__, __LINE__)
#define mem_set(DEST, VAL, SIZE)         mem__set(DEST, VAL, SIZE, __func__, __FILE__, __LINE__)
#define mem_copy(DEST, SRC, SIZE)        mem__copy(DEST, SRC, SIZE, __func__, __FILE__, __LINE__)
#define mem_move(DEST, SRC, SIZE)        mem__move(DEST, SRC, SIZE, __func__, __FILE__, __LINE__)
#else
#define mem_allocate(SIZE)               mem__allocate(SIZE)
#define mem_allocateCleared(SIZE, COUNT) mem__allocateCleared(SIZE, COUNT)
#define mem_allocateWith(SIZE, SRC)      mem__allocateWith(SIZE, SRC)
#define mem_reallocate(PTR, SIZE)        mem__reallocate(PTR, SIZE)
#define mem_deallocate(PTR_ADDR)         mem__deallocate((anyptr*)(PTR_ADDR))
#define mem_set(DEST, VAL, SIZE)         mem__set(DEST, VAL, SIZE)
#define mem_copy(DEST, SRC, SIZE)        mem__copy(DEST, SRC, SIZE)
#define mem_move(DEST, SRC, SIZE)        mem__move(DEST, SRC, SIZE)
#endif

#if defined(MEM_TRACE_ENABLED) && MEM_TRACE_ENABLED
/* Memory tracking structure */
typedef struct mem_Info mem_Info;
struct mem_Info {
    anyptr      ptr;          /* Pointer to allocated memory */
    usize       size;         /* Size of allocation */
    const char* func;         /* Function name */
    const char* file;         /* Source file */
    i32         line;         /* Line number */
    time_t      alloc_time;   /* Allocation timestamp */
    time_t      dealloc_time; /* Deallocation timestamp */
    i32         ref_count;    /* Reference count */
    mem_Info*   next;         /* Next in linked list */
};

/*
 * // Global memory tracking list
 * static mem_Info*                         mem_s_info_list = null;
 * // Register atexit handler for memory leak detection
 * static void __attribute__((constructor)) mem_initInfoList(void);
 * static void __attribute__((destructor))  mem_finiInfoList(void);
 * // Memory tracking functions
 * static void                              mem_addInfo(anyptr allocated, usize size, const char* func, const char* file, i32 line);
 * static void                              mem_removeInfo(anyptr target);
 * static void
 * mem__printInfoMemoryLeakTrace(void);
 */
#endif /* defined(MEM_TRACE_ENABLED) && MEM_TRACE_ENABLED */

/*========== Macros Implementation ==========================================*/

#define IMPL_mem_create(TYPE)          (Ptr(TYPE)) mem_allocate(sizeof(TYPE))
#define IMPL_mem_createCleared(TYPE)   (Ptr(TYPE)) mem_allocateCleared(sizeof(TYPE), 0)
#define IMPL_mem_createWith(TYPE, ...) (Ptr(TYPE)) mem_allocateWith(sizeof(TYPE), &makeWith(TYPE, __VA_ARGS__))
#define IMPL_mem_destroy(PTR)          mem_deallocate((anyptr*)(PTR))

#define IMPL_mem_alloc(TYPE, COUNT)          (Ptr(TYPE)) mem_allocate(sizeof(TYPE) * (COUNT))
#define IMPL_mem_allocCleared(TYPE, COUNT)   (Ptr(TYPE)) mem_allocateCleared(sizeof(TYPE), COUNT)
#define IMPL_mem_allocWith(TYPE, COUNT, ...) (Ptr(TYPE)) mem_allocateWith(sizeof(TYPE) * COUNT, &nArrayWith(COUNT, TYPE, __VA_ARGS__))
#define IMPL_mem_free(PTR)                   mem_deallocate((anyptr*)(PTR))

/*========== Extern Function Prototypes =====================================*/

/* Debug memory management functions */
#if defined(MEM_TRACE_ENABLED) && MEM_TRACE_ENABLED
extern anyptr mem__allocate(usize size, const char* func, const char* file, i32 line);
extern anyptr mem__allocateCleared(usize size, usize count, const char* func, const char* file, i32 line);
extern anyptr mem__allocateWith(usize size, anyptr src, const char* func, const char* file, i32 line);
extern anyptr mem__reallocate(anyptr ptr, usize size, const char* func, const char* file, i32 line);
extern void   mem__deallocate(anyptr* ptr_addr, const char* func, const char* file, i32 line);
extern anyptr mem__set(anyptr dest, i32 val, usize size, const char* func, const char* file, i32 line);
extern anyptr mem__copy(anyptr dest, const anyptr src, usize size, const char* func, const char* file, i32 line);
extern anyptr mem__move(anyptr dest, anyptr src, usize size, const char* func, const char* file, i32 line);
#else
extern anyptr mem__allocate(usize size);
extern anyptr mem__allocateCleared(usize size, usize count);
extern anyptr mem__allocateWith(usize size, anyptr src);
extern anyptr mem__reallocate(anyptr ptr, usize size);
extern void   mem__deallocate(anyptr* ptr_addr);
extern anyptr mem__set(anyptr dest, i32 val, usize size);
extern anyptr mem__copy(anyptr dest, const anyptr src, usize size);
extern anyptr mem__move(anyptr dest, anyptr src, usize size);
#endif

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* MEM_INFO_INCLUDED */
