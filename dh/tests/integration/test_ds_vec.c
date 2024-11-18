// build `clang -o test_ds_vec -xc -std=c17 test_ds_vec.c ../../src/*.c -static -g  -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\tests\test_terminal\dh\include`

#include "dh/core.h"
#include "dh/core/cfg.h"
#include "dh/ds/Vec.h"
#include "dh/defer.h"

#include <stdio.h>

i32 main() {
    // Stack-allocated Array<i32, 5>
    // ds_Arr_i32_5* arr = create(ds_Arr_i32_5);
    // ds_Arr_init(arr);

    // // Add elements
    // for (usize i = 0; i < ds_Arr_length(i32, arr); ++i) {
    //     ds_Arr_set(i32, arr, i, i);
    // }

    // // Access elements
    // for (usize i = 0; i < ds_Arr_length(i32, arr); ++i) {
    //     printf("%d ", ds_Arr_get(i32, arr, i));
    // }
    // printf("\n");

    defer_block {
        // Heap-allocated ds_Vec<f32>
        ds_Vec_f32* vec = create(ds_Vec_f32);
        ds_Vec_initWithCap(f32, vec, 1); // Initial capacity of 1
        defer(printf("finished\n"));
        defer({
            printf("finish\n");
            ds_Vec_fini(vec);
        });
        defer(printf("finishing\n"));

        // Add elements
        for (usize i = 0; i < 8; ++i) {
            ds_Vec_append(vec, &literal(f32, (i + 1) * 1.1f));
        }

        // Access elements
        printf("case 1: ");
        for (usize i = 0; i < ds_Vec_len(vec); ++i) {
            printf("%.2f ", accessCastPtr(f32, ds_Vec_at(vec, i)));
        }
        printf("\n");
        printf("case 2: ");
        foreach (f32, it, vec) {
            printf("%.2f ", *it);
        }
        printf("\n");
    }
    block_deferred();

    /* make memory leak */ {
        ds_Vec* vec = create(ds_Vec);
        ds_Vec_initWithCap(f32, vec, 1);

        // Add elements
        for (usize i = 0; i < 8; ++i) {
            ds_Vec_append(vec, &literal(f32, (i + 1) * 1.1f));
        }

        // Access elements
        for (usize i = 0; i < ds_Vec_len(vec); ++i) {
            printf("%.2f ", accessCastPtr(f32, ds_Vec_at(vec, i)));
        }
        foreach (f32, it, vec) {
            printf("%.2f ", *it);
        }
        printf("\n");
    }

    return 0;
}
