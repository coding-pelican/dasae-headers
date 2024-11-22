/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    cmp_expt.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-17 (date of creation)
 * @updated 2024-11-17 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  cmp
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef CMP_INCLUDED
#define CMP_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "core.h"
#include "claim/unreachable.h"

/*========== Macros and Definitions =========================================*/

#define match(value) \
    case value:      \
        return

// ------------------------------
// Basic comparison functions
// ------------------------------

// Default byte-wise comparison function for total ordering
// Returns negative if a < b, 0 if a == b, positive if a > b
static inline isize cmp_cmp(const anyptr self, const anyptr other, usize size) {
    const u8* bytes_a = (const u8*)self;
    const u8* bytes_b = (const u8*)other;

    for (usize i = 0; i < size; ++i) {
        if (bytes_a[i] != bytes_b[i]) {
            return (i32)bytes_a[i] - (i32)bytes_b[i];
        }
    }
    return 0;
}

static inline bool cmp_eq(const anyptr self, const anyptr other, usize size) {
    return cmp_cmp(self, other, size) == 0;
}

static inline bool cmp_ne(const anyptr self, const anyptr other, usize size) {
    return !cmp_eq(self, other, size);
}

// ------------------------------
// Ordering enum and functions
// ------------------------------

typedef enum cmp_Ordering {
    cmp_Ordering_LESS    = -1,
    cmp_Ordering_EQUAL   = 0,
    cmp_Ordering_GREATER = 1
} cmp_Ordering;

static inline cmp_Ordering cmp_Ordering_cmp(const anyptr self, const anyptr other, usize size) {
    isize cmp = cmp_cmp(self, other, size);
    if (cmp < 0) { return cmp_Ordering_LESS; }
    if (cmp > 0) { return cmp_Ordering_GREATER; }
    return cmp_Ordering_EQUAL;
}

// Helper functions for Ordering
static inline bool cmp_Ordering_isEq(cmp_Ordering ord) {
    return ord == cmp_Ordering_EQUAL;
}

static inline bool cmp_Ordering_isNe(cmp_Ordering ord) {
    return !cmp_Ordering_isEq(ord);
}

static inline bool cmp_Ordering_isLt(cmp_Ordering ord) {
    return ord == cmp_Ordering_LESS;
}

static inline bool cmp_Ordering_isGt(cmp_Ordering ord) {
    return ord == cmp_Ordering_GREATER;
}

static inline bool cmp_Ordering_isLe(cmp_Ordering ord) {
    return !cmp_Ordering_isGt(ord);
}

static inline bool cmp_Ordering_isGe(cmp_Ordering ord) {
    return !cmp_Ordering_isLt(ord);
}

static inline cmp_Ordering cmp_Ordering_reverse(cmp_Ordering ord) {
    switch (ord) {
        match(cmp_Ordering_LESS) cmp_Ordering_GREATER;
        match(cmp_Ordering_EQUAL) cmp_Ordering_EQUAL;
        match(cmp_Ordering_GREATER) cmp_Ordering_LESS;
    }
    claim_unreachable_msg("Invalid Ordering value in reverse()");
}

static inline cmp_Ordering cmp_Ordering_then(cmp_Ordering self, cmp_Ordering other) {
    return self == cmp_Ordering_EQUAL ? other : self;
}

// New: then_with implementation
typedef cmp_Ordering (*cmp_OrderingFn)(void);

static inline cmp_Ordering cmp_Ordering_thenWith(cmp_Ordering self, cmp_OrderingFn fn) {
    return cmp_Ordering_isEq(self) ? fn() : self;
}

// ------------------------------
// Core trait definitions
// ------------------------------

// PartialEq trait
typedef struct cmp_PartialEq {
    bool (*eq)(const anyptr self, const anyptr other, usize size);
    bool (*ne)(const anyptr self, const anyptr other, usize size);
} cmp_PartialEq;

// Eq trait (extends PartialEq)
typedef struct cmp_Eq {
    cmp_PartialEq PartialEq;
    // // New: Assert receiver method for compile-time checks
    // void (*assertReceiverIsTotalEq)(const anyptr self, usize size);
} cmp_Eq;

// PartialOrd trait (extends PartialEq)
typedef struct cmp_PartialOrd {
    cmp_PartialEq PartialEq;
    cmp_Ordering* (*partialCmp)(const anyptr self, const anyptr other, usize size);
    bool (*lt)(const anyptr self, const anyptr other, usize size);
    bool (*gt)(const anyptr self, const anyptr other, usize size);
    bool (*le)(const anyptr self, const anyptr other, usize size);
    bool (*ge)(const anyptr self, const anyptr other, usize size);
} cmp_PartialOrd;

// Ord trait (extends PartialOrd + Eq)
typedef struct cmp_Ord {
    cmp_PartialOrd PartialOrd;
    cmp_Eq         Eq;
    cmp_Ordering (*cmp)(const anyptr self, const anyptr other, usize size);
    const anyptr (*min)(const anyptr self, const anyptr other, usize size);
    const anyptr (*max)(const anyptr self, const anyptr other, usize size);
    const anyptr (*clamp)(const anyptr self, const anyptr min, const anyptr max, usize size);
} cmp_Ord;

typedef struct cmp_Obj {
    anyptr   inner;
    cmp_Ord* vt;
} cmp_Obj;

cmp_Obj* cmp_Obj_init(cmp_Obj* self, cmp_Ord* vt, anyptr inner) {
    self->inner = inner;
    self->vt    = vt;
    return self;
}

cmp_Ord vt = { 0 };

void test() {
    cmp_Obj a;
    cmp_Obj b;
    // cmp_PartialEq_eq(&vt, &a, &b, sizeof(i32));
}

// ------------------------------
// Default trait implementations
// ------------------------------

// PartialEq defaults
static inline bool cmp_PartialEq_eq(const cmp_PartialEq* vt, const anyptr self, const anyptr other, usize size) {
    return !vt->ne(self, other, size);
}

static inline bool cmp_PartialEq_ne(const cmp_PartialEq* vt, const anyptr self, const anyptr other, usize size) {
    return !vt->eq(self, other, size);
}

// // Eq defaults
// static inline void cmp_Eq_assertReceiverIsTotalEq(const anyptr self, usize size) {
//     (void)self;
//     (void)size;
//     // This is a no-op in C, used only for trait bounds checking in Rust
// }

// PartialOrd defaults
static inline bool cmp_PartialOrd_lt(const cmp_PartialOrd* vt, const anyptr self, const anyptr other, usize size) {
    cmp_Ordering* ord = vt->partialCmp(self, other, size);
    return ord && cmp_Ordering_isLt(*ord);
}

static inline bool cmp_PartialOrd_le(const cmp_PartialOrd* vt, const anyptr self, const anyptr other, usize size) {
    cmp_Ordering* ord = vt->partialCmp(self, other, size);
    return ord && cmp_Ordering_isLe(*ord);
}

static inline bool cmp_PartialOrd_gt(const cmp_PartialOrd* vt, const anyptr self, const anyptr other, usize size) {
    cmp_Ordering* ord = vt->partialCmp(self, other, size);
    return ord && cmp_Ordering_isGt(*ord);
}

static inline bool cmp_PartialOrd_ge(const cmp_PartialOrd* vt, const anyptr self, const anyptr other, usize size) {
    cmp_Ordering* ord = vt->partialCmp(self, other, size);
    return ord && cmp_Ordering_isGe(*ord);
}

// Ord defaults
static inline const anyptr cmp_Ord_min(const cmp_Ord* vt, const anyptr self, const anyptr other, usize size) {
    return !cmp_Ordering_isGt(vt->cmp(self, other, size)) ? self : other;
}

static inline const anyptr cmp_Ord_max(const cmp_Ord* vt, const anyptr self, const anyptr other, usize size) {
    return !cmp_Ordering_isLt(vt->cmp(self, other, size)) ? self : other;
}

static inline const anyptr cmp_Ord_clamp(const cmp_Ord* vt, const anyptr self, const anyptr min, const anyptr max, usize size) {
    if (cmp_Ordering_isGt(vt->cmp(min, max, size))) {
        claim_unreachable_msg("min > max in cmp_Ord_clamp");
    }
    if (cmp_Ordering_isLt(vt->cmp(self, min, size))) { return min; }
    if (cmp_Ordering_isGt(vt->cmp(self, max, size))) { return max; }
    return self;
}

// ------------------------------
// Reverse wrapper
// ------------------------------

typedef struct cmp_Reverse {
    const anyptr inner;
    cmp_Ordering (*cmp)(const anyptr self, const anyptr other, usize size);
} cmp_Reverse;

typedef cmp_Ordering (*cmp_CmpFn)(const anyptr self, const anyptr other, usize size);

static inline cmp_Reverse* cmp_Reverse_init(cmp_Reverse* self, const anyptr value, cmp_CmpFn cmp) {
    return self->inner = value, self->cmp = cmp, self;
}

static inline bool cmp_Reverse_eq(const cmp_Reverse* self, const cmp_Reverse* other, usize size) {
    return cmp_Ordering_isEq(self->cmp(self->inner, other->inner, size));
}

static inline bool cmp_Reverse_ne(const cmp_Reverse* self, const cmp_Reverse* other, usize size) {
    return !cmp_Reverse_eq(self, other, size);
}

static inline cmp_Ordering cmp_Reverse_cmp(const cmp_Reverse* self, const cmp_Reverse* other, usize size) {
    return cmp_Ordering_reverse(self->cmp(self->inner, other->inner, size));
}

// ------------------------------
// Utility functions
// ------------------------------

static inline const anyptr cmp_minBy(
    const anyptr self,
    const anyptr other,
    cmp_CmpFn    cmp,
    usize        size
) {
    return !cmp_Ordering_isGt(cmp(self, other, size)) ? self : other;
}

static inline const anyptr cmp_maxBy(
    const anyptr self,
    const anyptr other,
    cmp_CmpFn    cmp,
    usize        size
) {
    return !cmp_Ordering_isLt(cmp(self, other, size)) ? self : other;
}

static inline const anyptr cmp_clamp(
    const anyptr self,
    const anyptr min,
    const anyptr max,
    cmp_CmpFn    cmp,
    usize        size
) {
    if (cmp_Ordering_isGt(cmp(min, max, size))) {
        claim_unreachable_msg("min > max in cmp_clamp");
    }

    if (cmp_Ordering_isLt(cmp(self, min, size))) {
        return min;
    }
    if (cmp_Ordering_isGt(cmp(self, max, size))) {
        return max;
    }
    return self;
}

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif // CMP_INCLUDED
