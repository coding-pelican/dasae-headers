/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Arr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-17 (date of creation)
 * @updated 2025-01-21 (date of last update)
 * @version v0.1-alpha.2
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

/* Property */
#define Arr_len(var_self) OP__Arr_len(var_self)

/* Access */
#define Arr_at(var_self, usize_index)                 OP__Arr_at(var_self, usize_index)
#define Arr_getAt(var_self, usize_index)              OP__Arr_getAt(var_self, usize_index)
#define Arr_setAt(var_self, usize_index, val_item...) OP__Arr_setAt(var_self, usize_index, val_item)

/* Slice */
#define Arr_slice(var_self, usize_begin, usize_end) OP__Arr_slice(var_self, usize_begin, usize_end)
#define Arr_prefix(var_self, usize_end)             OP__Arr_prefix(var_self, usize_end)
#define Arr_suffix(var_self, usize_begin)           OP__Arr_suffix(var_self, usize_begin)

#define for_array(var_arr, var_item)                    SYN__for_array(var_arr, var_item)
#define for_array_indexed(var_arr, var_item, var_index) SYN__for_array_indexed(var_arr, var_item, var_index)

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

#define OP__Arr_asNamed$(TNamedArr, var_unnamed_arr) eval({                                            \
    let _unnamed_arr = &(var_unnamed_arr);                                                             \
    claim_assert_static(sizeOf(TypeOf(*_unnamed_arr)) == sizeOf(TNamedArr));                           \
    claim_assert_static(alignOf(TypeOf(*_unnamed_arr)) == alignOf(TNamedArr));                         \
    claim_assert_static(hasField(TypeOf(*_unnamed_arr), items));                                       \
    claim_assert_static(validateField(TypeOf(*_unnamed_arr), items, FieldTypeOf(TNamedArr, items)));   \
    claim_assert_static(fieldPadding(TypeOf(*_unnamed_arr), items) == fieldPadding(TNamedArr, items)); \
    eval_return(                                                                                       \
        *as$(rawptr$(TNamedArr), _unnamed_arr)                                                         \
    );                                                                                                 \
})

#define OP__Arr_len(var_self) safeCountOf((var_self).items)

#define OP__Arr_at(var_self, usize_index) eval({                                \
    let         pp_uniqTok(__self)  = &(var_self);                              \
    const usize pp_uniqTok(__index) = usize_index;                              \
    debug_assert(pp_uniqTok(__index) < safeCountOf(pp_uniqTok(__self)->items)); \
    eval_return(                                                                \
        &pp_uniqTok(__self)->items[pp_uniqTok(__index)]                         \
    );                                                                          \
})

#define OP__Arr_getAt(var_self, usize_index) eval({                             \
    let         pp_uniqTok(__self)  = &(var_self);                              \
    const usize pp_uniqTok(__index) = usize_index;                              \
    debug_assert(pp_uniqTok(__index) < safeCountOf(pp_uniqTok(__self)->items)); \
    eval_return(                                                                \
        pp_uniqTok(__self)->items[pp_uniqTok(__index)]                          \
    );                                                                          \
})

#define OP__Arr_setAt(var_self, usize_index, var_value) eval({                                             \
    let         pp_uniqTok(__self)  = &(var_self);                                                         \
    const usize pp_uniqTok(__index) = usize_index;                                                         \
    debug_assert(pp_uniqTok(__index) < safeCountOf(pp_uniqTok(__self)->items));                            \
    pp_uniqTok(__self)->items[pp_uniqTok(__index)] = as$(TypeOf(pp_uniqTok(__self)->items[0]), var_value); \
})

#define OP__Arr_slice(var_self, usize_begin, usize_end) eval({                 \
    let         pp_uniqTok(__self)  = &(var_self);                             \
    const usize pp_uniqTok(__begin) = usize_begin;                             \
    const usize pp_uniqTok(__end)   = usize_end;                               \
    debug_assert(pp_uniqTok(__begin) < pp_uniqTok(__end));                     \
    debug_assert(pp_uniqTok(__end) <= safeCountOf(pp_uniqTok(__self)->items)); \
    eval_return(                                                               \
        make$(                                                                 \
            Sli$(TypeOf(*pp_uniqTok(__self)->items)),                          \
            .ptr = pp_uniqTok(__self)->items + pp_uniqTok(__begin),            \
            .len = pp_uniqTok(__end) - pp_uniqTok(__begin)                     \
        )                                                                      \
    );                                                                         \
})

#define OP__Arr_prefix(var_self, usize_end) eval({                             \
    let         pp_uniqTok(__self) = &(var_self);                              \
    const usize pp_uniqTok(__end)  = usize_end;                                \
    debug_assert(pp_uniqTok(__end) <= safeCountOf(pp_uniqTok(__self)->items)); \
    eval_return(                                                               \
        make$(                                                                 \
            Sli$(TypeOf(*pp_uniqTok(__self)->items)),                          \
            .ptr = pp_uniqTok(__self)->items,                                  \
            .len = pp_uniqTok(__end)                                           \
        )                                                                      \
    );                                                                         \
})

#define OP__Arr_suffix(var_self, usize_begin) eval({                            \
    let         pp_uniqTok(__self)  = &(var_self);                              \
    const usize pp_uniqTok(__begin) = usize_begin;                              \
    debug_assert(pp_uniqTok(__begin) < safeCountOf(pp_uniqTok(__self)->items)); \
    eval_return(                                                                \
        make$(                                                                  \
            Sli$(TypeOf(*pp_uniqTok(__self)->items)),                           \
            .ptr = pp_uniqTok(__self)->items + pp_uniqTok(__begin),             \
            .len = safeCountOf(pp_uniqTok(__self)->items) - pp_uniqTok(__begin) \
        )                                                                       \
    );                                                                          \
})

#define SYN__for_array(var_arr, var_item)                   \
    for (usize _i = 0; _i < countOf((var_arr).items); ++_i) \
    scope_with(let var_item = Arr_at(var_arr, _i))

#define SYN__for_array_indexed(var_arr, var_item, var_index)                         \
    for (usize var_index = 0; (var_index) < countOf((var_arr).items); ++(var_index)) \
    scope_with(let var_item = Arr_at(var_arr, var_index))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ARR_INCLUDED */
