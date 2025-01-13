#include "State.h"

#include <stdlib.h>
#include <math.h>

Err$Ptr$game_State game_State_create(void) {
    reserveReturn(Err$Ptr$game_State);

    let state = (game_State*)malloc(sizeof(game_State));
    if (!state) { return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory)); }

    state->player_x     = 5.09f;
    state->player_y     = 14.7f;
    state->player_angle = 0.0f;
    state->fov          = math_f32_pi / 4.0f;
    state->depth        = 16.0f;
    state->map_width    = 16;
    state->map_height   = 16;

    // Initialize map (static for simplicity)
    static const char map_data[]
        = "#########......."
          "#..............."
          "#.......########"
          "#..............#"
          "#......##......#"
          "#......##......#"
          "#..............#"
          "###.#..........#"
          "#...#..........#"
          "#.###..###...###"
          "#.#....#.......#"
          "#..............#"
          "#......#.......#"
          "#......#########"
          "#..............#"
          "################";
    state->map        = map_data;
    state->is_running = true;

    return_ok(state);
}

void game_State_destroy(game_State* state) {
    if (!state) { return; }
    free(state);
}

void game_State_update(game_State* state, f32 elapsed_time) {
    if (engine_Key_pressed(engine_KeyCode_esc)) {
        state->is_running = false;
        return;
    }

    if (engine_Key_held(engine_KeyCode_a) || engine_Key_held(engine_KeyCode_arrow_left)) {
        state->player_angle -= 2.0f * elapsed_time;
    }
    if (engine_Key_held(engine_KeyCode_d) || engine_Key_held(engine_KeyCode_arrow_right)) {
        state->player_angle += 2.0f * elapsed_time;
    }

    const f32 move_speed = 5.0f * elapsed_time;
    const f32 cos_angle  = cosf(state->player_angle);
    const f32 sin_angle  = sinf(state->player_angle);

    if (engine_Key_held(engine_KeyCode_w) || engine_Key_held(engine_KeyCode_arrow_up)) {
        const f32 new_x = state->player_x + cos_angle * move_speed;
        const f32 new_y = state->player_y + sin_angle * move_speed;

        // Collision check
        if (state->map[as(i32, new_x) + as(i32, new_y) * state->map_width] != '#') {
            state->player_x = new_x;
            state->player_y = new_y;
        }
    }
    if (engine_Key_held(engine_KeyCode_s) || engine_Key_held(engine_KeyCode_arrow_down)) {
        const f32 new_x = state->player_x - cos_angle * move_speed;
        const f32 new_y = state->player_y - sin_angle * move_speed;

        if (state->map[as(i32, new_x) + as(i32, new_y) * state->map_width] != '#') {
            state->player_x = new_x;
            state->player_y = new_y;
        }
    }
}
