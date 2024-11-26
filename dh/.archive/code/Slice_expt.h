// /**
//  * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
//  *
//  * @file    Slice_expt.h
//  * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
//  * @date    2024-11-19 (date of creation)
//  * @updated 2024-11-19 (date of last update)
//  * @version v1.0.0
//  * @ingroup dasae-headers(dh)/core
//  * @prefix  Slice
//  *
//  * @brief   Header of some software
//  * @details Some detailed explanation
//  */


// #ifndef CORE_SLICE_INCLUDED
// #define CORE_SLICE_INCLUDED (1)
// #if defined(__cplusplus)
// extern "C" {
// #endif /* defined(__cplusplus) */

// /*========== Includes =======================================================*/

// #include "prim.h"
// #include "Ptr_expt.h"

// /*========== Macros and Definitions =========================================*/

// /* Core types */
// typedef struct Slice {
//     Ptr   ptr_; // Includes both pointer and total size
//     usize len_; // Number of elements
// } Slice;

// /* Constants */
// #define Slice_empty IMPL_Slice_empty

// /* Construction and validation */
// extern Slice Slice_from(Ptr ptr, usize len);
// #define Slice_fromArr(_arr...) \
//     /* Create slice from array \
//      * i32 arr[] = {1, 2, 3};  \
//      * Slice_fromArr(arr);     \
//      */                        \
//     IMPL_Slice_fromArr(_arr)
// #define Slice_make(TYPE, _len)    \
//     /* Create new allocated slice \
//      * Slice_make(i32, 10);       \
//      */                           \
//     IMPL_Slice_make(TYPE, _len)

// extern bool  Slice_isValid(Slice s);
// extern bool  Slice_isEmpty(Slice s);
// extern usize Slice_stride(Slice s); // Calculated as size/len

// /* Slice operations */
// extern Slice Slice_slice(Slice s, usize start, usize end);
// extern Slice Slice_subslice(Slice s, usize offset, usize len);
// extern Ptr   Slice_ptr(Slice s);

// /* Element access */
// #define Slice_at(_s, _idx, TYPE)               \
//     /* Get element at index with type checking \
//      * i32* val = Slice_at(slice, 0, i32);     \
//      */                                        \
//     IMPL_Slice_at(_s, _idx, TYPE)
// #define Slice_first(_s, TYPE)               \
//     /* Get first element with type checking \
//      * i32* val = Slice_first(slice, i32);  \
//      */                                     \
//     IMPL_Slice_first(_s, TYPE)
// #define Slice_last(_s, TYPE)               \
//     /* Get last element with type checking \
//      * i32* val = Slice_last(slice, i32);  \
//      */                                    \
//     IMPL_Slice_last(_s, TYPE)

// extern Ptr Slice__at(Slice s, usize idx);
// extern Ptr Slice__first(Slice s);
// extern Ptr Slice__last(Slice s);

// /* Iteration */
// #define Slice_iter(_s, TYPE, _var)   \
//     /* Iterate over elements         \
//      * Slice_iter(slice, i32, val) { \
//      *     printf("%d\n", *val);     \
//      * }                             \
//      */                              \
//     IMPL_Slice_iter(_s, TYPE, _var)

// /* Memory operations */
// extern bool Slice_copy(Slice dest, Slice src);

// /* Metadata access */
// extern usize Slice_len(Slice s);
// extern usize Slice_totalSize(Slice s);

// /*========== Macro Implementations ==========================================*/

// // NOLINTBEGIN(bugprone-macro-parentheses)
// #define IMPL_Slice_empty ((Slice){ .ptr_ = Ptr_null, .len_ = 0 })

// #define IMPL_Slice_fromArr(_arr...) \
//     Slice_from(Ptr_from(_arr, sizeof(_arr)), sizeof(_arr) / sizeof(_arr[0]))

// #define IMPL_Slice_make(TYPE, _len) \
//     Slice_from(Ptr_withAlloc(sizeof(TYPE) * (_len)), (_len))

// #define IMPL_Slice_at(_s, _idx, TYPE) \
//     Ptr_cast(TYPE*, Slice__at(_s, _idx))

// #define IMPL_Slice_begin(_s) \
//     Ptr_cast(TYPE*, Slice__first(_s))

// #define IMPL_Slice_end(_s) \
//     Ptr_cast(TYPE*, Slice__last(_s))

// #define IMPL_Slice_iter(_s, TYPE, _var)                                  \
//     for (TYPE* _var = (TYPE*)Ptr_raw((_s).ptr_);                         \
//          _var != null && _var < ((TYPE*)Ptr_raw((_s).ptr_) + (_s).len_); \
//          ++_var)
// // NOLINTEND(bugprone-macro-parentheses)


// #if defined(__cplusplus)
// } /* extern "C" */
// #endif /* defined(__cplusplus) */
// #endif /* CORE_SLICE_INCLUDED */
