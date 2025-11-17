#include "dh/main.h"
#include "dh/mem/common.h"

TEST_fn_("A_ref" $scope) {
    T_use_A$(10, i32);
    let arr = A_from$((i32){ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
    let sli = A_ref(arr);
    try_(TEST_expect(sli.ptr == A_ptr(arr)));
    try_(TEST_expect(sli.len == A_len(arr)));
    try_(TEST_expect(*A_at((arr)[0]) == 1));
    try_(TEST_expect(*A_at((arr)[9]) == 10));
} $unscoped_(TEST_fn);

#include "dh/io/common.h"
#include "dh/fs/File.h"
#include "dh/io/Writer.h"

#if UNUSED_CODE
$static fn_((test_io_printBits(io_Writer out, u8 byte))(void)) {
    catch_((io_Writer_print(out, u8_l("{:08b}"), byte))($ignore, claim_unreachable));
}
$static fn_((test_io_feedLine(io_Writer out))(void)) {
    catch_((io_Writer_print(out, u8_l("\n")))($ignore, claim_unreachable));
}
#endif /* UNUSED_CODE */

TEST_fn_("S_deref" $scope) {
    T_use_A$(10, i32);
    let arr = A_from$((i32){ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    let ref_arr = A_ref(arr);
    try_(TEST_expect(ref_arr.ptr == A_ptr(arr)));
    try_(TEST_expect(ref_arr.len == A_len(arr)));
    try_(TEST_expect(*S_at((ref_arr)[0]) == 1));
    try_(TEST_expect(*S_at((ref_arr)[9]) == 10));

    var deref_sli = S_deref$((A$10$i32)(ref_arr));
    try_(TEST_expect(A_ptr(deref_sli) != ref_arr.ptr));
    try_(TEST_expect(A_len(deref_sli) == ref_arr.len));
    try_(TEST_expect(A_ptr(deref_sli) != A_ptr(arr)));
    try_(TEST_expect(A_len(deref_sli) == A_len(arr)));
    try_(TEST_expect(*A_at((deref_sli)[0]) == 1));
    try_(TEST_expect(*A_at((deref_sli)[9]) == 10));

#if UNUSED_CODE
    let out = fs_File_writer(io_getStdOut());

    let as_bytes = mem_asBytesMut(A_at(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { test_io_printBits(out, Sli_getAt(as_bytes, i)); }
    test_io_feedLine(out);

    let as_bytes_const = mem_asBytesRef(A_at(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { test_io_printBits(out, Sli_getAt(as_bytes_const, i)); }
    test_io_feedLine(out);

    let to_bytes = mem_toBytes(A_getAt(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { test_io_printBits(out, A_getAt(to_bytes, i)); }
    test_io_feedLine(out);

    let to_bytes_const = mem_toBytes(A_getAt(deref_sli, 0));
    for (usize i = 0; i < sizeOf(i32); ++i) { test_io_printBits(out, A_getAt(to_bytes_const, i)); }
    test_io_feedLine(out);
#endif /* UNUSED_CODE */
} $unscoped_(TEST_fn);

#if UNUSED_CODE
#define mem_asBytes_useT$(_T...) \
    $maybe_unused static fn_((tpl_id(mem_asBytes, _T)(_T * val))(S$u8)) { \
        return Sli_from$(S$u8, as$(u8*)(val), sizeOf(*val)); \
    } \
    $maybe_unused static fn_((tpl_id(mem_asBytes_const, _T)(const _T* val))(S_const$u8)) { \
        return Sli_from$(S_const$u8, as$(const u8*)(val), sizeOf(*val)); \
    }
#define mem_toBytes_useT$(_T...) \
    $maybe_unused typedef A$$(sizeOf(_T), _T) tpl_Ret(mem_toBytes, _T); \
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
    for (usize i = 0; i < sizeOf(i32); ++i) { test_io_printBits(out, A_getAt(arr, i)); }
    test_io_feedLine(out);
} $unscoped_(TEST_fn);
#endif /* UNUSED_CODE */
