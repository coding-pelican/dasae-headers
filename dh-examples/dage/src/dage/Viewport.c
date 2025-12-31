#include "dage/Viewport.h"
#include "dage/Canvas.h"

/*========== Initialization ==========*/

$attr($inline_always)
$static fn_((dage_Viewport__defaultSrcRect(dage_Canvas* canvas))(dage_Rect_i32));
fn_((dage_Viewport_init(dage_Viewport_Cfg cfg))(dage_Viewport)) {
    claim_assert_nonnull(cfg.canvas);

    /* Set source rect - default to full canvas */
    return (dage_Viewport){
        .canvas = cfg.canvas,
        .src = expr_(dage_Rect_i32 $scope)(if_some((cfg.src_rect)(src)) {
            $break_(src);
        } else_none {
            $break_(dage_Viewport__defaultSrcRect(cfg.canvas));
        }) $unscoped_(expr),
        .dst = cfg.dst_rect,
        .fit = cfg.fit,
        .opacity = cfg.opacity == 0.0f ? 1.0f : cfg.opacity,
        .rotation = 0.0f,
        .z_order = cfg.z_order,
        .visible = cfg.visible,
    };
};

fn_((dage_Viewport__defaultSrcRect(dage_Canvas* canvas))(dage_Rect_i32)) {
    return (dage_Rect_i32){
        .pos = m_V2i32_zero,
        .size = (m_V2u32){
            .x = Grid_width(canvas->gird),
            .y = Grid_height(canvas->gird),
        },
    };
};

/*========== Coordinate Transformation ==========*/

fn_((dage_Viewport_canvasToWindow(const dage_Viewport* self, m_V2i32 canvas_pos))(m_V2i32)) {
    debug_assert_nonnull(self);

    let scale = dage_Viewport_getEffectiveScale(self);

    /* Transform: canvas → viewport-local → window */
    /* 1. Subtract source origin */
    let local_x = as$(f32)(canvas_pos.x - self->src.pos.x);
    let local_y = as$(f32)(canvas_pos.y - self->src.pos.y);

    /* 2. Apply scale */
    let scaled_x = local_x * scale.x;
    let scaled_y = local_y * scale.y;

    /* 3. Add destination origin */
    return m_V2i32_of(
        self->dst.pos.x + as$(i32)(scaled_x),
        self->dst.pos.y + as$(i32)(scaled_y)
    );
};

fn_((dage_Viewport_windowToCanvas(const dage_Viewport* self, m_V2i32 window_pos))(O$m_V2i32) $scope) {
    debug_assert_nonnull(self);

    /* Check if point is inside viewport bounds */
    if (!dage_Viewport_containsPoint(self, window_pos)) {
        return_none();
    }

    let scale = dage_Viewport_getEffectiveScale(self);

    /* Avoid division by zero */
    if (scale.x == 0.0f || scale.y == 0.0f) {
        return_none();
    }

    /* Transform: window → viewport-local → canvas */
    /* 1. Subtract destination origin */
    let local_x = as$(f32)(window_pos.x - self->dst.pos.x);
    let local_y = as$(f32)(window_pos.y - self->dst.pos.y);

    /* 2. Inverse scale */
    let unscaled_x = local_x / scale.x;
    let unscaled_y = local_y / scale.y;

    /* 3. Add source origin */
    let canvas_pos = m_V2i32_of(
        self->src.pos.x + as$(i32)(unscaled_x),
        self->src.pos.y + as$(i32)(unscaled_y)
    );

    /* Check if result is within source bounds */
    if (canvas_pos.x < self->src.pos.x
        || canvas_pos.y < self->src.pos.y
        || self->src.pos.x + as$(i32)(self->src.size.x) <= canvas_pos.x
        || self->src.pos.y + as$(i32)(self->src.size.y) <= canvas_pos.y) {
        return_none();
    }

    return_some(canvas_pos);
} $unscoped_(fn);

fn_((dage_Viewport_containsPoint(const dage_Viewport* self, m_V2i32 window_pos))(bool)) {
    debug_assert_nonnull(self);

    let x = window_pos.x;
    let y = window_pos.y;

    let left = self->dst.pos.x;
    let top = self->dst.pos.y;
    let right = left + as$(i32)(self->dst.size.x);
    let bottom = top + as$(i32)(self->dst.size.y);

    return left <= x && x < right && top <= y && y < bottom;
};

fn_((dage_Viewport_getEffectiveScale(const dage_Viewport* self))(m_V2f32)) {
    debug_assert_nonnull(self);

    /* Avoid division by zero */
    if (self->src.size.x == 0 || self->src.size.y == 0) {
        return m_V2f32_splat(1.0f);
    }

    let src_w = as$(f32)(self->src.size.x);
    let src_h = as$(f32)(self->src.size.y);
    let dst_w = as$(f32)(self->dst.size.x);
    let dst_h = as$(f32)(self->dst.size.y);

    switch (self->fit) {
    case_((dage_Viewport_Fit_stretch)) {
        /* Independent scaling for each axis */
        return m_V2f32_of(
            dst_w / src_w,
            dst_h / src_h
        );
    } $end(case);

    case_((dage_Viewport_Fit_contain)) {
        /* Uniform scale to fit inside dst (letterbox) */
        let scale = prim_min(dst_w / src_w, dst_h / src_h);
        return m_V2f32_splat(scale);
    } $end(case);

    case_((dage_Viewport_Fit_cover)) {
        /* Uniform scale to cover dst completely (may crop) */
        let scale = prim_max(dst_w / src_w, dst_h / src_h);
        return m_V2f32_splat(scale);
    } $end(case);

    case dage_Viewport_Fit_none: $fallthrough;
    default_() {
        /* 1:1 pixel mapping */
        return m_V2f32_splat(1.0f);
    } $end(default);
    };
};
