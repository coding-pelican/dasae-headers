/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim. All rights reserved.
 *
 * @file    color.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2025-12-28 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers/examples(dh-examples)
 * @prefix  color
 */
#ifndef color__included
#define color__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

/* --- Types --- */
typedef union color_RGBA color_RGBA;
typedef union color_RGB color_RGB;
typedef union color_HSL color_HSL;

/* --- RGBA --- */
enum_(color_RGBA_channels $bits(16)) {
    color_RGBA_channels_red = 0,
    color_RGBA_channels_green = 1,
    color_RGBA_channels_blue = 2,
    color_RGBA_channels_alpha = 3,
    color_RGBA_channels_count = 4,
    color_RGBA_channels_min_value = u8_limit_min,
    color_RGBA_channels_max_value = u8_limit_max,
    color_RGBA_channels_alpha_blank = color_RGBA_channels_min_value,
    color_RGBA_channels_alpha_opaque = color_RGBA_channels_max_value
};

#define color_RGBA_blank __const__color_RGBA_blank
#define color_RGBA_opaque __const__color_RGBA_opaque
#define color_RGBA_black __const__color_RGBA_black
#define color_RGBA_white __const__color_RGBA_white
#define color_RGBA_red __const__color_RGBA_red
#define color_RGBA_green __const__color_RGBA_green
#define color_RGBA_blue __const__color_RGBA_blue
#define color_RGBA_yellow __const__color_RGBA_yellow
#define color_RGBA_cyan __const__color_RGBA_cyan
#define color_RGBA_magenta __const__color_RGBA_magenta

union color_RGBA {
    var_(channels, A$$(color_RGBA_channels_count, u8)); // RGBA channel components in [0,255]
    struct {
        var_(r, u8); // Red channel in [0,255]
        var_(g, u8); // Green channel in [0,255]
        var_(b, u8); // Blue channel in [0,255]
        var_(a, u8); // Alpha channel in [0,255]
    };
    var_(packed, u32); // Packed RGBA value
};
T_use_prl$(color_RGBA);
#define color_RGBA_from_static(_r /*: u8*/, _g /*: u8*/, _b /*: u8*/, _a /*: u8*/... /*(color_RGBA)*/) \
    ____color_RGBA_from_static(_r, _g, _b, _a)
#define color_RGBA_fromBlank_static(_r /*: u8*/, _g /*: u8*/, _b /*: u8*/... /*(color_RGBA)*/) \
    ____color_RGBA_fromBlank_static(_r, _g, _b)
#define color_RGBA_fromOpaque_static(_r /*: u8*/, _g /*: u8*/, _b /*: u8*/... /*(color_RGBA)*/) \
    ____color_RGBA_fromOpaque_static(_r, _g, _b)

$extern fn_((color_RGBA_from(u8 r, u8 g, u8 b, u8 a))(color_RGBA));
$extern fn_((color_RGBA_fromBlank(u8 r, u8 g, u8 b))(color_RGBA));
$extern fn_((color_RGBA_fromOpaque(u8 r, u8 g, u8 b))(color_RGBA));

$extern fn_((color_RGBA_fromRGB(color_RGB rgb, u8 a))(color_RGBA));
$extern fn_((color_RGBA_fromBlankRGB(color_RGB rgb))(color_RGBA));
$extern fn_((color_RGBA_fromOpaqueRGB(color_RGB rgb))(color_RGBA));
$extern fn_((color_RGBA_toRGB(color_RGBA self))(color_RGB));

$extern fn_((color_RGBA_fromHSL(color_HSL hsl, u8 a))(color_RGBA));
$extern fn_((color_RGBA_fromBlankHSL(color_HSL hsl))(color_RGBA));
$extern fn_((color_RGBA_fromOpaqueHSL(color_HSL hsl))(color_RGBA));
$extern fn_((color_RGBA_toHSL(color_RGBA self))(color_HSL));

/* --- RGB --- */
#define color_RGB_channels_count __comp_int__color_RGB_channels_count
#define __comp_int__color_RGB_channels_count 3

union color_RGB {
    var_(channels, A$$(color_RGB_channels_count, u8)); // RGB channel components in [0,255]
    struct {
        var_(r, u8); // Red channel in [0,255]
        var_(g, u8); // Green channel in [0,255]
        var_(b, u8); // Blue channel in [0,255]
    };
};
T_use_prl$(color_RGB);
#define color_RGB_from_static(_r /*: u8*/, _g /*: u8*/, _b /*: u8*/... /*(color_RGB)*/) \
    ____color_RGB_from_static(_r, _g, _b)

$extern fn_((color_RGB_from(u8 r, u8 g, u8 b))(color_RGB));
$extern fn_((color_RGB_toRGBA(color_RGB self, u8 a))(color_RGBA));
$extern fn_((color_RGB_toBlankRGBA(color_RGB self))(color_RGBA));
$extern fn_((color_RGB_toOpaqueRGBA(color_RGB self))(color_RGBA));

$extern fn_((color_RGB_fromHSL(color_HSL hsl))(color_RGB));
$extern fn_((color_RGB_toHSL(color_RGB self))(color_HSL));

/* --- HSL --- */
#define color_HSL_channels_count __comp_int__color_HSL_channels_count
#define __comp_int__color_HSL_channels_count 3

union color_HSL {
    var_(channels, A$$(color_HSL_channels_count, f64)); // HSL color components, Hue in [0,360], Saturation in [0,100], Lightness in [0,100]
    struct {
        var_(h, f64); // Hue in [0,360]
        var_(s, f64); // Saturation in [0,100]
        var_(l, f64); // Lightness in [0,100]
    };
};
T_use_prl$(color_HSL);
#define color_HSL_from_static(_h /*: f64*/, _s /*: f64*/, _l /*: f64*/... /*(color_HSL)*/) \
    ____color_HSL_from_static(_h, _s, _l)

$extern fn_((color_HSL_from(f64 h, f64 s, f64 l))(color_HSL));
$extern fn_((color_HSL_toRGBA(color_HSL self, u8 a))(color_RGBA));
$extern fn_((color_HSL_toBlankRGBA(color_HSL self))(color_RGBA));
$extern fn_((color_HSL_toOpaqueRGBA(color_HSL self))(color_RGBA));

$extern fn_((color_HSL_fromRGB(color_RGB rgb))(color_HSL));
$extern fn_((color_HSL_toRGB(color_HSL self))(color_RGB));

/*========== Macros and Definitions =========================================*/

/* --- RGBA --- */
#define __const__color_RGBA_blank \
    color_RGBA_from_static(0, 0, 0, color_RGBA_channels_alpha_blank)
#define __const__color_RGBA_opaque \
    color_RGBA_from_static(0, 0, 0, color_RGBA_channels_alpha_opaque)
#define __const__color_RGBA_black \
    color_RGBA_fromOpaque_static(0, 0, 0)
#define __const__color_RGBA_white \
    color_RGBA_fromOpaque_static(255, 255, 255)
#define __const__color_RGBA_red \
    color_RGBA_fromOpaque_static(255, 0, 0)
#define __const__color_RGBA_green \
    color_RGBA_fromOpaque_static(0, 255, 0)
#define __const__color_RGBA_blue \
    color_RGBA_fromOpaque_static(0, 0, 255)
#define __const__color_RGBA_yellow \
    color_RGBA_fromOpaque_static(255, 255, 0)
#define __const__color_RGBA_cyan \
    color_RGBA_fromOpaque_static(0, 255, 255)
#define __const__color_RGBA_magenta \
    color_RGBA_fromOpaque_static(255, 0, 255)

#define ____color_RGBA_from_static(_r, _g, _b, _a...) lit$((color_RGBA){ \
    .r = (_r), \
    .g = (_g), \
    .b = (_b), \
    .a = (_a), \
})
#define ____color_RGBA_fromBlank_static(_r, _g, _b...) lit$((color_RGBA){ \
    .r = (_r), \
    .g = (_g), \
    .b = (_b), \
    .a = color_RGBA_channels_alpha_blank, \
})
#define ____color_RGBA_fromOpaque_static(_r, _g, _b...) lit$((color_RGBA){ \
    .r = (_r), \
    .g = (_g), \
    .b = (_b), \
    .a = color_RGBA_channels_alpha_opaque, \
})

/* --- RGB --- */
#define ____color_RGB_from_static(_r, _g, _b...) lit$((color_RGB){ \
    .r = (_r), \
    .g = (_g), \
    .b = (_b), \
})

/* --- HSL --- */
#define ____color_HSL_from_static(_h, _s, _l...) lit$((color_HSL){ \
    .h = (_h), \
    .s = (_s), \
    .l = (_l), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* color__included */
