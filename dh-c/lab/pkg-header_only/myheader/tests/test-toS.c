#include <dh-main.h>
#include "myheader.h"

TEST_fn_("Grid: to slice" $scope) {
    T_use_Grid$(i32);
    let mat_raw = A_from$((i32){
        0, 1, 2,
        3, 4, 5,
        6, 7, 8 });
    let mat = Grid_fromS$(Grid_const$i32, A_ref$((S_const$i32)(mat_raw)), 3, 3);
    let sli = mat.items;
    try_(TEST_expect(mat_raw.val == mat.items.ptr));
    try_(TEST_expect(mat.items.ptr == sli.ptr));
    return_ok({});
} $unscoped(TEST_fn);
