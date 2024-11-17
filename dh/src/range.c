#include "dh/Range.h"
#include "dh/debug/assert.h"


Range Range_from(usize begin, usize end) {
    debug_assert(begin <= end);
    return makeWith(Range, .begin = begin, .end = end);
}

usize Range_length(Range r) {
    return r.end - r.begin + 1;
}

bool Range_isValid(Range r) {
    return r.begin <= r.end;
}

bool Range_contains(Range r, usize index) {
    return r.begin <= index && index < r.end;
}

bool Range_eq(Range lhs, Range rhs) {
    return lhs.begin == rhs.begin && lhs.end == rhs.end;
}

bool Range_ne(Range lhs, Range rhs) {
    return lhs.begin != rhs.begin || lhs.end != rhs.end;
}
