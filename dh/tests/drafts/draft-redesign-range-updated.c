#ifndef RANGE_H
#define RANGE_H

#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

typedef unsigned char u8;
typedef size_t usize;

// ============================================================================
// Type Definitions
// ============================================================================

typedef enum R_Bound_Tag : u8 {
    R_Bound_Tag_incl = 0,
    R_Bound_Tag_excl = 1,
} R_Bound_Tag;

typedef struct R_Unbounded {
    R_Bound_Tag begin;
    R_Bound_Tag end;
} R_Unbounded;

typedef struct R_Bound {
    R_Bound_Tag tag;
    usize       point;
} R_Bound;

typedef struct R_Bounded {
    usize begin;
    usize end;
} R_Bounded;

typedef enum R_Tag : u8 {
    R_Tag_unbounded = 0,
    R_Tag_from      = 1,
    R_Tag_to        = 2,
    R_Tag_bounded   = 3,
} R_Tag;

typedef struct R {
    R_Tag tag;
    union {
        R_Unbounded unbounded;
        R_Bound     from;
        R_Bound     to;
        R_Bounded   bounded;
    } payload;
} R;

// ============================================================================
// Function Declarations
// ============================================================================

// Bound helpers
static inline __attribute__((always_inline))
usize
R_Bound_begin(R_Bound_Tag tag, usize point);

static inline __attribute__((always_inline))
usize
R_Bound_end(R_Bound_Tag tag, usize point);

static inline __attribute__((always_inline))
usize
R_Bound_from(R_Bound self);

static inline __attribute__((always_inline))
usize
R_Bound_to(R_Bound self);

// Constructors
static inline __attribute__((always_inline))
R
R_unbounded(R_Bound_Tag begin, R_Bound_Tag end);

static inline __attribute__((always_inline))
R
R_from(R_Bound_Tag bound, usize begin);

static inline __attribute__((always_inline))
R
R_to(R_Bound_Tag bound, usize end);

static inline __attribute__((always_inline))
R
R_bounded(usize begin, usize end);

// Queries
static inline __attribute__((always_inline)) bool R_isValid(R self);

static inline __attribute__((always_inline)) bool R_contains(R self, usize idx);

static inline __attribute__((always_inline))
usize
R_len(R self);

static inline __attribute__((always_inline))
usize
R_at(R self, usize idx);

// Operations
static inline __attribute__((always_inline))
R
R_slice(R self, R range);

static inline __attribute__((always_inline))
R
R_prefix(R self, usize end);

static inline __attribute__((always_inline))
R
R_suffix(R self, usize begin);

// Comparison
static inline __attribute__((always_inline)) bool R_eq(R lhs, R rhs);

// ============================================================================
// Function Implementations
// ============================================================================

static inline __attribute__((always_inline))
usize
R_Bound_begin(R_Bound_Tag tag, usize point) {
    return tag == R_Bound_Tag_incl ? point : point + 1;
}

static inline __attribute__((always_inline))
usize
R_Bound_end(R_Bound_Tag tag, usize point) {
    return tag == R_Bound_Tag_excl ? point : point + 1;
}

static inline __attribute__((always_inline))
usize
R_Bound_from(R_Bound self) {
    return R_Bound_begin(self.tag, self.point);
}

static inline __attribute__((always_inline))
usize
R_Bound_to(R_Bound self) {
    return R_Bound_end(self.tag, self.point);
}

static inline __attribute__((always_inline))
R
R_unbounded(R_Bound_Tag begin, R_Bound_Tag end) {
    return (R){
        .tag     = R_Tag_unbounded,
        .payload = { .unbounded = { .begin = begin, .end = end } }
    };
}

static inline __attribute__((always_inline))
R
R_from(R_Bound_Tag bound, usize begin) {
    return (R){
        .tag     = R_Tag_from,
        .payload = { .from = { .tag = bound, .point = begin } }
    };
}

static inline __attribute__((always_inline))
R
R_to(R_Bound_Tag bound, usize end) {
    return (R){
        .tag     = R_Tag_to,
        .payload = { .to = { .tag = bound, .point = end } }
    };
}

static inline __attribute__((always_inline))
R
R_bounded(usize begin, usize end) {
    return (R){
        .tag     = R_Tag_bounded,
        .payload = { .bounded = { .begin = begin, .end = end } }
    };
}

static inline __attribute__((always_inline)) bool R_isValid(R self) {
    switch (self.tag) {
    case R_Tag_unbounded:
    case R_Tag_from:
    case R_Tag_to:
        return true;
    case R_Tag_bounded:
        return self.payload.bounded.begin <= self.payload.bounded.end;
    default:
        return false;
    }
}

static inline __attribute__((always_inline)) bool R_contains(R self, usize idx) {
    usize begin, end;

    switch (self.tag) {
    case R_Tag_unbounded:
        return true;
    case R_Tag_from:
        begin = R_Bound_from(self.payload.from);
        return idx >= begin;
    case R_Tag_to:
        end = R_Bound_to(self.payload.to);
        return idx < end;
    case R_Tag_bounded:
        return idx >= self.payload.bounded.begin && idx < self.payload.bounded.end;
    default:
        return false;
    }
}

static inline __attribute__((always_inline))
usize
R_len(R self) {
    switch (self.tag) {
    case R_Tag_bounded:
        return self.payload.bounded.end - self.payload.bounded.begin;
    case R_Tag_to:
        // [0, end) → length is end
        return R_Bound_to(self.payload.to);
    case R_Tag_from:
    case R_Tag_unbounded:
        // Infinite length - not allowed
        assert(false && "R_len requires finite range (bounded or to)");
        return 0;
    default:
        assert(false && "Invalid range tag");
        return 0;
    }
}

static inline __attribute__((always_inline))
usize
R_at(R self, usize idx) {
    switch (self.tag) {
    case R_Tag_bounded:
        assert(idx < (self.payload.bounded.end - self.payload.bounded.begin) && "Index out of bounds");
        return self.payload.bounded.begin + idx;
    case R_Tag_from:
        // [begin, ∞) → at(idx) = begin + idx
        return R_Bound_from(self.payload.from) + idx;
    case R_Tag_to: {
        // [0, end) → at(idx) = idx
        usize end = R_Bound_to(self.payload.to);
        assert(idx < end && "Index out of bounds");
        return idx;
    }
    case R_Tag_unbounded:
        // [0, ∞) → at(idx) = idx
        return idx;
    default:
        assert(false && "Invalid range tag");
        return 0;
    }
}

static inline __attribute__((always_inline))
R
R_slice(R self, R range) {
    assert(R_isValid(self) && "Self range must be valid");
    assert(R_isValid(range) && "Range parameter must be valid");

    // Extract self's bounds
    bool  self_has_begin = false, self_has_end = false;
    usize self_begin = 0, self_end = 0;

    switch (self.tag) {
    case R_Tag_unbounded:
        self_has_begin = true;
        self_begin     = 0;
        self_has_end   = false;
        break;
    case R_Tag_from:
        self_has_begin = true;
        self_begin     = R_Bound_from(self.payload.from);
        self_has_end   = false;
        break;
    case R_Tag_to:
        self_has_begin = true;
        self_begin     = 0;
        self_has_end   = true;
        self_end       = R_Bound_to(self.payload.to);
        break;
    case R_Tag_bounded:
        self_has_begin = true;
        self_begin     = self.payload.bounded.begin;
        self_has_end   = true;
        self_end       = self.payload.bounded.end;
        break;
    }

    // Extract range's offsets
    bool  range_has_begin = false, range_has_end = false;
    usize range_offset_begin = 0, range_offset_end = 0;

    switch (range.tag) {
    case R_Tag_unbounded:
        range_has_begin = false;
        range_has_end   = false;
        break;
    case R_Tag_from:
        range_has_begin    = true;
        range_offset_begin = R_Bound_from(range.payload.from);
        range_has_end      = false;
        break;
    case R_Tag_to:
        range_has_begin  = false;
        range_has_end    = true;
        range_offset_end = R_Bound_to(range.payload.to);
        break;
    case R_Tag_bounded:
        range_has_begin    = true;
        range_offset_begin = range.payload.bounded.begin;
        range_has_end      = true;
        range_offset_end   = range.payload.bounded.end;
        break;
    }

    // Calculate new bounds
    bool  new_has_begin = self_has_begin || range_has_begin;
    bool  new_has_end   = self_has_end || range_has_end;
    usize new_begin     = self_begin + (range_has_begin ? range_offset_begin : 0);
    usize new_end       = 0;

    if (new_has_end) {
        if (self_has_end && range_has_end) {
            new_end = self_begin + range_offset_end;
            assert(new_end <= self_end && "Slice end out of bounds");
        } else if (self_has_end) {
            new_end = self_end;
        } else {
            new_end = self_begin + range_offset_end;
        }
    }

    // Bounds checking
    if (self_has_end && range_has_begin) {
        assert(new_begin <= self_end && "Slice begin out of bounds");
    }
    if (self_has_end && range_has_end) {
        assert(range_offset_begin <= range_offset_end && "Invalid slice range");
    }

    // Determine result type
    if (!new_has_begin && !new_has_end) {
        return R_unbounded(R_Bound_Tag_incl, R_Bound_Tag_excl);
    } else if (new_has_begin && !new_has_end) {
        return R_from(R_Bound_Tag_incl, new_begin);
    } else if (!new_has_begin && new_has_end) {
        return R_to(R_Bound_Tag_excl, new_end);
    } else {
        return R_bounded(new_begin, new_end);
    }
}

static inline __attribute__((always_inline))
R
R_prefix(R self, usize end) {
    return R_slice(self, R_to(R_Bound_Tag_excl, end));
}

static inline __attribute__((always_inline))
R
R_suffix(R self, usize begin) {
    return R_slice(self, R_from(R_Bound_Tag_incl, begin));
}

static inline __attribute__((always_inline)) bool R_eq(R lhs, R rhs) {
    if (lhs.tag != rhs.tag) {
        return false;
    }

    switch (lhs.tag) {
    case R_Tag_unbounded:
        return lhs.payload.unbounded.begin == rhs.payload.unbounded.begin && lhs.payload.unbounded.end == rhs.payload.unbounded.end;
    case R_Tag_from:
        return lhs.payload.from.tag == rhs.payload.from.tag && lhs.payload.from.point == rhs.payload.from.point;
    case R_Tag_to:
        return lhs.payload.to.tag == rhs.payload.to.tag && lhs.payload.to.point == rhs.payload.to.point;
    case R_Tag_bounded:
        return lhs.payload.bounded.begin == rhs.payload.bounded.begin && lhs.payload.bounded.end == rhs.payload.bounded.end;
    default:
        return false;
    }
}

#endif // RANGE_H
