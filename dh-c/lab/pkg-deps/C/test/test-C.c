#include "C.h"
#include "dh/TEST.h"

TEST_fn_("C: dependency call path stays usable" $scope) {
    B_init();
    try_(TEST_expect(B_len() > 0));
} $unscoped(TEST_fn);
