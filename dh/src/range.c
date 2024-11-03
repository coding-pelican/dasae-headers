#include "../range.h"
#include "../debug/debug_assert.h"


Range Range_from(usize start, usize end) {
    debug_assert(start <= end);
    return makeWith(Range, .start = start, .end = end);
}

// length == (end - start + 1)
usize Range_length(Range r) {
    return r.end - r.start + 1;
}

bool Range_isValid(Range r) {
    return r.start <= r.end;
}

// index must be in range [start, end)
bool Range_contains(Range r, usize index) {
    return r.start <= index && index < r.end;
}

bool Range_eq(Range lhs, Range rhs) {
    return lhs.start == rhs.start && lhs.end == rhs.end;
}

bool Range_ne(Range lhs, Range rhs) {
    return lhs.start != rhs.start || lhs.end != rhs.end;
}
