/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Arr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-17 (date of creation)
 * @updated 2025-03-23 (date of last update)
 * @version v0.1-alpha.5
 * @ingroup dasae-headers(dh)
 * @prefix  Arr
 *
 * @brief   Fixed-size array types and operations
 * @details This header provides types and macros for C-style fixed-size arrays.
 *          Supports both constant and mutable arrays, with separate type
 *          declarations and implementations.
 *          Array size and element type are determined at compile-time and
 *          cannot be changed at runtime.
 */

/*========== Cheat Sheet ====================================================*/

#if CHEAT_SHEET
/* Type Declarations */
Arr_const(3, i32) constArr;   // Constant i32 array with 3 elements
Arr(5, f32) arr;              // Mutable f32 array with 5 elements
typedef Arr$(4, u8) ByteQuad; // Anonymous array type for typedefs
use_Arr(4, u8);               // Declare and implement u8[4] array type
decl_Arr(3, i32);             // Declare i32[3] array type
impl_Arr(3, i32);             // Implement previously declared type

/* Initialization */
Arr(3, i32) arr1 = Arr_zero();                           // Initialize with all zeros
Arr(3, i32) arr2 = Arr_init({ 1, 2, 3 });                // Initialize with values
var anon1        = Arr_zero$(Arr$(3, i32));              // Initialize anonymous array with zeros
var anon2        = Arr_init$(Arr$(3, i32), { 1, 2, 3 }); // Initialize anonymous array with values
var typed1       = Arr_anonCast$(Arr$3$i32, anon1);      // Type conversion
var typed2       = Arr_anonCast$(Arr(3, i32), anon2);    // Type conversion

/* API Reference List =======================================================*/

/* Type Definitions */
/// Fixed-size array type
typedef struct Arr$N$T {
    T items[N]; ///< Array elements
} Arr$N$T;

/* Type Tokens */
/// Create a constant array type with N elements of type T
#define Arr_const(N, T) ...
/// Create a mutable array type with N elements of type T
#define Arr(N, T)       ...

/* Type Generations */
/// Declare and implement array type in one step
#define use_Arr(N, T)  ...
/// Declare array type without implementation
#define decl_Arr(N, T) ...
/// Implement previously declared array type
#define impl_Arr(N, T) ...

/* Anonymous Types */
/// Create anonymous constant array type
#define Arr_const$(N, T)                  ...
/// Create anonymous mutable array type
#define Arr$(N, T)                        ...
/// Convert anonymous array to specific type
#define Arr_anonCast$(T_Arr, var_anon...) ...

/* Initialization */
/// Initialize array with zeros
#define Arr_zero()                    ...
/// Initialize specific array type with zeros
#define Arr_zero$(T_Arr)              ...
/// Initialize array with specified values
#define Arr_init(_Initial...)         ...
/// Initialize specific array type with values
#define Arr_init$(T_Arr, _Initial...) ...

/* Operations */
/// Get number of elements in array
#define Arr_len(var_self...)                          ...
/// Get pointer to element at index (bounds-checked)
#define Arr_at(var_self, usize_index...)              ...
/// Get value of element at index (bounds-checked)
#define Arr_getAt(var_self, usize_index...)           ...
/// Set value at index (bounds-checked)
#define Arr_setAt(var_self, usize_index, val_item...) ...

/* Iteration */
/// Forward iteration over elements
#define for_array(var_arr, _Iter_item)                          ...
/// Forward iteration with index
#define for_array_indexed(var_arr, _Iter_item, _Iter_index)     ...
/// Reverse iteration over elements
#define for_array_rev(var_arr, _Iter_item)                      ...
/// Reverse iteration with index
#define for_array_rev_indexed(var_arr, _Iter_item, _Iter_index) ...

/* Operations Examples ======================================================*/

usize len = Arr_len(arr);      // Get array length
i32*  ptr = Arr_at(arr, 1);    // Get pointer to element (bounds-checked)
i32   val = Arr_getAt(arr, 1); // Get element value (bounds-checked)
Arr_setAt(arr, 1, 5);          // Set element value (bounds-checked)

/* Iteration Examples =======================================================*/

for_array (arr, item) {   // Forward iteration
    printf("%d ", *item); // Prints: 1 2 3
}
for_array_indexed (arr, item, idx) { // Forward with index
    printf("[%zu]=%d ", idx, *item); // Prints: [0]=1 [1]=2 [2]=3
}
for_array_rev (arr, item) { // Reverse iteration
    printf("%d ", *item);   // Prints: 3 2 1
}
for_array_rev_indexed (arr, item, idx) { // Reverse with index
    printf("[%zu]=%d ", idx, *item);     // Prints: [2]=3 [1]=2 [0]=1
}
#endif /* CHEAT_SHEET */

/*========== Header =======================================================*/

#ifndef ARR_INCLUDED
#define ARR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

#define Arr_const(N, T)                                                      \
    /**                                                                      \
     * @brief Creates a constant array type                                  \
     * @param N Size of the array                                            \
     * @param T Element type of the array                                    \
     * @return Constant array type token                                     \
     * @example                                                              \
     *     Arr_const(3, i32) myConstArr;  // const i32 array with 3 elements \
     */                                                                      \
    comp_type_token__Arr_const(N, T)
#define Arr(N, T)                                           \
    /**                                                     \
     * @brief Creates a mutable array type                  \
     * @param N Size of the array                           \
     * @param T Element type of the array                   \
     * @return Mutable Array type token                     \
     * @example                                             \
     *     Arr(5, f32) myArr;  // f32 array with 5 elements \
     */                                                     \
    comp_type_token__Arr(N, T)

#define use_Arr(N, T)                                                   \
    /**                                                                 \
     * @brief Declares and implements an array type                     \
     * @param N Size of the array                                       \
     * @param T Element type of the array                               \
     * @example                                                         \
     *     use_Arr(4, u8);  // Declares and implements u8[4] array type \
     */                                                                 \
    comp_type_gen__use_Arr(N, T)
#define decl_Arr(N, T)                                      \
    /**                                                     \
     * @brief Declares an array type without implementation \
     * @param N Size of the array                           \
     * @param T Element type of the array                   \
     * @example                                             \
     *     decl_Arr(3, i32);  // Declares i32[3] array type \
     */                                                     \
    comp_type_gen__decl_Arr(N, T)
#define impl_Arr(N, T)                                        \
    /**                                                       \
     * @brief Implements a previously declared array type     \
     * @param N Size of the array                             \
     * @param T Element type of the array                     \
     * @example                                               \
     *     impl_Arr(3, i32);  // Implements i32[3] array type \
     */                                                       \
    comp_type_gen__impl_Arr(N, T)

#define Arr_const$(N, T)                                                              \
    /**                                                                               \
     * @brief Creates an anonymous constant array type                                \
     * @param N Size of the array                                                     \
     * @param T Element type of the array                                             \
     * @return Anonymous constant array type token                                    \
     * @example                                                                       \
     *     typedef Arr_const$(3, i32) MyConstArrType;  // Anonymous const i32[3] type \
     */                                                                               \
    comp_type_anon__Arr_const$(N, T)
#define Arr$(N, T)                                                   \
    /**                                                              \
     * @brief Creates an anonymous array type                        \
     * @param N Size of the array                                    \
     * @param T Element type of the array                            \
     * @return Anonymous array type token                            \
     * @example                                                      \
     *     typedef Arr$(3, i32) MyArrType;  // Anonymous i32[3] type \
     */                                                              \
    comp_type_anon__Arr$(N, T)
#define Arr_anonCast$(T_Arr, var_anon...)                                                          \
    /**                                                                                            \
     * @brief Casts an anonymous array to a specific array type                                    \
     * @param T_Arr Target array type                                                              \
     * @param var_anon Anonymous array variable                                                    \
     * @return Cast array of specified type                                                        \
     * @details Performs type safety checks and converts the anonymous array to the specified type \
     * @example                                                                                    \
     *     Arr$3$i32 typed = Arr_anonCast$(Arr$3$i32, anon);                                       \
     */                                                                                            \
    comp_op__Arr_anonCast$(pp_uniqTok(anon), T_Arr, var_anon)

#define Arr_zero()                                     \
    /**                                                \
     * @brief Initializes array with zeros             \
     * @return Zero-initialized array literal          \
     * @example                                        \
     *     Arr(3, i32) arr = Arr_zero();  // {0, 0, 0} \
     */                                                \
    comp_op__Arr_zero()
#define Arr_zero$(T_Arr)                                           \
    /**                                                            \
     * @brief Initializes specified array type with zeros          \
     * @param T_Arr Array type                                     \
     * @return Zero-initialized array                              \
     * @example                                                    \
     *     Arr(3, i32) arr = Arr_zero$(Arr(3, i32));  // {0, 0, 0} \
     */                                                            \
    comp_op__Arr_zero$(T_Arr)

#define Arr_init(_Initial...)                         \
    /**                                               \
     * @brief Initializes array with specified values \
     * @param _Initial Initial values                 \
     * @return Initialized array literal              \
     * @example                                       \
     *     Arr(3, i32) arr = Arr_init({1, 2, 3});     \
     */                                               \
    comp_op__Arr_init(_Initial)
#define Arr_init$(T_Arr, _Initial...)                           \
    /**                                                         \
     * @brief Initializes specified array type with values      \
     * @param T_Arr Array type                                  \
     * @param _Initial Initial values                           \
     * @return Initialized array                                \
     * @example                                                 \
     *     Arr(3, i32) arr = Arr_init$(Arr(3, i32), {1, 2, 3}); \
     */                                                         \
    comp_op__Arr_init$(T_Arr, _Initial)

#define Arr_len(var_self...)                      \
    /**                                           \
     * @brief Returns the length of an array      \
     * @param var_self Array variable             \
     * @return Length of the array                \
     * @example                                   \
     *     Arr(3, i32) arr = Arr_init({1, 2, 3}); \
     *     usize len = Arr_len(arr);  // 3        \
     */                                           \
    comp_op__Arr_len(pp_uniqTok(self), var_self)
#define Arr_at(var_self, usize_index...)                                                             \
    /**                                                                                              \
     * @brief Returns a reference to an element at the specified index                               \
     * @param var_self Array variable                                                                \
     * @param usize_index Index                                                                      \
     * @return Reference to the element                                                              \
     * @details Performs bounds checking, triggers assertion in debug mode if index is out of bounds \
     * @example                                                                                      \
     *     Arr(3, i32) arr = Arr_init({1, 2, 3});                                                    \
     *     i32* pItem = Arr_at(arr, 1);  // &arr.items[1]                                            \
     */                                                                                              \
    comp_op__Arr_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Arr_getAt(var_self, usize_index...)                                                          \
    /**                                                                                              \
     * @brief Returns the value of an element at the specified index                                 \
     * @param var_self Array variable                                                                \
     * @param usize_index Index                                                                      \
     * @return Element value                                                                         \
     * @details Performs bounds checking, triggers assertion in debug mode if index is out of bounds \
     * @example                                                                                      \
     *     Arr(3, i32) arr = Arr_init({1, 2, 3});                                                    \
     *     i32 item = Arr_getAt(arr, 1);  // 2                                                       \
     */                                                                                              \
    comp_op__Arr_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Arr_setAt(var_self, usize_index, val_item...)                                                \
    /**                                                                                              \
     * @brief Sets the value of an element at the specified index                                    \
     * @param var_self Array variable                                                                \
     * @param usize_index Index                                                                      \
     * @param val_item Value to set                                                                  \
     * @return Reference to the array itself                                                         \
     * @details Performs bounds checking, triggers assertion in debug mode if index is out of bounds \
     * @example                                                                                      \
     *     Arr(3, i32) arr = Arr_init({1, 2, 3});                                                    \
     *     Arr_setAt(arr, 1, 5);  // arr = {1, 5, 3}                                                 \
     */                                                                                              \
    comp_op__Arr_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

#define for_array(var_arr, _Iter_item)                      \
    /**                                                     \
     * @brief Iterates through array elements               \
     * @param var_arr Array variable                        \
     * @param _Iter_item Iterator variable for each element \
     * @example                                             \
     *     Arr(3, i32) arr = Arr_init({1, 2, 3});           \
     *     for_array (arr, item) {                          \
     *         printf("%d ", *item);  // "1 2 3"            \
     *     }                                                \
     */                                                     \
    comp_syn__for_array(pp_uniqTok(arr), pp_uniqTok(i), var_arr, _Iter_item)
#define for_array_indexed(var_arr, _Iter_item, _Iter_index)             \
    /**                                                                 \
     * @brief Iterates through array elements with index                \
     * @param var_arr Array variable                                    \
     * @param _Iter_item Iterator variable for each element             \
     * @param _Iter_index Index variable                                \
     * @example                                                         \
     *     Arr(3, i32) arr = Arr_init({1, 2, 3});                       \
     *     for_array_indexed (arr, item, idx) {                         \
     *         printf("[%zu]=%d ", idx, *item);  // "[0]=1 [1]=2 [2]=3" \
     *     }                                                            \
     */                                                                 \
    comp_syn__for_array_indexed(pp_uniqTok(arr), var_arr, _Iter_item, _Iter_index)
#define for_array_rev(var_arr, _Iter_item)                     \
    /**                                                        \
     * @brief Iterates through array elements in reverse order \
     * @param var_arr Array variable                           \
     * @param _Iter_item Iterator variable for each element    \
     * @example                                                \
     *     Arr(3, i32) arr = Arr_init({1, 2, 3});              \
     *     for_array_rev (arr, item) {                         \
     *         printf("%d ", *item);  // "3 2 1"               \
     *     }                                                   \
     */                                                        \
    comp_syn__for_array_rev(pp_uniqTok(arr), pp_uniqTok(i), var_arr, _Iter_item)
#define for_array_rev_indexed(var_arr, _Iter_item, _Iter_index)           \
    /**                                                                   \
     * @brief Iterates through array elements in reverse order with index \
     * @param var_arr Array variable                                      \
     * @param _Iter_item Iterator variable for each element               \
     * @param _Iter_index Index variable                                  \
     * @example                                                           \
     *     Arr(3, i32) arr = Arr_init({1, 2, 3});                         \
     *     for_array_rev_indexed (arr, item, idx) {                       \
     *         printf("[%zu]=%d ", idx, *item);  // "[2]=3 [1]=2 [0]=1"   \
     *     }                                                              \
     */                                                                   \
    comp_syn__for_array_rev_indexed(pp_uniqTok(arr), var_arr, _Iter_item, _Iter_index)

/*========== Macros and Definitions =========================================*/

#define comp_type_token__Arr_const(N, T) pp_join3($, Arr_const, N, T)
#define comp_type_token__Arr(N, T)       pp_join3($, Arr, N, T)

#define comp_type_gen__use_Arr(N, T) \
    decl_Arr(N, T);                  \
    impl_Arr(N, T)
#define comp_type_gen__decl_Arr(N, T)               \
    typedef struct Arr_const(N, T) Arr_const(N, T); \
    typedef union Arr(N, T) Arr(N, T)
#define comp_type_gen__impl_Arr(N, T) \
    struct Arr_const(N, T) {          \
        const T items[N];             \
    };                                \
    union Arr(N, T) {                 \
        struct {                      \
            T items[N];               \
        };                            \
        Arr(N, T) as_const;           \
    }

#define comp_type_anon__Arr_const$(N, T) \
    struct {                             \
        const T items[N];                \
    }
#define comp_type_anon__Arr$(N, T) \
    struct {                       \
        T items[N];                \
    }
#define comp_op__Arr_anonCast$(__anon, T_Arr, var_anon...) eval({                            \
    let __anon = &var_anon;                                                                  \
    claim_assert_static(sizeOf(TypeOf(*__anon)) == sizeOf(T_Arr));                           \
    claim_assert_static(alignOf(TypeOf(*__anon)) == alignOf(T_Arr));                         \
    claim_assert_static(validateField(TypeOf(*__anon), items, FieldTypeOf(T_Arr, items)));   \
    claim_assert_static(fieldPadding(TypeOf(*__anon), items) == fieldPadding(T_Arr, items)); \
    eval_return rawderef(as$(rawptr$(T_Arr), __anon));                                       \
})

#define comp_op__Arr_zero()                    { .items = { 0 } }
#define comp_op__Arr_zero$(T_Arr)              ((T_Arr)Arr_zero())
#define comp_op__Arr_init(_Initial...)         { .items = _Initial }
#define comp_op__Arr_init$(T_Arr, _Initial...) ((T_Arr)Arr_init(_Initial))

#define comp_op__Arr_len(__self, var_self...) eval({ \
    const TypeOf(&var_self) __self = &var_self;      \
    eval_return countOf(__self->items);              \
})
#define comp_op__Arr_at(__self, __index, var_self, usize_index...) eval({ \
    const TypeOf(&var_self) __self = &var_self;                           \
    const usize __index            = usize_index;                         \
    debug_assert_fmt(                                                     \
        __index < Arr_len(*__self),                                       \
        "Index out of bounds: %zu >= %zu",                                \
        __index,                                                          \
        Arr_len(*__self)                                                  \
    );                                                                    \
    eval_return rawref(__self->items[__index]);                           \
})
#define comp_op__Arr_getAt(__self, __index, var_self, usize_index...) eval({ \
    const TypeOf(&var_self) __self = &var_self;                              \
    const usize __index            = usize_index;                            \
    debug_assert_fmt(                                                        \
        __index < Arr_len(*__self),                                          \
        "Index out of bounds: %zu >= %zu",                                   \
        __index,                                                             \
        Arr_len(*__self)                                                     \
    );                                                                       \
    eval_return __self->items[__index];                                      \
})
#define comp_op__Arr_setAt(__self, __index, var_self, usize_index, var_value...) eval({ \
    const TypeOf(&var_self) __self = &var_self;                                         \
    const usize __index            = usize_index;                                       \
    debug_assert_fmt(                                                                   \
        __index < Arr_len(*__self),                                                     \
        "Index out of bounds: %zu >= %zu",                                              \
        __index,                                                                        \
        Arr_len(*__self)                                                                \
    );                                                                                  \
    __self->items[__index] = as$(TypeOf(__self->items[0]), var_value);                  \
    eval_return __self;                                                                 \
})

#define comp_syn__for_array(__arr, __i, var_arr, _Iter_item) \
    with_(let __arr = &(var_arr)) for (usize __i = 0; (__i) < Arr_len(*(__arr)); ++(__i)) with_(let _Iter_item = Arr_at(*(__arr), __i))
#define comp_syn__for_array_indexed(__arr, var_arr, _Iter_item, _Iter_index) \
    with_(let __arr = &(var_arr)) for (usize _Iter_index = 0; (_Iter_index) < Arr_len(*(__arr)); ++(_Iter_index)) with_(let _Iter_item = Arr_at(*(__arr), _Iter_index))
#define comp_syn__for_array_rev(__arr, __i, var_arr, _Iter_item) \
    with_(let __arr = &(var_arr)) for (usize __i = Arr_len(*(__arr)); (__i)-- > 0;) with_(let _Iter_item = Arr_at(*(__arr), __i))
#define comp_syn__for_array_indexed_rev(__arr, var_arr, _Iter_item, _Iter_index) \
    with_(let __arr = &(var_arr)) for (usize _Iter_index = Arr_len(*(__arr)); (_Iter_index)-- > 0;) with_(let _Iter_item = Arr_at(*(__arr), _Iter_index))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ARR_INCLUDED */
