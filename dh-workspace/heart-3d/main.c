#include "dh/main.h"
#include "dh/core.h"
#include "dh/heap/Classic.h"
#include "dh/log.h"

#include "dh/debug.h"

#include "dh/math/vec.h"
#include "dh/mem/Allocator.h"
#include "dh/ArrList.h"
#include "dh/meta/common.h"
#include "dh/Mat.h"

#include "dh/Random.h"
#include "dh/Str.h"

#include "engine.h"
#include "engine/color.h"
#include "engine/platform_backend.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "libs/stb_truetype.h"

#define window_res_width__320x200  /* template value */ (320)
#define window_res_height__320x200 /* template value */ (200)
#define window_res_width__160x100  /* template value */ (160)
#define window_res_height__160x100 /* template value */ (100)
#define window_res_width__80x50    /* template value */ (80)
#define window_res_height__80x50   /* template value */ (50)
#define window_res_width__40x25    /* template value */ (40)
#define window_res_height__40x25   /* template value */ (25)

#define window_res_width  (window_res_width__160x100)
#define window_res_height (window_res_height__160x100)
#define window_res_size   (as$(usize, window_res_width) * window_res_height)

/* (1.0 / target_fps__62_50) ~16ms => ~60 FPS, Assume 62.5 FPS for simplicity */
#define target_fps__125_0 /* template value */ (125.0)
#define target_fps__62_50 /* template value */ (62.50)
#define target_fps__50_00 /* template value */ (50.00)
#define target_fps__31_25 /* template value */ (31.25)

#define target_fps (target_fps__62_50)
#define target_spf (1.0 / target_fps)

#define font_size (22)

use_Mat$(u8);
use_Mat$(f32);
use_Sli$(Color);
use_Mat$(Color);

// Helper function to scale colors
force_inline Color Color_scale(Color color, f32 factor) {
    return Color_fromOpaque(
        as$(u8, as$(f32, color.r) * factor),
        as$(u8, as$(f32, color.g) * factor),
        as$(u8, as$(f32, color.b) * factor)
    );
}
// Blend two colors based on factor
force_inline Color Color_blend(Color lhs, Color rhs, f32 factor) {
    return Color_fromOpaque(
        as$(u8, lhs.r * (1.0f - factor) + rhs.r * factor),
        as$(u8, lhs.g * (1.0f - factor) + rhs.g * factor),
        as$(u8, lhs.b * (1.0f - factor) + rhs.b * factor)
    );
}
force_inline Color Color_lerp(Color begin, Color end, f32 t) {
    // clamp t in [0..1]
    if (t < 0.0f) { t = 0.0f; }
    if (t > 1.0f) { t = 1.0f; }

    // linear interpolation per channel
    Color result;
    result.r = (u8)((1.0f - t) * (f32)begin.r + t * (f32)end.r);
    result.g = (u8)((1.0f - t) * (f32)begin.g + t * (f32)end.g);
    result.b = (u8)((1.0f - t) * (f32)begin.b + t * (f32)end.b);
    result.a = 255; // or blend alpha if you want
    return result;
}

// ========== Font Data Structures ==========
typedef struct KoreanFont {
    stbtt_fontinfo info;
    Sli$u8         font_buffer;
    f32            scale;
    i32            ascent;
    i32            descent;
    i32            line_gap;
    mem_Allocator  allocator;
} KoreanFont;
use_Opt$(KoreanFont);
use_Err$(KoreanFont);
use_Sli$(KoreanFont);
use_Mat$(KoreanFont);

typedef struct Glyph {
    Mat$u8 bitmap; // alpha values
    i32    width;
    i32    height;
    i32    x_offset;
    i32    y_offset;
    i32    advance;
} Glyph;
use_Opt$(Glyph);
use_Err$(Glyph);
use_Sli$(Glyph);
use_Mat$(Glyph);
use_ArrList$(Glyph);

// ========== Font Errors / Declarations (unchanged) ==========
config_ErrSet(
    FontSystemErr,
    FailedToOpenFontFile,
    FailedToLoadFontInfo,
    FailedToInitFontInfo,
    FailedToLoadGlyph,
    FailedToRenderGlyph,
    mem_AllocErr_OutOfMemory
);

// ========== FontSystem_init / fini (unchanged, except we read .ttf) ==========
use_ErrSet$(FontSystemErr, KoreanFont);
must_check FontSystemErr$KoreanFont FontSystem_init(mem_Allocator allocator, const char* filename, f32 size_pixels) {
    scope_reserveReturn(FontSystemErr$KoreanFont) {
        FILE* font_file = fopen(filename, "rb");
        if (!font_file) {
            return_err(FontSystemErr_FailedToOpenFontFile());
        }
        defer(ignore fclose(font_file));

        ignore fseek(font_file, 0, SEEK_END);
        long   file_size = ftell(font_file);
        ignore fseek(font_file, 0, SEEK_SET);

        let font_buffer = meta_cast$(Sli$u8, try(mem_Allocator_alloc(allocator, typeInfo(u8), file_size)));
        errdefer(mem_Allocator_free(allocator, anySli(font_buffer)));
        if (fread(font_buffer.ptr, 1, (size_t)file_size, font_file) != (size_t)file_size) {
            return_err(FontSystemErr_FailedToLoadFontInfo());
        }

        KoreanFont font = {
            .font_buffer = font_buffer,
            .allocator   = allocator
        };

        if (!stbtt_InitFont(&font.info, font_buffer.ptr, 0)) {
            return_err(FontSystemErr_FailedToInitFontInfo());
        }

        font.scale = stbtt_ScaleForPixelHeight(&font.info, size_pixels);

        stbtt_GetFontVMetrics(&font.info, &font.ascent, &font.descent, &font.line_gap);
        return_ok(font);
    }
    scope_returnReserved;
}

void FontSystem_fini(KoreanFont* font) {
    if (font->font_buffer.ptr) {
        mem_Allocator_free(font->allocator, anySli(font->font_buffer));
    }
}

// ========== Updated FontSystem_renderGlyph using stbtt_GetGlyphBitmapBox ==========
// 1) Use bounding-box approach with stbtt_GetGlyphBitmapBox
// 2) Allocate raw bytes with typeInfo(u8)
// 3) Remove the old bounding logic based on x0,y0 from stbtt_GetGlyphBox
use_ErrSet$(FontSystemErr, Glyph);
must_check FontSystemErr$Glyph FontSystem_renderGlyph(const KoreanFont* font, i32 codepoint, f32 scale_factor) {
    scope_reserveReturn(FontSystemErr$Glyph) {
        // 1) Find glyph index
        i32 glyph_index = stbtt_FindGlyphIndex(&font->info, codepoint);
        if (glyph_index == 0) {
            return_err(FontSystemErr_FailedToLoadGlyph());
        }

        // 2) Horizontal metrics (for advance)
        i32 advance = 0;
        i32 lsb     = 0;
        stbtt_GetGlyphHMetrics(&font->info, glyph_index, &advance, &lsb);

        // 3) bounding box in pixel coords
        f32 final_scale = font->scale * scale_factor;
        i32 ix0         = 0;
        i32 iy0         = 0;
        i32 ix1         = 0;
        i32 iy1         = 0;
        stbtt_GetGlyphBitmapBox(
            &font->info, glyph_index, final_scale, final_scale, &ix0, &iy0, &ix1, &iy1
        );

        i32 w = ix1 - ix0;
        i32 h = iy1 - iy0;
        if (w <= 0 || h <= 0) {
            // blank or empty glyph, return a zero-bitmap
            Glyph gBlank = {
                .bitmap   = Mat_fromSli$(Mat$u8, meta_cast$(Sli$u8, try(mem_Allocator_alloc(font->allocator, typeInfo(u8), 1))), 1, 1),
                .width    = 0,
                .height   = 0,
                .x_offset = 0,
                .y_offset = 0,
                .advance  = (i32)((f32)advance * final_scale)
            };
            return_ok(gBlank);
        }

        // 4) Allocate memory for the bitmap
        usize  size = (usize)w * h;
        Sli$u8 mem  = meta_cast$(
            Sli$u8,
            try(mem_Allocator_alloc(font->allocator, typeInfo(u8), size))
        );
        let bitmap = Mat_fromSli$(Mat$u8, mem, w, h);

        // 5) Render via stbtt_GetGlyphBitmap
        i32 outW     = 0;
        i32 outH     = 0;
        u8* stb_data = stbtt_GetGlyphBitmap(
            &font->info,
            final_scale,
            final_scale,
            glyph_index,
            &outW,
            &outH,
            0,
            0
        );
        if (!stb_data) {
            mem_Allocator_free(font->allocator, anySli(bitmap.items));
            return_err(FontSystemErr_FailedToRenderGlyph());
        }
        defer(stbtt_FreeBitmap(stb_data, null));

        // 6) copy
        if (outW != w || outH != h) {
            log_warn("Warning: bounding box mismatch: box(%d,%d), out(%d,%d)\n", w, h, outW, outH);
        }
        memcpy(bitmap.items.ptr, stb_data, (usize)outW * outH);

        // 7) Build final glyph
        Glyph glyph = {
            .bitmap   = bitmap,
            .width    = w,
            .height   = h,
            .x_offset = ix0, // subpixel offset from bounding box
            .y_offset = iy0,
            .advance  = (i32)((f32)advance * final_scale)
        };
        return_ok(glyph);
    }
    scope_returnReserved;
}

// ========== Render glyph to canvas (unchanged) ==========
void FontSystem_renderGlyphToCanvas(const Glyph* glyph, engine_Canvas* canvas, i32 x, i32 y, Color color) {
    for (i32 dy = 0; dy < glyph->height; ++dy) {
        for (i32 dx = 0; dx < glyph->width; ++dx) {
            u8 alpha = *Mat_at(glyph->bitmap, dx, dy);
            if (alpha <= 0) { continue; }
            // scale color
            Color pixel_color = Color_fromOpaque(
                (u8)((u32)color.r * alpha / 255),
                (u8)((u32)color.g * alpha / 255),
                (u8)((u32)color.b * alpha / 255)
            );
            engine_Canvas_drawPixel(
                canvas,
                x + glyph->x_offset + dx,
                y + glyph->y_offset + dy,
                pixel_color
            );
        }
    }
}

// ========== Remove utf8_decode() and use StrUtf8Iter instead ==========
// This function now takes Str_const instead of const char* for text input.
must_check FontSystemErr$void renderKoreanText(Str_const text, engine_Canvas* canvas, i32 x, i32 y, usize size_pixels) {
    scope_reserveReturn(FontSystemErr$void) {
        var        allocator = heap_Classic_allocator(&(heap_Classic){});
        // 1) Init font
        KoreanFont font      = try(FontSystem_init(allocator, "assets/Galmuri11-Bold.ttf", size_pixels));
        defer(FontSystem_fini(&font));

        i32 cursor_x = x;
        i32 cursor_y = y;

        // 2) Create an iterator over code points
        StrUtf8Iter iter = StrUtf8_iter(text);
        Opt$u32     cp   = { 0 };
        // 3) For each codepoint
        while (StrUtf8Iter_next(&iter, &cp)) {
            if (isNone(cp)) {
                // Invalid codepoint or end
                continue;
            }
            i32 codepoint = (i32)cp.value;

            // 4) Render glyph
            scope_with(let glyph = try(FontSystem_renderGlyph(&font, codepoint, 1.0f))) {
                FontSystem_renderGlyphToCanvas(
                    &glyph,
                    canvas,
                    cursor_x,
                    cursor_y,
                    Color_fromOpaque(255, 255, 255)
                );
                // move X
                cursor_x += glyph.advance;

                // release glyph memory
                mem_Allocator_free(allocator, anySli(glyph.bitmap.items));
            }
        }
        return_ok({});
    }
    scope_returnReserved;
}

/*========== Fireworks ======================================================*/

#define Firework_effects_max        (50)
#define Firework_effects_per_rocket (25)
#define Fireworks_max               (16)

typedef struct Particle {
    f64   position[2];
    f64   speed[2];
    f64   acceleration[2];
    f64   fading;
    f64   lifetime;
    Color color;
    u32   dimensions[2];
} Particle;
use_Opt$(Particle);
use_Err$(Particle);
use_Sli$(Particle);
extern Particle  Particle_new(f64 x, f64 y, f64 width, f64 height, Color color);
extern Particle* Particle_init(Particle* p, f64 x, f64 y, f64 width, f64 height, Color color);
extern Particle* Particle_initWithSpeed(Particle* p, f64 x, f64 y);
extern Particle* Particle_initWithAcceleration(Particle* p, f64 x, f64 y);
extern Particle* Particle_initWithFading(Particle* p, f64 fading);
extern bool      Particle_isDead(const Particle* p);
extern void      Particle_update(Particle* p, f64 dt);
extern void      Particle_render(const Particle* p, engine_Canvas* c, f64 dt);

use_ArrList$(Particle);
typedef struct Firework {
    Opt$Ptr$Particle rocket;
    ArrList$Particle effects;
    Hsl              effect_base_color;
    mem_Allocator    allocator;
} Firework;
use_Opt$(Firework);
use_Err$(Firework);
use_Err$(Opt$Ptr$Firework);
use_Sli$(Firework);
extern Err$Ptr$Firework Firework_init(Firework* f, mem_Allocator allocator, i64 rocket_x, i64 rocket_y, Color effect_base_color) must_check;
extern void             Firework_fini(Firework* f);
extern bool             Firework_isDead(const Firework* f);
extern Err$void         Firework_update(Firework* f, f64 dt) must_check;
extern void             Firework_render(const Firework* f, engine_Canvas* c, f64 dt);

use_ArrList$(Firework);
typedef struct Simulator {
    ArrList$Firework fireworks;
    u32              width;
    u32              height;
    mem_Allocator    allocator;
    bool             is_running;
} State;
use_Err$(State);
extern Err$State            State_init(mem_Allocator allocator, u32 width, u32 height) must_check;
extern void                 State_fini(State* s);
extern bool                 State_isDead(const State* s);
extern Err$void             State_update(State* s, f64 dt) must_check;
extern void                 State_render(const State* s, engine_Canvas* c, f64 dt);
extern Err$Opt$Ptr$Firework State_spawnFirework(State* s) must_check;

// Helper function to generate pastel colors for fireworks
static Color generatePastelColor(void);
// Helper to create a color palette based on the heart's pink
static Color generateHeartBasedColor(void);

// Add this helper function to create effects without a rocket
Err$void Firework_createEffectsAt(Firework* f, f64 x, f64 y, f64 scale, Color base_color) {
    reserveReturn(Err$void);
    debug_assert_nonnull(f);

    f->effect_base_color = Color_intoHsl(base_color);

    for (i64 i = 0; i < Firework_effects_per_rocket; ++i) {
        if (Firework_effects_max <= f->effects.items.len) { break; }

        scope_with(let particle = meta_castPtr$(Particle*, try(ArrList_addBackOne(&f->effects.base)))) {
            let width  = 1.0 * scale;
            let height = 1.0 * scale;

            Particle_init(particle, x, y, width, height, base_color);

            // More varied and slightly slower particle movement
            f32 angle = (f32)Random_f64() * math_f32_tau;
            f32 speed = (0.3f + (f32)Random_f64() * 0.7f) * (f32)scale; // Varied speeds

            Particle_initWithSpeed(
                particle,
                cosf(angle) * speed,
                sinf(angle) * speed
            );

            // Gentler acceleration
            Particle_initWithAcceleration(particle, 0.0, 0.01);

            // Slower fading for longer-lasting effect
            Particle_initWithFading(particle, 0.005 / scale);
        }
    }

    return_void();
}

// Add a function to create a firework without a rocket
Err$Ptr$Firework Firework_createAt(Firework* f, mem_Allocator allocator, f64 x, f64 y, f64 scale, Color base_color) {
    scope_reserveReturn(Err$Ptr$Firework) {
        debug_assert_nonnull(f);

        f->allocator = allocator;
        assignNone(f->rocket); // No rocket

        f->effects = typed(ArrList$Particle, try(ArrList_initCap(typeInfo(Particle), f->allocator, Firework_effects_per_rocket)));
        errdefer(ArrList_fini(&f->effects.base));

        try(Firework_createEffectsAt(f, x, y, scale, base_color));

        return_ok(f);
    }
    scope_returnReserved;
}

// Helper function for interpolation if not already defined
force_inline f32 math_f32_lerp(f32 a, f32 b, f32 t) {
    return a + (b - a) * t;
}

/*========== Heart 3d =======================================================*/

// #define render_depth_steps (24)     // Increased for smoother rendering
// #define render_step_size   (0.005f) // Smaller steps for higher resolution
#define render_depth_steps   (12)
#define render_step_size     (0.01f)
#define heart_beat_frequency (6.0f)
typedef struct RenderBuffer {
    engine_Canvas* canvas;
    Sli$f32        z_buffer;
} RenderBuffer;

typedef struct CanvasAscii {
    const engine_Canvas* color; /* using RenderBuffer's canvas: pixel upper(0,2,4...) as fg, lower(1,3,5...) as bg */
    Mat$u8               ascii;
} CanvasAscii;

// Rotation angles and scaling factor
static Vec3f angle           = math_Vec3f_zero;
static f32   scale           = 1.0f;
static f32   scale_direction = 0.001f;

// Light position and properties
// static Vec3f light_pos      = literal_math_Vec3f_from(2.0f, 2.0f, -2.0f);
// Make this truly static and pointing from "top-right" to bottom-left, for instance:
static Vec3f       g_light_dir    = { { -1.0f, 1.0f, -1.0f } };
static const Color heart_color    = literal_Color_fromOpaque(220, 20, 60); // Crimson red
static const Color specular_color = literal_Color_fromOpaque(255, 255, 255);

static const f32 ascii_on_time                      = 07.21f;        // full ASCII
static const f32 ascii_to_pixel_transition_begin    = ascii_on_time; // time (seconds) when fade starts
static const f32 ascii_to_pixel_transition_duration = 03.22f;        // to go from ASCII to pixels
static const f32 ascii_to_pixel_transition_end      = ascii_to_pixel_transition_begin + ascii_to_pixel_transition_duration;

static const f32 red_on_time                     = 10.07f;                                      // time (seconds) when fade starts
static const f32 red_to_pink_transition_start    = ascii_to_pixel_transition_end + red_on_time; // time (seconds) when fade starts
static const f32 red_to_pink_transition_duration = 03.00f;                                      // how many seconds the fade lasts
static const f32 red_to_pink_transition_end      = red_to_pink_transition_start + red_to_pink_transition_duration;

static const f32 pink_on_time                         = 10.28f;
static const f32 pink_to_firework_transition_begin    = red_to_pink_transition_end + pink_on_time; // time (seconds) when fade starts
static const f32 pink_to_firework_transition_duration = 01.00f;                                    // to go from pixels to firework
static const f32 pink_to_firework_transition_end      = pink_to_firework_transition_begin + pink_to_firework_transition_duration;

static bool  fireworks_started = false;
static State state             = cleared(); // For fireworks

static const f32 firework_on_time                                = 6.572f / 2;                                         // time
static const f32 firework_with_msg_1st_printing_fade_in_begin    = pink_to_firework_transition_end + firework_on_time; // time (seconds) when fade starts
static const f32 firework_with_msg_1st_printing_fade_in_duration = 2.00f;
static const f32 firework_with_msg_1st_printing_fade_in_end      = firework_with_msg_1st_printing_fade_in_begin + firework_with_msg_1st_printing_fade_in_duration;
static const f32 firework_with_msg_1st_on_time                   = 2.00f;
static const f32 msg_1st_next_2st_printing_fade_in_begin         = firework_with_msg_1st_printing_fade_in_end + firework_with_msg_1st_on_time; // time (seconds) when fade starts
static const f32 msg_1st_next_2st_printing_fade_in_duration      = 2.00f;
static const f32 msg_1st_next_2st_printing_fade_in_end           = msg_1st_next_2st_printing_fade_in_begin + msg_1st_next_2st_printing_fade_in_duration;

static const Str_const msg_1st = Str_l("안뇽 유니!");
static const Str_const msg_2nd = Str_l("사랑한다구 ㅎㅎ");

// static const f32 firework_on_time      = 6.572f;                        // time (seconds) when fade starts
// static const f32 firework = red_to_pink_transition_end + ; // to go from pixels to firework

// The original heart color (crimson).
static const Color color_heart_normal  = { { 220, 20, 60, 255 } };
// The “soft strawberry milk pink” color?
// static Color color_heart_pinkish = { { 255, 210, 220, 255 } };
static const Color color_heart_pinkish = { { 255, 108, 140, 255 } };

static const Color color_bg_normal = { { 0, 0, 0, 255 } };                                                                // black
static const Color color_bg_white  = { { as$(u8, 255.0f * 0.9f), as$(u8, 255.0f * 0.9f), as$(u8, 255.0f * 0.9f), 255 } }; // white

// Instead of a float ambientStrength, define:
static const Color color_ambient_begin = { { 25, 25, 25, 255 } };
static const Color color_ambient_end   = color_bg_white;
static Color       color_ambient_curr  = color_bg_white;

// Calculate surface normal for the heart at a given point
Vec3f calculateHeartNormal(Vec2f point, f32 radius) {
    unused(radius); // Unused parameter
    // Partial derivatives of the heart equation
    const Vec3f dp = {
        .x = -2.0f * point.x,
        .y = -2.0f * (1.2f * point.y - fabsf(point.x) * 2.0f / 3.0f) * 1.2f,
        .z = 1.0f
    };
    return math_Vec3f_norm(dp);
}

// Calculate the radius of the heart at a given y value
f32 calculateHeartRadius(f32 t, f32 y) {
    return 0.4f + 0.05f * powf(0.5f + 0.5f * sinf(t * heart_beat_frequency + y * 2), 8);
}

// Calculate heart surface z value for a given x,y point (determins shape)
f32 calculateHeartZ(Vec2f point, f32 radius) {
    const f32 z = -point.x * point.x - powf(1.2f * point.y - fabsf(point.x) * 2.0f / 3.0f, 2) + radius * radius;
    /*
    const f32 z = -math_Vec2f_lenSq(point)
                - powf(1.2f * point.y - math_abs(point.x) * 2.0f / 3.0f, 2)
                + radius * radius;
     */
    if (z < 0.0f) { return -1.0f; /* Invalid point */ }
    return sqrtf(z) / (2.0f - point.y);
}

// Apply rotation to a point
Vec3f applyRotation(Vec3f point, Vec3f angles) {
    Vec3f rotated = math_Vec3f_rotate(point, math_Vec3f_unit_x, angles.x);
    rotated       = math_Vec3f_rotate(rotated, math_Vec3f_unit_y, angles.y);
    rotated       = math_Vec3f_rotate(rotated, math_Vec3f_unit_z, angles.z);
    return rotated;
}

// Calculate lighting for a point
Color calculateHeartLighting(Vec3f normal, Vec3f view_pos, Vec3f frag_pos, Color color) {
    // // Calculate lighting vectors
    // // const Vec3f light_dir = math_Vec3f_norm(math_Vec3f_sub(light_pos, frag_pos));
    // Ensure length=1
    const Vec3f light_dir = math_Vec3f_norm(g_light_dir);
    const Vec3f view_dir  = math_Vec3f_norm(math_Vec3f_sub(view_pos, frag_pos));

    // // Calculate light angle
    // const f32 ambient_strength = 0.1f;
    // const f32 n_dot_l          = math_Vec3f_dot(normal, light_dir);
    // // Early exit if surface faces away from light
    // if (n_dot_l <= 0.0f) { return Color_scale(color, ambient_strength); /* Ambient only */ }

    const f32 n_dot_l = math_Vec3f_dot(normal, light_dir);
    if (n_dot_l <= 0.0f) {
        return Color_fromOpaque(
            (u8)prim_min(color.r * (color_ambient_curr.r / 255.0f), 255),
            (u8)prim_min(color.g * (color_ambient_curr.g / 255.0f), 255),
            (u8)prim_min(color.b * (color_ambient_curr.b / 255.0f), 255)
        );
    }

    // Ambient light
    // const Color ambient  = Color_scale(color, ambient_strength);
    const Color ambient = Color_fromOpaque(
        (u8)prim_min(color.r * (color_ambient_curr.r / 255.0f), 255),
        (u8)prim_min(color.g * (color_ambient_curr.g / 255.0f), 255),
        (u8)prim_min(color.b * (color_ambient_curr.b / 255.0f), 255)
    );
    // Diffuse light
    const Color diffuse  = Color_scale(color, n_dot_l);
    // Specular light using Blinn-Phong
    const Color specular = Color_scale(
        specular_color,
        eval({
            const Vec3f halfway_dir = math_Vec3f_norm(
                math_Vec3f_add(light_dir, view_dir)
            );
            const f32 spec = powf(
                prim_max(math_Vec3f_dot(normal, halfway_dir), 0.0f),
                64.0f
            );
            const f32   spec_mod_by_light_angle = spec * n_dot_l; // Modulate by light angle
            eval_return spec_mod_by_light_angle;
        })
    );

    // Combine components
    return Color_fromOpaque(
        as$(u8, prim_min(ambient.r + diffuse.r + specular.r, 255)),
        as$(u8, prim_min(ambient.g + diffuse.g + specular.g, 255)),
        as$(u8, prim_min(ambient.b + diffuse.b + specular.b, 255))
    );
}

// Previous functions remain the same up to renderHeart
void renderHeart(RenderBuffer* buffer, f32 t, Color color) {
    let z_buffer = Mat_fromSli$(Mat$f32, buffer->z_buffer, window_res_width, window_res_height);
    for_slice(z_buffer.items, depth) {
        *depth = -math_f32_inf;
    }

    // Calculate final scale based on transitions
    scale = eval({
        var ret = 0.0f;
        if (t > pink_to_firework_transition_begin) {
            var final_scale         = scale;
            // Calculate transition progress (0.0 to 1.0)
            f32 transition_progress = (t - pink_to_firework_transition_begin) / pink_to_firework_transition_duration;
            transition_progress     = math_clamp(transition_progress, 0.0f, 1.0f);

            // Scale from current size to 0
            final_scale *= (1.0f - transition_progress);
            ret = final_scale;
        } else {
            // Normal pulsing scale behavior
            scale += scale_direction;
            if (scale < 0.8f || 1.2f < scale) {
                scale_direction = -scale_direction;
            }
            ret = scale;
        };
        eval_return ret;
    });

    const Vec3f view_pos = math_Vec3f_scale(math_Vec3f_backward, 5.0f);

    // Render the heart
    for (f32 py = -0.5f; py <= 0.5f; py += render_step_size) {
        const f32 radius = calculateHeartRadius(t, py);

        for (f32 px = -0.5f; px <= 0.5f; px += render_step_size) {
            const Vec2f point = math_Vec2f_from(px, py);
            const f32   pz    = calculateHeartZ(point, radius);
            if (pz < 0.0f) { continue; }

            const Vec3f normal = calculateHeartNormal(point, radius);
            const f32   step_z = pz / render_depth_steps;
            for (isize i = 0; i <= (render_depth_steps * 2LL); ++i) {
                const f32 tz = -pz + ((f32)i * step_z);

                // Scale and rotate point
                const Vec3f pos = eval({
                    let         scaled  = math_Vec3f_scale(math_Vec3f_from(px, py, tz), scale);
                    let         rotated = applyRotation(scaled, angle);
                    eval_return rotated;
                });

                // Transform normal
                // const Vec3f transformed_normal = applyRotation(normal, angle);
                const Vec3f transformed_normal = math_Vec3f_norm(applyRotation(normal, angle));

                // Project to screen space
                const f32 perspective = fmaxf(0.01f, 1.0f + pos.z * 0.5f);

                const f32 half_w = as$(f32, window_res_width) * 0.5f;
                const f32 half_h = as$(f32, window_res_height) * 0.5f;

                const f32 sx = (pos.x * perspective) * half_w + half_w;
                const f32 sy = (-pos.y * perspective) * half_h + half_h;

                const i32 screen_x = lroundf(sx);
                const i32 screen_y = lroundf(sy);
                // const f32 perspective = prim_max(0.01f, 1.0f + pos.z * 0.5f);
                // const i32 screen_x    = lroundf((pos.x * perspective + 0.5f) * (window_res_width - 20) + 10);
                // const i32 screen_y    = lroundf((-pos.y * perspective + 0.5f) * (window_res_height - 1) + 2);

                if (screen_x < 0 || window_res_width <= screen_x) { continue; }
                if (screen_y < 0 || window_res_height <= screen_y) { continue; }

                scope_if(
                    let cell = Mat_at(z_buffer, screen_x, screen_y),
                    *cell < pos.z
                ) {
                    *cell = pos.z;
                    engine_Canvas_drawPixel(
                        buffer->canvas,
                        screen_x,
                        screen_y,
                        calculateHeartLighting(
                            transformed_normal,
                            view_pos,
                            pos,
                            color
                        )
                    );
                }
            }
        }
    }

    // for next frame
    // Update rotation angles for smooth 360-degree rotation
    // Keep angles in range [0, 2π]
    angle.x = math_wrap(angle.x + 0.02f, 0, math_tau);
    angle.y = math_wrap(angle.y + 0.03f, 0, math_tau);
    angle.z = math_wrap(angle.z + 0.01f, 0, math_tau);
}

void flipCanvasBuffer(engine_Canvas* canvas) {
    let       sli         = Sli_asNamed$(Sli$Color, canvas->buffer);
    let       mat         = Mat_fromSli$(Mat$Color, sli, canvas->width, canvas->height);
    const i32 width       = as$(i32, canvas->width);
    const i32 height      = as$(i32, canvas->height);
    const i32 half_height = height / 2;

    for (i32 y = 0; y < half_height; ++y) {
        for (i32 x = 0; x < width; ++x) {
            const i32 top_y    = y;
            const i32 bottom_y = height - 1 - y;

            prim_swap(
                *Mat_at(mat, x, top_y),
                *Mat_at(mat, x, bottom_y)
            );
        }
    }
}

void flipZBuffer(Mat$f32 z_buffer) {
    const i32 width       = (i32)z_buffer.width;
    const i32 height      = (i32)z_buffer.height;
    const i32 half_height = height / 2;
    for (i32 y = 0; y < half_height; ++y) {
        for (i32 x = 0; x < width; ++x) {
            const i32 top_y    = y;
            const i32 bottom_y = height - 1 - y;
            prim_swap(
                *Mat_at(z_buffer, x, top_y),
                *Mat_at(z_buffer, x, bottom_y)
            );
        }
    }
}

static const char g_ascii_shading[]   = " .,-~:;=!*#$@";
// ^ from dark to bright
static const i32  g_ascii_shading_len = countOf(g_ascii_shading) - 1;

// asciiMat has dimensions [width, height/2]
void asciiFromZBuffer(const RenderBuffer* buffer, Mat$u8 asciiMat) {
    const i32 width  = (i32)buffer->canvas->width;
    const i32 height = (i32)buffer->canvas->height;

    // 1) Find max Z so we can normalize
    f32 max_z = 0.0f;
    for (usize i = 0; i < buffer->z_buffer.len; ++i) {
        f32 z = buffer->z_buffer.ptr[i];
        // skip negative-infinity or unfilled
        if (max_z < z && -math_f32_inf < z) { max_z = z; }
    }
    if (max_z < 1e-6f) { max_z = 1.0f; }

    // 2) For each pair of rows => 1 ASCII row
    for (i32 y = 0; y < height; y += 2) {
        i32 asciiY = y / 2;

        for (i32 x = 0; x < width; ++x) {
            // top pixel's depth
            f32 zTop = buffer->z_buffer.ptr[x + y * width];
            // bottom pixel's depth, or same if y+1 is out of bounds
            f32 zBot = (y + 1 < height) ? buffer->z_buffer.ptr[x + (y + 1) * width] : zTop;

            // if both are -∞, it means nothing was drawn there => ' '
            if (zTop <= -math_f32_inf && zBot <= -math_f32_inf) {
                *Mat_at(asciiMat, x, asciiY) = ' ';
                continue;
            }

            // pick the max: get whichever is in front
            f32 zVal = fmaxf(zTop, zBot);

            // map zVal -> [0 .. shadingLen-1]
            i32 idx = (i32)((zVal / max_z) * (f32)(g_ascii_shading_len - 1));
            if (idx < 0) {
                idx = 0;
            }
            if (idx >= g_ascii_shading_len) {
                idx = g_ascii_shading_len - 1;
            }

            *Mat_at(asciiMat, x, asciiY) = g_ascii_shading[idx];
        }
    }
}

void printAscii(engine_Platform* platform, const engine_Canvas* canvas, Mat$u8 asciiMat) {
    let backend = (engine_Win32ConsoleBackend*)platform->backend;
    let pixels  = Mat_fromSli$(
        Mat$Color,
        Sli_asNamed$(Sli$Color, canvas->buffer),
        as$(u32, canvas->width),
        as$(u32, canvas->height)
    );

    /* Reset buffer */ {
        backend->buffer_size = 0;
    }

    // Optimized text path - no need to check for runs
    // Each cell is processed individually since it's at character resolution
    for (u32 y = 0; y < pixels.height; y += 2) {
        for (u32 x = 0; x < pixels.width; ++x) {
            let ch = *Mat_at(asciiMat, x, y / 2);
            backend->buffer_size += sprintf(
                backend->buffer + backend->buffer_size,
                "\033[40;37m%c",
                ch ? ch : ' ' // Space for empty cells
            );
        }
        backend->buffer[backend->buffer_size++] = '\n';
    }
    backend->buffer[--backend->buffer_size] = '\0';
    /* Reset cursor position */ {
        static const char* const reset_cursor = "\033[H";
        printf("%s", reset_cursor);
    }
    /* Write to console */ {
        DWORD written = 0;
        WriteConsoleA(
            backend->output_handle,
            backend->buffer,
            (DWORD)backend->buffer_size,
            &written,
            null
        );
    }
}

// Store whether each ASCII cell is "off" (pixel) or "on" (ASCII).
// For 80x25 = 2000 cells if window_res_width=80, window_res_height=50 => 80*(50/2)=2000
static bool ascii_disabled[window_res_size / 2];
static i32  total_cells      = 0; // total ASCII cells
static i32  disabled_count   = 0; // how many we have disabled so far
static bool overlay_complete = false;

static void disableSomeCellsLinear(i32 n) {
    // linear approach: from left->right, top->bottom
    // or keep a static “cursor” from last time we searched
    static i32 cursor = 0;

    i32 found = 0;
    while (found < n) {
        if (ascii_disabled[cursor] == false) {
            ascii_disabled[cursor] = true;
            found++;
        }
        cursor++;
        if (cursor >= total_cells) {
            // we've disabled everything
            break;
        }
    }
}
static i32  disable_cell_order[countOf(ascii_disabled)]; // Will store each cell index in random order
static i32  disable_cursor = 0;                          // "next index to disable" in cellOrder
static void disableSomeCellsRandom(i32 n) {
    i32 found = 0;
    while (found < n && disable_cursor < total_cells) {
        i32 index             = disable_cell_order[disable_cursor++];
        // mark that ASCII cell as disabled
        ascii_disabled[index] = true;
        found++;
    }
}

static void printAsciiWithColor(engine_Platform* platform, const engine_Canvas* canvas, Mat$u8 asciiMat) {
    let backend = (engine_Win32ConsoleBackend*)platform->backend;
    let pixels  = Mat_fromSli$(
        Mat$Color,
        Sli_asNamed$(Sli$Color, canvas->buffer),
        as$(u32, canvas->width),
        as$(u32, canvas->height)
    );
    /* Reset buffer */ {
        backend->buffer_size = 0;
    }
    for (u32 y = 0; y < pixels.height; y += 2) {
        for (u32 x = 0; x < pixels.width; ++x) {
            if (ascii_disabled[x + (y / 2) * pixels.width]) {
                // Standard half-block color mode
                const Color upper = *Mat_at(pixels, x, y);
                const Color lower = *Mat_at(pixels, x, (y + 1));
                backend->buffer_size += sprintf(
                    backend->buffer + backend->buffer_size,
                    "\033[38;2;%d;%d;%d;48;2;%d;%d;%dm▀",
                    upper.r,
                    upper.g,
                    upper.b,
                    lower.r,
                    lower.g,
                    lower.b
                );
            } else {
                // Character mode
                let ch = *Mat_at(asciiMat, x, y / 2);
                backend->buffer_size += sprintf(
                    backend->buffer + backend->buffer_size,
                    "\033[38;2;255;255;255;48;2;0;0;0m%c",
                    ch ? ch : ' ' // Space for empty cells
                );
            }
        }
        backend->buffer[backend->buffer_size++] = '\n';
    }
    backend->buffer[--backend->buffer_size] = '\0';
    /* Reset cursor position */ {
        static const char* const reset_cursor = "\033[H";
        printf("%s", reset_cursor);
    }
    /* Write to console */ {
        DWORD written = 0;
        WriteConsoleA(
            backend->output_handle,
            backend->buffer,
            (DWORD)backend->buffer_size,
            &written,
            null
        );
    }
}

// measureKoreanTextWidth: returns the total pixel width of the text at the chosen font size
// Using the same logic as renderKoreanText but skipping the drawing.
must_check i32 measureKoreanTextWidth(Str_const text, usize size_pixels) {
    // Use a temp font here. We re-init each time for simplicity.
    // If performance is a concern, cache the font + glyphs.
    var allocator   = heap_Classic_allocator(&(heap_Classic){});
    i32 total_width = 0;

    if_ok(FontSystem_init(allocator, "assets/Galmuri11-Bold.ttf", (f32)size_pixels), font) {
        // Iterate codepoints
        StrUtf8Iter iter = StrUtf8_iter(text);
        Opt$u32     cp   = { 0 };

        while (StrUtf8Iter_next(&iter, &cp)) {
            if (isNone(cp)) {
                continue;
            }
            i32 codepoint = (i32)cp.value;

            // Render glyph only to measure
            if_ok(FontSystem_renderGlyph(&font, codepoint, 1.0f), glyph) {
                total_width += glyph.advance;
                // clean glyph memory
                mem_Allocator_free(allocator, anySli(glyph.bitmap.items));
            }
        }
        // finish
        FontSystem_fini((void*)&font);
    }

    return total_width;
}

static f32 fadeInFactor(f32 t, f32 start, f32 duration) {
    if (t < start) {
        return 0.0f;
    }
    f32 end = start + duration;
    if (t > end) {
        return 1.0f;
    }
    f32 ratio = (t - start) / duration;
    if (ratio < 0.0f) {
        ratio = 0.0f;
    }
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    return ratio;
}

must_check Err$void renderKoreanTextFade(
    Str_const      text,
    engine_Canvas* canvas,
    i32            x,
    i32            y,
    usize          size_pixels,
    f32            alpha_factor // in [0..1]
) {
    scope_reserveReturn(Err$void) {
        var        allocator = heap_Classic_allocator(&(heap_Classic){});
        // 1) init font
        KoreanFont font      = try(FontSystem_init(allocator, "assets/Galmuri11.ttf", (f32)size_pixels));
        defer(FontSystem_fini(&font));

        i32 cursor_x = x;
        i32 cursor_y = y;

        // 2) codepoint iteration
        StrUtf8Iter iter = StrUtf8_iter(text);
        Opt$u32     cp   = { 0 };
        while (StrUtf8Iter_next(&iter, &cp)) {
            if (isNone(cp)) { continue; }
            i32 codepoint = (i32)cp.value;

            scope_with(let glyph = try(FontSystem_renderGlyph(&font, codepoint, 1.0f))) {
                // We'll draw in e.g. white * alpha
                Color c = Color_from(
                    255,
                    255,
                    255,
                    (u8)(255.0f * alpha_factor)
                );
                FontSystem_renderGlyphToCanvas(
                    &glyph,
                    canvas,
                    cursor_x,
                    cursor_y,
                    c
                );
                cursor_x += glyph.advance;
                mem_Allocator_free(allocator, anySli(glyph.bitmap.items));
            }
        }
        return_ok({});
    }
    scope_returnReserved;
}

Err$void renderMessagesIfTimeAllows(f32 t, engine_Canvas* canvas) {
    reserveReturn(Err$void);

    // 1) Check if it’s time to do fade in for msg_1st
    f32 factor1 = fadeInFactor(
        t,
        firework_with_msg_1st_printing_fade_in_begin,
        firework_with_msg_1st_printing_fade_in_duration
    );
    // After fade in, msg_1st is fully visible for up to (firework_with_msg_1st_on_time)

    // 2) Check if it’s time to do fade in for msg_2nd
    f32 factor2 = fadeInFactor(
        t,
        msg_1st_next_2st_printing_fade_in_begin,
        msg_1st_next_2st_printing_fade_in_duration
    );

    if (factor1 > 0.0f) {
        // We want alpha= factor1
        // measure text
        i32 w1 = measureKoreanTextWidth(msg_1st, font_size);
        // center
        i32 x1 = (i32)(canvas->width - w1) / 2;
        i32 y1 = (i32)((f32)canvas->height * 0.35f); // pick some Y near top or middle

        // render with alpha factor1
        // We'll use a version of renderKoreanText that can do alpha
        try(renderKoreanTextFade(msg_1st, canvas, x1, y1, font_size, factor1));
    }

    if (factor2 > 0.0f) {
        // measure text
        i32 w2 = measureKoreanTextWidth(msg_2nd, font_size);
        i32 x2 = (i32)(canvas->width - w2) / 2;
        // place below or above the first message
        i32 y2 = (i32)((f32)canvas->height * 0.5f);

        try(renderKoreanTextFade(msg_2nd, canvas, x2, y2 + font_size / 2, font_size, factor2));
    }

    return_ok({});
}


/*========== main ===========================================================*/

Err$void dh_main(i32 argc, const char* argv[]) { // NOLINT
    unused(argc), unused(argv);
    Random_init();
    scope_reserveReturn(Err$void) {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("main-debug.log", "w"), debug_file) {
            log_initWithFile(debug_file);
            // Configure logging behavior
            log_setLevel(log_Level_debug);
            log_showTimestamp(true);
            log_showLevel(true);
            log_showLocation(false);
            log_showFunction(true);
        }
        defer(log_fini());

        // Initialize platform with terminal backend
        let window = try(engine_Window_create(
            &(engine_PlatformParams){
                .backend_type  = engine_RenderBackendType_vt100,
                .window_title  = "Heart 3D",
                .width         = window_res_width,
                .height        = window_res_height,
                .default_color = Color_black,
            }
        ));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        // Create canvases
        let game_canvas = catch (
            engine_Canvas_create(
                window_res_width,
                window_res_height,
                engine_CanvasType_rgba
            ),
            err,
            {
                log_error("Failed to create canvas: %s\n", err);
                return_err(err);
            }
        );
        defer(engine_Canvas_destroy(game_canvas));
        let text_canvas = catch (
            engine_Canvas_create(
                window_res_width,
                window_res_height,
                engine_CanvasType_rgba
            ),
            err,
            {
                log_error("Failed to create canvas: %s\n", err);
                return_err(err);
            }
        );
        defer(engine_Canvas_destroy(text_canvas));
        log_info("canvas created\n");

        engine_Canvas_clearDefault(game_canvas);
        engine_Canvas_clearDefault(text_canvas);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, window_res_width, window_res_height);
        engine_Window_addCanvasView(window, text_canvas, 0, 0, window_res_width, window_res_height);
        log_info("canvas views added\n");

        // Create render buffer with engine canvas
        var allocator = heap_Classic_allocator(&(heap_Classic){});
        var buffer    = (RenderBuffer){
               .canvas   = game_canvas,
               .z_buffer = meta_cast$(Sli$f32, try(mem_Allocator_alloc(allocator, typeInfo(f32), window_res_size)))
        };
        defer(mem_Allocator_free(allocator, anySli(buffer.z_buffer)));
        // memset(buffer.z_buffer.ptr, 0, window_res_size * sizeof(f32));
        var overlay = (CanvasAscii){
            .color = buffer.canvas,
            .ascii = Mat_fromSli$(
                Mat$u8,
                meta_cast$(Sli$u8, try(mem_Allocator_alloc(allocator, typeInfo(u8), window_res_size / 2))),
                window_res_width,
                window_res_height / 2
            ),
        };
        defer(mem_Allocator_free(allocator, anySli(overlay.ascii.items)));
        // memset(overlay.ascii.items.ptr, 0, window_res_size / 2);

        memset(ascii_disabled, 0, sizeof(ascii_disabled));
        total_cells = (i32)(window_res_size / 2);
        // Fill cellOrder with 0..(totalCells-1)
        for (i32 i = 0; i < total_cells; ++i) {
            disable_cell_order[i] = i;
        }
        // Fisher-Yates shuffle (or any shuffle)
        for (i32 i = total_cells - 1; i > 0; --i) {
            i32 j = rand() % (i + 1);
            // swap cellOrder[i], cellOrder[j]
            prim_swap(disable_cell_order[i], disable_cell_order[j]);
        }
        disable_cursor   = 0; // start at beginning
        disabled_count   = 0;
        overlay_complete = false;
        log_info("game state created\n");
        ignore getchar();

        // Initialize timing variables
        let time_frame_target = time_Duration_fromSecs_f64(target_spf);
        var time_frame_prev   = time_Instant_now();
        f32 time_total        = 0.0f;
        log_info("game loop started\n");

        bool is_running = true;
        while (is_running) {
            // 1) Capture the start of the frame (and capture the end-of-frame time of prev iteration’s dt includes sleep)
            let time_frame_curr = time_Instant_now();

            // 2) Compute how long since last frame
            let time_elapsed = time_Instant_durationSince(time_frame_curr, time_frame_prev);
            let time_dt      = as$(f32, time_Duration_asSecs_f64(time_elapsed));

            // 3) Process input/events
            try(engine_Window_processEvents(window));

            // 4) Update game state
            if (engine_Key_pressed(engine_KeyCode_esc)) {
                log_debug("esc pressed\n");
                is_running = false;
            }
            // time_total += 0.003f;
            time_total += time_dt;

            /* Overlay ascii */ {
                if (!overlay_complete) {
                    let t = time_total;
                    if (t < ascii_on_time) {
                        // do nothing; all ASCII cells are enabled
                    } else {
                        // in the transition window:
                        // e.g. from time=3s to time=5s if transition_time=2.
                        f32 time_phase = t - ascii_on_time;

                        // clamp phaseTime to [0, transition_time]
                        if (time_phase < 0.0f) { time_phase = 0.0f; }
                        if (time_phase > ascii_to_pixel_transition_duration) { time_phase = ascii_to_pixel_transition_duration; }

                        // "progress" in [0..1], where 0 means "no cells disabled"
                        // and 1 means "all cells disabled"
                        f32 progress = time_phase / ascii_to_pixel_transition_duration;

                        // For slow->fast acceleration, square it:
                        f32 accel = progress * progress;

                        // The desired total number of disabled cells by now:
                        i32 desired_disabled = (i32)(accel * (f32)total_cells);
                        // How many new cells disable this frame
                        i32 new_disabled     = desired_disabled - disabled_count;
                        if (new_disabled < 0) {
                            new_disabled = 0; // should never happen if logic is correct
                        }

                        // If need to disable more cells, do it
                        if (new_disabled > 0) {
                            disableSomeCellsRandom(new_disabled);
                            disabled_count += new_disabled;
                        }

                        // If progress==1, hit the end of the transition:
                        if (progress >= 1.0f) {
                            overlay_complete = true;
                            log_debug("overlay complete\n");
                        }
                    }
                }
            }

            if (time_total < pink_to_firework_transition_end) {
                // 5) Render all views
                /* Transition color */ {
                    // top of the frame
                    let t = time_total;

                    // compute colorFactor
                    f32 color_factor = 0.0f;
                    if (t > red_to_pink_transition_start) {
                        f32 d = (t - red_to_pink_transition_start) / red_to_pink_transition_duration;
                        if (d < 0.0f) { d = 0.0f; }
                        if (d > 1.0f) { d = 1.0f; }
                        color_factor = d;
                    }

                    // build the actual colors
                    let color_bg       = Color_lerp(color_bg_normal, color_bg_white, color_factor);
                    let color_fg       = Color_lerp(color_heart_normal, color_heart_pinkish, color_factor);
                    color_ambient_curr = Color_lerp(color_ambient_begin, color_ambient_end, color_factor);

                    // Clear with the new BG color
                    engine_Canvas_clear(game_canvas, color_bg);

                    // Then either pass "heartCol" into renderHeart or store it in a global, e.g.
                    renderHeart(&buffer, t, color_fg);
                }
                flipCanvasBuffer(game_canvas);
                // Present the pixel canvas
                if (!overlay_complete) {
                    // (A) ASCII overlay path
                    flipZBuffer(Mat_fromSli$(Mat$f32, buffer.z_buffer, window_res_width, window_res_height));
                    asciiFromZBuffer(&buffer, overlay.ascii);
                    printAsciiWithColor(window->platform, game_canvas, overlay.ascii);
                } else {
                    // (B) Pixel path
                    engine_Window_present(window);
                }
            } else {
                if (!fireworks_started) {
                    // Initialize fireworks state
                    state = try(State_init(allocator, window_res_width, window_res_height));
                    defer(State_fini(&state));
                    fireworks_started = true;

                    // Create initial large burst at heart center
                    if_some(try(State_spawnFirework(&state)), firework) {
                        // Position at screen center
                        f64 center_x = window_res_width / 2.0;
                        f64 center_y = window_res_height / 2.0;

                        // Create large initial burst
                        try(Firework_createAt(
                            firework,
                            state.allocator,
                            center_x,
                            center_y,
                            2.0,               // Double size
                            color_heart_normal // Use the heart's pink color
                        ));
                    }
                } else {
                    // Randomly spawn normal-sized fireworks
                    if (Random_f64() < 0.05) { // 5% chance each frame
                        if_some(try(State_spawnFirework(&state)), firework) {
                            // Random position
                            f64 x = Random_f64() * window_res_width;
                            f64 y = window_res_height * 0.7; // Slightly above center

                            // Random color for variety
                            Color color = Color_fromOpaque(
                                Random_u8(),
                                Random_u8(),
                                Random_u8()
                            );

                            try(Firework_createAt(
                                firework,
                                state.allocator,
                                x,
                                y,
                                1.0, // Normal size
                                color
                            ));
                        }
                    }
                }

                // Update and render fireworks
                try(State_update(&state, time_dt));
                engine_Canvas_clear(game_canvas, color_bg_white);
                State_render(&state, game_canvas, time_dt);
                if (pink_to_firework_transition_end < time_total) {
                    renderMessagesIfTimeAllows(time_total, text_canvas);
                }
                engine_Window_present(window);
            }

            // 6) (Optional) Display instantaneous FPS
            const f64 fps = (0.0 < time_dt) ? (1.0 / time_dt) : 9999.0;
            printf("\033[H"); // move cursor to top-left
            printf("\rFPS: %6.2f\n", fps);
            debug_only(
                // log frame every 1s
                static f64 total_game_time_for_timestamp = 0.0;
                static f64 logging_after_duration        = 0.0;
                total_game_time_for_timestamp += time_dt;
                logging_after_duration += time_dt;
                if (1.0 < logging_after_duration) {
                    logging_after_duration = 0.0;
                    log_debug("[t=%6.2f] dt: %6.2f, fps %6.2f\n", total_game_time_for_timestamp, time_dt, 1.0 / time_dt);
                }
            );

            // 7) Measure how long the update+render actually took
            let time_now        = time_Instant_now();
            let time_frame_used = time_Instant_durationSince(time_now, time_frame_curr);

            // 8) Subtract from our target; clamp to zero if negative
            if_some(time_Duration_chkdSub(time_frame_target, time_frame_used), time_leftover) {
                time_sleep(time_leftover);
            }
            time_frame_prev = time_frame_curr;
        }
        return_ok({});
    }
    scope_returnReserved;
}

/*========== Implementation Fireworks =======================================*/

Particle Particle_new(f64 x, f64 y, f64 width, f64 height, Color color) {
    return (Particle){
        .position     = { x, y },
        .speed        = { 0.0, 0.0 },
        .acceleration = { 0.0, 0.0 },
        .fading       = 0.0,
        .lifetime     = 1.0,
        .color        = color,
        .dimensions   = { as$(i64, width), as$(i64, height) }
    };
}

Particle* Particle_init(Particle* p, f64 x, f64 y, f64 width, f64 height, Color color) {
    debug_assert_nonnull(p);
    *p = Particle_new(x, y, width, height, color);
    return p;
}

Particle* Particle_initWithSpeed(Particle* p, f64 x, f64 y) {
    debug_assert_nonnull(p);
    p->speed[0] = x;
    p->speed[1] = y;
    return p;
}

Particle* Particle_initWithAcceleration(Particle* p, f64 x, f64 y) {
    debug_assert_nonnull(p);
    p->acceleration[0] = x;
    p->acceleration[1] = y;
    return p;
}

Particle* Particle_initWithFading(Particle* p, f64 fading) {
    debug_assert_nonnull(p);
    p->fading = fading;
    return p;
}

bool Particle_isDead(const Particle* p) {
    debug_assert_nonnull(p);
    return p->lifetime <= 0.0;
}

void Particle_update(Particle* p, f64 dt) {
    debug_assert_nonnull(p);
    unused(dt);
    if (Particle_isDead(p)) { return; }

    p->speed[0] += p->acceleration[0];
    p->speed[1] += p->acceleration[1];

    p->position[0] += p->speed[0];
    p->position[1] += p->speed[1];

    p->lifetime -= p->fading;
}

// Helper function to generate pastel colors for fireworks
Color generatePastelColor(void) {
    // Start with high base values to ensure light colors
    u8 r = math_wrap(Random_u8(), 180u, 255u);
    u8 g = math_wrap(Random_u8(), 180u, 255u);
    u8 b = math_wrap(Random_u8(), 180u, 255u);

    // Ensure at least one channel is very high for vibrancy
    u8 max_channel = prim_max(prim_max(r, g), b);
    if (max_channel < 230) {
        f32 boost = 230.0f / (f32)max_channel;
        r         = as$(u8, prim_min(as$(f32, r) * boost, 255.0f));
        g         = as$(u8, prim_min(as$(f32, g) * boost, 255.0f));
        b         = as$(u8, prim_min(as$(f32, b) * boost, 255.0f));
    }

    return Color_fromOpaque(r, g, b);
}

// Helper to create a color palette based on the heart's pink
Color generateHeartBasedColor(void) {
    // Use heart pink as base
    const Hsl heart_hsl = Color_intoHsl(color_heart_pinkish);

    // Vary hue slightly around pink
    f32 hue_variation = (f32)((Random_f64() - 0.5) * 30.0); // ±15 degrees
    f32 new_hue       = math_wrap(heart_hsl.h + hue_variation, 0.0f, 360.0f);

    // Keep high saturation but vary it slightly
    f32 new_sat = math_clamp(heart_hsl.s + (Random_f64() - 0.5) * 20.0, 40.0f, 100.0f);

    // Keep high lightness
    f32 new_light = math_clamp(70.0f + Random_f64() * 30.0f, 70.0f, 95.0f);

    return Hsl_intoColorOpaque(Hsl_from(new_hue, new_sat, new_light));
}

// Modify Particle_render to use screen blending
void Particle_render(const Particle* p, engine_Canvas* c, f64 dt) {
    debug_assert_nonnull(p);
    debug_assert_nonnull(c);
    unused(dt);
    if (Particle_isDead(p)) { return; }

    // Alpha based on lifetime with smoother fade
    f32 alpha = as$(f32, math_pow(p->lifetime, 1.5));

    let x1 = as$(i32, p->position[0]);
    let y1 = as$(i32, p->position[1]);
    let x2 = as$(i32, p->position[0] + p->dimensions[0] - 1);
    let y2 = as$(i32, p->position[1] + p->dimensions[1] - 1);

    for (i32 y = y1; y <= y2; ++y) {
        for (i32 x = x1; x <= x2; ++x) {
            // Screen blend with background
            Color bg = color_bg_white; // Using white background
            Color fg = p->color;

            var blended = Color_lerp(fg, bg, (f32)p->lifetime);
            blended.a   = 192;

            // Blend with alpha
            Color final = Color_lerp(bg, blended, alpha);
            engine_Canvas_drawPixel(c, x, y, final);
        }
    }
}

Err$Ptr$Firework Firework_init(Firework* f, mem_Allocator allocator, i64 rocket_x, i64 rocket_y, Color effect_base_color) {
    scope_reserveReturn(Err$Ptr$Firework) {
        debug_assert_nonnull(f);

        log_debug("Initializing firework(%p) at (%d, %d)\n", f, rocket_x, rocket_y);
        f->allocator = allocator;

        scope_with(let rocket = meta_castPtr$(Particle*, try(mem_Allocator_create(f->allocator, typeInfo(Particle))))) {
            errdefer(mem_Allocator_destroy(f->allocator, anyPtr(rocket)));
            Particle_init(rocket, as$(f64, rocket_x), as$(f64, rocket_y), 1.0, 3.0, effect_base_color);
            Particle_initWithSpeed(rocket, 0.0, -2.0 - Random_f64() * -1.0);
            Particle_initWithAcceleration(rocket, 0.0, 0.02);
            assignSome(f->rocket, rocket);
        }

        f->effects = typed(ArrList$Particle, try(ArrList_initCap(typeInfo(Particle), f->allocator, Firework_effects_per_rocket)));
        errdefer(ArrList_fini(&f->effects.base));

        f->effect_base_color = Color_intoHsl(effect_base_color);

        return_ok(f);
    }
    scope_returnReserved;
}

void Firework_fini(Firework* f) {
    debug_assert_nonnull(f);

    log_debug("Destroying firework(%p)\n", f);

    if_some(f->rocket, rocket) {
        log_debug("Destroying rocket(%p)\n", rocket);
        mem_Allocator_destroy(f->allocator, anyPtr(rocket));
        log_debug("rocket destroyed\n");
    }

    log_debug("Destroying effects(%p)\n", f->effects.items);
    ArrList_fini(&f->effects.base);
    log_debug("effects destroyed\n");

    log_debug("firework destroyed\n");
}

static bool Firework__deadsAllEffect(const Firework* f) {
    debug_assert_nonnull(f);

    for_slice(f->effects.items, effect) {
        if (Particle_isDead(effect)) { continue; }
        return false;
    }
    return true;
}

bool Firework_isDead(const Firework* f) {
    debug_assert_nonnull(f);
    if (f->rocket.has_value) { return false; }
    /* NOTE: Why isn't dead firework removed? (deallocate memory)
        Looking at the log, when fireworks are removed:
            1. Dead fireworks should be removed in `State_spawnFirework`
            2. The log shows firework count reaching and staying at 16 (max capacity)
            3. Even after destroying rockets and effects, the firework objects remain
        The issue is in State_spawnFirework.
        `Firework_isDead` returns false when it should return true.
        The condition `f->effects.items.len != 0` prevents removal even when all effects are dead.
        It should only check if the effects are dead through `Firework__deadsAllEffect`, not the array length.
        This is because effects are being used like a memory pool, not reallocated each time, and are never destroyed.
     */
    // if (f->effects.items.len != 0) { return false; }
    if (!Firework__deadsAllEffect(f)) { return false; }
    return true;
}

Err$void Firework_update(Firework* f, f64 dt) { // NOLINT
    reserveReturn(Err$void);
    debug_assert_nonnull(f);
    unused(dt);

    if_some(f->rocket, rocket) {
        Particle_update(rocket, dt);
        if (-0.2 <= rocket->speed[1]) {
            log_debug(
                "Spawning %d effects at (%.2f, %.2f)",
                Firework_effects_per_rocket,
                rocket->position[0],
                rocket->position[1]
            );
            for (i64 i = 0; i < Firework_effects_per_rocket; ++i) {
                if (Firework_effects_max <= f->effects.items.len) { break; }
                scope_with(let particle = meta_castPtr$(Particle*, try(ArrList_addBackOne(&f->effects.base)))) {
                    let x      = rocket->position[0];
                    let y      = rocket->position[1];
                    let width  = 1.0;
                    let height = 1.0;
                    let color  = Hsl_intoColor(
                        Hsl_from(
                            f->effect_base_color.h,
                            f->effect_base_color.s + (Random_f64() - 0.5) * 20.0,
                            f->effect_base_color.l + (Random_f64() - 0.5) * 40.0
                        ),
                        192
                    );
                    Particle_init(particle, x, y, width, height, color);
                    Particle_initWithSpeed(particle, (Random_f64() - 0.5) * 1.0, (Random_f64() - 0.9) * 1.0);
                    Particle_initWithAcceleration(particle, 0.0, 0.02);
                    Particle_initWithFading(particle, 0.01);
                }
            }
            log_debug("destroying rocket(%p)\n", rocket);
            mem_Allocator_destroy(f->allocator, anyPtr(rocket));
            assignNone(f->rocket);
            log_debug("rocket destroyed\n");
        }
    }
    for_slice(f->effects.items, effect) {
        Particle_update(effect, dt);
    }

    return_void();
}

void Firework_render(const Firework* f, engine_Canvas* c, f64 dt) {
    debug_assert_nonnull(f);
    debug_assert_nonnull(c);

    if_some(f->rocket, rocket) {
        debug_assert_fmt(!Particle_isDead(rocket), "rocket must be alive");
        Particle_render(rocket, c, dt);
    }
    for_slice(f->effects.items, effect) {
        Particle_render(effect, c, dt);
    }
}

Err$State State_init(mem_Allocator allocator, u32 width, u32 height) {
    reserveReturn(Err$State);

    var fireworks = typed(ArrList$Firework, try(ArrList_initCap(typeInfo(Firework), allocator, Fireworks_max)));
    return_ok({
        .fireworks  = fireworks,
        .width      = width,
        .height     = height,
        .allocator  = allocator,
        .is_running = true,
    });
}

void State_fini(State* s) {
    debug_assert_nonnull(s);

    for_slice(s->fireworks.items, firework) {
        Firework_fini(firework);
    }
    ArrList_fini(&s->fireworks.base);
}

bool State_isDead(const State* s) {
    debug_assert_nonnull(s);
    return !s->is_running;
}

Err$void State_update(State* s, f64 dt) {
    reserveReturn(Err$void);
    debug_assert_nonnull(s);

    /* Add a new rocket with with 5% chance */
    if (Random_f64() < 0.05) {
        /* NOTE: Rule #0 - Don't put code that causes side effects in debug code like assertions
            What a stupid mistake...
            This explains why the fireworks were automatically triggered in debug mode,
            but not in release mode.
         */
        // debug_only(
        if_some(try(State_spawnFirework(s)), firework) {
            let rocket = unwrap(firework->rocket);
            log_debug("Spawning rocket at (%.2f, %.2f)", rocket->position[0], rocket->position[1]);
        }
        // );
    }

    /* Update all fireworks */
    for_slice(s->fireworks.items, firework) {
        try(Firework_update(firework, dt));
    }

    // Input handling
    if (engine_Key_pressed(engine_KeyCode_esc)) {
        log_debug("pressed esc\n");
        s->is_running = false;
        return_void();
    }

    if (engine_Key_pressed(engine_KeyCode_space)) {
        log_debug("pressed space\n");
        let maybe_firework = catch (State_spawnFirework(s), err, {
            log_error("failed to spawn firework: %s\n", Err_codeToCStr(err));
            return_err(err);
        });
        if_some(maybe_firework, firework) {
            let rocket = unwrap(firework->rocket);
            log_debug("Spawning rocket at (%.2f, %.2f)", rocket->position[0], rocket->position[1]);
        }
    }

    if (engine_Mouse_pressed(engine_MouseButton_left)) {
        log_debug("pressed left mouse button\n");
        let maybe_firework = catch (State_spawnFirework(s), err, {
            log_error("failed to spawn firework: %s\n", Err_codeToCStr(err));
            return_err(err);
        });
        if_some(maybe_firework, firework) {
            let rocket    = unwrap(firework->rocket);
            let mouse_pos = engine_Mouse_getPosition();
            log_debug("Spawning rocket at (%.2f, %.2f)", rocket->position[0], rocket->position[1]);
            Particle_init(rocket, mouse_pos.x, mouse_pos.y, 1, 1, rocket->color);
            Particle_initWithFading(rocket, 0.0);
            try(Firework_update(firework, 0.0));
        }
    }

    return_void();
}

void State_render(const State* s, engine_Canvas* c, f64 dt) {
    debug_assert_nonnull(s);
    debug_assert_nonnull(c);

    for_slice(s->fireworks.items, firework) {
        Firework_render(firework, c, dt);
    }
}

Err$Opt$Ptr$Firework State_spawnFirework(State* s) {
    reserveReturn(Err$Opt$Ptr$Firework);
    debug_assert_nonnull(s);

    log_debug("Spawning new firework...");
    log_debug("%d fireworks remaining: Removing dead fireworks...", s->fireworks.items.len);
    // Remove dead fireworks.
    for (usize index = 0; index < s->fireworks.items.len; ++index) {
        let firework = &s->fireworks.items.ptr[index];
        if (!Firework_isDead(firework)) { continue; }
        let removed = meta_castPtr$(Firework*, ArrList_removeSwap(&s->fireworks.base, index--));
        Firework_fini(removed);
    }
    log_debug("Removed dead fireworks: %d fireworks remaining", s->fireworks.items.len);

    if (s->fireworks.cap <= s->fireworks.items.len) {
        return_ok(none());
    }

    let firework = meta_castPtr$(Firework*, try(ArrList_addBackOne(&s->fireworks.base)));
    return_ok(some(try(Firework_init(
        firework,
        s->allocator,
        as$(i64, Random_u32() % s->width),
        s->height,
        Color_from(
            Random_u8(),
            Random_u8(),
            Random_u8(),
            192
        )
    ))));
}
