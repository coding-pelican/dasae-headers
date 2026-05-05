#include <stdio.h>
#include "dh/core.h"

/* Example: fixed maximum 32 defers in one scope_defer block. */
#ifndef MAX_DEFER_PER_SCOPE
#define MAX_DEFER_PER_SCOPE 32
#endif /* MAX_DEFER_PER_SCOPE */

#define scope_defer                                               \
    /*                                                            \
     * Start of a scope_defer block:                              \
     * - Create an array for labels                               \
     * - Create a 'top' index                                     \
     * - Jump to the user code                                    \
     * - Then define the 'cleanup' label which unwinds in reverse \
     */                                                           \
    IMPL__scope_defer
#define IMPL__scope_defer                            \
    /* store 1 label per 'defer(...)' call. */       \
    static void* _defer_labels[MAX_DEFER_PER_SCOPE]; \
    static int   _defer_count = 0;                   \
    goto _scope_body;                                \
    /* Cleanup: run them in reverse order: */        \
    _scope_cleanup:                                  \
    while (0 < _defer_count) {                       \
        _defer_count--;                              \
        if (_defer_labels[_defer_count]) {           \
            goto* _defer_labels[_defer_count];       \
        } else {                                     \
            continue;                                \
        }                                            \
    }                                                \
    goto _scope_end;                                 \
    _scope_body:

#define scope__deferred                  \
    /* End of scope: jump to cleanup. */ \
    goto _scope_cleanup;                 \
    _scope_end:                          \
    (void)0

#define defer(stmt...)                       \
    /* The 'defer(...)' macro:               \
     * - Create a unique label for the code. \
     * - Store its address in the array.     \
     * - Then skip over it in normal flow.   \
     */                                      \
    IMPL_defer0(__COUNTER__, stmt)
#define IMPL_defer0(_counter, stmt...) IMPL_defer1(_counter, stmt)
#define IMPL_defer1(_counter, stmt...) IMPL_defer2(_counter, pp_cat(_defer_run_, _counter), pp_cat(_defer_skip_, _counter), stmt)
#define IMPL_defer2(_counter, _defer_run, _defer_skip, stmt...) \
    do {                                                        \
        int local_id = _defer_count++;                          \
        if (local_id < MAX_DEFER_PER_SCOPE) {                   \
            _defer_labels[local_id] = &&_defer_run;             \
            goto _defer_skip;                                   \
        _defer_run:                                             \
            stmt;                                               \
            goto _scope_cleanup;                                \
        _defer_skip:;                                           \
        } else {                                                \
            /* error: too many defers in this scope */          \
        }                                                       \
    } while (0)

//---------------------------------
// Demo usage:

int main(void) {
    printf("Before scope\n");

    scope_defer {
        printf("Enter scope\n");

        defer(printf("Cleanup for scope after enter\n"));
        defer(printf("Cleanup for scope before switch\n"));

        switch (2) {
        case 1: {
            defer(printf("Cleanup for case1\n"));
        } break;
        case 2: {
            defer(printf("Cleanup for case2\n"));
        } break;
        default:
            defer(printf("Cleanup default\n"));
            break;
        }

        defer(printf("Cleanup for scope after switch\n"));
        defer(printf("Cleanup for scope before exit\n"));

        printf("Exiting scope\n");
    }
    scope__deferred;

    printf("After scope\n");
    return 0;
}
