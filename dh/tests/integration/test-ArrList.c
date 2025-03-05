#define main_no_args (1)
#include "dh/main.h"
#include "dh/core.h"
#include "dh/scope.h"
#include "dh/meta/common.h"
#include "dh/heap/Classic.h"
#include "dh/ArrList.h"

typedef struct Point {
    i32 x, y;
} Point;
use_Sli$(Point);
use_ArrList$(Point);

static must_check Err$void printPoints(const ArrList$Point* points) {
    reserveReturn(Err$void);
    debug_assert_nonnull(points);
    printf("Points (%zu items):\n", points->items.len);
    for_slice_indexed(points->items, point, index) {
        printf("  [%zu] = (%d, %d)\n", index, point->x, point->y);
    }
    return_void();
}

static must_check Err$void example(void) {
    scope_reserveReturn(Err$void) {
        // Initialize allocator
        let allocator = heap_Classic_allocator(&(heap_Classic){});

        // Create ArrList
        var points = type$(ArrList$Point, ArrList_init(typeInfo$(Point), allocator));
        defer(ArrList_fini(points.base));

        // Append individual items
        Point p1 = { .x = 10, .y = 20 };
        try(ArrList_append(points.base, meta_refPtr(&p1)));

        // Append multiple items at once
        Sli$Point more_points = Sli_arr((Point[]){
            { .x = 30, .y = 40 },
            { .x = 50, .y = 60 },
        });
        try(ArrList_appendSlice(points.base, meta_refSli(more_points)));

        // Print current state
        try(printPoints(&points));

        // Insert at specific position
        Point p4 = { .x = 70, .y = 80 };
        try(ArrList_insert(points.base, 1, meta_refPtr(&p4)));
        printf("\nAfter insert at position 1:\n");
        try(printPoints(&points));

        // Remove from middle
        printf("\nRemoved from index 1: ");
        let removed = meta_castPtr$(Point*, ArrList_removeOrdered(points.base, 1));
        printf("(%d, %d)\n", removed->x, removed->y);
        try(printPoints(&points));

        // Pop last element
        if_some(ArrList_popOrNull(points.base), item_last) {
            let last = meta_castPtr$(Point*, item_last);
            printf("\nPopped last element: (%d, %d)\n", last->x, last->y);
            try(printPoints(&points));
        }

        // Add many at once
        let new_space         = meta_castSli$(Sli$Point, try(ArrList_addBackManyAsSlice(points.base, 2)));
        *Sli_at(new_space, 0) = (Point){ .x = 90, .y = 100 };
        *Sli_at(new_space, 1) = (Point){ .x = 110, .y = 120 };
        printf("\nAfter adding two more points:\n");
        try(printPoints(&points));

        // Clone the ArrList
        var cloned = type$(ArrList$Point, try(ArrList_clone(points.base)));
        defer(ArrList_fini(cloned.base));
        printf("\nCloned array:\n");
        try(printPoints(&cloned));

        // Convert to owned slice
        let owned_slice = meta_castSli$(Sli$Point, try(ArrList_toOwnedSlice(points.base)));
        printf("\nConverted to owned slice (length: %zu)\n", owned_slice.len);

        // Create new list from owned slice
        var from_slice = type$(ArrList$Point, ArrList_fromOwnedSlice(allocator, meta_refSli(owned_slice)));
        defer(ArrList_fini(from_slice.base));
        printf("\nCreated from owned slice:\n");
        try(printPoints(&from_slice));

        return_void();
    }
    scope_returnReserved;
}

Err$void dh_main(void) {
    reserveReturn(Err$void);
    try(example());
    return_void();
}
