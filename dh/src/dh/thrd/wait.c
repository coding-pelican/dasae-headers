#include "dh/thrd/wait.h"

T_use$((thrd_wait_Link_Data)(
    ListSgl_Adp_init,
    ListSgl_empty,
    ListSgl_prepend,
    ListSgl_shift,
    ListSgl_Link_data,
    ListSgl_remove
));

fn_((thrd_wait_Link_from(thrd_Waker waker, usize case_idx))(thrd_wait_Link)) {
    return (thrd_wait_Link){
        .inner = ListSgl_Adp_init$thrd_wait_Link_Data((thrd_wait_Link_Data){
            .waker = thrd_Waker_ensureValid(waker),
            .case_idx = case_idx,
        }),
    };
};
fn_((thrd_wait_Link_data(const thrd_wait_Link* link))(const thrd_wait_Link_Data*)) {
    return &link->inner.data;
};
fn_((thrd_wait_Link_dataMut(thrd_wait_Link* link))(thrd_wait_Link_Data*)) {
    return &link->inner.data;
};

fn_((thrd_wait_Chain_init(void))(thrd_wait_Chain)) {
    return (thrd_wait_Chain){
        .inner = ListSgl_empty$thrd_wait_Link_Data(),
    };
};
fn_((thrd_wait_Chain_prepend(thrd_wait_Chain* self, thrd_wait_Link* link))(void)) {
    ListSgl_prepend$thrd_wait_Link_Data(&self->inner, &link->inner.link);
};
fn_((thrd_wait_Chain_wakeAll(thrd_wait_Chain* self))(void)) {
    while_some((ListSgl_shift$thrd_wait_Link_Data(&self->inner)), link) {
        thrd_Waker_wake(ListSgl_Link_data$thrd_wait_Link_Data(link)->waker);
    };
};
fn_((thrd_wait_Chain_wakeOne(thrd_wait_Chain* self))(void)) {
    if_none((ListSgl_shift$thrd_wait_Link_Data(&self->inner))) {
        return;
    } else_some(link) {
        thrd_Waker_wake(ListSgl_Link_data$thrd_wait_Link_Data(link)->waker);
    };
};
fn_((thrd_wait_Chain_unlink(thrd_wait_Chain* self, thrd_wait_Link* link))(void)) {
    ListSgl_remove$thrd_wait_Link_Data(&self->inner, &link->inner.link);
};
