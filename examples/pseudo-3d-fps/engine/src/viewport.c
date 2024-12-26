#include "../include/engine/viewport.h"

#include <math.h>

ResErr_Ptr_engine_Viewport engine_Viewport_create(f32 width, f32 height) {
    engine_Viewport* const viewport = (engine_Viewport*)malloc(sizeof(engine_Viewport));
    if (!viewport) {
        return ResErr_Ptr_engine_Viewport_err(Err_OutOfMemory);
    }

    const Ptr_engine_Viewport self = Ptr_engine_Viewport_from(viewport);
    viewport->width                = width;
    viewport->height               = height;

    // Initialize camera
    viewport->camera.position      = (engine_Vec3f32){ { 0.0f, 0.0f, -10.0f } };
    viewport->camera.rotation      = (engine_Vec3f32){ { 0.0f, 0.0f, 0.0f } };
    viewport->camera.field_of_view = 60.0f * (3.14159f / 180.0f); // 60 degrees in radians
    viewport->camera.near_plane    = 0.1f;
    viewport->camera.far_plane     = 1000.0f;

    // Allocate depth buffer
    viewport->depth_buffer = malloc(sizeof(f32) * as(usize, width * height));
    if (!viewport->depth_buffer) {
        free(viewport);
        return ResErr_Ptr_engine_Viewport_err(Err_OutOfMemory);
    }

    return ResErr_Ptr_engine_Viewport_ok(self);
}

void engine_Viewport_destroy(Ptr_engine_Viewport viewport) {
    if (!viewport.addr) { return; }

    if (viewport.addr->depth_buffer) {
        free(viewport.addr->depth_buffer);
    }
    free(viewport.addr);
}

void engine_Viewport_resize(Ptr_engine_Viewport viewport, f32 width, f32 height) {
    debug_assert_nonnull(viewport.addr);

    viewport.addr->width  = width;
    viewport.addr->height = height;

    // Reallocate depth buffer
    f32* const new_buffer = realloc(
        viewport.addr->depth_buffer,
        sizeof(f32) * as(usize, width * height)
    );

    if (new_buffer) {
        viewport.addr->depth_buffer = new_buffer;
    }
}

void engine_Camera_setPosition(Ptr_engine_Camera camera, engine_Vec3f32 position) {
    debug_assert_nonnull(camera.addr);
    camera.addr->position = position;
}

void engine_Camera_lookAt(Ptr_engine_Camera camera, engine_Vec3f32 target) {
    debug_assert_nonnull(camera.addr);

    // Calculate direction vector
    engine_Vec3f32 direction;
    for (int i = 0; i < 3; ++i) {
        direction.xyz[i] = target.xyz[i] - camera.addr->position.xyz[i];
    }

    // Calculate rotation angles
    // Yaw (around Y axis)
    camera.addr->rotation.y = atan2f(direction.x, direction.z);

    // Pitch (around X axis)
    const f32 horizontal_length = sqrtf(direction.x * direction.x + direction.z * direction.z);
    camera.addr->rotation.x     = atan2f(-direction.y, horizontal_length);

    // Roll (around Z axis) remains unchanged
    camera.addr->rotation.z = 0.0f;
}
