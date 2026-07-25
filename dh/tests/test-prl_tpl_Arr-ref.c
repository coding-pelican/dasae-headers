#include "dh-main.h"
#include "dh/mem/common.h"

TEST_fn_("prl/tpl/Arr: A_ref" $scope) {
    T_use_A$(10, i32);
    let arr = A_from$((i32){ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    let sli = A_ref(arr);
    try_(TEST_expect(sli.ptr == A_ptr(arr)));
    try_(TEST_expect(sli.len == A_len(arr)));
    try_(TEST_expect(*A_at((arr)[0]) == 1));
    try_(TEST_expect(*A_at((arr)[9]) == 10));
} $unscoped(TEST_fn);

TEST_fn_("prl/tpl/Arr: S_deref" $scope) {
    T_use_A$(10, i32);
    let arr = A_from$((i32){ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    let ref_arr = A_ref(arr);
    try_(TEST_expect(ref_arr.ptr == A_ptr(arr)));
    try_(TEST_expect(ref_arr.len == A_len(arr)));
    try_(TEST_expect(*S_at((ref_arr)[0]) == 1));
    try_(TEST_expect(*S_at((ref_arr)[9]) == 10));

    var deref_sli = S_deref$((const A$10$i32)(ref_arr));
    try_(TEST_expect(A_ptr(deref_sli) != ref_arr.ptr));
    try_(TEST_expect(A_len(deref_sli) == ref_arr.len));
    try_(TEST_expect(A_ptr(deref_sli) != A_ptr(arr)));
    try_(TEST_expect(A_len(deref_sli) == A_len(arr)));
    try_(TEST_expect(*A_at((deref_sli)[0]) == 1));
    try_(TEST_expect(*A_at((deref_sli)[9]) == 10));
} $unscoped(TEST_fn);
