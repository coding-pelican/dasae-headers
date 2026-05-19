#include "dh-main.h"
#include "dh/fs/path.h"
#include "dh/mem/common.h"

TEST_fn_("fs/path: split basename dirname stem extension" $scope) {
    let path = u8_l("foo/bar/archive.tar.gz");

    try_(TEST_expect(mem_eqlBytes(fs_path_basename(path), u8_l("archive.tar.gz"))));
    try_(TEST_expect(mem_eqlBytes(fs_path_dirname(path), u8_l("foo/bar"))));
    try_(TEST_expect(mem_eqlBytes(fs_path_extension(path), u8_l(".gz"))));
    try_(TEST_expect(mem_eqlBytes(fs_path_stem(path), u8_l("archive.tar"))));
} $unscoped(TEST_fn);

TEST_fn_("fs/path: normalize and join" $scope) {
    var_(buf, A$$(64, u8)) = A_zero();

    let joined = try_(fs_path_join2(u8_l("foo"), u8_l("bar"), A_ref$((S$u8)(buf))));
    let expected_joined = pp_if_(plat_is_windows)(
        pp_then_(u8_l("foo\\bar")),
        pp_else_(u8_l("foo/bar")));
    try_(TEST_expect(mem_eqlBytes(joined.as_const, expected_joined)));

    let normalized = try_(fs_path_normalize(u8_l("foo//bar\\baz"), A_ref$((S$u8)(buf))));
    let expected_normalized = pp_if_(plat_is_windows)(
        pp_then_(u8_l("foo\\bar\\baz")),
        pp_else_(u8_l("foo/bar/baz")));
    try_(TEST_expect(mem_eqlBytes(normalized.as_const, expected_normalized)));
} $unscoped(TEST_fn);

TEST_fn_("fs/path: absolute subpath replaces base on resolve" $scope) {
    var_(buf, A$$(64, u8)) = A_zero();

    let resolved = try_(fs_path_resolve(u8_l("foo"), u8_l("/bar"), A_ref$((S$u8)(buf))));
    let expected = pp_if_(plat_is_windows)(
        pp_then_(u8_l("\\bar")),
        pp_else_(u8_l("/bar")));
    try_(TEST_expect(mem_eqlBytes(resolved.as_const, expected)));
} $unscoped(TEST_fn);
