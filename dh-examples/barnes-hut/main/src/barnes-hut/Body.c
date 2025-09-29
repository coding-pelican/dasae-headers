#include "barnes-hut/Body.h"

fn_(Body_new(m_V2f32 pos, m_V2f32 vel, f32 mass, f32 radius), Body) {
    return (Body){
        .pos    = pos,
        .vel    = vel,
        .acc    = m_V2f32_zero,
        .mass   = mass,
        .radius = radius,
    };
}

fn_(Body_update(Body* self, f32 dt), void) {
    m_V2f32_addAsg(&self->vel, m_V2f32_scale(self->acc, dt));
    m_V2f32_addAsg(&self->pos, m_V2f32_scale(self->vel, dt));
}
