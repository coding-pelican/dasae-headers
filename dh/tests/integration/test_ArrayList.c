// ArrayList example usage
#define ERR_RES_MAIN_NO_ARGS (1)
#include "dh/main.h"
#include "dh/core.h"
#include "dh/meta/common.h"
#include "dh/heap/Classic.h"
#include "dh/ArrayList.h"
#include "dh/defer.h"
#include "dh/scope.h"

typedef struct Point {
    i32 x;
    i32 y;
} Point;
using_Sli$(Point);

Err$void printPoints(const ArrayList* points) {
    reserveReturn(Err$void);

    printf("Points (%zu items):\n", ArrayList_len(points));
    for (usize i = 0; i < ArrayList_len(points); ++i) {
        let point = (Point*)ArrayList_at(points, i).addr;
        printf("  [%zu] = (%d, %d)\n", i, point->x, point->y);
    }
    return_void();
}

Err$void example(void) {
    reserveReturn(Err$void);
    scope_defer {
        // Initialize allocator
        heap_Classic heap      = {};
        let          allocator = heap_Classic_allocator(&heap);
        try(heap_Classic_init(allocator));
        heap_Classic_fini(allocator);
        defer(heap_Classic_fini(allocator));

        // Create ArrayList
        var points = ArrayList_init(typeInfo(Point), allocator);
        defer(ArrayList_deinit(&points));

        // Append individual items
        Point p1 = { .x = 10, .y = 20 };
        try(ArrayList_append(&points, meta_sli((Sli$Point){ .ptr = &p1, .len = 1 })));

        // Append multiple items at once
        Sli$Point more_points = {
            .ptr = (Point[]){
                { .x = 30, .y = 40 },
                { .x = 50, .y = 60 } }
        };

        try(ArrayList_appendSlice(&points, meta_sli(more_points)));

        // Print current state
        try(printPoints(&points));

        // Insert at specific position
        Point p4 = { .x = 70, .y = 80 };
        try(ArrayList_insert(&points, 1, meta_ptr(&p4)));
        printf("\nAfter insert at position 1:\n");
        try(printPoints(&points));

        // Remove from middle
        printf("\nRemoved from index 1: ");
        let removed = ArrayList_orderedRemove(&points, 1);
        let point   = (Point*)removed.addr;
        printf("(%d, %d)\n", point->x, point->y);
        try(printPoints(&points));

        // Pop last element
        if_some(ArrayList_popOrNull(&points), last) {
            printf("\nPopped last element: (%d, %d)\n", ((Point*)last.addr)->x, ((Point*)last.addr)->y);
            try(printPoints(&points));
        }

        // Add many at once
        let    new_space  = try(ArrayList_addManyAsSlice(&points, 2));
        Point* new_points = (Point*)new_space.addr;
        new_points[0]     = (Point){ .x = 90, .y = 100 };
        new_points[1]     = (Point){ .x = 110, .y = 120 };
        printf("\nAfter adding two more points:\n");
        try(printPoints(&points));

        // Clone the ArrayList
        var cloned = try(ArrayList_clone(&points));
        defer(ArrayList_deinit(&cloned));
        printf("\nCloned array:\n");
        try(printPoints(&cloned));

        // Convert to owned slice
        let owned_slice = try(ArrayList_toOwnedSlice(&points));
        printf("\nConverted to owned slice (length: %zu)\n", owned_slice.len);

        // Create new list from owned slice
        var from_slice = ArrayList_fromOwnedSlice(allocator, owned_slice);
        defer(ArrayList_deinit(&from_slice));
        printf("\nCreated from owned slice:\n");
        try(printPoints(&from_slice));
    }
    scope_deferred;
    return_void();
}

Err$void dh_main(void) {
    reserveReturn(Err$void);
    try(example());
    return_void();
}
