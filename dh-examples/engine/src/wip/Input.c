#include "engine-wip/Input.h"
#include "dh/mem/common.h"

Err$Ptr$engine_Input engine_Input_init(mem_Allocator allocator) {
    scope_reserveReturn(Err$Ptr$engine_Input) {
        let input = meta_cast$(engine_Input*, try_(mem_Allocator_create(allocator, typeInfo$(engine_Input))));
        errdefer_(mem_Allocator_destroy(allocator, anyPtr(input)));
        input->allocator = allocator;

        /* event buffer */ {
            mem_setBytes(as$(u8*, input->event_buffer.events), 0, sizeOf(input->event_buffer.events));
            input->event_buffer.count = 0;
            input->event_buffer.head  = 0;
            input->event_buffer.tail  = 0;
        }

        /* keyboard */ {
            mem_setBytes(input->keyboard.keys.curr_states, 0, sizeOf(input->keyboard.keys.curr_states));
            mem_setBytes(input->keyboard.keys.prev_states, 0, sizeOf(input->keyboard.keys.prev_states));
        }

        /* mouse */ {
            mem_setBytes(input->mouse.buttons.curr_states, 0, sizeOf(input->mouse.buttons.curr_states));
            mem_setBytes(input->mouse.buttons.prev_states, 0, sizeOf(input->mouse.buttons.prev_states));

            input->mouse.cursor.curr_pos = math_Vec2i_zero;
            input->mouse.cursor.prev_pos = math_Vec2i_zero;

            input->mouse.wheel.curr_scroll_amount = math_Vec2f_zero;
            input->mouse.wheel.prev_scroll_amount = math_Vec2f_zero;
        }

        noneAsg(input->backend);
        return_ok(input);
    }
    scope_returnReserved;
}

void engine_Input_fini(engine_Input* self) {
    mem_Allocator_destroy(self->allocator, anyPtr(self));
}

Err$void engine_Input_update(engine_Input* self) {
    reserveReturn(Err$void);
    unused(self);
    return_err(Err_NotImplemented());
    /* TODO: Implement this function */
}

void engine_InputEventBuffer_push(engine_Input* self, engine_InputEvent event) {
    unused(self), unused(event);
    /* TODO: Implement this function */
}

Opt$engine_InputEvent engine_InputEventBuffer_pop(engine_Input* self) {
    reserveReturn(Opt$engine_InputEvent);
    unused(self);
    return_none();
    /* TODO: Implement this function */
}

Opt$engine_InputEvent engine_InputEventBuffer_peek(engine_Input* self) {
    reserveReturn(Opt$engine_InputEvent);
    unused(self);
    return_none();
    /* TODO: Implement this function */
}

void engine_InputEventBuffer_clear(engine_Input* self) {
    unused(self);
    /* TODO: Implement this function */
}
