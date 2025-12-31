/**
 * @file    example-usage.c
 * @brief   Example usage of the dage
 * @details Demonstrates the new architecture:
 *          - Backend injection (DIP)
 *          - Window-local input
 *          - Unified event system
 *          - Multi-window support
 *
 * @note run with `dh-c run example-usage.c --sample`
 */

#include "dage.h"
#include "dh/main.h"
#include "dh/Rand.h"
#include "dh/heap/Page.h"

/*========== Simple Game State ==========*/

typedef struct game_State {
    m_V2f32 player_pos;
    color_RGBA player_color;
    bool running;
    Rand rng;
} game_State;

/*========== Event Handling ==========*/

$static fn_((game_handleEvent(dage_Window* win, game_State* game))(void)) {
    /* Poll and handle events */
    while_some(dage_Window_pollEvent(win), event) {
        match_(event) {
        /* Window events */
        pattern_((dage_Event_close_request)($ignore)) {
            game->running = false;
        } $end(pattern);

        pattern_((dage_Event_resize)(on_resize)) {
            log_info(
                "Window resized: %ux%u -> %ux%u",
                on_resize->old_size.x, on_resize->old_size.y,
                on_resize->new_size.x, on_resize->new_size.y
            );
        } $end(pattern);

        pattern_((dage_Event_focus)($ignore)) {
            log_info("Window focused");
        } $end(pattern);

        pattern_((dage_Event_blur)($ignore)) {
            log_info("Window lost focus");
        } $end(pattern);

        /* Input events (optional - can use state queries instead) */
        pattern_((dage_Event_key_down)(on_key_down)) {
            if (on_key_down->key == dage_KeyCode_esc) {
                game->running = false;
            }

            /* Change color on space */
            if (on_key_down->key == dage_KeyCode_space) {
                game->player_color = color_RGBA_fromOpaque(
                    int_rem(Rand_next$u8(&game->rng), 255),
                    int_rem(Rand_next$u8(&game->rng), 255),
                    int_rem(Rand_next$u8(&game->rng), 255)
                );
            }
        } $end(pattern);

        default_() {
            /* Ignore other events */
        } $end(default);
        } $end(match);
    };
};

/*========== Game Logic ==========*/

$static fn_((game_update(dage_Window* win, game_State* game, f32 dt))(void)) {
    /* State-based input (continuous movement) */
    let speed = 100.0f * dt;
    let isKeyHeld = dage_Window_isKeyHeld;
    if (isKeyHeld(win, dage_KeyCode_w) || isKeyHeld(win, dage_KeyCode_arrow_up)) {
        game->player_pos.y -= speed;
    }
    if (isKeyHeld(win, dage_KeyCode_s) || isKeyHeld(win, dage_KeyCode_arrow_down)) {
        game->player_pos.y += speed;
    }
    if (isKeyHeld(win, dage_KeyCode_a) || isKeyHeld(win, dage_KeyCode_arrow_left)) {
        game->player_pos.x -= speed;
    }
    if (isKeyHeld(win, dage_KeyCode_d) || isKeyHeld(win, dage_KeyCode_arrow_right)) {
        game->player_pos.x += speed;
    }

    /* Clamp to window bounds */
    let size = dage_Window_getSize(win);
    game->player_pos.x = prim_clamp(game->player_pos.x, 0.0f, as$(f32)(size.x - 20));
    game->player_pos.y = prim_clamp(game->player_pos.y, 0.0f, as$(f32)(size.y - 20));
};

/*========== Rendering ==========*/

$static fn_((game_render(dage_Canvas* canvas, const game_State* game))(void)) {
    /* Clear */
    dage_Canvas_clear(canvas, some$((O$color_RGBA)color_RGBA_black));

    /* Draw player as filled rectangle */
    let x = as$(i32)(game->player_pos.x);
    let y = as$(i32)(game->player_pos.y);
    dage_Canvas_fillRect(canvas, x, y, x + 20, y + 20, game->player_color);

    /* Draw border */
    dage_Canvas_drawRect(canvas, x, y, x + 20, y + 20, color_RGBA_white);
};

/*========== Main ==========*/

#define game__use_wsi_backend pp_true

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    /* Setup allocator */
    var page = (heap_Page){};
    let gpa = heap_Page_allocator(&page);

    /*
     * Step 1: Create Backend
     * User chooses the implementation - engine doesn't know/care
     */
#if game__use_wsi_backend /* WSI Backend (native window) */
    var wsi = try_(dage_core_WSI_init((dage_core_WSI_Cfg){
        .gpa = gpa,
    }));
    defer_(dage_core_WSI_fini(&wsi));
    let backend = dage_core_WSI_backend(wsi);
#else /* VT100 Backend (terminal) */
    var vt100 = try_(dage_core_VT100_init((dage_core_VT100_Cfg){
        .gpa = gpa,
    }));
    defer_(dage_core_VT100_fini(&vt100));
    let backend = dage_core_VT100_backend(vt100);
#endif

    /*
     * Step 2: Create Runtime with injected Backend
     * Runtime manages windows and coordinates everything
     */
    var runtime = try_(dage_Runtime_init((dage_Runtime_Cfg){
        .gpa = gpa,
        .backend = backend,
    }));
    defer_(dage_Runtime_fini(&runtime));

    /*
     * Step 3: Create Window
     * Runtime creates both the Window and Backend target
     */
    let win_id = try_(dage_Runtime_createWindow(
        &runtime,
        (dage_Runtime_WindowCfg){
            .size = { .x = 640, .y = 480 },
            .clear_color = some(color_RGBA_fromOpaque_static(0x18, 0x18, 0x18)),
            .title = some(u8_l("Game Window")),
            .scale = 1.0f,
            .resizable = true,
        }
    ));

    /* Get window pointer for convenience */
    let win = unwrap_(dage_Runtime_getWindow(&runtime, win_id));

    /*
     * Step 4: Create game canvas and viewport
     */
    var game_canvas = try_(dage_Canvas_init((dage_Canvas_Cfg){
        .gpa = gpa,
        .width = 640,
        .height = 480,
        .type = dage_CanvasType_rgba,
    }));
    defer_(dage_Canvas_fini(&game_canvas, gpa));

    /* Add viewport that shows the game canvas */
    let vp_id = unwrap_(dage_Window_addViewport(
        win,
        (dage_Viewport_Cfg){
            .canvas = &game_canvas,
            .dst_rect = {
                .pos = { .x = 0, .y = 0 },
                .size = { .x = 640, .y = 480 },
            },
            .fit = dage_Viewport_Fit_stretch,
            .visible = true,
            .z_order = 0,
        }
    ));
    let_ignore = vp_id;

    /*
     * Step 5: Initialize game state
     */
    game_State game = {
        .player_pos = { .x = 310.0f, .y = 230.0f },
        .player_color = color_RGBA_green,
        .running = true,
        .rng = Rand_init(),
    };

    /*
     * Step 6: Main Loop
     */
    let target_fps = 60.0f;
    let frame_time = 1.0f / target_fps;

    while (game.running && !dage_Runtime_shouldQuit(&runtime)) {
        /*
         * 6a. Process Events
         * - Backend pumps OS events
         * - Runtime dispatches to windows
         * - InputState updated automatically
         */
        dage_Runtime_processEvents(&runtime);

        /*
         * 6b. Handle Events (optional - can use state queries)
         * Events are already in window's queue
         */
        game_handleEvent(win, &game);

        /*
         * 6c. Update Game Logic
         * Use window's input state for continuous input
         */
        game_update(win, &game, frame_time);

        /*
         * 6d. Render
         * Draw to canvas, viewport will be composited
         */
        game_render(&game_canvas, &game);

        /*
         * 6e. Present
         * Runtime composites viewports and presents to backend
         */
        dage_Runtime_presentAll(&runtime);

        /*
         * 6f. End Frame
         * Clears pressed/released flags for next frame
         */
        dage_Runtime_endFrame(&runtime);
    }

    log_info("Game exited cleanly");
    return_ok({});
} $unguarded_(fn);

/*========== Multi-Window Example ==========*/

/**
 * @brief Example of multi-window support
 * @details Each window has its own input state - no confusion!
 */
$attr($must_check $maybe_unused)
$static fn_((example_multi_window(mem_Allocator gpa, dage_Backend backend))(E$void) $guard) {
    var runtime = try_(dage_Runtime_init((dage_Runtime_Cfg){
        .gpa = gpa,
        .backend = backend,
    }));
    defer_(dage_Runtime_fini(&runtime));

    /* Create two windows */
    let win_a_id = try_(dage_Runtime_createWindow(
        &runtime,
        (dage_Runtime_WindowCfg){
            .size = { .x = 400, .y = 300 },
            .title = some(u8_l("Window A")),
        }
    ));

    let win_b_id = try_(dage_Runtime_createWindow(
        &runtime,
        (dage_Runtime_WindowCfg){
            .size = { .x = 400, .y = 300 },
            .title = some(u8_l("Window B")),

        }
    ));

    while (!dage_Runtime_shouldQuit(&runtime)) {
        dage_Runtime_processEvents(&runtime);

        /* Each window's input is independent */
        if_some((dage_Runtime_getWindow(&runtime, win_a_id))(win_a)) {
            /* This only detects key press in Window A */
            if (dage_Window_isKeyPressed(win_a, dage_KeyCode_space)) {
                log_info("Space pressed in Window A!");
            }
        }

        if_some((dage_Runtime_getWindow(&runtime, win_b_id))(win_b)) {
            /* This only detects key press in Window B */
            if (dage_Window_isKeyPressed(win_b, dage_KeyCode_space)) {
                log_info("Space pressed in Window B!");
            }
        }

        dage_Runtime_presentAll(&runtime);
        dage_Runtime_endFrame(&runtime);
    }

    return_ok({});
} $unguarded_(fn);
