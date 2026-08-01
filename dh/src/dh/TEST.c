#include "dh/TEST.h"
#include "dh/heap/Sys.h"
#include "dh/io/stream.h"

/* ANSI color codes */
#define TEST_color_reset "\033[0m"
#define TEST_color_red "\033[31m"
#define TEST_color_green "\033[32m"
#define TEST_color_yellow "\033[33m"
#define TEST_color_blue "\033[34m"

T_use$((TEST_Unit)(O, E));
T_use$((TEST_Unit)(ArrList_init, ArrList_fini, ArrList_append));
fn_((TEST_Framework_instance(void))(TEST_Framework*)) {
    /* Singleton instance */
    $static var_(heap, heap_Sys) $undefined_static;
    $static var_(instance, TEST_Framework) $undefined_static;
    $static var_(is_initialized, bool) = false;
    if (!is_initialized) {
        debug_StackTrace_setupCrashHandler();
        heap = catch_((heap_Sys_init())($ignore, claim_unreachable));
        instance.gpa = heap_Sys_alctr(&heap);
        instance.units $like_deref = catch_((ArrList_init$TEST_Unit(instance.gpa, 8))($ignore, claim_unreachable));
        is_initialized = true;
    }
    return &instance;
};

$static fn_((TEST_Framework_fini(void))(void)) {
    let instance = TEST_Framework_instance();
    let units = instance->units;
    let gpa = instance->gpa;
    ArrList_fini$TEST_Unit(units, gpa);
};

fn_((TEST_Framework_bindUnit(TEST_UnitFn fn, S_const$u8 name))(void)) {
    let instance = TEST_Framework_instance();
    let units = instance->units;
    let gpa = instance->gpa;
    catch_((ArrList_append$TEST_Unit(units, gpa, l$((TEST_Unit){ .fn = fn, .name = name })))($ignore, claim_unreachable));
};

fn_((TEST_Framework_run(void))(void) $guard) {
    defer_(TEST_Framework_fini());

    let instance = TEST_Framework_instance();
    let units = instance->units;
    // printf("--- debug print: TEST_Framework_run ---\n");
    // printf("len(%llu), cap(%llu)\n", units->items.len, units->cap);

    // Print header
    io_stream_nl();
    io_stream_println(u8_l(TEST_color_blue "=== Running Tests ===" TEST_color_reset));

    // Run each test case
    for_(($s(units->items))(unit)) {
        instance->stats.total++;
        io_stream_println(u8_l("Running test: {:s}{:s}{:s}"), u8_l(TEST_color_yellow), unit->name, u8_l(TEST_color_reset));
        // Run the test
        ETrace_reset();
        if_err((unit->fn())(err)) {
            switch (E_tag$TEST_E(*ptrCast$((const TEST_E*)(&err)))) {
            case_((E_Tag$TEST_Skip)) {
                instance->stats.skipped++;
                io_stream_println(u8_l("    {:s}"), u8_l(TEST_color_yellow "[SKIP]" TEST_color_reset));
            } $end(case);
            case E_Tag$TEST_Fail: $fallthrough;
            case_((E_Tag$TEST_E_Any)) {
                instance->stats.failed++;
                io_stream_eprint(u8_l("    {:s}: "), u8_l(TEST_color_red "[FAIL]" TEST_color_reset));
                E_print(&err);
                ETrace_print();
            } $end(case);
            }
            ETrace_reset();
        } else_ok_void {
            instance->stats.passed++;
            io_stream_println(u8_l("    {:s}"), u8_l(TEST_color_green "[PASS]" TEST_color_reset));
        }
    } $end(for);

    // Print summary
    io_stream_nl();
    io_stream_println(u8_l(TEST_color_blue "=== Test Summary ===" TEST_color_reset));
    io_stream_println(u8_l("Total: {:u}"), instance->stats.total);
    io_stream_println(u8_l(TEST_color_green "Passed: {:u}" TEST_color_reset), instance->stats.passed);
    io_stream_println(u8_l(TEST_color_yellow "Skipped: {:u}" TEST_color_reset), instance->stats.skipped);
    io_stream_println(u8_l(TEST_color_red "Failed: {:u}" TEST_color_reset), instance->stats.failed);
    io_stream_nl();
} $unguarded(fn);

fn_((TEST_Framework_failedCount(void))(u32)) {
    return TEST_Framework_instance()->stats.failed;
};

/* Debug versions of test functions */
fn_((TEST_skip_test(SrcLoc loc))(TEST_E$void) $scope) {
    /* TODO: Improve this */
    let_ignore = loc;
    return_err(E_cause$TEST_Skip());
} $unscoped(fn);

fn_((TEST_skipMsg_test(S_const$u8 msg, SrcLoc loc))(TEST_E$void) $scope) {
    /* TODO: Improve this */
    let_ignore = msg;
    let_ignore = loc;
    return_err(E_cause$TEST_Skip());
} $unscoped(fn);

fn_((TEST_expect_test(bool expr, SrcLoc loc, S_const$u8 eval_str))(TEST_E$void) $scope) {
    /* TODO: Improve this */
    let_ignore = loc;
    let_ignore = eval_str;
    if (!expr) return_err(E_cause$TEST_Fail());
    return_ok({});
} $unscoped(fn);

fn_((TEST_expectMsg_test(bool expr, S_const$u8 msg, SrcLoc loc, S_const$u8 eval_str))(TEST_E$void) $scope) {
    /* TODO: Improve this */
    let_ignore = msg;
    let_ignore = loc;
    let_ignore = eval_str;
    if (!expr) return_err(E_cause$TEST_Fail());
    return_ok({});
} $unscoped(fn);
