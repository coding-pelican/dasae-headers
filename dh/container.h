// /**
//  * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
//  *
//  * @file    container.h
//  * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
//  * @date    2024-02-14 (date of creation)
//  * @version v1.0.0
//  * @ingroup dasae-headers(dh)
//  * @prefix  container
//  *
//  * @brief   Container types and operations inspired by ZIG
//  */


// #ifndef CONTAINER_INCLUDED
// #define CONTAINER_INCLUDED (1)
// #if defined(__cplusplus)
// extern "C" {
// #endif /* defined(__cplusplus) */


// #include "debug/debug_assert.h"
// #include "preprocessor.h"
// #include <stdlib.h>
// #include <string.h>


// /* TYPES DECLARATION */
// #define Array(T, N)    \
//     struct {           \
//         T     data[N]; \
//         usize len;     \
//     }

// #define Vector(T)       \
//     struct {            \
//         T*    data;     \
//         usize len;      \
//         usize capacity; \
//     }
// typedef Vector(any) Vector_any;

// #define Slice(T)   \
//     struct {       \
//         T*    ptr; \
//         usize len; \
//     }
// typedef Slice(any) Slice_any;


// /* PUBLIC INTERFACE */
// #define Array_at(arr, index) RETURN_Array_at(arr, index)
// #define Array_len(arr)       ((arr)->len)
// #define Array_clear(arr)     ((arr)->len = 0)

// #define Vector_init(type, cap) RETURN_Vector_init(type, cap)
// #define Vector_push(vec, val)  RETURN_Vector_push(vec, val)
// #define Vector_len(vec)        ((vec)->len)
// #define Vector_cap(vec)        ((vec)->capacity)
// #define Vector_clear(vec)      ((vec)->len = 0)

// #define Slice_from(data, start, length) RETURN_Slice_from(data, start, length)
// #define Slice_len(slice)                ((slice)->len)


// /* FUNCTIONS DECLARATION */
// static inline any array_get(any arr, usize index, usize elem_size) {
//     debug_assertNotNull(arr);
//     return (u8*)arr + (index * elem_size);
// }

// static inline any vector_alloc(usize capacity, usize elem_size) {
//     any data = malloc(capacity * elem_size);
//     debug_assertNotNull(data);
//     return data;
// }

// static inline void vector_free(any data) {
//     debug_assertNotNull(data);
//     free(data);
// }

// static inline any vector_grow(any data, usize* capacity, usize elem_size) {
//     debug_assertNotNull(data);
//     debug_assertNotNull(capacity);

//     usize new_capacity = *capacity * 2 + 1;
//     any   new_data     = realloc(data, new_capacity * elem_size);
//     debug_assertNotNull(new_data);
//     *capacity = new_capacity;
//     return new_data;
// }

// /* MACROS IMPLEMENTATION */
// #define RETURN_Array_at(arr, index) \
//     (*((T*)array_get(&(arr)->data, index, sizeof((arr)->data[0]))))

// #define RETURN_Vector_init(type, cap) pp_func(     \
//     { .data     = vector_alloc(cap, sizeof(type)), \
//       .len      = 0,                               \
//       .capacity = (cap) }                          \
// )

// #define RETURN_Vector_push(vec, val) pp_func( \
//     if ((vec)->len == (vec)->capacity) {      \
//         (vec)->data = vector_grow(            \
//             (vec)->data,                      \
//             &(vec)->capacity,                 \
//             sizeof(*(vec)->data)              \
//         );                                    \
//     }(vec)                                    \
//         ->data[(vec)->len++]                  \
//     = (val);                                  \
// )

// #define RETURN_Slice_from(data, start, length) \
//     ((any)((u8*)(data) + ((start) * sizeof(*(data)))))



// #if defined(__cplusplus)
// } /* extern "C" */
// #endif /* defined(__cplusplus) */
// #endif /* CONTAINER_INCLUDED */
