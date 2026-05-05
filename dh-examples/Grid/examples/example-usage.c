#include <dh-main.h>
#include "Grid.h"

#include <dh/io/stream.h>

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    var mat_raw = A_from$((i32){
        0, 1, 2,
        3, 4, 5,
        6, 7, 8 });
    T_use_Grid$(i32);

    var mat = Grid_fromS$(Grid$i32, A_ref$((S$i32)(mat_raw)), 3, 3);
    for_grid(mat, (item, (x, y)), {
        io_stream_print(u8_l("[{:d}, {:d}] = {:d} "), x, y, item);
    });

    return_ok({});
} $unscoped(fn);
