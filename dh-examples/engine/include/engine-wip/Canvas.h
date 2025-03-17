#ifndef ENGINE_CANVAS_INCLUDED
#define ENGINE_CANVAS_INCLUDED (1)

#include "engine-wip/Color.h"

typedef enum engine_CanvasType {
    engine_CanvasType_rgba,
    engine_CanvasType_rgba_unicode
    // engine_CanvasType_indexed8,
    // engine_CanvasType_indexed16,
    // engine_CanvasType_indexed256
} engine_CanvasType;

typedef struct engine_Canvas {
    Grid$Color        buffer;
    engine_CanvasType type;
    Color             default_color;
    mem_Allocator     allocator;
} engine_Canvas;
use_Sli$(engine_Canvas);
use_Opt$(engine_Canvas);
use_Err$(engine_Canvas);

/// Canvas creation with specific type
extern fn_(engine_Canvas_init(mem_Allocator allocator, u32 width, u32 height, engine_CanvasType type, Opt$Color default_color), must_check Err$Ptr$engine_Canvas);
extern fn_(engine_Canvas_fini(engine_Canvas* self), void);
extern fn_(engine_Canvas_resize(engine_Canvas* self, u32 width, u32 height), must_check Err$void);

extern fn_(engine_Canvas_clear(engine_Canvas* self, Opt$Color other_color), void);

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

typedef engine_Canvas engine_ColorCanvas;
use_Ptr$(engine_ColorCanvas);
use_Err$(engine_ColorCanvas);

typedef struct engine_CanvasView engine_CanvasView;

#endif /* ENGINE_CANVAS_INCLUDED */
