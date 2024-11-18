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

typedef struct ds_Vec {
    usize cap;
    usize elem_size;
    usize len;
    union {
        u8*    bytes;
        anyptr data;
    };
} ds_Vec;

// #define ds_Vec(TYPE)    \
//     struct_Generic(     \
//         ds_Vec,         \
//         usize len;      \
//         Ptr(TYPE) data; \
//     )

#define ds_Vec(TYPE) \
    ds_Vec

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

/*========== Extern Function Prototypes =====================================*/

#define ds_Vec_init(TYPE, vec)             ds_Vec__init(vec, sizeof(TYPE))
#define ds_Vec_initWithCap(TYPE, vec, cap) ds_Vec__initWithCap(vec, sizeof(TYPE), cap)
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
#define ds_Vec_foreach(TYPE, iter, vec)        \
    for (                                      \
        TYPE* iter = (TYPE*)ds_Vec_first(vec); \
        iter < (TYPE*)ds_Vec_last(vec);        \
        ++iter                                 \
    )
// NOLINTEND(bugprone-macro-parentheses)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

// #ifdef UNIT_TEST
// #endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DS_VEC_INCLUDED */
