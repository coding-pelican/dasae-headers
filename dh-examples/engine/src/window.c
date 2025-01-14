#include "engine/window.h"
#include "engine/platform_backend.h"
#include "engine/canvas.h"
#include "engine/input.h"

#include <stdlib.h>

bool engine_WindowMetrics_eq(const engine_WindowMetrics* lhs, const engine_WindowMetrics* rhs) {
    debug_assert_nonnull(lhs);
    debug_assert_nonnull(rhs);

    return lhs->x == rhs->x
        && lhs->y == rhs->y
        && lhs->width == rhs->width
        && lhs->height == rhs->height
        && lhs->client_width == rhs->client_width
        && lhs->client_height == rhs->client_height
        && lhs->is_focused == rhs->is_focused
        && lhs->is_minimized == rhs->is_minimized
        && lhs->is_maximized == rhs->is_maximized;
}

Err$Ptr$engine_Window engine_Window_create(const engine_PlatformParams* params) {
    scope_reserveReturn(Err$Ptr$engine_Window) {
        debug_assert_nonnull(params);

        /* Create window */
        let window = (engine_Window*)malloc(sizeof(engine_Window));
        if (!window) {
            return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }
        errdefer(free(window));

        /* Create platform */
        scope_with(let platform = try(engine_Platform_create(params))) {
            errdefer(engine_Platform_destroy(platform));
            window->platform = platform;

            /* Initialize window metrics */
            window->metrics.width         = params->width;
            window->metrics.height        = params->height;
            window->metrics.client_width  = params->width;
            window->metrics.client_height = params->height;
            window->metrics.x             = 0;
            window->metrics.y             = 0;
            window->metrics.is_focused    = true;
            window->metrics.is_minimized  = false;
            window->metrics.is_maximized  = false;

            window->view_count = 0;
        }

        /* Create composite buffer */
        let composite_buffer = try(engine_Canvas_create(
            window->metrics.client_width,
            window->metrics.client_height,
            engine_CanvasType_rgba
        ));
        errdefer(engine_Canvas_destroy(composite_buffer));
        window->composite_buffer = composite_buffer;

        /* Initialize input */
        engine_Input_init();

        /* Created successfully */
        return_ok(window);
    }
    scope_returnReserved;
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

Err$void engine_Window_processEvents(engine_Window* window) {
    reserveReturn(Err$void);
    debug_assert_nonnull(window);
    debug_assert_nonnull(window->platform);

    engine_Input_update();

    let backend = window->platform->backend;
    if (!backend) { return_void(); }
    debug_assert_nonnull(backend->processEvents);
    debug_assert_nonnull(backend->getWindowMetrics);

    backend->processEvents(window->platform);
    if_some(backend->getWindowMetrics(window->platform), new_metrics) {
        usize w        = window->metrics.width;
        usize h        = window->metrics.height;
        usize client_w = window->metrics.client_width;
        usize client_h = window->metrics.client_height;

        window->metrics = new_metrics;

        window->metrics.width         = w;
        window->metrics.height        = h;
        window->metrics.client_width  = client_w;
        window->metrics.client_height = client_h;
    }

    return_void();
}

void engine_Window_present(engine_Window* window) {
    debug_assert_nonnull(window);
    debug_assert_nonnull(window->platform);
    debug_assert_nonnull(window->composite_buffer);
    // Skip presentation if window is minimized
    if (window->metrics.is_minimized) { return; }

    // Clear composite buffer
    engine_Canvas_clear(window->composite_buffer, Color_blue);

    // Compose all visible canvas views
    for (usize id = 0; id < window->view_count; ++id) {
        let view = &window->views[id];
        if (!view->visible) { continue; }
        if (!view->canvas) { continue; }

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
    debug_assert_nonnull(backend->presentBuffer);

    backend->presentBuffer(
        window->platform,
        window->composite_buffer->buffer.ptr,
        window->metrics.client_width,
        window->metrics.client_height
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
