#include "color.h"
#include "floats.h"


RGB RGB_from(u8 r, u8 g, u8 b) {
    return RGB_(
            .r = r,
            .g = g,
            .b = b,
    );
}

HSL HSL_from(f64 h, f64 s, f64 l) {
    return HSL_(
            .h = h,
            .s = s,
            .l = l,
    );
}

f64 HSL_hueToRGBSpace(f64 p, f64 q, f64 t) {
    if (t < 0.0) {
        t += 1;
    }
    if (1.0 < t) {
        t -= 1.0;
    }
    if (t < 1.0 / 6.0) {
        return p + (q - p) * 6.0 * t;
    }
    if (t < 1.0 / 2.0) {
        return q;
    }
    if (t < 2.0 / 3.0) {
        return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
    }
    return p;
}

Color Color_from(u8 r, u8 g, u8 b, u8 a) {
    return comptime_Color_from(r, g, b, a);
}

Color Color_fromOpaque(u8 r, u8 g, u8 b) {
    return comptime_Color_fromOpaque(r, g, b);
}

Color Color_fromTransparent(u8 r, u8 g, u8 b) {
    return comptime_Color_fromTransparent(r, g, b);
}

RGB RGB_fromHSL(HSL hsl) {
    f64 h = hsl.h / 360.0;
    f64 s = hsl.s / 100.0;
    f64 l = hsl.l / 100.0;

    f64 r = f64_nan;
    f64 g = f64_nan;
    f64 b = f64_nan;

    if (s == 0.0) {
        r = g = b = l;
    } else {
        f64 q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        f64 p = 2.0 * l - q;
        r     = HSL_hueToRGBSpace(p, q, h + 1.0 / 3.0);
        g     = HSL_hueToRGBSpace(p, q, h);
        b     = HSL_hueToRGBSpace(p, q, h - 1.0 / 3.0);
    }

    return RGB_from(
        as(u8, f64_clamp(r * 255.0, 0, 255.0)),
        as(u8, f64_clamp(g * 255.0, 0, 255.0)),
        as(u8, f64_clamp(b * 255.0, 0, 255.0))
    );
}

RGB HSL_intoRGB(HSL hsl) {
    return RGB_fromHSL(hsl);
}

RGB RGB_fromColor(Color color) {
    return RGB_from(color.r, color.g, color.b);
}

RGB Color_intoRGB(Color color) {
    return RGB_fromColor(color);
}

HSL HSL_fromRGB(RGB rgb) {
    f64 r = rgb.r / 255.0;
    f64 g = rgb.g / 255.0;
    f64 b = rgb.b / 255.0;

    f64 max  = f64_max(f64_max(r, g), b);
    f64 min  = f64_min(f64_min(r, g), b);
    f64 diff = max - min;

    f64 h = f64_nan;
    f64 s = f64_nan;
    f64 l = (max + min) / 2.0;

    if (diff != 0.0) {
        s = 0.5 < l ? diff / (2.0 - max - min) : diff / (max + min);

        if (max == r) {
            h = (g - b) / diff + g < b ? 6.0 : 0.0;
        } else if (max == g) {
            h = (b - r) / diff + 2.0;
        } else {
            h = (r - g) / diff + 4.0;
        }
        h /= 6.0;
    }
    return HSL_from(
        h * 360.0,
        s * 100.0,
        l * 100.0
    );
}

HSL RGB_intoHSL(RGB rgb) {
    return HSL_fromRGB(rgb);
}

HSL HSL_fromColor(Color color) {
    return RGB_intoHSL(Color_intoRGB(color));
}

HSL Color_intoHSL(Color color) {
    return HSL_fromColor(color);
}

Color Color_fromRGBWithAlpha(RGB rgb, u8 a) {
    return Color_from(rgb.r, rgb.g, rgb.b, a);
}

Color Color_fromRGBOpaque(RGB rgb) {
    return Color_fromRGBWithAlpha(rgb, 255);
}

Color Color_fromRGBTransparent(RGB rgb) {
    return Color_fromRGBWithAlpha(rgb, 0);
}

Color RGB_intoColorWithAlpha(RGB rgb, u8 a) {
    return Color_from(rgb.r, rgb.g, rgb.b, a);
}

Color RGB_intoColorOpaque(RGB rgb) {
    return Color_fromRGBWithAlpha(rgb, 255);
}

Color RGB_intoColorTransparent(RGB rgb) {
    return Color_fromRGBWithAlpha(rgb, 0);
}

Color Color_fromHSLWithAlpha(HSL hsl, u8 a) {
    return RGB_intoColorWithAlpha(HSL_intoRGB(hsl), a);
}

Color Color_fromHSLWithOpaque(HSL hsl) {
    return Color_fromHSLWithAlpha(hsl, 255);
}

Color Color_fromHSLWithTransparent(HSL hsl) {
    return Color_fromHSLWithAlpha(hsl, 0);
}

Color HSL_intoColorWithAlpha(HSL hsl, u8 a) {
    return Color_fromHSLWithAlpha(hsl, a);
}

Color HSL_intoColorOpaque(HSL hsl) {
    return Color_fromHSLWithOpaque(hsl);
}

Color HSL_intoColorTransparent(HSL hsl) {
    return Color_fromHSLWithTransparent(hsl);
}
