#include "engine/Input.h"
#include "dh/mem/common.h"

T_use$((engine_InputEvent)(
    ArrQue_init,
    ArrQue_fini,
    ArrQue_clearRetainingCap,
    ArrQue_enqueWithin,
    ArrQue_front,
    ArrQue_deque,
));

fn_((engine_Input_init(mem_Allocator gpa))(E$P$engine_Input) $guard) {
    let input = u_castP$((engine_Input*)(try_(mem_Allocator_create(gpa, typeInfo$(InnerType)))));
    errdefer_($ignore, mem_Allocator_destroy(gpa, u_anyP(input)));
    input->gpa = gpa;

    var event_queue = try_(ArrQue_init$engine_InputEvent(gpa, engine_Input_event_queue_len));
    errdefer_($ignore, ArrQue_fini$engine_InputEvent(&event_queue, gpa));
    input->event_queue $like_deref = event_queue;

    with_(let keyboard = input->keyboard) {
        mem_set0(u_anyS(A_ref(keyboard->keys.curr_states)));
        mem_set0(u_anyS(A_ref(keyboard->keys.prev_states)));
    }

    with_(let mouse = input->mouse) {
        mem_set0(u_anyS(A_ref(mouse->buttons.curr_states)));
        mem_set0(u_anyS(A_ref(mouse->buttons.prev_states)));

        mouse->cursor.curr_pos = m_V2i32_zero;
        mouse->cursor.prev_pos = m_V2i32_zero;

        mouse->wheel.curr_scroll_amount = m_V2f32_zero;
        mouse->wheel.prev_scroll_amount = m_V2f32_zero;
    }

    asg_lit((&input->backend)(none()));
    return_ok(input);
} $unguarded_(fn);

fn_((engine_Input_fini(engine_Input* self))(void)) {
    asg_lit((&self->backend)(none()));
    ArrQue_fini$engine_InputEvent(self->event_queue, self->gpa);
    mem_Allocator_destroy(self->gpa, u_anyP(self));
}

fn_((engine_Input_update(engine_Input* self))(E$void) $scope) {
    $unused(self);
    return_err(Err_NotImplemented());
    /* TODO: Implement this function */
} $unscoped_(fn);

fn_((engine_Input_clearEvent(engine_Input* self))(void)) {
    ArrQue_clearRetainingCap$engine_InputEvent(self->event_queue);
};

fn_((engine_Input_enqueEvent(engine_Input* self, engine_InputEvent event))(void)) {
    let q = self->event_queue;
    if (ArrQue_cap(*q->as_raw) <= q->len) {
        let_ignore = ArrQue_deque$engine_InputEvent(q);
    }
    ArrQue_enqueWithin$engine_InputEvent(q, event);
};

fn_((engine_Input_peekEvent(engine_Input* self))(O$engine_InputEvent) $scope) {
    return O_deref$((ReturnType)(ArrQue_front$engine_InputEvent(*self->event_queue)));
} $unscoped_(fn);


fn_((engine_Input_dequeEvent(engine_Input* self))(O$engine_InputEvent)) {
    return ArrQue_deque$engine_InputEvent(self->event_queue);
};
