#ifndef BODY_INCLUDED
#define BODY_INCLUDED (1)

#include "dh/math/vec.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/Arr.h"
#include "dh/ArrList.h"

typedef struct Body {
    math_Vec2f pos;
    math_Vec2f vel;
    math_Vec2f acc;
    f32        mass;
    f32        radius;
} Body;
use_Sli$(Body);
use_ArrList$(Body);
use_Err$(ArrList$Body);

extern Body Body_new(math_Vec2f pos, math_Vec2f vel, f32 mass, f32 radius);
extern void Body_update(Body* self, f32 dt);

#endif /* BODY_INCLUDED */
