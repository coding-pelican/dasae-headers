#include "dh/main.h"
#include "dh/prl.h"

TEST_fn_("test A_cat" $scope) {
    let arr3_lhs = A_from$((i32){ 0, 1, 2 });
    let arr3_rhs = A_from$((i32){ 3, 4, 5 });
    let res_3_add_3 = A_cat(arr3_lhs, arr3_rhs);

    try_(TEST_expect(A_len(res_3_add_3) == 6));
    for_(($a(res_3_add_3), $rf(0))(item, idx) {
        try_(TEST_expect(*item == as$(i32)(idx)));
    });

    let arr2_lhs = A_from$((i32){ 0, 1 });
    let arr6_rhs = A_from$((i32){ 2, 3, 4, 5, 6, 7 });
    let res_2_add_8 = A_cat(arr2_lhs, arr6_rhs);

    try_(TEST_expect(A_len(res_2_add_8) == 8));
    for_(($a(res_2_add_8), $rf(0))(item, idx) {
        try_(TEST_expect(*item == as$(i32)(idx)));
    });

    let arr6_lhs = A_from$((i32){ 0, 1, 2, 3, 4, 5 });
    let arr2_rhs = A_from$((i32){ 6, 7 });
    let res_6_add_2 = A_cat(arr6_lhs, arr2_rhs);

    try_(TEST_expect(A_len(res_6_add_2) == 8));
    for_(($a(res_6_add_2), $rf(0))(item, idx) {
        try_(TEST_expect(*item == as$(i32)(idx)));
    });
} $unscoped_(TEST_fn);

TEST_fn_("test A_cat$" $scope) {
    T_use_A$(6, i32);
    T_use_A$(8, i32);
    let arr3_lhs = A_from$((i32){ 0, 1, 2 });
    let arr3_rhs = A_from$((i32){ 3, 4, 5 });
    let res_3_add_3 = A_cat$((A$(6, i32))( arr3_lhs, arr3_rhs));

    try_(TEST_expect(A_len(res_3_add_3) == 6));
    for_(($a(res_3_add_3), $rf(0))(item, idx) {
        try_(TEST_expect(*item == as$(i32)(idx)));
    });

    let arr2_lhs = A_from$((i32){ 0, 1 });
    let arr6_rhs = A_from$((i32){ 2, 3, 4, 5, 6, 7 });
    let res_2_add_8 = A_cat$((A$(8, i32))(arr2_lhs, arr6_rhs));

    try_(TEST_expect(A_len(res_2_add_8) == 8));
    for_(($a(res_2_add_8), $rf(0))(item, idx) {
        try_(TEST_expect(*item == as$(i32)(idx)));
    });

    let arr6_lhs = A_from$((i32){ 0, 1, 2, 3, 4, 5 });
    let arr2_rhs = A_from$((i32){ 6, 7 });
    let res_6_add_2 = A_cat$((A$(8, i32))(arr6_lhs, arr2_rhs));

    try_(TEST_expect(A_len(res_6_add_2) == 8));
    for_(($a(res_6_add_2), $rf(0))(item, idx) {
        try_(TEST_expect(*item == as$(i32)(idx)));
    });
} $unscoped_(TEST_fn);
