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
    anyptr (*func)(anyptr args);
    anyptr context;
} DeferBlock;

typedef struct DeferScope {
    DeferBlock* current;
    bool        active;
} DeferScope;

#define block_defer                            \
    for (                                      \
        DeferScope _block_defer = { 0, true }; \
        _block_defer.active;                   \
        defer__run()                           \
    )

#define block_defer_return \
    defer__run();          \
    return

#define defer(FUNC, CTX) pp_func(                     \
    DeferBlock* _block   = mem_create(DeferBlock);    \
    _block->func         = (anyptr(*)(anyptr))(FUNC); \
    _block->context      = (CTX);                     \
    _block->prev         = _block_defer.current;      \
    _block_defer.current = _block;                    \
)

#define defer__run() pp_func(                             \
    DeferBlock* _curr = _block_defer.current;             \
    while (_curr) {                                       \
        if (_curr->func) { _curr->func(_curr->context); } \
        DeferBlock* _prev = _curr->prev;                  \
        mem_destroy(&_curr);                              \
        _curr = _prev;                                    \
    } _block_defer.active                                 \
    = false;                                              \
)

/*========== Extern Function Prototypes =====================================*/

extern void DeferScope__run(DeferScope* scope);

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEFER_INCLUDED  */
