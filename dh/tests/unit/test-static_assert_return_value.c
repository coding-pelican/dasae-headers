#include "dh/claim/assert_static.h"
#include "dh/core.h"
#include "dh/debug/assert.h"
#include "dh/main.h"

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

int TEST_func() {
    int  ub_addr_target = 0;
    int* target_value   = null;
    if (likely(target_value != null)) {
        return 0;
    }
    __builtin_unreachable();
}

Err$void dh_main(int argc, const char* argv[]) {
    reserveReturn(Err$void);
    unused(argc), unused(argv);
    int value = TEST_func();
    debug_assert(value == 0);
    return_void();
}
