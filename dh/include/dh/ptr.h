/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-19 (date of creation)
 * @updated 2025-03-19 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  Ptr
 *
 * @brief   Pointer types and operations
 * @details This header provides types and macros for pointer operations.
 *          Supports both constant and mutable pointers, with separate type
 *          declarations and implementations.
 */

#ifndef PTR_INCLUDED
#define PTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/types/Ptr.h"

#if UNUSED_CODE
/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/fn.h"
#include "dh/opt.h"

/*========== Macros and Declarations ========================================*/

typedef P_const$raw P_const;
typedef P$raw       Ptr;

#define use_P$(T) comp_type_gen__use_P$(T)

#define P_const$(T)  comp_type_alias__P_const$(T)
#define P$(T)        comp_type_alias__P$(T)
#define P_const$$(T) comp_type_raw__P_const$$(T)
#define P$$(T)       comp_type_raw__P$$(T)

extern fn_((constCastable$P(P_const$raw ptr))(bool));
#define constCast$P(T_Ptr, var_ptr... /* O$T_Ptr */)      comp_op__constCast$P(pp_uniqTok(ptr), pp_uniqTok(ret), T_Ptr, var_ptr)
#define P_constCast(var_ptr... /* O$P$T */)               comp_op__P_constCast(pp_uniqTok(ptr), pp_uniqTok(ret), var_ptr)
#define mutCast$P$(T_P_const, var_ptr... /* T_P_const */) comp_op__mutCast$P$(T_P_const, var_ptr)
#define mutCast$P(var_ptr... /* P_const$T */)             comp_op__mutCast$P(var_ptr)

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__use_P$(T) \
    $maybe_unused typedef $P_const$(T) P_const$(T); \
    $maybe_unused typedef $P$(T) P$(T)

#define comp_type_alias__P_const$(T) \
    pp_join($, P_const, T)
#define comp_type_alias__P$(T) \
    pp_join($, P, T)
#define comp_type_raw__P_const$$(T) \
    TypeOf(const T*)
#define comp_type_raw__P$$(T) \
    TypeOf(T*)

#define comp_op__constCast$P(__ptr, __ret, T_Ptr, var_ptr...) blk({ \
    const TypeOf(var_ptr) __ptr = var_ptr; \
    O$(T_Ptr) \
    __ret = cleared(); \
    if (constCastable$P(__ptr)) { \
        O_asg(&__ret, some(as$((T_Ptr)(__ptr)))); \
    } else { \
        O_asg(&__ret, none()); \
    } \
    blk_return __ret; \
})
#define comp_op__P_constCast(__ptr, __ret, var_ptr...) blk({ \
    const TypeOf(var_ptr) __ptr = var_ptr; \
    O$$(TypeOfUnqual(*__ptr)*) \
    __ret = cleared(); \
    if (constCastable$P(__ptr)) { \
        O_asg(&__ret, some(as$((TypeOfUnqual(*__ptr)*)(__ptr)))); \
    } else { \
        O_asg(&__ret, none()); \
    } \
    blk_return __ret; \
})

#define comp_op__mutCast$P$(T_P_const, var_ptr...) as$((T_P_const)(var_ptr))
#define comp_op__mutCast$P(var_ptr...)             mutCast$P$(const TypeOf(*var_ptr)*, var_ptr)
#endif /* UNUSED_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PTR_INCLUDED */
