// build `clang -o test_ds_vec -xc -std=c17 test_ds_vec.c ../../src/*.c -static -g  -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\tests\test_terminal\dh\include`

#include "dh/core.h"
#include "dh/core/Res.h"
#include "dh/heap/Classic.h"
#include "dh/ds/Vec.h"
#include "dh/defer.h"
#include "dh/mem/Allocator.h"

#include <stdio.h>

Vec(f32) Vec_f32;
Vec(isize) Vec_isize;

i32 main() {
    heap_Classic  classic = cleared();
    mem_Allocator heap    = heap_Classic_allocator(&classic);
    Res_unwrap(Res_Void, heap_Classic_init(&heap));
    scope_defer {
        defer(heap_Classic_fini(&heap));

        Vec_isize one_to_three_isize = Res_unwrap(Res_Vec, Vec_initWithCapacity(isize, &heap, 3));
        defer(Vec_fini(&one_to_three_isize));
        Res_unwrap(Res_Void, Vec_append(f32, &one_to_three_isize, literal(isize, 123)));
        Res_unwrap(Res_Void, Vec_append(f32, &one_to_three_isize, literal(isize, 456)));
        Res_unwrap(Res_Void, Vec_append(f32, &one_to_three_isize, literal(isize, 789)));
        printf("Vec_isize: ");
        printf("%lld ", Vec_at(isize, &one_to_three_isize, 0));
        printf("%lld ", Vec_at(isize, &one_to_three_isize, 1));
        printf("%lld ", Vec_at(isize, &one_to_three_isize, 2));
        printf("\n");

        Vec_f32 one_to_three = Res_unwrap(Res_Vec, Vec_initWithCapacity(f32, &heap, 3));
        defer(Vec_fini(&one_to_three));
        Res_unwrap(Res_Void, Vec_append(f32, &one_to_three, literal(f32, 0.0f)));
        Res_unwrap(Res_Void, Vec_append(f32, &one_to_three, literal(f32, 0.0f)));
        Res_unwrap(Res_Void, Vec_append(f32, &one_to_three, literal(f32, 0.0f)));
        printf("Vec_f32: ");
        printf("%g ", Vec_at(f32, &one_to_three, 0));
        printf("%g ", Vec_at(f32, &one_to_three, 1));
        printf("%g ", Vec_at(f32, &one_to_three, 2));
        printf("\n");

        // Heap-allocated Vec<f32>
        Vec_f32* vec = (Vec_f32[1]){ Res_unwrap(Res_Vec, Vec_initWithCapacity(f32, &heap, 1)) }; // Initial capacity of 1

        defer(printf("finished\n"));
        defer({
            printf("finish\n");
            Vec_fini(vec);
        });
        defer(printf("finishing\n"));

        // Add elements
        for (usize i = 0; i < 8; ++i) {
            Res_unwrap(Res_Void, Vec_append(f32, vec, literal(f32, (i + 1) * 1.1f)));
        }

        // Access elements
        printf("case 1: ");
        for (usize i = 0; i < Vec_len(vec); ++i) {
            printf("%.2f ", Vec_at(f32, vec, i));
        }
        printf("\n");
        printf("case 2: ");

        foreach (f32, it, vec->items_) {
            printf("%.2f ", *it);
        }
        printf("\n");
    }
    scope_deferred;

    // /* make memory leak */ {
    //     Vec* vec = create(Vec);
    //     Vec_initWithCap(f32, vec, 1);

    //     // Add elements
    //     for (usize i = 0; i < 8; ++i) {
    //         Vec_append(vec, &literal(f32, (i + 1) * 1.1f));
    //     }

    //     // Access elements
    //     for (usize i = 0; i < Vec_len(vec); ++i) {
    //         printf("%.2f ", rawCastDeref(f32, Vec_at(vec, i)));
    //     }
    //     foreach (f32, it, vec) {
    //         printf("%.2f ", *it);
    //     }
    //     printf("\n");
    // }

    return 0;
}
