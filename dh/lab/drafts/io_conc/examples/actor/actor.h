#ifndef actor__included
#define actor__included 1

#include "../../dh/exec.h"
#include "../../dh/Future.h"
#include "../../dh/Sched.h"
#include <dh/ListDbl.h>
#include <dh/heap/Arena.h>
#include <dh/mem/Alctr.h>

/*--- Message ---*/

T_alias$((actor_Msg)(variant_((actor_Msg $fits($packed))(
    (actor_Msg_increment, i64),
    (actor_Msg_get_value, Void),
    (actor_Msg_stop, Void),
))));
T_use$((actor_Msg)(P, O, (O, P), E, (E, P)));
$attr($inline_always)
$static fn_((actor_Msg_inc(i64 delta))(actor_Msg)) {
    return union_with$((actor_Msg)(actor_Msg_increment)(delta));
};
$attr($inline_always)
$static fn_((actor_Msg_getValue(void))(actor_Msg)) {
    return union_with$((actor_Msg)(actor_Msg_get_value)(l0$((Void))));
};
$attr($inline_always)
$static fn_((actor_Msg_stopNow(void))(actor_Msg)) {
    return union_with$((actor_Msg)(actor_Msg_stop)(l0$((Void))));
};

/*--- Mailbox ---*/

T_use$((actor_Msg)(
    ListDbl_Link,
    ListDbl_Adp,
    ListDbl_Adp_init,
    ListDbl_Adp_linkMut,
    ListDbl_Link_adpMut,
    ListDbl,
    ListDbl_empty,
    ListDbl_append,
    ListDbl_shift
));
T_alias$((actor_Mailbox)(ListDbl$actor_Msg));

/*--- Exit ---*/

T_alias$((actor_Exit)(variant_((actor_Exit $fits($packed))(
    (actor_Exit_normal, Void),
    (actor_Exit_error, EAny),
    (actor_Exit_canceled, Void),
))));
T_use$((actor_Exit)(P, O));
$attr($inline_always)
$static fn_((actor_Exit_normalNow(void))(actor_Exit)) {
    return union_with$((actor_Exit)(actor_Exit_normal)(l0$((Void))));
};
$attr($inline_always)
$static fn_((actor_Exit_errorOf(EAny reason))(actor_Exit)) {
    return union_with$((actor_Exit)(actor_Exit_error)(reason));
};
$attr($inline_always)
$static fn_((actor_Exit_canceledNow(void))(actor_Exit)) {
    return union_with$((actor_Exit)(actor_Exit_canceled)(l0$((Void))));
};

/*--- System ---*/

T_use$((Void)(Future));
T_use$((E$Void)(Future, Clsr_Ctx, Clsr_Rtn, Clsr));
T_alias$((actor_Unit_Ret)(variant_((actor_Unit_Ret $fits($packed))(
    (actor_Unit_Ret_undefined, Void),
    (actor_Unit_Ret_void, Future$Void),
    (actor_Unit_Ret_err_void, Future$E$Void),
))));

T_alias$((actor_Unit)(struct actor_Unit));
T_use$((actor_Unit)(P, (E, P), ($spec(E, $set(mem_E)), P), S, (E, S), ($spec(E, $set(mem_E)), S)));

T_alias$((actor_Sys)(struct actor_Sys));
T_decl_E$($set(mem_E)(actor_Sys));
$attr($must_check)
$extern fn_((actor_Sys_init(exec_Coop* coop, mem_Alctr base_alctr, usize max_units))(mem_E$actor_Sys));
$extern fn_((actor_Sys_fini(actor_Sys* self))(void));
$attr($must_check)
$extern fn_((actor_Sys_createUnit(actor_Sys* self))(mem_E$P$actor_Unit));
$extern fn_((actor_Sys_destroyUnit(actor_Sys* self, actor_Unit** unit))(void));

struct actor_Unit {
    var_(sys, actor_Sys*);
    var_(mailbox, actor_Mailbox);
    var_(task, O$P$exec_Task);
    var_(exit, O$actor_Exit);
    var_(completion, actor_Unit_Ret);
    var_(id, u32);
};
$extern fn_((actor_Unit_start(actor_Unit* self, Clsr$Void* worker))(void));
$extern fn_((actor_Unit_startE(actor_Unit* self, Clsr$E$Void* worker))(void));
#define T_use_actor_Unit_startE$(_E...) __stmt__T_use_actor_Unit_startE$(_E)
$extern fn_((actor_Unit_exit(actor_Unit* self))(O$actor_Exit));
$attr($must_check)
$extern fn_((actor_Unit_send(actor_Unit* self, actor_Msg msg))(mem_E$void));
$extern fn_((actor_Unit_tryRecv(actor_Unit* self))(O$actor_Msg));

struct actor_Sys {
    var_(coop, exec_Coop*);
    var_(sched, Sched);
    var_(alctr, mem_Alctr);
    var_(arena_units, heap_Arena);
    var_(arena_msgs, heap_Arena);
    var_(units, S$actor_Unit);
    var_(free_msgs, actor_Mailbox);
    var_(next_id, u32);
};
T_impl_E$($set(mem_E)(actor_Sys));

/*========== Macros and Definitions =========================================*/

#define __stmt__T_use_actor_Unit_startE$(_E...) /* clang-format off */ \
    $attr($inline_always) \
    $static fn_((tpl$(actor_Unit_startE, _E)(actor_Unit* self, Clsr$(E$($set(_E)(Void)))* worker))(void)) { \
        return actor_Unit_startE(self, ptrCast$((Clsr$E$Void*)(worker))); \
    } /* clang-format on */

#endif /* actor__included */
