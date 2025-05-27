/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    TEST.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-10 (date of creation)
 * @updated 2025-03-26 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  TEST
 *
 * @brief   Implementation of the test framework
 */

#include "dh/TEST.h"
#include "dh/mem.h"
#include "dh/heap/Page.h"

/* ANSI color codes */
#define TEST_color_reset  "\033[0m"
#define TEST_color_red    "\033[31m"
#define TEST_color_green  "\033[32m"
#define TEST_color_yellow "\033[33m"
#define TEST_color_blue   "\033[34m"

fn_(TEST_Framework_instance(void), TEST_Framework*) {
    /* Singleton instance */
    static TEST_Framework s_instance       = cleared();
    static bool           s_is_initialized = false;
    static heap_Page      s_allocator      = cleared();
    if (!s_is_initialized) {
        s_instance.cases = type$(
            ArrList$TEST_Case,
            ArrList_init(
                typeInfo$(TEST_Case),
                heap_Page_allocator(&s_allocator)));
        s_is_initialized = true;
    }
    return &s_instance;
}

fn_(TEST_Framework_bindCase(TEST_CaseFn fn, Str_const name), void) {
    let instance = TEST_Framework_instance();
    let result   = ArrList_append(
        instance->cases.base,
        meta_refPtr(create$(TEST_Case, .fn = fn, .name = name)) );
    if (isErr(result)) { return; } /* Occurs when heap is full (Out of memory) */
}

fn_(TEST_Framework_run(void), void) {
    let instance = TEST_Framework_instance();

    // Print header
    printf("\n");
    printf(TEST_color_blue "=== Running Tests ===" TEST_color_reset);
    printf("\n");

    // Run each test case
    let cases = instance->cases.items;
    for_slice (cases, test_case) {
        instance->stats.total++;
        printf("Running test: %s%.*s%s\n", TEST_color_yellow, as$(i32, test_case->name.len), test_case->name.ptr, TEST_color_reset);

        // Run the test
        if_err(test_case->fn(), err) {
            instance->stats.failed++;
            printf("  %s[FAIL]%s: [%s] %s\n", TEST_color_red, TEST_color_reset, Err_domainToCStr(err), Err_codeToCStr(err));
        } else_void {
            instance->stats.passed++;
            printf("  %s[PASS]%s\n", TEST_color_green, TEST_color_reset);
        }
    }

    // Print summary
    printf("\n");
    {
        printf(TEST_color_blue "=== Test Summary ===" TEST_color_reset "\n");
        printf("Total: %u\n", instance->stats.total);
        printf(TEST_color_green "Passed: %u" TEST_color_reset "\n", instance->stats.passed);
        printf(TEST_color_red "Failed: %u" TEST_color_reset "\n", instance->stats.failed);
    }
    printf("\n");

    ArrList_fini(instance->cases.base);
}

/* Debug versions of test functions */
fn_(TEST_expect_test(bool expr, SrcLoc loc, Str_const expr_str), Err$void, $scope) {
    $ignore = loc;
    $ignore = expr_str;

    if (!expr) {
        return_err(Err_InvalidArgument());
    }
    return_ok({});
} $unscoped;

fn_(TEST_expectMsg_test(bool expr, Str_const msg, SrcLoc loc, Str_const expr_str), Err$void, $scope) {
    $ignore = msg;
    $ignore = loc;
    $ignore = expr_str;

    if (!expr) {
        return_err(Err_InvalidArgument());
    }
    return_ok({});
} $unscoped;
