/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    test_defer_jmp.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05
 * @updated 2024-11-15
 * @version v1.0.0
 * @ingroup UNIT_TEST_IMPLEMENTATION
 * @prefix  TEST
 */
// build `clang -o test_defer_jmp -xc -std=c17 test_defer.c ../../src/*.c -static -g  -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\tests\test_terminal\dh\include`


/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/debug/assert.h"

#include "dh/ds/container.h"
#include "dh/ds/vector.h"
#include "dh/defer_jmp.h"

#include <stdio.h>

/*========== Test Cases =====================================================*/

static i32 cleanup_counter = 0;

static void TEST_defer_countCleanup(anyptr context) {
    unused(context);
    cleanup_counter++;
    printf("Cleanup called: %d\n", cleanup_counter);
}

#define get_stack_pointer() (__builtin_return_address(0))

static void TEST_defer_BasicUsage(void) {
    cleanup_counter = 0;

    int   stack_var = 42;                  // 스택 변수 추가
    void* sp_start  = get_stack_pointer(); // 시작 시 스택 포인터

    printf("Entering defer scope (sp: %p)\n", sp_start);
    defer_scope {
        printf("Entered defer scope (stack_var: %d, sp: %p)\n", stack_var, get_stack_pointer());

        defer({
            void* sp_defer1 = get_stack_pointer();
            printf("Cleanup 1 (stack_var: %d, sp: %p)\n", stack_var, sp_defer1);
            TEST_defer_countCleanup(null);
        });

        defer({
            void* sp_defer2 = get_stack_pointer();
            printf("Cleanup 2 (stack_var: %d, sp: %p)\n", stack_var, sp_defer2);
            TEST_defer_countCleanup(null);
        });

        printf("Exiting defer scope (stack_var: %d)\n", stack_var);
    }
    printf("Exited defer scope\n");

    debug_assert(cleanup_counter == 2);
}

static void TEST_defer_NestedDefers(void) {
    cleanup_counter = 0;

    defer_scope {
        defer({ TEST_defer_countCleanup(null); });

        defer({
            TEST_defer_countCleanup(null);
            defer({ TEST_defer_countCleanup(null); });
            TEST_defer_countCleanup(null);
        });

        defer({ TEST_defer_countCleanup(null); });
    }

    debug_assert(cleanup_counter == 3);
}

static void TEST_defer_NestedScopes(void) { // NOLINT
    cleanup_counter = 0;

    defer_scope {
        defer({ TEST_defer_countCleanup(null); });

        defer_scope {
            defer({ TEST_defer_countCleanup(null); });
        }

        defer({ TEST_defer_countCleanup(null); });
    }

    debug_assert(cleanup_counter == 3);
}

static void TEST_defer_EarlyReturn(void) {
    cleanup_counter = 0;

    defer_scope {
        defer({ TEST_defer_countCleanup(null); });
        defer_scope_return;
        defer({ TEST_defer_countCleanup(null); }); // Should not be executed
    }

    debug_assert(cleanup_counter == 1);
}

static void TEST_defer_modifyValue(anyptr context) {
    i32* value = (i32*)context;
    (*value)++;
}

static void TEST_defer_ContextPassing(void) {
    i32 test_value = 0;

    defer_scope {
        defer({ TEST_defer_modifyValue(&test_value); });
        defer({ TEST_defer_modifyValue(&test_value); });
    }

    debug_assert(test_value == 2);
}

TVector(f32);

static void TEST_defer_cleanupVec(void* vec) {
    f32Vector_fini((f32Vector*)vec);
}

static void TEST_defer_VectorCleanup(void) {
    bool vector_initialized = false;

    defer_scope {
        f32Vector* vec = create(f32Vector);
        f32Vector_init(vec, 4);
        defer({
            TEST_defer_cleanupVec(vec);
            printf("Vector cleanup");
        });
        vector_initialized = true;

        Vector_append(f32, vec, createWith(f32, 1.0f));
        Vector_append(f32, vec, createWith(f32, 2.0f));
        Vector_append(f32, vec, createWith(f32, 3.0f));
        Vector_append(f32, vec, createWith(f32, 4.0f));

        for (usize i = 0; i < Vector_length(f32, vec); ++i) {
            debug_assert(Vector_get(f32, vec, i) == (f32)(i + 1.0f));
            printf("Value at index %zu: %f\n", i, Vector_get(f32, vec, i));
        }
    }

    defer_scope {
        f32Vector* vec = create(f32Vector);
        f32Vector_init(vec, 4);
        defer({ f32Vector_fini(vec); });
        vector_initialized = true;

        Vector_append(f32, vec, createWith(f32, 5.0f));
        Vector_append(f32, vec, createWith(f32, 6.0f));
        Vector_append(f32, vec, createWith(f32, 7.0f));
        Vector_append(f32, vec, createWith(f32, 8.0f));

        for (usize i = 0; i < Vector_length(f32, vec); ++i) {
            debug_assert(Vector_get(f32, vec, i) == (f32)(i + 5.0f));
            printf("Value at index %zu: %f\n", i, Vector_get(f32, vec, i));
        }
    }

    debug_assert(vector_initialized);
}


i32 main(void) {
    TEST_defer_BasicUsage();
    TEST_defer_NestedDefers();
    TEST_defer_NestedScopes();
    TEST_defer_EarlyReturn();
    TEST_defer_ContextPassing();
    TEST_defer_VectorCleanup();

    return 0;
}
