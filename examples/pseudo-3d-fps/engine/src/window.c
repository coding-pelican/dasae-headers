#include "../include/engine/window.h"
#include "../include/engine/canvas.h"
#include "../include/engine/platform_backend.h"

#include <stdlib.h>

Err$Ptr$engine_Window engine_Window_create(const engine_PlatformParams* params) {
    reserveReturn(Err$Ptr$engine_Window);
    engine_Window* const window = (engine_Window*)malloc(sizeof(engine_Window));
    if (!window) {
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }

    // Create platform
    Ptr$engine_Platform platform_result = catch (engine_Platform_create(params), err, {
        free(window);
        return_err(err);
    });

    window->platform   = platform_result;
    window->width      = params->width;
    window->height     = params->height;
    window->view_count = 0;

    // Create composite buffer
    Ptr$engine_Canvas buffer_result = catch (
        engine_Canvas_create(params->width, params->height, engine_CanvasType_rgba), err, {
            engine_Platform_destroy(window->platform);
            free(window);
            return_err(err);
        }
    );
    window->composite_buffer = buffer_result;
    return_ok(window);
}

void engine_Window_destroy(engine_Window* window) {
    if (!window) { return; }

    if (window->composite_buffer) {
        engine_Canvas_destroy(window->composite_buffer);
    }
    if (window->platform) {
        engine_Platform_destroy(window->platform);
    }
    free(window);
}

void engine_Window_processEvents(engine_Window* window) {
    debug_assert_nonnull(window);
    debug_assert_nonnull(window->platform);

    if (window->platform->backend && window->platform->backend->processEvents) {
        window->platform->backend->processEvents(window->platform);
    }
}

void engine_Window_present(engine_Window* window) {
    debug_assert_nonnull(window);
    debug_assert_nonnull(window->platform);
    debug_assert_nonnull(window->composite_buffer);

    // Clear composite buffer
    engine_Canvas_clear(
        window->composite_buffer,
        (engine_ColorValue){ .rgba = { { 0, 0, 0, 255 } } }
    );

    // Compose all visible canvas views
    for (u32 i = 0; i < window->view_count; ++i) {
        engine_CanvasView* const view = &window->views[i];
        if (view->visible && view->canvas) {
            engine_Canvas_blitScaled(
                window->composite_buffer,
                view->canvas,
                view->x,
                view->y,
                as(f32, view->width) / as(f32, view->canvas->width)
            );
        }
    }

    // Present to platform
    if (window->platform->backend && window->platform->backend->presentBuffer) {
        window->platform->backend->presentBuffer(
            window->platform,
            window->composite_buffer->buffer,
            window->width,
            window->height
        );
    }
}

i32 engine_Window_addCanvasView(engine_Window* window, engine_Canvas* canvas, i32 x, i32 y, i32 width, i32 height) {
    debug_assert_nonnull(window);
    debug_assert_nonnull(canvas);

    if (window->view_count >= engine_Window_max_canvases) { return -1; }

    engine_CanvasView* const view = &window->views[window->view_count];
    view->x                       = x;
    view->y                       = y;
    view->width                   = width;
    view->height                  = height;
    view->canvas                  = canvas;
    view->visible                 = true;

    return as(i32, window->view_count++);
}

void engine_Window_removeCanvasView(engine_Window* window, i32 view_id) {
    debug_assert_nonnull(window);

    if (view_id < 0 || as(u32, view_id) >= window->view_count) {
        return;
    }

    // Shift remaining views down
    for (i32 i = view_id; i < as(i32, window->view_count) - 1; i++) {
        window->views[i] = window->views[i + 1];
    }
    window->view_count--;
}

void engine_Window_updateCanvasView(engine_Window* window, i32 view_id, i32 x, i32 y, i32 width, i32 height) {
    debug_assert_nonnull(window);

    if (view_id < 0 || as(u32, view_id) >= window->view_count) {
        return;
    }

    engine_CanvasView* const view = &window->views[view_id];
    view->x                       = x;
    view->y                       = y;
    view->width                   = width;
    view->height                  = height;
}
