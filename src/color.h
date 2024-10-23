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


typedef struct Color    Color;
typedef struct ColorHSL ColorHSL;


struct Color {
    union {
        u8 rgba[4]; // RGBA color components [0,255]
        struct {
            u8 r; // Red in [0,255]
            u8 g; // Green in [0,255]
            u8 b; // Blue in [0,255]
            u8 a; // Alpha in [0,255]
        };
    };
};
#define Color_(...)                     ((Color){ __VA_ARGS__ })
#define Color_from_RGBA(_r, _g, _b, _a) ((Color){ .rgba = { _r, _g, _b, _a } })
#define Color_from_RGB(_r, _g, _b)      Color_from_RGBA(_r, _g, _b, 255)
Color    Color_from_HSLA(ColorHSL color, u8 alpha);
Color    Color_from_HSL(ColorHSL color);
ColorHSL Color_to_HSL(Color color);

static const Color Color_transparent = Color_from_RGBA(0, 0, 0, 0);
static const Color Color_black       = Color_from_RGB(0, 0, 0);
static const Color Color_white       = Color_from_RGB(255, 255, 255);
static const Color Color_red         = Color_from_RGB(255, 0, 0);
static const Color Color_green       = Color_from_RGB(0, 255, 0);
static const Color Color_blue        = Color_from_RGB(0, 0, 255);
static const Color Color_yellow      = Color_from_RGB(255, 255, 0);
static const Color Color_cyan        = Color_from_RGB(0, 255, 255);
static const Color Color_magenta     = Color_from_RGB(255, 0, 255);


struct ColorHSL {
    union {
        f64 hsl[3]; // HSL color components, Hue in [0,360], Saturation in [0,100], Lightness in [0,100]
        struct {
            f64 h; // Hue in [0,360]
            f64 s; // Saturation in [0,100]
            f64 l; // Lightness in [0,100]
        };
    };
};
#define ColorHSL_(...)     ((ColorHSL){ __VA_ARGS__ })
#define ColorHSL_from(...) ((ColorHSL){ .hsl = { __VA_ARGS__ } })
ColorHSL ColorHSL_from_RGBA(Color color);
Color    ColorHSL_to_RGBA(ColorHSL color, u8 alpha);
Color    ColorHSL_to_RGB(ColorHSL color);
f64      ColorHSL_Hue_to_RGB(f64 p, f64 q, f64 t);


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* COLOR_INCLUDED */
