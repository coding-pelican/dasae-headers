/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    Arr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-17 (date of creation)
 * @updated 2025-03-13 (date of last update)
 * @version v0.1-alpha.4
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 */

#ifndef ARR_INCLUDED
#define ARR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Definitions =========================================*/

#define Arr_const$(N, T...)              comp_type_anon__Arr_const$(N, T)
#define Arr$(N, T...)                    comp_type_anon__Arr$(N, T)
#define Arr_asNamed$(T_Arr, var_anon...) comp_op__Arr_asNamed$(pp_uniqTok(anon), T_Arr, var_anon)

#define use_Arr$(N, T...)  comp_gen__use_Arr$(N, T)
#define decl_Arr$(N, T...) comp_gen__decl_Arr$(N, T)
#define impl_Arr$(N, T...) comp_gen__impl_Arr$(N, T)

#define Arr_init()                    comp_op__Arr_init()
#define Arr_init$(T_Arr)              comp_op__Arr_init$(T_Arr)
#define Arr_from(_Initial...)         comp_op__Arr_from(_Initial)
#define Arr_from$(T_Arr, _Initial...) comp_op__Arr_from$(T_Arr, _Initial)
#define Arr_make$(T, _Initial...)     comp_op__Arr_make$(T, _Initial)
#define Arr_asg()

#define Arr_len(var_self...)                          comp_op__Arr_len(pp_uniqTok(self), var_self)
#define Arr_at(var_self, usize_index...)              comp_op__Arr_at(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Arr_getAt(var_self, usize_index...)           comp_op__Arr_getAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index)
#define Arr_setAt(var_self, usize_index, val_item...) comp_op__Arr_setAt(pp_uniqTok(self), pp_uniqTok(index), var_self, usize_index, val_item)

// /* Slice */
// #define Arr_slice(var_self, usize_begin, usize_end...) comp_op__Arr_slice(pp_uniqTok(self), pp_uniqTok(begin), pp_uniqTok(end), var_self, usize_begin, usize_end)
// #define Arr_prefix(var_self, usize_end...)             comp_op__Arr_prefix(pp_uniqTok(self), pp_uniqTok(end), var_self, usize_end)
// #define Arr_suffix(var_self, usize_begin...)           comp_op__Arr_suffix(pp_uniqTok(self), pp_uniqTok(begin), var_self, usize_begin)

#define for_array(var_arr, _Iter_item)                      comp_syn__for_array(pp_uniqTok(arr), pp_uniqTok(i), var_arr, _Iter_item)
#define for_array_indexed(var_arr, _Iter_item, _Iter_index) comp_syn__for_array_indexed(pp_uniqTok(arr), var_arr, _Iter_item, _Iter_index)

#define for_array_rev(var_arr, _Iter_item)                      comp_syn__for_array_rev(pp_uniqTok(arr), pp_uniqTok(i), var_arr, _Iter_item)
#define for_array_rev_indexed(var_arr, _Iter_item, _Iter_index) comp_syn__for_array_rev_indexed(pp_uniqTok(arr), var_arr, _Iter_item, _Iter_index)

/*========== Implementations ================================================*/

#define comp_type_anon__Arr_const$(N, T...) \
    struct {                                \
        const T items[N];                   \
    }
#define comp_type_anon__Arr$(N, T...) \
    struct {                          \
        T items[N];                   \
    }
#define comp_op__Arr_asNamed$(__anon, T_Arr, var_anon...) eval({                             \
    let __anon = &var_anon;                                                                  \
    claim_assert_static(sizeOf(TypeOf(*__anon)) == sizeOf(T_Arr));                           \
    claim_assert_static(alignOf(TypeOf(*__anon)) == alignOf(T_Arr));                         \
    claim_assert_static(validateField(TypeOf(*__anon), items, FieldTypeOf(T_Arr, items)));   \
    claim_assert_static(fieldPadding(TypeOf(*__anon), items) == fieldPadding(T_Arr, items)); \
    eval_return rawderef(as$(rawptr$(T_Arr), __anon));                                       \
})

#define comp_gen__use_Arr$(N, T...) \
    decl_Arr$(N, T);                \
    impl_Arr$(N, T)
#define comp_gen__decl_Arr$(N, T...)                                          \
    typedef struct pp_join3($, Arr_const, N, T) pp_join3($, Arr_const, N, T); \
    typedef union pp_join3($, Arr, N, T) pp_join3($, Arr, N, T)
#define comp_gen__impl_Arr$(N, T...)           \
    struct pp_join3($, Arr_const, N, T) {      \
        const T items[N];                      \
    };                                         \
    union pp_join3($, Arr, N, T) {             \
        struct {                               \
            T items[N];                        \
        };                                     \
        pp_join3($, Arr_const, N, T) as_const; \
    }

#define comp_op__Arr_init()                    { .items = { 0 } }
#define comp_op__Arr_init$(T_Arr)              ((T_Arr)Arr_init())
#define comp_op__Arr_from(_Initial...)         { .items = _Initial }
#define comp_op__Arr_from$(T_Arr, _Initial...) ((T_Arr)Arr_from(_Initial))
#define comp_op__Arr_make$(T, _Initial...)     ((Arr$(countOf((T[])_Initial), T)){ .items = _Initial })

#define comp_op__Arr_len(__self, var_self...) eval({ \
    let __self = &var_self;                          \
    eval_return countOf(__self->items);              \
})
#define comp_op__Arr_at(__self, __index, var_self, usize_index...) eval({ \
    let         __self  = &var_self;                                      \
    const usize __index = usize_index;                                    \
    debug_assert_fmt(                                                     \
        __index < Arr_len(*__self),                                       \
        "Index out of bounds: %zu >= %zu",                                \
        __index,                                                          \
        Arr_len(*__self)                                                  \
    );                                                                    \
    eval_return rawref(__self->items[__index]);                           \
})
#define comp_op__Arr_getAt(__self, __index, var_self, usize_index...) eval({ \
    let         __self  = &var_self;                                         \
    const usize __index = usize_index;                                       \
    debug_assert_fmt(                                                        \
        __index < Arr_len(*__self),                                          \
        "Index out of bounds: %zu >= %zu",                                   \
        __index,                                                             \
        Arr_len(*__self)                                                     \
    );                                                                       \
    eval_return __self->items[__index];                                      \
})
#define comp_op__Arr_setAt(__self, __index, var_self, usize_index, var_value...) eval({ \
    let         __self  = &var_self;                                                    \
    const usize __index = usize_index;                                                  \
    debug_assert_fmt(                                                                   \
        __index < Arr_len(*__self),                                                     \
        "Index out of bounds: %zu >= %zu",                                              \
        __index,                                                                        \
        Arr_len(*__self)                                                                \
    );                                                                                  \
    __self->items[__index] = as$(TypeOf(__self->items[0]), var_value);                  \
    eval_return __self;                                                                 \
})

// #define comp_op__Arr_slice(__self, __begin, __end, var_self, usize_begin, usize_end...) eval({ \
//     let         __self  = &(var_self);                                                         \
//     const usize __begin = usize_begin;                                                         \
//     const usize __end   = usize_end;                                                           \
//     debug_assert((__begin) < (__end));                                                         \
//     debug_assert((__end) <= Arr_len(*(__self)));                                               \
//     eval_return(                                                                               \
//         make$(                                                                                 \
//             Sli$(TypeOf(*(__self)->items)),                                                    \
//             .ptr = (__self)->items + (__begin),                                                \
//             .len = (__end) - (__begin)                                                         \
//         )                                                                                      \
//     );                                                                                         \
// })
// #define comp_op__Arr_prefix(__self, __end, var_self, usize_end) eval({ \
//     let         __self = &(var_self);                                  \
//     const usize __end  = usize_end;                                    \
//     debug_assert((__end) <= Arr_len(*(__self)));                       \
//     eval_return(                                                       \
//         make$(                                                         \
//             Sli$(TypeOf(*(__self)->items)),                            \
//             .ptr = (__self)->items,                                    \
//             .len = (__end)                                             \
//         )                                                              \
//     );                                                                 \
// })
// #define comp_op__Arr_suffix(__self, __begin, var_self, usize_begin) eval({ \
//     let         __self  = &(var_self);                                     \
//     const usize __begin = usize_begin;                                     \
//     debug_assert((__begin) < Arr_len(*(__self)));                          \
//     eval_return(                                                           \
//         make$(                                                             \
//             Sli$(TypeOf(*(__self)->items)),                                \
//             .ptr = (__self)->items + (__begin),                            \
//             .len = Arr_len(*(__self)) - (__begin)                          \
//         )                                                                  \
//     );                                                                     \
// })

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
