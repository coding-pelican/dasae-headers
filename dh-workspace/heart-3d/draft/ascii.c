#include "engine.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define window_width  (80)
#define window_height (25)

#define DEPTH_STEPS     (12)
#define STEP_SIZE       (0.01)
#define BEAT_FREQUENCY  (6.0)
#define ANIMATION_DELAY (3000)

static const char shading[]   = " .,-~:;=!*#$@";
static const i32  shading_len = countOf(shading) - 2;

// Rotation angles and scaling factor
static f64 angleX = 0, angleY = 0, angleZ = 0;
static f64 scale          = 1.0;
static f64 scaleDirection = 0.001; // Controls scale pulsing

// 3D rotation matrices
void rotate3D(f64 x, f64 y, f64 z, f64* nx, f64* ny, f64* nz) {
    // Rotate around X axis
    f64 y1 = y * cos(angleX) - z * sin(angleX);
    f64 z1 = y * sin(angleX) + z * cos(angleX);

    // Rotate around Y axis
    f64 x2 = x * cos(angleY) - z1 * sin(angleY);
    f64 z2 = x * sin(angleY) + z1 * cos(angleY);

    // Rotate around Z axis
    *nx = x2 * cos(angleZ) - y1 * sin(angleZ);
    *ny = x2 * sin(angleZ) + y1 * cos(angleZ);
    *nz = z2;
}

f64 calculateHeartRadius(f64 t, f64 y) {
    return 0.4 + 0.05 * pow(0.5 + 0.5 * sin(t * BEAT_FREQUENCY + y * 2), 8);
}

f64 calculateHeartShape(f64 x, f64 y, f64 r) {
    return -x * x - pow(1.2 * y - fabs(x) * 2.0 / 3.0, 2) + r * r;
}

void renderFrame(f64 t) {
    f64 zBuffer[window_width * window_height] = { 0 };
    f64 maxZ                                  = 0;

    // Update scale with pulsing effect
    scale += scaleDirection;
    if (scale > 1.2 || scale < 0.8) {
        scaleDirection = -scaleDirection;
    }

    // Scan through y-axis
    for (f64 y = -0.5; y <= 0.5; y += STEP_SIZE) {
        f64 radius = calculateHeartRadius(t, y);

        // Scan through x-axis
        for (f64 x = -0.5; x <= 0.5; x += STEP_SIZE) {
            f64 z = calculateHeartShape(x, y, radius);

            if (z < 0) {
                continue;
            }

            z = sqrt(z) / (2.0 - y);

            // Render through the depth of the heart
            for (f64 tz = -z; tz <= z; tz += z / DEPTH_STEPS) {
                // Apply scaling
                f64 sx = x * scale;
                f64 sy = y * scale;
                f64 sz = tz * scale;

                // Apply 3D rotation
                f64 rotX = 0.0;
                f64 rotY = 0.0;
                f64 rotZ = 0.0;
                rotate3D(sx, sy, sz, &rotX, &rotY, &rotZ);

                // Apply perspective projection
                f64 perspective = 1.0 + rotZ / 2.0;
                i32 screenX     = lround((rotX * perspective + 0.5) * (window_width - 20) + 10);
                i32 screenY     = lround((-rotY * perspective + 0.5) * (window_height - 1) + 2);

                // Check bounds and update z-buffer
                if (screenX >= 0 && screenX < window_width && screenY >= 0 && screenY < window_height) {
                    i32 index = screenX + screenY * window_width;
                    if (zBuffer[index] <= rotZ) {
                        zBuffer[index] = rotZ;
                        maxZ           = fmax(maxZ, rotZ);
                    }
                }
            }
        }
    }

    // Render frame
    printf("\x1b[H");
    for (i32 i = 0; i < window_width * window_height; ++i) {
        if (i % window_width == 0 && i != 0) {
            putchar('\n');
        } else {
            i32 shadeIndex = lround((zBuffer[i] / maxZ) * shading_len);
            putchar(shading[shadeIndex]);
        }
    }
}

int main(void) {
    // Setup terminal
    printf("\x1b[2J\x1b[?25l"); // Clear screen and hide cursor

    // Animation loop
    f64 t = 0;
    while (true) {
        renderFrame(t);

        // Update rotation angles for smooth 360-degree rotation
        angleX += 0.02;
        angleY += 0.03;
        angleZ += 0.01;

        // Keep angles in range [0, 2π]
        angleX = fmod(angleX, 2.0 * math_pi);
        angleY = fmod(angleY, 2.0 * math_pi);
        angleZ = fmod(angleZ, 2.0 * math_pi);

        t += 0.003;
        time_sleep(time_Duration_fromMicros(ANIMATION_DELAY));
    }

    return 0;
}
