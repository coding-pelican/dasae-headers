/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Arr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-17 (date of creation)
 * @updated 2025-01-18 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  Arr
 */

#ifndef ARR_INCLUDED
#define ARR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/meta.h"

/*========== Macros and Definitions =========================================*/

#define Arr$(N, T)                               TYPE_UNNAMED__Arr$(N, T)
#define use_Arr$(N, T)                           GEN__use_Arr$(N, T)
#define decl_Arr$(N, T)                          GEN__decl_Arr$(N, T)
#define impl_Arr$(N, T)                          GEN__impl_Arr$(N, T)
#define Arr_asNamed$(TNamedArr, var_unnamed_arr) OP__Arr_asNamed$(TNamedArr, var_unnamed_arr)

#define Arr_at(var_self, usize_index) OP__Arr_at(pp_uniqueToken(_self), pp_uniqueToken(_index), var_self, usize_index)

/*========== Implementations ================================================*/

#define TYPE_UNNAMED__Arr$(N, T) \
    struct {                     \
        T items[N];              \
    }
#define GEN__use_Arr$(N, T) \
    decl_Arr$(N, T);        \
    impl_Arr$(N, T)
#define GEN__decl_Arr$(N, T) \
    typedef struct pp_join3($, Arr, N, T) pp_join3($, Arr, N, T)
#define GEN__impl_Arr$(N, T)        \
    struct pp_join3($, Arr, N, T) { \
        T items[N];                 \
    }
#define OP__Arr_asNamed$(TNamedArr, var_unnamed_arr) eval(                                             \
    let _unnamed_arr = &(var_unnamed_arr);                                                             \
    claim_assert_static(sizeOf(TypeOf(*_unnamed_arr)) == sizeOf(TNamedArr));                           \
    claim_assert_static(alignOf(TypeOf(*_unnamed_arr)) == alignOf(TNamedArr));                         \
    claim_assert_static(hasField(TypeOf(*_unnamed_arr), items));                                       \
    claim_assert_static(validateField(TypeOf(*_unnamed_arr), items, FieldTypeOf(TNamedArr, items)));   \
    claim_assert_static(fieldPadding(TypeOf(*_unnamed_arr), items) == fieldPadding(TNamedArr, items)); \
    eval_return(*as$(rawptr$(TNamedArr), _unnamed_arr));                                               \
)

#define OP__Arr_at(_self, _index, var_self, usize_index) eval( \
    let         _self  = &(var_self);                          \
    const usize _index = usize_index;                          \
    debug_assert((_index) < countOf((_self)->items));          \
    eval_return(&(_self)->items[_index]);                      \
)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ARR_INCLUDED */
