#include "canvas.h"
#include "assert.h"
#include "common.h"
#include "ints.h"


FrameBuffer* new_FrameBuffer() {
    FrameBuffer* buffer = (FrameBuffer*)malloc(sizeof(FrameBuffer));
    Assert(buffer);
    return buffer;
}

FrameBuffer* FrameBuffer_Init(Ref(FrameBuffer) self, u32 width, u32 height) {
    Assert(self);
    Assert(0 < width);
    Assert(0 < height);
    *self = (FrameBuffer){
        .data_   = (Color*)malloc((usize)width * (usize)height * sizeof(Color)),
        .width_  = width,
        .height_ = height,
        .size_   = width * height
    };
    Assert(self->data_);
    Assert(0 < self->size_);
    return self;
}

FrameBuffer* FrameBuffer_InitWithColor(Ref(FrameBuffer) self, Color color) {
    Assert(self);
    for (usize i = 0; i < self->size_; ++i) {
        self->data_[i] = color;
    }
    return self;
}

FrameBuffer* FrameBuffer_InitWithColorRange(Ref(FrameBuffer) self, Color color, usize rangeStart, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeStart);
    Assert(rangeStart < rangeEnd);
    Assert(rangeEnd <= self->size_);
    for (usize i = rangeStart; i < rangeEnd; ++i) {
        self->data_[i] = color;
    }
    return self;
}

void FrameBuffer_Fini(Ref(FrameBuffer) self) {
    Assert(self);
    free(self->data_);
    self->data_   = nullptr;
    self->width_  = 0;
    self->height_ = 0;
    self->size_   = 0;
}

void delete_FrameBuffer(Ref(FrameBuffer*) buffer) {
    Assert(buffer);
    Assert(*buffer);
    if ((*buffer)->data_) {
        FrameBuffer_Fini(*buffer);
        Assert((*buffer)->data_ == nullptr);
    }
    free(*buffer);
    *buffer = nullptr;
}

const Color* FrameBuffer_ReadData(const Ref(FrameBuffer) self) {
    Assert(self);
    return self->data_;
}

Color* FrameBuffer_AccessData(Ref(FrameBuffer) self) {
    Assert(self);
    return self->data_;
}

u32 FrameBuffer_Width(const Ref(FrameBuffer) self) {
    Assert(self);
    return self->width_;
}

u32 FrameBuffer_Height(const Ref(FrameBuffer) self) {
    Assert(self);
    return self->height_;
}

u32 FrameBuffer_Size(const Ref(FrameBuffer) self) {
    Assert(self);
    return self->size_;
}

const Color* FrameBuffer_ReadAt(const Ref(FrameBuffer) self, usize x, usize y) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < self->width_);
    Assert(0 <= y);
    Assert(y < self->height_);
    return self->data_ + (y * self->width_) + x;
}

Color* FrameBuffer_AccessAt(Ref(FrameBuffer) self, usize x, usize y) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < self->width_);
    Assert(0 <= y);
    Assert(y < self->height_);
    return self->data_ + (y * self->width_) + x;
}

Color FrameBuffer_Get(const Ref(FrameBuffer) self, usize x, usize y) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < self->width_);
    Assert(0 <= y);
    Assert(y < self->height_);
    return self->data_[(y * self->width_) + x];
}

void FrameBuffer_Set(Ref(FrameBuffer) self, Color color, usize x, usize y) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < self->width_);
    Assert(0 <= y);
    Assert(y < self->height_);
    self->data_[(y * self->width_) + x] = color;
}

// TODO: Impl Slice and Range Type
Color* FrameBuffer_GetRange(const Ref(FrameBuffer) self, usize rangeStart, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeStart);
    Assert(rangeStart <= rangeEnd);
    Assert(rangeEnd <= self->size_);
    return self->data_ + rangeStart;
}

void FrameBuffer_SetRange(Ref(FrameBuffer) self, Color color, usize rangeStart, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeStart);
    Assert(rangeStart < rangeEnd);
    Assert(rangeEnd <= self->size_);
    for (usize i = rangeStart; i < rangeEnd; ++i) {
        self->data_[i] = color;
    }
}

void FrameBuffer_SetRangeColors(Ref(FrameBuffer) self, Color* colors, usize rangeStart, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeStart);
    Assert(rangeStart < rangeEnd);
    Assert(rangeEnd <= self->size_);
    for (usize i = rangeStart; i < rangeEnd; ++i) {
        self->data_[i] = colors[i];
    }
}

void FrameBuffer_Clear(Ref(FrameBuffer) self, Color color) {
    Assert(self);
    for (usize i = 0; i < self->size_; ++i) {
        self->data_[i] = color;
    }
}

// TODO: Blitting functions
Canvas* new_Canvas() {
    Canvas* canvas = (Canvas*)malloc(sizeof(Canvas));
    Assert(canvas);
    return canvas;
}
Canvas* Canvas_Init(Ref(Canvas) self, u32 width, u32 height) {
    Assert(self);
    Assert(0 < width);
    Assert(0 < height);
    self->current_ = self->front_;
    self->next_    = self->back_;
    FrameBuffer_Init(self->current_, width, height);
    FrameBuffer_Init(self->next_, width, height);
    return self;
};

Canvas* Canvas_InitWithColor(Ref(Canvas) self, Color color) {
    Assert(self);
    FrameBuffer_InitWithColor(self->current_, color);
    FrameBuffer_InitWithColor(self->next_, color);
    return self;
}

Canvas* Canvas_InitWithColorRange(Ref(Canvas) self, Color color, usize rangeStart, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeStart);
    Assert(rangeStart <= rangeEnd);
    Assert(rangeEnd <= FrameBuffer_Size(self->current_));
    FrameBuffer_InitWithColorRange(self->current_, color, rangeStart, rangeEnd);
    FrameBuffer_InitWithColorRange(self->next_, color, rangeStart, rangeEnd);
    return self;
}

void Canvas_Fini(Ref(Canvas) self) {
    Assert(self);
    FrameBuffer_Fini(self->current_);
    FrameBuffer_Fini(self->next_);
    self->current_ = nullptr;
    self->next_    = nullptr;
}

void delete_Canvas(Ref(Canvas*) canvas) {
    Assert(canvas);
    Assert(*canvas);
    if ((*canvas)->current_) {
        Canvas_Fini(*canvas);
        Assert((*canvas)->current_ == nullptr);
    }
    free(*canvas);
    *canvas = nullptr;
}

const FrameBuffer* Canvas_ReadBuffer(const Ref(Canvas) self) {
    Assert(self);
    return self->next_;
}

FrameBuffer* Canvas_AccessBuffer(Ref(Canvas) self) {
    Assert(self);
    return self->next_;
}

const Color* Canvas_ReadAt(const Ref(Canvas) self, usize x, usize y) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < FrameBuffer_Width(Canvas_ReadBuffer(self)));
    Assert(0 <= y);
    Assert(y < FrameBuffer_Height(Canvas_ReadBuffer(self)));
    return FrameBuffer_ReadAt(Canvas_ReadBuffer(self), x, y);
}

Color* Canvas_AccessAt(Ref(Canvas) self, usize x, usize y) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < FrameBuffer_Width(Canvas_AccessBuffer(self)));
    Assert(0 <= y);
    Assert(y < FrameBuffer_Height(Canvas_AccessBuffer(self)));
    return FrameBuffer_AccessAt(Canvas_AccessBuffer(self), x, y);
}

Color Canvas_Get(const Ref(Canvas) self, usize x, usize y) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < FrameBuffer_Width(Canvas_ReadBuffer(self)));
    Assert(0 <= y);
    Assert(y < FrameBuffer_Height(Canvas_ReadBuffer(self)));
    return FrameBuffer_Get(Canvas_ReadBuffer(self), x, y);
}

void Canvas_Set(Ref(Canvas) self, usize x, usize y, Color color) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < FrameBuffer_Width(Canvas_AccessBuffer(self)));
    Assert(0 <= y);
    Assert(y < FrameBuffer_Height(Canvas_AccessBuffer(self)));
    FrameBuffer_Set(Canvas_AccessBuffer(self), color, x, y);
}

// TODO: Impl Slice and Range Type
Color* Canvas_GetRange(const Ref(Canvas) self, usize rangeStart, usize rangeEnd) {
    Assert(self);
    Assert(0 <= rangeStart);
    Assert(rangeStart < rangeEnd);
    Assert(rangeEnd <= FrameBuffer_Size(Canvas_ReadBuffer(self)));
    return FrameBuffer_GetRange(Canvas_ReadBuffer(self), rangeStart, rangeEnd);
}

void Canvas_SetRange(Ref(Canvas) self, usize rangeStart, usize rangeEnd, Color color) {
    Assert(self);
    Assert(0 <= rangeStart);
    Assert(rangeStart < rangeEnd);
    Assert(rangeEnd <= FrameBuffer_Size(Canvas_AccessBuffer(self)));
    FrameBuffer_SetRange(Canvas_AccessBuffer(self), color, rangeStart, rangeEnd);
}

void Canvas_SetRangeColors(Ref(Canvas) self, usize rangeStart, usize rangeEnd, Color* colors) {
    Assert(self);
    Assert(0 <= rangeStart);
    Assert(rangeStart < rangeEnd);
    Assert(rangeEnd <= FrameBuffer_Size(Canvas_AccessBuffer(self)));
    FrameBuffer_SetRangeColors(Canvas_AccessBuffer(self), colors, rangeStart, rangeEnd);
}

void Canvas_Clear(Ref(Canvas) self, Color color) {
    Assert(self);
    FrameBuffer_Clear(Canvas_AccessBuffer(self), color);
}

void Canvas_Swap(Ref(Canvas) self) {
    Assert(self);
    Swap(FrameBuffer*, self->current_, self->next_);
}

void Canvas_Draw(Ref(Canvas) self, u32 x, u32 y, Color color) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < FrameBuffer_Width(Canvas_AccessBuffer(self)));
    Assert(0 <= y);
    Assert(y < FrameBuffer_Height(Canvas_AccessBuffer(self)));
    FrameBuffer_Set(Canvas_AccessBuffer(self), color, x, y);
}

void Canvas_DrawPoint(Ref(Canvas) self, u32 x, u32 y, Color color) {
    Assert(self);
    Assert(0 <= x);
    Assert(x < FrameBuffer_Width(Canvas_AccessBuffer(self)));
    Assert(0 <= y);
    Assert(y < FrameBuffer_Height(Canvas_AccessBuffer(self)));
    Canvas_Draw(self, x, y, color);
}

void Canvas_FillRect(Ref(Canvas) self, Rect rect, Color color) {
    Assert(self);
    Assert(0 <= rect.x);
    Assert(rect.x < FrameBuffer_Width(Canvas_AccessBuffer(self)));
    Assert(0 <= rect.y);
    AssertFmt(
        rect.y <= FrameBuffer_Height(Canvas_AccessBuffer(self)),
        "%lld <= %u (%lld %lld %lld %lld)",
        rect.y,
        FrameBuffer_Height(Canvas_AccessBuffer(self)),
        rect.x,
        rect.y,
        rect.width,
        rect.height
    );
    Assert(0 <= rect.width);
    Assert(rect.x + rect.width <= FrameBuffer_Width(Canvas_AccessBuffer(self)));
    Assert(0 <= rect.height);
    AssertFmt(
        rect.y + rect.height <= FrameBuffer_Height(Canvas_AccessBuffer(self)),
        "%lld + %lld <= %u (%lld %lld %lld %lld)",
        rect.y,
        rect.height,
        FrameBuffer_Height(Canvas_AccessBuffer(self)),
        rect.x,
        rect.y,
        rect.width,
        rect.height
    );
    for (u32 y = rect.y; y < rect.y + rect.height; ++y) {
        for (u32 x = rect.x; x < rect.x + rect.width; ++x) {
            Canvas_Draw(self, x, y, color);
        }
    }
}

Rect Canvas_NormalizeRect(Ref(Canvas) self, Rect rect) {
    Assert(self);

    // Clamp x and y to valid range
    rect.x = i64_max(0, i64_min(rect.x, FrameBuffer_Width(Canvas_ReadBuffer(self)) - 1));
    rect.y = i64_max(0, i64_min(rect.y, FrameBuffer_Height(Canvas_ReadBuffer(self)) - 1));

    // Ensure width and height don't exceed buffer boundaries
    rect.width  = i64_min(rect.width, FrameBuffer_Width(Canvas_AccessBuffer(self)) - rect.x);
    rect.height = i64_min(rect.height, FrameBuffer_Height(Canvas_AccessBuffer(self)) - rect.y);

    return rect;
}
