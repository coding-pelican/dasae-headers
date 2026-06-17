#include "dh-main.h"
#include "dh/core/pri.h"

TEST_fn_("pri: bool reductions cover any all and none" $scope) {
    try_(TEST_expect(bool_any(false, true, false)));
    try_(TEST_expect(!bool_any(false, false)));
    try_(TEST_expect(bool_all(true, true, true)));
    try_(TEST_expect(!bool_all(true, false, true)));
    try_(TEST_expect(bool_none(false, false, false)));
    try_(TEST_expect(!bool_none(false, true, false)));
} $unscoped(TEST_fn);
