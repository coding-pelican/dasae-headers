#include "range.h"
#include "assert.h"


Range Range_make(usize start, usize end) {
    Assert(start <= end);
    return Range_(start, end);
}

// length == (end - start + 1)
usize Range_Length(const Ref(Range) self) {
    return self->end - self->start + 1;
}

bool Range_IsValid(const Ref(Range) self) {
    return self->start <= self->end;
}

// index must be in range [start, end)
bool Range_Contains(const Ref(Range) self, usize index) {
    return self->start <= index && index < self->end;
}

bool Range_eq(Range a, Range b) {
    return a.start == b.start && a.end == b.end;
}

bool Range_ne(Range a, Range b) {
    return a.start != b.start || a.end != b.end;
}
