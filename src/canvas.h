#ifndef CANVAS_INCLUDED
#define CANVAS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "color.h"
#include "primitive_types.h"


typedef union Rect         Rect;
typedef struct FrameBuffer FrameBuffer;
typedef struct Canvas      Canvas;


union Rect {
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
};
#define Rect_(...) makeWith(Rect, __VA_ARGS__)
Rect Rect_from(i64 x, i64 y, i64 width, i64 height);
Rect Rect_fromMinMax(i64 min_x, i64 min_y, i64 max_x, i64 max_y);

#define comptime_Rect_from(_x, _y, _width, _height) \
    Rect_(.x = (_x), .y = (_y), .width = (_width), .height = (_height))
#define comptime_Rect_fromMinMax(_min_x, _min_y, _max_x, _max_y) \
    Rect_(.min_x = (_min_x), .min_y = (_min_y), .max_x = (_max_x), .max_y = (_max_y))


struct FrameBuffer {
    Color* data_;
    union {
        u32 dimensions_[2];
        struct {
            u32 width_;
            u32 height_;
        };
    };
    u32 size_;
};
#define FrameBuffer_(...) makeWith(FrameBuffer, __VA_ARGS__)
FrameBuffer* FrameBuffer_init(Ref(FrameBuffer) self, u32 width, u32 height);
FrameBuffer* FrameBuffer_initWithColor(Ref(FrameBuffer) self, Color color);
FrameBuffer* FrameBuffer_initWithColorRange(Ref(FrameBuffer) self, Color color, usize range_start, usize range_end);
void         FrameBuffer_fini(Ref(FrameBuffer) self);

const Color* FrameBuffer_readData(const Ref(FrameBuffer) self);
Color*       FrameBuffer_accessData(Ref(FrameBuffer) self);
u32          FrameBuffer_width(const Ref(FrameBuffer) self);
u32          FrameBuffer_height(const Ref(FrameBuffer) self);
u32          FrameBuffer_size(const Ref(FrameBuffer) self);
const Color* FrameBuffer_readAt(const Ref(FrameBuffer) self, usize x, usize y);
Color*       FrameBuffer_accessAt(Ref(FrameBuffer) self, usize x, usize y);
Color        FrameBuffer_get(const Ref(FrameBuffer) self, usize x, usize y);
void         FrameBuffer_set(Ref(FrameBuffer) self, Color color, usize x, usize y);
Color*       FrameBuffer_getRange(const Ref(FrameBuffer) self, usize range_start, usize range_end);
void         FrameBuffer_setRange(Ref(FrameBuffer) self, Color color, usize range_start, usize range_end);
void         FrameBuffer_setRangeColors(Ref(FrameBuffer) self, Color* colors, usize range_start, usize range_end);
void         FrameBuffer_clear(Ref(FrameBuffer) self, Color color);


struct Canvas {
    union {
        FrameBuffer buffers_[2];
        struct {
            FrameBuffer back_[1];
            FrameBuffer front_[1];
        };
    };
    FrameBuffer* current_;
    FrameBuffer* next_;
};
#define Canvas_(...) makeWith(Canvas, __VA_ARGS__)
Canvas* Canvas_init(Ref(Canvas) self, u32 width, u32 height);
Canvas* Canvas_initWithColor(Ref(Canvas) self, Color color);
Canvas* Canvas_initWithColorRange(Ref(Canvas) self, Color color, usize range_start, usize range_end);
void    Canvas_fini(Ref(Canvas) self);

const FrameBuffer* Canvas_readBuffer(const Ref(Canvas) self);
FrameBuffer*       Canvas_accessBuffer(Ref(Canvas) self);
const Color*       Canvas_readAt(const Ref(Canvas) self, usize x, usize y);
Color*             Canvas_accessAt(Ref(Canvas) self, usize x, usize y);
Color              Canvas_get(const Ref(Canvas) self, usize x, usize y);
void               Canvas_set(Ref(Canvas) self, usize x, usize y, Color color);
Color*             Canvas_getRange(const Ref(Canvas) self, usize range_start, usize range_end);
void               Canvas_setRange(Ref(Canvas) self, usize range_start, usize range_end, Color color);
void               Canvas_setRangeColors(Ref(Canvas) self, usize range_start, usize range_end, Color* colors);
void               Canvas_clear(Ref(Canvas) self, Color color);
void               Canvas_swap(Ref(Canvas) self);
void               Canvas_draw(Ref(Canvas) self, u32 x, u32 y, Color color);
void               Canvas_drawPoint(Ref(Canvas) self, u32 x, u32 y, Color color);
void               Canvas_fillRect(Ref(Canvas) self, Rect rect, Color color);
Rect               Canvas_normalizeRect(Ref(Canvas) self, Rect rect);


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* CANVAS_INCLUDED */
