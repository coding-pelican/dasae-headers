#include "dh/main.h"
#include "dh/Arr.h"
#include "dh/sli.h"
#include "dh/mem/common.h"

TEST_fn_("test Arr_ref", $scope) {
    use_Arr$(10, i32);
    let arr = (Arr$10$i32){ .buf = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };
    let sli = Arr_ref(arr);
    try_(TEST_expect(sli.ptr == Arr_ptr(arr)));
    try_(TEST_expect(sli.len == Arr_len(arr)));
    try_(TEST_expect(Arr_getAt(arr, 0) == 1));
    try_(TEST_expect(Arr_getAt(arr, 9) == 10));
} $unscoped_TEST_fn;

static fn_(printByteBits(u8 byte), void) {
    for (var i = 0; i < 8; ++i) { printf("%d", (byte >> (7 - i)) & 1); }
}

TEST_fn_("test Sli_deref", $scope) {
    use_Arr$(10, i32);
    let arr = (Arr$10$i32){ .buf = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };

    let ref_arr = Arr_ref(arr);
    try_(TEST_expect(Sli_ptr(ref_arr) == Arr_ptr(arr)));
    try_(TEST_expect(Sli_len(ref_arr) == Arr_len(arr)));
    try_(TEST_expect(Sli_getAt(ref_arr, 0) == 1));
    try_(TEST_expect(Sli_getAt(ref_arr, 9) == 10));

    var deref_sli = Sli_deref$(Arr$10$i32, ref_arr);
    try_(TEST_expect(Arr_ptr(deref_sli) != Sli_ptr(ref_arr)));
    try_(TEST_expect(Arr_len(deref_sli) == Sli_len(ref_arr)));
    try_(TEST_expect(Arr_ptr(deref_sli) != Arr_ptr(arr)));
    try_(TEST_expect(Arr_len(deref_sli) == Arr_len(arr)));
    try_(TEST_expect(Arr_getAt(deref_sli, 0) == 1));
    try_(TEST_expect(Arr_getAt(deref_sli, 9) == 10));

    let as_bytes = mem_asBytes(Arr_at(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { printByteBits(Sli_getAt(as_bytes, i)); }
    printf("\n");
    let as_bytes_const = mem_asBytes_const(Arr_at(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { printByteBits(Sli_getAt(as_bytes_const, i)); }
    printf("\n");
    let to_bytes = mem_toBytes(Arr_getAt(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { printByteBits(Arr_getAt(to_bytes, i)); }
    printf("\n");
    let to_bytes_const = mem_toBytes_const(Arr_getAt(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { printByteBits(Arr_getAt(to_bytes_const, i)); }
    printf("\n");
} $unscoped_TEST_fn;
