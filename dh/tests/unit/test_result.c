/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    test_result.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-30 (date of creation)
 * @updated 2024-12-01 (date of last update)
 * @version v1.0.0
 * @ingroup UNIT_TEST_IMPLEMENTATION
 * @prefix  TEST
 *
 * @brief   Test of Result type
 */

#include "dh/Result.h"
#include "dh/debug/assert.h"

#include <stdio.h>

// Test helper functions
void TEST_PrintSection(const char* section_name) {
    printf("\n=== Testing %s ===\n", section_name);
}

void TEST_PrintResult(const char* test_name, i32 success) {
    printf("%s: %s\n", test_name, success ? "PASSED" : "FAILED");
}

static Result_i32 divide(i32 a, i32 b) {
    if (b == 0) {
        return Result_err(Result_i32, Error_invalid_argument);
    }
    return Result_ok(Result_i32, a / b);
}

void TEST_Result_basics(void) {
    TEST_PrintSection("Result Basic Operations");

    // Test Ok result
    Result_i32 ok_result = Result_ok(Result_i32, 42);
    TEST_PrintResult("Result Ok creation", Result_isOk(ok_result));
    TEST_PrintResult("Result Ok value", Result_unwrap(Result_i32, ok_result) == 42);

    // Test Err result
    Result_i32 err_result = Result_err(Result_i32, Error_invalid_argument);
    TEST_PrintResult("Result Err creation", Result_isErr(err_result));
    TEST_PrintResult("Result Err value", Result_unwrapErr(Result_i32, err_result) == Error_invalid_argument);
}

void TEST_Result_division(void) {
    TEST_PrintSection("Result Division Operations");

    // Test successful division
    Result_i32 success = divide(10, 2);
    TEST_PrintResult("Division success check", Result_isOk(success));
    TEST_PrintResult("Division success value", Result_unwrap(Result_i32, success) == 5);

    // Test division by zero
    Result_i32 failure = divide(10, 0);
    TEST_PrintResult("Division error check", Result_isErr(failure));
    TEST_PrintResult("Division error value", Result_unwrapErr(Result_i32, failure) == Error_invalid_argument);
}

void TEST_Result_unwrap_or(void) {
    TEST_PrintSection("Result UnwrapOr Operations");

    const i32 items[] = { 1, 2, 3, 4, 5 };
    i32       index   = 2;

    // Test unwrap_or with side effects
    i32 value = Result_unwrapOr(Result_i32, divide(items[index++], 0), 1);
    TEST_PrintResult("UnwrapOr side effect", index == 3);
    TEST_PrintResult("UnwrapOr default value", value == 1);

    // Test unwrap_or with successful case
    i32 success_value = Result_unwrapOr(Result_i32, divide(6, 2), 0);
    TEST_PrintResult("UnwrapOr success value", success_value == 3);
}

int main(void) {
    printf("Starting Result Type Tests\n");

    TEST_Result_basics();
    TEST_Result_division();
    TEST_Result_unwrap_or();

    printf("\nAll tests completed.\n");
    return 0;
}
