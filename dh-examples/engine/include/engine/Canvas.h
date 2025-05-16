#ifndef ENGINE_CANVAS_INCLUDED
#define ENGINE_CANVAS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/Color.h"

typedef enum engine_CanvasType {
    engine_CanvasType_rgba,
    engine_CanvasType_rgba_unicode
} engine_CanvasType;

typedef struct engine_Canvas {
    Grid$Color        buffer;
    mem_Allocator     allocator;
    Color             default_color;
    engine_CanvasType type;
} engine_Canvas;
use_Ptr$(engine_Canvas);
use_Sli$(engine_Canvas);
use_Opt$(engine_Canvas);
use_Err$(engine_Canvas);

typedef struct engine_Canvas_Config {
    Opt$mem_Allocator allocator;
    u32               width;
    u32               height;
    Opt$Color         default_color;
    Opt$$(engine_CanvasType) type;
} engine_Canvas_Config;

/// Canvas creation with specific type
extern fn_(engine_Canvas_init(const engine_Canvas_Config* config), Err$Ptr$engine_Canvas) $must_check;
extern fn_(engine_Canvas_fini(engine_Canvas* self), void);
extern fn_(engine_Canvas_resize(engine_Canvas* self, u32 width, u32 height), Err$void) $must_check;
extern fn_(engine_Canvas_clear(engine_Canvas* self, Opt$Color color), void);

extern fn_(engine_Canvas_drawPixel(engine_Canvas* self, i32 x, i32 y, Color color), void);
extern fn_(engine_Canvas_drawLine(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, Color color), void);
extern fn_(engine_Canvas_drawHLine(engine_Canvas* self, i32 x1, i32 x2, i32 y, Color color), void);
extern fn_(engine_Canvas_drawVLine(engine_Canvas* self, i32 x, i32 y1, i32 y2, Color color), void);
extern fn_(engine_Canvas_drawRect(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, Color color), void);
extern fn_(engine_Canvas_drawCircle(engine_Canvas* self, i32 cx, i32 cy, i32 radius, Color color), void);
extern fn_(engine_Canvas_drawRing(engine_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, Color color), void);
extern fn_(engine_Canvas_drawArc(engine_Canvas* self, i32 cx, i32 cy, i32 radius, f32 start_angle, f32 end_angle, Color color), void);
extern fn_(engine_Canvas_drawAngleRing(engine_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, f32 start_angle, f32 end_angle, Color color), void);
extern fn_(engine_Canvas_drawCapsule(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, Color color), void);
extern fn_(engine_Canvas_drawLineThick(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, f32 thickness, Color color), void);
extern fn_(engine_Canvas_drawRectThick(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, f32 thickness, Color color), void);
extern fn_(engine_Canvas_drawRectBorderByCutout(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, i32 thickness, Color inner_color, Color border_color), void);

extern fn_(engine_Canvas_fillRect(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, Color color), void);
extern fn_(engine_Canvas_fillCircle(engine_Canvas* self, i32 cx, i32 cy, i32 radius, Color color), void);
extern fn_(engine_Canvas_fillRingByCutout(engine_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, Color inner_color, Color outer_color), void);
extern fn_(engine_Canvas_fillRingByScanlines(engine_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, Color color), void);
extern fn_(engine_Canvas_fillArc(engine_Canvas* self, i32 cx, i32 cy, i32 radius, f32 start_angle, f32 end_angle, Color color), void);
extern fn_(engine_Canvas_fillAngleRingByCutout(engine_Canvas* self, i32 cx, i32 cy, i32 r_inner, i32 r_outer, f32 start_angle, f32 end_angle, Color inner_color, Color outer_color), void);
extern fn_(engine_Canvas_fillCapsule(engine_Canvas* self, i32 x1, i32 y1, i32 x2, i32 y2, Color color), void);

/// Specialized canvas operations
extern fn_(engine_Canvas_blit(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y), void);
extern fn_(engine_Canvas_blitScaled(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y, f32 scale), void);

typedef struct engine_CanvasView {
    engine_Canvas* canvas; // Associated canvas
    struct {
        Vec2i top_left;
    } pos_on_window; // Position in window coordinates
    struct {
        Vec2u size;  // Size in window coordinates
        Vec2f scale; // Scale in window coordinates
        /* TODO: Add options: stretch, crop, resizable, scalable, ... */
        struct {
            bool x;
            bool y;
        } resizable;
    } rect;
    bool visible; // Visibility flag
} engine_CanvasView;

typedef struct engine_CanvasView_Config {
    engine_Canvas* canvas;
    Vec2i          pos;
    Vec2u          size;
    Vec2f          scale;
    bool           resizable_x;
    bool           resizable_y;
    bool           visible;
} engine_CanvasView_Config;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_CANVAS_INCLUDED */
