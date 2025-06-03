#include "dh/main.h"
#include "dh/debug.h"
#include "dh/log.h"

#include "dh/mem.h"
#include "dh/ArrList.h"
#include "dh/heap/Page.h"
#include "dh/meta/common.h"
#include "dh/Grid.h"

#include "dh/time/common.h"
#include "dh/time/Duration.h"

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
#define target_fps__62_50 /* template value */ (62.50)
#define target_fps__50_00 /* template value */ (50.00)
#define target_fps__31_25 /* template value */ (31.25)

#define target_fps (target_fps__62_50)
#define target_spf (1.0 / target_fps)

#define GameOfLife_default_tick_threshold (6ull * 5ull)

use_Grid$(i8);
typedef struct GameOfLife {
    Sli$i8 curr_states;
    Sli$i8 next_states;
    usize  width;
    usize  height;
    usize  tick_current;
    usize  tick_threshold;
} GameOfLife;
static fn_(GameOfLife_update(GameOfLife* self, f64 dt), void);
static fn_(GameOfLife_entireCells(const GameOfLife* self), Grid$i8);
static fn_(GameOfLife_setCell(GameOfLife* self, usize x, usize y, i8 state), void);
static fn_(GameOfLife_setCellSlice(GameOfLife* self, usize left_top_x, usize left_top_y, Sli$i8 cells), void);
static fn_(GameOfLife_setCellGrid(GameOfLife* self, usize left_top_x, usize left_top_y, Grid$i8 cells), void);
static fn_(GameOfLife_toggleCell(GameOfLife* self, usize x, usize y, i8 state), void);
static fn_(GameOfLife_toggleCellSlice(GameOfLife* self, usize left_top_x, usize left_top_y, Sli$i8 cells), void);
static fn_(GameOfLife_toggleCellGrid(GameOfLife* self, usize left_top_x, usize left_top_y, Grid$i8 cells), void);

typedef struct State {
    mem_Allocator  allocator;
    engine_Window* window;
    engine_Canvas* canvas;
    engine_Input*  input;
    GameOfLife     cells;
    bool           is_paused;
    bool           is_running;
} State;
use_Err$(State);
$must_check;
$static fn_(
    State_init(
        mem_Allocator  allocator,
        engine_Window* window,
        engine_Canvas* canvas,
        engine_Input*  input,
        usize          states_width,
        usize          states_height
    ),
    Err$State
);
$static fn_(State_fini(State* self), void);
$static fn_(State_update(State* self, f64 dt), void);
$static fn_(State_render(const State* self, engine_Canvas* canvas, f64 dt), void);



fn_(dh_main(Sli$Str_const args), Err$void, $guard) {
    $ignore = args;
    // Initialize logging to a file
    try_(log_init("log/debug.log"));
    defer_(log_fini());
    {
        // Configure logging behavior
        log_setLevel(log_Level_debug);
        log_showTimestamp(true);
        log_showLevel(true);
        log_showLocation(false);
        log_showFunction(true);
    }

    // Initialize heap allocator and page pool
    var allocator = heap_Page_allocator(&(heap_Page){});
    log_info("allocator reserved");

    // Create window
    let window = try_(engine_Window_init(&(engine_Window_Config){
        .allocator = some(allocator),
        .rect_size = {
            .x = window_res_width,
            .y = window_res_height,
        },
        .default_color = some(Color_blue),
        .title         = some(u8_l("Game of Life"))
    }));
    defer_(engine_Window_fini(window));
    log_info("window created");

    // Create canvases
    let game_canvas = try_(engine_Canvas_init(&(engine_Canvas_Config){
        .allocator     = some(allocator),
        .width         = window_res_width,
        .height        = window_res_height,
        .default_color = none(),
        .type          = some(engine_CanvasType_rgba),
    }));
    defer_(engine_Canvas_fini(game_canvas));
    {
        log_info("canvas created: %s", nameOf(game_canvas));
        engine_Canvas_clear(game_canvas, none$(Opt$Color));
        log_info("canvas cleared: %s", nameOf(game_canvas));
        engine_Window_appendView(
            window,
            &(engine_CanvasView_Config){
                .canvas      = game_canvas,
                .pos         = { .x = 0, .y = 0 },
                .size        = { .x = window_res_width, .y = window_res_height },
                .scale       = { .x = 1.0f, .y = 1.0f },
                .resizable_x = true,
                .resizable_y = true,
                .visible     = true,
            }
        );
        log_info("canvas views added: %s", nameOf(game_canvas));
    }

    // Create input system
    let input = try_(engine_Input_init(allocator));
    defer_(engine_Input_fini(input));

    // Bind engine core
    let core = try_(engine_core_Vt100_init(&(engine_core_Vt100_Config){
        .allocator = some(allocator),
        .window    = window,
        .input     = input,
    }));
    defer_(engine_core_Vt100_fini(core));
    log_info("engine ready");

    // Create game state
    var state = try_(State_init(
        allocator, window, game_canvas, input,
        window_res_width, window_res_height
    ));
    defer_(State_fini(&state));
    // R-Pentomino
    {
        let shape_w = 5;
        let shape_h = 3;
        let x       = (window_res_width - shape_w) / 2;
        let y       = (window_res_height - shape_h) / 2;
        GameOfLife_setCellSlice(&state.cells, x, y + 0, Sli_from$(Sli$i8, ((i8[]){ 0, 0, 1, 1, 0 }), shape_w));
        GameOfLife_setCellSlice(&state.cells, x, y + 1, Sli_from$(Sli$i8, ((i8[]){ 0, 1, 1, 0, 0 }), shape_w));
        GameOfLife_setCellSlice(&state.cells, x, y + 2, Sli_from$(Sli$i8, ((i8[]){ 0, 0, 1, 0, 0 }), shape_w));
    }
    log_info("game state created\n");

    $ignore = engine_utils_getch();

    // Initialize timing variables
    let target_frame_time = time_Duration_fromSecs_f64(target_spf);
    var prev_frame_time   = time_Instant_now();
    log_info("game loop started\n");

    // Game loop
    while (state.is_running) {
        // 1) Capture the start of the frame (and capture the end-of-frame time of prev iteration's dt includes sleep)
        let curr_frame_time = time_Instant_now();

        // 2) Compute how long since last frame (purely for your dt usage)
        let elapsed = time_Instant_durationSince(curr_frame_time, prev_frame_time);
        let dt      = time_Duration_asSecs_f64(elapsed);

        // 3) Process input/events
        try_(engine_Window_update(window));

        // 4) Update game state
        State_update(&state, dt);

        // 5) Render all views
        State_render(&state, game_canvas, dt);
        engine_Window_present(window);

        // 6) (Optional) Display instantaneous FPS
        const f64 fps = (0.0 < dt) ? (1.0 / dt) : 9999.0;
        printf("\033[H\033[40;37m"); // Move cursor to top left
        printf("\rFPS: %6.2f\n", fps);
        printf(state.is_paused ? "Paused\n" : "Running\n");
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
    return_ok({});
}$unguarded;



fn_(State_init(
        mem_Allocator  allocator,
        engine_Window* window,
        engine_Canvas* canvas,
        engine_Input*  input,
        usize          states_width,
        usize          states_height
    ),
    Err$State, $guard) {
    const usize buffer_width  = states_width + 2;
    const usize buffer_height = states_height + 2;
    const usize buffer_size   = buffer_width * buffer_height;

    let mem_curr_states = meta_cast$(Sli$i8, try_(mem_Allocator_alloc(allocator, typeInfo$(i8), buffer_size)));
    errdefer_(mem_Allocator_free(allocator, anySli(mem_curr_states)));
    memset(mem_curr_states.ptr, 0, buffer_size);

    let mem_next_states = meta_cast$(Sli$i8, try_(mem_Allocator_alloc(allocator, typeInfo$(i8), buffer_size)));
    errdefer_(mem_Allocator_free(allocator, anySli(mem_next_states)));
    memset(mem_next_states.ptr, 0, buffer_size);

    return_ok({
            .allocator  = allocator,
            .window     = window,
            .canvas     = canvas,
            .input      = input,
            .cells = {
                .curr_states    = mem_curr_states,
                .next_states    = mem_next_states,
                .width          = buffer_width,
                .height         = buffer_height,
                .tick_current   = 0,
                .tick_threshold = GameOfLife_default_tick_threshold,
            },
            .is_paused = true,
            .is_running = true,
        });
} $unguarded;
fn_(State_fini(State* self), void) {
    debug_assert_nonnull(self);
    mem_Allocator_free(self->allocator, anySli(self->cells.curr_states));
    mem_Allocator_free(self->allocator, anySli(self->cells.next_states));
}
fn_(State_update(State* self, f64 dt), void) {
    debug_assert_nonnull(self);
    /* Quit on Escape */
    let keyboard = self->input->keyboard;
    if (engine_Keyboard_pressed(keyboard, engine_KeyCode_esc)) {
        self->is_running = false;
    }
    /* Toggle pause/resume */
    if (engine_Keyboard_pressed(keyboard, engine_KeyCode_space)) {
        log_debug("toggle pause/resume\n");
        self->is_paused = !self->is_paused;
    }

    /* Add cells */
    let mouse = self->input->mouse;
    if (engine_Mouse_held(mouse, engine_MouseButton_left)) {
        let mouse_pos = engine_Mouse_getPos(mouse);
        log_debug("add cell: %d, %d\n", mouse_pos.x, mouse_pos.y);
        GameOfLife_setCell(&self->cells, mouse_pos.x, mouse_pos.y, 1);
    }
    /* Remove cells */
    if (engine_Mouse_held(mouse, engine_MouseButton_right)) {
        let mouse_pos = engine_Mouse_getPos(mouse);
        log_debug("remove cell: %d, %d\n", mouse_pos.x, mouse_pos.y);
        GameOfLife_setCell(&self->cells, mouse_pos.x, mouse_pos.y, 0);
    }

    /* Simulate game of life tick */
    if (self->is_paused) { return; }
    GameOfLife_update(&self->cells, dt);
}
fn_(State_render(const State* self, engine_Canvas* canvas, f64 dt), void) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(canvas);
    $ignore = dt;

    let cells  = GameOfLife_entireCells(&self->cells);
    let buffer = canvas->buffer;

    // Add robust bounds checking
    let max_x = prim_min(buffer.width, cells.width - 1);
    let max_y = prim_min(buffer.height, cells.height - 1);

    for (u32 y = 0; y < max_y; ++y) {
        for (u32 x = 0; x < max_x; ++x) {
            let cell_state = Grid_getAt(cells, x + 1, y + 1);
            engine_Canvas_drawPixel(
                canvas, as$(i32, x), as$(i32, y),
                cell_state ? Color_white : Color_black
            );
        }
    }
}
fn_(GameOfLife_update(GameOfLife* self, f64 dt), void) {
    debug_assert_nonnull(self);
    $ignore = dt;

    if (self->tick_current < self->tick_threshold) {
        self->tick_current++;
        return;
    }
    self->tick_current = 0;

    let width  = as$(i32, self->width);
    let height = as$(i32, self->height);
    let curr   = Grid_fromSli$(Grid$i8, self->curr_states, width, height);
    let next   = Grid_fromSli$(Grid$i8, self->next_states, width, height);

    // Clear the next states buffer for computation
    memset(self->next_states.ptr, 0, self->width * self->height * sizeof(i8));

    // Update reflection at edges cells (work on current state)
    for (i32 x = 1; x < width - 1; ++x) {
        *Grid_at(curr, x, 0)          = Grid_getAt(curr, x, height - 2); // Top edge
        *Grid_at(curr, x, height - 1) = Grid_getAt(curr, x, 1);          // Bottom edge
    }
    for (i32 y = 1; y < height - 1; ++y) {
        *Grid_at(curr, 0, y)         = Grid_getAt(curr, width - 2, y); // Left edge
        *Grid_at(curr, width - 1, y) = Grid_getAt(curr, 1, y);         // Right edge
    }

    // Update corner cells (work on current state)
    *Grid_at(curr, 0, 0)                  = Grid_getAt(curr, width - 2, height - 2); // Top-left
    *Grid_at(curr, width - 1, 0)          = Grid_getAt(curr, 1, height - 2);         // Top-right
    *Grid_at(curr, 0, height - 1)         = Grid_getAt(curr, width - 2, 1);          // Bottom-left
    *Grid_at(curr, width - 1, height - 1) = Grid_getAt(curr, 1, 1);                  // Bottom-right

    // Compute next generation and write to next_states
    for (i32 y = 1; y < height - 1; ++y) {
        for (i32 x = 1; x < width - 1; ++x) {
            // clang-format off
            // The secret of artificial life =================================================
            let neighbors_num
                = (Grid_getAt(curr, x - 1, y - 1) + Grid_getAt(curr, x, y - 1) + Grid_getAt(curr, x + 1, y - 1))
                + (Grid_getAt(curr, x - 1, y)     + 0                          + Grid_getAt(curr, x + 1, y))
                + (Grid_getAt(curr, x - 1, y + 1) + Grid_getAt(curr, x, y + 1) + Grid_getAt(curr, x + 1, y + 1));
            // clang-format on

            if (*Grid_at(curr, x, y) == 1) {
                *Grid_at(next, x, y) = as$(i8, neighbors_num == 2 || neighbors_num == 3);
            } else {
                *Grid_at(next, x, y) = as$(i8, neighbors_num == 3);
            }
        }
    }

    // Copy next_states back to curr_states to complete the generation update
    memcpy(self->curr_states.ptr, self->next_states.ptr, self->width * self->height * sizeof(i8));
}
fn_(GameOfLife_entireCells(const GameOfLife* self), Grid$i8) {
    debug_assert_nonnull(self);
    return Grid_fromSli$(Grid$i8, self->curr_states, self->width, self->height);
}
fn_(GameOfLife_setCell(GameOfLife* self, usize x, usize y, i8 state), void) {
    debug_assert_nonnull(self);
    // Add boundary checks
    if (self->width - 2 <= x || self->height - 2 <= y) { return; } /* Silently fail */

    let width  = as$(i32, self->width);
    let height = as$(i32, self->height);
    let curr   = Grid_fromSli$(Grid$i8, self->curr_states, width, height);

    *Grid_at(curr, x + 1, y + 1) = state;
}
fn_(GameOfLife_setCellSlice(GameOfLife* self, usize left_top_x, usize left_top_y, Sli$i8 cells), void) {
    debug_assert_nonnull(self);
    // Add boundary checks
    if (self->width - 2 <= left_top_x || self->height - 2 <= left_top_y) { return; } /* Silently fail */

    let width  = as$(i32, self->width);
    let height = as$(i32, self->height);
    let curr   = Grid_fromSli$(Grid$i8, self->curr_states, width, height);
    let len    = prim_min(cells.len, as$(usize, width - left_top_x - 2)); // Fix: Adjust limit

    for (usize i = 0; i < len; ++i) {
        *Grid_at(curr, left_top_x + i + 1, left_top_y + 1) = *Sli_at(cells, i);
    }
}
fn_(GameOfLife_setCellGrid(GameOfLife* self, usize left_top_x, usize left_top_y, Grid$i8 cells), void) {
    debug_assert_nonnull(self);
    // Add boundary checks
    if (self->width - 2 <= left_top_x || self->height - 2 <= left_top_y) { return; } /* Silently fail */

    let width  = as$(i32, self->width);
    let height = as$(i32, self->height);
    let curr   = Grid_fromSli$(Grid$i8, self->curr_states, width, height);
    let area_w = prim_min(cells.width, as$(usize, width - left_top_x - 1));
    let area_h = prim_min(cells.height, as$(usize, height - left_top_y - 1));
    for (usize y = 0; y < area_h; ++y) {
        for (usize x = 0; x < area_w; ++x) {
            *Grid_at(curr, left_top_x + x + 1, left_top_y + y + 1) = Grid_getAt(cells, x, y);
        }
    }
}
fn_(GameOfLife_toggleCell(GameOfLife* self, usize x, usize y, i8 state), void) {
    debug_assert_nonnull(self);
    // Add boundary checks
    if (self->width - 2 <= x || self->height - 2 <= y) { return; } /* Silently fail */

    let width  = as$(i32, self->width);
    let height = as$(i32, self->height);
    let curr   = Grid_fromSli$(Grid$i8, self->curr_states, width, height);

    *Grid_at(curr, x + 1, y + 1) = as$(i8, Grid_getAt(curr, x + 1, y + 1) ^ state);
}
fn_(GameOfLife_toggleCellSlice(GameOfLife* self, usize left_top_x, usize left_top_y, Sli$i8 cells), void) {
    debug_assert_nonnull(self);
    // Add boundary checks
    if (self->width - 2 <= left_top_x || self->height - 2 <= left_top_y) { return; } /* Silently fail */

    let width  = as$(i32, self->width);
    let height = as$(i32, self->height);
    let curr   = Grid_fromSli$(Grid$i8, self->curr_states, width, height);
    let len    = prim_min(cells.len, as$(usize, width - left_top_x - 1));

    for (usize i = 0; i < len; ++i) {
        let toggled = Grid_getAt(curr, left_top_x + i + 1, left_top_y + 1) ^ Sli_getAt(cells, i);

        *Grid_at(curr, left_top_x + i + 1, left_top_y + 1) = as$(i8, toggled);
    }
}
fn_(GameOfLife_toggleCellGrid(GameOfLife* self, usize left_top_x, usize left_top_y, Grid$i8 cells), void) {
    debug_assert_nonnull(self);
    // Add boundary checks
    if (self->width - 2 <= left_top_x || self->height - 2 <= left_top_y) { return; } /* Silently fail */

    let width  = as$(i32, self->width);
    let height = as$(i32, self->height);
    let curr   = Grid_fromSli$(Grid$i8, self->curr_states, width, height);
    let area_w = prim_min(cells.width, as$(usize, width - left_top_x - 1));
    let area_h = prim_min(cells.height, as$(usize, height - left_top_y - 1));

    for (usize y = 0; y < area_h; ++y) {
        for (usize x = 0; x < area_w; ++x) {
            let toggled = Grid_getAt(curr, left_top_x + x + 1, left_top_y + y + 1) ^ Grid_getAt(cells, x, y);

            *Grid_at(curr, left_top_x + x + 1, left_top_y + y + 1) = as$(i8, toggled);
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
