/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    test_core_Res.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-30 (date of creation)
 * @updated 2024-12-01 (date of last update)
 * @version v1.0.0
 * @ingroup UNIT_TEST_IMPLEMENTATION
 * @prefix  TEST
 *
 * @brief   Test of Res type
 */

#include "dh/core/Res.h"
#include "dh/debug/assert.h"

#include <stdio.h>

// Test helper functions
void TEST_PrintSection(const char* section_name) {
    printf("\n=== Testing %s ===\n", section_name);
}

void TEST_PrintResult(const char* test_name, i32 success) {
    printf("%s: %s\n", test_name, success ? "PASSED" : "FAILED");
}

static Res_i32 divide(i32 a, i32 b) {
    if (b == 0) {
        return Res_err(Res_i32, Err_invalid_argument);
    }
    return Res_ok(Res_i32, a / b);
}

void TEST_Res_basics(void) {
    TEST_PrintSection("Res Basic Operations");

    // Test Ok result
    Res_i32 ok_result = Res_ok(Res_i32, 42);
    TEST_PrintResult("Res Ok creation", Res_isOk(ok_result));
    TEST_PrintResult("Res Ok value", Res_unwrap(Res_i32, ok_result) == 42);

    // Test Err result
    Res_i32 err_result = Res_err(Res_i32, Err_invalid_argument);
    TEST_PrintResult("Res Err creation", Res_isErr(err_result));
    TEST_PrintResult("Res Err value", Res_unwrapErr(Res_i32, err_result) == Err_invalid_argument);
}

void TEST_Res_division(void) {
    TEST_PrintSection("Res Division Operations");

    // Test successful division
    Res_i32 success = divide(10, 2);
    TEST_PrintResult("Division success check", Res_isOk(success));
    TEST_PrintResult("Division success value", Res_unwrap(Res_i32, success) == 5);

    // Test division by zero
    Res_i32 failure = divide(10, 0);
    TEST_PrintResult("Division error check", Res_isErr(failure));
    TEST_PrintResult("Division error value", Res_unwrapErr(Res_i32, failure) == Err_invalid_argument);
}

void TEST_Res_unwrap_or(void) {
    TEST_PrintSection("Res UnwrapOr Operations");

    const i32 items[] = { 1, 2, 3, 4, 5 };
    i32       index   = 2;

    // Test unwrap_or with side effects
    i32 value = Res_unwrapOr(Res_i32, divide(items[index++], 0), 1);
    TEST_PrintResult("UnwrapOr side effect", index == 3);
    TEST_PrintResult("UnwrapOr default value", value == 1);

    // Test unwrap_or with successful case
    i32 success_value = Res_unwrapOr(Res_i32, divide(6, 2), 0);
    TEST_PrintResult("UnwrapOr success value", success_value == 3);
}

int main(void) {
    printf("Starting Res Type Tests\n");

    TEST_Res_basics();
    TEST_Res_division();
    TEST_Res_unwrap_or();

    printf("\nAll tests completed.\n");
    return 0;
}
