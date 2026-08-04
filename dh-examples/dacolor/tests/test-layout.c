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

TEST_fn_("dacolor: alpha constructors preserve RGB channels" $scope) {
    let rgb = dacolor_RGB_from(0x12, 0x34, 0x56);
    let blank = dacolor_RGB_toBlankRGBA(rgb);
    let opaque = dacolor_RGB_toOpaqueRGBA(rgb);
    let custom = dacolor_RGB_toRGBA(rgb, 0x78);

    try_(TEST_expect(blank.r == rgb.r && blank.g == rgb.g && blank.b == rgb.b));
    try_(TEST_expect(blank.a == dacolor_RGBA_chans_alpha_blank));
    try_(TEST_expect(opaque.r == rgb.r && opaque.g == rgb.g && opaque.b == rgb.b));
    try_(TEST_expect(opaque.a == dacolor_RGBA_chans_alpha_opaque));
    try_(TEST_expect(custom.r == rgb.r && custom.g == rgb.g && custom.b == rgb.b));
    try_(TEST_expect(custom.a == 0x78));
    try_(TEST_expect(dacolor_RGBA_toRGB(custom).r == rgb.r));
    try_(TEST_expect(dacolor_RGBA_toRGB(custom).g == rgb.g));
    try_(TEST_expect(dacolor_RGBA_toRGB(custom).b == rgb.b));
} $unscoped(TEST_fn);

TEST_fn_("dacolor: canonical HSL colors convert to RGB" $scope) {
    let red = dacolor_HSL_toRGB(dacolor_HSL_from(0.0, 100.0, 50.0));
    let green = dacolor_HSL_toRGB(dacolor_HSL_from(120.0, 100.0, 50.0));
    let blue = dacolor_HSL_toRGB(dacolor_HSL_from(240.0, 100.0, 50.0));
    let gray = dacolor_HSL_toRGB(dacolor_HSL_from(0.0, 0.0, 50.0));

    try_(TEST_expect(red.r == 255 && red.g == 0 && red.b == 0));
    try_(TEST_expect(green.r == 0 && green.g == 255 && green.b == 0));
    try_(TEST_expect(blue.r == 0 && blue.g == 0 && blue.b == 255));
    try_(TEST_expect(gray.r == gray.g && gray.g == gray.b));
} $unscoped(TEST_fn);

TEST_fn_("dacolor: RGB primaries map to canonical HSL" $scope) {
    let red = dacolor_RGB_toHSL(dacolor_RGB_from(255, 0, 0));
    let green = dacolor_RGB_toHSL(dacolor_RGB_from(0, 255, 0));
    let blue = dacolor_RGB_toHSL(dacolor_RGB_from(0, 0, 255));

    try_(TEST_expect(red.h == 0.0 && red.s == 100.0 && red.l == 50.0));
    try_(TEST_expect(green.h == 120.0 && green.s == 100.0 && green.l == 50.0));
    try_(TEST_expect(blue.h == 240.0 && blue.s == 100.0 && blue.l == 50.0));
} $unscoped(TEST_fn);
