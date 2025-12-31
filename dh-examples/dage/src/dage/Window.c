#include "dage/Window.h"

T_use$((dage_Event)(
    ArrDeq_init,
    ArrDeq_fini,
    ArrDeq_len,
    ArrDeq_shift,
    ArrDeq_appendWithin,
    ArrDeq_front,
    ArrDeq_clearRetainingCap
));
T_use_E$($set(mem_Err)(P$dage_Viewport));
T_use$((dage_Viewport)(
    ArrList_fixed,
    ArrList_init,
    ArrList_fini,
    ArrList_addBackFixed,
    ArrList_removeSwap
));

/*========== Lifecycle ==========*/

fn_((dage_Window_init(dage_Window_Cfg cfg))(E$dage_Window) $guard) {
    debug_assert(cfg.size.x > 0);
    debug_assert(cfg.size.y > 0);

    let gpa = mem_Allocator_ensureValid(cfg.gpa);

    dage_Window window = cleared();
    window.gpa = gpa;
    window.id = dage_WindowId_invalid;
    window.target_id = dage_TargetId_invalid;

    /* Initialize configuration */
    window.cfg.clear_color = orelse_((cfg.clear_color)(color_RGBA_fromOpaque(0x18, 0x18, 0x18)));
    window.cfg.min_size = orelse_((cfg.min_size)((m_V2u32){ .x = 16, .y = 16 }));
    window.cfg.max_size = orelse_((cfg.max_size)((m_V2u32){ .x = 4096, .y = 2048 }));

    /* Initialize event queue */
    window.event_que = try_(ArrDeq_init$dage_Event(gpa, dage_Window_event_queue_capacity));
    errdefer_($ignore, ArrDeq_fini$dage_Event(&window.event_que, gpa));

    /* Initialize input state */
    dage_InputState_init(&window.input);

    /* Create composite buffer */
    let composite_buf = u_castP$((P$dage_Canvas)(try_(mem_Allocator_create(gpa, typeInfo$(dage_Canvas)))));
    errdefer_($ignore, mem_Allocator_destroy(gpa, u_anyP(composite_buf)));

    *composite_buf = try_(dage_Canvas_init((dage_Canvas_Cfg){
        .gpa = gpa,
        .width = cfg.size.x,
        .height = cfg.size.y,
        .type = dage_CanvasType_rgba,
        .default_color = some(window.cfg.clear_color),
    }));
    errdefer_($ignore, dage_Canvas_fini(composite_buf, gpa));

    window.composite_buf = composite_buf;

    /* Initialize viewport list */
    window.viewports_buf = cfg.viewports_buf;
    window.viewports = expr_(ArrList$dage_Viewport $scope)(if_some((window.viewports_buf)(buf)) {
        $break_(ArrList_fixed$dage_Viewport(buf));
    } else_none {
        $break_(try_(ArrList_init$dage_Viewport(gpa, dage_Window_viewport_limit)));
    }) $unscoped_(expr);

    return_ok(window);
} $unguarded_(fn);

fn_((dage_Window_fini(dage_Window* self))(void)) {
    claim_assert_nonnull(self);

    /* Free viewport list */
    if_none(self->viewports_buf) {
        ArrList_fini$dage_Viewport(&self->viewports, self->gpa);
    }

    /* Free composite buffer */
    dage_Canvas_fini(self->composite_buf, self->gpa);
    mem_Allocator_destroy(self->gpa, u_anyP(self->composite_buf));

    /* Free event queue */
    ArrDeq_fini$dage_Event(&self->event_que, self->gpa);

    /* Clear state */
    asg_lit((self)(cleared()));
};

/*========== Event Processing ==========*/

fn_((dage_Window_pushEvent(dage_Window* self, dage_Event event))(void)) {
    claim_assert_nonnull(self);
    /* Try to push - if queue full, drop oldest events */
    if (ArrDeq_len$dage_Event(self->event_que) >= dage_Window_event_queue_capacity) {
        /* Drop oldest */
        let_ignore = ArrDeq_shift$dage_Event(&self->event_que);
    }
    ArrDeq_appendWithin$dage_Event(&self->event_que, event);
};

fn_((dage_Window_pollEvent(dage_Window* self))(O$dage_Event) $scope) {
    claim_assert_nonnull(self);
    if (ArrDeq_len$dage_Event(self->event_que) == 0) {
        return_none();
    }
    return_(ArrDeq_shift$dage_Event(&self->event_que));
} $unscoped_(fn);

fn_((dage_Window_peekEvent(const dage_Window* self))(O$dage_Event) $scope) {
    claim_assert_nonnull(self);
    if (ArrDeq_len$dage_Event(self->event_que) == 0) {
        return_none();
    }
    return_some(*unwrap_(ArrDeq_front$dage_Event(self->event_que)));
} $unscoped_(fn);

fn_((dage_Window_clearEvents(dage_Window* self))(void)) {
    claim_assert_nonnull(self);
    ArrDeq_clearRetainingCap$dage_Event(&self->event_que);
};

/*========== Viewport Management ==========*/

fn_((dage_Window_addViewport(dage_Window* self, dage_Viewport_Cfg cfg))(O$dage_ViewportId) $scope) {
    claim_assert_nonnull(self);

    let slot = catch_((ArrList_addBackFixed$dage_Viewport(&self->viewports))($ignore, {
        return_none();
    }));

    let viewport = dage_Viewport_init(cfg);
    let id = as$(dage_ViewportId)(self->viewports.items.len);
    *slot = viewport;

    return_some(id);
} $unscoped_(fn);

fn_((dage_Window_removeViewport(dage_Window* self, dage_ViewportId id))(void)) {
    claim_assert_nonnull(self);
    if (id >= self->viewports.items.len) {
        return; /* Invalid ID - ignore */
    }
    /* Swap with last and shrink */
    ArrList_removeSwap$dage_Viewport(&self->viewports, id);
};

fn_((dage_Window_getViewport(dage_Window* self, dage_ViewportId id))(O$P$dage_Viewport) $scope) {
    claim_assert_nonnull(self);
    if (id >= self->viewports.items.len) {
        return_none();
    }
    return_some(S_at((self->viewports.items)[id]));
} $unscoped_(fn);

fn_((dage_Window_getMouseCanvasPos(const dage_Window* self, dage_ViewportId viewport_id))(O$m_V2i32) $scope) {
    claim_assert_nonnull(self);
    if (viewport_id >= self->viewports.items.len) {
        return_none();
    }
    let vp = S_at((self->viewports.items)[viewport_id]);
    let mouse_pos = dage_InputState_getMousePos(&self->input);
    return dage_Viewport_windowToCanvas(vp, mouse_pos);
} $unscoped_(fn);

/*========== Composition ==========*/

#if UNUSED_CODE
/**
 * @brief Compare viewports by z_order for sorting
 */
$static fn_((viewport_compare_z(const void* a, const void* b))(i32)) {
    let va = (const dage_Viewport*)a;
    let vb = (const dage_Viewport*)b;
    return va->z_order - vb->z_order;
}
#endif /* UNUSED_CODE */

fn_((dage_Window_composite(dage_Window* self))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->composite_buf);

    /* Clear composite buffer */
    dage_Canvas_clear(self->composite_buf, some$((O$color_RGBA)(self->cfg.clear_color)));

#if UNUSED_CODE
    /* Sort viewports by z_order (simple insertion sort for small N) */
    /* Note: In production, might want to keep sorted or use indices */
    if (self->viewports.items.len > 1) {
        qsort(
            self->viewports.items.ptr,
            self->viewports.items.len,
            sizeof(dage_Viewport),
            viewport_compare_z
        );
    }
#endif /* UNUSED_CODE */

    /* Blit each visible viewport */
    for_(($s(self->viewports.items))(vp) {
        if (!vp->visible) { continue; }
        /* Simple blit for now - ignores fit mode, rotation, opacity */
        /* TODO: Implement proper transform in Viewport.c */
        dage_Canvas_blit(
            self->composite_buf,
            vp->canvas,
            vp->dst.pos.x,
            vp->dst.pos.y
        );
    });
};

/*========== Frame Lifecycle ==========*/

#if UNUSED_CODE
fn_((dage_Window_beginFrame(dage_Window* self))(void)) {
    claim_assert_nonnull(self);
    /* Currently nothing to do at frame start */
    /* Reserved for future use (e.g., timestamp capture) */
};
#endif /* UNUSED_CODE */

fn_((dage_Window_endFrame(dage_Window* self))(void)) {
    claim_assert_nonnull(self);
    dage_InputState_endFrame(&self->input);
};

fn_((dage_Window_handleResize(dage_Window* self, m_V2u32 new_size))(E$void) $scope) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->composite_buf);

    /* Clamp to limits */
    let clamped_w = prim_clamp(new_size.x, self->cfg.min_size.x, self->cfg.max_size.x);
    let clamped_h = prim_clamp(new_size.y, self->cfg.min_size.y, self->cfg.max_size.y);

    /* Skip if no change */
    let current_size = dage_Window_getSize(self);
    if (clamped_w == current_size.x && clamped_h == current_size.y) {
        return_ok({});
    }

    /* Resize composite buffer */
    try_(dage_Canvas_resize(self->composite_buf, self->gpa, clamped_w, clamped_h));

    return_ok({});
} $unscoped_(fn);
