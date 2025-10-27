#ifndef BARNES_HUT_BODY_INCLUDED
#define BARNES_HUT_BODY_INCLUDED (1)

#include "dh/sli.h"
#include "dh/ArrList.h"
#include "dh/math/vec.h"

typedef struct Body {
    m_V2f32 pos;
    m_V2f32 vel;
    m_V2f32 acc;
    f32     mass;
    f32     radius;
} Body;
use_S$(Body);
use_ArrList$(Body);

extern fn_(Body_new(m_V2f32 pos, m_V2f32 vel, f32 mass, f32 radius), Body);
extern fn_(Body_update(Body* self, f32 dt), void);

#endif /* BARNES_HUT_BODY_INCLUDED */
