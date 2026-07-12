#include "dh/thrd/wait.h"

T_use$((thrd_wait_Link_Data)(
    ListSgl_empty,
    ListSgl_prepend,
    ListSgl_shift,
    ListSgl_remove,
    ListSgl_Link_dataMut
));

fn_((thrd_wait_List_init(void))(thrd_wait_List)) {
    return (thrd_wait_List){
        .inner = ListSgl_empty$thrd_wait_Link_Data(),
    };
};
fn_((thrd_wait_List_prepend(thrd_wait_List* self, thrd_wait_Link* link))(void)) {
    ListSgl_prepend$thrd_wait_Link_Data(&self->inner, &link->link);
};
fn_((thrd_wait_List_wakeAll(thrd_wait_List* self))(void)) {
    while_some((ListSgl_shift$thrd_wait_Link_Data(&self->inner)), link) {
        let data = ListSgl_Link_dataMut$thrd_wait_Link_Data(link);
        data->wakeFn(data->wake_ctx);
    };
};
fn_((thrd_wait_List_wakeOne(thrd_wait_List* self))(void)) {
    if_none((ListSgl_shift$thrd_wait_Link_Data(&self->inner))) {
        return;
    } else_some(link) {
        let data = ListSgl_Link_dataMut$thrd_wait_Link_Data(link);
        data->wakeFn(data->wake_ctx);
    };
};
fn_((thrd_wait_List_unlink(thrd_wait_List* self, thrd_wait_Link* link))(void)) {
    ListSgl_remove$thrd_wait_Link_Data(&self->inner, &link->link);
};
