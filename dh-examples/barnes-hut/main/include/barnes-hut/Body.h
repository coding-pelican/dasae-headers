#ifndef BARNES_HUT_BODY_INCLUDED
#define BARNES_HUT_BODY_INCLUDED (1)

#include "dh/sli.h"
#include "dh/ArrList.h"
#include "dh/math/vec.h"

typedef struct Body {
    math_Vec2f pos;
    math_Vec2f vel;
    math_Vec2f acc;
    f32        mass;
    f32        radius;
} Body;
use_Sli$(Body);
use_ArrList$(Body);

extern fn_(Body_new(math_Vec2f pos, math_Vec2f vel, f32 mass, f32 radius), Body);
extern fn_(Body_update(Body* self, f32 dt), void);

#endif /* BARNES_HUT_BODY_INCLUDED */
