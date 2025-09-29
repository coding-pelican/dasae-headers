#include "Body.h"

Body Body_new(m_V2f32 pos, m_V2f32 vel, f32 mass, f32 radius) {
    return (Body){
        .pos    = pos,
        .vel    = vel,
        .acc    = m_V2f32_zero,
        .mass   = mass,
        .radius = radius,
    };
}

void Body_update(Body* self, f32 dt) {
    m_V2f32_addTo(&self->vel, m_V2f32_scale(self->acc, dt));
    m_V2f32_addTo(&self->pos, m_V2f32_scale(self->vel, dt));
}
