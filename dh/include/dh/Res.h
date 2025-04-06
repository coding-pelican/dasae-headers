/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Res.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-20 (date of creation)
 * @updated 2025-03-20 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  Res
 *
 * @brief   Result type implementation
 * @details Provides a result type that can be used to return a value and an error.
 */

#ifndef RES_INCLUDED
#define RES_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

#define use_Res$(T_First, T_Second)  comp_type_gen__use_Res$(T_First, T_Second)
#define decl_Res$(T_First, T_Second) comp_type_gen__decl_Res$(T_First, T_Second)
#define impl_Res$(T_First, T_Second) comp_type_gen__impl_Res$(T_First, T_Second)

#define Res$(T_First, T_Second)  comp_type_alias__Res$(T_First, T_Second)
#define Res$$(T_First, T_Second) comp_type_anon__Res$$(T_First, T_Second)

#define Res_detFirst(val_first...)   comp_op__Res_detFirst(val_first)
#define Res_detSecond(val_second...) comp_op__Res_detSecond(val_second)

#define Res_detFirst$(T_Res, val_first...)   comp_op__Res_detFirst$(T_Res, val_first)
#define Res_detSecond$(T_Res, val_second...) comp_op__Res_detSecond$(T_Res, val_second)

#define Res_detFirstAsg(var_addr_res, val_first...)   comp_op__Res_detFirstAsg(pp_uniqTok(addr_res), var_addr_res, val_first)
#define Res_detSecondAsg(var_addr_res, val_second...) comp_op__Res_detSecondAsg(pp_uniqTok(addr_res), var_addr_res, val_second)

#define Res_isFirst(val_res...)  comp_op__Res_isFirst(val_res)
#define Res_isSecond(val_res...) comp_op__Res_isSecond(val_res)

#define Res_first(var_addr_res...)                 comp_op__Res_first(pp_uniqTok(addr_res), var_addr_res)
#define Res_second(var_addr_res...)                comp_op__Res_second(pp_uniqTok(addr_res), var_addr_res)
#define Res_getFirst(var_res...)                   comp_op__Res_getFirst(pp_uniqTok(res), var_res)
#define Res_getSecond(var_res...)                  comp_op__Res_getSecond(pp_uniqTok(res), var_res)
#define Res_setFirst(var_addr_res, val_first...)   comp_op__Res_setFirst(pp_uniqTok(addr_res), var_addr_res, val_first)
#define Res_setSecond(var_addr_res, val_second...) comp_op__Res_setSecond(pp_uniqTok(addr_res), var_addr_res, val_second)

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__use_Res$(T_First, T_Second) \
    decl_Res$(T_First, T_Second);                  \
    impl_Res$(T_First, T_Second)
#define comp_type_gen__decl_Res$(T_First, T_Second) \
    typedef struct Res$(T_First, T_Second) Res$(T_First, T_Second)
#define comp_type_gen__impl_Res$(T_First, T_Second) \
    struct Res$(T_First, T_Second) {                \
        union {                                     \
            T_First  first;                         \
            T_Second second;                        \
        } data[1];                                  \
        bool is_first;                              \
    }

#define comp_type_alias__Res$(T_First, T_Second) \
    pp_cat3(Res, pp_cat3($, 1, T_First), pp_cat3($, 2, T_Second))
#define comp_type_anon__Res$$(T_First, T_Second) \
    struct {                                     \
        union {                                  \
            T_First  first;                      \
            T_Second second;                     \
        } data[1];                               \
        bool is_first;                           \
    }

#define comp_op__Res_detFirst(val_first...)   { .is_first = true, .data[0].first = val_first }
#define comp_op__Res_detSecond(val_second...) { .is_first = false, .data[0].second = val_second }

#define comp_op__Res_detFirst$(T_Res, val_first...)   ((T_Res)Res_detFirst(val_first))
#define comp_op__Res_detSecond$(T_Res, val_second...) ((T_Res)Res_detSecond(val_second))

#define comp_op__Res_detFirstAsg(__addr_res, var_addr_res, val_first...) eval({ \
    const TypeOf(var_addr_res) __addr_res = var_addr_res;                       \
    debug_assert_nonnull(__addr_res);                                           \
    *__addr_res = Res_detFirst$(TypeOf(*var_addr_res), val_first);              \
    eval_return __addr_res;                                                     \
})
#define comp_op__Res_detSecondAsg(__addr_res, var_addr_res, val_second...) eval({ \
    const TypeOf(var_addr_res) __addr_res = var_addr_res;                         \
    debug_assert_nonnull(__addr_res);                                             \
    *__addr_res = Res_detSecond$(TypeOf(*var_addr_res), val_second);              \
    eval_return __addr_res;                                                       \
})

#define comp_op__Res_isFirst(val_res...)  ((val_res).is_first == true)
#define comp_op__Res_isSecond(val_res...) ((val_res).is_first == false)

#define comp_op__Res_first(__addr_res, var_addr_res...) eval({ \
    const TypeOf(var_addr_res) __addr_res = var_addr_res;      \
    debug_assert_true(Res_isFirst(*__addr_res));               \
    eval_return & __addr_res->data[0].first;                   \
})
#define comp_op__Res_second(__addr_res, var_addr_res...) eval({ \
    const TypeOf(var_addr_res) __addr_res = var_addr_res;       \
    debug_assert_true(Res_isSecond(*__addr_res));               \
    eval_return & __addr_res->data[0].second;                   \
})

#define comp_op__Res_getFirst(__res, val_res...) eval({ \
    const TypeOf(val_res) __res = val_res;              \
    debug_assert_true(Res_isFirst(__res));              \
    eval_return __res.data[0].first;                    \
})
#define comp_op__Res_getSecond(__res, val_res...) eval({ \
    const TypeOf(val_res) __res = val_res;               \
    debug_assert_true(Res_isSecond(__res));              \
    eval_return __res.data[0].second;                    \
})

#define comp_op__Res_setFirst(__addr_res, var_addr_res, val_first...) eval({ \
    const TypeOf(var_addr_res) __addr_res = var_addr_res;                    \
    debug_assert_true(Res_isFirst(*__addr_res));                             \
    __addr_res->data[0].first = val_first;                                   \
    eval_return __addr_res;                                                  \
})
#define comp_op__Res_setSecond(__addr_res, var_addr_res, val_second...) eval({ \
    const TypeOf(var_addr_res) __addr_res = var_addr_res;                      \
    debug_assert_true(Res_isSecond(*__addr_res));                              \
    __addr_res->data[0].second = val_second;                                   \
    eval_return __addr_res;                                                    \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* RES_INCLUDED  */
