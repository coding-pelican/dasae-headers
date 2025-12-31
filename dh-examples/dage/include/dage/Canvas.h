#ifndef dage_Canvas__included
#define dage_Canvas__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dage/common.h"

typedef enum_(dage_CanvasType $bits(8)) {
    dage_CanvasType_rgba,
    dage_CanvasType_rgba_unicode
} dage_CanvasType;

typedef struct dage_Canvas {
    Grid$color_RGBA gird;
    color_RGBA default_color;
    dage_CanvasType type;
} dage_Canvas;
T_use_prl$(dage_Canvas);

typedef struct dage_Canvas_Cfg {
    mem_Allocator gpa;
    u32 width;
    u32 height;
    O$color_RGBA default_color;
    dage_CanvasType type;
} dage_Canvas_Cfg;

/// Canvas creation with specific type
extern fn_((dage_Canvas_init(dage_Canvas_Cfg cfg))(E$dage_Canvas)) $must_check;
extern fn_((dage_Canvas_fini(dage_Canvas* self, mem_Allocator gpa))(void));
extern fn_((dage_Canvas_resize(dage_Canvas* self, mem_Allocator gpa, u32 width, u32 height))(E$void)) $must_check;
extern fn_((dage_Canvas_clear(dage_Canvas* self, O$color_RGBA color))(void));

extern fn_((dage_Canvas_drawPixel(dage_Canvas* self, i32 x, i32 y, color_RGBA color))(void));
extern fn_((dage_Canvas_drawLine(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, color_RGBA color))(void));
extern fn_((dage_Canvas_drawHLine(dage_Canvas* self, i32 x1, i32 x2, i32 y, color_RGBA color))(void));
extern fn_((dage_Canvas_drawVLine(dage_Canvas* self, i32 x, i32 y1, i32 y2, color_RGBA color))(void));
extern fn_((dage_Canvas_drawRect(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, color_RGBA color))(void));
extern fn_((dage_Canvas_drawCircle(dage_Canvas* self, i32 cx, i32 cy, i32 radius, color_RGBA color))(void));
extern fn_((dage_Canvas_drawRing(dage_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, color_RGBA color))(void));
extern fn_((dage_Canvas_drawArc(dage_Canvas* self, i32 cx, i32 cy, i32 radius, f32 start_angle, f32 end_angle, color_RGBA color))(void));
extern fn_((dage_Canvas_drawAngleRing(dage_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, f32 start_angle, f32 end_angle, color_RGBA color))(void));
extern fn_((dage_Canvas_drawCapsule(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, color_RGBA color))(void));
extern fn_((dage_Canvas_drawLineThick(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, f32 thickness, color_RGBA color))(void));
extern fn_((dage_Canvas_drawRectThick(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, f32 thickness, color_RGBA color))(void));
extern fn_((dage_Canvas_drawRectBorderByCutout(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, i32 thickness, color_RGBA inner_color, color_RGBA border_color))(void));

extern fn_((dage_Canvas_fillRect(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, color_RGBA color))(void));
extern fn_((dage_Canvas_fillCircle(dage_Canvas* self, i32 cx, i32 cy, i32 radius, color_RGBA color))(void));
extern fn_((dage_Canvas_fillRingByCutout(dage_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, color_RGBA inner_color, color_RGBA outer_color))(void));
extern fn_((dage_Canvas_fillRingByScanlines(dage_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, color_RGBA color))(void));
extern fn_((dage_Canvas_fillArc(dage_Canvas* self, i32 cx, i32 cy, i32 radius, f32 start_angle, f32 end_angle, color_RGBA color))(void));
extern fn_((dage_Canvas_fillAngleRingByCutout(dage_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, f32 start_angle, f32 end_angle, color_RGBA inner_color, color_RGBA outer_color))(void));
extern fn_((dage_Canvas_fillCapsule(dage_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, color_RGBA color))(void));

/// Specialized canvas operations
extern fn_((dage_Canvas_blit(dage_Canvas* dst, const dage_Canvas* src, i32 x, i32 y))(void));
extern fn_((dage_Canvas_blitScaled(dage_Canvas* dst, const dage_Canvas* src, i32 x, i32 y, f32 scale))(void));

typedef struct dage_CanvasView {
    dage_Canvas* canvas; // Associated canvas
    struct {
        m_V2i32 top_left;
    } pos_on_window; // Position in window coordinates
    struct {
        m_V2u32 size; // Size in window coordinates
        m_V2f32 scale; // Scale in window coordinates
        /* TODO: Add options: stretch, crop, resizable, scalable, ... */
        struct {
            bool x;
            bool y;
        } resizable;
    } rect;
    bool visible; // Visibility flag
} dage_CanvasView;
T_use_prl$(dage_CanvasView);

typedef struct dage_CanvasView_Cfg {
    dage_Canvas* canvas;
    m_V2i32 pos;
    m_V2u32 size;
    m_V2f32 scale;
    bool resizable_x;
    bool resizable_y;
    bool visible;
} dage_CanvasView_Cfg;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dage_Canvas__included */
