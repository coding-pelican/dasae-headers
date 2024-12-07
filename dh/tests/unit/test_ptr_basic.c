#include "dh/core/ptr.h"

void example_usage(void) {
    // Single item usage
    i32 value   = 42;
    Sptr(i32) p = Sptr_ref(i32, value);

    i32 x = Sptr_get(i32, p); // Read
    Sptr_set(i32, p, x + 1);  // Write

    // Array usage
    i32 arr[]        = { 1, 2, 3, 4, 5 };
    Slice(i32) slice = Slice_fromArray(i32, arr);

    // Array access
    i32 first = Slice_get(i32, slice, 0);
    Slice_set(i32, slice, 0, first + 1);

    // Sub-slice
    Slice(i32) sub = Slice_slice(i32, slice, 1, 3);
    for (i32* num = (i32*)Slice_raw(sub); num < ((i32*)Slice_raw(sub) + Slice_len(sub)); ++num) {
        *num += 1;
        printf("%d\n", *num);
    }

    // Memory operations
    // Slice dest = ...;
    // Slice_copy(dest, sub);

    // Type safety
    debug_assert(ptr_isType(i32, p));
    debug_assert(ptr_isType(i32, slice.ptr));
}
