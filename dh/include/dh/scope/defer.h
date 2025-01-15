/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    defer.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-15 (date of creation)
 * @updated 2025-01-15 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/scope
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef SCOPE_DEFER_INCLUDED
#define SCOPE_DEFER_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "return.h"

/*========== Macros and Definitions =========================================*/

#if !SCOPE_RESERVE_RETURN_CONTAINS_DEFER

#define defer(_Statement...) SYN__defer(_Statement)

#define block_defer    SYN__block_defer
#define block_deferral SYN__block_deferral
#define defer_break    SYN__defer_break

#define scope_defer                 SYN__scope_defer
#define scope_deferral              SYN__scope_deferral
#define defer_return(val_return...) SYN__defer_return(val_return)

#else

#define defer(_Statement...) SYN__defer(_Statement)

#define block_defer    SYN__block_defer
#define block_deferral SYN__block_deferral
#define defer_break    SYN__break_defer

#endif /* !SCOPE_RESERVE_RETURN_CONTAINS_DEFER */

/*========== Macros Implementation ==========================================*/

// NOLINTBEGIN(bugprone-terminating-continue)
#define defer__snapshot(_Statement...) OP__defer__snapshot(_Statement)

#define SYN__defer(_Statement...) \
    defer__snapshot(_Statement; goto _deferred)

#define SYN__block_defer            \
    do {                            \
    defer__snapshot(                \
        if (_scope_defer.returns) { \
            goto _deferred;         \
        } else {                    \
            continue;               \
        }                           \
    )

#define SYN__block_deferral \
    goto _deferred;         \
    }                       \
    while (false)

#define SYN__defer_break \
    {                    \
        goto _deferred;  \
    }

#define SYN__scope_defer                    \
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

#define SYN__scope_deferral \
    goto _deferred;         \
    }                       \
    unused(0)

#define SYN__defer_return(val_return...) \
    {                                    \
        setReservedReturn(val_return);   \
        _scope_defer.returns = true;     \
        goto _deferred;                  \
    }

#define OP__defer__snapshot(_Statement...)         \
    {                                              \
        i32 _defer_scope_prev = _scope_defer.curr; \
        _scope_defer.curr     = __LINE__;          \
        if (false) {                               \
        case __LINE__:                             \
            _scope_defer.curr = _defer_scope_prev; \
            _Statement;                            \
        }                                          \
    }
// NOLINTEND(bugprone-terminating-continue)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SCOPE_DEFER_INCLUDED  */
