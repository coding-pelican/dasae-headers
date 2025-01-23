/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Allocator.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-07 (date of creation)
 * @updated 2025-01-09 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem_Allocator
 *
 * @brief   Memory allocator interface using smart pointers
 */

#ifndef MEM_ALLOCATOR_INCLUDED
#define MEM_ALLOCATOR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/meta/common.h"

/*========== Allocator Interface ============================================*/

/* Allocator vtable */
typedef struct mem_AllocatorVT {
    Opt$Ptr$u8 (*alloc)(anyptr ctx, usize len, usize ptr_align) must_check;
    bool (*resize)(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_len) must_check;
    void (*free)(anyptr ctx, Sli$u8 buf, usize buf_align);
} mem_AllocatorVT;

/* Allocator instance */
typedef struct mem_Allocator {
    anyptr                 ptr; /* Context pointer with type info */
    const mem_AllocatorVT* vt;  /* Virtual table */
} mem_Allocator;
use_Opt$(mem_Allocator);

/*========== Core Allocator Functions =======================================*/

#if defined(MEM_NO_TRACE_ALLOC_AND_FREE) || (!COMP_TIME || (COMP_TIME && !DEBUG_ENABLED))

/* Raw allocation */
extern Opt$Ptr$u8 mem_Allocator_rawAlloc(mem_Allocator self, usize len, usize ptr_align) must_check;
/* Try to resize in-place */
extern bool       mem_Allocator_rawResize(mem_Allocator self, Sli$u8 buf, usize buf_align, usize new_len) must_check;
/* Free memory */
extern void       mem_Allocator_rawFree(mem_Allocator self, Sli$u8 buf, usize buf_align);

#else /* !defined(MEM_NO_TRACE_ALLOC_AND_FREE) && (COMP_TIME && (!COMP_TIME || DEBUG_ENABLED)) */

#define mem_Allocator_rawAlloc(mem_Allocator_self, usize_len, usize_ptr_align...) \
    mem_Allocator_rawAlloc_callDebug(                                             \
        mem_Allocator_self,                                                       \
        usize_len,                                                                \
        usize_ptr_align,                                                          \
        __FILE__,                                                                 \
        __LINE__,                                                                 \
        __func__                                                                  \
    )
#define mem_Allocator_rawResize(mem_Allocator_self, Sli$u8_buf, usize_buf_align, usize_new_len...) \
    mem_Allocator_rawResize_callDebug(                                                             \
        mem_Allocator_self,                                                                        \
        Sli$u8_buf,                                                                                \
        usize_buf_align,                                                                           \
        usize_new_len,                                                                             \
        __FILE__,                                                                                  \
        __LINE__,                                                                                  \
        __func__                                                                                   \
    )
#define mem_Allocator_rawFree(mem_Allocator_self, Sli$u8_buf, usize_buf_align...) \
    mem_Allocator_rawFree_callDebug(                                              \
        mem_Allocator_self,                                                       \
        Sli$u8_buf,                                                               \
        usize_buf_align,                                                          \
        __FILE__,                                                                 \
        __LINE__,                                                                 \
        __func__                                                                  \
    )

#define mem_Allocator_rawAlloc_callDebug(mem_Allocator_self, usize_len, usize_ptr_align, const_char_ptr_file, i32_line, const_char_ptr_func, ...) \
    mem_Allocator_rawAlloc_debug(                                                                                                                 \
        mem_Allocator_self,                                                                                                                       \
        usize_len,                                                                                                                                \
        usize_ptr_align,                                                                                                                          \
        const_char_ptr_file,                                                                                                                      \
        i32_line,                                                                                                                                 \
        const_char_ptr_func                                                                                                                       \
    )
#define mem_Allocator_rawResize_callDebug(mem_Allocator_self, Sli$u8_buf, usize_buf_align, usize_new_len, const_char_ptr_file, i32_line, const_char_ptr_func, ...) \
    mem_Allocator_rawResize_debug(                                                                                                                                 \
        mem_Allocator_self,                                                                                                                                        \
        Sli$u8_buf,                                                                                                                                                \
        usize_buf_align,                                                                                                                                           \
        usize_new_len,                                                                                                                                             \
        const_char_ptr_file,                                                                                                                                       \
        i32_line,                                                                                                                                                  \
        const_char_ptr_func                                                                                                                                        \
    )
#define mem_Allocator_rawFree_callDebug(mem_Allocator_self, Sli$u8_buf, usize_buf_align, const_char_ptr_file, i32_line, const_char_ptr_func, ...) \
    mem_Allocator_rawFree_debug(                                                                                                                  \
        mem_Allocator_self,                                                                                                                       \
        Sli$u8_buf,                                                                                                                               \
        usize_buf_align,                                                                                                                          \
        const_char_ptr_file,                                                                                                                      \
        i32_line,                                                                                                                                 \
        const_char_ptr_func                                                                                                                       \
    )

extern Opt$Ptr$u8 mem_Allocator_rawAlloc_debug(mem_Allocator self, usize len, usize ptr_align, const char* file, i32 line, const char* func) must_check;
extern bool       mem_Allocator_rawResize_debug(mem_Allocator self, Sli$u8 buf, usize buf_align, usize new_len, const char* file, i32 line, const char* func) must_check;
extern void       mem_Allocator_rawFree_debug(mem_Allocator self, Sli$u8 buf, usize buf_align, const char* file, i32 line, const char* func);

#endif /* defined(MEM_NO_TRACE_ALLOC_AND_FREE) || (!COMP_TIME || (COMP_TIME && !DEBUG_ENABLED)) */

/*========== High-level Allocator Functions =================================*/

#if defined(MEM_NO_TRACE_ALLOC_AND_FREE) || (!COMP_TIME || (COMP_TIME && !DEBUG_ENABLED))

/* Single-item allocation */
extern Err$meta_Ptr mem_Allocator_create(mem_Allocator self, TypeInfo type) must_check;
/* Free single-item */
extern void         mem_Allocator_destroy(mem_Allocator self, AnyType ptr);
/* Sli allocation */
extern Err$meta_Sli mem_Allocator_alloc(mem_Allocator self, TypeInfo type, usize count) must_check;
/* Try to resize slice in-place */
extern bool         mem_Allocator_resize(mem_Allocator self, AnyType old_mem, usize new_len) must_check;
/* Reallocate slice with new size */
extern Opt$meta_Sli mem_Allocator_realloc(mem_Allocator self, AnyType old_mem, usize new_len) must_check;
/* Free slice */
extern void         mem_Allocator_free(mem_Allocator self, AnyType memory);

#else /* !defined(MEM_NO_TRACE_ALLOC_AND_FREE) && (COMP_TIME && (!COMP_TIME || DEBUG_ENABLED)) */

#define mem_Allocator_create(mem_Allocator_self, TypeInfo_type...) \
    mem_Allocator_create_callDebug(                                \
        mem_Allocator_self,                                        \
        TypeInfo_type,                                             \
        __FILE__,                                                  \
        __LINE__,                                                  \
        __func__                                                   \
    )
#define mem_Allocator_destroy(mem_Allocator_self, AnyType_ptr...) \
    mem_Allocator_destroy_callDebug(                              \
        mem_Allocator_self,                                       \
        AnyType_ptr,                                              \
        __FILE__,                                                 \
        __LINE__,                                                 \
        __func__                                                  \
    )
#define mem_Allocator_alloc(mem_Allocator_self, TypeInfo_type, usize_count...) \
    mem_Allocator_alloc_callDebug(                                             \
        mem_Allocator_self,                                                    \
        TypeInfo_type,                                                         \
        usize_count,                                                           \
        __FILE__,                                                              \
        __LINE__,                                                              \
        __func__                                                               \
    )
#define mem_Allocator_resize(mem_Allocator_self, AnyType_old_mem, usize_new_len...) \
    mem_Allocator_resize_callDebug(                                                 \
        mem_Allocator_self,                                                         \
        AnyType_old_mem,                                                            \
        usize_new_len,                                                              \
        __FILE__,                                                                   \
        __LINE__,                                                                   \
        __func__                                                                    \
    )
#define mem_Allocator_realloc(mem_Allocator_self, AnyType_old_mem, usize_new_len...) \
    mem_Allocator_realloc_callDebug(                                                 \
        mem_Allocator_self,                                                          \
        AnyType_old_mem,                                                             \
        usize_new_len,                                                               \
        __FILE__,                                                                    \
        __LINE__,                                                                    \
        __func__                                                                     \
    )
#define mem_Allocator_free(mem_Allocator_self, AnyType_memory...) \
    mem_Allocator_free_callDebug(                                 \
        mem_Allocator_self,                                       \
        AnyType_memory,                                           \
        __FILE__,                                                 \
        __LINE__,                                                 \
        __func__                                                  \
    )

#define mem_Allocator_create_callDebug(mem_Allocator_self, TypeInfo_type, const_char_ptr_file, i32_line, const_char_ptr_func, ...) \
    mem_Allocator_create_debug(                                                                                                    \
        mem_Allocator_self,                                                                                                        \
        TypeInfo_type,                                                                                                             \
        const_char_ptr_file,                                                                                                       \
        i32_line,                                                                                                                  \
        const_char_ptr_func                                                                                                        \
    )
#define mem_Allocator_destroy_callDebug(mem_Allocator_self, AnyType_ptr, const_char_ptr_file, i32_line, const_char_ptr_func, ...) \
    mem_Allocator_destroy_debug(                                                                                                  \
        mem_Allocator_self,                                                                                                       \
        AnyType_ptr,                                                                                                              \
        const_char_ptr_file,                                                                                                      \
        i32_line,                                                                                                                 \
        const_char_ptr_func                                                                                                       \
    )
#define mem_Allocator_alloc_callDebug(mem_Allocator_self, TypeInfo_type, usize_count, const_char_ptr_file, i32_line, const_char_ptr_func, ...) \
    mem_Allocator_alloc_debug(                                                                                                                 \
        mem_Allocator_self,                                                                                                                    \
        TypeInfo_type,                                                                                                                         \
        usize_count,                                                                                                                           \
        const_char_ptr_file,                                                                                                                   \
        i32_line,                                                                                                                              \
        const_char_ptr_func                                                                                                                    \
    )
#define mem_Allocator_resize_callDebug(mem_Allocator_self, AnyType_old_mem, usize_new_len, const_char_ptr_file, i32_line, const_char_ptr_func, ...) \
    mem_Allocator_resize_debug(                                                                                                                     \
        mem_Allocator_self,                                                                                                                         \
        AnyType_old_mem,                                                                                                                            \
        usize_new_len,                                                                                                                              \
        const_char_ptr_file,                                                                                                                        \
        i32_line,                                                                                                                                   \
        const_char_ptr_func                                                                                                                         \
    )
#define mem_Allocator_realloc_callDebug(mem_Allocator_self, AnyType_old_mem, usize_new_len, const_char_ptr_file, i32_line, const_char_ptr_func, ...) \
    mem_Allocator_realloc_debug(                                                                                                                     \
        mem_Allocator_self,                                                                                                                          \
        AnyType_old_mem,                                                                                                                             \
        usize_new_len,                                                                                                                               \
        const_char_ptr_file,                                                                                                                         \
        i32_line,                                                                                                                                    \
        const_char_ptr_func                                                                                                                          \
    )
#define mem_Allocator_free_callDebug(mem_Allocator_self, AnyType_memory, const_char_ptr_file, i32_line, const_char_ptr_func, ...) \
    mem_Allocator_free_debug(                                                                                                     \
        mem_Allocator_self,                                                                                                       \
        AnyType_memory,                                                                                                           \
        const_char_ptr_file,                                                                                                      \
        i32_line,                                                                                                                 \
        const_char_ptr_func                                                                                                       \
    )

extern Err$meta_Ptr mem_Allocator_create_debug(mem_Allocator self, TypeInfo type, const char* file, i32 line, const char* func) must_check;
extern void         mem_Allocator_destroy_debug(mem_Allocator self, AnyType ptr, const char* file, i32 line, const char* func);
extern Err$meta_Sli mem_Allocator_alloc_debug(mem_Allocator self, TypeInfo type, usize count, const char* file, i32 line, const char* func) must_check;
extern bool         mem_Allocator_resize_debug(mem_Allocator self, AnyType old_mem, usize new_len, const char* file, i32 line, const char* func) must_check;
extern Opt$meta_Sli mem_Allocator_realloc_debug(mem_Allocator self, AnyType old_mem, usize new_len, const char* file, i32 line, const char* func) must_check;
extern void         mem_Allocator_free_debug(mem_Allocator self, AnyType memory, const char* file, i32 line, const char* func);

#endif /* defined(MEM_NO_TRACE_ALLOC_AND_FREE) || (!COMP_TIME || (COMP_TIME && !DEBUG_ENABLED)) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_ALLOCATOR_INCLUDED */
