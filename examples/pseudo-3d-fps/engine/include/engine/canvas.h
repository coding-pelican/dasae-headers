#ifndef ENGINE_CANVAS_INCLUDED
#define ENGINE_CANVAS_INCLUDED (1)

#include "platform.h"
#include "viewport.h"
#include "color.h"

typedef enum engine_CanvasType {
    engine_CanvasType_rgba,
    // engine_CanvasType_indexed8,
    // engine_CanvasType_indexed16,
    // engine_CanvasType_indexed256
} engine_CanvasType;

typedef struct engine_Canvas {
    struct Sli$Color {
        Color* ptr;
        usize  len;
    } buffer;
    u32 width;
    u32 height;

    engine_CanvasType type;
    // Canvas-specific conversion functions
    Color (*pixelToColor)(const Color* buffer, u32 x, u32 y);
    void (*colorToPixel)(Color* buffer, u32 x, u32 y, Color color);
} engine_Canvas;
using_Ptr$(engine_Canvas);
using_Err$(engine_Canvas);

// Canvas creation with specific type
extern Err$Ptr$engine_Canvas engine_Canvas_create(u32 width, u32 height, engine_CanvasType type) must_check;
extern void                  engine_Canvas_destroy(engine_Canvas* canvas);
extern void                  engine_Canvas_clear(engine_Canvas* canvas, Color color);
extern void                  engine_Canvas_drawPixel(engine_Canvas* canvas, i32 x, i32 y, Color color);
extern void                  engine_Canvas_fillRect(engine_Canvas* canvas, i32 x, i32 y, i32 w, i32 h, Color color);
extern void                  engine_Canvas_resize(engine_Canvas* canvas, u32 width, u32 height);

// Specialized canvas operations
extern void engine_Canvas_blit(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y);
extern void engine_Canvas_blitScaled(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y, f32 scale);

#endif /* ENGINE_CANVAS_INCLUDED */
