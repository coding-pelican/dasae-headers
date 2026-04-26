#include "Coop.h"

fn_((exec_Coop_init(mem_Alctr gpa, time_Awake clock, exec_Evented evented))(exec_Coop)) {
    return (exec_Coop){
        .timed = exec_LaneTimed_init(gpa, clock),
        .evented = evented,
    };
};

fn_((exec_Coop_fini(exec_Coop* self))(void)) {
    claim_assert_nonnull(self);
    exec_Evented_fini(&self->evented);
    exec_LaneTimed_fini(&self->timed);
    asg_l((self)(cleared()));
};

fn_((exec_Coop_task(exec_Coop* self))(O$P$exec_Task)) {
    claim_assert_nonnull(self);
    return exec_Lane_task(&self->timed.lane);
};

fn_((exec_Coop_evented(exec_Coop* self))(exec_Evented)) {
    claim_assert_nonnull(self);
    return self->evented;
};

fn_((exec_Coop_yield(exec_Coop* self))(void)) {
    claim_assert_nonnull(self);
    exec_Lane_yield(&self->timed.lane);
};

fn_((exec_Coop_run(exec_Coop* self))(void)) {
    claim_assert_nonnull(self);
    exec_LaneTimed_run(&self->timed);
};

fn_((exec_Coop_runUntil(exec_Coop* self, time_Awake_Inst deadline))(void)) {
    claim_assert_nonnull(self);
    exec_LaneTimed_runUntil(&self->timed, deadline);
};
