#include "dh/core/prim/ref.h"
#include "dh/debug/assert.h"


Nonnull(anyptr) ref_checkNonNull(Nonnull(anyptr) ptr) {
    debug_assert_nonnull_fmt(ptr, "Reference cannot be null");
    return ptr;
}
