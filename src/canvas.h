#include "assert.h"
#include "color.h"
#include "common.h"
#include "primitive_types.h"
#include "range.h"


typedef struct CanvasBuffer CanvasBuffer;
typedef struct Canvas       Canvas;


struct CanvasBuffer {
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
CanvasBuffer  FrameBuffer_new(u32 width, u32 height);
CanvasBuffer* FrameBuffer_withColor(RefT(CanvasBuffer) self, Color color);
CanvasBuffer* FrameBuffer_withColorRange(RefT(CanvasBuffer) self, Color color, usize rangeBegin, usize rangeEnd);
Color*        FrameBuffer_data(RefT(CanvasBuffer) self);
u32           FrameBuffer_width(const RefT(CanvasBuffer) self);
u32           FrameBuffer_height(const RefT(CanvasBuffer) self);
u32           FrameBuffer_size(const RefT(CanvasBuffer) self);
Color*        FrameBuffer_at(RefT(CanvasBuffer) self, usize x, usize y);
Color*        FrameBuffer_GetRange(RefT(CanvasBuffer) self, usize rangeBegin, usize rangeEnd);
void          FrameBuffer_SetRange(RefT(CanvasBuffer) self, Color color, usize rangeBegin, usize rangeEnd);
void          FrameBuffer_SetRangeColors(RefT(CanvasBuffer) self, Color* colors, usize rangeBegin, usize rangeEnd);
void          FrameBuffer_Clear(RefT(CanvasBuffer) self, Color color);


struct Canvas {
    union {
        CanvasBuffer buffers_[2];
        struct {
            CanvasBuffer back_[1];
            CanvasBuffer front_[1];
        };
    };
    CanvasBuffer* current_;
    CanvasBuffer* next_;
};
Canvas        Canvas_new(u32 width, u32 height);
Canvas*       Canvas_withColor(RefT(Canvas) self, Color color);
Canvas*       Canvas_withColorRange(RefT(Canvas) self, Color color, usize rangeBegin, usize rangeEnd);
CanvasBuffer* Canvas_buffer(RefT(Canvas) self);
Color*        Canvas_at(RefT(Canvas) self, usize x, usize y);
Color*        Canvas_GetRange(RefT(Canvas) self, usize rangeBegin, usize rangeEnd);
void          Canvas_SetRange(RefT(Canvas) self, usize rangeBegin, usize rangeEnd, Color color);
void          Canvas_SetRangeColors(RefT(Canvas) self, usize rangeBegin, usize rangeEnd, Color* colors);
void          Canvas_Clear(RefT(Canvas) self, Color color);
void          Canvas_Swap(RefT(Canvas) self);
void          Canvas_Draw(RefT(Canvas) self, u32 x, u32 y, Color color);
void          Canvas_DrawPoint(RefT(Canvas) self, u32 x, u32 y, Color color);


CanvasBuffer FrameBuffer_new(u32 width, u32 height) {
    return (CanvasBuffer){
        .data_       = (Color*)malloc((usize)width * (usize)height * sizeof(Color)),
        .dimensions_ = { width, height },
        .size_       = width * height
    };
}

CanvasBuffer* FrameBuffer_withColor(RefT(CanvasBuffer) self, Color color) {
    Assert(self);
    for (usize i = 0; i < self->size_; ++i) {
        self->data_[i] = color;
    }
    return self;
}

CanvasBuffer* FrameBuffer_withColorRange(RefT(CanvasBuffer) self, Color color, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= self->size_);
    for (usize i = rangeBegin; i < rangeEnd; ++i) {
        self->data_[i] = color;
    }
    return self;
}

Color* FrameBuffer_data(RefT(CanvasBuffer) self) {
    Assert(self);
    return self->data_;
}

u32 FrameBuffer_width(const RefT(CanvasBuffer) self) {
    Assert(self);
    return self->width_;
}

u32 FrameBuffer_height(const RefT(CanvasBuffer) self) {
    Assert(self);
    return self->height_;
}

u32 FrameBuffer_size(const RefT(CanvasBuffer) self) {
    Assert(self);
    return self->size_;
}

Color* FrameBuffer_at(RefT(CanvasBuffer) self, usize x, usize y) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < self->width_);
    Assert(0 <= y);
    Assert(y < self->height_);
    return self->data_ + (y * self->width_) + x;
}

// TODO: Impl Slice and Range Type
Color* FrameBuffer_GetRange(RefT(CanvasBuffer) self, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= self->size_);
    return self->data_ + rangeBegin;
}

void FrameBuffer_SetRange(RefT(CanvasBuffer) self, Color color, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= self->size_);
    for (usize i = rangeBegin; i < rangeEnd; ++i) {
        self->data_[i] = color;
    }
}

void FrameBuffer_SetRangeColors(RefT(CanvasBuffer) self, Color* colors, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= self->size_);
    for (usize i = rangeBegin; i < rangeEnd; ++i) {
        self->data_[i] = colors[i];
    }
}

void FrameBuffer_Clear(RefT(CanvasBuffer) self, Color color) {
    Assert(self);
    for (usize i = 0; i < self->size_; ++i) {
        self->data_[i] = color;
    }
}


// TODO: Blitting functions
Canvas Canvas_new(u32 width, u32 height) {
    Canvas canvas = {
        .buffers_ = {
            FrameBuffer_new(width, height),
            FrameBuffer_new(width, height) }
    };
    canvas.current_ = &canvas.back_[0];
    canvas.next_    = &canvas.front_[0];
    return canvas;
}

Canvas* Canvas_withColor(RefT(Canvas) self, Color color) {
    Assert(self);
    FrameBuffer_withColor(self->current_, color);
    return self;
}

Canvas* Canvas_withColorRange(RefT(Canvas) self, Color color, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= FrameBuffer_size(self->current_));
    FrameBuffer_withColorRange(self->current_, color, rangeBegin, rangeEnd);
    return self;
}

CanvasBuffer* Canvas_buffer(RefT(Canvas) self) {
    Assert(self);
    return self->next_;
}

Color* Canvas_at(RefT(Canvas) self, usize x, usize y) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < FrameBuffer_width(Canvas_buffer(self)));
    Assert(0 <= y);
    Assert(y < FrameBuffer_height(Canvas_buffer(self)));
    return FrameBuffer_at(Canvas_buffer(self), x, y);
}

// TODO: Impl Slice and Range Type
Color* Canvas_GetRange(RefT(Canvas) self, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= FrameBuffer_size(Canvas_buffer(self)));
    return FrameBuffer_GetRange(Canvas_buffer(self), rangeBegin, rangeEnd);
}

void Canvas_SetRange(RefT(Canvas) self, usize rangeBegin, usize rangeEnd, Color color) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= FrameBuffer_size(Canvas_buffer(self)));
    FrameBuffer_SetRange(Canvas_buffer(self), color, rangeBegin, rangeEnd);
}

void Canvas_SetRangeColors(RefT(Canvas) self, usize rangeBegin, usize rangeEnd, Color* colors) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= FrameBuffer_size(Canvas_buffer(self)));
    FrameBuffer_SetRangeColors(Canvas_buffer(self), colors, rangeBegin, rangeEnd);
}

void Canvas_Clear(RefT(Canvas) self, Color color) {
    Assert(self);
    FrameBuffer_Clear(Canvas_buffer(self), color);
}

void Canvas_Swap(RefT(Canvas) self) {
    Assert(self);
    Swap(CanvasBuffer*, self->current_, self->next_);
}

void Canvas_Draw(RefT(Canvas) self, u32 x, u32 y, Color color) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < FrameBuffer_width(Canvas_buffer(self)));
    Assert(0 <= y);
    Assert(y < FrameBuffer_height(Canvas_buffer(self)));
    FrameBuffer_at(Canvas_buffer(self), x, y)[0] = color;
}

void Canvas_DrawPoint(RefT(Canvas) self, u32 x, u32 y, Color color) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < FrameBuffer_width(Canvas_buffer(self)));
    Assert(0 <= y);
    Assert(y < FrameBuffer_height(Canvas_buffer(self)));
    FrameBuffer_at(Canvas_buffer(self), x, y)[0] = color;
}
