/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Pair.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-20 (date of creation)
 * @updated 2025-03-20 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  Pair
 *
 * @brief   Pair type implementation
 * @details Provides a pair type that can be used to represent a pair of values.
 */

#ifndef PAIR_INCLUDED
#define PAIR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

#define use_Pair$(T_First, T_Second)  comp_type_gen__Pair$(T_First, T_Second)
#define decl_Pair$(T_First, T_Second) comp_type_decl__Pair$(T_First, T_Second)
#define impl_Pair$(T_First, T_Second) comp_type_impl__Pair$(T_First, T_Second)

#define Pair$(T_First, T_Second)  comp_type_alias__Pair$(T_First, T_Second)
#define Pair$$(T_First, T_Second) comp_type_anon__Pair$$(T_First, T_Second)

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__Pair$(T_First, T_Second) \
    decl_Pair$(T_First, T_Second);              \
    impl_Pair$(T_First, T_Second)
#define comp_type_decl__Pair$(T_First, T_Second) \
    $maybe_unused typedef struct Pair$(T_First, T_Second) Pair$(T_First, T_Second)
#define comp_type_impl__Pair$(T_First, T_Second) \
    struct Pair$(T_First, T_Second) {            \
        T_First  first;                          \
        T_Second second;                         \
    }

#define comp_type_alias__Pair$(T_First, T_Second) \
    pp_cat3(Pair, pp_cat3($, 1, T_First), pp_cat3($, 2, T_Second))
#define comp_type_anon__Pair$$(T_First, T_Second) \
    struct {                                      \
        T_First  first;                           \
        T_Second second;                          \
    }

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PAIR_INCLUDED  */
