#include "State.h"

#include <stdlib.h>
#include <math.h>

#define engine_KeyCode_W (0x57)
#define engine_KeyCode_A (0x41)
#define engine_KeyCode_S (0x53)
#define engine_KeyCode_D (0x44)

ResErr_Ptr_game_State game_State_create(void) {
    game_State* const state = (game_State*)malloc(sizeof(game_State));
    if (!state) { return ResErr_Ptr_game_State_err(Err_OutOfMemory); }
    const Ptr_game_State self = Ptr_game_State_from(state);

    self.addr->player_x     = 14.7f;
    self.addr->player_y     = 5.09f;
    self.addr->player_angle = 0.0f;
    self.addr->fov          = 3.14159f / 4.0f;
    self.addr->depth        = 16.0f;
    self.addr->map_width    = 16;
    self.addr->map_height   = 16;

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
    self.addr->map = map_data;

    return ResErr_Ptr_game_State_ok(self);
}

void game_State_update(Ptr_game_State state, f32 elapsed_time) {
    // Platform-independent key state checking would go here
    // For this example, we'll assume a platform-specific engine_Key_pressed() function

    if (engine_Key_pressed(engine_KeyCode_A)) {
        state.addr->player_angle -= 2.0f * elapsed_time;
    }
    if (engine_Key_pressed(engine_KeyCode_D)) {
        state.addr->player_angle += 2.0f * elapsed_time;
    }

    const f32 move_speed = 5.0f * elapsed_time;
    const f32 sin_angle  = sinf(state.addr->player_angle);
    const f32 cos_angle  = cosf(state.addr->player_angle);

    if (engine_Key_pressed(engine_KeyCode_W)) {
        const f32 new_x = state.addr->player_x + sin_angle * move_speed;
        const f32 new_y = state.addr->player_y + cos_angle * move_speed;

        // Collision check
        if (state.addr->map[as(i32, new_x) * state.addr->map_width + as(i32, new_y)] != '#') {
            state.addr->player_x = new_x;
            state.addr->player_y = new_y;
        }
    }
    if (engine_Key_pressed(engine_KeyCode_S)) {
        const f32 new_x = state.addr->player_x - sin_angle * move_speed;
        const f32 new_y = state.addr->player_y - cos_angle * move_speed;

        if (state.addr->map[as(i32, new_x) * state.addr->map_width + as(i32, new_y)] != '#') {
            state.addr->player_x = new_x;
            state.addr->player_y = new_y;
        }
    }
}
