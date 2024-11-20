/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Vec.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-17 (date of creation)
 * @updated 2024-11-17 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/ds
 * @prefix  ds_Vec
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef DS_VEC_INCLUDED
#define DS_VEC_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/gen_expt.h"

/*========== Macros and Definitions =========================================*/

// TODO: Consider fat pointer and expanded member visibility scope of struct

// typedef struct ds_Vec {
//     struct_GenericBase(
//         ds_Vec,
//         usize cap;
//         usize elem_size;
//     );
//     usize len;
//     union {
//         u8*    bytes;
//         anyptr data;
//     };
// } ds_Vec;

// TODO: Add Byte-Wise Cmp and Ops
// TODO: Replace 'void*' to expt_SizedPtr
// TODO: Use expt_SizedPtr insted of elem_size with bytes-data's union

typedef_struct(ds_Vec) {
    usize elem_size;
    usize cap;
    usize len;
    union {
        u8*    bytes;
        anyptr data;
    };
};

#define ds_Vec(_T) \
    ds_Vec

/*========== Extern Function Prototypes =====================================*/

#define ds_Vec_init(_T, _vec)              ds_Vec__init(_vec, sizeof(_T))
#define ds_Vec_initWithCap(_T, _vec, _cap) ds_Vec__initWithCap(_vec, sizeof(_T), _cap)
extern ds_Vec* ds_Vec_fini(ds_Vec* vec);

extern usize ds_Vec_cap(const ds_Vec* vec);
extern usize ds_Vec_len(const ds_Vec* vec);
extern bool  ds_Vec_isEmpty(const ds_Vec* vec);
extern bool  ds_Vec_isFull(const ds_Vec* vec);

extern const anyptr ds_Vec_data(const ds_Vec* vec);
extern anyptr       ds_Vec_mut_data(ds_Vec* vec);
extern const anyptr ds_Vec_dataOrNull(const ds_Vec* vec);
extern anyptr       ds_Vec_mut_dataOrNull(ds_Vec* vec);

extern const u8* ds_Vec_dataAsBytes(const ds_Vec* vec);
extern u8*       ds_Vec_mut_dataAsBytes(ds_Vec* vec);
extern const u8* ds_Vec_dataAsBytesOrNull(const ds_Vec* vec);
extern u8*       ds_Vec_mut_dataAsBytesOrNull(ds_Vec* vec);

extern const anyptr ds_Vec_at(const ds_Vec* vec, usize index);
extern anyptr       ds_Vec_mut_at(ds_Vec* vec, usize index);
extern const anyptr ds_Vec_atOrNull(const ds_Vec* vec, usize index);
extern anyptr       ds_Vec_mut_atOrNull(ds_Vec* vec, usize index);

extern const anyptr ds_Vec_first(const ds_Vec* vec);
extern anyptr       ds_Vec_mut_first(ds_Vec* vec);
extern const anyptr ds_Vec_firstOrNull(const ds_Vec* vec);
extern anyptr       ds_Vec_mut_firstOrNull(ds_Vec* vec);

extern const anyptr ds_Vec_last(const ds_Vec* vec);
extern anyptr       ds_Vec_mut_last(ds_Vec* vec);
extern const anyptr ds_Vec_lastOrNull(const ds_Vec* vec);
extern anyptr       ds_Vec_mut_lastOrNull(ds_Vec* vec);

extern bool ds_Vec_reserve(ds_Vec* vec, usize new_cap);
extern bool ds_Vec_resize(ds_Vec* vec, usize new_len);
extern bool ds_Vec_append(ds_Vec* vec, const anyptr elem);
extern bool ds_Vec_prepend(ds_Vec* vec, const anyptr elem);
extern bool ds_Vec_pop(ds_Vec* vec, anyptr const out_elem);
extern bool ds_Vec_shift(ds_Vec* vec, anyptr const out_elem);
extern bool ds_Vec_insert(ds_Vec* vec, usize index, const anyptr elem);
extern bool ds_Vec_removeOrdered(ds_Vec* vec, usize index, anyptr const out_elem);
extern bool ds_Vec_removeSwap(ds_Vec* vec, usize index, anyptr const out_elem);

extern ds_Vec* ds_Vec__init(ds_Vec* vec, usize elem_size);
extern ds_Vec* ds_Vec__initWithCap(ds_Vec* vec, usize elem_size, usize cap);

// NOLINTBEGIN(bugprone-macro-parentheses)
#define ds_Vec_foreach(_T, _iter, vec)                    \
    for (                                                 \
        rawptr(_T) _iter = (rawptr(_T))ds_Vec_first(vec); \
        _iter < (rawptr(_T))ds_Vec_last(vec);             \
        ++_iter                                           \
    )
// NOLINTEND(bugprone-macro-parentheses)


// Define the base structure of Vec's extra members
#define ds_Vec_fields(T)  \
    usize elem_size;      \
    usize cap;            \
    usize len;            \
    union {               \
        rawptr(u8) bytes; \
        rawptr(T) data;   \
    };

// Generate all Vec method wrappers
#define ds_Vec_methods(BaseType, T, Alias)                                                                      \
    /* Capacity methods */                                                                                      \
    gen_wrapFn(BaseType, T, Alias, cap, (const rawptr(Alias) vec), (vec->Base), usize);                         \
    gen_wrapFn(BaseType, T, Alias, len, (const rawptr(Alias) vec), (vec->Base), usize);                         \
    gen_wrapFn(BaseType, T, Alias, isEmpty, (const rawptr(Alias) vec), (vec->Base), bool);                      \
    gen_wrapFn(BaseType, T, Alias, isFull, (const rawptr(Alias) vec), (vec->Base), bool);                       \
                                                                                                                \
    /* Data access methods with type conversion */                                                              \
    gen_wrapFnConvert(BaseType, T, Alias, data, (const rawptr(Alias) vec), (vec->Base), const rawptr(T));       \
    gen_wrapFnConvert(BaseType, T, Alias, mut_data, (rawptr(Alias) vec), (vec->Base), rawptr(T));               \
    gen_wrapFnConvert(BaseType, T, Alias, dataOrNull, (const rawptr(Alias) vec), (vec->Base), const rawptr(T)); \
    gen_wrapFnConvert(BaseType, T, Alias, mut_dataOrNull, (rawptr(Alias) vec), (vec->Base), rawptr(T));

// Main macro to generate a type-safe Vec wrapper
#define gen_type_for(BaseType, T, Alias) \
    gen_Type(BaseType, T, Alias);        \
    gen_wrapFnAll(BaseType, T, Alias)

/* Built-in types */
typedef ds_Vec(anyptr) ds_Vec_anyptr;
typedef ds_Vec(bool) ds_Vec_bool;

typedef ds_Vec(u8) ds_Vec_u8;
typedef ds_Vec(u16) ds_Vec_u16;
typedef ds_Vec(u32) ds_Vec_u32;
typedef ds_Vec(u64) ds_Vec_u64;
typedef ds_Vec(usize) ds_Vec_usize;

typedef ds_Vec(i8) ds_Vec_i8;
typedef ds_Vec(i16) ds_Vec_i16;
typedef ds_Vec(i32) ds_Vec_i32;
typedef ds_Vec(i64) ds_Vec_i64;
typedef ds_Vec(isize) ds_Vec_isize;

typedef ds_Vec(f32) ds_Vec_f32;
typedef ds_Vec(f64) ds_Vec_f64;

/*========== Usage Example ==============================================*/

// Generate a type-safe vector of integers
gen_type_for(ds_Vec, i32, i32s);

// Use it with type safety
void example(void) {
    i32s vec;

    // Type-safe access
    i32* data = i32s_mut_data(&vec);

    // Original functions still work
    ds_Vec_append(&vec.Base[0], &(int){ 42 });

    // Type-safe iteration
    for (const int* it = i32s_data(&vec);
         it < i32s_data(&vec) + i32s_len(&vec);
         ++it) {
        // Use it...
    }
}

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


// #define gen_ds_Vec_for(_where_T, _Alias)                                                              \
//     typedef_union(_Alias) {                                                                           \
//         ds_Vec(_where_T) Base[1];                                                                     \
//         struct {                                                                                      \
//             usize elem_size;                                                                          \
//             usize cap;                                                                                \
//             usize len;                                                                                \
//             union {                                                                                   \
//                 u8*      bytes;                                                                       \
//                 _where_T data;                                                                        \
//             };                                                                                        \
//         };                                                                                            \
//     };                                                                                                \
//     static_inline usize gen_methName(_Alias, cap)(const rawptr(_Alias) vec) {                         \
//         return gen_methName(ds_Vec, cap)(vec->Base);                                                  \
//     }                                                                                                 \
//     static_inline usize gen_methName(_Alias, len)(const rawptr(_Alias) vec) {                         \
//         return gen_methName(ds_Vec, len)(vec->Base);                                                  \
//     }                                                                                                 \
//     static_inline bool gen_methName(_Alias, isEmpty)(const rawptr(_Alias) vec) {                      \
//         return gen_methName(ds_Vec, isEmpty)(vec->Base);                                              \
//     }                                                                                                 \
//     static_inline bool gen_methName(_Alias, isFull)(const rawptr(_Alias) vec) {                       \
//         return gen_methName(ds_Vec, isFull)(vec->Base);                                               \
//     }                                                                                                 \
//     static_inline const rawptr(_where_T) gen_methName(_Alias, data)(const rawptr(_Alias) vec) {       \
//         return (const rawptr(_where_T))gen_methName(ds_Vec, data)(vec->Base);                         \
//     }                                                                                                 \
//     static_inline rawptr(_where_T) gen_methName(_Alias, mut_data)(rawptr(_Alias) vec) {               \
//         return (rawptr(_where_T))gen_methName(ds_Vec, mut_data)(vec->Base);                           \
//     }                                                                                                 \
//     static_inline const rawptr(_where_T) gen_methName(_Alias, dataOrNull)(const rawptr(_Alias) vec) { \
//         return (const rawptr(_where_T))gen_methName(ds_Vec, dataOrNull)(vec->Base);                   \
//     }                                                                                                 \
//     static_inline rawptr(_where_T) gen_methName(_Alias, mut_dataOrNull)(rawptr(_Alias) vec) {         \
//         return (rawptr(_where_T))gen_methName(ds_Vec, mut_dataOrNull)(vec->Base);                     \
//     }

// gen_ds_Vec_for(i32, i32s);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DS_VEC_INCLUDED */
