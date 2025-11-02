#include "dh/main.h"
#include "dh/Arr.h"

// Test state tracking
static struct TestState {
    A$$(8, i32) cleanup_orders;
    i32 cleanup_index;
    i32 counter;
} s_test_state = cleared();

// Helper to record cleanup order
$static
fn_((recordCleanup(i32 value))(void)) {
    A_setAt(s_test_state.cleanup_orders, s_test_state.cleanup_index++, value);
}

// Forward declarations of test scope functions
static fn_((testBasicDeferScope(void))(E$void)) $must_check;
static fn_((testMultipleDeferScope(void))(void));
static fn_((testDeferWithReturnScope(void))(void));
static fn_((testBlockDeferScope(void))(void));

// Test basic defer functionality
TEST_fn_("test basic defer" $scope) {
    s_test_state = cleared$(struct TestState); // Clear state
    try_(testBasicDeferScope());
    try_(TEST_expect(s_test_state.counter == 2));
    return_ok({});
} $unscoped_(TEST_fn);

$static
fn_((testBasicDeferScope(void))(E$void $guard)) {
    s_test_state.counter = 1;
    defer_(s_test_state.counter = 2);
    try_(TEST_expect(s_test_state.counter == 1));
    return_ok({});
} $unguarded_(fn);

// Test multiple defers (LIFO order)
TEST_fn_("test multiple defers" $scope) {
    s_test_state = cleared$(struct TestState); // Clear state
    testMultipleDeferScope();
    // Verify LIFO order: 3, 2, 1
    try_(TEST_expect(A_getAt(s_test_state.cleanup_orders, 0) == 3));
    try_(TEST_expect(A_getAt(s_test_state.cleanup_orders, 1) == 2));
    try_(TEST_expect(A_getAt(s_test_state.cleanup_orders, 2) == 1));
    return_ok({});
} $unscoped_(TEST_fn);

$static
fn_((testMultipleDeferScope(void))(void $guard)) {
    defer_(recordCleanup(1));
    defer_(recordCleanup(2));
    defer_(recordCleanup(3));
    return_void();
} $unguarded_(fn);

// Test defer with early return
TEST_fn_("test defer with early return" $scope) {
    s_test_state = cleared$(struct TestState); // Clear state
    testDeferWithReturnScope();
    // Verify cleanup executed in correct order despite early return
    try_(TEST_expect(A_getAt(s_test_state.cleanup_orders, 0) == 2));
    try_(TEST_expect(A_getAt(s_test_state.cleanup_orders, 1) == 1));
    try_(TEST_expect(s_test_state.cleanup_index == 2));
    return_ok({});
} $unscoped_(TEST_fn);

$static
fn_((testDeferWithReturnScope(void))(void $guard)) {
    defer_(recordCleanup(1));
    if (true) {
        defer_(recordCleanup(2));
        return_void();
    }
    recordCleanup(3); // Should not be executed
    return_void();
} $unguarded_(fn);

// Test block defer
TEST_fn_("test block defer" $scope) {
    s_test_state = cleared$(struct TestState); // Clear state
    testBlockDeferScope();
    // Verify block defer behavior
    try_(TEST_expect(A_getAt(s_test_state.cleanup_orders, 0) == 3));
    try_(TEST_expect(A_getAt(s_test_state.cleanup_orders, 1) == 2));
    try_(TEST_expect(A_getAt(s_test_state.cleanup_orders, 2) == 5));
    try_(TEST_expect(A_getAt(s_test_state.cleanup_orders, 3) == 1));
    try_(TEST_expect(s_test_state.cleanup_index == 4));
    return_ok({});
} $unscoped_(TEST_fn);

$static
fn_((testBlockDeferScope(void))(void $guard)) {
    defer_(recordCleanup(1));

    blk_defer {
        defer_(recordCleanup(2));
        if (true) {
            defer_(recordCleanup(3));
            break_defer;
        }
        recordCleanup(4); // Should not be executed
    } blk_deferral;

    defer_(recordCleanup(5));
    return_void();
} $unguarded_(fn);
