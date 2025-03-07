#include "engine-wip/Window.h"
#include "engine-wip/Canvas.h"
#include "Backend_Internal.h"
#include "dh/log.h"

Err$Ptr$engine_Window engine_Window_init(const engine_WindowConfig* config) {
    scope_reserveReturn(Err$Ptr$engine_Window) {
        debug_assert_nonnull(config);
        /* Create window */
        let allocator = config->allocator;
        let window    = meta_cast$(engine_Window*, try_(mem_Allocator_create(allocator, typeInfo$(engine_Window))));
        errdefer_(mem_Allocator_destroy(allocator, anyPtr(window)));
        window->allocator = allocator;

        /* Create composite buffer */
        let rect_size        = config->rect_size;
        let composite_buffer = try_(engine_Canvas_create(
            rect_size.x,
            rect_size.y,
            engine_CanvasType_rgba
        ));
        errdefer_(engine_Canvas_destroy(composite_buffer));
        window->composite_buffer = composite_buffer;

        /* Set default color */
        composite_buffer->default_color = engine_Window_composite_buffer_color_default;
        if_some(config->default_color, color) {
            if (color.a == ColorChannel_alpha_opaque) {
                composite_buffer->default_color = color;
            }
        }
        engine_Canvas_clearDefault(composite_buffer);

        /* Init canvas views */
        window->views.count = 0;

        /* Reserve backend for init */
        noneAsg(window->backend);

        /* Created successfully */
        return_ok(window);
    }
    scope_returnReserved;
}

void engine_Window_fini(engine_Window* self) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self->composite_buffer);

    engine_Canvas_destroy(self->composite_buffer);
    mem_Allocator_destroy(self->allocator, anyPtr(self));
}

Err$void engine_Window_update(engine_Window* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    engine_Backend_processEvents(unwrap(self->backend));

    /* resize */
    let res = engine_Window_getRes(self);
    for (u32 id = 0; id < self->views.count; ++id) {
        let view = Arr_at(self->views.list, id);
        if (!view->visible) { continue; }
        let size = eval({
            var full = view->rect.size;
            if (view->rect.resizable.x) { full.x = res.x; }
            if (view->rect.resizable.y) { full.y = res.y; }
            eval_return full;
        });
        try_(engine_Canvas_resize(
            view->canvas,
            size.x,
            size.y
        ));
        view->rect.size = size;
    }
    return_void();
}

/* TODO: Apply canvas view rect scale */
void engine_Window_present(engine_Window* self) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self->composite_buffer);

    // Skip presentation if window is minimized
    if (engine_Window_isMinimized(self)) { return; }

    // Clear composite buffer
    engine_Canvas_clearDefault(self->composite_buffer);

    // Compose all visible canvas views
    for (u32 id = 0; id < self->views.count; ++id) {
        let view = Arr_at(self->views.list, id);
        if (!view->visible) { continue; }
        if (!view->canvas) { continue; }

        engine_Canvas_blit(
            self->composite_buffer,
            view->canvas,
            as$(i32, view->pos_on_window.top_left.x),
            as$(i32, view->pos_on_window.top_left.y)
        );
    }

    // Present to platform
    engine_Backend_presentBuffer(unwrap(self->backend));
}

Opt$u32 engine_Window_appendCanvasView(engine_Window* self, engine_Canvas* canvas, Vec2i pos, Vec2u size, Vec2f scale, bool visible, bool resizable_x, bool resizable_y) {
    reserveReturn(Opt$u32);
    debug_assert_nonnull(self);
    debug_assert_nonnull(canvas);

    if (engine_Window_max_canvases <= self->views.count) { return_none(); }
    return_some(eval({
        let view                     = Arr_at(self->views.list, self->views.count);
        view->canvas                 = canvas;
        view->pos_on_window.top_left = pos;
        view->rect.size              = size;
        view->rect.scale             = scale;
        view->rect.resizable.x       = resizable_x;
        view->rect.resizable.y       = resizable_y;
        view->visible                = visible;
        eval_return self->views.count++;
    }));
}

void engine_Window_removeCanvasView(engine_Window* self, u32 view_id) {
    debug_assert_nonnull(self);
    unused(self), unused(view_id); /* TODO: Implement this function */
    /* debug_assert_nonnull(self);
    debug_assert(0 <= view_id);
    debug_assert(view_id < self->views.count);

    // Shift remaining views down
    for (u32 id = view_id; id < self->views.count - 1; ++id) {
        *Arr_at(self->views.list, id) = *Arr_at(self->views.list, id + 1);
    }
    self->views.count--; */
}

Vec2i engine_Window_getPos(const engine_Window* self) {
    debug_assert_nonnull(self);
    return engine_Backend_getWindowPos(unwrap(self->backend));
}

Vec2u engine_Window_getDim(const engine_Window* self) {
    debug_assert_nonnull(self);
    return engine_Backend_getWindowDim(unwrap(self->backend));
}

Vec2u engine_Window_getRes(const engine_Window* self) {
    debug_assert_nonnull(self);
    return engine_Backend_getWindowRes(unwrap(self->backend));
}

Vec2u engine_Window_getMinRes(const engine_Window* self) {
    debug_assert_nonnull(self);
    return engine_Backend_getWindowMinRes(unwrap(self->backend));
}

Vec2u engine_Window_getMaxRes(const engine_Window* self) {
    debug_assert_nonnull(self);
    return engine_Backend_getWindowMaxRes(unwrap(self->backend));
}

Err$void engine_Window_setMinRes(engine_Window* self, Vec2u size) {
    debug_assert_nonnull(self);
    return engine_Backend_setWindowMinRes(unwrap(self->backend), size);
}

Err$void engine_Window_setMaxRes(engine_Window* self, Vec2u size) {
    debug_assert_nonnull(self);
    return engine_Backend_setWindowMaxRes(unwrap(self->backend), size);
}

bool engine_Window_isFocused(const engine_Window* self) {
    debug_assert_nonnull(self);
    return engine_Backend_isWindowFocused(unwrap(self->backend));
}

bool engine_Window_isMinimized(const engine_Window* self) {
    debug_assert_nonnull(self);
    return engine_Backend_isWindowMinimized(unwrap(self->backend));
}

bool engine_Window_isMaximized(const engine_Window* self) {
    debug_assert_nonnull(self);
    return engine_Backend_isWindowMaximized(unwrap(self->backend));
}
