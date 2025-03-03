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

/*========== Implementations ================================================*/

#if SCOPE_RESERVE_RETURN_CONTAINS_DEFER

#define SYN__scope_reserveReturn(T)                   \
    rawptr$(T) _reserved_return = null;               \
    struct {                                          \
        i32  curr;                                    \
        bool returns;                                 \
    } _scope_defer = { .curr = 0, .returns = false }; \
    if (0) {                                          \
    _returned_scope:                                  \
        _scope_defer.returns = true;                  \
        goto _deferred;                               \
    }                                                 \
    _deferred:                                        \
    switch (_scope_defer.curr) {                      \
    default:                                          \
        break;                                        \
    case 0:                                           \
        _scope_defer.curr = -1;

#define SYN__scope_returnReserved \
    goto _returned_scope;         \
    }                             \
    return (debug_assert_nonnull(_reserved_return), _reserved_return[0])

#define SYN__scope_return(val_return...) \
    {                                    \
        setReservedReturn(val_return);   \
        goto _returned_scope;            \
    }

#define SYN__return_(val_return...) \
    scope_return(*(TypeOf(getReservedReturn()[0])[1]){ [0] = val_return })

#else
#endif /* SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

#define SYN__reserveReturn(T)                                                 \
    T* _reserved_return = null;                                               \
    if (0) {                                                                  \
    _returned_scope:                                                          \
        return (debug_assert_nonnull(_reserved_return), _reserved_return[0]); \
    }                                                                         \
    unused(0)

#define SYN__returnReserved \
    return (debug_assert_nonnull(_reserved_return), _reserved_return[0])

#define FUNC__getReservedReturn() \
    (_reserved_return)

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
