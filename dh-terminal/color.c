#include "color.h"
#include <dh/core.h>


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

Color Color_fromTransparent(u8 r, u8 g, u8 b) {
    return comptime_Color_fromTransparent(r, g, b);
}

Color Color_fromOpaque(u8 r, u8 g, u8 b) {
    return comptime_Color_fromOpaque(r, g, b);
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
        prim_as(u8, f64_clamp(r * CCH_max, 0, CCH_max)),
        prim_as(u8, f64_clamp(g * CCH_max, 0, CCH_max)),
        prim_as(u8, f64_clamp(b * CCH_max, 0, CCH_max))
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

RGB HSL_asRGB(HSL hsl) {
    return RGB_fromHSL(hsl);
}

RGB Color_asRGB(Color color) {
    return RGB_fromColor(color);
}

HSL HSL_fromRGB(RGB rgb) {
    f64 r = rgb.r / prim_as(f64, CCH_max);
    f64 g = rgb.g / prim_as(f64, CCH_max);
    f64 b = rgb.b / prim_as(f64, CCH_max);

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

HSL RGB_asHSL(RGB rgb) {
    return HSL_fromRGB(rgb);
}

HSL Color_asHSL(Color color) {
    return HSL_fromColor(color);
}

Color Color_fromRGB(RGB rgb, u8 a) {
    return Color_from(rgb.r, rgb.g, rgb.b, a);
}

Color Color_fromRgbTransparent(RGB rgb) {
    return Color_fromRGB(rgb, CCH_transparent);
}

Color Color_fromRgbOpaque(RGB rgb) {
    return Color_fromRGB(rgb, CCH_opaque);
}

Color RGB_intoColor(RGB rgb, u8 a) {
    return Color_from(rgb.r, rgb.g, rgb.b, a);
}

Color RGB_intoColorTransparent(RGB rgb) {
    return Color_fromRGB(rgb, CCH_transparent);
}

Color RGB_intoColorOpaque(RGB rgb) {
    return Color_fromRGB(rgb, CCH_opaque);
}

Color Color_fromHSL(HSL hsl, u8 a) {
    return RGB_intoColor(HSL_intoRGB(hsl), a);
}

Color Color_fromHslTransparent(HSL hsl) {
    return Color_fromHSL(hsl, CCH_transparent);
}

Color Color_fromHslOpaque(HSL hsl) {
    return Color_fromHSL(hsl, CCH_opaque);
}

Color HSL_intoColor(HSL hsl, u8 a) {
    return Color_fromHSL(hsl, a);
}

Color HSL_intoColorTransparent(HSL hsl) {
    return Color_fromHslTransparent(hsl);
}

Color HSL_intoColorOpaque(HSL hsl) {
    return Color_fromHslOpaque(hsl);
}
