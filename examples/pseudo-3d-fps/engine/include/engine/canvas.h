#ifndef ENGINE_CANVAS_INCLUDED
#define ENGINE_CANVAS_INCLUDED (1)

#include "platform.h"
#include "viewport.h"

typedef enum engine_CanvasType {
    engine_CanvasType_rgba,
    engine_CanvasType_indexed8,
    engine_CanvasType_indexed16,
    engine_CanvasType_indexed256
} engine_CanvasType;

typedef struct engine_Canvas {
    u32               width;
    u32               height;
    engine_CanvasType type;
    anyptr            buffer; // Generic buffer pointer
    u32               buffer_size;

    // Canvas-specific conversion functions
    engine_ColorValue (*pixel_to_color)(const anyptr buffer, u32 x, u32 y);
    void (*color_to_pixel)(anyptr buffer, u32 x, u32 y, engine_ColorValue color);
} engine_Canvas;
using_Ptr$(engine_Canvas);
using_Err$(Ptr$engine_Canvas);

// Canvas creation with specific type
Err$Ptr$engine_Canvas engine_Canvas_create(u32 width, u32 height, engine_CanvasType type) must_check;
void                  engine_Canvas_destroy(engine_Canvas* canvas);
void                  engine_Canvas_clear(engine_Canvas* canvas, engine_ColorValue color);
void                  engine_Canvas_drawPixel(engine_Canvas* canvas, i32 x, i32 y, engine_ColorValue color);
void                  engine_Canvas_resize(engine_Canvas* canvas, u32 width, u32 height);

// Specialized canvas operations
void engine_Canvas_blit(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y);
void engine_Canvas_blitScaled(engine_Canvas* dst, const engine_Canvas* src, i32 x, i32 y, f32 scale);

#endif // ENGINE_CANVAS_INCLUDED
