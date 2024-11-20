#include "dh/debug/assert.h"
#include "dh/mem.h"
#include "dh/defer_old.h"

#include <stdio.h>

// static DeferBlock block_defer__s_sentinel = { &block_defer__s_sentinel, { 0 }, true };
// DeferBlock* const block_defer__sentinel   = &block_defer__s_sentinel;

// void block_defer__add(DeferScope* scope, DeferBlock* block) {
//     debug_assertNonNull(scope);
//     debug_assertNonNull(block);
//     if (!scope->active) {
//         mem_destroy(&block);
//         return;
//     }

//     /* FIFO 구현을 위해 tail 앞에 새 노드 삽입 */
//     // block->next     = block_defer__sentinel;
//     // block->executed = false;
//     if (scope->curr == block_defer__sentinel) {
//         scope->curr = block;
//     } else {
//         scope->tail->next = block;
//     }
//     scope->tail = block;
// }

// void block_defer__execute(DeferScope* scope) {
//     debug_assertNonNull(scope);
//     if (!scope->active) { return; }

//     printf("Execute defer blocks...\n");
//     DeferBlock* curr = scope->head;
//     while (curr != block_defer__sentinel) {
//         if (!curr->executed) {
//             curr->executed = true;

//             // 현재 실행 컨텍스트를 저장
//             if (setjmp(scope->execute_return) == 0) {
//                 // defer block으로 점프
//                 printf("Executing defer block...\n");
//                 longjmp(curr->jump_buf, 1);
//                 // 이 지점 이후로는 실행되지 않음
//             }
//             printf("Returned from defer block\n");
//             // defer block 실행 완료 후 여기로 돌아옴
//         }
//         DeferBlock* next = curr->next;
//         mem_destroy(&curr);
//         curr = next;
//         printf("Executed defer block\n");
//     }

//     scope->active = false;
// }

// void block_defer__execute(DeferScope* scope) {
//     debug_assertNonNull(scope);
//     if (!scope->active) { return; }

//     DeferBlock* curr = scope->head;
//     while (curr != block_defer__sentinel) {
//         if (!curr->executed) {
//             curr->executed   = true;
//             curr->in_cleanup = true;

//             // cleanup으로 점프하기 전에 현재 위치 저장
//             if (setjmp(curr->return_buf) == 0) {
//                 longjmp(curr->jump_buf, 1);
//             }
//             // cleanup 실행 후 여기로 돌아옴
//             curr->in_cleanup = false;
//         }
//         DeferBlock* next = curr->next;
//         mem_destroy(&curr);
//         curr = next;
//     }
//     scope->active = false;
// }

// void block_defer__execute(DeferScope* scope) {
//     debug_assertNonNull(scope);
//     if (!scope->active) { return; }

//     DeferBlock* curr = scope->head;
//     while (curr != block_defer__sentinel) {
//         if (!curr->executed) {
//             curr->executed = true;
//             longjmp(curr->jump_buf, 1); // 원래 스택 프레임으로 점프
//         }
//         DeferBlock* next = curr->next;
//         mem_destroy(&curr);
//         curr = next;
//     }
//     scope->active = false;
// }

// void block_defer__execute(DeferScope* scope) {
//     debug_assertNonNull(scope);
//     if (!scope->active) { return; }

//     DeferBlock* const begin = scope->head;
//     DeferBlock* const end   = block_defer__sentinel;

//     /* 모든 블록을 실행 */ {
//         DeferBlock* mark = begin;
//         while (mark != end) {
//             if (!mark->executed) {
//                 mark->executed = true;
//                 // defer block으로 점프하기 전에 돌아올 위치 저장
//                 if (setjmp(mark->return_point) == 0) {
//                     // defer block으로 점프
//                     longjmp(mark->jump_point, 1);
//                 }
//             }
//             mark = mark->next;
//         }
//     }
//     /* 메모리 정리 */ {
//         DeferBlock* curr = begin;
//         while (curr != end) {
//             DeferBlock* next = curr->next;
//             mem_destroy(&curr);
//             curr = next;
//         }
//     }

//     scope->active = false;
// }

/* void defer__run(DeferScope* scope) {
    DeferBlock* curr = scope->head;

    while (curr) {
        if (!curr->executed) {
            curr->executed = true;

            // defer block으로 점프하기 전에 돌아올 위치 저장
            if (setjmp(curr->return_point) == 0) {
                // defer block으로 점프
                longjmp(curr->jump_point, 1);
            }
        }

        // 현재 block 정리 및 다음으로 이동
        DeferBlock* next = curr->next;
        mem_destroy(&curr);
        curr = next;
    }

    scope->active = false;
} */

/* void defer__run(DeferScope* scope) {
    DeferBlock* curr = scope->current;
    while (curr) {
        if (!curr->executed) {
            curr->executed = true;
            longjmp(curr->jmp, 1);
        }
        DeferBlock* prev = curr->prev;
        mem_destroy(&curr);
        curr = prev;
    }
    scope->active = false;
} */
