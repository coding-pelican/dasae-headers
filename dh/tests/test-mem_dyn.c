#include "dh-main.h"
#include "dh/mem/dyn.h"

TEST_fn_("mem/dyn: init capacity is at least one and cache-line based" $scope) {
    let u8_cap = mem_dyn_initCap(typeInfo$(u8));
    let u64_cap = mem_dyn_initCap(typeInfo$(u64));

    try_(TEST_expect(u8_cap >= 1));
    try_(TEST_expect(u64_cap >= 1));
    try_(TEST_expect(u8_cap >= u64_cap));
    try_(TEST_expect(u8_cap == mem_dyn_initCap_static$(u8)));
} $unscoped(TEST_fn)

TEST_fn_("mem/dyn: grow capacity always advances and reaches minimum" $scope) {
    let init_cap = mem_dyn_initCap(typeInfo$(u8));
    let from_zero = mem_dyn_growCap(typeInfo$(u8), 0, 0);
    let next_even_when_min_is_current = mem_dyn_growCap(typeInfo$(u8), init_cap, init_cap);
    let grown_to_min = mem_dyn_growCap(typeInfo$(u8), init_cap, init_cap * 4 + 1);

    try_(TEST_expect(from_zero == init_cap));
    try_(TEST_expect(next_even_when_min_is_current > init_cap));
    try_(TEST_expect(grown_to_min >= init_cap * 4 + 1));
} $unscoped(TEST_fn)

TEST_fn_("mem/dyn: checked length addition reports OutOfMemory on overflow" $scope) {
    try_(TEST_expect(try_(mem_dyn_addOrOOM(2, 3)) == 5));

    if_err((mem_dyn_addOrOOM(usize_limit, 1))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$OutOfMemory().as_any)));
    } else_ok(value) {
        let_ignore = value;
        try_(TEST_expect(false));
    }
} $unscoped(TEST_fn)
