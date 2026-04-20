/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    dacolor.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2026-03-25 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers-examples(dh-examples)
 * @prefix  dacolor
 */
#ifndef dacolor__included
#define dacolor__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/prl.h>

/*========== Macros and Declarations ========================================*/

/* --- Types --- */
T_alias$((dacolor_RGBA)(union dacolor_RGBA));
T_alias$((dacolor_RGB)(union dacolor_RGB));
T_alias$((dacolor_HSL)(union dacolor_HSL));

/* --- RGBA --- */
enum_((dacolor_RGBA_Channels $fits($packed))(
    dacolor_RGBA_channels_red = 0,
    dacolor_RGBA_channels_green = 1,
    dacolor_RGBA_channels_blue = 2,
    dacolor_RGBA_channels_alpha = 3,
    dacolor_RGBA_channels_count = 4,
    dacolor_RGBA_channels_min_value = u8_limit_min,
    dacolor_RGBA_channels_max_value = u8_limit_max,
    dacolor_RGBA_channels_alpha_blank = dacolor_RGBA_channels_min_value,
    dacolor_RGBA_channels_alpha_opaque = dacolor_RGBA_channels_max_value
));

#define dacolor_RGBA_blank __const__dacolor_RGBA_blank
#define dacolor_RGBA_opaque __const__dacolor_RGBA_opaque
#define dacolor_RGBA_black __const__dacolor_RGBA_black
#define dacolor_RGBA_white __const__dacolor_RGBA_white
#define dacolor_RGBA_red __const__dacolor_RGBA_red
#define dacolor_RGBA_green __const__dacolor_RGBA_green
#define dacolor_RGBA_blue __const__dacolor_RGBA_blue
#define dacolor_RGBA_yellow __const__dacolor_RGBA_yellow
#define dacolor_RGBA_cyan __const__dacolor_RGBA_cyan
#define dacolor_RGBA_magenta __const__dacolor_RGBA_magenta

union dacolor_RGBA {
    var_(channels, A$$(dacolor_RGBA_channels_count, u8)); // RGBA channel components in [0,255]
    T_embed$(struct {
        var_(r, u8); // Red channel in [0,255]
        var_(g, u8); // Green channel in [0,255]
        var_(b, u8); // Blue channel in [0,255]
        var_(a, u8); // Alpha channel in [0,255]
    });
    var_(packed, u32); // Packed RGBA value
};
T_use_prl$(dacolor_RGBA);
#define dacolor_RGBA_from_static(_r /*: u8*/, _g /*: u8*/, _b /*: u8*/, _a /*: u8*/... /*(dacolor_RGBA)*/) \
    ____dacolor_RGBA_from_static(_r, _g, _b, _a)
#define dacolor_RGBA_fromBlank_static(_r /*: u8*/, _g /*: u8*/, _b /*: u8*/... /*(dacolor_RGBA)*/) \
    ____dacolor_RGBA_fromBlank_static(_r, _g, _b)
#define dacolor_RGBA_fromOpaque_static(_r /*: u8*/, _g /*: u8*/, _b /*: u8*/... /*(dacolor_RGBA)*/) \
    ____dacolor_RGBA_fromOpaque_static(_r, _g, _b)

$extern fn_((dacolor_RGBA_from(u8 r, u8 g, u8 b, u8 a))(dacolor_RGBA));
$extern fn_((dacolor_RGBA_fromBlank(u8 r, u8 g, u8 b))(dacolor_RGBA));
$extern fn_((dacolor_RGBA_fromOpaque(u8 r, u8 g, u8 b))(dacolor_RGBA));

$extern fn_((dacolor_RGBA_fromRGB(dacolor_RGB rgb, u8 a))(dacolor_RGBA));
$extern fn_((dacolor_RGBA_fromBlankRGB(dacolor_RGB rgb))(dacolor_RGBA));
$extern fn_((dacolor_RGBA_fromOpaqueRGB(dacolor_RGB rgb))(dacolor_RGBA));
$extern fn_((dacolor_RGBA_toRGB(dacolor_RGBA self))(dacolor_RGB));

$extern fn_((dacolor_RGBA_fromHSL(dacolor_HSL hsl, u8 a))(dacolor_RGBA));
$extern fn_((dacolor_RGBA_fromBlankHSL(dacolor_HSL hsl))(dacolor_RGBA));
$extern fn_((dacolor_RGBA_fromOpaqueHSL(dacolor_HSL hsl))(dacolor_RGBA));
$extern fn_((dacolor_RGBA_toHSL(dacolor_RGBA self))(dacolor_HSL));

/* --- RGB --- */
#define dacolor_RGB_channels_count __comp_int__dacolor_RGB_channels_count
#define __comp_int__dacolor_RGB_channels_count 3

union dacolor_RGB {
    var_(channels, A$$(dacolor_RGB_channels_count, u8)); // RGB channel components in [0,255]
    T_embed$(struct {
        var_(r, u8); // Red channel in [0,255]
        var_(g, u8); // Green channel in [0,255]
        var_(b, u8); // Blue channel in [0,255]
    });
};
T_use_prl$(dacolor_RGB);
#define dacolor_RGB_from_static(_r /*: u8*/, _g /*: u8*/, _b /*: u8*/... /*(dacolor_RGB)*/) \
    ____dacolor_RGB_from_static(_r, _g, _b)

$extern fn_((dacolor_RGB_from(u8 r, u8 g, u8 b))(dacolor_RGB));
$extern fn_((dacolor_RGB_toRGBA(dacolor_RGB self, u8 a))(dacolor_RGBA));
$extern fn_((dacolor_RGB_toBlankRGBA(dacolor_RGB self))(dacolor_RGBA));
$extern fn_((dacolor_RGB_toOpaqueRGBA(dacolor_RGB self))(dacolor_RGBA));

$extern fn_((dacolor_RGB_fromHSL(dacolor_HSL hsl))(dacolor_RGB));
$extern fn_((dacolor_RGB_toHSL(dacolor_RGB self))(dacolor_HSL));

/* --- HSL --- */
#define dacolor_HSL_channels_count __comp_int__dacolor_HSL_channels_count
#define __comp_int__dacolor_HSL_channels_count 3

union dacolor_HSL {
    var_(channels, A$$(dacolor_HSL_channels_count, f64)); // HSL color components, Hue in [0,360], Saturation in [0,100], Lightness in [0,100]
    T_embed$(struct {
        var_(h, f64); // Hue in [0,360]
        var_(s, f64); // Saturation in [0,100]
        var_(l, f64); // Lightness in [0,100]
    });
};
T_use_prl$(dacolor_HSL);
#define dacolor_HSL_from_static(_h /*: f64*/, _s /*: f64*/, _l /*: f64*/... /*(dacolor_HSL)*/) \
    ____dacolor_HSL_from_static(_h, _s, _l)

$extern fn_((dacolor_HSL_from(f64 h, f64 s, f64 l))(dacolor_HSL));
$extern fn_((dacolor_HSL_toRGBA(dacolor_HSL self, u8 a))(dacolor_RGBA));
$extern fn_((dacolor_HSL_toBlankRGBA(dacolor_HSL self))(dacolor_RGBA));
$extern fn_((dacolor_HSL_toOpaqueRGBA(dacolor_HSL self))(dacolor_RGBA));

$extern fn_((dacolor_HSL_fromRGB(dacolor_RGB rgb))(dacolor_HSL));
$extern fn_((dacolor_HSL_toRGB(dacolor_HSL self))(dacolor_RGB));

/*========== Macros and Definitions =========================================*/

/* --- RGBA --- */
#define __const__dacolor_RGBA_blank \
    dacolor_RGBA_from_static(0, 0, 0, dacolor_RGBA_channels_alpha_blank)
#define __const__dacolor_RGBA_opaque \
    dacolor_RGBA_from_static(0, 0, 0, dacolor_RGBA_channels_alpha_opaque)
#define __const__dacolor_RGBA_black \
    dacolor_RGBA_fromOpaque_static(0, 0, 0)
#define __const__dacolor_RGBA_white \
    dacolor_RGBA_fromOpaque_static(255, 255, 255)
#define __const__dacolor_RGBA_red \
    dacolor_RGBA_fromOpaque_static(255, 0, 0)
#define __const__dacolor_RGBA_green \
    dacolor_RGBA_fromOpaque_static(0, 255, 0)
#define __const__dacolor_RGBA_blue \
    dacolor_RGBA_fromOpaque_static(0, 0, 255)
#define __const__dacolor_RGBA_yellow \
    dacolor_RGBA_fromOpaque_static(255, 255, 0)
#define __const__dacolor_RGBA_cyan \
    dacolor_RGBA_fromOpaque_static(0, 255, 255)
#define __const__dacolor_RGBA_magenta \
    dacolor_RGBA_fromOpaque_static(255, 0, 255)

#define ____dacolor_RGBA_from_static(_r, _g, _b, _a...) l$((dacolor_RGBA){ \
    .r = (_r), \
    .g = (_g), \
    .b = (_b), \
    .a = (_a), \
})
#define ____dacolor_RGBA_fromBlank_static(_r, _g, _b...) l$((dacolor_RGBA){ \
    .r = (_r), \
    .g = (_g), \
    .b = (_b), \
    .a = dacolor_RGBA_channels_alpha_blank, \
})
#define ____dacolor_RGBA_fromOpaque_static(_r, _g, _b...) l$((dacolor_RGBA){ \
    .r = (_r), \
    .g = (_g), \
    .b = (_b), \
    .a = dacolor_RGBA_channels_alpha_opaque, \
})

/* --- RGB --- */
#define ____dacolor_RGB_from_static(_r, _g, _b...) l$((dacolor_RGB){ \
    .r = (_r), \
    .g = (_g), \
    .b = (_b), \
})

/* --- HSL --- */
#define ____dacolor_HSL_from_static(_h, _s, _l...) l$((dacolor_HSL){ \
    .h = (_h), \
    .s = (_s), \
    .l = (_l), \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dacolor__included */
