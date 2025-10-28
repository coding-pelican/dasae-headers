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

/*========== Header =======================================================*/

#ifndef ARR_INCLUDED
#define ARR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/types.h"

#if UNUSED_CODE
/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

#define use_A$(N, T) \
    /** \
     * @brief Declares and implements an array type \
     * @param N Size of the array \
     * @param T Element type of the array \
     * @example \
     *     use_A$(4, u8);  // Declares and implements [4]u8 array type \
     */ \
    comp_type_gen__use_A$(N, T)
#define decl_A$(N, T) \
    /** \
     * @brief Declares an array type without implementation \
     * @param N Size of the array \
     * @param T Element type of the array \
     * @example \
     *     decl_A$(3, i32);  // Declares [3]i32 array type \
     */ \
    comp_type_gen__decl_A$(N, T)
#define impl_A$(N, T) \
    /** \
     * @brief Implements a previously declared array type \
     * @param N Size of the array \
     * @param T Element type of the array \
     * @example \
     *     impl_A$(3, i32);  // Implements [3]i32 array type \
     */ \
    comp_type_gen__impl_A$(N, T)

#define A_const$(N, T) \
    comp_type_alias__A_const$(N, T)
#define A$(N, T) \
    comp_type_alias__A$(N, T)
#define A$$(N, T) \
    /** \
     * @brief Creates an anonymous array type \
     * @param N Size of the array \
     * @param T Element type of the array \
     * @return Anonymous array type alias \
     * @example \
     *     typedef A$$(3, i32) MyArrType;  // Anonymous [3]i32 type \
     */ \
    comp_type_anon__A$$(N, T)
#define A_anonCast$(_A$T, var_anon...) \
    /** \
     * @brief Casts an anonymous array to a specific array type \
     * @param _A$T Target array type \
     * @param var_anon Anonymous array variable \
     * @return Cast array of specified type \
     * @details Performs type safety checks and converts the anonymous array to the specified type \
     * @example \
     *     A$3$i32 typed = A_anonCast$(A$3$i32, anon); \
     */ \
    comp_op__A_anonCast$(pp_uniqTok(anon), _A$T, var_anon)

#define A_zero() \
    /** \
     * @brief Initializes array with zeros \
     * @return Zero-initialized array literal \
     * @example \
     *     A$(3, i32) arr = A_zero();  // {0, 0, 0} \
     */ \
    comp_op__A_zero()
#define A_zero$(_A$T) \
    /** \
     * @brief Initializes specified array type with zeros \
     * @param _A$T Array type \
     * @return Zero-initialized array \
     * @example \
     *     A$(3, i32) arr = A_zero$(A$(3, i32));  // {0, 0, 0} \
     */ \
    comp_op__A_zero$(_A$T)

#define A_init(_Initial...) \
    /** \
     * @brief Initializes array with specified values \
     * @param _Initial Initial values \
     * @return Initialized array literal \
     * @example \
     *     A$(3, i32) arr = A_init({1, 2, 3}); \
     */ \
    comp_op__A_init(_Initial)
#define A_init$(_A$T, _Initial...) \
    /** \
     * @brief Initializes specified array type with values \
     * @param _A$T Array type \
     * @param _Initial Initial values \
     * @return Initialized array \
     * @example \
     *     A$(3, i32) arr = A_init$(A$(3, i32), {1, 2, 3}); \
     */ \
    comp_op__A_init$(_A$T, _Initial)
#define A_from$(T, _Initial...) \
    /** \
     * @brief Initializes array inferred size from initial values \
     * @param T Element type of the array \
     * @param _Initial Initial values \
     * @return Initialized array \
     * @example \
     *     var arr = A_from$(i32, {1, 2, 3}); \
     */ \
    comp_op__A_from$(T, _Initial)
#define A_asg(var_self, var_other...) comp_op__A_asg(var_self, var_other)

#define ref$A(var_self...) comp_op__A_ref(var_self)

#define A_ref$(_S$T, _val_arr...) comp_op__A_ref$(_S$T, _val_arr)
#define A_ref(_val_arr...)        comp_op__A_ref(_val_arr)

#define val$A(var_self...)             comp_op__A_val(var_self)
#define ptr$A(var_self...)             comp_op__A_ptr(var_self)
#define len$A(var_self...)             comp_op__A_len(var_self)
#define at$A(var_self, usize_index...) A_at(var_self, usize_index)

#define A_val(var_self...) comp_op__A_val(var_self)
#define A_ptr(var_self...) comp_op__A_ptr(var_self)
#define A_len(var_self...) \
    /** \
     * @brief `arr.len` | Get number of elements in array \
     * @param var_self Array variable \
     * @return Length of the array \
     * @example \
     *     A$(3, i32) arr = A_init({1, 2, 3}); \
     *     usize len = A_len(arr);  // 3 \
     */ \
    comp_op__A_len(var_self)
#define A_at(var_self, usize_index...) \
    /** \
     * @brief `&arr[index]` | Get pointer to element at index (bounds-checked) \
     * @param var_self Array variable \
     * @param usize_index Index \
     * @return Reference to the element \
     * @details Performs bounds checking, triggers assertion in debug mode if index is out of bounds \
     * @example \
     *     A$(3, i32) arr = A_init({1, 2, 3}); \
     *     i32* pItem = A_at(arr, 1);  // &arr.val[1] \
     */ \
    comp_op__A_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define A_getAt(var_self, usize_index...) \
    /** \
     * @brief `arr[index]` | Get value of element at index (bounds-checked) \
     * @param var_self Array variable \
     * @param usize_index Index \
     * @return Element value \
     * @details Performs bounds checking, triggers assertion in debug mode if index is out of bounds \
     * @example \
     *     A$(3, i32) arr = A_init({1, 2, 3}); \
     *     i32 item = A_getAt(arr, 1);  // 2 \
     */ \
    comp_op__A_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define A_setAt(var_self, usize_index, val_item...) \
    /** \
     * @brief `arr[index] = val` | Set value at index (bounds-checked) \
     * @param var_self Array variable \
     * @param usize_index Index \
     * @param val_item Value to set \
     * @return Reference to the array itself \
     * @details Performs bounds checking, triggers assertion in debug mode if index is out of bounds \
     * @example \
     *     A$(3, i32) arr = A_init({1, 2, 3}); \
     *     A_setAt(arr, 1, 5);  // arr = {1, 5, 3} \
     */ \
    comp_op__A_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

#define A_cat(var_self, var_other...) \
    /** \
     * @brief Concatenates two arrays \
     * @param var_self First array variable \
     * @param var_other Second array variable \
     * @return Concatenated array \
     * @example \
     *     A$(3, i32) arr1 = A_init({1, 2, 3}); \
     *     A$(3, i32) arr2 = A_init({4, 5, 6}); \
     *     var          arr3 = A_cat(arr1, arr2); \
     *     // arr3 = {1, 2, 3, 4, 5, 6} \
     */ \
    comp_op__A_cat(pp_uniqTok(temp), var_self, var_other)
#define A_cat$(_A$T, var_self, var_other...) \
    /** \
     * @brief Concatenates two arrays \
     * @param _A$T Target array type \
     * @param var_self First array variable \
     * @param var_other Second array variable \
     * @return Concatenated array \
     * @example \
     *     A$(3, i32) arr1 = A_init({1, 2, 3}); \
     *     A$(3, i32) arr2 = A_init({4, 5, 6}); \
     *     A$(6, i32) arr3 = A_cat$(A$(6, i32), arr1, arr2); \
     *     // arr3 = {1, 2, 3, 4, 5, 6} \
     */ \
    comp_op__A_cat$(pp_uniqTok(temp), _A$T, var_self, var_other)

#define slice$A(var_self, range_index_begin_end...) A_slice(var_self, range_index_begin_end)
#define prefix$A(var_self, usize_index_end...)      A_prefix(var_self, usize_index_end)
#define suffix$A(var_self, usize_index_begin...)    A_suffix(var_self, usize_index_begin)

#define A_slice(var_self, range_index_begin_end...) \
    /** \
     * @brief `arr[begin..end]` | Get slice from begin to end \
     * @param var_self Array variable \
     * @param range_index_begin_end Range and index \
     * @return Slice of the array \
     * @example \
     *     A$(3, i32) arr = A_init({1, 2, 3}); \
     *     S$(i32)    sli = A_slice(arr, (1, 2)); \
     *     // sli = {2} \
     */ \
    comp_op__A_slice(pp_uniqTok(self), pp_uniqTok(range), var_self, range_index_begin_end)
#define A_slice$(T_Sli, var_self, range_index_begin_end...) \
    /** \
     * @brief `arr[begin..end]` | Get specific slice from begin to end \
     * @param T_Sli Slice type \
     * @param var_self Array variable \
     * @param range_index_begin_end Range and index \
     * @return Slice of the array \
     * @example \
     *     A$(3, i32) arr = A_init({1, 2, 3}); \
     *     S$(i32)    sli = A_slice$(S$(i32), arr, (1, 2)); \
     *     // sli = {2} \
     */ \
    comp_op__A_slice$(T_Sli, pp_uniqTok(self), pp_uniqTok(range), var_self, range_index_begin_end)

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__use_A$(N, T) \
    decl_A$(N, T); \
    impl_A$(N, T)
#define comp_type_gen__decl_A$(N, T) \
    $maybe_unused typedef struct A$(N, T) A$(N, T);
#define comp_type_gen__impl_A$(N, T...) \
    struct A$(N, T) { \
        TypeOf(T[N]) val; \
    }

#define comp_type_alias__A$(N, T) \
    pp_join3($, A, N, T)
#define comp_type_anon__A$$(N, T) \
    struct { \
        T val[N]; \
    }

#define comp_op__A_anonCast$(__anon, _A$T, var_anon...) blk({ \
    let_(__anon, TypeOf(&var_anon)) = &var_anon; \
    claim_assert_static(sizeOf(TypeOf(*__anon)) == sizeOf(_A$T)); \
    claim_assert_static(alignOf(TypeOf(*__anon)) == alignOf(_A$T)); \
    claim_assert_static(validateField(TypeOf(*__anon), val, FieldTypeOf(_A$T, val))); \
    claim_assert_static(fieldPadding(TypeOf(*__anon), val) == fieldPadding(_A$T, val)); \
    blk_return rawderef(as$(($P$(_A$T))(__anon))); \
})

#define comp_op__A_zero()                   { .val = { 0 } }
#define comp_op__A_zero$(_A$T)              ((_A$T)A_zero())
#define comp_op__A_init(_Initial...)        { .val = _Initial }
#define comp_op__A_init$(_A$T, _Initial...) ((_A$T)A_init(_Initial))
#if comp_A_allows_gnu_folding_constant_for_inferred_len
#define comp_op__A_from$(_T, _Initial...) pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wgnu-folding-constant\"", \
    "clang diagnostic pop", \
    A_init$(A$$(blk({ blk_return sizeOf((_T[])_Initial) / sizeOf(_T); }), _T), _Initial) \
)
#else /* !comp_A_allows_gnu_folding_constant_for_inferred_len */
#define comp_op__A_from$(_T, _Initial...) A_init$(A$$(pp_countArg(_Initial), _T), _Initial)
#endif /* !comp_A_allows_gnu_folding_constant_for_inferred_len */
#define comp_op__A_asg(var_self, var_other...) blk({ \
    let __self  = &var_self; \
    let __other = &var_other; \
    claim_assert_static(sizeOf$(TypeOf(*__self)) == sizeOf$(TypeOf(*__other))); \
    claim_assert_static(alignOf$(TypeOf(*__self)) == alignOf$(TypeOf(*__other))); \
    claim_assert_static(isSameType$(TypeOf((*__self).val), TypeOf((*__other).val))); \
    blk_return deref(__self) = deref(as$((TypeOf(__self))(__other))); \
})

#define comp_op__A_ref$(_S$T, _val_arr...) \
    ((_S$T){ .ptr = A_ptr(_val_arr), A_len(_val_arr) })
#define comp_op__A_ref(_val_arr...) \
    A_ref$(S$$(TypeOf(*A_ptr(_val_arr))), _val_arr)

#define comp_op__A_val(var_self...)                              ((var_self).val)
#define comp_op__A_ptr(var_self...)                              (&*A_val(var_self))
#define comp_op__A_len(var_self...)                              countOf$(TypeOf((var_self).val))
#define comp_op__A_at(__self, __index, var_self, usize_index...) blk({ \
    let_(__self, TypeOf(&var_self)) = &var_self; \
    const usize __index             = usize_index; \
    claim_assert_static_msg( \
        isConstExpr(__index) \
            ? (__index < A_len(*__self)) \
            : true, \
        "index out of bounds" \
    ); \
    debug_assert_fmt( \
        __index <= A_len(*__self), \
        "Index out of bounds: %zu > %zu", \
        __index, \
        A_len(*__self) \
    ); \
    blk_return rawref(__self->val[__index]); \
})
#define comp_op__A_getAt(__self, __index, var_self, usize_index...) blk({ \
    const TypeOf(&var_self) __self = &var_self; \
    const usize __index            = usize_index; \
    debug_assert_fmt( \
        __index <= A_len(*__self), \
        "Index out of bounds: %zu > %zu", \
        __index, \
        A_len(*__self) \
    ); \
    blk_return __self->val[__index]; \
})
#define comp_op__A_setAt(__self, __index, var_self, usize_index, var_value...) blk({ \
    const TypeOf(&var_self) __self = &var_self; \
    const usize __index            = usize_index; \
    debug_assert_fmt( \
        __index <= A_len(*__self), \
        "Index out of bounds: %zu > %zu", \
        __index, \
        A_len(*__self) \
    ); \
    __self->val[__index] = as$((TypeOf(__self->val[0]))(var_value)); \
    blk_return __self; \
})

#define comp_op__A_cat(__temp, var_self, var_other...) blk({ \
    claim_assert_static(isSameType$(TypeOf(var_self.val[0]), TypeOf(var_other.val[0]))); \
    union { \
        A$$( \
            countOf$(TypeOf((var_self).val)) + countOf$(TypeOf((var_other).val)), \
            TypeOf((var_self).val[0]) \
        ) \
        concatted; \
        struct { \
            TypeOf(var_self) lhs; \
            TypeOf(var_other) rhs; \
        }; \
    } __temp = { .lhs = var_self, .rhs = var_other }; \
    blk_return __temp.concatted; \
})
#define comp_op__A_cat$(__temp, _A$T, var_self, var_other...) blk({ \
    claim_assert_static(isSameType$(TypeOf(var_self.val[0]), TypeOf(var_other.val[0]))); \
    union { \
        _A$T concatted; \
        struct { \
            TypeOf(var_self) lhs; \
            TypeOf(var_other) rhs; \
        }; \
    } __temp = { .lhs = var_self, .rhs = var_other }; \
    blk_return __temp.concatted; \
})

#define comp_op__A_slice(__self, __range, var_self, range_index_begin_end...) blk({ \
    let_(__self, TypeOf(var_self)*) = &var_self; \
    let_(__range, R)                = range_index_begin_end; \
    debug_assert_fmt( \
        __range.begin <= __range.end, \
        "Invalid slice range: begin(%zu) > end(%zu)", \
        __range.begin, \
        __range.end \
    ); \
    debug_assert_fmt( \
        __range.end <= A_len(*__self), \
        "Index out of bounds: end(%zu) > len(%zu)", \
        __range.end, \
        A_len(*__self) \
    ); \
    blk_return make$( \
        S$$(TypeOf(__self->val[0])), \
        .ptr = __self->val + __range.begin, \
        .len = R_len(__range) \
    ); \
})
#define comp_op__A_slice$(T_Sli, __self, __range, var_self, range_index_begin_end...) blk({ \
    let_(__self, TypeOf(&var_self)) = &var_self; \
    let_(__range, R)                = range_index_begin_end; \
    debug_assert_fmt( \
        __range.begin <= __range.end, \
        "Invalid slice range: begin(%zu) > end(%zu)", \
        __range.begin, \
        __range.end \
    ); \
    debug_assert_fmt( \
        __range.end <= A_len(*__self), \
        "Index out of bounds: end(%zu) > len(%zu)", \
        __range.end, \
        A_len(*__self) \
    ); \
    blk_return make$( \
        T_Sli, \
        .ptr = __self->val + __range.begin, \
        .len = R_len(__range) \
    ); \
})

#define comp_syn__for_array(__arr, __i, var_arr, _Iter_item) \
    with_(let __arr = &(var_arr)) for (usize __i = 0; (__i) < A_len(*(__arr)); ++(__i)) with_(let _Iter_item = A_at(*(__arr), __i))
#define comp_syn__for_array_indexed(__arr, var_arr, _Iter_item, _Iter_index) \
    with_(let __arr = &(var_arr)) for (usize _Iter_index = 0; (_Iter_index) < A_len(*(__arr)); ++(_Iter_index)) with_(let _Iter_item = A_at(*(__arr), _Iter_index))
#define comp_syn__for_array_rev(__arr, __i, var_arr, _Iter_item) \
    with_(let __arr = &(var_arr)) for (usize __i = A_len(*(__arr)); (__i)-- > 0;) with_(let _Iter_item = A_at(*(__arr), __i))
#define comp_syn__for_array_indexed_rev(__arr, var_arr, _Iter_item, _Iter_index) \
    with_(let __arr = &(var_arr)) for (usize _Iter_index = A_len(*(__arr)); (_Iter_index)-- > 0;) with_(let _Iter_item = A_at(*(__arr), _Iter_index))
#endif /* UNUSED_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ARR_INCLUDED */
