#include "../include/engine/window.h"
#include "../include/engine/canvas.h"
#include "../include/engine/platform_backend.h"
#include "../include/engine/input.h"
#include "dh/defer.h"

#include <stdlib.h>

Err$Ptr$engine_Window engine_Window_create(const engine_PlatformParams* params) {
    reserveReturn(Err$Ptr$engine_Window);
    scope_defer {
        /* Create window */
        let window = (engine_Window*)malloc(sizeof(engine_Window));
        if (!window) {
            defer_return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }
        errdefer(free(window));

        /* Create platform */
        scope_with(let platform = try_defer(engine_Platform_create(params))) {
            errdefer(engine_Platform_destroy(platform));
            window->platform   = platform;
            window->width      = params->width;
            window->height     = params->height;
            window->view_count = 0;
        }

        /* Create composite buffer */
        let composite_buffer = try_defer(engine_Canvas_create(
            params->width,
            params->height,
            engine_CanvasType_rgba
        ));
        errdefer(engine_Canvas_destroy(composite_buffer));
        window->composite_buffer = composite_buffer;

        /* Initialize input */
        engine_Input_init();

        /* Created successfully */
        defer_return_ok(window);
    }
    return_deferred;
}

void engine_Window_destroy(engine_Window* window) {
    if (!window) { return; }

    if (window->composite_buffer) {
        engine_Canvas_destroy(window->composite_buffer);
    }
    if (window->platform) {
        engine_Platform_destroy(window->platform);
    }
    engine_Input_fini();

    free(window);
}

void engine_Window_processEvents(engine_Window* window) {
    debug_assert_nonnull(window);
    debug_assert_nonnull(window->platform);

    engine_Input_update();

    let backend = window->platform->backend;
    if (!backend) { return; }
    if (!backend->processEvents) { return; }

    backend->processEvents(window->platform);
}

void engine_Window_present(engine_Window* window) {
    debug_assert_nonnull(window);
    debug_assert_nonnull(window->platform);
    debug_assert_nonnull(window->composite_buffer);

    // Clear composite buffer
    engine_Canvas_clear(window->composite_buffer, Color_blue);

    // Compose all visible canvas views
    for (usize id = 0; id < window->view_count; ++id) {
        let view = &window->views[id];
        if (!view->visible) { continue; }
        if (!view->canvas) { continue; }

        /* engine_Canvas_blitScaled(
            window->composite_buffer,
            view->canvas,
            view->x,
            view->y,
            as(f32, view->width) / as(f32, view->canvas->width)
        ); */
        engine_Canvas_blit(
            window->composite_buffer,
            view->canvas,
            view->x,
            view->y
        );
    }

    // Present to platform
    let backend = window->platform->backend;
    if (!backend) { return; }
    if (!backend->presentBuffer) { return; }

    backend->presentBuffer(
        window->platform,
        window->composite_buffer->buffer.ptr,
        window->width,
        window->height
    );
}

i32 engine_Window_addCanvasView(engine_Window* window, engine_Canvas* canvas, i32 x, i32 y, i32 width, i32 height) {
    debug_assert_nonnull(window);
    debug_assert_nonnull(canvas);

    if (engine_Window_max_canvases <= window->view_count) { return -1; }

    let view      = &window->views[window->view_count];
    view->x       = x;
    view->y       = y;
    view->width   = width;
    view->height  = height;
    view->canvas  = canvas;
    view->visible = true;

    return as(i32, window->view_count++);
}

void engine_Window_removeCanvasView(engine_Window* window, i32 view_id) {
    debug_assert_nonnull(window);
    if (view_id < 0 || window->view_count <= as(u32, view_id)) { return; }

    // Shift remaining views down
    for (i32 id = view_id; id < as(i32, window->view_count) - 1; ++id) {
        window->views[id] = window->views[id + 1];
    }
    window->view_count--;
}

void engine_Window_updateCanvasView(engine_Window* window, i32 view_id, i32 x, i32 y, i32 width, i32 height) {
    debug_assert_nonnull(window);
    if (view_id < 0 || window->view_count <= as(u32, view_id)) { return; }

    let view     = &window->views[view_id];
    view->x      = x;
    view->y      = y;
    view->width  = width;
    view->height = height;
}
