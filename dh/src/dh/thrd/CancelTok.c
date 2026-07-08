#include "dh/thrd/CancelTok.h"

fn_((thrd_CancelTok_isCanceled(thrd_CancelTok self))(bool)) {
    return thrd_OnceEvt_Tok_isSet(self.event);
};
fn_((thrd_CancelTok_check(thrd_CancelTok self))(Sched_Cancelable$void) $scope) {
    if (thrd_CancelTok_isCanceled(self)) {
        return_err(E_cause$Sched_Canceled());
    }
    return_ok({});
} $unscoped(fn);
fn_((thrd_CancelTok_wait(thrd_CancelTok self))(void)) {
    thrd_OnceEvt_Tok_wait(self.event);
};
fn_((thrd_CancelTok_timedWait(thrd_CancelTok self, time_Dur timeout))(thrd_ftx_E$void)) {
    return thrd_OnceEvt_Tok_timedWait(self.event, timeout);
};

fn_((thrd_CancelTok_Src_init(void))(thrd_CancelTok_Src)) {
    return thrd_CancelTok_Src_init_static();
};
fn_((thrd_CancelTok_Src_fini(thrd_CancelTok_Src* self))(void)) {
    thrd_OnceEvt_fini(&self->event);
};
fn_((thrd_CancelTok_Src_tok(thrd_CancelTok_Src* self))(thrd_CancelTok)) {
    return (thrd_CancelTok){
        .event = thrd_OnceEvt_tok(&self->event),
    };
};
fn_((thrd_CancelTok_Src_cancel(thrd_CancelTok_Src* self))(void)) {
    thrd_OnceEvt_set(&self->event);
};
fn_((thrd_CancelTok_Src_isCanceled(thrd_CancelTok_Src* self))(bool)) {
    return thrd_OnceEvt_isSet(&self->event);
};
