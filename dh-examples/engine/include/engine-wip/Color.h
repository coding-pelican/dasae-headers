/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    color.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-12-30 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef ENGINE_COLOR_INCLUDED
#define ENGINE_COLOR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "common.h"

enum ColorChannel {
    ColorChannel_red               = 0,
    ColorChannel_green             = 1,
    ColorChannel_blue              = 2,
    ColorChannel_rgb_count         = 3,
    ColorChannel_alpha             = 3,
    ColorChannel_rgba_count        = 4,
    ColorChannel_min_value         = 0,
    ColorChannel_max_value         = 0xFF,
    ColorChannel_alpha_transparent = ColorChannel_min_value,
    ColorChannel_alpha_blank       = ColorChannel_alpha_transparent,
    ColorChannel_alpha_opaque      = ColorChannel_max_value
};

typedef union Rgb   Rgb;
typedef union Hsl   Hsl;
// typedef union Hsv   Hsv;
typedef union Color Color;

union Rgb {
    u8 channels[3]; // Rgb channel components in [0,255]
    struct {
        u8 r; // Red channel in [0,255]
        u8 g; // Green channel in [0,255]
        u8 b; // Blue channel in [0,255]
    };
};
#define Rgb_(...)                    make$(Rgb, __VA_ARGS__)
#define literal_Rgb_from(_r, _g, _b) Rgb_(.r = (_r), .g = (_g), .b = (_b))
extern Rgb Rgb_from(u8 r, u8 g, u8 b);

extern Rgb Rgb_fromHsl(Hsl hsl);
extern Rgb Hsl_intoRgb(Hsl hsl);
extern Rgb Rgb_fromColor(Color color);
extern Rgb Color_intoRgb(Color color);
extern Rgb Hsl_asRgb(Hsl hsl);
extern Rgb Color_asRgb(Color color);

union Hsl {
    f64 channels[3]; // Hsl color components, Hue in [0,360], Saturation in [0,100], Lightness in [0,100]
    struct {
        f64 h; // Hue in [0,360]
        f64 s; // Saturation in [0,100]
        f64 l; // Lightness in [0,100]
    };
};
#define Hsl_(...)                    make$(Hsl, __VA_ARGS__)
#define literal_Hsl_from(_h, _s, _l) Hsl_(.h = (_h), .s = (_s), .l = (_l))
extern Hsl Hsl_from(f64 h, f64 s, f64 l);
extern f64 Hsl_hueToRgbSpace(f64 p, f64 q, f64 t);

extern Hsl Hsl_fromRgb(Rgb rgb);
extern Hsl Rgb_intoHsl(Rgb rgb);
extern Hsl Hsl_fromColor(Color color);
extern Hsl Color_intoHsl(Color color);
extern Hsl Rgb_asHsl(Rgb rgb);
extern Hsl Color_asHsl(Color color);

union Color {
    u8 channels[4]; // RGBA channel components in [0,255]
    struct {
        union {
            Rgb rgb; // RGB channel components in [0,255]
            struct {
                u8 r; // Red channel in [0,255]
                u8 g; // Green channel in [0,255]
                u8 b; // Blue channel in [0,255]
            };
        };
        u8 a; // Alpha channel in [0,255]
    };
    u32 packed; // RGBA channel components in [0,255]
};
#define Color_(...)                               make$(Color, __VA_ARGS__)
#define literal_Color_from(_r, _g, _b, _a)        Color_(.r = (_r), .g = (_g), .b = (_b), .a = (_a))
#define literal_Color_fromTransparent(_r, _g, _b) Color_(.r = (_r), .g = (_g), .b = (_b), .a = ColorChannel_min_value)
#define literal_Color_fromOpaque(_r, _g, _b)      Color_(.r = (_r), .g = (_g), .b = (_b), .a = ColorChannel_max_value)
extern Color Color_from(u8 r, u8 g, u8 b, u8 a);
extern Color Color_fromTransparent(u8 r, u8 g, u8 b);
extern Color Color_fromOpaque(u8 r, u8 g, u8 b);

extern Color Color_fromRgb(Rgb rgb, u8 a);
extern Color Color_fromRgbTransparent(Rgb rgb);
extern Color Color_fromRgbOpaque(Rgb rgb);
extern Color Rgb_intoColor(Rgb rgb, u8 a);
extern Color Rgb_intoColorTransparent(Rgb rgb);
extern Color Rgb_intoColorOpaque(Rgb rgb);
extern Color Color_fromHsl(Hsl hsl, u8 a);
extern Color Color_fromHslTransparent(Hsl hsl);
extern Color Color_fromHslOpaque(Hsl hsl);
extern Color Hsl_intoColor(Hsl hsl, u8 a);
extern Color Hsl_intoColorTransparent(Hsl hsl);
extern Color Hsl_intoColorOpaque(Hsl hsl);

static const Color Color_transparent = literal_Color_from(0, 0, 0, ColorChannel_alpha_transparent);
static const Color Color_blank       = literal_Color_from(0, 0, 0, ColorChannel_alpha_blank);
static const Color Color_opaque      = literal_Color_from(0, 0, 0, ColorChannel_alpha_opaque);
static const Color Color_black       = literal_Color_fromOpaque(0, 0, 0);
static const Color Color_white       = literal_Color_fromOpaque(255, 255, 255);
static const Color Color_red         = literal_Color_fromOpaque(255, 0, 0);
static const Color Color_green       = literal_Color_fromOpaque(0, 255, 0);
static const Color Color_blue        = literal_Color_fromOpaque(0, 0, 255);
static const Color Color_yellow      = literal_Color_fromOpaque(255, 255, 0);
static const Color Color_cyan        = literal_Color_fromOpaque(0, 255, 255);
static const Color Color_magenta     = literal_Color_fromOpaque(255, 0, 255);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* ENGINE_COLOR_INCLUDED */
