/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    range.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2024-10-23 (date of last update)
 * @version v1.0.0
 * @ingroup SOME_GROUP
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef RANGE_INCLUDED
#define RANGE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "primitive_types.h"


typedef struct Range Range;


struct Range {
    usize start; // start index in array
    usize end; // end index in array
};
#define Range_(...)               ((Range){ __VA_ARGS__ })
#define Range_make_(_start, _end) Range_(.start = (_start), .end = (_end))
Range Range_make(usize start, usize end);
usize Range_Length(const RefT(Range) self);
bool  Range_IsValid(const RefT(Range) self);
bool  Range_Contains(const RefT(Range) self, usize index);

bool Range_eq(Range a, Range b);
bool Range_ne(Range a, Range b);


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* RANGE_INCLUDED */


#if defined(DH_IMPL) && !defined(RANGE_IMPL)
#define RANGE_IMPL
#endif
#ifdef RANGE_IMPL
#define RANGE_IMPL_INCLUDED (1)

#include "assert.h"

Range Range_make(usize start, usize end) {
    Assert(start <= end);
    return Range_(start, end);
}

// length == (end - start + 1)
usize Range_Length(const RefT(Range) self) {
    return self->end - self->start + 1;
}

bool Range_IsValid(const RefT(Range) self) {
    return self->start <= self->end;
}

// index must be in range [start, end)
bool Range_Contains(const RefT(Range) self, usize index) {
    return self->start <= index && index < self->end;
}

bool Range_eq(Range a, Range b) {
    return a.start == b.start && a.end == b.end;
}

bool Range_ne(Range a, Range b) {
    return a.start != b.start || a.end != b.end;
}

#endif /* RANGE_IMPL */
