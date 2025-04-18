#include "dh/main.h"
#include "dh/Arr.h"

fn_TEST_scope("test Arr_cat") {
    let arr3_lhs    = Arr_from$(i32, { 0, 1, 2 });
    let arr3_rhs    = Arr_from$(i32, { 3, 4, 5 });
    let res_3_add_3 = Arr_cat(arr3_lhs, arr3_rhs);

    try_(TEST_expect(Arr_len(res_3_add_3) == 6));
    for_array_indexed (res_3_add_3, item, idx) {
        try_(TEST_expect(*item == as$(i32, idx)));
    }

    let arr2_lhs    = Arr_from$(i32, { 0, 1 });
    let arr6_rhs    = Arr_from$(i32, { 2, 3, 4, 5, 6, 7 });
    let res_2_add_8 = Arr_cat(arr2_lhs, arr6_rhs);

    try_(TEST_expect(Arr_len(res_2_add_8) == 8));
    for_array_indexed (res_2_add_8, item, idx) {
        try_(TEST_expect(*item == as$(i32, idx)));
    }

    let arr6_lhs    = Arr_from$(i32, { 0, 1, 2, 3, 4, 5 });
    let arr2_rhs    = Arr_from$(i32, { 6, 7 });
    let res_6_add_2 = Arr_cat(arr6_lhs, arr2_rhs);

    try_(TEST_expect(Arr_len(res_6_add_2) == 8));
    for_array_indexed (res_6_add_2, item, idx) {
        try_(TEST_expect(*item == as$(i32, idx)));
    }
} TEST_unscoped;

use_Arr$(6, i32);
use_Arr$(8, i32);
fn_TEST_scope("test Arr_cat$") {
    let arr3_lhs    = Arr_from$(i32, { 0, 1, 2 });
    let arr3_rhs    = Arr_from$(i32, { 3, 4, 5 });
    let res_3_add_3 = Arr_cat$(Arr$(6, i32), arr3_lhs, arr3_rhs);

    try_(TEST_expect(Arr_len(res_3_add_3) == 6));
    for_array_indexed (res_3_add_3, item, idx) {
        try_(TEST_expect(*item == as$(i32, idx)));
    }

    let arr2_lhs    = Arr_from$(i32, { 0, 1 });
    let arr6_rhs    = Arr_from$(i32, { 2, 3, 4, 5, 6, 7 });
    let res_2_add_8 = Arr_cat$(Arr$(8, i32), arr2_lhs, arr6_rhs);

    try_(TEST_expect(Arr_len(res_2_add_8) == 8));
    for_array_indexed (res_2_add_8, item, idx) {
        try_(TEST_expect(*item == as$(i32, idx)));
    }

    let arr6_lhs    = Arr_from$(i32, { 0, 1, 2, 3, 4, 5 });
    let arr2_rhs    = Arr_from$(i32, { 6, 7 });
    let res_6_add_2 = Arr_cat$(Arr$(8, i32), arr6_lhs, arr2_rhs);

    try_(TEST_expect(Arr_len(res_6_add_2) == 8));
    for_array_indexed (res_6_add_2, item, idx) {
        try_(TEST_expect(*item == as$(i32, idx)));
    }
} TEST_unscoped;
