/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    container.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated 2024-11-10 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/ds
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */
// TODO(dev-dasae): Improve generic usability, foreach, etc.


#ifndef DS_CONTAINER_INCLUDED
#define DS_CONTAINER_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include <dh/core.h>
#include <dh/mem.h>

/*========== Forward Declarations ===========================================*/
/*========== Macros and Definitions =========================================*/

/* Base container structure */
typedef struct Container {
    usize elem_size;
    usize length;
    union {
        anyptr data;
        u8*    bytes;
    };
} Container;

/* Base array structure */
typedef struct Array {
    Container container;
    anyptr    fixed;
} Array;

/* Base vector structure */
typedef struct Vector {
    Container container;
    usize     capacity;
} Vector;

#define TArrayN(_T, _N)                 \
    /* Generic array type definition */ \
    RETURN_TArrayN(_T, _N)
#define TVector(_T)                      \
    /* Generic vector type definition */ \
    RETURN_TVector(_T)

/* Base container operations (force_inline functions) */

force_inline anyptr       Container_peekData(const Container* c) { return c->data; }
force_inline anyptr       Container_accessData(Container* c) { return c->data; }
force_inline const u8*    Container_peekAsBytes(const Container* c) { return c->bytes; }
force_inline u8*          Container_accessAsBytes(Container* c) { return c->bytes; }
force_inline usize        Container_length(const Container* c) { return c->length; }
force_inline const anyptr Container_peekAt(const Container* c, usize index) {
    return (index < c->length)
             ? (const anyptr)((const u8*)c->data + (index * c->elem_size))
             : null;
}
force_inline anyptr Container_accessAt(Container* c, usize index) {
    return (index < c->length)
             ? (anyptr)((u8*)c->data + (index * c->elem_size))
             : null;
}

/* Alias for generic containers */

#define Array(_T, _N) RETURN_Array(_T, _N)
#define Vector(_T)    RETURN_Vector(_T)

/* Generic method wrappers */

#define Array_peekData(_T, _arr)       RETURN_Array_peekData(_T, _arr)
#define Array_accessData(_T, _arr)     RETURN_Array_accessData(_T, _arr)
#define Array_peekAsBytes(_T, _arr)    RETURN_Array_peekAsBytes(_T, _arr)
#define Array_accessAsBytes(_T, _arr)  RETURN_Array_accessAsBytes(_T, _arr)
#define Array_length(_T, _arr)         RETURN_Array_length(_T, _arr)
#define Array_peekAt(_T, _arr, _idx)   RETURN_Array_peekAt(_T, _arr, _idx)
#define Array_accessAt(_T, _arr, _idx) RETURN_Array_accessAt(_T, _arr, _idx)

#define Vector_peekData(_T, _vec)       RETURN_Vector_peekData(_T, _vec)
#define Vector_accessData(_T, _vec)     RETURN_Vector_accessData(_T, _vec)
#define Vector_peekAsBytes(_T, _vec)    RETURN_Vector_peekAsBytes(_T, _vec)
#define Vector_accessAsBytes(_T, _vec)  RETURN_Vector_accessAsBytes(_T, _vec)
#define Vector_length(_T, _vec)         RETURN_Vector_length(_T, _vec)
#define Vector_peekAt(_T, _vec, _idx)   RETURN_Vector_peekAt(_T, _vec, _idx)
#define Vector_accessAt(_T, _vec, _idx) RETURN_Vector_accessAt(_T, _vec, _idx)

/* Type-safe element access macros */

#define Array_get(_T, _arr, _idx) \
    (*Array_peekAt(_T, _arr, _idx))

#define Array_getMut(_T, _arr, _idx) \
    (*Array_accessAt(_T, _arr, _idx))

#define Array_set(_T, _arr, _idx, _val) \
    (*Array_accessAt(_T, _arr, _idx)) = _val

#define Vector_get(_T, _vec, _idx) \
    (*Vector_peekAt(_T, _vec, _idx))

#define Vector_getMut(_T, _vec, _idx) \
    (*Vector_accessAt(_T, _vec, _idx))

#define Vector_set(_T, _vec, _idx, _val) \
    (*Vector_accessAt(_T, _vec, _idx)) = _val

/*========== Macros Implementation ==========================================*/

#define RETURN_TArrayN(_T, _N)                                      \
    struct Array(_T, _N) {                                          \
        Container container;                                        \
        _T        fixed[_N];                                        \
    };                                                              \
    static void _T##Array##_N##_init(struct Array(_T, _N) * _arr) { \
        _arr->container = (Container){                              \
            .data      = _arr->fixed,                               \
            .length    = (_N),                                      \
            .elem_size = sizeof(_T)                                 \
        };                                                          \
    }                                                               \
    typedef struct Array(_T, _N) Array(_T, _N)

#define RETURN_TVector(_T)                                                                     \
    struct Vector(_T) {                                                                        \
        Container container;                                                                   \
        usize     capacity;                                                                    \
    };                                                                                         \
    static void _T##Vector##_init(struct Vector(_T) * _vec, usize initial_capacity) {          \
        *_vec = (struct Vector(_T)){                                                           \
            .container = {                                                                     \
                          .data      = initial_capacity ? mem_allocCleared(_T, initial_capacity) : null, \
                          .length    = 0,                                                                \
                          .elem_size = sizeof(_T) },                                                     \
            .capacity = initial_capacity                                                       \
        };                                                                                     \
    }                                                                                          \
    static void _T##Vector##_fini(struct Vector(_T) * _vec) {                                  \
        mem_free(&(_vec)->container.data);                                                     \
        _vec->container.length = 0;                                                            \
        _vec->capacity         = 0;                                                            \
    }                                                                                          \
    typedef struct Vector(_T) Vector(_T)

#define RETURN_Array(_T, _N) _T##Array##_N
#define RETURN_Vector(_T)    _T##Vector

#define RETURN_Array_peekData(_T, _arr)       (const _T*)Container_peekData(&(_arr)->container)
#define RETURN_Array_accessData(_T, _arr)     (_T*)Container_accessData(&(_arr)->container)
#define RETURN_Array_peekAsBytes(_T, _arr)    Container_peekAsBytes(&(_arr)->container)
#define RETURN_Array_accessAsBytes(_T, _arr)  Container_accessAsBytes(&(_arr)->container)
#define RETURN_Array_length(_T, _arr)         Container_length(&(_arr)->container)
#define RETURN_Array_peekAt(_T, _arr, _idx)   (const _T*)Container_peekAt(&(_arr)->container, _idx)
#define RETURN_Array_accessAt(_T, _arr, _idx) (_T*)Container_accessAt(&(_arr)->container, _idx)

#define RETURN_Vector_peekData(_T, _vec)       (const _T*)Container_peekData(&(_vec)->container)
#define RETURN_Vector_accessData(_T, _vec)     (_T*)Container_accessData(&(_vec)->container)
#define RETURN_Vector_peekAsBytes(_T, _vec)    Container_peekAsBytes(&(_vec)->container)
#define RETURN_Vector_accessAsBytes(_T, _vec)  Container_accessAsBytes(&(_vec)->container)
#define RETURN_Vector_length(_T, _vec)         Container_length(&(_vec)->container)
#define RETURN_Vector_peekAt(_T, _vec, _idx)   (const _T*)Container_peekAt(&(_vec)->container, _idx)
#define RETURN_Vector_accessAt(_T, _vec, _idx) (_T*)Container_accessAt(&(_vec)->container, _idx)

/*========== Extern Constant and Variable Declarations ======================*/
/*========== Extern Function Prototypes =====================================*/
/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST

TArrayN(f32, 4);
TVector(f32);

void TEST_container_f32ArrayAndVector(void) {
    // Array usage
    f32Array4 _arr = make(f32Array4);
    f32Array4_init(&_arr);

    // Initialize array elements
    f32* data = Array_accessData(f32, &_arr);
    data[0]   = 1.0f;
    data[1]   = 2.0f;
    data[2]   = 3.0f;
    data[3]   = 4.0f;

    // Access elements safely
    f32 first                   = Array_get(f32, &_arr, 0);
    Array_getMut(f32, &_arr, 1) = 5.0f;

    // Vector usage
    f32Vector _vec = make(f32Vector);
    f32Vector_init(&_vec, 4);
    f32Vector_fini(&_vec);

    // Cleanup vector
    mem_free(_vec.container.data);
}

#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DS_CONTAINER_INCLUDED */


// #define Slice(_T)   \
//     struct {       \
//         _T*    ptr; \
//         usize len; \
//     }
// typedef Slice(any) Slice_any;


// /* PUBLIC INTERFACE */
// #define Array_at(_arr, index) RETURN_Array_at(_arr, index)
// #define Array_len(_arr)       ((_arr)->len)
// #define Array_clear(_arr)     ((_arr)->len = 0)

// #define Vector_init(type, cap) RETURN_Vector_init(type, cap)
// #define Vector_push(_vec, val)  RETURN_Vector_push(_vec, val)
// #define Vector_len(_vec)        ((_vec)->len)
// #define Vector_cap(_vec)        ((_vec)->capacity)
// #define Vector_clear(_vec)      ((_vec)->len = 0)

// #define Slice_from(data, start, length) RETURN_Slice_from(data, start, length)
// #define Slice_len(slice)                ((slice)->len)


// /* FUNCTIONS DECLARATION */
// force_inline any array_get(any _arr, usize index, usize elem_size) {
//     debug_assertNotnull(_arr);
//     return (u8*)_arr + (index * elem_size);
// }

// force_inline any vector_alloc(usize capacity, usize elem_size) {
//     any data = malloc(capacity * elem_size);
//     debug_assertNotnull(data);
//     return data;
// }

// force_inline void vector_free(any data) {
//     debug_assertNotnull(data);
//     free(data);
// }

// force_inline any vector_grow(any data, usize* capacity, usize elem_size) {
//     debug_assertNotnull(data);
//     debug_assertNotnull(capacity);

//     usize new_capacity = *capacity * 2 + 1;
//     any   new_data     = realloc(data, new_capacity * elem_size);
//     debug_assertNotnull(new_data);
//     *capacity = new_capacity;
//     return new_data;
// }

// /* MACROS IMPLEMENTATION */
// #define RETURN_Array_at(_arr, index) \
//     (*((_T*)array_get(&(_arr)->data, index, sizeof((_arr)->data[0]))))

// #define RETURN_Vector_init(type, cap) pp_func(     \
//     { .data     = vector_alloc(cap, sizeof(type)), \
//       .len      = 0,                               \
//       .capacity = (cap) }                          \
// )

// #define RETURN_Vector_push(_vec, val) pp_func( \
//     if ((_vec)->len == (_vec)->capacity) {      \
//         (_vec)->data = vector_grow(            \
//             (_vec)->data,                      \
//             &(_vec)->capacity,                 \
//             sizeof(*(_vec)->data)              \
//         );                                    \
//     }(_vec)                                    \
//         ->data[(_vec)->len++]                  \
//     = (val);                                  \
// )

// #define RETURN_Slice_from(data, start, length) \
//     ((any)((u8*)(data) + ((start) * sizeof(*(data)))))
