#include "dh/claim/assert_static.h"
#include "dh/core.h"
#include "dh/debug/assert.h"
#include "dh/main.h"

int TEST_func() {
    int* target_value = null;
    if (likely(target_value != null)) {
        return 0;
    }
    __builtin_unreachable();
}

E$void dh_main(S$S_const$u8 args) {
    reserveReturn(E$void);
    $unused(args);
    int value = TEST_func();
    debug_assert(value == 0);
    return_void();
}
