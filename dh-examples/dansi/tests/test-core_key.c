#include <dh-main.h>
#include <dansi-core.h>

TEST_fn_("dansi-core/key: enhanced key mode sequences" $scope) {
    var_(buf, dansi_key_OptionBuf) $undefined;
    let modify_cursor = dansi_key_setModifyResource(dansi_key_ModifyResource_cursor_keys, 2, &buf);
    try_(TEST_expect(mem_eqlBytes(
        u8_l(dansi_key_setModifyResource_static(dansi_key_ModifyResource_staticParse(dansi_key_ModifyResource_cursor_keys), "2")),
        u8_l("\x1b[>1;2m")
    )));
    try_(TEST_expect(mem_eqlBytes(modify_cursor.as_const, u8_l("\x1b[>1;2m"))));
    try_(TEST_expect(mem_eqlBytes(
        u8_l(dansi_key_disableModifyResource_static("2")),
        u8_l("\x1b[>2n")
    )));
    try_(TEST_expect(mem_eqlBytes(
        u8_l(dansi_key_queryModifyResource_static("4")),
        u8_l("\x1b[?4m")
    )));
    try_(TEST_expect(mem_eqlBytes(
        u8_l(dansi_key_queryFormatResource_static(dansi_key_FormatResource_staticParse(dansi_key_FormatResource_special_keys))),
        u8_l("\x1b[?7g")
    )));
    try_(TEST_expect(mem_eqlBytes(
        u8_l(dansi_key_enableModifyOtherKeys_static("2")),
        u8_l("\x1b[>4;2m")
    )));
    try_(TEST_expect(mem_eqlBytes(
        u8_l(dansi_key_enableCSIUFormat_static()),
        u8_l("\x1b[>4;1f")
    )));
    try_(TEST_expect(mem_eqlBytes(
        u8_l(dansi_key_disableModifyOtherKeys_static()),
        u8_l("\x1b[>4m")
    )));
    return_ok({});
} $unscoped(TEST_fn);
