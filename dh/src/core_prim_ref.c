#include "dh/core/prim/ref.h"
#include "dh/debug/assert.h"

Nonnull(anyptr) ref_checkNotNull(Nonnull(anyptr) ptr) {
    debug_assertNotNullFmt(ptr, "Reference cannot be null");
    return ptr;
}
