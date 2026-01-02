#include "color.h"

/*========== Internal Declarations ==========================================*/

$attr($inline_always)
$static fn_((color_HSL__hueToRGBSpace(f64 p, f64 q, f64 t))(f64));

/*========== External Definitions ===========================================*/

/* --- RGBA --- */

fn_((color_RGBA_from(u8 r, u8 g, u8 b, u8 a))(color_RGBA)) {
    return color_RGBA_from_static(r, g, b, a);
};

fn_((color_RGBA_fromBlank(u8 r, u8 g, u8 b))(color_RGBA)) {
    return color_RGBA_fromBlank_static(r, g, b);
};

fn_((color_RGBA_fromOpaque(u8 r, u8 g, u8 b))(color_RGBA)) {
    return color_RGBA_fromOpaque_static(r, g, b);
};

fn_((color_RGBA_fromRGB(color_RGB rgb, u8 a))(color_RGBA)) {
    return color_RGBA_from(rgb.r, rgb.g, rgb.b, a);
};

fn_((color_RGBA_fromBlankRGB(color_RGB rgb))(color_RGBA)) {
    return color_RGBA_fromBlank(rgb.r, rgb.g, rgb.b);
};

fn_((color_RGBA_fromOpaqueRGB(color_RGB rgb))(color_RGBA)) {
    return color_RGBA_fromOpaque(rgb.r, rgb.g, rgb.b);
};

fn_((color_RGBA_toRGB(color_RGBA self))(color_RGB)) {
    return color_RGB_from(self.r, self.g, self.b);
};

fn_((color_RGBA_fromHSL(color_HSL hsl, u8 a))(color_RGBA)) {
    return color_RGBA_fromRGB(color_HSL_toRGB(hsl), a);
};

fn_((color_RGBA_fromBlankHSL(color_HSL hsl))(color_RGBA)) {
    return color_RGBA_fromBlankRGB(color_HSL_toRGB(hsl));
};

fn_((color_RGBA_fromOpaqueHSL(color_HSL hsl))(color_RGBA)) {
    return color_RGBA_fromOpaqueRGB(color_HSL_toRGB(hsl));
};

fn_((color_RGBA_toHSL(color_RGBA self))(color_HSL)) {
    return color_RGB_toHSL(color_RGBA_toRGB(self));
};

/* --- RGB --- */

fn_((color_RGB_from(u8 r, u8 g, u8 b))(color_RGB)) {
    return color_RGB_from_static(r, g, b);
};

fn_((color_RGB_toRGBA(color_RGB self, u8 a))(color_RGBA)) {
    return color_RGBA_fromRGB(self, a);
};

fn_((color_RGB_toBlankRGBA(color_RGB self))(color_RGBA)) {
    return color_RGBA_fromBlankRGB(self);
};

fn_((color_RGB_toOpaqueRGBA(color_RGB self))(color_RGBA)) {
    return color_RGBA_fromOpaqueRGB(self);
};

fn_((color_RGB_fromHSL(color_HSL hsl))(color_RGB)) {
    let h = hsl.h / 360.0;
    let s = hsl.s / 100.0;
    let l = hsl.l / 100.0;

    f64 r = 0.0;
    f64 g = 0.0;
    f64 b = 0.0;
    if (s == 0.0) {
        r = g = b = l;
    } else {
        let q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        let p = 2.0 * l - q;
        r = color_HSL__hueToRGBSpace(p, q, h + 1.0 / 3.0);
        g = color_HSL__hueToRGBSpace(p, q, h);
        b = color_HSL__hueToRGBSpace(p, q, h - 1.0 / 3.0);
    }

    let max_val = intToFlt$((f64)(color_RGBA_channels_max_value));
    return color_RGB_from(
        fltToInt$((u8)(flt_clamp(r * max_val, 0.0, max_val))),
        fltToInt$((u8)(flt_clamp(g * max_val, 0.0, max_val))),
        fltToInt$((u8)(flt_clamp(b * max_val, 0.0, max_val))));
};

fn_((color_RGB_toHSL(color_RGB self))(color_HSL)) {
    let max_val = intToFlt$((f64)(color_RGBA_channels_max_value));
    let r = self.r / max_val;
    let g = self.g / max_val;
    let b = self.b / max_val;

    let max = flt_max(flt_max(r, g), b);
    let min = flt_min(flt_min(r, g), b);
    let diff = max - min;
    let l = (max + min) / 2.0;

    f64 h = 0.0;
    f64 s = 0.0;
    if (diff != 0.0) {
        s = 0.5 < l ? diff / (2.0 - max - min) : diff / (max + min);
        if (max == r) {
            h = (g - b) / diff + (g < b ? 6.0 : 0.0);
        } else if (max == g) {
            h = (b - r) / diff + 2.0;
        } else {
            h = (r - g) / diff + 4.0;
        }
        h /= 6.0;
    }

    return color_HSL_from(h * 360.0, s * 100.0, l * 100.0);
};

/* --- HSL --- */

fn_((color_HSL_from(f64 h, f64 s, f64 l))(color_HSL)) {
    return color_HSL_from_static(h, s, l);
};

fn_((color_HSL_toRGBA(color_HSL self, u8 a))(color_RGBA)) {
    return color_RGB_toRGBA(color_HSL_toRGB(self), a);
};

fn_((color_HSL_toBlankRGBA(color_HSL self))(color_RGBA)) {
    return color_RGB_toBlankRGBA(color_HSL_toRGB(self));
};

fn_((color_HSL_toOpaqueRGBA(color_HSL self))(color_RGBA)) {
    return color_RGB_toOpaqueRGBA(color_HSL_toRGB(self));
};

fn_((color_HSL_fromRGB(color_RGB rgb))(color_HSL)) {
    return color_RGB_toHSL(rgb);
};

fn_((color_HSL_toRGB(color_HSL self))(color_RGB)) {
    return color_RGB_fromHSL(self);
};

/*========== Internal Definitions ===========================================*/

fn_((color_HSL__hueToRGBSpace(f64 p, f64 q, f64 t))(f64)) {
    if (t < 0.0) { t += 1.0; }
    if (1.0 < t) { t -= 1.0; }
    if (t < 1.0 / 6.0) { return p + (q - p) * 6.0 * t; }
    if (t < 1.0 / 2.0) { return q; }
    if (t < 2.0 / 3.0) { return p + (q - p) * (2.0 / 3.0 - t) * 6.0; }
    return p;
};
