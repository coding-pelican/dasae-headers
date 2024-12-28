#include "State.h"

#include <stdlib.h>
#include <math.h>

#define engine_KeyCode_W (0x57)
#define engine_KeyCode_A (0x41)
#define engine_KeyCode_S (0x53)
#define engine_KeyCode_D (0x44)

Err$Ptr$game_State game_State_create(void) {
    reserveReturn(Err$Ptr$game_State);

    let state = (game_State*)malloc(sizeof(game_State));
    if (!state) { return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory)); }

    state->player_x     = 14.7f;
    state->player_y     = 5.09f;
    state->player_angle = 0.0f;
    state->fov          = 3.14159f / 4.0f;
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
    state->map = map_data;

    return_ok(state);
}

void game_State_update(game_State* state, f32 elapsed_time) {
    // Platform-independent key state checking would go here
    // For this example, we'll assume a platform-specific engine_Key_pressed() function

    if (engine_Key_pressed(engine_KeyCode_A)) {
        state->player_angle -= 2.0f * elapsed_time;
    }
    if (engine_Key_pressed(engine_KeyCode_D)) {
        state->player_angle += 2.0f * elapsed_time;
    }

    const f32 move_speed = 5.0f * elapsed_time;
    const f32 sin_angle  = sinf(state->player_angle);
    const f32 cos_angle  = cosf(state->player_angle);

    if (engine_Key_pressed(engine_KeyCode_W)) {
        const f32 new_x = state->player_x + sin_angle * move_speed;
        const f32 new_y = state->player_y + cos_angle * move_speed;

        // Collision check
        if (state->map[as(i32, new_x) * state->map_width + as(i32, new_y)] != '#') {
            state->player_x = new_x;
            state->player_y = new_y;
        }
    }
    if (engine_Key_pressed(engine_KeyCode_S)) {
        const f32 new_x = state->player_x - sin_angle * move_speed;
        const f32 new_y = state->player_y - cos_angle * move_speed;

        if (state->map[as(i32, new_x) * state->map_width + as(i32, new_y)] != '#') {
            state->player_x = new_x;
            state->player_y = new_y;
        }
    }
}
