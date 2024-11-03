#include "container.h"
#include <stdio.h>


typedef Array(int, 5) Array_int_5;
typedef Vector(float) Vector_float;

int main() {
    // Stack-allocated Array<int, 5>
    Array_int_5 arr = Array_init(int, 5, { 1, 2, 3, 4, 5 });

    // Add elements
    for (size_t i = 0; i < Array_len(&arr); ++i) {
        Array_at(&arr, i) = (int)i;
    }

    // Access elements
    for (size_t i = 0; i < Array_len(&arr); ++i) {
        printf("%d ", Array_at(&arr, i));
    }
    printf("\n");

    // Heap-allocated Vector<float>
    Vector_float vec = Vector_init(float, 2); // Initial capacity of 5
    defer(Vector_cleanup((void*)&vec)) {
        // Add elements
        for (size_t i = 0; i < 5; ++i) {
            Vector_push(&vec, (float)(i + 1) * 1.1f);
        }

        // Access elements
        for (size_t i = 0; i < Vector_len(&vec); ++i) {
            printf("%.2f ", Vector_at(&vec, i));
        }
        printf("\n");

        foreach (it, &vec) {
            printf("%.2f ", *it);
        }
    }

    return 0;
}
