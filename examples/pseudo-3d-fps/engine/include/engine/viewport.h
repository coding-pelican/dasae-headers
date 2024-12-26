#ifndef ENGINE_VIEWPORT_INCLUDED
#define ENGINE_VIEWPORT_INCLUDED (1)

#include "world.h"

typedef struct engine_Viewport engine_Viewport;
typedef struct engine_Camera   engine_Camera;

typedef struct engine_Camera {
    engine_Vec3f32 position;
    engine_Vec3f32 rotation;
    f32            field_of_view;
    f32            near_plane;
    f32            far_plane;
} engine_Camera;
using_ExtTypes(engine_Camera);

typedef struct engine_Viewport {
    f32           width;
    f32           height;
    engine_Camera camera;
    f32*          depth_buffer;
} engine_Viewport;
using_ExtTypes(engine_Viewport);

// Viewport and Camera functions
ResErr_Ptr_engine_Viewport engine_Viewport_create(f32 width, f32 height);
void                       engine_Viewport_destroy(Ptr_engine_Viewport viewport);
void                       engine_Viewport_resize(Ptr_engine_Viewport viewport, f32 width, f32 height);
void                       engine_Camera_setPosition(Ptr_engine_Camera camera, engine_Vec3f32 position);
void                       engine_Camera_lookAt(Ptr_engine_Camera camera, engine_Vec3f32 target);

#endif // ENGINE_VIEWPORT_INCLUDED
