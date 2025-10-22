#include "dh/main.h"
#include "dh/mem/common.h"

TEST_fn_("Arr_ref" $scope) {
    use_Arr$(10, i32);
    let arr = (Arr$10$i32){ .buf = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };
    let sli = Arr_ref(arr);
    try_(TEST_expect(sli.ptr == Arr_ptr(arr)));
    try_(TEST_expect(sli.len == Arr_len(arr)));
    try_(TEST_expect(Arr_getAt(arr, 0) == 1));
    try_(TEST_expect(Arr_getAt(arr, 9) == 10));
} $unscoped_(TEST_fn);

#include "dh/io/common.h"
#include "dh/fs/File.h"
#include "dh/io/Writer.h"

$static
fn_((test_io_printBits(io_Writer out, u8 byte))(void)) {
    catch_((io_Writer_print(out, u8_l("{:08b}"), byte))($ignore, claim_unreachable));
}
$static
fn_((test_io_feedLine(io_Writer out))(void)) {
    catch_((io_Writer_print(out, u8_l("\n")))($ignore, claim_unreachable));
}

TEST_fn_("Sli_deref" $scope) {
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

    let out = fs_File_writer(io_getStdOut());

    let as_bytes = mem_asBytes(Arr_at(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { test_io_printBits(out, Sli_getAt(as_bytes, i)); }
    test_io_feedLine(out);

    let as_bytes_const = mem_asBytes_const(Arr_at(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { test_io_printBits(out, Sli_getAt(as_bytes_const, i)); }
    test_io_feedLine(out);

    let to_bytes = mem_toBytes(Arr_getAt(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { test_io_printBits(out, Arr_getAt(to_bytes, i)); }
    test_io_feedLine(out);

    let to_bytes_const = mem_toBytes_const(Arr_getAt(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { test_io_printBits(out, Arr_getAt(to_bytes_const, i)); }
    test_io_feedLine(out);
} $unscoped_(TEST_fn);

#define mem_asBytes_useT$(_T...) \
    $maybe_unused static fn_((tpl_id(mem_asBytes, _T)(_T * val))(Sli$u8)) { \
        return Sli_from$(Sli$u8, as$((u8*)(val)), sizeOf(*val)); \
    } \
    $maybe_unused static fn_((tpl_id(mem_asBytes_const, _T)(const _T* val))(Sli_const$u8)) { \
        return Sli_from$(Sli_const$u8, as$((const u8*)(val)), sizeOf(*val)); \
    }
#define mem_toBytes_useT$(_T...) \
    $maybe_unused typedef Arr$$(sizeOf(_T), _T) tpl_Ret(mem_toBytes, _T); \
    $maybe_unused static fn_((tpl_id(mem_toBytes, _T)(_T val))(tpl_Ret(mem_toBytes, _T))) { \
        return Sli_deref$(tpl_Ret(mem_toBytes, _T), tpl_id(mem_asBytes, _T)(&val)); \
    }

mem_asBytes_useT$(i32);
mem_toBytes_useT$(i32);
TEST_fn_("mem_asBytes/toBytes_useT$(i32)" $scope) {
    let out = fs_File_writer(io_getStdOut());

    i32 val = 1234567890;

    let sli = mem_asBytes_i32(&val);
    for (usize i = 0; i < sizeOf(i32); ++i) { test_io_printBits(out, Sli_getAt(sli, i)); }
    test_io_feedLine(out);

    let arr = mem_toBytes_i32(val);
    for (usize i = 0; i < sizeOf(i32); ++i) { test_io_printBits(out, Arr_getAt(arr, i)); }
    test_io_feedLine(out);
} $unscoped_(TEST_fn);
