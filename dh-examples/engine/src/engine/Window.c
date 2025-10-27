#include "engine/Window.h"
#include "engine/Canvas.h"
#include "Backend_Internal.h"

fn_((engine_Window_init(engine_Window_Config config))(E$P$engine_Window) $guard) {
    debug_assert(0 < config.rect_size.x);
    debug_assert(0 < config.rect_size.y);

    /* Create window */
    let allocator = unwrap(config.allocator);
    let window    = meta_cast$(engine_Window*, try_(mem_Allocator_create(allocator, typeInfo$(engine_Window))));
    errdefer_($ignore, mem_Allocator_destroy(allocator, anyPtr(window)));
    window->allocator = allocator;
    /* Create composite buffer */
    let rect_size        = config.rect_size;
    let composite_buffer = try_(engine_Canvas_init((engine_Canvas_Config){
        .allocator     = some(allocator),
        .width         = rect_size.x,
        .height        = rect_size.y,
        .type          = some(engine_CanvasType_rgba),
        .default_color = some(expr_(Color $scope)if_none(config.default_color) {
            $break_(engine_Window_composite_buffer_color_default);
        } else_some(color) {
            $break_(expr_(Color $scope) if (color.a != ColorChannel_alpha_opaque) {
                $break_(engine_Window_composite_buffer_color_default);
            } else {
                $break_(color);
            } $unscoped_(expr));
        } $unscoped_(expr)),
    }));
    errdefer_($ignore, engine_Canvas_fini(composite_buffer));
    window->composite_buffer = composite_buffer;
    /* Init canvas views */
    window->view.count = 0;
    /* Reserve backend for init */
    O_asg(&window->backend, none());

    /* Created successfully */
    return_ok(window);
} $unguarded_(fn);

fn_((engine_Window_fini(engine_Window* self))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self->composite_buffer);

    engine_Canvas_fini(self->composite_buffer);
    mem_Allocator_destroy(self->allocator, anyPtr(self));
}

fn_((engine_Window_update(engine_Window* self))(E$void) $scope) {
    debug_assert_nonnull(self);
    engine_Backend_processEvents(unwrap(self->backend));

    /* resize */
    let res = engine_Window_getRes(self);
    for (u32 id = 0; id < self->view.count; ++id) {
        let view = A_at(self->view.list, id);
        if (!view->visible) { continue; }
        let size = blk({
            var full = view->rect.size;
            if (view->rect.resizable.x) { full.x = res.x; }
            if (view->rect.resizable.y) { full.y = res.y; }
            blk_return full;
        });
        try_(engine_Canvas_resize(
            view->canvas,
            size.x,
            size.y
        ));
        view->rect.size = size;
    }
    return_ok({});
} $unscoped_(fn);

/* TODO: Apply canvas view rect scale */
fn_((engine_Window_present(engine_Window* self))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(self->composite_buffer);

    // Skip presentation if window is minimized
    if (engine_Window_isMinimized(self)) { return; }

    // Clear composite buffer
    engine_Canvas_clear(self->composite_buffer, none$(O$Color));

    // Compose all visible canvas views
    for (u32 id = 0; id < self->view.count; ++id) {
        let view = A_at(self->view.list, id);
        if (!view->visible) { continue; }
        if (!view->canvas) { continue; }

        engine_Canvas_blit(
            self->composite_buffer,
            view->canvas,
            as$((i32)(view->pos_on_window.top_left.x)),
            as$((i32)(view->pos_on_window.top_left.y))
        );
    }

    // Present to platform
    engine_Backend_presentBuffer(unwrap(self->backend));
}

fn_((engine_Window_appendView(engine_Window* self, engine_CanvasView_Config config))(O$u32) $scope) {
    debug_assert_nonnull(self);

    if (engine_Window_view_count_limit <= self->view.count) { return_none(); }
    return_some(blk({
        let view                     = A_at(self->view.list, self->view.count);
        view->canvas                 = config.canvas;
        view->pos_on_window.top_left = config.pos;
        view->rect.size              = config.size;
        view->rect.scale             = config.scale;
        view->rect.resizable.x       = config.resizable_x;
        view->rect.resizable.y       = config.resizable_y;
        view->visible                = config.visible;
        blk_return self->view.count++;
    }));
} $unscoped_(fn);

fn_((engine_Window_removeView(engine_Window* self, u32 view_id))(void)) {
    debug_assert_nonnull(self);
    $unused(self), $unused(view_id); /* TODO: Implement this function */
    /* debug_assert_nonnull(self);
    debug_assert(0 <= view_id);
    debug_assert(view_id < self->views.count);

    // Shift remaining views down
    for (u32 id = view_id; id < self->views.count - 1; ++id) {
        *A_at(self->views.list, id) = *A_at(self->views.list, id + 1);
    }
    self->views.count--; */
}

fn_((engine_Window_getPos(const engine_Window* self))(Vec2i)) {
    debug_assert_nonnull(self);
    return engine_Backend_getWindowPos(unwrap(self->backend));
}

fn_((engine_Window_getDim(const engine_Window* self))(Vec2u)) {
    debug_assert_nonnull(self);
    return engine_Backend_getWindowDim(unwrap(self->backend));
}

fn_((engine_Window_getRes(const engine_Window* self))(Vec2u)) {
    debug_assert_nonnull(self);
    return engine_Backend_getWindowRes(unwrap(self->backend));
}

fn_((engine_Window_getMinRes(const engine_Window* self))(Vec2u)) {
    debug_assert_nonnull(self);
    return engine_Backend_getWindowMinRes(unwrap(self->backend));
}

fn_((engine_Window_getMaxRes(const engine_Window* self))(Vec2u)) {
    debug_assert_nonnull(self);
    return engine_Backend_getWindowMaxRes(unwrap(self->backend));
}

fn_((engine_Window_setMinRes(engine_Window* self, Vec2u size))(E$void)) {
    debug_assert_nonnull(self);
    return engine_Backend_setWindowMinRes(unwrap(self->backend), size);
}

fn_((engine_Window_setMaxRes(engine_Window* self, Vec2u size))(E$void)) {
    debug_assert_nonnull(self);
    return engine_Backend_setWindowMaxRes(unwrap(self->backend), size);
}

fn_((engine_Window_isFocused(const engine_Window* self))(bool)) {
    debug_assert_nonnull(self);
    return engine_Backend_isWindowFocused(unwrap(self->backend));
}

fn_((engine_Window_isMinimized(const engine_Window* self))(bool)) {
    debug_assert_nonnull(self);
    return engine_Backend_isWindowMinimized(unwrap(self->backend));
}

fn_((engine_Window_isMaximized(const engine_Window* self))(bool)) {
    debug_assert_nonnull(self);
    return engine_Backend_isWindowMaximized(unwrap(self->backend));
}
