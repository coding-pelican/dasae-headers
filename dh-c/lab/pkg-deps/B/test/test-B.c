#include "B.h"
#include "dh/TEST.h"

TEST_fn_("B: len stays positive" $scope) {
    B_init();
    try_(TEST_expect(B_len() > 0));
} $unscoped(TEST_fn);
