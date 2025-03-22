/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    return.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-13 (date of creation)
 * @updated 2025-01-13 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef SCOPE_RETURN_INCLUDED
#define SCOPE_RETURN_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

#define scope_reserveReturn(T)      SYN__scope_reserveReturn(T)
#define scope_returnReserved        SYN__scope_returnReserved
#define scope_return(val_return...) SYN__scope_return(val_return)
#define return_(val_return...)      SYN__return_(val_return)

#define reserveReturn(T) SYN__reserveReturn(T)
#define returnReserved   SYN__returnReserved

#define getReservedReturn()              FUNC__getReservedReturn()
#define setReservedReturn(val_return...) FUNC__setReservedReturn(val_return)

#define ext_scope(T_ReservedReturn...) comp_syn__ext_scope(T_ReservedReturn)
#define ext_unscoped                   comp_syn__ext_unscoped

/*========== Implementations ================================================*/

#if SCOPE_RESERVE_RETURN_CONTAINS_DEFER

// clang-format off
#define comp_syn__ext_scope(T_ReservedReturn...)            \
    {                                                       \
        rawptr$(T_ReservedReturn) __reserved_return = null; \
        struct {                                            \
            i32  curr;                                      \
            bool returns;                                   \
        } __scope_defer = { .curr = 0, .returns = false };  \
        if (0) {                                            \
        __returned_scope:                                   \
            __scope_defer.returns = true;                   \
            goto __deferred;                                \
        }                                                   \
        __deferred:                                         \
        switch (__scope_defer.curr) {                       \
        default:                                            \
            break;                                          \
        case 0:                                             \
            __scope_defer.curr = -1;
#define comp_syn__ext_unscoped                      \
            goto __returned_scope;                  \
        }                                           \
        return (eval({ if (!isSameType(TypeOf(*__reserved_return), void)) { debug_assert_nonnull(__reserved_return); } }), __reserved_return[0]);\
    }
/* #define comp_syn__ext_scope(T_ReservedReturn...)            \
    {                                                       \
        rawptr$(T_ReservedReturn) __reserved_return = null; \
        struct {                                            \
            i32  curr;                                      \
            bool returns;                                   \
        } __scope_defer = { .curr = 0, .returns = false };  \
        if (0) {                                            \
        __returned_scope:                                   \
            goto __ext_unscoped;                            \
        }                                                   \
        __deferred:                                         \
        switch (__scope_defer.curr) {                       \
        default:                                            \
            break;                                          \
        case 0:                                             \
            __scope_defer.curr = -1;
#define comp_syn__ext_unscoped                      \
            goto __returned_scope;                  \
        }                                           \
        if (0) {                                    \
        __ext_unscoped:                             \
            __scope_defer.returns = true;           \
            goto __deferred;                        \
        }                                           \
        return ensureNonnull(__reserved_return)[0]; \
    } */
// clang-format on

#define SYN__scope_reserveReturn(T_ReservedReturn)      \
    rawptr$(T_ReservedReturn) __reserved_return = null; \
    struct {                                            \
        i32  curr;                                      \
        bool returns;                                   \
    } __scope_defer = { .curr = 0, .returns = false };  \
    if (0) {                                            \
    __returned_scope:                                   \
        goto __ext_unscoped;                            \
    }                                                   \
    __deferred:                                         \
    switch (__scope_defer.curr) {                       \
    default:                                            \
        break;                                          \
    case 0:                                             \
        __scope_defer.curr = -1;
#define SYN__scope_returnReserved     \
    goto __returned_scope;            \
    }                                 \
    if (0) {                          \
    __ext_unscoped:                   \
        __scope_defer.returns = true; \
        goto __deferred;              \
    }                                 \
    return (eval({ if (!isSameType(TypeOf(*__reserved_return), void)) { debug_assert_nonnull(__reserved_return); } }), __reserved_return[0])

#define SYN__scope_return(val_return...) \
    {                                    \
        setReservedReturn(val_return);   \
        goto __returned_scope;           \
    }

#define SYN__return_(val_return...) \
    scope_return(*(TypeOf(getReservedReturn()[0])[1]){ [0] = val_return })

#else
#endif /* SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define SYN__reserveReturn(T)                                                   \
    T* __reserved_return = null;                                                \
    if (0) {                                                                    \
    __returned_scope:                                                           \
        return (debug_assert_nonnull(__reserved_return), __reserved_return[0]); \
    }                                                                           \
    unused(0)

#define SYN__returnReserved \
    return (eval({ if (!isSameType(TypeOf(*__reserved_return), void)) { debug_assert_nonnull(__reserved_return); } }), __reserved_return[0])

#define FUNC__getReservedReturn() \
    (__reserved_return)

#define FUNC__setReservedReturn(val_return...) eval({ \
    getReservedReturn() = &val_return;                \
    eval_return getReservedReturn()[0];               \
})
/* #define FUNC__setReservedReturn(val_return...) eval({ \
    getReservedReturn()[0] = val_return;              \
    eval_return getReservedReturn()[0];               \
}) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SCOPE_RETURN_INCLUDED */
