#include "engine-wip/Color.h"

$inline_always f64 f64_clamp(f64 val, f64 low, f64 high) {
    return prim_min(prim_max(low, val), high);
}

Rgb Rgb_from(u8 r, u8 g, u8 b) {
    return Rgb_(
            .r = r,
            .g = g,
            .b = b,
    );
}

Hsl Hsl_from(f64 h, f64 s, f64 l) {
    return Hsl_(
            .h = h,
            .s = s,
            .l = l,
    );
}

f64 Hsl_hueToRgbSpace(f64 p, f64 q, f64 t) {
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
    return literal_Color_from(r, g, b, a);
}

Color Color_fromTransparent(u8 r, u8 g, u8 b) {
    return literal_Color_fromTransparent(r, g, b);
}

Color Color_fromOpaque(u8 r, u8 g, u8 b) {
    return literal_Color_fromOpaque(r, g, b);
}

Rgb Rgb_fromHsl(Hsl hsl) {
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
        r     = Hsl_hueToRgbSpace(p, q, h + 1.0 / 3.0);
        g     = Hsl_hueToRgbSpace(p, q, h);
        b     = Hsl_hueToRgbSpace(p, q, h - 1.0 / 3.0);
    }

    return Rgb_from(
        as$(u8, f64_clamp(r * ColorChannel_max_value, 0, ColorChannel_max_value)),
        as$(u8, f64_clamp(g * ColorChannel_max_value, 0, ColorChannel_max_value)),
        as$(u8, f64_clamp(b * ColorChannel_max_value, 0, ColorChannel_max_value))
    );
}

Rgb Hsl_intoRgb(Hsl hsl) {
    return Rgb_fromHsl(hsl);
}

Rgb Rgb_fromColor(Color color) {
    return Rgb_from(color.r, color.g, color.b);
}

Rgb Color_intoRgb(Color color) {
    return Rgb_fromColor(color);
}

Rgb Hsl_asRgb(Hsl hsl) {
    return Rgb_fromHsl(hsl);
}

Rgb Color_asRgb(Color color) {
    return Rgb_fromColor(color);
}

Hsl Hsl_fromRgb(Rgb rgb) {
    f64 r = rgb.r / as$(f64, ColorChannel_max_value);
    f64 g = rgb.g / as$(f64, ColorChannel_max_value);
    f64 b = rgb.b / as$(f64, ColorChannel_max_value);

    f64 max  = prim_max(prim_max(r, g), b);
    f64 min  = prim_min(prim_min(r, g), b);
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
    return Hsl_from(
        h * 360.0,
        s * 100.0,
        l * 100.0
    );
}

Hsl Rgb_intoHsl(Rgb rgb) {
    return Hsl_fromRgb(rgb);
}

Hsl Hsl_fromColor(Color color) {
    return Rgb_intoHsl(Color_intoRgb(color));
}

Hsl Color_intoHsl(Color color) {
    return Hsl_fromColor(color);
}

Hsl Rgb_asHsl(Rgb rgb) {
    return Hsl_fromRgb(rgb);
}

Hsl Color_asHsl(Color color) {
    return Hsl_fromColor(color);
}

Color Color_fromRgb(Rgb rgb, u8 a) {
    return Color_from(rgb.r, rgb.g, rgb.b, a);
}

Color Color_fromRgbTransparent(Rgb rgb) {
    return Color_fromRgb(rgb, ColorChannel_alpha_transparent);
}

Color Color_fromRgbOpaque(Rgb rgb) {
    return Color_fromRgb(rgb, ColorChannel_alpha_opaque);
}

Color Rgb_intoColor(Rgb rgb, u8 a) {
    return Color_from(rgb.r, rgb.g, rgb.b, a);
}

Color Rgb_intoColorTransparent(Rgb rgb) {
    return Color_fromRgb(rgb, ColorChannel_alpha_transparent);
}

Color Rgb_intoColorOpaque(Rgb rgb) {
    return Color_fromRgb(rgb, ColorChannel_alpha_opaque);
}

Color Color_fromHsl(Hsl hsl, u8 a) {
    return Rgb_intoColor(Hsl_intoRgb(hsl), a);
}

Color Color_fromHslTransparent(Hsl hsl) {
    return Color_fromHsl(hsl, ColorChannel_alpha_transparent);
}

Color Color_fromHslOpaque(Hsl hsl) {
    return Color_fromHsl(hsl, ColorChannel_alpha_opaque);
}

Color Hsl_intoColor(Hsl hsl, u8 a) {
    return Color_fromHsl(hsl, a);
}

Color Hsl_intoColorTransparent(Hsl hsl) {
    return Color_fromHslTransparent(hsl);
}

Color Hsl_intoColorOpaque(Hsl hsl) {
    return Color_fromHslOpaque(hsl);
}
