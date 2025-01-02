// ArrayList example usage
#define MAIN_NO_ARGS (1)
#include "dh/main.h"
#include "dh/core.h"
#include "dh/meta/common.h"
#include "dh/heap/Classic.h"
#include "dh/ArrayList.h"
#include "dh/scope.h"
#include "dh/defer.h"

typedef struct Point {
    i32 x, y;
} Point;
using_Sli$(Point);

Err$void printPoints(const ArrayList* points) {
    reserveReturn(Err$void);
    debug_assert_nonnull(points);

    printf("Points (%zu items):\n", ArrayList_len(points));
    scope_with(let items = meta_castSli(SliConst$Point, ArrayList_items(points))) {
        for_indexed_slice(items, point, index) {
            printf("  [%zu] = (%d, %d)\n", index, point->x, point->y);
        }
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
        defer(ArrayList_fini(&points));

        // Append individual items
        Point p1 = { .x = 10, .y = 20 };
        try(ArrayList_append(&points, meta_ptr(&p1)));

        // Append multiple items at once
        Sli$Point more_points = Sli_arr((Point[]){
            { .x = 30, .y = 40 },
            { .x = 50, .y = 60 },
        });
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
        let removed = meta_castPtr(Point*, ArrayList_orderedRemove(&points, 1));
        printf("(%d, %d)\n", removed->x, removed->y);
        try(printPoints(&points));

        // Pop last element
        if_some(ArrayList_popOrNull(&points), item_last) {
            let last = meta_castPtr(Point*, item_last);
            printf("\nPopped last element: (%d, %d)\n", last->x, last->y);
            try(printPoints(&points));
        }

        // Add many at once
        let new_space         = meta_castSli(Sli$Point, try(ArrayList_addManyAsSlice(&points, 2)));
        *Sli_at(new_space, 0) = (Point){ .x = 90, .y = 100 };
        *Sli_at(new_space, 1) = (Point){ .x = 110, .y = 120 };
        printf("\nAfter adding two more points:\n");
        try(printPoints(&points));

        // Clone the ArrayList
        var cloned = try(ArrayList_clone(&points));
        defer(ArrayList_fini(&cloned));
        printf("\nCloned array:\n");
        try(printPoints(&cloned));

        // Convert to owned slice
        let owned_slice = meta_castSli(Sli$Point, try(ArrayList_toOwnedSlice(&points)));
        printf("\nConverted to owned slice (length: %zu)\n", owned_slice.len);

        // Create new list from owned slice
        var from_slice = ArrayList_fromOwnedSlice(allocator, meta_sli(owned_slice));
        defer(ArrayList_fini(&from_slice));
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
