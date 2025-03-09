/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    TEST.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-10 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  TEST
 *
 * @brief   Test framework for unit testing
 * @details This header provides a simple test framework for unit testing.
 */

#ifndef TEST_INCLUDED
#define TEST_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

#include <stdio.h>

/*========== Test Framework =================================================*/

#define test_scope           comp_syn__test_scope
#define test_unscoped        comp_syn__test_unscoped
#define fn_test_scope(_Name) comp_syn__fn_test_scope(_Name)

#define comp_syn__test_scope
#define comp_syn__test_unscoped
#define comp_syn__fn_test_scope(_Name) fn_(pp_join(_, TEST, _Name)(void), must_check TEST_Result)

typedef struct TEST_Condition {
    const char* expr;
    bool        result;
    const char* file;
    int         line;
} TEST_Condition;

typedef struct TEST_Result {
    const char*     test_name;
    TEST_Condition* conditions;
    usize           condition_count;
    bool            passed;
} TEST_Result;

typedef struct TEST_Stats {
    usize        total_tests;
    usize        passed_tests;
    usize        failed_tests;
    TEST_Result* failed_results;
    usize        failed_count;
} TEST_Stats;

#define TEST_max_conditions (32768)
#define TEST_max_failures   (32768)

static TEST_Stats     TEST_stats = { 0 };
static TEST_Condition TEST_conditions[TEST_max_conditions];
static TEST_Result    TEST_failed_results[TEST_max_failures];
static usize          TEST_condition_index = 0;

/* ANSI color codes */
#define TEST_color_red    "\x1b[31m"
#define TEST_color_green  "\x1b[32m"
#define TEST_color_yellow "\x1b[33m"
#define TEST_color_reset  "\x1b[0m"

/* Test condition macro */
#define TEST_condition(_Expr)                                       \
    do {                                                            \
        bool result                             = (_Expr);          \
        TEST_conditions[TEST_condition_index++] = (TEST_Condition){ \
            .expr   = #_Expr,                                       \
            .result = result,                                       \
            .file   = __FILE__,                                     \
            .line   = __LINE__                                      \
        };                                                          \
    } while (false)

#define TEST_init() TEST_condition_index = 0;     /* Reset condition index */
#define TEST_fini() (TEST_stats.failed_tests > 0) /* Return 1 if exists failed tests */

static_inline void TEST_printSection(const char* section) {
    printf("\n=== Testing %s ===\n", section);
}

static_inline void TEST_printResult(TEST_Result result) {
    TEST_stats.total_tests++;
    if (result.passed) {
        TEST_stats.passed_tests++;
        printf(TEST_color_green "%s: PASSED" TEST_color_reset "\n", result.test_name);
    } else {
        TEST_stats.failed_tests++;
        printf(TEST_color_red "%s: FAILED" TEST_color_reset "\n", result.test_name);
        if (TEST_stats.failed_count < TEST_max_failures) {
            TEST_failed_results[TEST_stats.failed_count++] = result;
        }
    }

    // Print detailed conditions
    for (usize i = 0; i < result.condition_count; ++i) {
        TEST_Condition cond = result.conditions[i];
        printf("  %s%s" TEST_color_reset ": %s\n", cond.result ? TEST_color_green : TEST_color_red, cond.result ? "PASS" : "FAIL", cond.expr);
    }
}

static_inline void TEST_printSummary(void) {
    printf("\n=== Test Summary ===\n");
    printf("Total Tests: %zu\n", TEST_stats.total_tests);
    printf(TEST_color_green "Passed: %zu" TEST_color_reset "\n", TEST_stats.passed_tests);
    printf(TEST_color_red "Failed: %zu" TEST_color_reset "\n", TEST_stats.failed_tests);

    if (TEST_stats.failed_count > 0) {
        printf("\n=== Failed Tests Details ===\n");
        for (usize i = 0; i < TEST_stats.failed_count; i++) {
            TEST_Result failed = TEST_failed_results[i];
            printf("\n" TEST_color_red "%s" TEST_color_reset "\n", failed.test_name);

            for (usize j = 0; j < failed.condition_count; j++) {
                TEST_Condition cond = failed.conditions[j];
                if (!cond.result) {
                    printf("  Failed at %s:%d\n", cond.file, cond.line);
                    printf("  Expression: %s\n", cond.expr);
                }
            }
        }
    }
}

static_inline TEST_Result TEST_makeResult(const char* test_name) {
    TEST_Result result = {
        .test_name       = test_name,
        .conditions      = &TEST_conditions[TEST_condition_index],
        .condition_count = 0,
        .passed          = true
    };
    return result;
}

static_inline TEST_Result TEST_completeResult(TEST_Result* result) {
    result->condition_count = TEST_condition_index - (result->conditions - TEST_conditions);
    result->passed          = true;

    for (usize i = 0; i < result->condition_count; i++) {
        if (!result->conditions[i].result) {
            result->passed = false;
            break;
        }
    }

    return *result;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TEST_INCLUDED */
