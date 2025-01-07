/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    defer.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-15 (date of creation)
 * @updated 2024-11-29 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef DEFER_INCLUDED
#define DEFER_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"

/*========== Macros and Definitions =========================================*/

#define scope_defer IMPL_scope_defer
#define block_defer IMPL_block_defer

#define defer_break                 IMPL_defer_break
#define defer_return(val_return...) IMPL_defer_return(val_return)

#define block_deferred  IMPL_block_deferred
#define scope_deferred  IMPL_scope_deferred
#define return_deferred IMPL_return_deferred

#define defer(_Statement...) IMPL_defer(_Statement)

#define scope_defer__snapshot(_Statement...) IMPL_scope_defer__snapshot(_Statement)

// TODO: Implement errdefer

/*========== Macros Implementation ==========================================*/

// NOLINTBEGIN(bugprone-terminating-continue)
#define IMPL_scope_defer                    \
    struct {                                \
        i32  curr;                          \
        bool returns;                       \
    } _scope_defer = {                      \
        .curr    = 0,                       \
        .returns = false                    \
    };                                      \
    _deferred:                              \
    switch (_scope_defer.curr) {            \
    default:                                \
        /* fallthrough if no valid label */ \
        break;                              \
    case 0:                                 \
        _scope_defer.curr = -1;

#define IMPL_block_defer            \
    do {                            \
    scope_defer__snapshot(          \
        if (_scope_defer.returns) { \
            goto _deferred;         \
        } else {                    \
            continue;               \
        }                           \
    )

#define IMPL_defer_break \
    {                    \
        goto _deferred;  \
    }

#define IMPL_defer_return(val_return...) \
    {                                    \
        setReservedReturn(val_return);   \
        _scope_defer.returns = true;     \
        goto _deferred;                  \
    }

#define IMPL_block_deferred \
    goto _deferred;         \
    }                       \
    while (false)

#define IMPL_scope_deferred \
    goto _deferred;         \
    }                       \
    unused(0)

#define IMPL_return_deferred                       \
    scope_deferred;                                \
    return eval(                                   \
        debug_assert_nonnull(getReservedReturn()); \
        eval_return getReservedReturn()[0];        \
    )

#define IMPL_defer(_Statement...) \
    scope_defer__snapshot(_Statement; goto _deferred)

#define IMPL_scope_defer__snapshot(_Statement...)  \
    {                                              \
        i32 _scope_defer_prev = _scope_defer.curr; \
        _scope_defer.curr     = __LINE__;          \
        if (false) {                               \
        case __LINE__:                             \
            _scope_defer.curr = _scope_defer_prev; \
            _Statement;                            \
        }                                          \
    }
// NOLINTEND(bugprone-terminating-continue)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEFER_INCLUDED  */
