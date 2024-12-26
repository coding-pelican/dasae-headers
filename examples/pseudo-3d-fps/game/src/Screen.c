#include "Screen.h"
#include <math.h>

void game_Screen_renderFirstPersonView(Ptr_engine_Canvas canvas, PtrConst_game_State state) {
    const i32 width  = as(i32, canvas.addr->width);
    const i32 height = as(i32, canvas.addr->height);

    for (i32 x = 0; x < width; ++x) {
        // Calculate ray angle
        const f32 ray_angle = (state.addr->player_angle - state.addr->fov / 2.0f) + (as(f32, x) / as(f32, width)) * state.addr->fov;

        // Cast ray
        const f32 eye_x    = sinf(ray_angle);
        const f32 eye_y    = cosf(ray_angle);
        f32       distance = 0.0f;
        bool      hit_wall = false;

        while (!hit_wall && distance < state.addr->depth) {
            distance += 0.1f;
            const i32 test_x = (i32)(state.addr->player_x + eye_x * distance);
            const i32 test_y = (i32)(state.addr->player_y + eye_y * distance);

            if (test_x < 0 || test_x >= state.addr->map_width || test_y < 0 || test_y >= state.addr->map_height) {
                hit_wall = true;
                distance = state.addr->depth;
            } else if (state.addr->map[test_x * state.addr->map_width + test_y] == '#') {
                hit_wall = true;
            }
        }

        // Calculate wall height
        const i32 ceiling = as(i32, as(f32, height) / 2.0f - as(f32, height) / distance);
        const i32 floor   = height - ceiling;

        // Draw vertical line
        for (i32 y = 0; y < height; ++y) {
            engine_ColorValue color;

            if (y <= ceiling) {
                // Sky - Gradient from darker to lighter blue
                const f32 sky_fade = as(f32, y) / as(f32, ceiling);
                color.rgba.r       = as(u8, 100 * sky_fade);
                color.rgba.g       = as(u8, 150 * sky_fade);
                color.rgba.b       = 255;
                color.rgba.a       = 255;
            } else if (y > ceiling && y <= floor) {
                // Wall - Shade based on distance with slight color variation
                const f32 brightness = 1.0f - (distance / state.addr->depth);
                const u8  base_shade = as(u8, 255.0f * brightness);

                // Add slight color variation based on wall orientation
                const f32 angle_factor = fabsf(eye_x) / (fabsf(eye_x) + fabsf(eye_y));
                color.rgba.r           = base_shade;
                color.rgba.g           = as(u8, base_shade * (0.9f + angle_factor * 0.1f));
                color.rgba.b           = as(u8, base_shade * (0.8f + angle_factor * 0.2f));
                color.rgba.a           = 255;
            } else {
                // Floor - Gradient with distance
                const f32 floor_dist  = 1.0f - (as(f32, y - height / 2.0f) / as(f32, height / 2.0f));
                const u8  floor_shade = as(u8, floor_dist * 128.0f);
                color.rgba.r          = floor_shade;
                color.rgba.g          = as(u8, floor_shade * 0.7f);
                color.rgba.b          = as(u8, floor_shade * 0.5f);
                color.rgba.a          = 255;
            }

            engine_Canvas_drawPixel(canvas, x, y, color);
        }
    }
}

void game_Screen_renderMinimap(Ptr_engine_Canvas canvas, PtrConst_game_State state) {
    // Calculate scaling factors
    const f32 scale_x = as(f32, canvas.addr->width) / as(f32, state.addr->map_width);
    const f32 scale_y = as(f32, canvas.addr->height) / as(f32, state.addr->map_height);

    // Draw map
    for (i32 y = 0; y < state.addr->map_height; ++y) {
        for (i32 x = 0; x < state.addr->map_width; ++x) {
            engine_ColorValue color;

            if (state.addr->map[y * state.addr->map_width + x] == '#') {
                // Wall tiles - White with slight blue tint
                color.rgba.r = 240;
                color.rgba.g = 240;
                color.rgba.b = 255;
                color.rgba.a = 255;
            } else {
                // Floor tiles - Dark gray with slight depth
                color.rgba.r = 40;
                color.rgba.g = 40;
                color.rgba.b = 50;
                color.rgba.a = 255;
            }

            // Draw scaled tile
            for (i32 sy = 0; sy < as(i32, scale_y); ++sy) {
                for (i32 sx = 0; sx < as(i32, scale_x); ++sx) {
                    engine_Canvas_drawPixel(
                        canvas,
                        as(i32, (as(f32, x) * scale_x + as(f32, sx))),
                        as(i32, (as(f32, y) * scale_y + as(f32, sy))),
                        color
                    );
                }
            }
        }
    }

    // Draw player position and direction indicator
    const i32 player_screen_x = (i32)(state.addr->player_x * scale_x);
    const i32 player_screen_y = (i32)(state.addr->player_y * scale_y);

    // Draw player direction line
    const i32 line_length = 5;
    const i32 dir_x       = player_screen_x + (i32)(sinf(state.addr->player_angle) * (f32)line_length);
    const i32 dir_y       = player_screen_y + (i32)(cosf(state.addr->player_angle) * (f32)line_length);

    // Player marker (red dot with orange direction line)
    engine_ColorValue player_color = {
        .rgba.r = 255,
        .rgba.g = 0,
        .rgba.b = 0,
        .rgba.a = 255
    };

    engine_ColorValue direction_color = {
        .rgba.r = 255,
        .rgba.g = 165,
        .rgba.b = 0,
        .rgba.a = 255
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

void game_Screen_renderUi(Ptr_engine_Canvas canvas, PtrConst_game_State state) {
    unused(state);

    // Crosshair configuration
    const engine_ColorValue ui_color = {
        .rgba.r = 255,
        .rgba.g = 255,
        .rgba.b = 255,
        .rgba.a = 192 // Semi-transparent
    };

    const i32 center_x            = as(i32, canvas.addr->width) / 2;
    const i32 center_y            = as(i32, canvas.addr->height) / 2;
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
