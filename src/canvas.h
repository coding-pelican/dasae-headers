#include "assert.h"
#include "color.h"
#include "common.h"
#include "primitive_types.h"


typedef struct ColorBuffer ColorBuffer;
typedef struct ColorCanvas ColorCanvas;


struct ColorBuffer {
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
ColorBuffer ColorBuffer_new(u32 width, u32 height) {
    return (ColorBuffer){
        .data_       = (Color*)malloc((usize)width * (usize)height * sizeof(Color)),
        .dimensions_ = { width, height },
        .size_       = width * height
    };
}
ColorBuffer* ColorBuffer_withColor(RefT(ColorBuffer) self, Color color) {
    Assert(self);
    for (usize i = 0; i < self->size_; ++i) {
        self->data_[i] = color;
    }
    return self;
}
ColorBuffer* ColorBuffer_withColorRange(RefT(ColorBuffer) self, Color color, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= self->size_);
    for (usize i = rangeBegin; i < rangeEnd; ++i) {
        self->data_[i] = color;
    }
    return self;
}
Color* ColorBuffer_data(RefT(ColorBuffer) self) {
    Assert(self);
    return self->data_;
}
u32 ColorBuffer_width(const RefT(ColorBuffer) self) {
    Assert(self);
    return self->width_;
}
u32 ColorBuffer_height(const RefT(ColorBuffer) self) {
    Assert(self);
    return self->height_;
}
u32 ColorBuffer_size(const RefT(ColorBuffer) self) {
    Assert(self);
    return self->size_;
}
Color* ColorBuffer_at(RefT(ColorBuffer) self, usize x, usize y) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < self->width_);
    Assert(0 <= y);
    Assert(y < self->height_);
    return self->data_ + (y * self->width_) + x;
}
// TODO: Impl Slice and Range Type
Color* ColorBuffer_GetRange(RefT(ColorBuffer) self, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= self->size_);
    return self->data_ + rangeBegin;
}
void ColorBuffer_SetRange(RefT(ColorBuffer) self, Color color, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= self->size_);
    for (usize i = rangeBegin; i < rangeEnd; ++i) {
        self->data_[i] = color;
    }
}
void ColorBuffer_SetRangeColors(RefT(ColorBuffer) self, Color* colors, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= self->size_);
    for (usize i = rangeBegin; i < rangeEnd; ++i) {
        self->data_[i] = colors[i];
    }
}
void ColorBuffer_Clear(RefT(ColorBuffer) self, Color color) {
    Assert(self);
    for (usize i = 0; i < self->size_; ++i) {
        self->data_[i] = color;
    }
}


struct ColorCanvas {
    union {
        ColorBuffer buffers_[2];
        struct {
            ColorBuffer back_[1];
            ColorBuffer front_[1];
        };
    };
    ColorBuffer* current_;
    ColorBuffer* next_;
};
ColorCanvas  ColorCanvas_new(u32 width, u32 height);
ColorCanvas* ColorCanvas_withColor(RefT(ColorCanvas) self, Color color);
ColorCanvas* ColorCanvas_withColorRange(RefT(ColorCanvas) self, Color color, usize rangeBegin, usize rangeEnd);
ColorBuffer* ColorCanvas_buffer(RefT(ColorCanvas) self);
Color*       ColorCanvas_at(RefT(ColorCanvas) self, usize x, usize y);
Color*       ColorCanvas_GetRange(RefT(ColorCanvas) self, usize rangeBegin, usize rangeEnd);
void         ColorCanvas_SetRange(RefT(ColorCanvas) self, usize rangeBegin, usize rangeEnd, Color color);
void         ColorCanvas_SetRangeColors(RefT(ColorCanvas) self, usize rangeBegin, usize rangeEnd, Color* colors);
void         ColorCanvas_Clear(RefT(ColorCanvas) self, Color color);
void         ColorCanvas_Swap(RefT(ColorCanvas) self);
void         ColorCanvas_Draw(RefT(ColorCanvas) self, u32 x, u32 y, Color color);
void         ColorCanvas_DrawPoint(RefT(ColorCanvas) self, u32 x, u32 y, Color color);
// TODO: Blitting functions
ColorCanvas ColorCanvas_new(u32 width, u32 height) {
    ColorCanvas canvas = {
        .buffers_ = {
            ColorBuffer_new(width, height),
            ColorBuffer_new(width, height) }
    };
    canvas.current_ = &canvas.back_[0];
    canvas.next_    = &canvas.front_[0];
    return canvas;
}
ColorCanvas* ColorCanvas_withColor(RefT(ColorCanvas) self, Color color) {
    Assert(self);
    ColorBuffer_withColor(self->current_, color);
    return self;
}
ColorCanvas* ColorCanvas_withColorRange(RefT(ColorCanvas) self, Color color, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= ColorBuffer_size(self->current_));
    ColorBuffer_withColorRange(self->current_, color, rangeBegin, rangeEnd);
    return self;
}
ColorBuffer* ColorCanvas_buffer(RefT(ColorCanvas) self) {
    Assert(self);
    return self->next_;
}
Color* ColorCanvas_at(RefT(ColorCanvas) self, usize x, usize y) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < ColorBuffer_width(ColorCanvas_buffer(self)));
    Assert(0 <= y);
    Assert(y < ColorBuffer_height(ColorCanvas_buffer(self)));
    return ColorBuffer_at(ColorCanvas_buffer(self), x, y);
}
// TODO: Impl Slice and Range Type
Color* ColorCanvas_GetRange(RefT(ColorCanvas) self, usize rangeBegin, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= ColorBuffer_size(ColorCanvas_buffer(self)));
    return ColorBuffer_GetRange(ColorCanvas_buffer(self), rangeBegin, rangeEnd);
}
void ColorCanvas_SetRange(RefT(ColorCanvas) self, usize rangeBegin, usize rangeEnd, Color color) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= ColorBuffer_size(ColorCanvas_buffer(self)));
    ColorBuffer_SetRange(ColorCanvas_buffer(self), color, rangeBegin, rangeEnd);
}
void ColorCanvas_SetRangeColors(RefT(ColorCanvas) self, usize rangeBegin, usize rangeEnd, Color* colors) {
    Assert(self);
    Assert(0 <= rangeBegin);
    Assert(rangeBegin <= rangeEnd);
    Assert(rangeEnd <= ColorBuffer_size(ColorCanvas_buffer(self)));
    ColorBuffer_SetRangeColors(ColorCanvas_buffer(self), colors, rangeBegin, rangeEnd);
}
void ColorCanvas_Clear(RefT(ColorCanvas) self, Color color) {
    Assert(self);
    ColorBuffer_Clear(ColorCanvas_buffer(self), color);
}
void ColorCanvas_Swap(RefT(ColorCanvas) self) {
    Assert(self);
    Swap(ColorBuffer*, self->current_, self->next_);
}
void ColorCanvas_Draw(RefT(ColorCanvas) self, u32 x, u32 y, Color color) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < ColorBuffer_width(ColorCanvas_buffer(self)));
    Assert(0 <= y);
    Assert(y < ColorBuffer_height(ColorCanvas_buffer(self)));
    ColorBuffer_at(ColorCanvas_buffer(self), x, y)[0] = color;
}
void ColorCanvas_DrawPoint(RefT(ColorCanvas) self, u32 x, u32 y, Color color) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < ColorBuffer_width(ColorCanvas_buffer(self)));
    Assert(0 <= y);
    Assert(y < ColorBuffer_height(ColorCanvas_buffer(self)));
    ColorBuffer_at(ColorCanvas_buffer(self), x, y)[0] = color;
}
