#include "dh/main.h"

int TEST_func() {
    int* target_value = null;
    if ($branch_likely(target_value != null)) {
        return 0;
    }
    $unreachable;
}

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    int value = TEST_func();
    debug_assert(value == 0);
    return_ok({});
} $unscoped_(fn);
