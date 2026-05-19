#include "actor.h"

T_use$((ListDbl_Adp$actor_Msg)(P, (E, P)));

$static fn_((actor_Sys__takeMsgNode(actor_Sys* self))(E$P$ListDbl_Adp$actor_Msg) $scope) {
    claim_assert_nonnull(self);
    if_some((ListDbl_shift$actor_Msg(&self->free_msgs))(link)) {
        return_ok(ListDbl_Link_adpMut$actor_Msg(link));
    }
    let gpa = heap_Arena_alctr(&self->arena_msgs);
    let node = u_castP$((ListDbl_Adp$actor_Msg*)(try_(mem_Alctr_create($trace gpa, typeInfo$(ListDbl_Adp$actor_Msg)))));
    return_ok(node);
} $unscoped(fn);

$static fn_((actor_Unit__setExit(actor_Unit* self, actor_Exit exit))(void)) {
    claim_assert_nonnull(self);
    asg_l((&self->exit)(some(exit)));
    asg_l((&self->task)(none()));
};

$static fn_((actor_Unit__syncExit(actor_Unit* self))(void)) {
    claim_assert_nonnull(self);
    if (isSome(self->exit) || isNone(self->task)) return;

    let task = unwrap_(self->task);
    if (task->state == exec_Task_State_canceled) {
        return actor_Unit__setExit(self, actor_Exit_canceledNow());
    }
    if (task->state != exec_Task_State_done) return;

    match_((self->completion)) {
    pattern_((actor_Unit_Ret_void)(future)) {
        let_ignore = future;
        actor_Unit__setExit(self, actor_Exit_normalNow());
    } $end(pattern);
    pattern_((actor_Unit_Ret_err_void)(future)) {
        actor_Unit__setExit(
            self,
            expr_(actor_Exit $scope)(if_ok_void((future.result)) {
                $break_(actor_Exit_normalNow());
            } else_err((err)) {
                $break_(actor_Exit_errorOf(err));
            }) $unscoped(expr)
        );
    } $end(pattern);
    pattern_((actor_Unit_Ret_undefined)($ignore)) claim_unreachable $end(pattern);
    } $end(match);
};

fn_((actor_Sys_init(exec_Coop* coop, mem_Alctr base_alctr, usize max_units))(mem_E$actor_Sys) $scope) {
    claim_assert_nonnull(coop);
    var arena_units = heap_Arena_init(base_alctr);
    let arena_msgs = heap_Arena_init(base_alctr);
    let units = local_({
        let gpa = heap_Arena_alctr(&arena_units);
        let mem = u_castS$((S$actor_Unit)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(actor_Unit), max_units))));
        for_(($s(mem))(unit)) asg_l((unit)({
            .mailbox = ListDbl_empty$actor_Msg(),
            .task = none(),
            .exit = none(),
            .completion = union_of((actor_Unit_Ret_undefined){}),
            .id = 0,
        })) $end(for);
        local_return_(mem);
    });
    return_ok({
        .coop = coop,
        .sched = Sched_coop(coop),
        .alctr = base_alctr,
        .arena_units = arena_units,
        .arena_msgs = arena_msgs,
        .units = units,
        .free_msgs = ListDbl_empty$actor_Msg(),
        .next_id = 0,
    });
} $unscoped(fn);

fn_((actor_Sys_fini(actor_Sys* self))(void)) {
    claim_assert_nonnull(self);
    heap_Arena_fini(&self->arena_msgs);
    heap_Arena_fini(&self->arena_units);
};

fn_((actor_Sys_createUnit(actor_Sys* self))(mem_E$P$actor_Unit) $scope) {
    claim_assert_nonnull(self);
    if (self->next_id >= self->units.len) return_err(E_cause$OutOfMemory());

    let unit = S_at((self->units)[self->next_id]);
    asg_l((unit)({
        .sys = self,
        .mailbox = ListDbl_empty$actor_Msg(),
        .task = none(),
        .exit = none(),
        .completion = union_of((actor_Unit_Ret_undefined){}),
        .id = self->next_id++,
    }));
    return_ok(unit);
} $unscoped(fn);

fn_((actor_Sys_destroyUnit(actor_Sys* self, actor_Unit** unit))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(unit);
    *unit = null;
};

T_use$((Void)(Sched_async));
fn_((actor_Unit_start(actor_Unit* self, Clsr$Void* worker))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(worker);
    claim_assert(isNone(self->task));
    claim_assert(isNone(self->exit));

    let completion = local_({
        let async = Sched_async$Void;
        let unbound = &self->completion;
        let bound = asg_l((unbound)(union_of((actor_Unit_Ret_void)(async(self->sys->sched, worker)))));
        local_return_(union_as((bound)(actor_Unit_Ret_void)));
    });
    if_some((completion->any_future)(any_future)) {
        asg_l((&self->task)(some(ptrAlignCast$((P$exec_Task)(any_future)))));
    } else_none {
        actor_Unit__setExit(self, actor_Exit_normalNow());
    }
};

T_use$((E$Void)(Sched_async));
fn_((actor_Unit_startE(actor_Unit* self, Clsr$E$Void* worker))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(worker);
    claim_assert(isNone(self->task));
    claim_assert(isNone(self->exit));

    let completion = local_({
        let async = Sched_async$E$Void;
        let unbound = &self->completion;
        let bound = asg_l((unbound)(union_of((actor_Unit_Ret_err_void)(async(self->sys->sched, worker)))));
        local_return_(union_as((bound)(actor_Unit_Ret_err_void)));
    });
    if_some((completion->any_future)(any_future)) {
        asg_l((&self->task)(some(ptrAlignCast$((P$exec_Task)(any_future)))));
    } else_none {
        let result = completion->result;
        actor_Unit__setExit(
            self,
            expr_(actor_Exit $scope)(if_ok_void((result)) {
                $break_(actor_Exit_normalNow());
            } else_err((err)) {
                $break_(actor_Exit_errorOf(err));
            }) $unscoped(expr)
        );
    }
};

fn_((actor_Unit_exit(actor_Unit* self))(O$actor_Exit)) {
    claim_assert_nonnull(self);
    actor_Unit__syncExit(self);
    return self->exit;
};

fn_((actor_Unit_send(actor_Unit* self, actor_Msg msg))(mem_E$void) $scope) {
    claim_assert_nonnull(self);
    actor_Unit__syncExit(self);
    if (isSome(self->exit) || isNone(self->task)) {
        return_err(E_cause$Unexpected());
    }

    let task = unwrap_(self->task);
    $supress_switch_enum(switch (task->state) {
        case exec_Task_State_done:
        case exec_Task_State_canceled: {
            return_err(E_cause$Unexpected());
        } break;
        default: {
        } break;
    });

    let node = try_(actor_Sys__takeMsgNode(self->sys));
    *node = ListDbl_Adp_init$actor_Msg(msg);
    ListDbl_append$actor_Msg(&self->mailbox, ListDbl_Adp_linkMut$actor_Msg(node));

    if (task->state == exec_Task_State_waiting) {
        if (!exec_Lane_readyTask(&self->sys->coop->timed.lane, task)) {
            return_err(E_cause$OutOfMemory());
        }
    }
    return_ok({});
} $unscoped(fn);

fn_((actor_Unit_tryRecv(actor_Unit* self))(O$actor_Msg) $scope) {
    claim_assert_nonnull(self);
    let link = orelse_((ListDbl_shift$actor_Msg(&self->mailbox))({
        if_some((exec_Coop_task(self->sys->coop))(curr_task)) {
            if_some((self->task)(task)) {
                claim_assert(curr_task == task);
                curr_task->state = exec_Task_State_waiting;
            }
        }
        return_none();
    }));
    let node = ListDbl_Link_adpMut$actor_Msg(link);
    let msg = node->data;
    ListDbl_append$actor_Msg(&self->sys->free_msgs, link);
    return_some(msg);
} $unscoped(fn);
