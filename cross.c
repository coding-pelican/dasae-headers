#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PP_Unused(x) ((void)(x))


typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float_t  f32;
typedef double_t f64;


typedef struct RGBAColor RGBAColor;
struct RGBAColor {
    union {
        struct {
            u8 rgba[4];
        };
        struct {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        };
    };
};
typedef RGBAColor Color;
#define Color_From(...) (              \
    (Color){ .rgba = { __VA_ARGS__ } } \
)

typedef struct HSLColor HSLColor;
struct HSLColor {
    union {
        struct {
            f64 hsl[3];
        };
        struct {
            f64 h; // Hue in [0, 360]
            f64 s; // Saturation in [0, 100]
            f64 l; // Lightness in [0, 100]
        };
    };
};
#define HSLColor_From(...) (             \
    (HSLColor){ .hsl = { __VA_ARGS__ } } \
)


const uint8_t* Color_AsByteSlice(const Color* colors, size_t len) {
    PP_Unused(len);
    return colors->rgba; // Interpret the Color array as a byte array
}

const Color* Color_FromBytes(const uint8_t* bytes, size_t len) {
    if (len % sizeof(Color) != 0) {
        perror("Byte slice must be a multiple of Color size\n");
        exit(EXIT_FAILURE);
    }
    return (const Color*)bytes; // Reinterpret the byte slice as Color array
}


HSLColor Color_toHSL(const Color* color) {
    f64 r = color->r / 255.0;
    f64 g = color->g / 255.0;
    f64 b = color->b / 255.0;

    f64 max_val = fmax(r, fmax(g, b));
    f64 min_val = fmin(r, fmin(g, b));
    f64 l       = (max_val + min_val) / 2.0;

    HSLColor hsl = HSLColor_From(0.0, 0.0, l * 100.0);

    if (max_val == min_val) {
        return hsl; // Achromatic
    }

    f64 delta = max_val - min_val;
    hsl.s     = (l > 0.5) ? delta / (2.0 - max_val - min_val) : delta / (max_val + min_val);

    if (max_val == r) {
        hsl.h = (g - b) / delta + (g < b ? 6.0 : 0.0);
    } else if (max_val == g) {
        hsl.h = (b - r) / delta + 2.0;
    } else {
        hsl.h = (r - g) / delta + 4.0;
    }

    hsl.h *= 60.0; // Convert to degrees
    hsl.s *= 100.0; // Scale to percentage
    hsl.l *= 100.0; // Scale to percentage
    return hsl;
}


f64 HUE_toRGB(f64 p, f64 q, f64 t) {
    if (t < 0) {
        t += 1;
    }
    if (t > 1) {
        t -= 1;
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

Color HSL_toColor(const HSLColor* hsl) {
    f64 r = 0.0;
    f64 g = 0.0;
    f64 b = 0.0;

    f64 h = hsl->h / 360.0;
    f64 s = hsl->s / 100.0;
    f64 l = hsl->l / 100.0;

    if (s == 0.0) {
        r = g = b = l; // Achromatic
    } else {
        f64 q = (l < 0.5) ? (l * (1.0 + s)) : (l + s - l * s);
        f64 p = 2.0 * l - q;
        r     = HUE_toRGB(p, q, h + 1.0 / 3.0);
        g     = HUE_toRGB(p, q, h);
        b     = HUE_toRGB(p, q, h - 1.0 / 3.0);
    }
    return Color_From((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), 255);
}


int main() {
    // Create a color from RGB values
    Color color[1] = { Color_From(255, 100, 50, 255) };

    // Convert it to HSL
    HSLColor hsl[1] = { Color_toHSL(color) };
    printf("HSL: (%f, %f, %f)\n", hsl->h, hsl->s, hsl->l);

    // Convert it back to RGB
    Color rgb_color[1] = { HSL_toColor(hsl) };
    printf("RGB: (%u, %u, %u, %u)\n", rgb_color->r, rgb_color->g, rgb_color->b, rgb_color->a);

    return 0;
}
