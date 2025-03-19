/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-19 (date of creation)
 * @updated 2025-03-19 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix
 *
 * @brief
 * @details
 */

#ifndef PTR_INCLUDED
#define PTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

typedef anyptr_const Ptr_const;
typedef anyptr       Ptr;

#define Ptr_const$(T) comp_type__Ptr_const$(T)
#define Ptr$(T)       comp_type__Ptr$(T)
#define use_Ptr$(T)   comp_gen__use_Ptr$(T)
extern Ptr Ptr_constCast(Ptr_const);

/*========== Macros and Definitions =========================================*/

#define comp_type__Ptr_const$(T) const T*
#define comp_type__Ptr$(T)       T*
#define comp_gen__use_Ptr$(T)                       \
    typedef Ptr_const$(T) pp_join($, Ptr_const, T); \
    typedef Ptr$(T) pp_join($, Ptr, T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PTR_INCLUDED */
