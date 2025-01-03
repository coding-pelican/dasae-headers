#include "../include/engine/viewport.h"
#include "dh/defer.h"

#include <math.h>

Err$Ptr$engine_Viewport engine_Viewport_create(f32 width, f32 height) {
    reserveReturn(Err$Ptr$engine_Viewport);
    scope_defer {
        /* Create viewport */
        let viewport = (engine_Viewport*)malloc(sizeof(engine_Viewport));
        if (!viewport) {
            defer_return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }
        errdefer(free(viewport));

        /* Initialize viewport */ {
            viewport->width  = width;
            viewport->height = height;
        }

        /* Initialize camera */ {
            viewport->camera.position      = (engine_Vec3f32){ { 0.0f, 0.0f, -10.0f } };
            viewport->camera.rotation      = (engine_Vec3f32){ { 0.0f, 0.0f, 0.0f } };
            viewport->camera.field_of_view = 60.0f * (3.14159f / 180.0f); // 60 degrees in radians
            viewport->camera.near_plane    = 0.1f;
            viewport->camera.far_plane     = 1000.0f;
        }

        /* Allocate depth buffer */
        let depth_buffer = malloc(sizeof(f32) * as(usize, width * height));
        if (!depth_buffer) {
            defer_return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }
        errdefer(free(depth_buffer));
        viewport->depth_buffer = depth_buffer;

        /* Created successfully */
        defer_return_ok(viewport);
    }
    return_deferred;
}

void engine_Viewport_destroy(engine_Viewport* viewport) {
    if (!viewport) { return; }

    if (viewport->depth_buffer) {
        free(viewport->depth_buffer);
    }

    free(viewport);
}

void engine_Viewport_resize(engine_Viewport* viewport, f32 width, f32 height) {
    debug_assert_nonnull(viewport);

    viewport->width  = width;
    viewport->height = height;

    /* Reallocate depth buffer */
    let new_buffer = (f32*)realloc(
        viewport->depth_buffer,
        sizeof(f32) * as(usize, width * height)
    );
    if (!new_buffer) { return; }

    viewport->depth_buffer = new_buffer;
}

void engine_Camera_setPosition(engine_Camera* camera, engine_Vec3f32 position) {
    debug_assert_nonnull(camera);
    camera->position = position;
}

void engine_Camera_lookAt(engine_Camera* camera, engine_Vec3f32 target) {
    debug_assert_nonnull(camera);

    /* Calculate direction vector */
    engine_Vec3f32 direction = cleared();
    for (i32 i = 0; i < 3; ++i) {
        direction.xyz[i] = target.xyz[i] - camera->position.xyz[i];
    }

    /* Calculate rotation angles */ {
        // Yaw (around Y axis)
        camera->rotation.y = atan2f(direction.x, direction.z);

        // Pitch (around X axis)
        const f32 horizontal_length = sqrtf(direction.x * direction.x + direction.z * direction.z);
        camera->rotation.x          = atan2f(-direction.y, horizontal_length);

        // Roll (around Z axis) remains unchanged
        camera->rotation.z = 0.0f;
    }
}
