/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    defer.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-15 (date of creation)
 * @updated 2024-11-15 (date of last update)
 * @version v1.0.0
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

#define defer_block IMPL_defer_block
#define defer_scope IMPL_defer_scope

#define defer_break  IMPL_defer_break
#define defer_return IMPL_defer_return

#define scope_deferred() IMPL_scope_deferred()
#define block_deferred() IMPL_block_deferred()

#define defer(STMT...) IMPL_defer(STMT)

#define defer_block__snapshot(STMT...) IMPL_defer_block__snapshot(STMT)

/*========== Macros Implementation ==========================================*/

// NOLINTBEGIN(bugprone-terminating-continue)
#define IMPL_defer_block   \
    i32 _defer_return = 0; \
    unused(_defer_return); \
    i32 _defer_curr = 0;   \
    _deferred:             \
    switch (_defer_curr) { \
    default:               \
        break;             \
    case 0:                \
        _defer_curr = -1;

#define IMPL_defer_scope     \
    do {                     \
    defer_block__snapshot(   \
        if (_defer_return) { \
            goto _deferred;  \
        } else {             \
            continue;        \
        }                    \
    )

#define IMPL_defer_break \
    {                    \
        goto _deferred;  \
    }

#define IMPL_defer_return  \
    {                      \
        _defer_return = 1; \
        goto _deferred;    \
    }

#define IMPL_scope_deferred() \
    goto _deferred;           \
    }                         \
    while (false)

#define IMPL_block_deferred() \
    goto _deferred;           \
    }                         \
    while (false)

#define IMPL_defer(STMT...) \
    defer_block__snapshot(STMT; goto _deferred)

#define IMPL_defer_block__snapshot(STMT...) \
    {                                       \
        i32 _defer_prev = _defer_curr;      \
        _defer_curr     = __LINE__;         \
        if (false) {                        \
        case __LINE__:                      \
            _defer_curr = _defer_prev;      \
            STMT;                           \
        }                                   \
    }
// NOLINTEND(bugprone-terminating-continue)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEFER_INCLUDED  */
