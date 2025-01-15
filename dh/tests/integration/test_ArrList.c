// ArrList example usage
#define MAIN_NO_ARGS (1)
#include "dh/main.h"
#include "dh/core.h"
#include "dh/meta/common.h"
#include "dh/heap/Classic.h"
#include "dh/ArrList.h"
#include "dh/scope.h"
#include "dh/defer.h"

typedef struct Point {
    i32 x, y;
} Point;
use_Sli$(Point);
use_ArrList$(Point);

Err$void printPoints(const ArrList$Point* points) {
    reserveReturn(Err$void);
    debug_assert_nonnull(points);

    printf("Points (%zu items):\n", points->items.len);
    for_slice_indexed(points->items, point, index) {
        printf("  [%zu] = (%d, %d)\n", index, point->x, point->y);
    }
    return_void();
}

Err$void example(void) {
    reserveReturn(Err$void);
    scope_defer {
        // Initialize allocator
        heap_Classic heap      = {};
        let          allocator = heap_Classic_allocator(&heap);
        try_defer(heap_Classic_init(allocator));
        heap_Classic_fini(allocator);
        defer(heap_Classic_fini(allocator));

        // Create ArrList
        var points = typed(ArrList$Point, ArrList_init(typeInfo(Point), allocator));
        defer(ArrList_fini(&points.base));

        // Append individual items
        Point p1 = { .x = 10, .y = 20 };
        try_defer(ArrList_append(&points.base, meta_refPtr(&p1)));

        // Append multiple items at once
        Sli$Point more_points = Sli_arr((Point[]){
            { .x = 30, .y = 40 },
            { .x = 50, .y = 60 },
        });
        try_defer(ArrList_appendSlice(&points.base, meta_refSli(more_points)));

        // Print current state
        try_defer(printPoints(&points));

        // Insert at specific position
        Point p4 = { .x = 70, .y = 80 };
        try_defer(ArrList_insert(&points.base, 1, meta_refPtr(&p4)));
        printf("\nAfter insert at position 1:\n");
        try_defer(printPoints(&points));

        // Remove from middle
        printf("\nRemoved from index 1: ");
        let removed = meta_castPtr$(Point*, ArrList_removeOrdered(&points.base, 1));
        printf("(%d, %d)\n", removed->x, removed->y);
        try_defer(printPoints(&points));

        // Pop last element
        if_some(ArrList_popOrNull(&points.base), item_last) {
            let last = meta_castPtr$(Point*, item_last);
            printf("\nPopped last element: (%d, %d)\n", last->x, last->y);
            try_defer(printPoints(&points));
        }

        // Add many at once
        let new_space         = meta_castSli$(Sli$Point, try_defer(ArrList_addBackManyAsSlice(&points.base, 2)));
        *Sli_at(new_space, 0) = (Point){ .x = 90, .y = 100 };
        *Sli_at(new_space, 1) = (Point){ .x = 110, .y = 120 };
        printf("\nAfter adding two more points:\n");
        try_defer(printPoints(&points));

        // Clone the ArrList
        var cloned = typed(ArrList$Point, try_defer(ArrList_clone(&points.base)));
        defer(ArrList_fini(&cloned.base));
        printf("\nCloned array:\n");
        try_defer(printPoints(&cloned));

        // Convert to owned slice
        let owned_slice = meta_castSli$(Sli$Point, try_defer(ArrList_toOwnedSlice(&points.base)));
        printf("\nConverted to owned slice (length: %zu)\n", owned_slice.len);

        // Create new list from owned slice
        var from_slice = typed(ArrList$Point, ArrList_fromOwnedSlice(allocator, meta_refSli(owned_slice)));
        defer(ArrList_fini(&from_slice.base));
        printf("\nCreated from owned slice:\n");
        try_defer(printPoints(&from_slice));
        defer_return_void();
    }
    return_deferred;
}

Err$void dh_main(void) {
    reserveReturn(Err$void);
    try(example());
    return_void();
}
