#include "../include/engine/window.h"
#include "../include/engine/canvas.h"
#include "../include/engine/platform_backend.h"

#include <stdlib.h>

ResErr_Ptr_engine_Window engine_Window_create(PtrConst_engine_PlatformParams params) {
    engine_Window* const window = (engine_Window*)malloc(sizeof(engine_Window));
    if (!window) {
        return ResErr_Ptr_engine_Window_err(Err_OutOfMemory);
    }

    const Ptr_engine_Window self = Ptr_engine_Window_from(window);

    // Create platform
    ResErr_Ptr_engine_Platform platform_result = engine_Platform_create(params);
    if (platform_result.tag == ResTag_err) {
        free(window);
        return ResErr_Ptr_engine_Window_err(platform_result.error);
    }

    window->platform   = platform_result.value;
    window->width      = params.addr->width;
    window->height     = params.addr->height;
    window->view_count = 0;

    // Create composite buffer
    ResErr_Ptr_engine_Canvas buffer_result = engine_Canvas_create(
        params.addr->width,
        params.addr->height,
        engine_CanvasType_rgba
    );

    if (buffer_result.tag == ResTag_err) {
        engine_Platform_destroy(window->platform);
        free(window);
        return ResErr_Ptr_engine_Window_err(buffer_result.error);
    }

    window->composite_buffer = buffer_result.value;

    return ResErr_Ptr_engine_Window_ok(self);
}

void engine_Window_destroy(Ptr_engine_Window window) {
    if (!window.addr) { return; }

    if (window.addr->composite_buffer.addr) {
        engine_Canvas_destroy(window.addr->composite_buffer);
    }
    if (window.addr->platform.addr) {
        engine_Platform_destroy(window.addr->platform);
    }
    free(window.addr);
}

void engine_Window_processEvents(Ptr_engine_Window window) {
    debug_assert_nonnull(window.addr);
    debug_assert_nonnull(window.addr->platform.addr);

    if (window.addr->platform.addr->backend && window.addr->platform.addr->backend->processEvents) {
        window.addr->platform.addr->backend->processEvents(window.addr->platform.addr);
    }
}

void engine_Window_present(Ptr_engine_Window window) {
    debug_assert_nonnull(window.addr);
    debug_assert_nonnull(window.addr->platform.addr);
    debug_assert_nonnull(window.addr->composite_buffer.addr);

    // Clear composite buffer
    engine_Canvas_clear(
        window.addr->composite_buffer,
        (engine_ColorValue){ .rgba = { { 0, 0, 0, 255 } } }
    );

    // Compose all visible canvas views
    for (u32 i = 0; i < window.addr->view_count; ++i) {
        engine_CanvasView* const view = &window.addr->views[i];
        if (view->visible && view->canvas.addr) {
            engine_Canvas_blitScaled(
                window.addr->composite_buffer,
                view->canvas,
                view->x,
                view->y,
                as(f32, view->width) / as(f32, view->canvas.addr->width)
            );
        }
    }

    // Present to platform
    if (window.addr->platform.addr->backend && window.addr->platform.addr->backend->presentBuffer) {
        window.addr->platform.addr->backend->presentBuffer(
            window.addr->platform.addr,
            window.addr->composite_buffer.addr->buffer,
            window.addr->width,
            window.addr->height
        );
    }
}

i32 engine_Window_addCanvasView(Ptr_engine_Window window, Ptr_engine_Canvas canvas, i32 x, i32 y, i32 width, i32 height) {
    debug_assert_nonnull(window.addr);
    debug_assert_nonnull(canvas.addr);

    if (window.addr->view_count >= engine_Window_max_canvases) { return -1; }

    engine_CanvasView* const view = &window.addr->views[window.addr->view_count];
    view->x                       = x;
    view->y                       = y;
    view->width                   = width;
    view->height                  = height;
    view->canvas                  = canvas;
    view->visible                 = true;

    return as(i32, window.addr->view_count++);
}

void engine_Window_removeCanvasView(Ptr_engine_Window window, i32 view_id) {
    debug_assert_nonnull(window.addr);

    if (view_id < 0 || as(u32, view_id) >= window.addr->view_count) {
        return;
    }

    // Shift remaining views down
    for (i32 i = view_id; i < as(i32, window.addr->view_count) - 1; i++) {
        window.addr->views[i] = window.addr->views[i + 1];
    }
    window.addr->view_count--;
}

void engine_Window_updateCanvasView(Ptr_engine_Window window, i32 view_id, i32 x, i32 y, i32 width, i32 height) {
    debug_assert_nonnull(window.addr);

    if (view_id < 0 || as(u32, view_id) >= window.addr->view_count) {
        return;
    }

    engine_CanvasView* const view = &window.addr->views[view_id];
    view->x                       = x;
    view->y                       = y;
    view->width                   = width;
    view->height                  = height;
}
