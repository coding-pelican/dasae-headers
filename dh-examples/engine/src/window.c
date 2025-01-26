#include "engine/window.h"
#include "engine/platform_backend.h"
#include "engine/canvas.h"
#include "engine/input.h"
#include "dh/log.h"

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
            window->metrics.min_width     = engine_Window_min_width_default;
            window->metrics.min_height    = engine_Window_min_height_default;
            window->metrics.max_width     = engine_Window_max_width_default;
            window->metrics.max_height    = engine_Window_max_height_default;
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
        composite_buffer->default_color = params->default_color;
        if (composite_buffer->default_color.a != ColorChannel_alpha_opaque) {
            composite_buffer->default_color = engine_Window_composite_buffer_default_color;
        }
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
    /* if_some(backend->getWindowMetrics(window->platform), new_metrics) {
        let applied = eval({
            // Apply constraints
            var m          = new_metrics;
            m.client_width = prim_clamp(
                m.client_width,
                window->metrics.min_width,
                window->metrics.max_width
            );
            m.client_height = prim_clamp(
                m.client_height,
                window->metrics.min_height,
                window->metrics.max_height
            );
            log_debug(
                "Applied constraints: client_width[%d,%d] = %d, client_height[%d,%d] = %d",
                window->metrics.min_width,
                window->metrics.max_width,
                m.client_width,
                window->metrics.min_height,
                window->metrics.max_height,
                m.client_height
            );
            eval_return m;
        });

        // Update metrics with constrained values
        window->metrics.client_width  = applied.client_width;
        window->metrics.client_height = applied.client_height;

        // Force backend to use constrained size
        if (window->platform->backend->type == engine_RenderBackendType_vt100) {
            engine_Win32ConsoleBackend* backend = (engine_Win32ConsoleBackend*)window->platform->backend;
            COORD                       size    = {
                (SHORT)window->metrics.client_width,
                (SHORT)window->metrics.client_height
            };
            SetConsoleScreenBufferSize(backend->output_handle, size);
        }

        if (window->composite_buffer->width != applied.client_width || window->composite_buffer->height != applied.client_height) {
            engine_Canvas_resize(window->composite_buffer, applied.client_width, applied.client_height);
        }
    } */
    return_void();
}

void engine_Window_present(engine_Window* window) {
    debug_assert_nonnull(window);
    debug_assert_nonnull(window->platform);
    debug_assert_nonnull(window->composite_buffer);
    // Skip presentation if window is minimized
    if (window->metrics.is_minimized) { return; }

    // Clear composite buffer.
    engine_Canvas_clearDefault(window->composite_buffer);

    // Compose all visible canvas views
    for (u32 id = 0; id < window->view_count; ++id) {
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

Opt$u32 engine_Window_addCanvasView(engine_Window* window, engine_Canvas* canvas, i32 x, i32 y, u32 width, u32 height) {
    reserveReturn(Opt$u32);
    debug_assert_nonnull(window);
    debug_assert_nonnull(canvas);

    if (engine_Window_max_canvases <= window->view_count) { return_none(); }
    return_some(eval({
        let view      = &window->views[window->view_count++];
        view->x       = x;
        view->y       = y;
        view->width   = width;
        view->height  = height;
        view->canvas  = canvas;
        view->visible = true;
        eval_return window->view_count;
    }));
}

void engine_Window_removeCanvasView(engine_Window* window, u32 view_id) {
    debug_assert_nonnull(window);
    debug_assert(0 <= view_id);
    debug_assert(view_id < window->view_count);

    // Shift remaining views down
    for (u32 id = view_id; id < window->view_count - 1; ++id) {
        window->views[id] = window->views[id + 1];
    }
    window->view_count--;
}

void engine_Window_updateCanvasView(engine_Window* window, u32 view_id, i32 x, i32 y, u32 width, u32 height) {
    debug_assert_nonnull(window);
    debug_assert(0 <= view_id);
    debug_assert(view_id < window->view_count);

    let view     = &window->views[view_id];
    view->x      = x;
    view->y      = y;
    view->width  = width;
    view->height = height;
}
