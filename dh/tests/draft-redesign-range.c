#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

typedef unsigned char u8;
typedef size_t usize;

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
/// default: incl
usize R_Bound_begin(R_Bound_Tag tag, usize point) {
    return point + tag; /* tag == R_Bound_Tag_incl ? point : point + 1 */
}
/// default: excl
usize R_Bound_end(R_Bound_Tag tag, usize point) {
    return point + (1 - tag); /* tag == R_Bound_Tag_excl ? point : point + 1 */
}

usize R_Bound_from(R_Bound self) {
    return R_Bound_begin(self.tag, self.point);
}

usize R_Bound_to(R_Bound self) {
    return R_Bound_end(self.tag, self.point);
}

typedef struct R_Bounded {
    usize begin;
    usize end;
} R_Bounded;

typedef enum R_Tag : u8 {
    R_Tag_unbounded = 0,
    R_Tag_from = 1,
    R_Tag_to = 2,
    R_Tag_bounded = 3,
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

R R_unbounded(R_Bound_Tag begin, R_Bound_Tag end) {
    return (R){ .tag = R_Tag_unbounded, .payload = { .unbounded = { .begin = begin, .end = end } } };
}

R R_from(R_Bound_Tag bound, usize begin) {
    return (R){ .tag = R_Tag_from, .payload = { .from = { .tag = bound, .point = begin } } };
}

R R_to(R_Bound_Tag bound, usize end) {
    return (R){ .tag = R_Tag_to, .payload = { .to = { .tag = bound, .point = end } } };
}
/// [begin..end] => [begin, end)
R R_bounded(usize begin, usize end) {
    return (R){ .tag = R_Tag_bounded, .payload = { .bounded = { .begin = begin, .end = end } } };
}

/// self[begin..end] => [self.begin + range.begin, self.begin + range.end)
/// (self.begin <= range.begin) && (range.begin <= range.end) && (range.end <= self.end)
static R R_slice(R self, R range);
/// self[..end] => [self.begin, self.begin + end)
static R R_prefix(R self, usize end);
/// self[begin..] => [self.begin + begin, self.end)
static R R_suffix(R self, usize begin);

/// self.end - self.begin
static usize R_len(R self);
/// self.begin + idx
static usize R_at(R self, usize idx);

/// self.begin <= self.end
static bool R_isValid(R self);
/// (self.begin <= idx) && (idx < self.end)
static bool R_contains(R self, usize idx);

static bool R_eq(R lhs, R rhs);
static bool R_ne(R lhs, R rhs);
