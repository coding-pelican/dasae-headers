#include "dacolor.h"

/*========== Internal Declarations ==========================================*/

$attr($inline_always)
$static fn_((dacolor_HSL__hueToRGBSpace(f64 p, f64 q, f64 t))(f64));

/*========== External Definitions ===========================================*/

/* --- RGBA --- */

fn_((dacolor_RGBA_from(u8 r, u8 g, u8 b, u8 a))(dacolor_RGBA)) {
    return dacolor_RGBA_from_static(r, g, b, a);
};

fn_((dacolor_RGBA_fromBlank(u8 r, u8 g, u8 b))(dacolor_RGBA)) {
    return dacolor_RGBA_fromBlank_static(r, g, b);
};

fn_((dacolor_RGBA_fromOpaque(u8 r, u8 g, u8 b))(dacolor_RGBA)) {
    return dacolor_RGBA_fromOpaque_static(r, g, b);
};

fn_((dacolor_RGBA_fromRGB(dacolor_RGB rgb, u8 a))(dacolor_RGBA)) {
    return dacolor_RGBA_from(rgb.r, rgb.g, rgb.b, a);
};

fn_((dacolor_RGBA_fromBlankRGB(dacolor_RGB rgb))(dacolor_RGBA)) {
    return dacolor_RGBA_fromBlank(rgb.r, rgb.g, rgb.b);
};

fn_((dacolor_RGBA_fromOpaqueRGB(dacolor_RGB rgb))(dacolor_RGBA)) {
    return dacolor_RGBA_fromOpaque(rgb.r, rgb.g, rgb.b);
};

fn_((dacolor_RGBA_toRGB(dacolor_RGBA self))(dacolor_RGB)) {
    return dacolor_RGB_from(self.r, self.g, self.b);
};

fn_((dacolor_RGBA_fromHSL(dacolor_HSL hsl, u8 a))(dacolor_RGBA)) {
    return dacolor_RGBA_fromRGB(dacolor_HSL_toRGB(hsl), a);
};

fn_((dacolor_RGBA_fromBlankHSL(dacolor_HSL hsl))(dacolor_RGBA)) {
    return dacolor_RGBA_fromBlankRGB(dacolor_HSL_toRGB(hsl));
};

fn_((dacolor_RGBA_fromOpaqueHSL(dacolor_HSL hsl))(dacolor_RGBA)) {
    return dacolor_RGBA_fromOpaqueRGB(dacolor_HSL_toRGB(hsl));
};

fn_((dacolor_RGBA_toHSL(dacolor_RGBA self))(dacolor_HSL)) {
    return dacolor_RGB_toHSL(dacolor_RGBA_toRGB(self));
};

/* --- RGB --- */

fn_((dacolor_RGB_from(u8 r, u8 g, u8 b))(dacolor_RGB)) {
    return dacolor_RGB_from_static(r, g, b);
};

fn_((dacolor_RGB_toRGBA(dacolor_RGB self, u8 a))(dacolor_RGBA)) {
    return dacolor_RGBA_fromRGB(self, a);
};

fn_((dacolor_RGB_toBlankRGBA(dacolor_RGB self))(dacolor_RGBA)) {
    return dacolor_RGBA_fromBlankRGB(self);
};

fn_((dacolor_RGB_toOpaqueRGBA(dacolor_RGB self))(dacolor_RGBA)) {
    return dacolor_RGBA_fromOpaqueRGB(self);
};

fn_((dacolor_RGB_fromHSL(dacolor_HSL hsl))(dacolor_RGB)) {
    let h = hsl.h / 360.0;
    let s = hsl.s / 100.0;
    let l = hsl.l / 100.0;

    var_(r, f64) = 0.0;
    var_(g, f64) = 0.0;
    var_(b, f64) = 0.0;
    if (s == 0.0) {
        r = g = b = l;
    } else {
        let q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        let p = 2.0 * l - q;
        r = dacolor_HSL__hueToRGBSpace(p, q, h + 1.0 / 3.0);
        g = dacolor_HSL__hueToRGBSpace(p, q, h);
        b = dacolor_HSL__hueToRGBSpace(p, q, h - 1.0 / 3.0);
    }

    let max_val = intToFlt$((f64)(dacolor_RGBA_channels_max_value));
    return dacolor_RGB_from(
        fltToInt$((u8)(flt_clamp(r * max_val, 0.0, max_val))),
        fltToInt$((u8)(flt_clamp(g * max_val, 0.0, max_val))),
        fltToInt$((u8)(flt_clamp(b * max_val, 0.0, max_val))) );
};

fn_((dacolor_RGB_toHSL(dacolor_RGB self))(dacolor_HSL)) {
    let max_val = intToFlt$((f64)(dacolor_RGBA_channels_max_value));
    let r = self.r / max_val;
    let g = self.g / max_val;
    let b = self.b / max_val;

    let max = flt_max(flt_max(r, g), b);
    let min = flt_min(flt_min(r, g), b);
    let diff = max - min;
    let l = (max + min) / 2.0;

    var_(h, f64) = 0.0;
    var_(s, f64) = 0.0;
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

    return dacolor_HSL_from(h * 360.0, s * 100.0, l * 100.0);
};

/* --- HSL --- */

fn_((dacolor_HSL_from(f64 h, f64 s, f64 l))(dacolor_HSL)) {
    return dacolor_HSL_from_static(h, s, l);
};

fn_((dacolor_HSL_toRGBA(dacolor_HSL self, u8 a))(dacolor_RGBA)) {
    return dacolor_RGB_toRGBA(dacolor_HSL_toRGB(self), a);
};

fn_((dacolor_HSL_toBlankRGBA(dacolor_HSL self))(dacolor_RGBA)) {
    return dacolor_RGB_toBlankRGBA(dacolor_HSL_toRGB(self));
};

fn_((dacolor_HSL_toOpaqueRGBA(dacolor_HSL self))(dacolor_RGBA)) {
    return dacolor_RGB_toOpaqueRGBA(dacolor_HSL_toRGB(self));
};

fn_((dacolor_HSL_fromRGB(dacolor_RGB rgb))(dacolor_HSL)) {
    return dacolor_RGB_toHSL(rgb);
};

fn_((dacolor_HSL_toRGB(dacolor_HSL self))(dacolor_RGB)) {
    return dacolor_RGB_fromHSL(self);
};

/*========== Internal Definitions ===========================================*/

fn_((dacolor_HSL__hueToRGBSpace(f64 p, f64 q, f64 t))(f64)) {
    if (t < 0.0) t += 1.0;
    if (1.0 < t) t -= 1.0;
    if (t < 1.0 / 6.0) return p + (q - p) * 6.0 * t;
    if (t < 1.0 / 2.0) return q;
    if (t < 2.0 / 3.0) return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
    return p;
};
