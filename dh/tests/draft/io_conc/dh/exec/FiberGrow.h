#ifndef exec_FiberGrow__included
#define exec_FiberGrow__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Fiber.h"
#include "../atom.h"

/*========== Macros and Declarations ========================================*/

$attr($inline_always)
$static fn_((exec_Fiber__alignPage(usize size))(usize));
$attr($inline_always)
$static fn_((exec_Fiber__usableStack(exec_Fiber* self, usize commit_size))(S$u8));

$extern fn_((exec_Fiber_initStorage(exec_Fiber* self, mem_Alctr gpa, exec_Fiber_StackPolicy policy))(mem_E$void));
$extern fn_((exec_Fiber_finiStorage(exec_Fiber* self, mem_Alctr gpa))(void));

/*========== Macros and Definitions =========================================*/

fn_((exec_Fiber__alignPage(usize size))(usize)) {
    return mem_alignFwd(size == 0 ? mem_page_size : size, mem_page_size);
};

fn_((exec_Fiber__usableStack(exec_Fiber* self, usize commit_size))(S$u8)) {
    claim_assert_nonnull(self), claim_assert(commit_size <= self->storage.len), claim_assert(commit_size >= self->guard_size);
    let storage_begin = ptrToInt(self->storage.ptr);
    let commit_begin = storage_begin + (self->storage.len - commit_size);
    return (S$u8){
        .ptr = intToPtr$((u8*)(commit_begin + self->guard_size)),
        .len = commit_size - self->guard_size,
    };
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_FiberGrow__included */
