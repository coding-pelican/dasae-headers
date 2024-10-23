#ifndef CANVAS_INCLUDED
#define CANVAS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "color.h"
#include "primitive_types.h"


typedef struct Rect        Rect;
typedef struct FrameBuffer FrameBuffer;
typedef struct Canvas      Canvas;


struct Rect {
    union {
        i64 components[4];
        struct {
            i64 x;
            i64 y;
            i64 width;
            i64 height;
        };
        struct {
            i64 minX;
            i64 minY;
            i64 maxX;
            i64 maxY;
        };
    };
};
#define Rect_(...)                                  ((Rect){ __VA_ARGS__ })
#define Rect_Make(_x, _y, _width, _height)          (Rect_(.x = (_x), .y = (_y), .width = (_width), .height = (_height)))
#define Rect_FromMinMax(_minX, _minY, _maxX, _maxY) (Rect_(.minX = (_minX), .minY = (_minY), .maxX = (_maxX), .maxY = (_maxY)))


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
FrameBuffer* new_FrameBuffer();
FrameBuffer* FrameBuffer_Init(Ref(FrameBuffer) self, u32 width, u32 height);
FrameBuffer* FrameBuffer_InitWithColor(Ref(FrameBuffer) self, Color color);
FrameBuffer* FrameBuffer_InitWithColorRange(Ref(FrameBuffer) self, Color color, usize rangeStart, usize rangeEnd);
void         FrameBuffer_Fini(Ref(FrameBuffer) self);
void         delete_FrameBuffer(Ref(FrameBuffer*) buffer);

const Color* FrameBuffer_ReadData(const Ref(FrameBuffer) self);
Color*       FrameBuffer_AccessData(Ref(FrameBuffer) self);
u32          FrameBuffer_Width(const Ref(FrameBuffer) self);
u32          FrameBuffer_Height(const Ref(FrameBuffer) self);
u32          FrameBuffer_Size(const Ref(FrameBuffer) self);
const Color* FrameBuffer_ReadAt(const Ref(FrameBuffer) self, usize x, usize y);
Color*       FrameBuffer_AccessAt(Ref(FrameBuffer) self, usize x, usize y);
Color        FrameBuffer_Get(const Ref(FrameBuffer) self, usize x, usize y);
void         FrameBuffer_Set(Ref(FrameBuffer) self, Color color, usize x, usize y);
Color*       FrameBuffer_GetRange(const Ref(FrameBuffer) self, usize rangeStart, usize rangeEnd);
void         FrameBuffer_SetRange(Ref(FrameBuffer) self, Color color, usize rangeStart, usize rangeEnd);
void         FrameBuffer_SetRangeColors(Ref(FrameBuffer) self, Color* colors, usize rangeStart, usize rangeEnd);
void         FrameBuffer_Clear(Ref(FrameBuffer) self, Color color);


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
Canvas* new_Canvas();
Canvas* Canvas_Init(Ref(Canvas) self, u32 width, u32 height);
Canvas* Canvas_InitWithColor(Ref(Canvas) self, Color color);
Canvas* Canvas_InitWithColorRange(Ref(Canvas) self, Color color, usize rangeStart, usize rangeEnd);
void    Canvas_Fini(Ref(Canvas) self);
void    delete_Canvas(Ref(Canvas*) canvas);

const FrameBuffer* Canvas_ReadBuffer(const Ref(Canvas) self);
FrameBuffer*       Canvas_AccessBuffer(Ref(Canvas) self);
const Color*       Canvas_ReadAt(const Ref(Canvas) self, usize x, usize y);
Color*             Canvas_AccessAt(Ref(Canvas) self, usize x, usize y);
Color              Canvas_Get(const Ref(Canvas) self, usize x, usize y);
void               Canvas_Set(Ref(Canvas) self, usize x, usize y, Color color);
Color*             Canvas_GetRange(const Ref(Canvas) self, usize rangeStart, usize rangeEnd);
void               Canvas_SetRange(Ref(Canvas) self, usize rangeStart, usize rangeEnd, Color color);
void               Canvas_SetRangeColors(Ref(Canvas) self, usize rangeStart, usize rangeEnd, Color* colors);
void               Canvas_Clear(Ref(Canvas) self, Color color);
void               Canvas_Swap(Ref(Canvas) self);
void               Canvas_Draw(Ref(Canvas) self, u32 x, u32 y, Color color);
void               Canvas_DrawPoint(Ref(Canvas) self, u32 x, u32 y, Color color);
void               Canvas_FillRect(Ref(Canvas) self, Rect rect, Color color);
Rect               Canvas_NormalizeRect(Ref(Canvas) self, Rect rect);


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* CANVAS_INCLUDED */
