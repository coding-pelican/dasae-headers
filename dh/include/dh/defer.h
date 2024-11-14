/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    defer.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated 2024-11-12 (date of last update)
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
#include "mem.h"

#include <setjmp.h>

/*========== Macros and Definitions =========================================*/

/* More sophisticated version combining best aspects */
typedef struct DeferBlock {
    struct DeferBlock* prev;
    jmp_buf            jump_buf;
    bool               executed;
} DeferBlock;
static DeferBlock DeferBlock_sentinel_node = { &DeferBlock_sentinel_node, { 0 }, true };

force_inline DeferBlock* DeferBlock_init(DeferBlock* b) {
    b->prev     = &DeferBlock_sentinel_node;
    b->executed = false;
    return b;
}


typedef struct DeferScope {
    DeferBlock* curr;
    bool        active;
    jmp_buf     execute_return; // defer_scope__execute의 컨텍스트를 저장
} DeferScope;
static DeferBlock* const DeferScope__sentinel = &DeferBlock_sentinel_node;

force_inline DeferScope* DeferScope_init(DeferScope* s) {
    s->curr   = DeferScope__sentinel;
    s->active = true;
    return s;
}
force_inline DeferScope* DeferScope_fini(DeferScope* s) {
    while (s->curr != DeferScope__sentinel) {
        DeferBlock* curr = s->curr;
        s->curr          = curr->prev;
        mem_destroy(curr);
    }
    s->curr   = DeferScope__sentinel;
    s->active = false;
    return s;
}
force_inline void DeferScope_addBlock(DeferScope* s, DeferBlock* block) {
    if (s->curr != DeferScope__sentinel) {
        block->prev = s->curr;
    }
    s->curr = block;
}

#define DeferScope_deferred(S) ({                                   \
    printf("Execute defer blocks...\n");                            \
    DeferBlock* _curr = (S)->curr;                                  \
    while (_curr != DeferScope__sentinel) {                         \
        if (!_curr->executed) {                                     \
            _curr->executed = true;                                 \
            /* 현재 실행 컨텍스트를 저장 */              \
            if (setjmp((S)->execute_return) == 0) {                 \
                /* defer block으로 점프 */                      \
                printf("Executing defer block...\n");               \
                longjmp(_curr->jump_buf, 1);                        \
                /* 이 지점 이후로는 실행되지 않음 */   \
            }                                                       \
            printf("Returned from defer block\n");                  \
            /* defer block 실행 완료 후 여기로 돌아옴 */ \
        }                                                           \
        _curr = _curr->prev;                                        \
        printf("Executed defer block\n");                           \
    }                                                               \
})

#define defer_scope                             \
    for (DeferScope* const _defer_scope         \
         = DeferScope_init(create(DeferScope)); \
         _defer_scope->active;                  \
         /* none */)                            \
        for (bool _run_once = true;             \
             _run_once;                         \
             (_run_once = false,                \
             DeferScope_deferred(_defer_scope), \
             DeferScope_fini(_defer_scope)))

#define defer_scope_return             \
    DeferScope_deferred(_defer_scope); \
    DeferScope_fini(_defer_scope);\
    return

#define defer(STATEMENTS...)                                                \
    do {                                                                    \
        DeferBlock* const _block = DeferBlock_init(mem_create(DeferBlock)); \
        if (setjmp(_block->jump_buf) == 0) {                                \
            /* first call setjmp */                                         \
            DeferScope_addBlock(_defer_scope, _block);                      \
            break;                                                          \
        }                                                                   \
        STATEMENTS                                                          \
        /* return DeferScope_deferred */                                    \
        longjmp(_defer_scope->execute_return, 1);                           \
    } while (false)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEFER_INCLUDED  */
