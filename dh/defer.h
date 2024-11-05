/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    defer.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated 2024-11-05 (date of last update)
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

#include "preprocessor.h"

#include "container.h"
#include "mem.h"
#include "types.h"

/*========== Macros and Definitions =========================================*/

/* More sophisticated version combining best aspects */
typedef struct DeferBlock {
    struct DeferBlock* prev;
    void (*func)(void*);
    void* context;
} DeferBlock;

typedef struct DeferScope {
    DeferBlock* current;
    bool        active;
} DeferScope;

#define defer_scope_begin             \
    DeferScope __defer_scope = { 0 }; \
    __defer_scope.active     = true;  \
    do

#define defer_scope_end                                      \
    while (0);                                               \
    if (__defer_scope.active) {                              \
        DeferBlock* __curr = __defer_scope.current;          \
        while (__curr) {                                     \
            if (__curr->func) __curr->func(__curr->context); \
            DeferBlock* __prev = __curr->prev;               \
            mem_destroy(__curr);                             \
            __curr = __prev;                                 \
        }                                                    \
    }

#define defer(_func, _ctx) pp_func(                 \
    DeferBlock* __block   = mem_create(DeferBlock); \
    __block->func         = _func;                  \
    __block->context      = _ctx;                   \
    __block->prev         = __defer_scope.current;  \
    __defer_scope.current = __block;                \
)

#define defer_scope_return            \
    pp_func(                          \
        __defer_scope.active = false; \
    );                                \
    return

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST

void cleanup_vec(void* vec) {
    f32Vector_fini((f32Vector*)vec);
}

/* Example usage of sophisticated version */
void sophisticated_example(void) {
    defer_scope_begin {
        f32Vector vec = make(f32Vector);
        f32Vector_init(&vec, 4);

        defer(cleanup_vec, &vec);

        // Use vector...

        // Optional early return:
        bool some_condition = true;
        if (some_condition) {
            defer_scope_return;
        }

        // More operations...
    }
    defer_scope_end;
}

#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEFER_INCLUDED  */
