/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    color.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-21 (date of last update)
 * @version v1.0.0
 * @ingroup SOME_GROUP
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef COLOR_INCLUDED
#define COLOR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "primitive_types.h"


typedef union RGB   RGB;
typedef union HSL   HSL;
typedef union Color Color;


union RGB {
    u8 rgb[3]; // RGB channel components in [0,255]
    struct {
        u8 r; // Red channel in [0,255]
        u8 g; // Green channel in [0,255]
        u8 b; // Blue channel in [0,255]
    };
};
#define RGB_(...) makeWith(RGB, __VA_ARGS__)
RGB RGB_from(u8 r, u8 g, u8 b);

RGB RGB_fromHSL(HSL hsl);
RGB HSL_intoRGB(HSL hsl);
RGB RGB_fromColor(Color color);
RGB Color_intoRGB(Color color);

#define comptime_RGB_from(_r, _g, _b) \
    RGB_(.r = (_r), .g = (_g), .b = (_b))


union HSL {
    f64 hsl[3]; // HSL color components, Hue in [0,360], Saturation in [0,100], Lightness in [0,100]
    struct {
        f64 h; // Hue in [0,360]
        f64 s; // Saturation in [0,100]
        f64 l; // Lightness in [0,100]
    };
};
#define HSL_(...) makeWith(HSL, __VA_ARGS__)
HSL HSL_from(f64 h, f64 s, f64 l);
f64 HSL_hueToRGBSpace(f64 p, f64 q, f64 t);

HSL HSL_fromRGB(RGB rgb);
HSL RGB_intoHSL(RGB rgb);
HSL HSL_fromColor(Color color);
HSL Color_intoHSL(Color color);

#define comptime_HSL_from(_h, _s, _l) \
    HSL_(.h = (_h), .s = (_s), .l = (_l))


union Color {
    u8 rgba[4]; // RGBA channel components in [0,255]
    union {
        RGB rgb; // RGB channel components in [0,255]
        struct {
            u8 r; // Red channel in [0,255]
            u8 g; // Green channel in [0,255]
            u8 b; // Blue channel in [0,255]
        };
        u8 a; // Alpha channel in [0,255]
    };
};
#define Color_(...) makeWith(Color, __VA_ARGS__)
Color Color_from(u8 r, u8 g, u8 b, u8 a);
Color Color_fromOpaque(u8 r, u8 g, u8 b);
Color Color_fromTransparent(u8 r, u8 g, u8 b);

Color Color_fromRGBWithAlpha(RGB rgb, u8 a);
Color Color_fromRGBOpaque(RGB rgb);
Color Color_fromRGBTransparent(RGB rgb);
Color RGB_intoColorWithAlpha(RGB rgb, u8 a);
Color RGB_intoColorOpaque(RGB rgb);
Color RGB_intoColorTransparent(RGB rgb);
Color Color_fromHSLWithAlpha(HSL hsl, u8 a);
Color Color_fromHSLWithOpaque(HSL hsl);
Color Color_fromHSLWithTransparent(HSL hsl);
Color HSL_intoColorWithAlpha(HSL hsl, u8 a);
Color HSL_intoColorOpaque(HSL hsl);
Color HSL_intoColorTransparent(HSL hsl);

#define comptime_Color_from(_r, _g, _b, _a) \
    Color_(.rgba = { _r, _g, _b, _a })
#define comptime_Color_fromOpaque(_r, _g, _b) \
    Color_(.rgba = { _r, _g, _b, 255 })
#define comptime_Color_fromTransparent(_r, _g, _b) \
    Color_(.rgba = { _r, _g, _b, 0 })

static const Color Color_transparent = comptime_Color_from(0, 0, 0, 0);
static const Color Color_black       = comptime_Color_fromOpaque(0, 0, 0);
static const Color Color_white       = comptime_Color_fromOpaque(255, 255, 255);
static const Color Color_red         = comptime_Color_fromOpaque(255, 0, 0);
static const Color Color_green       = comptime_Color_fromOpaque(0, 255, 0);
static const Color Color_blue        = comptime_Color_fromOpaque(0, 0, 255);
static const Color Color_yellow      = comptime_Color_fromOpaque(255, 255, 0);
static const Color Color_cyan        = comptime_Color_fromOpaque(0, 255, 255);
static const Color Color_magenta     = comptime_Color_fromOpaque(255, 0, 255);


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* COLOR_INCLUDED */
