#include "../include/dh/debug/debug_assert.h"


Nonnull(anyptr) ref_checkNotNull(Nonnull(anyptr) ptr) {
    debug_assertNotNullFmt(ptr, "Reference cannot be null");
    return ptr;
}
