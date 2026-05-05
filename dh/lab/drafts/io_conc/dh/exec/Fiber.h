#ifndef exec_Fiber__included
#define exec_Fiber__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../prl/Co/Fiber.h"
#include "dh/mem/Alctr.h"
#include "dh/mem/common.h"

/*========== Macros and Declarations ========================================*/

#define exec_Fiber_stack_reserve_size (usize_(1) * 1024 * 1024)
#define exec_Fiber_stack_commit_size (usize_(256) * 1024)
#define exec_Fiber_stack_grow_size (usize_(64) * 1024)
#define exec_Fiber_stack_guard_size mem_page_size
#define exec_Fiber_stack_size exec_Fiber_stack_reserve_size
T_alias$((exec_Fiber_StackPolicy)(struct exec_Fiber_StackPolicy {
    var_(reserve_size, usize);
    var_(initial_commit_size, usize);
    var_(grow_commit_size, usize);
    var_(guard_size, usize);
}));
T_alias$((exec_Fiber)(struct exec_Fiber {
    var_(storage, S$u8);
    var_(stack, S$u8);
    var_(grow_size, usize);
    var_(guard_size, usize);
    var_(is_virtual, bool);
    var_(context, Co_Fiber_Context);
}));
T_use_P$(exec_Fiber);
T_use_O$(P$exec_Fiber);
T_use_E$($set(mem_E)(P$exec_Fiber));
T_alias$((exec_Fiber_WorkFn)(fn_(((*)(P$raw owner, P$raw task))(void) $T)));
$attr($inline_always)
$static fn_((exec_Fiber_defaultStackPolicy(void))(exec_Fiber_StackPolicy));
$attr($must_check)
$extern fn_((exec_Fiber_init(
    mem_Alctr gpa,
    P$raw owner,
    P$raw task,
    exec_Fiber_WorkFn workFn
))(mem_E$P$exec_Fiber));
$attr($must_check)
$extern fn_((exec_Fiber_initWithPolicy(
    mem_Alctr gpa,
    P$raw owner,
    P$raw task,
    exec_Fiber_WorkFn workFn,
    exec_Fiber_StackPolicy policy
))(mem_E$P$exec_Fiber));
$extern fn_((exec_Fiber_fini(exec_Fiber* self, mem_Alctr gpa))(void));
$extern fn_((exec_Fiber_enter(exec_Fiber* self))(O$P$exec_Fiber));
$extern fn_((exec_Fiber_restore(O$P$exec_Fiber prev))(void));

T_alias$((exec_Fiber_Starter)(struct exec_Fiber_Starter {
    var_(owner, P$raw);
    var_(task, P$raw);
    var_(workFn, exec_Fiber_WorkFn);
}));

/*========== Macros and Definitions =========================================*/

fn_((exec_Fiber_defaultStackPolicy(void))(exec_Fiber_StackPolicy)) {
    return (exec_Fiber_StackPolicy){
        .reserve_size = exec_Fiber_stack_reserve_size,
        .initial_commit_size = exec_Fiber_stack_commit_size,
        .grow_commit_size = exec_Fiber_stack_grow_size,
        .guard_size = exec_Fiber_stack_guard_size,
    };
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Fiber__included */
