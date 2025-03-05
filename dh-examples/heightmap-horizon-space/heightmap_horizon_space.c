#include "dh/main.h"
#include "dh/debug.h"
#define log_comp_disabled_not_debug_comp_enabled (1)
#include "dh/log.h"

#include "dh/mem.h"
#include "dh/heap/Classic.h"
#include "dh/ArrList.h"
#include "dh/meta/common.h"
#include "dh/Grid.h"

#include "engine.h"
#include "engine/canvas.h"
#include "engine/input.h"
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

use_Grid$(u8);
use_Sli$(Color);
use_Grid$(Color);

typedef struct TerrainData {
    Grid$u8    heightmap; // grayscale
    Grid$Color colormap;  // RGB format (based RGBA)
    u32        width;
    u32        height;
} TerrainData;
use_Opt$(TerrainData);

config_ErrSet(
    TerrainDataErr,
    FailedToLoadHeightMap,
    FailedToLoadColorMap,
    NotEqualMapsDimension
);
use_ErrSet$(TerrainDataErr, TerrainData);

// Suppose "heightmap.png" is an 8-bit grayscale PNG,
// and "colormap.png" is an 8-bit RGB or RGBA PNG.
TerrainDataErr$TerrainData loadSample(mem_Allocator allocator, const char* heightmap_file, const char* colormap_file) {
    scope_reserveReturn(TerrainDataErr$TerrainData) {
        debug_assert_nonnull(heightmap_file);
        debug_assert_nonnull(colormap_file);

        // 1) Load heightmap (expecting 1 channel, but we can request 1 explicitly)
        i32 width                     = 0;
        i32 height                    = 0;
        i32 heightmap_actual_channels = 0;
        u8* heightmap_data            = stbi_load(heightmap_file, &width, &height, &heightmap_actual_channels, 1); // request 1 channel
        if (!heightmap_data) {
            log_error("Failed to load heightmap: %s\n", heightmap_file);
            return_err(TerrainDataErr_FailedToLoadHeightMap());
        }
        defer(stbi_image_free(heightmap_data));

        printf("Heightmap: %s\n", heightmap_file);
        printf("  Loaded w=%d, h=%d, channels in file=%d, forced to=1\n", width, height, heightmap_actual_channels);
        // top-left pixel in heightmap
        printf("  Top-left gray value = %d\n", heightmap_data[0]);

        let heightmap_sli = meta_castSli$(Sli$u8, try(mem_Allocator_alloc(allocator, typeInfo$(u8), (usize)width * height)));
        errdefer(mem_Allocator_free(allocator, anySli(heightmap_sli)));
        for_slice_indexed(heightmap_sli, item, i) {
            *item = heightmap_data[i];
        }
        let heightmap_mat = Grid_fromSli$(Grid$u8, heightmap_sli, width, height);

        // 2) Load colormap (could be 3 or 4 channels, but we’ll let stb_image decide)
        i32 colormap_channels = 0;
        u8* colormap_data     = stbi_load(colormap_file, &width, &height, &colormap_channels, 0); // 0 means "load as-is"
        if (!colormap_data) {
            log_error("Failed to load colormap: %s\n", colormap_file);
            return_err(TerrainDataErr_err(TerrainDataErrCode_FailedToLoadColorMap));
        }
        defer(stbi_image_free(colormap_data));
        printf("Colormap: %s\n", colormap_file);
        printf("  Loaded w=%d, h=%d, channels=%d\n", width, height, colormap_channels);
        // Print top-left color
        if (colormap_channels == 3) {
            printf("  Top-left color = R=%d, G=%d, B=%d\n", colormap_data[0], colormap_data[1], colormap_data[2]);
        } else if (colormap_channels == 4) {
            printf("  Top-left color = R=%d, G=%d, B=%d, A=%d\n", colormap_data[0], colormap_data[1], colormap_data[2], colormap_data[3]);
        } else {
            printf("  (Unexpected number of channels: %d)\n", colormap_channels);
        }

        let colormap_sli = meta_castSli$(Sli$Color, try(mem_Allocator_alloc(allocator, typeInfo$(Color), (usize)width * height)));
        errdefer(mem_Allocator_free(allocator, anySli(colormap_sli)));
        for_slice_indexed(colormap_sli, item, i) {
            *item = (Color){
                .r = colormap_data[i * colormap_channels + 0],
                .g = colormap_data[i * colormap_channels + 1],
                .b = colormap_data[i * colormap_channels + 2],
                .a = ColorChannel_max_value,
            };
        }
        let colormap_mat = Grid_fromSli$(Grid$Color, colormap_sli, width, height);

        if (heightmap_mat.width != colormap_mat.width || heightmap_mat.height != colormap_mat.height) {
            log_error("Heightmap and colormap dimensions do not match!\n");
            return_err(TerrainDataErr_err(TerrainDataErrCode_NotEqualMapsDimension));
        }
        return_ok({
            .heightmap = heightmap_mat,
            .colormap  = colormap_mat,
            .width     = width,
            .height    = height,
        });
    }
    scope_returnReserved;
}

/* typedef struct Control {
    engine_KeyCode key;
    Vec2f          vec;
} Control;
use_Sli$(Control);

static Sli_const$Control Control_list(void) {
    static const Control controls[] = {
        { .key = engine_KeyCode_W, .vec = math_Vec_up$(Vec2f) },
        { .key = engine_KeyCode_A, .vec = math_Vec_lt$(Vec2f) },
        { .key = engine_KeyCode_S, .vec = math_Vec_dn$(Vec2f) },
        { .key = engine_KeyCode_D, .vec = math_Vec_rt$(Vec2f) },
    };
    return (Sli_const$Control){
        .ptr = controls,
        .len = countOf(controls),
    };
} */

#define window_res_width__320x200  /* template value */ (320)
#define window_res_height__320x200 /* template value */ (200)
#define window_res_width__160x100  /* template value */ (160)
#define window_res_height__160x100 /* template value */ (100)
#define window_res_width__80x50    /* template value */ (80)
#define window_res_height__80x50   /* template value */ (50)
#define window_res_width__40x25    /* template value */ (40)
#define window_res_height__40x25   /* template value */ (25)

#if debug_comp_enabled
#define window_res_width  (window_res_width__80x50)
#define window_res_height (window_res_height__80x50)
#else /* !debug_comp_enabled */
#define window_res_width  (window_res_width__320x200)
#define window_res_height (window_res_height__320x200)
#endif /* debug_comp_enabled */
#define window_res_size (as$(usize, window_res_width) * window_res_height)

/* (1.0 / target_fps__62_50) ~16ms => ~60 FPS, Assume 62.5 FPS for simplicity */
#define target_fps__125_0 /* template value */ (125.0)
#define target_fps__62_50 /* template value */ (62.50)
#define target_fps__50_00 /* template value */ (50.00)
#define target_fps__31_25 /* template value */ (31.25)

#define target_fps (target_fps__125_0)
#define target_spf (1.0 / target_fps)

typedef struct Simulator {
    TerrainData terrain;
    Vec2d       camera_pos;
    f64         camera_angle; // <== We'll use this as 'phi'
    f64         height;       // camera height above ground
    f64         horizon;
    f64         scale_height;
    f64         distance;
    bool        is_running;
    // Input state
    struct {
        f64   forward_backward;
        f64   left_right;
        f64   up_down;
        bool  look_up;
        bool  look_down;
        f64   dist_up_down;
        Vec2i mouse_position;
        bool  mouse_active;
    } input;
} State;

// Input handling
void State_handleInputKey(State* self) {
    // Reset input state
    self->input.forward_backward = 0;
    self->input.left_right       = 0;
    self->input.up_down          = 0;
    self->input.dist_up_down     = 0;

    // WASD/Arrow key movement
    if (engine_Key_held(engine_KeyCode_w) || engine_Key_held(engine_KeyCode_arrow_up)) {
        self->input.forward_backward = 3.0;
    }
    if (engine_Key_held(engine_KeyCode_s) || engine_Key_held(engine_KeyCode_arrow_down)) {
        self->input.forward_backward = -3.0;
    }
    if (engine_Key_held(engine_KeyCode_a) || engine_Key_held(engine_KeyCode_arrow_left)) {
        self->input.left_right = 1.0;
    }
    if (engine_Key_held(engine_KeyCode_d) || engine_Key_held(engine_KeyCode_arrow_right)) {
        self->input.left_right = -1.0;
    }

    // Space/Shift-Space for up/down
    if (engine_Key_held(engine_KeyCode_space)) {
        if (!engine_Key_held(engine_KeyCode_shift)) {
            self->input.up_down = 2.0;
        } else {
            self->input.up_down = -2.0;
        }
    }

    // Q/E for look up/down
    self->input.look_up   = engine_Key_held(engine_KeyCode_q);
    self->input.look_down = engine_Key_held(engine_KeyCode_e);

    // R/F for distance up/down
    if (engine_Key_held(engine_KeyCode_r)) {
        self->input.dist_up_down = 4.0;
    }
    if (engine_Key_held(engine_KeyCode_f)) {
        self->input.dist_up_down = -4.0;
    }

    // ESC to quit
    if (engine_Key_pressed(engine_KeyCode_esc)) {
        self->is_running = false;
    }
}

void State_handleInputMouse(State* state) {
    if (engine_Mouse_isState(engine_MouseButton_left, engine_KeyStates_pressed)) {
        state->input.mouse_active     = true;
        state->input.mouse_position   = engine_Mouse_getPos();
        state->input.forward_backward = 3.0;
    }
    if (engine_Mouse_isState(engine_MouseButton_left, engine_KeyStates_released)) {
        state->input.mouse_active     = false;
        state->input.forward_backward = 0;
        state->input.left_right       = 0;
        state->input.up_down          = 0;
    }
    /* 이걸 어케 바꾼담담 */
    if (state->input.mouse_active) {
        Vec2i delta             = engine_Mouse_getPosDelta();
        state->input.left_right = (f64)delta.x / window_res_width * 2.0;
        state->horizon          = 10.0 + (f64)delta.y / window_res_height * 50.0;
        state->input.up_down    = (f64)delta.y / window_res_height * 10.0;
    }
}

void State_updateCamera(State* self, f64 dt) {
    // movement
    if (self->input.left_right != 0) {
        self->camera_angle += self->input.left_right * 0.1 * dt * 30.0;
    }
    if (self->input.forward_backward != 0) {
        self->camera_pos.x -= self->input.forward_backward * sin(self->camera_angle) * dt * 30.0;
        self->camera_pos.y -= self->input.forward_backward * cos(self->camera_angle) * dt * 30.0;
    }

    // up/down
    if (self->input.up_down != 0) {
        self->height += self->input.up_down * dt * 30.0;
    }

    // look up/down
    if (self->input.look_up) {
        self->horizon += 2.0 * dt * 30.0;
    }
    if (self->input.look_down) {
        self->horizon -= 2.0 * dt * 30.0;
    }

    // distance up/down
    if (self->input.dist_up_down != 0) {
        self->distance += self->input.dist_up_down * dt * 30.0;
    }

    // Collision detection - don't fly below surface
    i32 map_x = (i32)((u32)self->camera_pos.x & (self->terrain.width - 1));
    i32 map_y = (i32)((u32)self->camera_pos.y & (self->terrain.height - 1));

    if (self->height < (*Grid_at(self->terrain.heightmap, map_x, map_y) + 10.0)) {
        self->height = *Grid_at(self->terrain.heightmap, map_x, map_y) + 10.0;
    }
}

// heightmap-based horizon renderer
void State_render(const State* state, engine_Canvas* canvas, f64 dt) {
    debug_assert_nonnull(state);
    debug_assert_nonnull(canvas);
    unused(dt);

    // Extract parameters from state
    let phi          = state->camera_angle;
    let sin_phi      = math_sin(phi);
    let cos_phi      = math_cos(phi);
    let p            = state->camera_pos;
    let height       = state->height;
    let horizon      = state->horizon;
    let scale_height = state->scale_height;
    let distance     = state->distance;

    i32 screen_width  = (i32)canvas->width;
    i32 screen_height = (i32)canvas->height;

    // Get terrain data
    const TerrainData* terrain    = &state->terrain;
    const u8*          heightmap  = terrain->heightmap.items.ptr;
    const Color*       colormap   = terrain->colormap.items.ptr;
    i32                map_width  = (i32)terrain->width;
    i32                map_height = (i32)terrain->height;

    // Draw from back to front (high z to low z)
    for (var z = distance; z > 1.0; z -= 1.0) {
        // Calculate line endpoints on map (90° field of view)
        var p_left = (Vec2d){
            .x = (-cos_phi * z - sin_phi * z) + p.x,
            .y = (sin_phi * z - cos_phi * z) + p.y
        };

        var p_right = (Vec2d){
            .x = (cos_phi * z - sin_phi * z) + p.x,
            .y = (-sin_phi * z - cos_phi * z) + p.y
        };

        // Calculate step sizes for line segment
        let dx = (p_right.x - p_left.x) / (f64)screen_width;
        let dy = (p_right.y - p_left.y) / (f64)screen_width;

        // Raster line and draw vertical line for each segment
        for (i32 i = 0; i < screen_width; ++i) {
            // Convert and clamp coordinates to map bounds
            i32 map_x = (i32)p_left.x;
            i32 map_y = (i32)p_left.y;

            map_x = math_clamp(map_x, 0, map_width - 1);
            map_y = math_clamp(map_y, 0, map_height - 1);

            // Get height and color from maps
            i32   map_index     = map_y * map_width + map_x;
            f64   ground_height = (f64)heightmap[map_index];
            Color color         = colormap[map_index];

            // Calculate height on screen
            f64 height_on_screen = (height - ground_height) / z * scale_height + horizon;

            // Clamp height to screen bounds
            i32 screen_y = (i32)height_on_screen;
            screen_y     = math_clamp(screen_y, 0, screen_height - 1);

            // Draw vertical line
            engine_Canvas_drawVLine(canvas, i, screen_y, screen_height, color);

            // Step to next point on line
            p_left.x += dx;
            p_left.y += dy;
        }
    }
}

Err$void dh_main(Sli$Str_const args) {
    unused(args);
    scope_reserveReturn(Err$void) {
        // Initialize logging to a file
        try(log_init("log/debug.log"));
        {
            defer(log_fini());
            // Configure logging behavior
            log_setLevel(log_Level_debug);
            log_showTimestamp(true);
            log_showLevel(true);
            log_showLocation(false);
            log_showFunction(true);
        }

        // Initialize platform with terminal backend
        let window = try(engine_Window_init(
            &(engine_PlatformParams){
                .backend_type  = engine_RenderBackendType_vt100,
                .window_title  = "Heightmap Horizon Space",
                .width         = window_res_width,
                .height        = window_res_height,
                .default_color = Color_blue,
            }
        ));
        defer(engine_Window_fini(window));
        log_info("engine initialized\n");

        // Create canvases
        let game_canvas = catch (engine_Canvas_create(window_res_width, window_res_height, engine_CanvasType_rgba), err, {
            log_error("Failed to create canvas: %s\n", err);
            return_err(err);
        });
        defer(engine_Canvas_destroy(game_canvas));
        log_info("canvas created\n");

        engine_Canvas_clearDefault(game_canvas);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, window_res_width, window_res_height);
        log_info("canvas views added\n");

        // Initialize rendering with camera parameters
        var   heap      = (heap_Classic){};
        var   allocator = heap_Classic_allocator(&heap);
        State state     = {
                .terrain      = try(loadSample(allocator, "assets/D1.png", "assets/C1W.png")),
                .camera_pos   = { .x = 512, .y = 512 },   // Starting in middle is good if map is 1024x1024
                .camera_angle = 0.0f,                     // Starting angle (looking north)
                .height       = 150.f / 2.0f,             // Height of camera above ground
                .horizon      = window_res_height / 2.0f, // Center of screen
                .scale_height = window_res_height,        // Full screen height for scaling
                .distance     = 300.0f,
                .is_running   = true,
        };
        defer(mem_Allocator_free(allocator, anySli(state.terrain.heightmap.items)));
        defer(mem_Allocator_free(allocator, anySli(state.terrain.colormap.items)));
        log_info("game state created\n");
        ignore getchar();

        let target_frame_time = time_Duration_fromSecs_f64(1.0 / target_fps);
        var prev_frame_time   = time_Instant_now();
        log_info("game loop started\n");

        // Game loop
        while (state.is_running) {
            let curr_frame_time = time_Instant_now();
            let elapsed_time    = time_Instant_durationSince(curr_frame_time, prev_frame_time);
            let dt              = time_Duration_asSecs_f64(elapsed_time);
            prev_frame_time     = curr_frame_time;

            // Process events
            try(engine_Window_processEvents(window));
            State_handleInputKey(&state);
            // State_handleInputMouse(&state);

            // Update game state
            State_updateCamera(&state, dt);

            // Render all views
            engine_Canvas_clearDefault(game_canvas);
            State_render(&state, game_canvas, dt);

            // Present to screen
            engine_Window_present(window);
            const f64 fps = (0.0 < dt) ? (1.0 / dt) : 9999.0;
            printf("\033[H\033[40;37m"); // Move cursor to top left
            printf(
                "\rFPS: %6.2f RES: %dx%d POS: %4d,%4d H: %3d*%02.2f\n",
                fps,
                window_res_width,
                window_res_height,
                (i32)state.camera_pos.x,
                (i32)state.camera_pos.y,
                (i32)state.height,
                state.scale_height
            );
            printf(
                "\rANGLE: %6.2f DIST: %06.2f HORIZ: %06.2f",
                state.camera_angle,
                state.distance,
                state.horizon
            );
            debug_only(
                // log frame every 1s
                static f64 total_game_time_for_timestamp = 0.0;
                static f64 logging_after_duration        = 0.0;
                total_game_time_for_timestamp += dt;
                logging_after_duration += dt;
                if (1.0 < logging_after_duration) {
                    logging_after_duration = 0.0;
                    log_debug("[t=%6.2f] dt: %6.2f, fps %6.2f\n", total_game_time_for_timestamp, dt, 1.0 / dt);
                }
            );

            let now        = time_Instant_now();
            let frame_used = time_Instant_durationSince(now, curr_frame_time);
            if_some(time_Duration_chkdSub(target_frame_time, frame_used), leftover) {
                time_sleep(leftover);
            }
        }
        return_void();
    }
    scope_returnReserved;
}
