/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    defer_old.h
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


#ifndef DEFER_OLD_INCLUDED
#define DEFER_OLD_INCLUDED (1)
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

typedef struct DeferScope {
    DeferBlock* curr;
    DeferBlock* tail;
    bool        active;
    jmp_buf     execute_return; // block_defer__execute의 컨텍스트를 저장
} DeferScope;

// typedef struct DeferBlock {
//     struct DeferBlock* next;
//     jmp_buf            jump_buf;
//     jmp_buf            return_buf; // 복귀 지점 저장
//     bool               executed;
//     bool               in_cleanup; // cleanup 실행 중인지 표시
// } DeferBlock;

// // typedef struct DeferBlock {
// //     struct DeferBlock* next;     // prev 대신 next를 사용하여 FIFO 구현
// //     jmp_buf            jump_buf; // setjmp/longjmp 용 버퍼
// //     bool               executed;
// // } DeferBlock;

// typedef struct DeferScope {
//     DeferBlock* head; // FIFO queue의 시작
//     DeferBlock* tail; // 새로운 defer block을 붙일 위치
//     bool        active;
// } DeferScope;

// FIXME: Critical error
#define block_defer                  \
    for (DeferScope _block_defer = { \
             block_defer__sentinel,  \
             block_defer__sentinel,  \
             true,                   \
             { 0 } };                \
         _block_defer.active;        \
         /* None */)                 \
        for (bool _run_once = true;  \
             _run_once;              \
             (_run_once = false, block_defer__execute(&_block_defer)))

#define block_defer_return               \
    block_defer__execute(&_block_defer); \
    return

#define defer(STATEMENTS...)                         \
    do {                                             \
        DeferBlock* _block = mem_create(DeferBlock); \
        if (!_block) { break; }                      \
        _block->next     = block_defer__sentinel;    \
        _block->executed = false;                    \
        if (setjmp(_block->jump_buf) == 0) {         \
            /* 처음 setjmp 호출 시 */           \
            block_defer__add(&_block_defer, _block); \
            break;                                   \
        }                                            \
        /* cleanup 실행 */                         \
        STATEMENTS                                   \
        /* block_defer__execute로 돌아가기 */   \
        longjmp(_block_defer.execute_return, 1);     \
    } while (false)

#define block_defer__execute(scope) ({                              \
    debug_assertNonNull(scope);                                     \
    if (!(scope)->active) { return; }                               \
                                                                    \
    printf("Execute defer blocks...\n");                            \
    DeferBlock* curr = (scope)->head;                               \
    while (curr != block_defer__sentinel) {                         \
        if (!curr->executed) {                                      \
            curr->executed = true;                                  \
                                                                    \
            /* 현재 실행 컨텍스트를 저장 */              \
            if (setjmp((scope)->execute_return) == 0) {             \
                /* defer block으로 점프 */                      \
                printf("Executing defer block...\n");               \
                longjmp(curr->jump_buf, 1);                         \
                /* 이 지점 이후로는 실행되지 않음 */   \
            }                                                       \
            printf("Returned from defer block\n");                  \
            /* defer block 실행 완료 후 여기로 돌아옴 */ \
        }                                                           \
        DeferBlock* next = curr->next;                              \
        mem_destroy(&curr);                                         \
        curr = next;                                                \
        printf("Executed defer block\n");                           \
    }                                                               \
                                                                    \
    (scope)->active = false;                                        \
})

// #define defer(STATEMENTS...)                             \
//     do {                                                 \
//         DeferBlock* _block = mem_create(DeferBlock);     \
//         if (!_block) { break; }                          \
//         _block->next       = block_defer__sentinel;      \
//         _block->executed   = false;                      \
//         _block->in_cleanup = false;                      \
//         if (setjmp(_block->jump_buf) == 0) {             \
//             /* 처음 setjmp 호출 시 */               \
//             block_defer__add(&_block_defer, _block);     \
//             break;                                       \
//         }                                                \
//         /* cleanup 실행 */                             \
//         STATEMENTS                                       \
//         /* cleanup 실행 후 원래 위치로 복귀 */ \
//         if (_block->in_cleanup) {                        \
//             longjmp(_block->return_buf, 1);              \
//         }                                                \
//     } while (false)

// #define defer(STATEMENTS...)                         \
//     do {                                             \
//         DeferBlock* _block = mem_create(DeferBlock); \
//         if (!_block) { break; }                      \
//         _block->next     = block_defer__sentinel;    \
//         _block->executed = false;                    \
//         if (setjmp(_block->jump_buf) == 0) {         \
//             /* 처음 setjmp 호출 시 */           \
//             block_defer__add(&_block_defer, _block); \
//             break;                                   \
//         }                                            \
//         /* longjmp로 돌아왔을 때 */            \
//         STATEMENTS                                   \
//     } while (false)

// #define defer(STATEMENTS...)                                                                 \
//     do {                                                                                     \
//         DeferBlock* const _block = mem_create(DeferBlock);                                   \
//         if (!_block) { break; }                                                              \
//         /* defer block을 초기화 */                                                       \
//         _block->next     = block_defer__sentinel;                                            \
//         _block->executed = false;                                                            \
//         /* FIFO queue에 추가 */                                                           \
//         block_defer__add(&_block_defer, _block);                                             \
//         /* defer block을 평가하지 않고 스킵하기 위한 점프 포인트 설정 */ \
//         if (setjmp(_block->jump_point) == 0 && !_block->executed) { break; }                 \
//         /* defer block 실행 */                                                             \
//         _block->executed = true;                                                             \
//         STATEMENTS                                                                           \
//         /* defer__run으로 돌아가기 */                                                  \
//         if (_block->executed) {                                                              \
//             longjmp(_block->return_point, 1);                                                \
//         }                                                                                    \
//     } while (false)

extern DeferBlock* const block_defer__sentinel;

/*========== Extern Function Prototypes =====================================*/

extern void block_defer__add(DeferScope* scope, DeferBlock* block);
// extern void block_defer__execute(DeferScope* scope);

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEFER_OLD_INCLUDED  */
