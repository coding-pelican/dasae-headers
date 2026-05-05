#include <dh-main.h>
#include "myheader.h"

TEST_fn_("Grid: len" $scope) {
    T_use_Grid$(i32);
    let mat_raw = A_from$((i32){
        0, 1, 2,
        3, 4, 5,
        6, 7, 8 });
    let mat = Grid_fromS$(Grid_const$i32, A_ref$((S_const$i32)(mat_raw)), 3, 3);
    try_(TEST_expect(S_len(mat.items) == 9));
    return_ok({});
} $unscoped(TEST_fn);
