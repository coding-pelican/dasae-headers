#include "color.h"
#include "floats.h"


Color Color_from_HSLA(ColorHSL color, u8 alpha) {
    return ColorHSL_to_RGBA(color, alpha);
}

Color Color_from_HSL(ColorHSL color) {
    return ColorHSL_to_RGBA(color, 255);
}

// RGB to HSL conversion
ColorHSL Color_to_HSL(Color color) {
    f64 r = color.r / 255.0;
    f64 g = color.g / 255.0;
    f64 b = color.b / 255.0;

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
    return ColorHSL_from(
        h * 360.0,
        s * 100.0,
        l * 100.0
    );
}

ColorHSL ColorHSL_from_RGBA(Color color) {
    return Color_to_HSL(color);
}

// HSL to RGBA conversion
Color ColorHSL_to_RGBA(ColorHSL color, u8 alpha) {
    f64 h = color.h / 360.0;
    f64 s = color.s / 100.0;
    f64 l = color.l / 100.0;

    f64 r = f64_nan;
    f64 g = f64_nan;
    f64 b = f64_nan;

    if (s == 0.0) {
        r = g = b = l;
    } else {
        f64 q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        f64 p = 2.0 * l - q;
        r     = ColorHSL_Hue_to_RGB(p, q, h + 1.0 / 3.0);
        g     = ColorHSL_Hue_to_RGB(p, q, h);
        b     = ColorHSL_Hue_to_RGB(p, q, h - 1.0 / 3.0);
    }

    return Color_from_RGBA(
        (u8)f64_clamp(r * 255.0, 0, 255.0),
        (u8)f64_clamp(g * 255.0, 0, 255.0),
        (u8)f64_clamp(b * 255.0, 0, 255.0),
        alpha
    );
}

// HSL to RGB conversion
Color ColorHSL_to_RGB(ColorHSL color) {
    return ColorHSL_to_RGBA(color, 255);
}

// Helper function for HSL to RGB conversion
f64 ColorHSL_Hue_to_RGB(f64 p, f64 q, f64 t) {
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
