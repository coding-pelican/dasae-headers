#include "canvas.h"
#include "assert.h"
#include "common.h"
#include "ints.h"
#include "mem.h"


FrameBuffer* FrameBuffer_init(Ref(FrameBuffer) self, u32 width, u32 height) {
    assert(self);
    assert(0 < width);
    assert(0 < height);
    *self = FrameBuffer_(
            .data_   = mem_new(Color, (usize)width * height),
            .width_  = width,
            .height_ = height,
            .size_   = width * height
    );
    assert(self->data_);
    assert(0 < self->size_);
    return self;
}

FrameBuffer* FrameBuffer_initWithColor(Ref(FrameBuffer) self, Color color) {
    assert(self);
    for (usize i = 0; i < self->size_; ++i) {
        self->data_[i] = color;
    }
    return self;
}

FrameBuffer* FrameBuffer_initWithColorRange(Ref(FrameBuffer) self, Color color, usize range_start, usize range_end) {
    assert(self);
    assert(0 <= range_start);
    assert(range_start < range_end);
    assert(range_end <= self->size_);
    for (usize i = range_start; i < range_end; ++i) {
        self->data_[i] = color;
    }
    return self;
}

void FrameBuffer_fini(Ref(FrameBuffer) self) {
    assert(self);
    mem_delete(&self->data_);
    self->data_   = null;
    self->width_  = 0;
    self->height_ = 0;
    self->size_   = 0;
}

const Color* FrameBuffer_readData(const Ref(FrameBuffer) self) {
    assert(self);
    return self->data_;
}

Color* FrameBuffer_accessData(Ref(FrameBuffer) self) {
    assert(self);
    return self->data_;
}

u32 FrameBuffer_width(const Ref(FrameBuffer) self) {
    assert(self);
    return self->width_;
}

u32 FrameBuffer_height(const Ref(FrameBuffer) self) {
    assert(self);
    return self->height_;
}

u32 FrameBuffer_size(const Ref(FrameBuffer) self) {
    assert(self);
    return self->size_;
}

const Color* FrameBuffer_readAt(const Ref(FrameBuffer) self, usize x, usize y) {
    assert(self);
    assert(0 <= x);
    assert(x < self->width_);
    assert(0 <= y);
    assert(y < self->height_);
    return self->data_ + (y * self->width_) + x;
}

Color* FrameBuffer_accessAt(Ref(FrameBuffer) self, usize x, usize y) {
    assert(self);
    assert(0 <= x);
    assert(x < self->width_);
    assert(0 <= y);
    assert(y < self->height_);
    return self->data_ + (y * self->width_) + x;
}

Color FrameBuffer_get(const Ref(FrameBuffer) self, usize x, usize y) {
    assert(self);
    assert(0 <= x);
    assert(x < self->width_);
    assert(0 <= y);
    assert(y < self->height_);
    return self->data_[(y * self->width_) + x];
}

void FrameBuffer_set(Ref(FrameBuffer) self, Color color, usize x, usize y) {
    assert(self);
    assert(0 <= x);
    assert(x < self->width_);
    assert(0 <= y);
    assert(y < self->height_);
    self->data_[(y * self->width_) + x] = color;
}

// TODO: Impl Slice and Range Type
Color* FrameBuffer_getRange(const Ref(FrameBuffer) self, usize range_start, usize range_end) {
    assert(self);
    assert(0 <= range_start);
    assert(range_start <= range_end);
    assert(range_end <= self->size_);
    return self->data_ + range_start;
}

void FrameBuffer_setRange(Ref(FrameBuffer) self, Color color, usize range_start, usize range_end) {
    assert(self);
    assert(0 <= range_start);
    assert(range_start < range_end);
    assert(range_end <= self->size_);
    for (usize i = range_start; i < range_end; ++i) {
        self->data_[i] = color;
    }
}

void FrameBuffer_setRangeColors(Ref(FrameBuffer) self, Color* colors, usize range_start, usize range_end) {
    assert(self);
    assert(0 <= range_start);
    assert(range_start < range_end);
    assert(range_end <= self->size_);
    for (usize i = range_start; i < range_end; ++i) {
        self->data_[i] = colors[i];
    }
}

void FrameBuffer_clear(Ref(FrameBuffer) self, Color color) {
    assert(self);
    for (usize i = 0; i < self->size_; ++i) {
        self->data_[i] = color;
    }
}

// TODO: Blitting functions
Canvas* Canvas_init(Ref(Canvas) self, u32 width, u32 height) {
    assert(self);
    assert(0 < width);
    assert(0 < height);
    self->current_ = self->front_;
    self->next_    = self->back_;
    FrameBuffer_init(self->current_, width, height);
    FrameBuffer_init(self->next_, width, height);
    return self;
};

Canvas* Canvas_initWithColor(Ref(Canvas) self, Color color) {
    assert(self);
    FrameBuffer_initWithColor(self->current_, color);
    FrameBuffer_initWithColor(self->next_, color);
    return self;
}

Canvas* Canvas_initWithColorRange(Ref(Canvas) self, Color color, usize range_start, usize range_end) {
    assert(self);
    assert(0 <= range_start);
    assert(range_start <= range_end);
    assert(range_end <= FrameBuffer_size(self->current_));
    FrameBuffer_initWithColorRange(self->current_, color, range_start, range_end);
    FrameBuffer_initWithColorRange(self->next_, color, range_start, range_end);
    return self;
}

void Canvas_fini(Ref(Canvas) self) {
    assert(self);
    FrameBuffer_fini(self->current_);
    FrameBuffer_fini(self->next_);
    self->current_ = null;
    self->next_    = null;
}

// void delete_Canvas(Ref(Canvas*) canvas) {
//     assert(canvas);
//     assert(*canvas);
//     if ((*canvas)->current_) {
//         Canvas_fini(*canvas);
//         assert((*canvas)->current_ == null);
//     }
//     free(*canvas);
//     *canvas = null;
// }

const FrameBuffer* Canvas_readBuffer(const Ref(Canvas) self) {
    assert(self);
    return self->next_;
}

FrameBuffer* Canvas_accessBuffer(Ref(Canvas) self) {
    assert(self);
    return self->next_;
}

const Color* Canvas_readAt(const Ref(Canvas) self, usize x, usize y) {
    assert(self);
    assert(0 <= x);
    assert(x < FrameBuffer_width(Canvas_readBuffer(self)));
    assert(0 <= y);
    assert(y < FrameBuffer_height(Canvas_readBuffer(self)));
    return FrameBuffer_readAt(Canvas_readBuffer(self), x, y);
}

Color* Canvas_accessAt(Ref(Canvas) self, usize x, usize y) {
    assert(self);
    assert(0 <= x);
    assert(x < FrameBuffer_width(Canvas_accessBuffer(self)));
    assert(0 <= y);
    assert(y < FrameBuffer_height(Canvas_accessBuffer(self)));
    return FrameBuffer_accessAt(Canvas_accessBuffer(self), x, y);
}

Color Canvas_get(const Ref(Canvas) self, usize x, usize y) {
    assert(self);
    assert(0 <= x);
    assert(x < FrameBuffer_width(Canvas_readBuffer(self)));
    assert(0 <= y);
    assert(y < FrameBuffer_height(Canvas_readBuffer(self)));
    return FrameBuffer_get(Canvas_readBuffer(self), x, y);
}

void Canvas_set(Ref(Canvas) self, usize x, usize y, Color color) {
    assert(self);
    assert(0 <= x);
    assert(x < FrameBuffer_width(Canvas_accessBuffer(self)));
    assert(0 <= y);
    assert(y < FrameBuffer_height(Canvas_accessBuffer(self)));
    FrameBuffer_set(Canvas_accessBuffer(self), color, x, y);
}

// TODO: Impl Slice and Range Type
Color* Canvas_getRange(const Ref(Canvas) self, usize range_start, usize range_end) {
    assert(self);
    assert(0 <= range_start);
    assert(range_start < range_end);
    assert(range_end <= FrameBuffer_size(Canvas_readBuffer(self)));
    return FrameBuffer_getRange(Canvas_readBuffer(self), range_start, range_end);
}

void Canvas_setRange(Ref(Canvas) self, usize range_start, usize range_end, Color color) {
    assert(self);
    assert(0 <= range_start);
    assert(range_start < range_end);
    assert(range_end <= FrameBuffer_size(Canvas_accessBuffer(self)));
    FrameBuffer_setRange(Canvas_accessBuffer(self), color, range_start, range_end);
}

void Canvas_setRangeColors(Ref(Canvas) self, usize range_start, usize range_end, Color* colors) {
    assert(self);
    assert(0 <= range_start);
    assert(range_start < range_end);
    assert(range_end <= FrameBuffer_size(Canvas_accessBuffer(self)));
    FrameBuffer_setRangeColors(Canvas_accessBuffer(self), colors, range_start, range_end);
}

void Canvas_clear(Ref(Canvas) self, Color color) {
    assert(self);
    FrameBuffer_clear(Canvas_accessBuffer(self), color);
}

void Canvas_swap(Ref(Canvas) self) {
    assert(self);
    pp_swap(FrameBuffer*, self->current_, self->next_);
}

void Canvas_draw(Ref(Canvas) self, u32 x, u32 y, Color color) {
    assert(self);
    assert(0 <= x);
    assert(x < FrameBuffer_width(Canvas_accessBuffer(self)));
    assert(0 <= y);
    assert(y < FrameBuffer_height(Canvas_accessBuffer(self)));
    FrameBuffer_set(Canvas_accessBuffer(self), color, x, y);
}

void Canvas_drawPoint(Ref(Canvas) self, u32 x, u32 y, Color color) {
    assert(self);
    assert(0 <= x);
    assert(x < FrameBuffer_width(Canvas_accessBuffer(self)));
    assert(0 <= y);
    assert(y < FrameBuffer_height(Canvas_accessBuffer(self)));
    Canvas_draw(self, x, y, color);
}

void Canvas_fillRect(Ref(Canvas) self, Rect rect, Color color) {
    assert(self);
    assert(0 <= rect.x);
    assert(rect.x < FrameBuffer_width(Canvas_accessBuffer(self)));
    assert(0 <= rect.y);
    assertFmt(
        rect.y <= FrameBuffer_height(Canvas_accessBuffer(self)),
        "%lld <= %u (%lld %lld %lld %lld)",
        rect.y,
        FrameBuffer_height(Canvas_accessBuffer(self)),
        rect.x,
        rect.y,
        rect.width,
        rect.height
    );
    assert(0 <= rect.width);
    assert(rect.x + rect.width <= FrameBuffer_width(Canvas_accessBuffer(self)));
    assert(0 <= rect.height);
    assertFmt(
        rect.y + rect.height <= FrameBuffer_height(Canvas_accessBuffer(self)),
        "%lld + %lld <= %u (%lld %lld %lld %lld)",
        rect.y,
        rect.height,
        FrameBuffer_height(Canvas_accessBuffer(self)),
        rect.x,
        rect.y,
        rect.width,
        rect.height
    );
    for (u32 y = rect.y; y < rect.y + rect.height; ++y) {
        for (u32 x = rect.x; x < rect.x + rect.width; ++x) {
            Canvas_draw(self, x, y, color);
        }
    }
}

Rect Canvas_normalizeRect(Ref(Canvas) self, Rect rect) {
    assert(self);

    // Clamp x and y to valid range
    rect.x = i64_max(0, i64_min(rect.x, FrameBuffer_width(Canvas_readBuffer(self)) - 1));
    rect.y = i64_max(0, i64_min(rect.y, FrameBuffer_height(Canvas_readBuffer(self)) - 1));

    // Ensure width and height don't exceed buffer boundaries
    rect.width  = i64_min(rect.width, FrameBuffer_width(Canvas_accessBuffer(self)) - rect.x);
    rect.height = i64_min(rect.height, FrameBuffer_height(Canvas_accessBuffer(self)) - rect.y);

    return rect;
}
