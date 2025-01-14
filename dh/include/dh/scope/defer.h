/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    defer.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-15 (date of creation)
 * @updated 2025-01-13 (date of last update)
 * @version v0.1-alpha.1
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

#define defer(_Statement...) SYN_defer(_Statement)

#define block_defer    SYN_block_defer
#define block_deferral SYN_block_deferral
#define defer_break    SYN_defer_break

#define scope_defer    SYN_scope_defer
#define scope_deferral SYN_scope_deferral
#define defer_return(val_return...) SYN_defer_return(val_return)

#else

#define defer(_Statement...) SYN_defer(_Statement)

#define block_defer    SYN_block_defer
#define block_deferral SYN_block_deferral
#define defer_break    SYN_break_defer

#endif /* !SCOPE_RESERVE_RETURN_CONTAINS_DEFER */



/*========== Macros Implementation ==========================================*/

// NOLINTBEGIN(bugprone-terminating-continue)
#define defer__snapshot(_Statement...) FUNC_defer__snapshot(_Statement)

#define SYN_defer(_Statement...) \
    defer__snapshot(_Statement; goto _deferred)

#define SYN_block_defer             \
    do {                            \
    defer__snapshot(                \
        if (_scope_defer.returns) { \
            goto _deferred;         \
        } else {                    \
            continue;               \
        }                           \
    )

#define SYN_block_deferral \
    goto _deferred;        \
    }                      \
    while (false)

#define SYN_defer_break \
    {                   \
        goto _deferred; \
    }

#define SYN_scope_defer                     \
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

#define SYN_scope_deferral \
    goto _deferred;        \
    }                      \
    unused(0)

#define SYN_defer_return(val_return...) \
    {                                   \
        setReservedReturn(val_return);  \
        _scope_defer.returns = true;    \
        goto _deferred;                 \
    }

#define FUNC_defer__snapshot(_Statement...)        \
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
