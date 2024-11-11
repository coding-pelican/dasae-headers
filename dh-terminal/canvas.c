#include "canvas.h"
#include <dh/core.h>
#include <dh/mem.h>
#include <dh/cmp.h>
#include <dh/debug/assert.h>


FrameBuffer* FrameBuffer_init(FrameBuffer* const b, u32 width, u32 height) {
    debug_assertNotNull(b);
    debug_assert(0 < width);
    debug_assert(0 < height);

    usize const size = prim_as(usize, width) * height;
    debug_assert(0 < size);

    anyptr const data = mem_alloc(Color, size);
    debug_assertNotNull(data);

    *b = makeWith(
        FrameBuffer,
        .pixels_ = {
            .data     = data,
            .length   = 0,
            .capacity = size },
        .width_  = width,
        .height_ = height,
        .size_   = size
    );
    debug_assertNotNull(b->pixels_.data);

    return b;
}

FrameBuffer* FrameBuffer_withColor(FrameBuffer* const b, Color color) {
    debug_assertNotNull(b);

    for (usize i = 0; i < b->size_; ++i) {
        b->pixels_.data[i] = color;
    }

    return b;
}

// FrameBuffer* FrameBuffer_withColorRange(FrameBuffer* const b, Color color, usize range_start, usize range_end) {
//     debug_assertNotNull(b);
//     debug_assert(0 <= range_start);
//     debug_assert(range_start < range_end);
//     debug_assert(range_end <= b->size_);

//     for (usize i = range_start; i < range_end; ++i) {
//         b->pixels_.data[i] = color;
//     }

//     return b;
// }

FrameBuffer* FrameBuffer_fini(FrameBuffer* const b) {
    debug_assert(b);

    mem_free(&b->pixels_.data);
    b->pixels_.length   = 0;
    b->pixels_.capacity = 0;

    b->width_  = 0;
    b->height_ = 0;
    b->size_   = 0;

    return b;
}

const Color* FrameBuffer_peekData(const FrameBuffer* const b) {
    debug_assertNotNull(b);
    return b->pixels_.data;
}

Color* FrameBuffer_accessData(FrameBuffer* const b) {
    debug_assertNotNull(b);
    return b->pixels_.data;
}

u32 FrameBuffer_width(const FrameBuffer* const b) {
    debug_assertNotNull(b);
    return b->width_;
}

u32 FrameBuffer_height(const FrameBuffer* const b) {
    debug_assertNotNull(b);
    return b->height_;
}

u32 FrameBuffer_size(const FrameBuffer* const b) {
    debug_assertNotNull(b);
    return b->size_;
}

force_inline Color* FrameBuffer__at(FrameBuffer* b, usize x, usize y) {
    debug_assertNotNull(b);
    debug_assert(0 <= x);
    debug_assert(x < b->width_);
    debug_assert(0 <= y);
    debug_assert(y < b->height_);
    return b->pixels_.data + x + (y * b->width_);
}

const Color* FrameBuffer_peekAt(const FrameBuffer* const b, usize x, usize y) {
    return FrameBuffer__at((FrameBuffer*)b, x, y);
}

Color* FrameBuffer_accessAt(FrameBuffer* const b, usize x, usize y) {
    debug_assertNotNull(b);
    debug_assert(0 <= x);
    debug_assert(x < b->width_);
    debug_assert(0 <= y);
    debug_assert(y < b->height_);
    return FrameBuffer__at(b, x, y);
}

Color FrameBuffer_get(const FrameBuffer* const b, usize x, usize y) {
    debug_assertNotNull(b);
    debug_assert(0 <= x);
    debug_assert(x < b->width_);
    debug_assert(0 <= y);
    debug_assert(y < b->height_);
    return *FrameBuffer_peekAt(b, x, y);
}

void FrameBuffer_set(FrameBuffer* const b, Color color, usize x, usize y) {
    debug_assertNotNull(b);
    debug_assert(0 <= x);
    debug_assert(x < b->width_);
    debug_assert(0 <= y);
    debug_assert(y < b->height_);
    *FrameBuffer_accessAt(b, x, y) = color;
}

// TODO: Impl Slice and Range Type
// Color* FrameBuffer_getRange(const FrameBuffer* self, usize range_start, usize range_end) {
//     debug_assertNotNull(self);
//     debug_assert(0 <= range_start);
//     debug_assert(range_start <= range_end);
//     debug_assert(range_end <= self->size_);
//     return self->data_ + range_start;
// }

// void FrameBuffer_setRange(FrameBuffer* self, Color color, usize range_start, usize range_end) {
//     debug_assertNotNull(self);
//     debug_assert(0 <= range_start);
//     debug_assert(range_start < range_end);
//     debug_assert(range_end <= self->size_);
//     for (usize i = range_start; i < range_end; ++i) {
//         self->data_[i] = color;
//     }
// }

// void FrameBuffer_setRangeColors(FrameBuffer* self, Color* colors, usize range_start, usize range_end) {
//     debug_assertNotNull(self);
//     debug_assert(0 <= range_start);
//     debug_assert(range_start < range_end);
//     debug_assert(range_end <= self->size_);
//     for (usize i = range_start; i < range_end; ++i) {
//         self->data_[i] = colors[i];
//     }
// }

void FrameBuffer_clear(FrameBuffer* const b, Color color) {
    debug_assertNotNull(b);

    for (usize i = 0; i < b->size_; ++i) {
        b->pixels_.data[i] = color;
    }
}

// TODO: Blitting functions
Canvas* Canvas_init(Canvas* const c, u32 width, u32 height) {
    debug_assertNotNull(c);
    debug_assert(0 < width);
    debug_assert(0 < height);

    c->current_ = c->front_;
    c->next_    = c->back_;
    FrameBuffer_init(c->current_, width, height);
    FrameBuffer_init(c->next_, width, height);

    return c;
};

Canvas* Canvas_withColor(Canvas* const c, Color color) {
    debug_assertNotNull(c);
    FrameBuffer_withColor(c->current_, color);
    FrameBuffer_withColor(c->next_, color);
    return c;
}

// Canvas* Canvas_initWithColorRange(Canvas* c, Color color, usize range_start, usize range_end) {
//     debug_assertNotNull(c);
//     debug_assert(0 <= range_start);
//     debug_assert(range_start <= range_end);
//     debug_assert(range_end <= FrameBuffer_size(c->current_));
//     FrameBuffer_withColorRange(c->current_, color, range_start, range_end);
//     FrameBuffer_withColorRange(c->next_, color, range_start, range_end);
//     return c;
// }

Canvas* Canvas_fini(Canvas* const c) {
    debug_assertNotNull(c);

    FrameBuffer_fini(c->current_);
    FrameBuffer_fini(c->next_);
    c->current_ = null;
    c->next_    = null;

    return c;
}

// void delete_Canvas(Ref(Canvas*) canvas) {
//     debug_assert(canvas);
//     debug_assert(*canvas);
//     if ((*canvas)->current_) {
//         Canvas_fini(*canvas);
//         debug_assert((*canvas)->current_ == null);
//     }
//     free(*canvas);
//     *canvas = null;
// }

force_inline FrameBuffer* Canvas__buffer(Canvas* c) {
    return c->next_;
}

const FrameBuffer* Canvas_peekBuffer(const Canvas* const c) {
    debug_assertNotNull(c);
    return Canvas__buffer((Canvas*)c);
}

FrameBuffer* Canvas_accessBuffer(Canvas* const c) {
    debug_assertNotNull(c);
    return Canvas__buffer(c);
}

const Color* Canvas_peekAt(const Canvas* const c, usize x, usize y) {
    debug_assertNotNull(c);
    debug_assert(0 <= x);
    debug_assert(x < FrameBuffer_width(Canvas_peekBuffer(c)));
    debug_assert(0 <= y);
    debug_assert(y < FrameBuffer_height(Canvas_peekBuffer(c)));
    return FrameBuffer_peekAt(Canvas_peekBuffer(c), x, y);
}

Color* Canvas_accessAt(Canvas* const c, usize x, usize y) {
    debug_assertNotNull(c);
    debug_assert(0 <= x);
    debug_assert(x < FrameBuffer_width(Canvas_accessBuffer(c)));
    debug_assert(0 <= y);
    debug_assert(y < FrameBuffer_height(Canvas_accessBuffer(c)));
    return FrameBuffer_accessAt(Canvas_accessBuffer(c), x, y);
}

Color Canvas_get(const Canvas* const c, usize x, usize y) {
    debug_assertNotNull(c);
    debug_assert(0 <= x);
    debug_assert(x < FrameBuffer_width(Canvas_peekBuffer(c)));
    debug_assert(0 <= y);
    debug_assert(y < FrameBuffer_height(Canvas_peekBuffer(c)));
    return FrameBuffer_get(Canvas_peekBuffer(c), x, y);
}

void Canvas_set(Canvas* const c, usize x, usize y, Color color) {
    debug_assertNotNull(c);
    debug_assert(0 <= x);
    debug_assert(x < FrameBuffer_width(Canvas_accessBuffer(c)));
    debug_assert(0 <= y);
    debug_assert(y < FrameBuffer_height(Canvas_accessBuffer(c)));
    FrameBuffer_set(Canvas_accessBuffer(c), color, x, y);
}

// TODO: Impl Slice and Range Type
// Color* Canvas_getRange(const Canvas* self, usize range_start, usize range_end) {
//     debug_assertNotNull(self);
//     debug_assert(0 <= range_start);
//     debug_assert(range_start < range_end);
//     debug_assert(range_end <= FrameBuffer_size(Canvas_peekBuffer(self)));
//     return FrameBuffer_getRange(Canvas_peekBuffer(self), range_start, range_end);
// }

// void Canvas_setRange(Canvas* self, usize range_start, usize range_end, Color color) {
//     debug_assertNotNull(self);
//     debug_assert(0 <= range_start);
//     debug_assert(range_start < range_end);
//     debug_assert(range_end <= FrameBuffer_size(Canvas_accessBuffer(self)));
//     FrameBuffer_setRange(Canvas_accessBuffer(self), color, range_start, range_end);
// }

// void Canvas_setRangeColors(Canvas* self, usize range_start, usize range_end, Color* colors) {
//     debug_assertNotNull(self);
//     debug_assert(0 <= range_start);
//     debug_assert(range_start < range_end);
//     debug_assert(range_end <= FrameBuffer_size(Canvas_accessBuffer(self)));
//     FrameBuffer_setRangeColors(Canvas_accessBuffer(self), colors, range_start, range_end);
// }

void Canvas_clear(Canvas* const c, Color color) {
    debug_assertNotNull(c);
    FrameBuffer_clear(Canvas_accessBuffer(c), color);
}

void Canvas_swap(Canvas* const c) {
    debug_assertNotNull(c);
    swap(FrameBuffer*, c->current_, c->next_);
}

force_inline void Canvas__draw(Canvas* c, u32 x, u32 y, Color color) {
    FrameBuffer_set(Canvas_accessBuffer(c), color, x, y);
}

void Canvas_draw(Canvas* const c, u32 x, u32 y, Color color) {
    debug_assertNotNull(c);
    debug_assert(0 <= x);
    debug_assert(x < FrameBuffer_width(Canvas_accessBuffer(c)));
    debug_assert(0 <= y);
    debug_assert(y < FrameBuffer_height(Canvas_accessBuffer(c)));
    Canvas__draw(c, x, y, color);
}

void Canvas_drawPoint(Canvas* const c, u32 x, u32 y, Color color) {
    debug_assertNotNull(c);
    debug_assert(0 <= x);
    debug_assert(x < FrameBuffer_width(Canvas_accessBuffer(c)));
    debug_assert(0 <= y);
    debug_assert(y < FrameBuffer_height(Canvas_accessBuffer(c)));
    Canvas__draw(c, x, y, color);
}

void Canvas_fillRect(Canvas* const c, Rect rect, Color color) {
    debug_assertNotNull(c);
    debug_assert(0 <= rect.x);
    debug_assert(rect.x < FrameBuffer_width(Canvas_accessBuffer(c)));
    debug_assert(0 <= rect.y);
    debug_assertFmt(
        rect.y <= FrameBuffer_height(Canvas_accessBuffer(c)),
        "%lld <= %u (%lld %lld %lld %lld)",
        rect.y,
        FrameBuffer_height(Canvas_accessBuffer(c)),
        rect.x,
        rect.y,
        rect.width,
        rect.height
    );
    debug_assert(0 <= rect.width);
    debug_assert(rect.x + rect.width <= FrameBuffer_width(Canvas_accessBuffer(c)));
    debug_assert(0 <= rect.height);
    debug_assertFmt(
        rect.y + rect.height <= FrameBuffer_height(Canvas_accessBuffer(c)),
        "%lld + %lld <= %u (%lld %lld %lld %lld)",
        rect.y,
        rect.height,
        FrameBuffer_height(Canvas_accessBuffer(c)),
        rect.x,
        rect.y,
        rect.width,
        rect.height
    );

    for (u32 y = rect.y; y < rect.y + rect.height; ++y) {
        for (u32 x = rect.x; x < rect.x + rect.width; ++x) {
            Canvas__draw(c, x, y, color);
        }
    }
}

// #define clamp(TYPE, VAL, LOW, HIGH) pp_func(\
//     pp_type(TYPE),
//     pp_name(clamp),
//     pp_params(
//         pp_type(TYPE), VAL,
//         pp_type(TYPE), LOW,
//         pp_type(TYPE), HIGH
//     ),
//     pp_body(
//         pp_type(TYPE) result = VAL;
//         if (result < LOW) {
//     result = LOW;
//         } else if (result > HIGH) {
//     result = HIGH;
//         }
//         return result;
//     )
// )

cmp_Ordering i64_cmp(const i64 lhs, const i64 rhs) {
    return lhs < rhs ? cmp_Ordering_less
         : lhs > rhs ? cmp_Ordering_greater
                     : cmp_Ordering_equal;
}

force_inline i64 i64_min(i64 lhs, i64 rhs) {
    return lhs < rhs ? lhs : rhs;
}
force_inline i64 i64_max(i64 lhs, i64 rhs) {
    return lhs > rhs ? lhs : rhs;
}
force_inline i64 i64_clamp(i64 val, i64 low, i64 high) {
    return i64_min(i64_max(low, val), high);
}

Rect Canvas_normalizeRect(Canvas* const c, Rect rect) {
    debug_assertNotNull(c);

    // Clamp x and y to valid range
    rect.x = i64_clamp(rect.x, 0, FrameBuffer_width(Canvas_peekBuffer(c)) - 1);
    rect.y = i64_clamp(rect.y, 0, FrameBuffer_height(Canvas_peekBuffer(c)) - 1);

    // Ensure width and height don't exceed buffer boundaries
    rect.width  = i64_min(rect.width, FrameBuffer_width(Canvas_accessBuffer(c)) - rect.x);
    rect.height = i64_min(rect.height, FrameBuffer_height(Canvas_accessBuffer(c)) - rect.y);

    return rect;
}
