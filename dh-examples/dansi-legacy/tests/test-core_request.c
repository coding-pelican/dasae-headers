#include <dh-main.h>
#include <dansi-core.h>
#include <dh/mem/common.h>

TEST_fn_("dansi-core/cursor: cursor position request has static and runtime forms" $scope) {
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_cursor_requestPos_static()), u8_l("\x1b[6n"))));
    try_(TEST_expect(mem_eqlBytes(dansi_cursor_requestPos(), u8_l("\x1b[6n"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/screen: screen size requests have static and runtime forms" $scope) {
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_screen_requestCellSizePixels_static()), u8_l("\x1b[16t"))));
    try_(TEST_expect(mem_eqlBytes(dansi_screen_requestCellSizePixels(), u8_l("\x1b[16t"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_screen_requestTextAreaSizeChars_static()), u8_l("\x1b[18t"))));
    try_(TEST_expect(mem_eqlBytes(dansi_screen_requestTextAreaSizeChars(), u8_l("\x1b[18t"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_screen_requestTextAreaSizePixels_static()), u8_l("\x1b[14t"))));
    try_(TEST_expect(mem_eqlBytes(dansi_screen_requestTextAreaSizePixels(), u8_l("\x1b[14t"))));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("dansi-core/device: status request has static and runtime forms" $scope) {
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_device_requestStatus_static()), u8_l("\x1b[5n"))));
    try_(TEST_expect(mem_eqlBytes(dansi_device_requestStatus(), u8_l("\x1b[5n"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_device_requestPriAttrs_static()), u8_l("\x1b[c"))));
    try_(TEST_expect(mem_eqlBytes(dansi_device_requestPriAttrs(), u8_l("\x1b[c"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_device_requestSecAttrs_static()), u8_l("\x1b[>c"))));
    try_(TEST_expect(mem_eqlBytes(dansi_device_requestSecAttrs(), u8_l("\x1b[>c"))));
    try_(TEST_expect(mem_eqlBytes(u8_l(dansi_device_requestTerAttrs_static()), u8_l("\x1b[=c"))));
    try_(TEST_expect(mem_eqlBytes(dansi_device_requestTerAttrs(), u8_l("\x1b[=c"))));
    return_ok({});
} $unscoped(TEST_fn);
