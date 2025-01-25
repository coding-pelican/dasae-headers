#include "dh/main.h"
#include "dh/core.h"
#include "dh/heap/Classic.h"
#include "dh/log.h"

#include "dh/debug.h"

#include "dh/math/vec.h"
#include "dh/mem/cfg.h"
#include "dh/Mat.h"

#include "engine.h"
#include "engine/platform_backend.h"

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

// #define render_depth_steps (24)     // Increased for smoother rendering
// #define render_step_size   (0.005f) // Smaller steps for higher resolution
#define render_depth_steps   (12)
#define render_step_size     (0.01f)
#define heart_beat_frequency (6.0f)

use_Mat$(u8);
use_Mat$(f32);
use_Sli$(Color);
use_Mat$(Color);

typedef struct RenderBuffer {
    engine_Canvas* canvas;
    Sli$f32        z_buffer;
} RenderBuffer;

typedef struct CanvasAscii {
    const engine_Canvas* color; /* using RenderBuffer's canvas: pixel upper(0,2,4...) as fg, lower(1,3,5...) as bg */
    Mat$u8               ascii;
} CanvasAscii;

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


Err$void dh_main(i32 argc, const char* argv[]) {
    unused(argc), unused(argv);
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
        log_info("canvas created\n");

        engine_Canvas_clearDefault(game_canvas);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, window_res_width, window_res_height);
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
            let time_leftover = time_Duration_sub(time_frame_target, time_frame_used);

            const bool is_positive_time_leftover = 0 < time_leftover.secs_ || 0 < time_leftover.nanos_;
            if (is_positive_time_leftover) {
                time_sleep(time_leftover);
            }
            time_frame_prev = time_frame_curr;
        }
        return_ok({});
    }
    scope_returnReserved;
}
