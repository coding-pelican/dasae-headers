#include "dh/main.h"
#include "dh/debug.h"
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
        ignore fprintf(stderr, "Failed to load heightmap: %s\n", heightmap_file);
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
        ignore fprintf(stderr, "Failed to load colormap: %s\n", colormap_file);
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

typedef struct State {
    TerrainData terrain;
    Vec2i       camera_pos;
    f32         camera_angle; // <== We'll use this as 'phi'
    f32         height;       // camera height above ground
    f32         horizon;
    f32         scale_height;
    f32         distance;
    bool        is_running;
} State;

/* FIXME: 해상도 대응이 잘못됨. 스케일링이 필요함. 렌더링 과정 전 전달 값들의 사전 점검이 필요함 */
void State_render(const State* self, engine_Canvas* canvas, f64 dt) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(canvas);
    unused(dt);

    // Short names
    f32   phi      = self->camera_angle;
    f32   sin_phi  = sinf(phi);
    f32   cos_phi  = cosf(phi);
    Vec2f cam_p    = Vec_as$(Vec2f, self->camera_pos); // (p.x, p.y)
    f32   max_dist = self->distance;                   // 'distance'
    f32   scale_h  = self->scale_height;
    f32   horizon  = self->horizon;
    f32   cam_h    = self->height; // 'height'

    let heightmap = self->terrain.heightmap;
    let colormap  = self->terrain.colormap;
    i32 map_w     = (i32)self->terrain.width;
    i32 map_h     = (i32)self->terrain.height;

    i32 scr_w = (i32)canvas->width;  // 'screen_width'
    i32 scr_h = (i32)canvas->height; // 'screen_height'

    // ----------------------------------------------------------------
    // y_buffer: array of the "topmost drawn pixel" for each column
    // Initialize each entry to the bottom of the screen (scr_h).
    // ----------------------------------------------------------------
    let y_buffer = (f32*)malloc(sizeof(f32) * scr_w);
    for (i32 i = 0; i < scr_w; ++i) {
        y_buffer[i] = (f32)scr_h;
    }

    // We'll step forward in z from near (1.0) to far (max_dist).
    // Like Python, start with z=1 and increment by dz each iteration.
    // Also, after drawing each line, we do dz += 0.2
    f32 z  = 1.0f;
    f32 dz = 1.0f;

    while (z < max_dist) {
        // pleft, pright as in Python:
        //   pleft = ( -cosphi*z - sinphi*z + camP.x,
        //             sinphi*z - cosphi*z + camP.y )
        //   pright= (  cosphi*z - sinphi*z + camP.x,
        //            ( -sinphi*z - cosphi*z) + camP.y )
        // => This creates a 90° field-of-view line from left to right
        Vec2f p_left = {
            .x = (-cos_phi * z - sin_phi * z) + cam_p.x,
            .y = (sin_phi * z - cos_phi * z) + cam_p.y
        };
        Vec2f p_right = {
            .x = (cos_phi * z - sin_phi * z) + cam_p.x,
            .y = (-sin_phi * z - cos_phi * z) + cam_p.y
        };

        // dx, dy for stepping across that line in screen columns
        f32 dx = (p_right.x - p_left.x) / (f32)scr_w;
        f32 dy = (p_right.y - p_left.y) / (f32)scr_w;

        // For each column on screen, sample the terrain.
        for (i32 i = 0; i < scr_w; ++i) {
            // Convert f32 pleft.(x,y) to integer map indices
            // IMPORTANT: real code must clamp these to [0, map_w-1], [0, map_h-1]
            i32 ix = (i32)p_left.x;
            i32 iy = (i32)p_left.y;
            if (ix < 0) { ix = 0; }
            if (iy < 0) { iy = 0; }
            if (ix >= map_w) { ix = map_w - 1; }
            if (iy >= map_h) { iy = map_h - 1; }

            // Index in arrays
            i32 map_index = iy * map_w + ix;

            // heightmap[mapIndex] = ground height at (ix, iy)
            f32 ground_h = (f32)heightmap[map_index];

            // same formula as Python:
            //   height_on_screen = (cameraHeight - groundHeight)/z * scale_height + horizon
            f32 height_on_screen = (cam_h - ground_h) / z * scale_h + horizon;

            // Draw a vertical line from height_on_screen up to whatever
            // was stored in y_buffer[i]
            // => In Python, it’s DrawVerticalLine(i, top, bottom, color).
            //    We pass the color from colormap[mapIndex].
            i32 top    = (i32)height_on_screen;
            i32 bottom = (i32)y_buffer[i]; // previously drawn top
            if (top < 0) { top = 0; };
            if (bottom > scr_h) { bottom = scr_h; };


            // Only draw if top < bottom
            // if (top < bottom) {
            engine_Canvas_drawVLine(canvas, i, top, bottom, colormap[map_index]);
            // update y_buffer to the new top
            y_buffer[i] = (f32)top;
            // }

            // Step one column’s worth of x and y
            p_left.x += dx;
            p_left.y += dy;
        }

        // Next line in the distance
        z += dz;
        dz += 0.2f;
    }

    free(y_buffer);
}

Err$void dh_main(i32 argc, const char* argv[]) {
    reserveReturn(Err$void);
    unused(argc), unused(argv);

    scope_defer {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("voxel_space-debug.log", "w"), debug_file) {
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
                .width        = 80 * 2,
                .height       = 50 * 2,
            }
        ));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        // Create canvases
        let game_canvas = catch (engine_Canvas_create(80 * 2, 50 * 2, engine_CanvasType_rgba), err, {
            log_error("Failed to create canvas: %s\n", err);
            defer_return_err(err);
        });
        defer(engine_Canvas_destroy(game_canvas));
        log_info("canvas created\n");

        engine_Canvas_clear(game_canvas, Color_black);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, 80 * 2, 50 * 2);
        log_info("canvas views added\n");

        // Initialize rendering with camera parameters
        State state = {
            .terrain      = unwrap(loadSample("assets/hmap.png", "assets/cmap.png")),
            .camera_pos   = { .x = 512, .y = 512 },
            .camera_angle = 0.5f,
            .height       = 50.0f,
            .horizon      = 120.0f,
            .scale_height = 120.0f,
            .distance     = 300.0f,
            .is_running   = true,
        };
        defer(stbi_image_free(state.terrain.heightmap));
        defer(free(state.terrain.colormap));
        // var heap = (heap_Classic){};
        // var allocator = heap_Classic_allocator(&heap);
        log_info("game state created\n");
        ignore getchar();

        var  curr_time   = time_Instant_now();
        var  prev_time   = curr_time;
        let  target_time = time_Duration_fromSecs_f64(0.016f); // Assume 62.5 FPS for simplicity
        bool is_running  = true;
        log_info("game loop started\n");

        // Game loop
        while (is_running) {
            curr_time        = time_Instant_now();
            let elapsed_time = time_Instant_durationSince(curr_time, prev_time);
            let dt           = time_Duration_asSecs_f64(elapsed_time);

            // Process events
            try_defer(engine_Window_processEvents(window));

            // Update game state
            if (engine_Key_pressed(engine_KeyCode_Esc)) {
                is_running = false;
            }
            // try_defer(State_update(&state, dt));

            // Render all views
            engine_Canvas_clear(game_canvas, Color_black);
            State_render(&state, game_canvas, dt);

            // Present to screen
            engine_Window_present(window);

            // Sleep for the remaining time to maintain FPS
            time_SysTime_sleep(time_Duration_sub(target_time, elapsed_time));
            prev_time = curr_time;
        }
        defer_return_void();
    }
    return_deferred;
}
