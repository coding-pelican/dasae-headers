/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Arr.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-17 (date of creation)
 * @updated 2025-04-18 (date of last update)
 * @version v0.1-alpha.7
 * @ingroup dasae-headers(dh)
 * @prefix  Arr
 *
 * @brief   Fixed-size array types and operations
 * @details This header provides types and macros for C-style fixed-size arrays.
 *          Array size and element type are determined at compile-time and
 *          cannot be changed at runtime.
 */

/*========== Cheat Sheet ====================================================*/

#if CHEAT_SHEET
/* Type Declarations */
Arr$(5, f32) arr;              // [5]f32 array
typedef Arr$$(4, u8) ByteQuad; // Anonymous [4]u8 type for typedefs
use_Arr$(4, u8);               // Declare and implement [4]u8 array type
decl_Arr$(3, i32);             // Declare [3]i32 array type
impl_Arr$(3, i32);             // Implement previously declared type

/* Initialization */
Arr$(3, i32) arr1 = Arr_zero();                            // Initialize with all zeros
Arr$(3, i32) arr2 = Arr_init({ 1, 2, 3 });                 // Initialize with values
var anon1         = Arr_zero$(Arr$$(3, i32));              // Initialize anonymous array with zeros
var anon2         = Arr_init$(Arr$$(3, i32), { 1, 2, 3 }); // Initialize anonymous array with values
var typed1        = Arr_anonCast$(Arr$3$i32, anon1);       // Type conversion
var typed2        = Arr_anonCast$(Arr$(3, i32), anon2);    // Type conversion

/* API Reference List =======================================================*/

/* Type Definitions */
/// Fixed-size array type
typedef struct Arr$N$T {
    T buf[N]; ///< Array elements
} Arr$N$T;

/* Type Generations */
/// Declare and implement array type in one step
#define use_Arr$(N, T)  ...
/// Declare array type without implementation
#define decl_Arr$(N, T) ...
/// Implement previously declared array type
#define impl_Arr$(N, T) ...

/* Type Alias and Anonymous Types */
/// Create an array type with N elements of type T
#define Arr$(N, T)                         ...
/// Create an anonymous array type with N elements of type T
#define Arr$$(N, T)                        ...
/// Convert anonymous array to specific type
#define Arr_anonCast$(_Arr$T, var_anon...) ...

/* Initialization */
/// Initialize array with zeros
#define Arr_zero()                     ...
/// Initialize specific array type with zeros
#define Arr_zero$(_Arr$T)              ...
/// Initialize array with specified values
#define Arr_init(_Initial...)          ...
/// Initialize specific array type with values
#define Arr_init$(_Arr$T, _Initial...) ...
/// Initialize array inferred size from initial values
#define Arr_from$(T, _Initial...)      ...

/* Operations */
/// `arr.len` | Get number of elements in array
#define Arr_len(var_self...)                          ...
/// `&arr[index]` | Get pointer to element at index (bounds-checked)
#define Arr_at(var_self, usize_index...)              ...
/// `arr[index]` | Get value of element at index (bounds-checked)
#define Arr_getAt(var_self, usize_index...)           ...
/// `arr[index] = val` | Set value at index (bounds-checked)
#define Arr_setAt(var_self, usize_index, val_item...) ...

/* Concatenation */
/// Concatenate two arrays
#define Arr_cat(var_self, var_other...)          ...
/// Concatenate two arrays with specific target type
#define Arr_cat$(_Arr$T, var_self, var_other...) ...

/* Range-based Slice Operations */
/// `arr[begin..end]` | Get slice from begin to end
#define Arr_slice(var_self, range_index_begin_end...)                        ...
/// `arr[begin..end]` | Get specific slice from begin to end
#define Arr_slice$(T_Sli, var_self, range_index_begin_end...)                ...
/// `arr[begin..end:0]` | Get slice from begin to end with zero sentinel
#define Arr_sliceZ(var_self, range_index_begin_end...)                       ...
/// `arr[begin..end:0]` | Get specific slice from begin to end with zero sentinel
#define Arr_sliceZ$(T_Sli, var_self, range_index_begin_end...)               ...
/// `arr[begin..end:sentinel]` | Get slice from begin to end with sentinel
#define Arr_sliceS(var_self, range_index_begin_end, val_sentinel...)         ...
/// `arr[begin..end:sentinel]` | Get specific slice from begin to end with sentinel
#define Arr_sliceS$(T_Sli, var_self, range_index_begin_end, val_sentinel...) ...
/// `arr[0..index]` | Get slice from begin to index
#define Arr_prefix(var_self, usize_index_end...)                             ...
/// `arr[0..index]` | Get specific slice from begin to index
#define Arr_prefix$(T_Sli, var_self, usize_index_end...)                     ...
/// `arr[0..index:0]` | Get slice from begin to zero sentinel
#define Arr_prefixZ(var_self...)                                             ...
/// `arr[0..index:0]` | Get specific slice from begin to zero sentinel
#define Arr_prefixZ$(T_Sli, var_self...)                                     ...
/// `arr[0..index:sentinel]` | Get slice from begin to sentinel
#define Arr_prefixS(var_self, val_sentinel...)                               ...
/// `arr[0..index:sentinel]` | Get specific slice from begin to sentinel
#define Arr_prefixS$(T_Sli, var_self, val_sentinel...)                       ...
/// `arr[index..len]` | Get slice from index to end
#define Arr_suffix(var_self, usize_index_begin...)                           ...
/// `arr[index..len]` | Get specific slice from index to end
#define Arr_suffix$(T_Sli, var_self, usize_index_begin...)                   ...
/// `arr[index..len:0]` | Get slice from index to zero sentinel
#define Arr_suffixZ(var_self, usize_index_begin...)                          ...
/// `arr[index..len:0]` | Get specific slice from index to zero sentinel
#define Arr_suffixZ$(T_Sli, var_self, usize_index_begin...)                  ...
/// `arr[index..len:sentinel]` | Get slice from index to sentinel
#define Arr_suffixS(var_self, usize_index_begin, val_sentinel...)            ...
/// `arr[index..len:sentinel]` | Get specific slice from index to sentinel
#define Arr_suffixS$(T_Sli, var_self, usize_index_begin, val_sentinel...)    ...

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

#define use_Arr$(N, T) \
    /** \
     * @brief Declares and implements an array type \
     * @param N Size of the array \
     * @param T Element type of the array \
     * @example \
     *     use_Arr$(4, u8);  // Declares and implements [4]u8 array type \
     */ \
    comp_type_gen__use_Arr$(N, T)
#define decl_Arr$(N, T) \
    /** \
     * @brief Declares an array type without implementation \
     * @param N Size of the array \
     * @param T Element type of the array \
     * @example \
     *     decl_Arr$(3, i32);  // Declares [3]i32 array type \
     */ \
    comp_type_gen__decl_Arr$(N, T)
#define impl_Arr$(N, T) \
    /** \
     * @brief Implements a previously declared array type \
     * @param N Size of the array \
     * @param T Element type of the array \
     * @example \
     *     impl_Arr$(3, i32);  // Implements [3]i32 array type \
     */ \
    comp_type_gen__impl_Arr$(N, T)

#define Arr_const$(N, T) \
    /** \
     * @brief Creates a constant array type \
     * @param N Size of the array \
     * @param T Element type of the array \
     * @return Constant array type alias \
     * @example \
     */ \
    comp_type_alias__Arr_const$(N, T)

#define Arr$(N, T) \
    /** \
     * @brief Creates an array type \
     * @param N Size of the array \
     * @param T Element type of the array \
     * @return Array type alias \
     * @example \
     *     Arr$(5, f32) myArr;  // [5]f32 array \
     */ \
    comp_type_alias__Arr$(N, T)
#define Arr$$(N, T) \
    /** \
     * @brief Creates an anonymous array type \
     * @param N Size of the array \
     * @param T Element type of the array \
     * @return Anonymous array type alias \
     * @example \
     *     typedef Arr$$(3, i32) MyArrType;  // Anonymous [3]i32 type \
     */ \
    comp_type_anon__Arr$$(N, T)
#define ArrZ$$(N, T) \
    /** \
     * @brief Creates an anonymous array type with N elements of type T and zero sentinel \
     * @param N Size of the array \
     * @param T Element type of the array \
     * @return Anonymous array type alias \
     */ \
    comp_type_anon__ArrZ$$(N, T)
#define Arr_anonCast$(_Arr$T, var_anon...) \
    /** \
     * @brief Casts an anonymous array to a specific array type \
     * @param _Arr$T Target array type \
     * @param var_anon Anonymous array variable \
     * @return Cast array of specified type \
     * @details Performs type safety checks and converts the anonymous array to the specified type \
     * @example \
     *     Arr$3$i32 typed = Arr_anonCast$(Arr$3$i32, anon); \
     */ \
    comp_op__Arr_anonCast$(pp_uniqTok(anon), _Arr$T, var_anon)

#define Arr_zero() \
    /** \
     * @brief Initializes array with zeros \
     * @return Zero-initialized array literal \
     * @example \
     *     Arr$(3, i32) arr = Arr_zero();  // {0, 0, 0} \
     */ \
    comp_op__Arr_zero()
#define Arr_zero$(_Arr$T) \
    /** \
     * @brief Initializes specified array type with zeros \
     * @param _Arr$T Array type \
     * @return Zero-initialized array \
     * @example \
     *     Arr$(3, i32) arr = Arr_zero$(Arr$(3, i32));  // {0, 0, 0} \
     */ \
    comp_op__Arr_zero$(_Arr$T)

#define Arr_init(_Initial...) \
    /** \
     * @brief Initializes array with specified values \
     * @param _Initial Initial values \
     * @return Initialized array literal \
     * @example \
     *     Arr$(3, i32) arr = Arr_init({1, 2, 3}); \
     */ \
    comp_op__Arr_init(_Initial)
#define Arr_init$(_Arr$T, _Initial...) \
    /** \
     * @brief Initializes specified array type with values \
     * @param _Arr$T Array type \
     * @param _Initial Initial values \
     * @return Initialized array \
     * @example \
     *     Arr$(3, i32) arr = Arr_init$(Arr$(3, i32), {1, 2, 3}); \
     */ \
    comp_op__Arr_init$(_Arr$T, _Initial)
#define Arr_from$(T, _Initial...) \
    /** \
     * @brief Initializes array inferred size from initial values \
     * @param T Element type of the array \
     * @param _Initial Initial values \
     * @return Initialized array \
     * @example \
     *     var arr = Arr_from$(i32, {1, 2, 3}); \
     */ \
    comp_op__Arr_from$(T, _Initial)
#define Arr_asg(var_self, var_other...) comp_op__Arr_asg(var_self, var_other)

#define Arr_ref$(_Sli$T, _val_arr...) comp_op__Arr_ref$(_Sli$T, _val_arr)
#define Arr_ref(_val_arr...)          comp_op__Arr_ref(_val_arr)

#define Arr_ptr(var_self...) comp_op__Arr_ptr(var_self)
#define Arr_buf(var_self...) comp_op__Arr_buf(var_self)
#define Arr_len(var_self...) \
    /** \
     * @brief `arr.len` | Get number of elements in array \
     * @param var_self Array variable \
     * @return Length of the array \
     * @example \
     *     Arr$(3, i32) arr = Arr_init({1, 2, 3}); \
     *     usize len = Arr_len(arr);  // 3 \
     */ \
    comp_op__Arr_len(var_self)
#define Arr_at(var_self, usize_index...) \
    /** \
     * @brief `&arr[index]` | Get pointer to element at index (bounds-checked) \
     * @param var_self Array variable \
     * @param usize_index Index \
     * @return Reference to the element \
     * @details Performs bounds checking, triggers assertion in debug mode if index is out of bounds \
     * @example \
     *     Arr$(3, i32) arr = Arr_init({1, 2, 3}); \
     *     i32* pItem = Arr_at(arr, 1);  // &arr.buf[1] \
     */ \
    comp_op__Arr_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Arr_getAt(var_self, usize_index...) \
    /** \
     * @brief `arr[index]` | Get value of element at index (bounds-checked) \
     * @param var_self Array variable \
     * @param usize_index Index \
     * @return Element value \
     * @details Performs bounds checking, triggers assertion in debug mode if index is out of bounds \
     * @example \
     *     Arr$(3, i32) arr = Arr_init({1, 2, 3}); \
     *     i32 item = Arr_getAt(arr, 1);  // 2 \
     */ \
    comp_op__Arr_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Arr_setAt(var_self, usize_index, val_item...) \
    /** \
     * @brief `arr[index] = val` | Set value at index (bounds-checked) \
     * @param var_self Array variable \
     * @param usize_index Index \
     * @param val_item Value to set \
     * @return Reference to the array itself \
     * @details Performs bounds checking, triggers assertion in debug mode if index is out of bounds \
     * @example \
     *     Arr$(3, i32) arr = Arr_init({1, 2, 3}); \
     *     Arr_setAt(arr, 1, 5);  // arr = {1, 5, 3} \
     */ \
    comp_op__Arr_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

#define Arr_cat(var_self, var_other...) \
    /** \
     * @brief Concatenates two arrays \
     * @param var_self First array variable \
     * @param var_other Second array variable \
     * @return Concatenated array \
     * @example \
     *     Arr$(3, i32) arr1 = Arr_init({1, 2, 3}); \
     *     Arr$(3, i32) arr2 = Arr_init({4, 5, 6}); \
     *     var          arr3 = Arr_cat(arr1, arr2); \
     *     // arr3 = {1, 2, 3, 4, 5, 6} \
     */ \
    comp_op__Arr_cat(pp_uniqTok(temp), var_self, var_other)
#define Arr_cat$(_Arr$T, var_self, var_other...) \
    /** \
     * @brief Concatenates two arrays \
     * @param _Arr$T Target array type \
     * @param var_self First array variable \
     * @param var_other Second array variable \
     * @return Concatenated array \
     * @example \
     *     Arr$(3, i32) arr1 = Arr_init({1, 2, 3}); \
     *     Arr$(3, i32) arr2 = Arr_init({4, 5, 6}); \
     *     Arr$(6, i32) arr3 = Arr_cat$(Arr$(6, i32), arr1, arr2); \
     *     // arr3 = {1, 2, 3, 4, 5, 6} \
     */ \
    comp_op__Arr_cat$(pp_uniqTok(temp), _Arr$T, var_self, var_other)

#define Arr_slice(var_self, range_index_begin_end...) \
    /** \
     * @brief `arr[begin..end]` | Get slice from begin to end \
     * @param var_self Array variable \
     * @param range_index_begin_end Range and index \
     * @return Slice of the array \
     * @example \
     *     Arr$(3, i32) arr = Arr_init({1, 2, 3}); \
     *     Sli$(i32)    sli = Arr_slice(arr, (1, 2)); \
     *     // sli = {2} \
     */ \
    comp_op__Arr_slice(pp_uniqTok(self), pp_uniqTok(range), var_self, range_index_begin_end)
#define Arr_slice$(T_Sli, var_self, range_index_begin_end...) \
    /** \
     * @brief `arr[begin..end]` | Get specific slice from begin to end \
     * @param T_Sli Slice type \
     * @param var_self Array variable \
     * @param range_index_begin_end Range and index \
     * @return Slice of the array \
     * @example \
     *     Arr$(3, i32) arr = Arr_init({1, 2, 3}); \
     *     Sli$(i32)    sli = Arr_slice$(Sli$(i32), arr, (1, 2)); \
     *     // sli = {2} \
     */ \
    comp_op__Arr_slice$(T_Sli, pp_uniqTok(self), pp_uniqTok(range), var_self, range_index_begin_end)

#define for_array(var_arr, _Iter_item) \
    /** \
     * @brief Iterates through array elements \
     * @param var_arr Array variable \
     * @param _Iter_item Iterator variable for each element \
     * @example \
     *     Arr$(3, i32) arr = Arr_init({1, 2, 3}); \
     *     for_array (arr, item) { \
     *         printf("%d ", *item);  // "1 2 3" \
     *     } \
     */ \
    comp_syn__for_array(pp_uniqTok(arr), pp_uniqTok(i), var_arr, _Iter_item)
#define for_array_indexed(var_arr, _Iter_item, _Iter_index) \
    /** \
     * @brief Iterates through array elements with index \
     * @param var_arr Array variable \
     * @param _Iter_item Iterator variable for each element \
     * @param _Iter_index Index variable \
     * @example \
     *     Arr$(3, i32) arr = Arr_init({1, 2, 3}); \
     *     for_array_indexed (arr, item, idx) { \
     *         printf("[%zu]=%d ", idx, *item);  // "[0]=1 [1]=2 [2]=3" \
     *     } \
     */ \
    comp_syn__for_array_indexed(pp_uniqTok(arr), var_arr, _Iter_item, _Iter_index)
#define for_array_rev(var_arr, _Iter_item) \
    /** \
     * @brief Iterates through array elements in reverse order \
     * @param var_arr Array variable \
     * @param _Iter_item Iterator variable for each element \
     * @example \
     *     Arr$(3, i32) arr = Arr_init({1, 2, 3}); \
     *     for_array_rev (arr, item) { \
     *         printf("%d ", *item);  // "3 2 1" \
     *     } \
     */ \
    comp_syn__for_array_rev(pp_uniqTok(arr), pp_uniqTok(i), var_arr, _Iter_item)
#define for_array_rev_indexed(var_arr, _Iter_item, _Iter_index) \
    /** \
     * @brief Iterates through array elements in reverse order with index \
     * @param var_arr Array variable \
     * @param _Iter_item Iterator variable for each element \
     * @param _Iter_index Index variable \
     * @example \
     *     Arr$(3, i32) arr = Arr_init({1, 2, 3}); \
     *     for_array_rev_indexed (arr, item, idx) { \
     *         printf("[%zu]=%d ", idx, *item);  // "[2]=3 [1]=2 [0]=1" \
     *     } \
     */ \
    comp_syn__for_array_rev_indexed(pp_uniqTok(arr), var_arr, _Iter_item, _Iter_index)

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__use_Arr$(N, T) \
    decl_Arr$(N, T); \
    impl_Arr$(N, T)
#define comp_type_gen__decl_Arr$(N, T) \
    $maybe_unused typedef union Arr_const$(N, T) Arr_const$(N, T); \
    $maybe_unused typedef union Arr$(N, T) Arr$(N, T);
#define comp_type_gen__impl_Arr$(N, T...) \
    union Arr_const$(N, T) { \
        TypeOf(const T[N]) buf; \
    }; \
    union Arr$(N, T) { \
        TypeOf(T[N]) buf; \
        Arr_const$(N, T) as_const; \
    }

#define comp_type_alias__Arr_const$(N, T) \
    pp_join3($, Arr_const, N, T)
#define comp_type_alias__Arr$(N, T) \
    pp_join3($, Arr, N, T)
#define comp_type_anon__Arr$$(N, T) \
    struct { \
        T buf[N]; \
    }
#define comp_type_anon__ArrZ$$(N, T) \
    struct { \
        T buf[(N) + 1]; \
    }

#define comp_op__Arr_anonCast$(__anon, _Arr$T, var_anon...) eval({ \
    let_(__anon, TypeOf(&var_anon)) = &var_anon; \
    claim_assert_static(sizeOf(TypeOf(*__anon)) == sizeOf(_Arr$T)); \
    claim_assert_static(alignOf(TypeOf(*__anon)) == alignOf(_Arr$T)); \
    claim_assert_static(validateField(TypeOf(*__anon), buf, FieldTypeOf(_Arr$T, buf))); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), buf) == fieldPadding(_Arr$T, buf)); \
    eval_return rawderef(as$((rawptr$(_Arr$T))(__anon))); \
})

#define comp_op__Arr_zero()                     { .buf = { 0 } }
#define comp_op__Arr_zero$(_Arr$T)              ((_Arr$T)Arr_zero())
#define comp_op__Arr_init(_Initial...)          { .buf = _Initial }
#define comp_op__Arr_init$(_Arr$T, _Initial...) ((_Arr$T)Arr_init(_Initial))
#if comp_Arr_allows_gnu_folding_constant_for_inferred_len
#define comp_op__Arr_from$(_T, _Initial...) pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wgnu-folding-constant\"", \
    "clang diagnostic pop", \
    Arr_init$(Arr$$(eval({ eval_return sizeOf((_T[])_Initial) / sizeOf(_T); }), _T), _Initial) \
)
#else /* !comp_Arr_allows_gnu_folding_constant_for_inferred_len */
#define comp_op__Arr_from$(_T, _Initial...) Arr_init$(Arr$$(pp_countArg(_Initial), _T), _Initial)
#endif /* !comp_Arr_allows_gnu_folding_constant_for_inferred_len */
#define comp_op__Arr_asg(var_self, var_other...) eval({ \
    let __self  = &var_self; \
    let __other = &var_other; \
    claim_assert_static(sizeOf(TypeOf(*__self)) == sizeOf(TypeOf(*__other))); \
    claim_assert_static(alignOf(TypeOf(*__self)) == alignOf(TypeOf(*__other))); \
    claim_assert_static(isSameType(TypeOf((*__self).buf), TypeOf((*__other).buf))); \
    eval_return deref(__self) = deref(as$((TypeOf(__self))(__other))); \
})

#define comp_op__Arr_ref$(_Sli$T, _val_arr...) \
    ((_Sli$T){ .ptr = Arr_ptr(_val_arr), Arr_len(_val_arr) })
#define comp_op__Arr_ref(_val_arr...) \
    Arr_ref$(Sli$$(TypeOf(*Arr_ptr(_val_arr))), _val_arr)

#define comp_op__Arr_buf(var_self...)                              ((var_self).buf)
#define comp_op__Arr_ptr(var_self...)                              (&*Arr_buf(var_self))
#define comp_op__Arr_len(var_self...)                              countOf((var_self).buf)
#define comp_op__Arr_at(__self, __index, var_self, usize_index...) eval({ \
    let_(__self, TypeOf(&var_self)) = &var_self; \
    const usize __index             = usize_index; \
    claim_assert_static_msg( \
        __builtin_constant_p(__index) \
            ? (__index < Arr_len(*__self)) \
            : true, \
        "index out of bounds" \
    ); \
    debug_assert_fmt( \
        __index <= Arr_len(*__self), \
        "Index out of bounds: %zu > %zu", \
        __index, \
        Arr_len(*__self) \
    ); \
    eval_return rawref(__self->buf[__index]); \
})
#define comp_op__Arr_getAt(__self, __index, var_self, usize_index...) eval({ \
    const TypeOf(&var_self) __self = &var_self; \
    const usize __index            = usize_index; \
    debug_assert_fmt( \
        __index <= Arr_len(*__self), \
        "Index out of bounds: %zu > %zu", \
        __index, \
        Arr_len(*__self) \
    ); \
    eval_return __self->buf[__index]; \
})
#define comp_op__Arr_setAt(__self, __index, var_self, usize_index, var_value...) eval({ \
    const TypeOf(&var_self) __self = &var_self; \
    const usize __index            = usize_index; \
    debug_assert_fmt( \
        __index <= Arr_len(*__self), \
        "Index out of bounds: %zu > %zu", \
        __index, \
        Arr_len(*__self) \
    ); \
    __self->buf[__index] = as$((TypeOf(__self->buf[0]))(var_value)); \
    eval_return __self; \
})

#define comp_op__Arr_cat(__temp, var_self, var_other...) eval({ \
    claim_assert_static(isSameType(TypeOf(var_self.buf[0]), TypeOf(var_other.buf[0]))); \
    union { \
        Arr$$( \
            countOf((var_self).buf) + countOf((var_other).buf), \
            TypeOf((var_self).buf[0]) \
        ) concatted; \
        struct { \
            TypeOf(var_self) lhs; \
            TypeOf(var_other) rhs; \
        }; \
    } __temp = { .lhs = var_self, .rhs = var_other }; \
    eval_return __temp.concatted; \
})
#define comp_op__Arr_cat$(__temp, _Arr$T, var_self, var_other...) eval({ \
    claim_assert_static(isSameType(TypeOf(var_self.buf[0]), TypeOf(var_other.buf[0]))); \
    union { \
        _Arr$T concatted; \
        struct { \
            TypeOf(var_self) lhs; \
            TypeOf(var_other) rhs; \
        }; \
    } __temp = { .lhs = var_self, .rhs = var_other }; \
    eval_return __temp.concatted; \
})

#define comp_op__Arr_slice(__self, __range, var_self, range_index_begin_end...) eval({ \
    let_(__self, TypeOf(var_self)*) = &var_self; \
    let_(__range, R)                = range_index_begin_end; \
    debug_assert_fmt( \
        __range.begin <= __range.end, \
        "Invalid slice range: begin(%zu) > end(%zu)", \
        __range.begin, \
        __range.end \
    ); \
    debug_assert_fmt( \
        __range.end <= Arr_len(*__self), \
        "Index out of bounds: end(%zu) > len(%zu)", \
        __range.end, \
        Arr_len(*__self) \
    ); \
    eval_return make$( \
        Sli$$(TypeOf(__self->buf[0])), \
        .ptr = __self->buf + __range.begin, \
        .len = R_len(__range) \
    ); \
})
#define comp_op__Arr_slice$(T_Sli, __self, __range, var_self, range_index_begin_end...) eval({ \
    let_(__self, TypeOf(&var_self)) = &var_self; \
    let_(__range, R)                = range_index_begin_end; \
    debug_assert_fmt( \
        __range.begin <= __range.end, \
        "Invalid slice range: begin(%zu) > end(%zu)", \
        __range.begin, \
        __range.end \
    ); \
    debug_assert_fmt( \
        __range.end <= Arr_len(*__self), \
        "Index out of bounds: end(%zu) > len(%zu)", \
        __range.end, \
        Arr_len(*__self) \
    ); \
    eval_return make$( \
        T_Sli, \
        .ptr = __self->buf + __range.begin, \
        .len = R_len(__range) \
    ); \
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
