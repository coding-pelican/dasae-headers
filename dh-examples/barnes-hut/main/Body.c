#include "Body.h"
#include "dh/pipe.h"

fn_(Body_new(math_Vec2f pos, math_Vec2f vel, f32 mass, f32 radius), Body) {
    return (Body){
        .pos    = pos,
        .vel    = vel,
        .acc    = math_Vec2f_zero,
        .mass   = mass,
        .radius = radius,
    };
}

fn_(Body_update(Body* self, f32 dt), void) {
    math_Vec2f_addAsg(&self->vel, math_Vec2f_scale(self->acc, dt));
    math_Vec2f_addAsg(&self->pos, math_Vec2f_scale(self->vel, dt));
}
