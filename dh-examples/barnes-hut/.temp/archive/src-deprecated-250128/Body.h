#ifndef BODY_INCLUDED
#define BODY_INCLUDED (1)

#include "dh/math/vec.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/Arr.h"
#include "dh/ArrList.h"

typedef struct Body {
    m_V2f32 pos;
    m_V2f32 vel;
    m_V2f32 acc;
    f32     mass;
    f32     radius;
} Body;
use_Sli$(Body);
use_ArrList$(Body);
use_Opt$(Body);
use_Err$(ArrList$Body);

extern Body Body_new(m_V2f32 pos, m_V2f32 vel, f32 mass, f32 radius);
extern void Body_update(Body* self, f32 dt);

#endif /* BODY_INCLUDED */
