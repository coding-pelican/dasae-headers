#include "dh/TEST.h"
#include "fixture_dep.h"

TEST_fn_("fixture-dep: recursive dsl includes dependency once" $scope) {
    try_(TEST_expect(recursive_dsl_dependency_answer() == 7));
} $unscoped(TEST_fn);
