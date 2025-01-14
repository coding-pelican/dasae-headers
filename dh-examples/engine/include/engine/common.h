#ifndef ENGINE_COMMON_INCLUDED
#define ENGINE_COMMON_INCLUDED (1)

#include "dh/core.h"
#include "dh/scope.h"
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/mem/Allocator.h"
#include "dh/time.h"
#include "dh/math/common.h"
#include "dh/math/vec.h"

typedef math_Vec2f Vec2f;
typedef math_Vec3f Vec3f;
typedef math_Vec4f Vec4f;
typedef math_Vec2d Vec2d;
typedef math_Vec3d Vec3d;
typedef math_Vec4d Vec4d;

typedef math_Vec2i Vec2i;
typedef math_Vec3i Vec3i;
typedef math_Vec4i Vec4i;
typedef math_Vec2l Vec2l;
typedef math_Vec3l Vec3l;
typedef math_Vec4l Vec4l;
typedef math_Vec2z Vec2z;
typedef math_Vec3z Vec3z;
typedef math_Vec4z Vec4z;

typedef math_Vec2u  Vec2u;
typedef math_Vec3u  Vec3u;
typedef math_Vec4u  Vec4u;
typedef math_Vec2ul Vec2ul;
typedef math_Vec3ul Vec3ul;
typedef math_Vec4ul Vec4ul;
typedef math_Vec2uz Vec2uz;
typedef math_Vec3uz Vec3uz;
typedef math_Vec4uz Vec4uz;

typedef struct engine_Transform {
    Vec3f position;
    Vec3f rotation;
    Vec3f scale;
} engine_Transform;

#endif /* ENGINE_COMMON_INCLUDED */
