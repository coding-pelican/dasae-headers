#include "Body.h"

Body Body_new(math_Vec2f pos, math_Vec2f vel, f32 mass, f32 radius) {
    return (Body){
        .pos    = pos,
        .vel    = vel,
        .acc    = math_Vec2f_zero,
        .mass   = mass,
        .radius = radius,
    };
}

void Body_update(Body* self, f32 dt) {
    math_Vec2f_addAsg(&self->vel, math_Vec2f_scale(self->acc, dt));
    math_Vec2f_addAsg(&self->pos, math_Vec2f_scale(self->vel, dt));
}
