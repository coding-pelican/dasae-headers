/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    defer.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated 2024-11-05 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  defer
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */
// TODO(dev-dasae): Implement to be generic for function (like lambda)


#ifndef DEFER_INCLUDED
#define DEFER_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "core.h"
#include "mem.h"

/*========== Macros and Definitions =========================================*/

/* More sophisticated version combining best aspects */
typedef struct DeferBlock {
    struct DeferBlock* prev;
    void (*func)(void* args);
    void* context;
} DeferBlock;

typedef struct DeferScope {
    DeferBlock* current;
    bool        active;
} DeferScope;

#define defer_scope                             \
    for (                                       \
        DeferScope __defer_scope = { 0, true }; \
        __defer_scope.active;                   \
        defer__run()                            \
    )

#define defer_scope_return \
    defer__run();          \
    return

#define defer(_func, _ctx) pp_func(                 \
    DeferBlock* __block   = mem_create(DeferBlock); \
    __block->func         = _func;                  \
    __block->context      = _ctx;                   \
    __block->prev         = __defer_scope.current;  \
    __defer_scope.current = __block;                \
)

#define defer__run() pp_func(                                \
    DeferBlock* __curr = __defer_scope.current;              \
    while (__curr) {                                         \
        if (__curr->func) { __curr->func(__curr->context); } \
        DeferBlock* __prev = __curr->prev;                   \
        mem_destroy(&__curr);                                \
        __curr = __prev;                                     \
    } __defer_scope.active                                   \
    = false;                                                 \
)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEFER_INCLUDED  */
