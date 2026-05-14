#include "dh/TEST.h"
#include "fixture.h"

TEST_fn_("fixture: recursive dsl reaches current project" $scope) {
    try_(TEST_expect(recursive_dsl_fixture_answer() == 42));
} $unscoped(TEST_fn);
