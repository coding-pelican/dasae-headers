#include "dh-main.h"
#include "dh/Target.h"

TEST_fn_("Target: compile-time and linked runtime targets agree" $scope) {
    let here = Target_here();
    let self = Target_self();

    try_(TEST_expect(here.arch.cache_line_bytes == arch_cache_line_bytes));
    try_(TEST_expect(here.arch.cache_line_bytes != 0));
    try_(TEST_expect(
        (here.arch.cache_line_bytes & (here.arch.cache_line_bytes - 1)) == 0
    ));
    try_(TEST_expect(Target_eql(here, self)));
    try_(TEST_expect(!Target_neq(here, self)));
    try_(TEST_expect(Target_Arch_eql(here.arch, self.arch)));
    try_(TEST_expect(Target_Plat_eql(here.plat, self.plat)));
    try_(TEST_expect(Target_ABI_eql(here.abi, self.abi)));
} $unscoped(TEST_fn);

TEST_fn_("Target: component changes participate in target equality" $scope) {
    let here = Target_here();
    var changed = here;
    changed.abi.pointer_bits ^= 1;

    try_(TEST_expect(Target_ABI_neq(here.abi, changed.abi)));
    try_(TEST_expect(Target_neq(here, changed)));
    changed = here;
    changed.plat.kind = here.plat.kind == Target_Plat_Kind_unknown
        ? Target_Plat_Kind_windows
        : Target_Plat_Kind_unknown;
    try_(TEST_expect(Target_Plat_neq(here.plat, changed.plat)));
    try_(TEST_expect(Target_neq(here, changed)));
} $unscoped(TEST_fn);
