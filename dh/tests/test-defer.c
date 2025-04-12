#include "dh/main.h"
#include "dh/Arr.h"

// Test state tracking
static struct TestState {
    Arr$$(8, i32) cleanup_orders;
    i32 cleanup_index;
    i32 counter;
} g_test_state = cleared();

// Helper to record cleanup order
static fn_(recordCleanup(i32 value), void) {
    Arr_setAt(g_test_state.cleanup_orders, g_test_state.cleanup_index++, value);
}

// Forward declarations of test scope functions
static fn_(testBasicDeferScope(void), $must_check Err$void);
static fn_(testMultipleDeferScope(void), void);
static fn_(testDeferWithReturnScope(void), void);
static fn_(testBlockDeferScope(void), void);

// Test basic defer functionality
fn_TEST_scope("test basic defer") {
    g_test_state = cleared$(struct TestState); // Clear state
    try_(testBasicDeferScope());
    try_(TEST_expect(g_test_state.counter == 2));
    return_ok({});
} TEST_unscoped

static fn_scope_ext(testBasicDeferScope(void), Err$void) {
    g_test_state.counter = 1;
    defer_(g_test_state.counter = 2);
    try_(TEST_expect(g_test_state.counter == 1));
    return_ok({});
} unscoped_ext

// Test multiple defers (LIFO order)
fn_TEST_scope("test multiple defers") {
    g_test_state = cleared$(struct TestState); // Clear state
    testMultipleDeferScope();
    // Verify LIFO order: 3, 2, 1
    try_(TEST_expect(Arr_getAt(g_test_state.cleanup_orders, 0) == 3));
    try_(TEST_expect(Arr_getAt(g_test_state.cleanup_orders, 1) == 2));
    try_(TEST_expect(Arr_getAt(g_test_state.cleanup_orders, 2) == 1));
    return_ok({});
} TEST_unscoped

static fn_scope_ext(testMultipleDeferScope(void), void) {
    defer_(recordCleanup(1));
    defer_(recordCleanup(2));
    defer_(recordCleanup(3));
    return_void();
} unscoped_ext

// Test defer with early return
fn_TEST_scope("test defer with early return") {
    g_test_state = cleared$(struct TestState); // Clear state
    testDeferWithReturnScope();
    // Verify cleanup executed in correct order despite early return
    try_(TEST_expect(Arr_getAt(g_test_state.cleanup_orders, 0) == 2));
    try_(TEST_expect(Arr_getAt(g_test_state.cleanup_orders, 1) == 1));
    try_(TEST_expect(g_test_state.cleanup_index == 2));
    return_ok({});
} TEST_unscoped

static fn_scope_ext(testDeferWithReturnScope(void), void) {
    defer_(recordCleanup(1));
    if (true) {
        defer_(recordCleanup(2));
        return_void();
    }
    recordCleanup(3); // Should not be executed
    return_void();
} unscoped_ext

// Test block defer
fn_TEST_scope("test block defer") {
    g_test_state = cleared$(struct TestState); // Clear state
    testBlockDeferScope();
    // Verify block defer behavior
    try_(TEST_expect(Arr_getAt(g_test_state.cleanup_orders, 0) == 3));
    try_(TEST_expect(Arr_getAt(g_test_state.cleanup_orders, 1) == 2));
    try_(TEST_expect(Arr_getAt(g_test_state.cleanup_orders, 2) == 5));
    try_(TEST_expect(Arr_getAt(g_test_state.cleanup_orders, 3) == 1));
    try_(TEST_expect(g_test_state.cleanup_index == 4));
    return_ok({});
} TEST_unscoped

static fn_scope_ext(testBlockDeferScope(void), void) {
    defer_(recordCleanup(1));

        block_defer
    {
        defer_(recordCleanup(2));
        if (true) {
            defer_(recordCleanup(3));
            defer_break;
        }
        recordCleanup(4); // Should not be executed
    } block_deferral;

    defer_(recordCleanup(5));
    return_void();
} unscoped_ext
