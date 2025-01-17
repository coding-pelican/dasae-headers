#ifndef ENGINE_CANVAS_INCLUDED
#define ENGINE_CANVAS_INCLUDED (1)

#include "platform.h"
#include "viewport.h"
#include "color.h"

typedef enum engine_CanvasType {
    engine_CanvasType_rgba,
    engine_CanvasType_rgba_unicode
    // engine_CanvasType_indexed8,
    // engine_CanvasType_indexed16,
    // engine_CanvasType_indexed256
} engine_CanvasType;

typedef struct engine_Canvas {
    struct {
        Color* ptr;
        usize  len;
    } buffer;
    u32 width;
    u32 height;

    engine_CanvasType type;
    Color             default_color;
    // Canvas-specific conversion functions
    Color (*pixelToColor)(const Color* buffer, u32 x, u32 y);
    void (*colorToPixel)(Color* buffer, u32 x, u32 y, Color color);
} engine_Canvas;
use_Ptr$(engine_Canvas);
use_Err$(engine_Canvas);

// Canvas creation with specific type
extern Err$Ptr$engine_Canvas engine_Canvas_create(u32 width, u32 height, engine_CanvasType type) must_check;
extern Err$Ptr$engine_Canvas engine_Canvas_createWithDefaultColor(u32 width, u32 height, engine_CanvasType type, Color default_color) must_check;
extern void                  engine_Canvas_destroy(engine_Canvas* canvas);
extern void                  engine_Canvas_resize(engine_Canvas* canvas, u32 width, u32 height);
extern void                  engine_Canvas_clear(engine_Canvas* canvas, Color color);
extern void                  engine_Canvas_clearDefaultColor(engine_Canvas* canvas);
extern void                  engine_Canvas_drawPixel(engine_Canvas* canvas, i32 x, i32 y, Color color);
extern void                  engine_Canvas_drawLine(engine_Canvas* canvas, i32 x1, i32 y1, i32 x2, i32 y2, Color color);
extern void                  engine_Canvas_drawHLine(engine_Canvas* canvas, i32 x1, i32 x2, i32 y, Color color);
extern void                  engine_Canvas_drawVLine(engine_Canvas* canvas, i32 x, i32 y1, i32 y2, Color color);
extern void                  engine_Canvas_drawRect(engine_Canvas* canvas, i32 x1, i32 y1, i32 x2, i32 y2, Color color);
extern void                  engine_Canvas_drawCircle(engine_Canvas* canvas, i32 cx, i32 cy, i32 radius, Color color);
extern void                  engine_Canvas_drawRing(engine_Canvas* canvas, i32 cx, i32 cy, i32 r_inner, i32 r_outer, Color color);
extern void                  engine_Canvas_drawArc(engine_Canvas* canvas, i32 cx, i32 cy, i32 radius, f32 start_angle, f32 end_angle, Color color);
extern void                  engine_Canvas_drawAngleRing(engine_Canvas* canvas, i32 cx, i32 cy, i32 r_inner, i32 r_outer, f32 start_angle, f32 end_angle, Color color);
extern void                  engine_Canvas_drawCapsule(engine_Canvas* canvas, i32 x1, i32 y1, i32 x2, i32 y2, Color color);
extern void                  engine_Canvas_drawLineThick(engine_Canvas* canvas, i32 x1, i32 y1, i32 x2, i32 y2, f32 thickness, Color color);
extern void                  engine_Canvas_drawRectThick(engine_Canvas* canvas, i32 x1, i32 y1, i32 x2, i32 y2, f32 thickness, Color color);
extern void                  engine_Canvas_drawRectBorderByCutout(engine_Canvas* canvas, i32 x1, i32 y1, i32 x2, i32 y2, i32 thickness, Color inner_color, Color border_color);
extern void                  engine_Canvas_fillRect(engine_Canvas* canvas, i32 x1, i32 y1, i32 x2, i32 y2, Color color);
extern void                  engine_Canvas_fillCircle(engine_Canvas* canvas, i32 cx, i32 cy, i32 radius, Color color);
extern void                  engine_Canvas_fillRingByCutout(engine_Canvas* canvas, i32 cx, i32 cy, i32 r_inner, i32 r_outer, Color inner_color, Color outer_color);
extern void                  engine_Canvas_fillRingByScanlines(engine_Canvas* canvas, i32 cx, i32 cy, i32 r_inner, i32 r_outer, Color color);
extern void                  engine_Canvas_fillArc(engine_Canvas* canvas, i32 cx, i32 cy, i32 radius, f32 start_angle, f32 end_angle, Color color);
extern void                  engine_Canvas_fillAngleRingByCutout(engine_Canvas* canvas, i32 cx, i32 cy, i32 r_inner, i32 r_outer, f32 start_angle, f32 end_angle, Color inner_color, Color outer_color);
extern void                  engine_Canvas_fillCapsule(engine_Canvas* canvas, i32 x1, i32 y1, i32 x2, i32 y2, Color color);

// Specialized canvas operations
extern void engine_Canvas_blit(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y);
extern void engine_Canvas_blitScaled(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y, f32 scale);

#endif /* ENGINE_CANVAS_INCLUDED */
