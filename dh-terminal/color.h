/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    color.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
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


#include "dh/core.h"


enum EColorChannel {
    ColorChannel_RED         = 0,
    ColorChannel_GREEN       = 1,
    ColorChannel_BLUE        = 2,
    ColorChannel_ALPHA       = 3,
    ColorChannel_COUNT       = 4,
    ColorChannel_MIN         = 0,
    ColorChannel_MAX         = 0xFF,
    ColorChannel_TRANSPARENT = ColorChannel_MIN,
    ColorChannel_OPAQUE      = ColorChannel_MAX,
};


typedef union Rgb   Rgb;
typedef union Hsl   Hsl;
typedef union Color Color;


union Rgb {
    u8 rgb[3]; // Rgb channel components in [0,255]
    struct {
        u8 r; // Red channel in [0,255]
        u8 g; // Green channel in [0,255]
        u8 b; // Blue channel in [0,255]
    };
};
#define Rgb_(...) make(Rgb, __VA_ARGS__)
Rgb Rgb_from(u8 r, u8 g, u8 b);

Rgb Rgb_fromHsl(Hsl hsl);
Rgb Hsl_intoRgb(Hsl hsl);
Rgb Rgb_fromColor(Color color);
Rgb Color_intoRgb(Color color);
Rgb Hsl_asRgb(Hsl hsl);
Rgb Color_asRgb(Color color);

#define literal_Rgb_from(_r, _g, _b) \
    Rgb_(.r = (_r), .g = (_g), .b = (_b))


union Hsl {
    f64 hsl[3]; // Hsl color components, Hue in [0,360], Saturation in [0,100], Lightness in [0,100]
    struct {
        f64 h; // Hue in [0,360]
        f64 s; // Saturation in [0,100]
        f64 l; // Lightness in [0,100]
    };
};
#define Hsl_(...) make(Hsl, __VA_ARGS__)
Hsl Hsl_from(f64 h, f64 s, f64 l);
f64 Hsl_hueToRgbSpace(f64 p, f64 q, f64 t);

Hsl Hsl_fromRgb(Rgb rgb);
Hsl Rgb_intoHsl(Rgb rgb);
Hsl Hsl_fromColor(Color color);
Hsl Color_intoHsl(Color color);
Hsl Rgb_asHsl(Rgb rgb);
Hsl Color_asHsl(Color color);

#define literal_Hsl_from(_h, _s, _l) \
    Hsl_(.h = (_h), .s = (_s), .l = (_l))


union Color {
    u8 rgba[4]; // RgbA channel components in [0,255]
    union {
        Rgb rgb; // Rgb channel components in [0,255]
        struct {
            u8 r; // Red channel in [0,255]
            u8 g; // Green channel in [0,255]
            u8 b; // Blue channel in [0,255]
        };
        u8 a; // Alpha channel in [0,255]
    };
};
#define Color_(...) make(Color, __VA_ARGS__)
Color Color_from(u8 r, u8 g, u8 b, u8 a);
Color Color_fromTransparent(u8 r, u8 g, u8 b);
Color Color_fromOpaque(u8 r, u8 g, u8 b);

Color Color_fromRgb(Rgb rgb, u8 a);
Color Color_fromRgbTransparent(Rgb rgb);
Color Color_fromRgbOpaque(Rgb rgb);
Color Rgb_intoColor(Rgb rgb, u8 a);
Color Rgb_intoColorTransparent(Rgb rgb);
Color Rgb_intoColorOpaque(Rgb rgb);
Color Color_fromHsl(Hsl hsl, u8 a);
Color Color_fromHslTransparent(Hsl hsl);
Color Color_fromHslOpaque(Hsl hsl);
Color Hsl_intoColor(Hsl hsl, u8 a);
Color Hsl_intoColorTransparent(Hsl hsl);
Color Hsl_intoColorOpaque(Hsl hsl);

#define literal_Color_from(_r, _g, _b, _a) \
    Color_(.rgba = { _r, _g, _b, _a })
#define literal_Color_fromTransparent(_r, _g, _b) \
    Color_(.rgba = { _r, _g, _b, ColorChannel_MIN })
#define literal_Color_fromOpaque(_r, _g, _b) \
    Color_(.rgba = { _r, _g, _b, ColorChannel_MAX })

static const Color Color_TRANSPARENT = literal_Color_from(0, 0, 0, 0);
static const Color Color_BLACK       = literal_Color_fromOpaque(0, 0, 0);
static const Color Color_white       = literal_Color_fromOpaque(255, 255, 255);
static const Color Color_RED         = literal_Color_fromOpaque(255, 0, 0);
static const Color Color_GREEN       = literal_Color_fromOpaque(0, 255, 0);
static const Color Color_BLUE        = literal_Color_fromOpaque(0, 0, 255);
static const Color Color_YELLOW      = literal_Color_fromOpaque(255, 255, 0);
static const Color Color_CYAN        = literal_Color_fromOpaque(0, 255, 255);
static const Color Color_MAGENTA     = literal_Color_fromOpaque(255, 0, 255);
static const Color Color_BLANK       = Color_TRANSPARENT;


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* COLOR_INCLUDED */
