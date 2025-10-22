/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
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

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/fn.h"
#include "dh/opt.h"

/*========== Macros and Declarations ========================================*/

typedef anyptr_const Ptr_const;
typedef anyptr       Ptr;

#define use_Ptr$(T) comp_type_gen__use_Ptr$(T)

#define Ptr_const$(T)  comp_type_alias__Ptr_const$(T)
#define Ptr$(T)        comp_type_alias__Ptr$(T)
#define Ptr_const$$(T) comp_type_raw__Ptr_const$$(T)
#define Ptr$$(T)       comp_type_raw__Ptr$$(T)

extern fn_((Ptr_constCastable(anyptr_const ptr))(bool));
#define Ptr_constCast$(T_Ptr, var_ptr... /* Opt$T_Ptr */)       comp_op__Ptr_constCast$(pp_uniqTok(ptr), pp_uniqTok(ret), T_Ptr, var_ptr)
#define Ptr_constCast(var_ptr... /* Opt$Ptr$T */)               comp_op__Ptr_constCast(pp_uniqTok(ptr), pp_uniqTok(ret), var_ptr)
#define Ptr_mutCast$(T_Ptr_const, var_ptr... /* T_Ptr_const */) comp_op__Ptr_mutCast$(T_Ptr_const, var_ptr)
#define Ptr_mutCast(var_ptr... /* Ptr_const$T */)               comp_op__Ptr_mutCast(var_ptr)

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__use_Ptr$(T) \
    $maybe_unused typedef rawptr_const$(T) Ptr_const$(T); \
    $maybe_unused typedef rawptr$(T) Ptr$(T)

#define comp_type_alias__Ptr_const$(T) \
    pp_join($, Ptr_const, T)
#define comp_type_alias__Ptr$(T) \
    pp_join($, Ptr, T)
#define comp_type_raw__Ptr_const$$(T) \
    const T*
#define comp_type_raw__Ptr$$(T) \
    T*

#define comp_op__Ptr_constCast$(__ptr, __ret, T_Ptr, var_ptr...) eval({ \
    const TypeOf(var_ptr) __ptr = var_ptr; \
    Opt$(T_Ptr) __ret           = cleared(); \
    if (Ptr_constCastable(__ptr)) { \
        Opt_asg(&__ret, some(as$((T_Ptr)(__ptr)))); \
    } else { \
        Opt_asg(&__ret, none()); \
    } \
    eval_return __ret; \
})
#define comp_op__Ptr_constCast(__ptr, __ret, var_ptr...) eval({ \
    const TypeOf(var_ptr) __ptr        = var_ptr; \
    Opt$$(TypeOfUnqual(*__ptr)*) __ret = cleared(); \
    if (Ptr_constCastable(__ptr)) { \
        Opt_asg(&__ret, some(as$((TypeOfUnqual(*__ptr)*)(__ptr)))); \
    } else { \
        Opt_asg(&__ret, none()); \
    } \
    eval_return __ret; \
})


#define comp_op__Ptr_mutCast$(T_Ptr_const, var_ptr...) as$((T_Ptr_const)(var_ptr))
#define comp_op__Ptr_mutCast(var_ptr...)               Ptr_mutCast$(const TypeOf(*var_ptr)*, var_ptr)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PTR_INCLUDED */
