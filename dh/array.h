// /**
//  * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
//  *
//  * @file    template.h
//  * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
//  * @date    2024-10-29 (date of creation)
//  * @updated 2024-10-30 (date of last update)
//  * @version v1.0.0
//  * @ingroup dasae-headers(dh)
//  * @prefix  NONE
//  *
//  * @brief   Header of some software
//  * @details Some detailed explanation
//  */


// #include "compat/compat_inline.h"
// #ifndef ARRAY_INCLUDED
// #define ARRAY_INCLUDED (1)
// #if defined(__cplusplus)
// extern "C" {
// #endif /* defined(__cplusplus) */


// /*========== Includes =======================================================*/

// #include "mem.h"
// #include "primitive.h"

// #include "debug/debug_assert.h"

// /*========== Macros and Definitions =========================================*/

// typedef struct Array Array;


// /* Fixed-size array structure */
// struct Array {
//     anyptr data_;
//     usize  length_;
// };
// #define Array_(...)                             RETURN_Array_(__VA_ARGS__)
// /* Macros for generic array operations */
// #define Array(_T)                               RETURN_Array(_T)
// /* Create an array specified length */
// #define Array_make(_T, _length)                 RETURN_Array_make(_T, _length)
// /* Create an array specified length with braced initializer */
// #define Array_makeWith(_T, _length, _braced...) RETURN_Array_makeWith(_T, _length, _braced)
// /* Create an array from a braced initializer and automatically determine length */
// #define Array_makeFrom(_T, _braced...)          RETURN_Array_makeFrom(_T, _braced)
// /* Get array length */
// #define Array_length(_T, _a)                    RETURN_Array_length(_T, _a)
// /* Get value at index */
// #define Array_get(_T, _a, _index)               RETURN_Array_get(_T, _a, _index)
// /* Set value at index */
// #define Array_set(_T, _a, _index, _value)       RETURN_Array_set(_T, _a, _index, _value)
// /* Copy fixed-size array contents to destination */
// #define Array_copy(_T, _dest, _src)             RETURN_Array_copy(_T, _dest, _src)
// /* Move fixed-size array contents to destination */
// #define Array_move(_T, _dest, _src)             RETURN_Array_move(_T, _dest, _src)

// /*========== Macros Implementation ==========================================*/

// #define RETURN_Array_(...) makeWith(Array, { __VA_ARGS__ })

// #define RETURN_Array(_T) Array

// #define RETURN_Array_make(_T, _length)    \
//     Array_(                               \
//             .data_   = new (_T, _length), \
//             .length_ = (_length)          \
//     )

// #define RETURN_Array_makeWith(_T, _length, _braced...) \
//     Array_(                                            \
//             .data_   = newWith(_T, _length, _braced),  \
//             .length_ = (_length)                       \
//     )

// #define RETURN_Array_makeFrom(_T, _braced...)                           \
//     Array_(                                                             \
//             .data_   = prim_arrayFrom(_T, _braced),                     \
//             .length_ = sizeof(prim_arrayFrom(_T, _braced)) / sizeof(_T) \
//     )

// #define RETURN_Array_length(_T, _a) \
//     Array__length(_a)

// #define RETURN_Array_get(_T, _a, _index) \
//     (*((const _T*)Array__at(_a, _index, sizeof(_T))))

// #define RETURN_Array_set(_T, _a, _index, _value) \
//     (*((_T*)Array__at(_a, _index, sizeof(_T)))) = (_value)

// #define RETURN_Array_copy(_T, _dest, _src) \
//     Array__copy(_dest, _src, sizeof(_T))

// #define RETURN_Array_move(_T, _dest, _src) \
//     Array__move(_dest, _src, sizeof(_T))

// /*========== Inline Function Implementations ================================*/

// force_inline usize Array__length(Array a) {
//     return a.length_;
// }

// force_inline anyptr Array__at(Array a, usize index, usize type_size) {
//     debug_assert(index < a.length_);
//     return (u8*)a.data_ + index * type_size;
// }

// force_inline void Array__copy(Array dest, Array src, usize type_size) {
//     debug_assert(dest.length_ == src.length_);
//     mem_copy(dest.data_, src.data_, dest.length_ * type_size);
// }

// force_inline void Array__move(Array dest, Array src, usize type_size) {
//     debug_assert(dest.length_ == src.length_);
//     mem_move(dest.data_, src.data_, dest.length_ * type_size);
//     mem_set(src.data_, 0, src.length_ * type_size);
// }

// /*========== Extern Constant and Variable Declarations ======================*/

// /*========== Extern Function Prototypes =====================================*/

// /*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

// #ifdef UNIT_TEST

// static void TEST_Array(void) {
//     Array(i32) by_make      = Array_make(i32, 10);
//     Array(i32) by_make_with = Array_makeWith(i32, 10, { 0, 1, 2, 3 });
//     Array(i32) by_make_from = Array_makeFrom(i32, { 9, 8, 7 });
//     Array_set(i32, by_make, 0, 1);
//     Array_copy(i32, by_make_from, by_make_with);
// }

// #endif /* UNIT_TEST */


// #if defined(__cplusplus)
// } /* extern "C" */
// #endif /* defined(__cplusplus) */
// #endif /* ARRAY_INCLUDED */
