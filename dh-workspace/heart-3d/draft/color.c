#include "dh/main.h"
#include "dh/core.h"
#include "dh/log.h"

#include "dh/debug.h"

#include "dh/math/vec.h"
#include "dh/mem/cfg.h"
#include "engine.h"

#define window_res_width__320x200  /* template value */ (320)
#define window_res_height__320x200 /* template value */ (200)
#define window_res_width__160x100  /* template value */ (160)
#define window_res_height__160x100 /* template value */ (100)
#define window_res_width__80x50    /* template value */ (80)
#define window_res_height__80x50   /* template value */ (50)
#define window_res_width__40x25    /* template value */ (40)
#define window_res_height__40x25   /* template value */ (25)

#define window_res_width  (window_res_width__80x50)
#define window_res_height (window_res_height__80x50)
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

#define render_depth_steps (12)
#define render_step_size   (0.01f)

typedef struct RenderBuffer {
    engine_Canvas* canvas;
    f32*           z_buffer;
} RenderBuffer;

// Rotation angles and scaling factor
f32 angleX = 0, angleY = 0, angleZ = 0;
f32 scale          = 1.0f;
f32 scaleDirection = 0.001f;

// Light position and properties
Vec3f lightPos      = literal_math_Vec3f_from(2.0f, 2.0f, -2.0f);
Color heartColor    = literal_Color_fromOpaque(220, 20, 60); // Crimson red
Color specularColor = literal_Color_fromOpaque(255, 255, 255);

// Calculate surface normal for the heart at a given point
Vec3f calculateHeartNormal(f32 x, f32 y, f32 r) {
    unused(r); // Unused parameter

    // Partial derivatives of the heart equation
    f32 dx = -2 * x;
    f32 dy = -2 * (1.2f * y - fabsf(x) * 2.0f / 3.0f) * 1.2f;
    f32 dz = 1.0f;

    return math_Vec3f_norm(math_Vec3f_from(dx, dy, dz));
}

void rotate3D(f32 x, f32 y, f32 z, f32* nx, f32* ny, f32* nz) {
    // Same rotation function as before
    f32 y1 = y * cosf(angleX) - z * sinf(angleX);
    f32 z1 = y * sinf(angleX) + z * cosf(angleX);

    f32 x2 = x * cosf(angleY) - z1 * sinf(angleY);
    f32 z2 = x * sinf(angleY) + z1 * cosf(angleY);

    *nx = x2 * cosf(angleZ) - y1 * sinf(angleZ);
    *ny = x2 * sinf(angleZ) + y1 * cosf(angleZ);
    *nz = z2;
}

// Blend two colors based on factor
Color blendColors(Color a, Color b, f32 factor) {
    return Color_fromOpaque(
        as$(u8, a.r * (1 - factor) + b.r * factor),
        as$(u8, a.g * (1 - factor) + b.g * factor),
        as$(u8, a.b * (1 - factor) + b.b * factor)
    );
}

// Calculate lighting for a point
Color calculateHeartLighting(Vec3f normal, Vec3f viewPos, Vec3f fragPos) {
    // Normalize vectors
    Vec3f lightDir = math_Vec3f_norm(math_Vec3f_from(
        lightPos.x - fragPos.x,
        lightPos.y - fragPos.y,
        lightPos.z - fragPos.z
    ));
    Vec3f viewDir  = math_Vec3f_norm(math_Vec3f_from(
        viewPos.x - fragPos.x,
        viewPos.y - fragPos.y,
        viewPos.z - fragPos.z
    ));

    // Ambient light (constant background illumination)
    f32   ambientStrength = 0.1f;
    Color ambient         = Color_fromOpaque(
        as$(u8, as$(f32, heartColor.r) * ambientStrength),
        as$(u8, as$(f32, heartColor.g) * ambientStrength),
        as$(u8, as$(f32, heartColor.b) * ambientStrength)
    );

    // Calculate light angle
    f32 ndotl = math_Vec3f_dot(normal, lightDir);

    // Early exit if surface faces away from light
    if (ndotl <= 0.0f) {
        return ambient; // Return only ambient light
    }

    // Diffuse lighting
    f32   diff    = ndotl; // We already checked it's positive
    Color diffuse = Color_fromOpaque(
        as$(u8, as$(f32, heartColor.r) * diff),
        as$(u8, as$(f32, heartColor.g) * diff),
        as$(u8, as$(f32, heartColor.b) * diff)
    );

    // Specular lighting using Blinn-Phong model
    Vec3f halfwayDir = math_Vec3f_norm(math_Vec3f_from(
        lightDir.x + viewDir.x,
        lightDir.y + viewDir.y,
        lightDir.z + viewDir.z
    ));

    f32 spec = powf(fmaxf(math_Vec3f_dot(normal, halfwayDir), 0.0f), 64.0f);

    // Modulate specular intensity based on light angle
    spec *= ndotl;

    Color specular = Color_fromOpaque(
        as$(u8, specularColor.r * spec),
        as$(u8, specularColor.g * spec),
        as$(u8, specularColor.b * spec)
    );

    // Combine components
    return Color_fromOpaque(
        as$(u8, math_min(ambient.r + diffuse.r + specular.r, 255)),
        as$(u8, math_min(ambient.g + diffuse.g + specular.g, 255)),
        as$(u8, math_min(ambient.b + diffuse.b + specular.b, 255))
    );
}

// Previous functions remain the same up to renderHeart
void renderHeart(RenderBuffer* buffer, f32 t) {
    // Clear canvas with black color
    engine_Canvas_clearDefault(buffer->canvas);

    // Clear z-buffer
    for (usize i = 0; i < window_res_size; ++i) { buffer->z_buffer[i] = -math_f32_inf; }

    // Update scale with pulsing effect
    scale += scaleDirection;
    if (scale < 0.8f || 1.2f < scale) { scaleDirection = -scaleDirection; }

    Vec3f viewPos = math_Vec3f_from(0.0f, 0.0f, -5.0f);

    // Render the heart
    for (f32 y = -0.5f; y <= 0.5f; y += render_step_size) {
        f32 radius = 0.4f + 0.05f * powf(0.5f + 0.5f * sinf(t * 6.0f + y * 2), 8);

        for (f32 x = -0.5f; x <= 0.5f; x += render_step_size) {
            f32 z = -x * x - powf(1.2f * y - fabsf(x) * 2.0f / 3.0f, 2) + radius * radius;
            if (z < 0) { continue; }
            z = math_sqrt(z) / (2.0f - y);

            Vec3f normal = calculateHeartNormal(x, y, radius);
            for (f32 tz = -z; tz <= z; tz += z / render_depth_steps) {
                f32 sx = x * scale;
                f32 sy = y * scale;
                f32 sz = tz * scale;

                f32 rotX = 0;
                f32 rotY = 0;
                f32 rotZ = 0;
                rotate3D(sx, sy, sz, &rotX, &rotY, &rotZ);

                f32 normalX = 0;
                f32 normalY = 0;
                f32 normalZ = 0;
                rotate3D(normal.x, normal.y, normal.z, &normalX, &normalY, &normalZ);
                Vec3f transformedNormal = math_Vec3f_norm(math_Vec3f_from(normalX, normalY, normalZ));

                f32 perspective = 1.0f + rotZ * 0.5f; // ~ rotZ/2
                i32 screenX     = lroundf(
                    (rotX * perspective + 0.5f) * (window_res_width - 20) + 10
                );
                i32 screenY = lroundf(
                    (-rotY * perspective + 0.5f) * (window_res_height - 1) + 2
                );

                if (0 <= screenX && screenX < window_res_width && 0 <= screenY && screenY < window_res_height) {
                    i32 index = screenX + screenY * window_res_width;
                    if (buffer->z_buffer[index] < rotZ) {
                        buffer->z_buffer[index] = rotZ;

                        Vec3f fragPos    = math_Vec3f_from(rotX, rotY, rotZ);
                        Color pixelColor = calculateHeartLighting(transformedNormal, viewPos, fragPos);

                        // Use the engine's canvas drawing function
                        engine_Canvas_drawPixel(buffer->canvas, screenX, screenY, pixelColor);
                    }
                }
            }
        }
    }
}

Err$void dh_main(int argc, const char* argv[]) {
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
        RenderBuffer buffer = {
            .canvas   = game_canvas,
            .z_buffer = (f32*)malloc(window_res_size * sizeof(f32))
        };
        if (!buffer.z_buffer) {
            log_error("Failed to allocate memory for z-buffer\n");
            return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
        }
        defer(free(buffer.z_buffer));
        log_info("game state created\n");
        ignore getchar();

        f32 t                 = 0;
        // Initialize timing variables
        let time_frame_target = time_Duration_fromSecs_f64(target_spf);
        var time_frame_prev   = time_Instant_now();
        log_info("game loop started\n");

        bool is_running = true;
        while (is_running) {
            // 1) Capture the start of the frame (and capture the end-of-frame time of prev iteration’s dt includes sleep)
            let time_frame_curr = time_Instant_now();

            // 2) Compute how long since last frame (purely for your dt usage)
            let time_elapsed = time_Instant_durationSince(time_frame_curr, time_frame_prev);
            let time_dt      = time_Duration_asSecs_f64(time_elapsed);

            // 3) Process input/events
            try(engine_Window_processEvents(window));

            // 4) Update game state
            if (engine_Key_pressed(engine_KeyCode_esc)) {
                is_running = false;
            }

            // Update rotation angles for smooth 360-degree rotation
            angleX += 0.02f;
            angleY += 0.03f;
            angleZ += 0.01f;

            // Keep angles in range [0, 2π]
            angleX = fmodf(angleX, 2.0f * math_f32_pi);
            angleY = fmodf(angleY, 2.0f * math_f32_pi);
            angleZ = fmodf(angleZ, 2.0f * math_f32_pi);
            t += 0.003f;

            // 5) Render all views
            engine_Canvas_clearDefault(game_canvas);
            renderHeart(&buffer, t);
            engine_Window_present(window);

            // The canvas is already updated through engine_Canvas_drawPixel calls
            // Add any additional engine-specific frame handling here

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
