#include "dh/main.h"
#include "dh/debug.h"
#include "engine/input.h"
#define LOG_NO_DISABLE_RELEASE (1)
#include "dh/log.h"

#include "dh/mem.h"
#include "dh/heap/Classic.h"
#include "dh/ArrList.h"

#include "engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

typedef struct TerrainData {
    u8*    heightmap; // grayscale
    Color* colormap;  // RGB format
    u32    width;
    u32    height;
} TerrainData;
using_Opt$(TerrainData);

// Suppose "heightmap.png" is an 8-bit grayscale PNG,
// and "colormap.png" is an 8-bit RGB or RGBA PNG.
Opt$TerrainData loadSample(const char* heightmap_file, const char* colormap_file) {
    reserveReturn(Opt$TerrainData);
    debug_assert_nonnull(heightmap_file);
    debug_assert_nonnull(colormap_file);

    // 1) Load heightmap (expecting 1 channel, but we can request 1 explicitly)
    i32 widthH          = 0;
    i32 heightH         = 0;
    i32 actualChannelsH = 0;
    u8* heightmapData   = stbi_load(heightmap_file, &widthH, &heightH, &actualChannelsH, 1); // request 1 channel
    if (!heightmapData) {
        log_error("Failed to load heightmap: %s\n", heightmap_file);
        return_none();
    }
    printf("Heightmap: %s\n", heightmap_file);
    printf("  Loaded w=%d, h=%d, channels in file=%d, forced to=1\n", widthH, heightH, actualChannelsH);
    // top-left pixel in heightmap
    printf("  Top-left gray value = %d\n", heightmapData[0]);

    // 2) Load colormap (could be 3 or 4 channels, but we’ll let stb_image decide)
    i32 widthC       = 0;
    i32 heightC      = 0;
    i32 channelsC    = 0;
    u8* colormapData = stbi_load(colormap_file, &widthC, &heightC, &channelsC, 0); // 0 means "load as-is"
    if (!colormapData) {
        log_error("Failed to load colormap: %s\n", colormap_file);
        stbi_image_free(heightmapData);
        return_none();
    }
    printf("Colormap: %s\n", colormap_file);
    printf("  Loaded w=%d, h=%d, channels=%d\n", widthC, heightC, channelsC);
    // Print top-left color
    if (channelsC == 3) {
        printf("  Top-left color = R=%d, G=%d, B=%d\n", colormapData[0], colormapData[1], colormapData[2]);
    } else if (channelsC == 4) {
        printf("  Top-left color = R=%d, G=%d, B=%d, A=%d\n", colormapData[0], colormapData[1], colormapData[2], colormapData[3]);
    } else {
        printf("  (Unexpected number of channels: %d)\n", channelsC);
    }

    debug_assert(widthH == widthC);
    debug_assert(heightH == heightC);
    let rgba_buffer = (Color*)malloc(sizeof(Color) * widthC * heightC);
    for (usize i = 0; i < ((usize)widthC * heightC); ++i) {
        rgba_buffer[i] = (Color){
            .r = colormapData[i * 3 + 0],
            .g = colormapData[i * 3 + 1],
            .b = colormapData[i * 3 + 2],
            .a = ColorChannel_max_value,
        };
    }
    stbi_image_free(colormapData);

    return_some({
        .heightmap = heightmapData,
        .colormap  = rgba_buffer,
        .width     = widthH,
        .height    = heightH,
    });
}

typedef struct Control {
    engine_KeyCode key;
    Vec2f          vec;
} Control;
using_Sli$(Control);

static SliConst$Control Control_list(void) {
    static const Control controls[] = {
        { .key = engine_KeyCode_W, .vec = math_Vec_up$(Vec2f) },
        { .key = engine_KeyCode_A, .vec = math_Vec_lt$(Vec2f) },
        { .key = engine_KeyCode_S, .vec = math_Vec_dn$(Vec2f) },
        { .key = engine_KeyCode_D, .vec = math_Vec_rt$(Vec2f) },
    };
    return (SliConst$Control){
        .ptr = controls,
        .len = countOf(controls),
    };
}

#define window_width  (80)
#define window_height (50)

typedef struct State {
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
    if (engine_Key_held(engine_KeyCode_W) || engine_Key_held(engine_KeyCode_ArrowUp)) {
        self->input.forward_backward = 3.0;
    }
    if (engine_Key_held(engine_KeyCode_S) || engine_Key_held(engine_KeyCode_ArrowDn)) {
        self->input.forward_backward = -3.0;
    }
    if (engine_Key_held(engine_KeyCode_A) || engine_Key_held(engine_KeyCode_ArrowLt)) {
        self->input.left_right = 1.0;
    }
    if (engine_Key_held(engine_KeyCode_D) || engine_Key_held(engine_KeyCode_ArrowRt)) {
        self->input.left_right = -1.0;
    }

    // Space/Shift-Space for up/down
    if (engine_Key_held(engine_KeyCode_Space)) {
        if (!engine_Key_held(engine_KeyCode_Shift)) {
            self->input.up_down = 2.0;
        } else {
            self->input.up_down = -2.0;
        }
    }

    // Q/E for look up/down
    self->input.look_up   = engine_Key_held(engine_KeyCode_Q);
    self->input.look_down = engine_Key_held(engine_KeyCode_E);

    // R/F for distance up/down
    if (engine_Key_held(engine_KeyCode_R)) {
        self->input.dist_up_down = 4.0;
    }
    if (engine_Key_held(engine_KeyCode_F)) {
        self->input.dist_up_down = -4.0;
    }

    // ESC to quit
    if (engine_Key_pressed(engine_KeyCode_Esc)) {
        self->is_running = false;
    }
}

void State_handleInputMouse(State* state) {
    if (engine_Mouse_isState(engine_MouseButton_Left, engine_KeyStates_pressed)) {
        state->input.mouse_active     = true;
        state->input.mouse_position   = engine_Mouse_getPosition();
        state->input.forward_backward = 3.0;
    }
    if (engine_Mouse_isState(engine_MouseButton_Left, engine_KeyStates_released)) {
        state->input.mouse_active     = false;
        state->input.forward_backward = 0;
        state->input.left_right       = 0;
        state->input.up_down          = 0;
    }
    /* 이걸 어케 바꾼담담 */
    if (state->input.mouse_active) {
        Vec2i delta             = engine_Mouse_getDelta();
        state->input.left_right = (f64)delta.x / window_width * 2.0;
        state->horizon          = 10.0 + (f64)delta.y / window_height * 50.0;
        state->input.up_down    = (f64)delta.y / window_height * 10.0;
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
    i32 map_x      = (i32)((u32)self->camera_pos.x & (self->terrain.width - 1));
    i32 map_y      = (i32)((u32)self->camera_pos.y & (self->terrain.height - 1));
    i32 map_offset = map_y * (i32)self->terrain.width + map_x;

    if (self->height < (self->terrain.heightmap[map_offset] + 10.0)) {
        self->height = self->terrain.heightmap[map_offset] + 10.0;
    }
}

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
    const u8*          heightmap  = terrain->heightmap;
    const Color*       colormap   = terrain->colormap;
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

Err$void dh_main(i32 argc, const char* argv[]) {
    reserveReturn(Err$void);
    unused(argc), unused(argv);

    scope_defer {
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
        let window = try_defer(engine_Window_create(
            &(engine_PlatformParams){
                .backend_type = engine_RenderBackendType_vt100,
                .window_title = "Voxel Space",
                .width        = window_width,
                .height       = window_height,
            }
        ));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        // Create canvases
        let game_canvas = catch (engine_Canvas_create(window_width, window_height, engine_CanvasType_rgba), err, {
            log_error("Failed to create canvas: %s\n", err);
            defer_return_err(err);
        });
        defer(engine_Canvas_destroy(game_canvas));
        log_info("canvas created\n");

        engine_Canvas_clear(game_canvas, Color_black);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, window_width, window_height);
        log_info("canvas views added\n");

        // Initialize rendering with camera parameters
        State state = {
            .terrain      = unwrap(loadSample("assets/D1.png", "assets/C1W.png")),
            .camera_pos   = { .x = 512, .y = 512 }, // Starting in middle is good if map is 1024x1024
            .camera_angle = 0.0f,                   // Starting angle (looking north)
            .height       = 150.f / 2.0f,           // Height of camera above ground
            .horizon      = window_height / 2.0f,   // Center of screen
            .scale_height = window_height,          // Full screen height for scaling
            .distance     = 300.0f,
            .is_running   = true,
        };
        defer(stbi_image_free(state.terrain.heightmap));
        defer(free(state.terrain.colormap));
        // var heap = (heap_Classic){};
        // var allocator = heap_Classic_allocator(&heap);
        log_info("game state created\n");
        ignore getchar();

        var curr_time   = time_Instant_now();
        var prev_time   = curr_time;
        let target_time = time_Duration_fromSecs_f64(0.016f); // Assume 62.5 FPS for simplicity
        log_info("game loop started\n");

        // Game loop
        while (state.is_running) {
            curr_time        = time_Instant_now();
            let elapsed_time = time_Instant_durationSince(curr_time, prev_time);
            let dt           = time_Duration_asSecs_f64(elapsed_time);

            // Process events
            try_defer(engine_Window_processEvents(window));
            State_handleInputKey(&state);
            // State_handleInputMouse(&state);

            // Update game state
            State_updateCamera(&state, dt);

            // Render all views
            engine_Canvas_clear(game_canvas, Color_blank);
            State_render(&state, game_canvas, dt);

            // Present to screen
            engine_Window_present(window);
            printf("\033[A"); // Move cursor up one line
            printf("\033[A"); // Move cursor up one line
            printf(
                "\rFPS: %6.2f RES: %dx%d POS: %4d,%4d H: %3d*%02.2f\n",
                1.0f / dt,
                window_width,
                window_height,
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

            // Sleep for the remaining time to maintain FPS
            time_SysTime_sleep(time_Duration_sub(target_time, elapsed_time));
            prev_time = curr_time;
        }
        defer_return_void();
    }
    return_deferred;
}
