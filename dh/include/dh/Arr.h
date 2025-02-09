/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Arr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-17 (date of creation)
 * @updated 2025-02-01 (date of last update)
 * @version v0.1-alpha.3
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

#define Arr$(N, T...)                            TYPE_UNNAMED__Arr$(N, T)
#define use_Arr$(N, T...)                        GEN__use_Arr$(N, T)
#define decl_Arr$(N, T...)                       GEN__decl_Arr$(N, T)
#define impl_Arr$(N, T...)                       GEN__impl_Arr$(N, T)
#define Arr_asNamed$(TNamedArr, var_unnamed_arr) OP__Arr_asNamed$(TNamedArr, var_unnamed_arr)

/* Property */
#define Arr_len(var_self) OP__Arr_len(var_self)

/* Access */
#define Arr_at(var_self, usize_index)                 OP__Arr_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Arr_getAt(var_self, usize_index)              OP__Arr_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Arr_setAt(var_self, usize_index, val_item...) OP__Arr_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

/* Slice */
#define Arr_slice(var_self, usize_begin, usize_end) OP__Arr_slice(pp_uniqTok(self), pp_uniqTok(begin), pp_uniqTok(end), var_self, usize_begin, usize_end)
#define Arr_prefix(var_self, usize_end)             OP__Arr_prefix(pp_uniqTok(self), pp_uniqTok(end), var_self, usize_end)
#define Arr_suffix(var_self, usize_begin)           OP__Arr_suffix(pp_uniqTok(self), pp_uniqTok(begin), var_self, usize_begin)

#define for_array(var_arr, _Iter_item)                      SYN__for_array(pp_uniqTok(arr), pp_uniqTok(i), var_arr, _Iter_item)
#define for_array_indexed(var_arr, _Iter_item, _Iter_index) SYN__for_array_indexed(pp_uniqTok(arr), var_arr, _Iter_item, _Iter_index)

#define for_array_rev(var_arr, _Iter_item)                      SYN__for_array_rev(pp_uniqTok(arr), pp_uniqTok(i), var_arr, _Iter_item)
#define for_array_rev_indexed(var_arr, _Iter_item, _Iter_index) SYN__for_array_rev_indexed(pp_uniqTok(arr), var_arr, _Iter_item, _Iter_index)

/*========== Implementations ================================================*/

#define TYPE_UNNAMED__Arr$(N, T...) \
    struct {                        \
        T items[N];                 \
    }

#define GEN__use_Arr$(N, T...) \
    decl_Arr$(N, T);           \
    impl_Arr$(N, T)

#define GEN__decl_Arr$(N, T...) \
    typedef struct pp_join3($, Arr, N, T) pp_join3($, Arr, N, T)

#define GEN__impl_Arr$(N, T...)     \
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

#define OP__Arr_len(var_self) countOf((var_self).items)

#define OP__Arr_at(__self, __index, var_self, usize_index) eval({ \
    let         __self  = &(var_self);                            \
    const usize __index = usize_index;                            \
    debug_assert((__index) < Arr_len(*(__self)));                 \
    eval_return(                                                  \
        &(__self)->items[__index]                                 \
    );                                                            \
})

#define OP__Arr_getAt(__self, __index, var_self, usize_index) eval({ \
    let         __self  = &(var_self);                               \
    const usize __index = usize_index;                               \
    debug_assert((__index) < Arr_len(*(__self)));                    \
    eval_return(                                                     \
        (__self)->items[__index]                                     \
    );                                                               \
})

#define OP__Arr_setAt(__self, __index, var_self, usize_index, var_value) eval({ \
    let         __self  = &(var_self);                                          \
    const usize __index = usize_index;                                          \
    debug_assert((__index) < Arr_len(*(__self)));                               \
    (__self)->items[(__index)] = as$(TypeOf((__self)->items[0]), var_value);    \
})

#define OP__Arr_slice(__self, __begin, __end, var_self, usize_begin, usize_end) eval({ \
    let         __self  = &(var_self);                                                 \
    const usize __begin = usize_begin;                                                 \
    const usize __end   = usize_end;                                                   \
    debug_assert((__begin) < (__end));                                                 \
    debug_assert((__end) <= Arr_len(*(__self)));                                       \
    eval_return(                                                                       \
        make$(                                                                         \
            Sli$(TypeOf(*(__self)->items)),                                            \
            .ptr = (__self)->items + (__begin),                                        \
            .len = (__end) - (__begin)                                                 \
        )                                                                              \
    );                                                                                 \
})

#define OP__Arr_prefix(__self, __end, var_self, usize_end) eval({ \
    let         __self = &(var_self);                             \
    const usize __end  = usize_end;                               \
    debug_assert((__end) <= Arr_len(*(__self)));                  \
    eval_return(                                                  \
        make$(                                                    \
            Sli$(TypeOf(*(__self)->items)),                       \
            .ptr = (__self)->items,                               \
            .len = (__end)                                        \
        )                                                         \
    );                                                            \
})

#define OP__Arr_suffix(__self, __begin, var_self, usize_begin) eval({ \
    let         __self  = &(var_self);                                \
    const usize __begin = usize_begin;                                \
    debug_assert((__begin) < Arr_len(*(__self)));                     \
    eval_return(                                                      \
        make$(                                                        \
            Sli$(TypeOf(*(__self)->items)),                           \
            .ptr = (__self)->items + (__begin),                       \
            .len = Arr_len(*(__self)) - (__begin)                     \
        )                                                             \
    );                                                                \
})

#define SYN__for_array(__arr, __i, var_arr, _Iter_item)                                        \
    scope_with(let __arr = &(var_arr)) for (usize __i = 0; (__i) < Arr_len(*(__arr)); ++(__i)) \
        scope_with(let _Iter_item = Arr_at(*(__arr), __i))

#define SYN__for_array_indexed(__arr, var_arr, _Iter_item, _Iter_index)                                                \
    scope_with(let __arr = &(var_arr)) for (usize _Iter_index = 0; (_Iter_index) < Arr_len(*(__arr)); ++(_Iter_index)) \
        scope_with(let _Iter_item = Arr_at(*(__arr), _Iter_index))

#define SYN__for_array_rev(__arr, __i, var_arr, _Iter_item)                              \
    scope_with(let __arr = &(var_arr)) for (usize __i = Arr_len(*(__arr)); (__i)-- > 0;) \
        scope_with(let _Iter_item = Arr_at(*(__arr), __i))

#define SYN__for_array_indexed_rev(__arr, var_arr, _Iter_item, _Iter_index)                              \
    scope_with(let __arr = &(var_arr)) for (usize _Iter_index = Arr_len(*(__arr)); (_Iter_index)-- > 0;) \
        scope_with(let _Iter_item = Arr_at(*(__arr), _Iter_index))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ARR_INCLUDED */
