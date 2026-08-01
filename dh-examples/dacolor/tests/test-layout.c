#include "dh-main.h"
#include "dacolor.h"
#include "dh/mem/common.h"

TEST_fn_("dacolor: channel members preserve canonical packed layout" $scope) {
    let color = dacolor_RGBA_from_static(0x12, 0x34, 0x56, 0x78);

    try_(TEST_expect(mem_nativeToBig32(color.packed) == u32_(0x12345678u)));
    try_(TEST_expect(*A_at((color.chans)[dacolor_RGBA_chans_red]) == color.r));
    try_(TEST_expect(*A_at((color.chans)[dacolor_RGBA_chans_green]) == color.g));
    try_(TEST_expect(*A_at((color.chans)[dacolor_RGBA_chans_blue]) == color.b));
    try_(TEST_expect(*A_at((color.chans)[dacolor_RGBA_chans_alpha]) == color.a));
} $unscoped(TEST_fn);

TEST_fn_("dacolor: RGB and HSL arrays match named members" $scope) {
    let rgb = dacolor_RGB_from_static(0x12, 0x34, 0x56);
    let hsl = dacolor_HSL_from_static(120.0, 50.0, 25.0);

    try_(TEST_expect(*A_at((rgb.chans)[0]) == rgb.r));
    try_(TEST_expect(*A_at((rgb.chans)[1]) == rgb.g));
    try_(TEST_expect(*A_at((rgb.chans)[2]) == rgb.b));
    try_(TEST_expect(*A_at((hsl.chans)[0]) == hsl.h));
    try_(TEST_expect(*A_at((hsl.chans)[1]) == hsl.s));
    try_(TEST_expect(*A_at((hsl.chans)[2]) == hsl.l));
} $unscoped(TEST_fn);
