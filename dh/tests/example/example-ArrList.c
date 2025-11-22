#include "dh/main.h"
#include "dh/heap/Page.h"
#include "dh/ArrList.h"
#include "dh/io/stream.h"

typedef struct Point {
    i32 x, y;
} Point;
T_use$((Point)(P, S, O));
T_use$((Point)(
    ArrList,
    ArrList_init,
    ArrList_fini,
    ArrList_append,
    ArrList_appendS,
    ArrList_insert,
    ArrList_pop,
    ArrList_removeOrd
));

$static fn_((printPoints(ArrList$Point points))(void)) {
    io_stream_print(u8_l("Points ({:uz} items):\n"), points.items.len);
    for_(($rf(0), $s(points.items))(index, point) {
        io_stream_print(u8_l("  [{:uz}] = ({:d}, {:d})\n"), index, point->x, point->y);
    });
}

$must_check
$static
fn_((example(void))(mem_Err$void) $guard) {
    // Initialize the allocator
    let gpa = heap_Page_allocator(&(heap_Page){});
    // Initialize the array list
    var points = try_(ArrList_init$Point(gpa, 8));

    // ArrList$Point points = { .as_raw[0] =  };
    defer_(ArrList_fini$Point(&points, gpa));

    // Append a single item
    try_(ArrList_append$Point(&points, gpa, (Point){ .x = 10, .y = 20 }));

    // Append multiple items at once
    var more_points = A_from$((Point){
        [0] = { .x = 30, .y = 40 },
        [1] = { .x = 50, .y = 60 },
    });
    try_(ArrList_appendS$Point(&points, gpa, ref$A$((Point)(more_points)).as_const));

    // Print current state
    printPoints(points);

    // Insert at specific position
    Point p4 = { .x = 70, .y = 80 };
    try_(ArrList_insert$Point(&points, gpa, 1, p4));
    io_stream_print(u8_l("After insert at position 1:\n"));
    printPoints(points);

    // Remove from middle
    io_stream_print(u8_l("Removed from index 1: "));
    let removed = ArrList_removeOrd$Point(&points, 1);
    io_stream_print(u8_l("({:d}, {:d})\n"), removed.x, removed.y);
    printPoints(points);

    // Pop last element
    while_some(ArrList_pop$Point(&points), item_last) {
        io_stream_print(u8_l("Popped last element: ({:d}, {:d})\n"), item_last.x, item_last.y);
        printPoints(points);
    }

    return_ok({});
} $unguarded_(fn);

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    try_(example());
    io_stream_print(u8_l("Hello, world!\n"));
    return_ok({});
} $unscoped_(fn);
