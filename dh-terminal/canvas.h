#ifndef CANVAS_INCLUDED
#define CANVAS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "dh/core.h"
#include "color.h"


typedef union Rect {
    i64 components[4];
    struct {
        i64 x;
        i64 y;
        i64 width;
        i64 height;
    };
    struct {
        i64 min_x;
        i64 min_y;
        i64 max_x;
        i64 max_y;
    };
} Rect;
#define Rect_(...) makeWith(Rect, __VA_ARGS__)
Rect Rect_from(i64 x, i64 y, i64 width, i64 height);
Rect Rect_fromMinMax(i64 min_x, i64 min_y, i64 max_x, i64 max_y);

#define comptime_Rect_from(_x, _y, _width, _height) \
    Rect_(.x = (_x), .y = (_y), .width = (_width), .height = (_height))
#define comptime_Rect_fromMinMax(_min_x, _min_y, _max_x, _max_y) \
    Rect_(.min_x = (_min_x), .min_y = (_min_y), .max_x = (_max_x), .max_y = (_max_y))


typedef struct FrameBuffer {
    struct ds_Vec_Color {
        Color* data;
        usize  length;
        usize  capacity;
    } pixels_;
    union {
        u32 dimensions_[2];
        struct {
            u32 width_;
            u32 height_;
        };
    };
    u32 size_;
} FrameBuffer;
FrameBuffer* FrameBuffer_init(FrameBuffer* const b, u32 width, u32 height);
FrameBuffer* FrameBuffer_withColor(FrameBuffer* const b, Color color);
// FrameBuffer* FrameBuffer_withColorRange(FrameBuffer* const b, Color color, usize range_start, usize range_end);
FrameBuffer* FrameBuffer_fini(FrameBuffer* const b);

const Color* FrameBuffer_peekData(const FrameBuffer* const b);
Color*       FrameBuffer_accessData(FrameBuffer* const b);
u32          FrameBuffer_width(const FrameBuffer* const b);
u32          FrameBuffer_height(const FrameBuffer* const b);
u32          FrameBuffer_size(const FrameBuffer* const b);
const Color* FrameBuffer_peekAt(const FrameBuffer* const b, usize x, usize y);
Color*       FrameBuffer_accessAt(FrameBuffer* const b, usize x, usize y);
Color        FrameBuffer_get(const FrameBuffer* const b, usize x, usize y);
void         FrameBuffer_set(FrameBuffer* const b, Color color, usize x, usize y);
// Color*       FrameBuffer_getRange(const FrameBuffer* const b, usize range_start, usize range_end);
// void         FrameBuffer_setRange(FrameBuffer* const b, Color color, usize range_start, usize range_end);
// void         FrameBuffer_setRangeColors(FrameBuffer* const b, Color* colors, usize range_start, usize range_end);
void         FrameBuffer_clear(FrameBuffer* const b, Color color);


typedef struct Canvas {
    union {
        FrameBuffer buffers_[2];
        struct {
            FrameBuffer back_[1];
            FrameBuffer front_[1];
        };
    };
    FrameBuffer* current_;
    FrameBuffer* next_;
} Canvas;
Canvas* Canvas_init(Canvas* const c, u32 width, u32 height);
Canvas* Canvas_withColor(Canvas* const c, Color color);
// Canvas* Canvas_withColorRange(Canvas* const c, Color color, usize range_start, usize range_end);
Canvas* Canvas_fini(Canvas* const c);

const FrameBuffer* Canvas_peekBuffer(const Canvas* const c);
FrameBuffer*       Canvas_accessBuffer(Canvas* const c);
const Color*       Canvas_peekAt(const Canvas* const c, usize x, usize y);
Color*             Canvas_accessAt(Canvas* const c, usize x, usize y);
Color              Canvas_get(const Canvas* const c, usize x, usize y);
void               Canvas_set(Canvas* const c, usize x, usize y, Color color);
// Color*             Canvas_getRange(const Canvas* const c, usize range_start, usize range_end);
// void               Canvas_setRange(Canvas* const c, usize range_start, usize range_end, Color color);
// void               Canvas_setRangeColors(Canvas* const c, usize range_start, usize range_end, Color* colors);
void               Canvas_clear(Canvas* const c, Color color);
void               Canvas_swap(Canvas* const c);
void               Canvas_draw(Canvas* const c, u32 x, u32 y, Color color);
void               Canvas_drawPoint(Canvas* const c, u32 x, u32 y, Color color);
void               Canvas_fillRect(Canvas* const c, Rect rect, Color color);
Rect               Canvas_normalizeRect(Canvas* const c, Rect rect);


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* CANVAS_INCLUDED */
