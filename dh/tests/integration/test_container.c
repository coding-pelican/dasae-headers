// build `clang -xc test_container.c ../../src/*.c -o test_container -g -static`

#include "../../include/dh/core/core.h"
#include "../../include/dh/ds/ds_container.h"
#include "../../include/dh/ds/ds_vector.h"

#include <stdio.h>


TArrayN(i32, 5);
TVector(f32);

i32 main() {
    // Stack-allocated Array<i32, 5>
    i32Array5* arr = create(i32Array5);
    i32Array5_init(arr);

    // Add elements
    for (usize i = 0; i < Array_length(i32, arr); ++i) {
        Array_set(i32, arr, i, i);
    }

    // Access elements
    for (usize i = 0; i < Array_length(i32, arr); ++i) {
        printf("%d ", Array_get(i32, arr, i));
    }
    printf("\n");

    defer_scope {
        // Heap-allocated Vector<f32>
        f32Vector* vec = create(f32Vector);
        f32Vector_init(vec, 1); // Initial capacity of 1
        defer((void (*)(void*))f32Vector_fini, vec);

        // Add elements
        for (usize i = 0; i < 16; ++i) {
            Vector_append(f32, vec, prim_value(f32, (i + 1) * 1.1f));
        }

        // Access elements
        for (usize i = 0; i < Vector_length(f32, vec); ++i) {
            printf("%.2f ", Vector_get(f32, vec, i));
        }
        // foreach (it, &vec) {
        //     printf("%.2f ", *it);
        // }
        printf("\n");
    }

    /* make memory leak */ {
        f32Vector* vec = create(f32Vector);
        f32Vector_init(vec, 1);

        // Add elements
        for (usize i = 0; i < 16; ++i) {
            Vector_append(f32, vec, prim_value(f32, (i + 1) * 1.1f));
        }

        // Access elements
        for (usize i = 0; i < Vector_length(f32, vec); ++i) {
            printf("%.2f ", Vector_get(f32, vec, i));
        }
        // foreach (it, &vec) {
        //     printf("%.2f ", *it);
        // }
        printf("\n");
    }

    return 0;
}
