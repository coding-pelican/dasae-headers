#include "engine/Input.h"
#include "dh/mem/common.h"

fn_((engine_Input_init(mem_Allocator allocator))(E$P$engine_Input) $guard) {
    let input = u_castP$((engine_Input*)(try_(mem_Allocator_create(allocator, typeInfo$(engine_Input)))));
    errdefer_($ignore, mem_Allocator_destroy(allocator, u_anyP(input)));
    input->allocator = allocator;

    with_(let event_buffer = &input->event_buffer) {
        mem_setBytes(as$(u8*)(event_buffer->events.val), 0, sizeOf$(event_buffer->events.val));
        event_buffer->count = 0;
        event_buffer->head = 0;
        event_buffer->tail = 0;
    }

    with_(let keyboard = input->keyboard) {
        mem_setBytes(keyboard->keys.curr_states.val, 0, sizeOf$(keyboard->keys.curr_states.val));
        mem_setBytes(keyboard->keys.prev_states.val, 0, sizeOf$(keyboard->keys.prev_states.val));
    }

    with_(let mouse = input->mouse) {
        mem_setBytes(mouse->buttons.curr_states.val, 0, sizeOf$(mouse->buttons.curr_states.val));
        mem_setBytes(mouse->buttons.prev_states.val, 0, sizeOf$(mouse->buttons.prev_states.val));

        mouse->cursor.curr_pos = m_V2i32_zero;
        mouse->cursor.prev_pos = m_V2i32_zero;

        mouse->wheel.curr_scroll_amount = m_V2f32_zero;
        mouse->wheel.prev_scroll_amount = m_V2f32_zero;
    }

    asg_lit((&input->backend)(none()));
    return_ok(input);
} $unguarded_(fn);

fn_((engine_Input_fini(engine_Input* self))(void)) {
    mem_Allocator_destroy(self->allocator, u_anyP(self));
}

fn_((engine_Input_update(engine_Input* self))(E$void) $scope) {
    $unused(self);
    return_err(Err_NotImplemented());
    /* TODO: Implement this function */
} $unscoped_(fn);

fn_((engine_InputEventBuffer_push(engine_Input* self, engine_InputEvent event))(void)) {
    $unused(self), $unused(event);
    /* TODO: Implement this function */
}

fn_((engine_InputEventBuffer_pop(engine_Input* self))(O$engine_InputEvent) $scope) {
    $unused(self);
    return_none();
    /* TODO: Implement this function */
} $unscoped_(fn);

fn_((engine_InputEventBuffer_peek(engine_Input* self))(O$engine_InputEvent) $scope) {
    $unused(self);
    return_none();
    /* TODO: Implement this function */
} $unscoped_(fn);

fn_((engine_InputEventBuffer_clear(engine_Input* self))(void)) {
    $unused(self);
    /* TODO: Implement this function */
}
