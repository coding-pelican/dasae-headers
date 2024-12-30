#include "Screen.h"
#include <math.h>

#define PI (3.14159265358979323846)

void game_Screen_renderFirstPersonView(engine_Canvas* canvas, const game_State* state) {
    for (usize x = 0; x < canvas->width; ++x) {
        // Calculate ray angle - matches original implementation
        const f32 ray_angle = (state->player_angle - state->fov * 0.5f) + (as(f32, x) / as(f32, canvas->width)) * state->fov;

        // Use sin/cos matching the original implementation
        const f32 eye_x = sinf(ray_angle); // Changed to sin
        const f32 eye_y = cosf(ray_angle); // Changed to cos

        f32  distance_to_wall = 0.0f;
        bool hits_wall        = false;

        // Ray casting
        while (!hits_wall && distance_to_wall < state->depth) {
            distance_to_wall += 0.1f; // Match original step size
            const i32 test_x = as(i32, state->player_x + eye_x * distance_to_wall);
            const i32 test_y = as(i32, state->player_y + eye_y * distance_to_wall);

            // Bounds checking
            if (test_x < 0 || test_x >= state->map_width || test_y < 0 || test_y >= state->map_height) {
                hits_wall        = true;
                distance_to_wall = state->depth;
            } else {
                // Fixed map indexing to match original
                if (state->map[test_x * state->map_width + test_y] == '#') {
                    hits_wall = true;
                }
            }
        }

        // Calculate wall height - note the y-axis inversion handling
        const i32 ceiling = as(i32, (as(f32, canvas->height) / 2.0f) - (as(f32, canvas->height) / distance_to_wall));
        const i32 floor   = as(i32, canvas->height) - ceiling;

        // Draw vertical slice
        for (usize y = 0; y < canvas->height; ++y) {
            Color color = Color_transparent;

            // Ceiling (sky)
            if (as(i32, y) <= ceiling) {
                // Sky gradient - darker at top, lighter at horizon
                f32 t   = 1.0f - (as(f32, y) / as(f32, ceiling));
                color.r = as(u8, 50 * t);
                color.g = as(u8, 100 * t);
                color.b = as(u8, 200 * t);
                color.a = ColorChannel_max_value;
            }
            // Walls
            else if (ceiling < as(i32, y) && as(i32, y) <= floor) {
                if (distance_to_wall < state->depth) {
                    // Match original shading logic but with RGB values
                    f32 brightness = 0.0f;
                    if (distance_to_wall <= state->depth / 4.0f) {
                        brightness = 1.0f; // Fullest block
                    } else if (distance_to_wall < state->depth / 3.0f) {
                        brightness = 0.8f; // Dark shade
                    } else if (distance_to_wall < state->depth / 2.0f) {
                        brightness = 0.6f; // Medium shade
                    } else if (distance_to_wall < state->depth) {
                        brightness = 0.4f; // Light shade
                    } else {
                        brightness = 0.2f; // Distant walls
                    }

                    color.r = as(u8, brightness * ColorChannel_max_value);
                    color.g = as(u8, brightness * ColorChannel_max_value);
                    color.b = as(u8, brightness * ColorChannel_max_value);
                    color.a = ColorChannel_max_value;
                }
            }
            // Floor
            else {
                // Match original floor shading logic
                f32 b = 1.0f - (as(f32, y) - as(f32, canvas->height / 2.0)) / (as(f32, canvas->height / 2.0));

                f32 brightness = 0.0f;
                if (b < 0.25f) {
                    brightness = 0.9f; // Closest floor sections
                } else if (b < 0.5f) {
                    brightness = 0.7f;
                } else if (b < 0.75f) {
                    brightness = 0.5f;
                } else if (b < 0.9f) {
                    brightness = 0.3f;
                } else {
                    brightness = 0.1f; // Distant floor
                }

                color.r = as(u8, brightness * ColorChannel_max_value);
                color.g = as(u8, (brightness * 0.8f) * ColorChannel_max_value); // Slight color tint
                color.b = as(u8, (brightness * 0.6f) * ColorChannel_max_value);
                color.a = ColorChannel_max_value;
            }

            // Draw pixel with y-coordinate properly inverted for screen space
            const i32 screen_y = as(i32, canvas->height) - 1 - as(i32, y);
            engine_Canvas_drawPixel(canvas, as(i32, x), screen_y, color);
        }
    }
}

void game_Screen_renderMinimap(engine_Canvas* canvas, const game_State* state) {
    // Calculate scaling factors
    const f32 scale_x = as(f32, canvas->width) / as(f32, state->map_width);
    const f32 scale_y = as(f32, canvas->height) / as(f32, state->map_height);

    // Draw map tiles
    for (i32 y = 0; y < state->map_height; ++y) {
        for (i32 x = 0; x < state->map_width; ++x) {
            // Calculate pixel ranges for this tile
            i32 start_px = as(i32, as(f32, x) * scale_x);
            i32 end_px   = as(i32, as(f32, x + 1) * scale_x);
            i32 start_py = as(i32, as(f32, y) * scale_y);
            i32 end_py   = as(i32, as(f32, y + 1) * scale_y);

            Color color;
            if (state->map[y * state->map_width + x] == '#') {
                color = (Color){
                    .channels = { 240, 240, ColorChannel_max_value, ColorChannel_max_value }
                };
            } else {
                color = (Color){
                    .channels = { 40, 40, 50, ColorChannel_max_value }
                };
            }

            // Fill entire tile area
            for (i32 py = start_py; py < end_py; ++py) {
                for (i32 px = start_px; px < end_px; ++px) {
                    if (px < as(i32, canvas->width) && py < as(i32, canvas->height)) {
                        engine_Canvas_drawPixel(canvas, px, py, color);
                    }
                }
            }
        }
    }

    // Draw player position and direction indicator
    const i32 player_screen_x = (i32)(state->player_x * scale_x);
    const i32 player_screen_y = (i32)(state->player_y * scale_y);

    // Draw player direction line
    const i32 line_length = 5;
    const i32 dir_x       = player_screen_x + (i32)(sinf(state->player_angle) * (f32)line_length);
    const i32 dir_y       = player_screen_y + (i32)(cosf(state->player_angle) * (f32)line_length);

    // Player marker (red dot with orange direction line)
    Color player_color = {
        .channels = {
            ColorChannel_max_value,
            0,
            0,
            ColorChannel_max_value,
        }
    };

    Color direction_color = {
        .channels = {
            ColorChannel_max_value,
            165,
            0,
            ColorChannel_max_value,
        }
    };

    // Draw player marker (3x3 pixel dot)
    for (i32 dy = -1; dy <= 1; ++dy) {
        for (i32 dx = -1; dx <= 1; ++dx) {
            engine_Canvas_drawPixel(
                canvas,
                player_screen_x + dx,
                player_screen_y + dy,
                player_color
            );
        }
    }

    // Draw direction line
    engine_Canvas_drawPixel(canvas, dir_x, dir_y, direction_color);
}

void game_Screen_renderUi(engine_Canvas* canvas, const game_State* state) {
    unused(state);

    // Crosshair configuration
    const Color ui_color = {
        .channels = {
            ColorChannel_max_value,
            ColorChannel_max_value,
            ColorChannel_max_value,
            192, // Semi-transparent
        }
    };

    const i32 center_x            = as(i32, canvas->width) / 2;
    const i32 center_y            = as(i32, canvas->height) / 2;
    const i32 crosshair_size      = 5;
    const i32 crosshair_thickness = 1;

    // Draw horizontal line of crosshair
    for (i32 y = -crosshair_thickness; y <= crosshair_thickness; ++y) {
        for (i32 x = -crosshair_size; x <= crosshair_size; ++x) {
            engine_Canvas_drawPixel(
                canvas,
                center_x + x,
                center_y + y,
                ui_color
            );
        }
    }

    // Draw vertical line of crosshair
    for (i32 x = -crosshair_thickness; x <= crosshair_thickness; ++x) {
        for (i32 y = -crosshair_size; y <= crosshair_size; ++y) {
            engine_Canvas_drawPixel(
                canvas,
                center_x + x,
                center_y + y,
                ui_color
            );
        }
    }
}
