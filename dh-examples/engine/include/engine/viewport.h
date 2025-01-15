#ifndef ENGINE_VIEWPORT_INCLUDED
#define ENGINE_VIEWPORT_INCLUDED (1)

#include "world.h"

typedef struct engine_Viewport engine_Viewport;
typedef struct engine_Camera   engine_Camera;

typedef struct engine_Camera {
    Vec3f position;
    Vec3f rotation;
    f32   field_of_view;
    f32   near_plane;
    f32   far_plane;
} engine_Camera;

typedef struct engine_Viewport {
    f32           width;
    f32           height;
    engine_Camera camera;
    f32*          depth_buffer;
} engine_Viewport;
use_Ptr$(engine_Viewport);
use_Err$(engine_Viewport);

// Viewport and Camera functions
extern Err$Ptr$engine_Viewport engine_Viewport_create(f32 width, f32 height) must_check;
extern void                    engine_Viewport_destroy(engine_Viewport* viewport);
extern void                    engine_Viewport_resize(engine_Viewport* viewport, f32 width, f32 height);
extern void                    engine_Camera_setPosition(engine_Camera* camera, Vec3f position);
extern void                    engine_Camera_lookAt(engine_Camera* camera, Vec3f target);

#endif /* ENGINE_VIEWPORT_INCLUDED */
