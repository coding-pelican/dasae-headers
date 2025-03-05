#include "dh/main.h"
#include "dh/debug.h"
#include "dh/log.h"

#include "dh/mem.h"
#include "dh/heap/Page.h"
#include "dh/ArrList.h"
#include "dh/meta/common.h"
#include "dh/Grid.h"

#include "dh/time/common.h"
#include "dh/time/Duration.h"

#include "engine.h"
#include "engine/canvas.h"
#include "engine/input.h"

use_Grid$(i8);

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
#define target_fps__62_50 /* template value */ (62.50)
#define target_fps__50_00 /* template value */ (50.00)
#define target_fps__31_25 /* template value */ (31.25)

#define target_fps (target_fps__62_50)
#define target_spf (1.0 / target_fps)

#define GameOfLife_default_tick_threshold (6ull * 5ull)

typedef struct GameOfLife GameOfLife;
struct GameOfLife {
    Sli$i8 curr_states;
    Sli$i8 next_states;
    usize  width;
    usize  height;
    usize  tick_current;
    usize  tick_threshold;
};
use_Err$(GameOfLife);
void    GameOfLife_update(GameOfLife* self, f64 dt);
Grid$i8 GameOfLife_getCellMatrix(const GameOfLife* self);
void    GameOfLife_setCell(GameOfLife* self, usize x, usize y, i8 state);
void    GameOfLife_setCellSlice(GameOfLife* self, usize left_top_x, usize left_top_y, Sli$i8 cells);
void    GameOfLife_setCellMatrix(GameOfLife* self, usize left_top_x, usize left_top_y, Grid$i8 cells);
void    GameOfLife_toggleCell(GameOfLife* self, usize x, usize y, i8 state);
void    GameOfLife_toggleCellSlice(GameOfLife* self, usize left_top_x, usize left_top_y, Sli$i8 cells);
void    GameOfLife_toggleCellMatrix(GameOfLife* self, usize left_top_x, usize left_top_y, Grid$i8 cells);

typedef struct State {
    GameOfLife    cells;
    bool          is_running;
    mem_Allocator allocator;
} State;
use_Err$(State);
Err$State State_init(mem_Allocator allocator, usize states_width, usize states_height) must_check;
void      State_update(State* self, f64 dt);
void      State_render(const State* self, engine_Canvas* canvas, f64 dt);
void      State_fini(State* self);


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
                .window_title  = "Game of Life",
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

        // Initialize game state
        var allocator = heap_Page_allocator(&(heap_Page){});
        var state     = try(State_init(allocator, window_res_width, window_res_height));
        defer(State_fini(&state));
        // R-Pentomino
        {
            let x = (window_res_width - 5) / 2;
            let y = (window_res_height - 3) / 2;
            GameOfLife_setCellSlice(&state.cells, x, y + 0, (Sli$i8)Sli_from(((i8[]){ 0, 0, 1, 1, 0 }), 5));
            GameOfLife_setCellSlice(&state.cells, x, y + 1, (Sli$i8)Sli_from(((i8[]){ 0, 1, 1, 0, 0 }), 5));
            GameOfLife_setCellSlice(&state.cells, x, y + 2, (Sli$i8)Sli_from(((i8[]){ 0, 0, 1, 0, 0 }), 5));
        }
        log_info("game state created\n");
        ignore getchar();

        // Initialize timing variables
        let target_frame_time = time_Duration_fromSecs_f64(1.0 / target_fps);
        var prev_frame_time   = time_Instant_now();
        log_info("game loop started\n");

        // Game loop
        while (state.is_running) {
            // 1) Capture the start of the frame (and capture the end-of-frame time of prev iteration’s dt includes sleep)
            let curr_frame_time = time_Instant_now();

            // 2) Compute how long since last frame (purely for your dt usage)
            let elapsed = time_Instant_durationSince(curr_frame_time, prev_frame_time);
            let dt      = time_Duration_asSecs_f64(elapsed);

            // 3) Process input/events
            try(engine_Window_processEvents(window));

            // 4) Update game state
            State_update(&state, dt);

            // 5) Render all views
            engine_Canvas_clearDefault(game_canvas);
            State_render(&state, game_canvas, dt);
            engine_Window_present(window);

            // 6) (Optional) Display instantaneous FPS
            const f64 fps = (0.0 < dt) ? (1.0 / dt) : 9999.0;
            printf("\033[H\033[40;37m"); // Move cursor to top left
            printf("\rFPS: %6.2f\n", fps);
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

            // 7) Measure how long the update+render actually took
            let now        = time_Instant_now();
            let frame_used = time_Instant_durationSince(now, curr_frame_time);

            // 8) Subtract from our target
            if_some(time_Duration_chkdSub(target_frame_time, frame_used), leftover) {
                time_sleep(leftover);
            }
            prev_frame_time = curr_frame_time;
        }
        return_void();
    }
    scope_returnReserved;
}


Err$State State_init(mem_Allocator allocator, usize states_width, usize states_height) {
    scope_reserveReturn(Err$State) {
        const usize buffer_width    = states_width + 2;
        const usize buffer_height   = states_height + 2;
        const usize buffer_size     = buffer_width * buffer_height;
        let         mem_curr_states = meta_cast$(Sli$i8, try(mem_Allocator_alloc(allocator, typeInfo$(i8), buffer_size)));
        errdefer(mem_Allocator_free(allocator, anySli(mem_curr_states)));
        memset(mem_curr_states.ptr, 0, buffer_size);
        let mem_next_states = meta_cast$(Sli$i8, try(mem_Allocator_alloc(allocator, typeInfo$(i8), buffer_size)));
        errdefer(mem_Allocator_free(allocator, anySli(mem_next_states)));
        memset(mem_next_states.ptr, 0, buffer_size);
        return_ok((State){
            .cells = {
                .curr_states    = mem_curr_states,
                .next_states    = mem_next_states,
                .width          = buffer_width,
                .height         = buffer_height,
                .tick_current   = 0,
                .tick_threshold = GameOfLife_default_tick_threshold,
            },
            .is_running = true,
            .allocator  = allocator,
        });
    }
    scope_returnReserved;
}
void State_update(State* self, f64 dt) {
    debug_assert_nonnull(self);

    /* Quit on Escape */
    if (engine_Key_pressed(engine_KeyCode_esc)) {
        self->is_running = false;
    }

    /* Simple mouse input  */
    if (engine_Mouse_held(engine_MouseButton_left)) {
        let mouse_pos = engine_Mouse_getPos();
        GameOfLife_setCell(&self->cells, mouse_pos.x, mouse_pos.y, 1);
    }
    if (engine_Mouse_held(engine_MouseButton_right)) {
        let mouse_pos = engine_Mouse_getPos();
        GameOfLife_setCell(&self->cells, mouse_pos.x, mouse_pos.y, 0);
    }

    /* Simulate game of life tick */
    GameOfLife_update(&self->cells, dt);
}
void State_render(const State* self, engine_Canvas* canvas, f64 dt) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(canvas);

    let states = GameOfLife_getCellMatrix(&self->cells);
    unused(dt);
    for (usize y = 0; y < canvas->height; ++y) {
        for (usize x = 0; x < canvas->width; ++x) {
            let cell_state = *Grid_at(states, x + 1, y + 1);
            engine_Canvas_drawPixel(canvas, (i32)x, (i32)y, cell_state ? Color_white : Color_black);
        }
    }
}
void State_fini(State* self) {
    debug_assert_nonnull(self);

    mem_Allocator_free(self->allocator, anySli(self->cells.curr_states));
    mem_Allocator_free(self->allocator, anySli(self->cells.next_states));
}


void GameOfLife_update(GameOfLife* self, f64 dt) {
    debug_assert_nonnull(self);
    unused(dt);

    if (self->tick_current < self->tick_threshold) {
        self->tick_current += 1;
        return;
    }
    self->tick_current = 0;

    let width  = (i32)self->width;
    let height = (i32)self->height;
    let curr   = Grid_fromSli$(Grid$i8, self->curr_states, width, height);
    let next   = Grid_fromSli$(Grid$i8, self->next_states, width, height);

    memcpy(self->curr_states.ptr, self->next_states.ptr, self->width * self->height * sizeof(i8));

    // Update reflection at edges cells
    for (i32 x = 1; x < width - 1; ++x) {
        *Grid_at(curr, x, 0)          = *Grid_at(curr, x, height - 2); // Top edge
        *Grid_at(curr, x, height - 1) = *Grid_at(curr, x, 1);          // Bottom edge
    }
    for (i32 y = 1; y < height - 1; ++y) {
        *Grid_at(curr, 0, y)         = *Grid_at(curr, width - 2, y); // Left edge
        *Grid_at(curr, width - 1, y) = *Grid_at(curr, 1, y);         // Right edge
    }

    // Update corner cells
    *Grid_at(curr, 0, 0)                  = *Grid_at(curr, width - 2, height - 2); // Top-left
    *Grid_at(curr, width - 1, 0)          = *Grid_at(curr, 1, height - 2);         // Top-right
    *Grid_at(curr, 0, height - 1)         = *Grid_at(curr, width - 2, 1);          // Bottom-left
    *Grid_at(curr, width - 1, height - 1) = *Grid_at(curr, 1, 1);                  // Bottom-right

    // Update inbounds cells
    for (i32 y = 1; y < height - 1; ++y) {
        for (i32 x = 1; x < width - 1; ++x) {
            // clang-format off
            // The secret of artificial life =================================================
            i32 neighbors_num
                = (*Grid_at(curr, x - 1, y - 1) + *Grid_at(curr, x, y - 1) + *Grid_at(curr, x + 1, y - 1))
                + (*Grid_at(curr, x - 1, y)     + 0                       + *Grid_at(curr, x + 1, y))
                + (*Grid_at(curr, x - 1, y + 1) + *Grid_at(curr, x, y + 1) + *Grid_at(curr, x + 1, y + 1));
            // clang-format on

            if (*Grid_at(curr, x, y) == 1) {
                *Grid_at(next, x, y) = as$(i8, neighbors_num == 2 || neighbors_num == 3);
            } else {
                *Grid_at(next, x, y) = as$(i8, neighbors_num == 3);
            }
        }
    }
}
Grid$i8 GameOfLife_getCellMatrix(const GameOfLife* self) {
    debug_assert_nonnull(self);
    return Grid_fromSli$(Grid$i8, self->curr_states, self->width, self->height);
}
void GameOfLife_setCell(GameOfLife* self, usize x, usize y, i8 state) {
    debug_assert_nonnull(self);

    // Add boundary checks
    if (self->width - 2 <= x || self->height - 2 <= y) {
        return; // Silently fail
    }

    let width                    = (i32)self->width;
    let height                   = (i32)self->height;
    let next                     = Grid_fromSli$(Grid$i8, self->next_states, width, height);
    *Grid_at(next, x + 1, y + 1) = state;
}
void GameOfLife_setCellSlice(GameOfLife* self, usize left_top_x, usize left_top_y, Sli$i8 cells) {
    debug_assert_nonnull(self);

    // Add boundary checks
    if (self->width - 2 <= left_top_x || self->height - 2 <= left_top_y) {
        return; // Silently fail
    }

    let width  = (i32)self->width;
    let height = (i32)self->height;
    let next   = Grid_fromSli$(Grid$i8, self->next_states, width, height);
    let len    = math_min(cells.len, as$(usize, width - left_top_x - 2)); // Fix: Adjust limit

    for (usize i = 0; i < len; ++i) {
        *Grid_at(next, left_top_x + i + 1, left_top_y + 1) = cells.ptr[i];
    }
}
void GameOfLife_setCellMatrix(GameOfLife* self, usize left_top_x, usize left_top_y, Grid$i8 cells) {
    debug_assert_nonnull(self);

    // Add boundary checks
    if (self->width - 2 <= left_top_x || self->height - 2 <= left_top_y) {
        return; // Silently fail
    }

    let width  = (i32)self->width;
    let height = (i32)self->height;
    let next   = Grid_fromSli$(Grid$i8, self->next_states, width, height);
    let area_w = math_min(cells.width, as$(usize, width - left_top_x - 1));
    let area_h = math_min(cells.height, as$(usize, height - left_top_y - 1));

    for (usize y = 0; y < area_h; ++y) {
        for (usize x = 0; x < area_w; ++x) {
            *Grid_at(next, left_top_x + x + 1, left_top_y + y + 1) = *Grid_at(cells, x, y);
        }
    }
}
void GameOfLife_toggleCell(GameOfLife* self, usize x, usize y, i8 state) {
    debug_assert_nonnull(self);

    // Add boundary checks
    if (self->width - 2 <= x || self->height - 2 <= y) {
        return; // Silently fail
    }

    let width  = (i32)self->width;
    let height = (i32)self->height;
    let curr   = Grid_fromSli$(Grid$i8, self->curr_states, width, height);
    let next   = Grid_fromSli$(Grid$i8, self->next_states, width, height);

    *Grid_at(next, x + 1, y + 1) = as$(i8, *Grid_at(curr, x + 1, y + 1) ^ state);
}
void GameOfLife_toggleCellSlice(GameOfLife* self, usize left_top_x, usize left_top_y, Sli$i8 cells) {
    debug_assert_nonnull(self);

    // Add boundary checks
    if (self->width - 2 <= left_top_x || self->height - 2 <= left_top_y) {
        return; // Silently fail
    }

    let width  = (i32)self->width;
    let height = (i32)self->height;
    let curr   = Grid_fromSli$(Grid$i8, self->curr_states, width, height);
    let next   = Grid_fromSli$(Grid$i8, self->next_states, width, height);
    let len    = math_min(cells.len, as$(usize, width - left_top_x - 1));

    for (usize i = 0; i < len; ++i) {
        let toggled = *Grid_at(curr, left_top_x + i + 1, left_top_y + 1) ^ *Sli_at(cells, i);

        *Grid_at(next, left_top_x + i + 1, left_top_y + 1) = (i8)toggled;
    }
}
void GameOfLife_toggleCellMatrix(GameOfLife* self, usize left_top_x, usize left_top_y, Grid$i8 cells) {
    debug_assert_nonnull(self);

    // Add boundary checks
    if (self->width - 2 <= left_top_x || self->height - 2 <= left_top_y) {
        return; // Silently fail
    }

    let width  = (i32)self->width;
    let height = (i32)self->height;
    let curr   = Grid_fromSli$(Grid$i8, self->curr_states, width, height);
    let next   = Grid_fromSli$(Grid$i8, self->next_states, width, height);
    let area_w = math_min(cells.width, as$(usize, width - left_top_x - 1));
    let area_h = math_min(cells.height, as$(usize, height - left_top_y - 1));

    for (usize y = 0; y < area_h; ++y) {
        for (usize x = 0; x < area_w; ++x) {
            let toggled = *Grid_at(curr, left_top_x + x + 1, left_top_y + y + 1) ^ *Grid_at(cells, x, y);

            *Grid_at(next, left_top_x + x + 1, left_top_y + y + 1) = (i8)toggled;
        }
    }
}

// void GameOfLife_Init() {
//     const int size = Display_Size;

//     self->states_     = (int*)malloc(size * sizeof(int));
//     self->nextStates_ = (int*)malloc(size * sizeof(int));
//     assert(self->states_);
//     assert(self->nextStates_);

//     memset(self->states_, 0, size * sizeof(int));
//     memset(self->nextStates_, 0, size * sizeof(int));

//     srand((unsigned int)time(NULL)); // NOLINT
//     // for (int i = 0; i < size; ++i) {
//     //     self->nextStates_[i] = rand() % 2;
//     // }

//     // // R-Pentomino
//     // setCell(80, 50, "  ## ");
//     // setCell(80, 51, " ##  ");
//     // setCell(80, 52, "  #  ");

//     // // Gosper Glider Gun
//     GameOfLife_setCell(60, 45, "........................#............");
//     GameOfLife_setCell(60, 46, "......................#.#............");
//     GameOfLife_setCell(60, 47, "............##......##............##.");
//     GameOfLife_setCell(60, 48, "...........#...#....##............##.");
//     GameOfLife_setCell(60, 49, "##........#.....#...##...............");
//     GameOfLife_setCell(60, 50, "##........#...#.##....#.#............");
//     GameOfLife_setCell(60, 51, "..........#.....#.......#............");
//     GameOfLife_setCell(60, 52, "...........#...#.....................");
//     GameOfLife_setCell(60, 53, "............##.......................");

//     // // Infinite Growth
//     // setCell(20, 50, "########.#####...###......#######.#####");
// }
