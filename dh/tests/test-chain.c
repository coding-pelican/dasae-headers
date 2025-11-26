#include "dh/main.h"
#include "dh/core/chain.h"

/// Test basic fold operation - sum all elements
TEST_fn_("test_chain_fold_sum" $scope) {
    let data = A_from$((i32){ 1, 2, 3, 4, 5 });
    let slice = A_ref(data);

    let sum = chain$((i32)(slice)(
        fold_((0), (acc, item)(acc + *item))
    ));

    try_(TEST_expect(sum == 15));
} $unscoped_(TEST_fn);

/// Test fold with initial value
TEST_fn_("test_chain_fold_with_initial" $scope) {
    let data = A_from$((i32){ 1, 2, 3, 4, 5 });
    let slice = A_ref(data);

    let sum = chain$((i32)(slice)(
        fold_((100), (acc, item)(acc + *item))
    ));

    try_(TEST_expect(sum == 115));
} $unscoped_(TEST_fn);

/// Test reduce operation - no initial value
TEST_fn_("test_chain_reduce" $scope) {
    let data = A_from$((i32){ 1, 2, 3, 4, 5 });
    let slice = A_ref(data);

    let result = chain$((O$i32)(slice)(
        reduce_((acc, item)(acc + *item))
    ));

    try_(TEST_expect(isSome(result)));
    try_(TEST_expect(unwrap_(result) == 15));
} $unscoped_(TEST_fn);

/// Test reduce on empty slice returns none
TEST_fn_("test_chain_reduce_empty" $scope) {
    let data = A_from$((i32){});
    let empty_slice = A_ref(data);

    let result = chain$((O$i32)(empty_slice)(
        reduce_((acc, item)(acc + *item))
    ));

    try_(TEST_expect(!isSome(result)));
} $unscoped_(TEST_fn);

/// Test filter operation
TEST_fn_("test_chain_filter" $scope) {
    let data = A_from$((i32){ 1, 2, 3, 4, 5 });
    let slice = A_ref(data);

    let sum = chain$((i32)(slice)(
        filter_((item)(*item % 2 == 0)),
        fold_((0), (acc, item)(acc + *item))
    ));

    try_(TEST_expect(sum == 6)); // 2 + 4 = 6
} $unscoped_(TEST_fn);

/// Test map operation
TEST_fn_("test_chain_map" $scope) {
    let data = A_from$((i32){ 1, 2, 3, 4, 5 });
    let slice = A_ref(data);

    let sum = chain$((i32)(slice)(
        map$((i32)(item)(*item * 2)),
        fold_((0), (acc, item)(acc + *item))
    ));

    try_(TEST_expect(sum == 30)); // (1+2+3+4+5)*2 = 30
} $unscoped_(TEST_fn);

/// Test filter + map combination
TEST_fn_("test_chain_filter_map" $scope) {
    let data = A_from$((i32){ 1, 2, 3, 4, 5 });
    let slice = A_ref(data);

    let result = chain$((i32)(slice)(
        filter_((item)(*item % 2 == 0)),
        map$((i32)(item)(*item * *item)),
        fold_((0), (acc, item)(acc + *item))
    ));

    try_(TEST_expect(result == 20)); // 2^2 + 4^2 = 4 + 16 = 20
} $unscoped_(TEST_fn);

/// Test filter + map + reduce
TEST_fn_("test_chain_filter_map_reduce" $scope) {
    let data = A_from$((i32){ 1, 2, 3, 4, 5 });
    let slice = A_ref(data);

    let result = chain$((O$u32)(slice)(
        filter_((item)(*item > 0)),
        map$((u32)(item)(as$(u32)(*item))),
        reduce_((acc, item)(prim_min(acc, *item)))
    ));

    try_(TEST_expect(isSome(result)));
    try_(TEST_expect(unwrap_(result) == 1)); // min of 1,2,3,4,5 is 1
} $unscoped_(TEST_fn);

/// Test each operation for side effects
TEST_fn_("test_chain_each" $scope) {
    let data = A_from$((i32){ 1, 2, 3, 4, 5 });
    let slice = A_ref(data);

    var_(sum, i32) = 0;
    chain$((i32)(slice)(
        each_((item) { sum += *item; }),
        fold_((0), (acc, $ignore)(acc))
    ));

    try_(TEST_expect(sum == 15));
} $unscoped_(TEST_fn);

/// Test complex chain with multiple operations
TEST_fn_("test_chain_complex" $scope) {
    let data = A_from$((i32){ -2, -1, 0, 1, 2, 3, 4, 5, 6 });
    let slice = A_ref(data);

    let result = chain$((i32)(slice)(
        filter_((item)(*item > 0)),
        filter_((item)(*item % 2 == 0)),
        map$((i32)(item)(*item * 10)),
        fold_((0), (acc, item)(acc + *item))
    ));

    try_(TEST_expect(result == 120)); // (2 + 4 + 6) * 10 = 120
} $unscoped_(TEST_fn);
