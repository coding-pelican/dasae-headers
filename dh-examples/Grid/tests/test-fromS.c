#include <dh-main.h>
#include "Grid.h"

TEST_fn_("Grid: from slice" $scope) {
    T_use_Grid$(i32);
    let mat_raw = A_from$((i32){
        0, 1, 2,
        3, 4, 5,
        6, 7, 8 });
    let mat = Grid_fromS$(Grid_const$i32, A_ref$((S_const$i32)(mat_raw)), 3, 3);
    try_(TEST_expect(Grid_width(mat) == 3));
    try_(TEST_expect(Grid_height(mat) == 3));
    try_(TEST_expect(*Grid_at(mat, 0, 0) == 0));
    try_(TEST_expect(*Grid_at(mat, 1, 0) == 1));
    try_(TEST_expect(*Grid_at(mat, 2, 0) == 2));
    try_(TEST_expect(*Grid_at(mat, 0, 1) == 3));
    try_(TEST_expect(*Grid_at(mat, 1, 1) == 4));
    try_(TEST_expect(*Grid_at(mat, 2, 1) == 5));
    try_(TEST_expect(*Grid_at(mat, 0, 2) == 6));
    try_(TEST_expect(*Grid_at(mat, 1, 2) == 7));
    try_(TEST_expect(*Grid_at(mat, 2, 2) == 8));
    return_ok({});
} $unscoped(TEST_fn);
