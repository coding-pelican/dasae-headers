#include <stdio.h>
#include "dh/core.h"

/* Example: fixed maximum 64 defers in one scope_defer block. */
#ifndef MAX_DEFER_PER_SCOPE
#define MAX_DEFER_PER_SCOPE 64
#endif /* MAX_DEFER_PER_SCOPE */

#define scope_defer                                               \
    /*                                                            \
     * Start of a scope_defer block:                              \
     * - Create an array for labels                               \
     * - Create a 'top' index                                     \
     * - Jump to the user code                                    \
     * - Then define the 'cleanup' label which unwinds in reverse \
     */                                                           \
    IMPL_scope_defer
#define IMPL_scope_defer                             \
    /* store 1 label per 'defer(...)' call. */       \
    static void* _defer_labels[MAX_DEFER_PER_SCOPE]; \
    static int   _defer_count = 0;                   \
    goto _scope_body;                                \
    /* Cleanup: run them in reverse order: */        \
    _scope_cleanup:                                  \
    while (_defer_count > 0) {                       \
        _defer_count--;                              \
        if (_defer_labels[_defer_count]) {           \
            goto* _defer_labels[_defer_count];       \
        } else {                                     \
            continue;                                \
        }                                            \
    }                                                \
    goto _scope_end;                                 \
    _scope_body:

#define scope_deferred                   \
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
        int local_id = _counter;                                \
        if (local_id < MAX_DEFER_PER_SCOPE) {                   \
            _defer_labels[local_id] = &&_defer_run;             \
            _defer_count            = local_id + 1;             \
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
    static void* _defer_labels[32];
    static int   _defer_count = 0;
    goto _scope_body;
_scope_cleanup:
    while (0 < _defer_count) {
        _defer_count--;
        if (_defer_labels[_defer_count]) {
            goto* _defer_labels[_defer_count];
        } else {
            continue;
        }
    }
    goto _scope_end;
_scope_body: {
    printf("Enter scope\n");

    do {
        int local_id = 0;
        if (local_id < 32) {
            _defer_labels[local_id] = &&_defer_run_0;
            _defer_count            = local_id + 1;
            goto _defer_skip_0;
        _defer_run_0:
            printf("Cleanup for scope after enter\n");
            goto _scope_cleanup;
        _defer_skip_0:;
        } else {
        }
    } while (0);

    switch (2) {
    case 1: {
        do {
            int local_id = 1;
            if (local_id < 32) {
                _defer_labels[local_id] = &&_defer_run_1;
                _defer_count            = local_id + 1;
                goto _defer_skip_1;
            _defer_run_1:
                printf("Cleanup for case1\n");
                goto _scope_cleanup;
            _defer_skip_1:;
            } else {
            }
        } while (0);
        break;
    }
    case 2: {
        do {
            int local_id = 2;
            if (local_id < 32) {
                _defer_labels[local_id] = &&_defer_run_2;
                _defer_count            = local_id + 1;
                goto _defer_skip_2;
            _defer_run_2:
                printf("Cleanup for case2\n");
                goto _scope_cleanup;
            _defer_skip_2:;
            } else {
            }
        } while (0);
        break;
    }
    default:
        do {
            int local_id = 3;
            if (local_id < 32) {
                _defer_labels[local_id] = &&_defer_run_3;
                _defer_count            = local_id + 1;
                goto _defer_skip_3;
            _defer_run_3:
                printf("Cleanup default\n");
                goto _scope_cleanup;
            _defer_skip_3:;
            } else {
            }
        } while (0);
        break;
    }

    do {
        int local_id = 4;
        if (local_id < 32) {
            _defer_labels[local_id] = &&_defer_run_4;
            _defer_count            = local_id + 1;
            goto _defer_skip_4;
        _defer_run_4:
            printf("Cleanup for scope before exit\n");
            goto _scope_cleanup;
        _defer_skip_4:;
        } else {
        }
    } while (0);

    printf("Exiting scope\n");
}
    goto _scope_cleanup;
_scope_end:
    (void)0;

    printf("After scope\n");
    return 0;
}
