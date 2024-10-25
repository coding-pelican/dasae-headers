// build `clang -xc fireworks.c ../src/*.c -o fireworks -DNDEBUG -O3 -static`
// run with `.\launcher fireworks 160 50`

/* use anyhow::Result;
use crossterm::style::Print;
use crossterm::terminal::{ Clear, ClearType };
use crossterm::{ cursor, execute, terminal };
use pixel_loop::canvas::CrosstermCanvas;
use pixel_loop::input::{ CrosstermInputState, KeyboardKey, KeyboardState };
use pixel_loop::{ Canvas, Color, HslColor, RenderableCanvas }; */


#include "../src/assert.h"
#include "../src/canvas.h"
#include "../src/color.h"
#include "../src/common.h"
#include "../src/mem.h"
#include "../src/primitive_types.h"
#include "../src/terminal.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>


f64 Rand_random_f64() { return (f64)rand() / (f64)RAND_MAX; }
u8  Rand_random_u8() { return (u8)rand(); }
u32 Rand_random_u32() { return (u32)rand(); }


#define Firework_effects_max        (25)
#define Firework_effects_per_rocket (25)
#define Fireworks_max               (16)


typedef struct Particle Particle;
struct Particle {
    f64   position[2];
    f64   speed[2];
    f64   acceleration[2];
    f64   fading;
    f64   lifetime;
    Color color;
    u32   dimensions[2];
};
#define Particle_(...) makeWith(Particle, __VA_ARGS__)
Particle* Particle_init(Ref(Particle) self, f64 x, f64 y, f64 width, f64 height, Color color) {
    *self = Particle_(
            .position     = { x, y },
            .speed        = { 0.0, 0.0 },
            .acceleration = { 0.0, 0.0 },
            .fading       = 0.0,
            .lifetime     = 1.0,
            .color        = color,
            .dimensions   = { width, height }
    );
    return self;
}
#define comptime_Particle_make(_x, _y, _width, _height, _color) ( \
    Particle_(                                                    \
            .position     = { (f64)(_x), (f64)(_y) },             \
            .speed        = { 0.0, 0.0 },                         \
            .acceleration = { 0.0, 0.0 },                         \
            .fading       = 0.0,                                  \
            .lifetime     = 1.0,                                  \
            .color        = (_color),                             \
            .dimensions   = { (u32)(_width), (u32)(_height) }     \
    )                                                             \
)
Particle* Particle_withSpeed(Ref(Particle) self, f64 x, f64 y) {
    assertNotNull(self);
    self->speed[0] = x;
    self->speed[1] = y;
    return self;
}
Particle* Particle_withAcceleration(Ref(Particle) self, f64 x, f64 y) {
    assertNotNull(self);
    self->acceleration[0] = x;
    self->acceleration[1] = y;
    return self;
}
Particle* Particle_withFading(Ref(Particle) self, f64 fading) {
    assertNotNull(self);
    self->fading = fading;
    return self;
}
bool Particle_isDead(const Ref(Particle) self) {
    assertNotNull(self);
    return self->lifetime <= 0.0;
}
void Particle_update(Ref(Particle) self) {
    assertNotNull(self);
    if (Particle_isDead(self)) { return; }

    self->speed[0] += self->acceleration[0];
    self->speed[1] += self->acceleration[1];

    self->position[0] += self->speed[0];
    self->position[1] += self->speed[1];

    self->lifetime -= self->fading;
}
void Particle_render(const Ref(Particle) self, Ref(Canvas) canvas) {
    assertNotNull(self);
    if (Particle_isDead(self)) { return; }

    const Color renderColor = Color_fromOpaque(
        (u8)((f64)(self->color.r) * self->lifetime),
        (u8)((f64)(self->color.g) * self->lifetime),
        (u8)((f64)(self->color.b) * self->lifetime)
    );

    Canvas_fillRect(
        canvas,
        Canvas_normalizeRect(
            canvas,
            comptime_Rect_from(
                (i64)self->position[0],
                (i64)self->position[1],
                (u32)self->dimensions[0],
                (u32)self->dimensions[1]
            )
        ),
        renderColor
    );

    /* impl Particle {
    pub fn render<C : Canvas>(&self, canvas : &mut C) -> Result<()> {
        if self {
            .lifetime <= 0.0 {
                return Ok(());
            }
        }

        // @HACK: PixelLoop with CrosstermCanvas does not support proper alpha
        // blending at the moment. Therefore we calculate the coler against a
        // given base (black) and the lifetime as opacity and apply it.
        let render_color = Color::from_rgb(
            (self.color.r as f64 * self.lifetime) as u8,
            (self.color.g as f64 * self.lifetime) as u8,
            (self.color.b as f64 * self.lifetime) as u8,
        );

        canvas.filled_rect(
            self.position .0.round() as i64,
            self.position .1.round() as i64,
            self.dimensions .0,
            self.dimensions .1,
            &render_color,
        );
        Ok(())
    }
} */
}


typedef struct Firework Firework;
struct Firework {
    Particle* rocket;
    Particle  effect[Firework_effects_max];
    i64       effect_count;
    HSL       effect_base_color;
};
#define Firework_(...) makeWith(Firework, __VA_ARGS__)
Firework* Firework_init(Ref(Firework) self, i64 x, i64 y, Color effect_base_color) {
    *self = Firework_(
            .effect_count      = 0,
            .effect_base_color = Color_intoHSL(effect_base_color)
    );
    // Particle* const rocket = mem_createWith(
    //     Particle,
    //     .position   = { x, y },
    //     .dimensions = { 1, 3 },
    //     .color      = Color_white
    // );
    // Particle_withSpeed(rocket, 0.0, -2.0 - Rand_Random_f64() * -1.0);
    // Particle_withAcceleration(rocket, 0.0, 0.02);
    // firework.rocket = rocket;
    Particle* const rocket = mem_create(Particle);
    Particle_init(rocket, (f64)x, (f64)y, 1, 3, Color_white);
    Particle_withSpeed(rocket, 0.0, -2.0 - Rand_random_f64() * -1.0);
    Particle_withAcceleration(rocket, 0.0, 0.02);
    self->rocket = rocket;
    // .with_speed(0.0, -2.0 - rand::random::<f64>() * -1.0)
    return self;

    /*
    pub fn new (x : i64, y : i64, effect_base_color : Color)->Self {
        let rocket = Some(
            Particle::new (x, y, 1, 3, Color::from_rgb(255, 255, 255))
                // Rocket flies upwards with gravity pulling it down.
                // Initial speed slightly randomized.
                .with_acceleration(0.0, 0.02),
        );

        Self {
            rocket,
                effect : vec ![],
                         effect_base_color : effect_base_color.as_hsl(),
        }
    }
    */
}
void Firework_update(Ref(Firework) self) {
    if (self->rocket) {
        Particle** rocket = &self->rocket;
        Particle_update(rocket[0]);

        if (-0.2 <= rocket[0]->speed[1]) {
            for (i64 i = 0; i < Firework_effects_per_rocket; ++i) {
                if (Firework_effects_max <= self->effect_count) { break; }

                i64 const x      = (i64)rocket[0]->position[0];
                i64 const y      = (i64)rocket[0]->position[1];
                i64 const width  = 1;
                i64 const height = 1;
                HSL const color  = HSL_from(
                    self->effect_base_color.h,
                    self->effect_base_color.s + (Rand_random_f64() - 0.5) * 20.0,
                    self->effect_base_color.l + (Rand_random_f64() - 0.5) * 40.0
                );
                Particle particle[1] = { comptime_Particle_make(x, y, width, height, HSL_intoColorOpaque(color)) };
                Particle_withSpeed(
                    particle,
                    (Rand_random_f64() - 0.5) * 1.0,
                    (Rand_random_f64() - 0.9) * 1.0
                );
                Particle_withAcceleration(particle, 0.0, 0.02);
                Particle_withFading(particle, 0.01);
                self->effect[self->effect_count++] = particle[0];
            }
            mem_destroy(rocket);
            self->rocket = null;
        }
    }

    for (Particle* particle = self->effect; particle < self->effect + self->effect_count; ++particle) {
        Particle_update(particle);
    }

    /* pub fn update(&mut self) {
        if let {
            Some(ref mut rocket) = self.rocket {
                rocket.update();

                if rocket {
                    .speed .1 >= -0.2 {
                        // Rocket has reached its peak and is now exploding.
                        // Create a bunch of particles to simulate the explosion.
                    for{
                        _ in 0..25 {
                            let x      = rocket.position .0 as i64;
                            let y      = rocket.position .1 as i64;
                            let width  = 1;
                            let height = 1;
                            // Randomize color based on the base color of the rocket. using the hsl form
                            // of the color.
                            let color = HslColor::new (
                                self.effect_base_color.h,
                                self.effect_base_color.s + (rand::random::<f64>() - 0.5) * 20.0,
                                self.effect_base_color.l + (rand::random::<f64>() - 0.5) * 40.0,
                            );

                            let particle = Particle::new (x, y, width, height, color.into())
                                               .with_speed(
                                                   (rand::random::<f64>() - 0.5) * 1.0,
                                                   (rand::random::<f64>() - 0.9) * 1.0,
                                               )
                                               .with_acceleration(0.0, 0.02)
                                               .with_fading(0.01);
                            self.effect.push(particle);
                        }
                    }
                    self.rocket = None;
                    }
                }
            }
        }

        for{
            particle in& mut self.effect {
                particle.update();
            }
        }
    } */
}
void Firework_render(const Ref(Firework) self, Ref(Canvas) canvas) {
    if (self->rocket) {
        Particle_render(self->rocket, canvas);
    }
    for (const Particle* particle = self->effect; particle < self->effect + self->effect_count; ++particle) {
        Particle_render(particle, canvas);
    }
}
static bool Firework__deadsAllEffect(const Ref(Firework) self) {
    for (const Particle* particle = self->effect; particle < self->effect + self->effect_count; ++particle) {
        if (!Particle_isDead(particle)) {
            return false;
        }
    }
    return true;
}
bool Firework_isDead(const Ref(Firework) self) {
    return self->rocket == null && (self->effect_count == 0 || Firework__deadsAllEffect(self));
}

typedef struct State State;
struct State {
    Firework fireworks[Fireworks_max];
    i64      fireworksCount;
    u32      width;
    u32      height;
};
#define State_(...) makeWith(State, __VA_ARGS__)
State* State_init(State* self, u32 width, u32 height) {
    *self = State_(
            .fireworksCount = 0,
            .width          = width,
            .height         = height,
    );
    return self;
}
void State_fini(State* self) {
    for (i64 i = 0; i < self->fireworksCount; ++i) {
        if (!self->fireworks[i].rocket) { continue; }
        mem_delete(&self->fireworks[i].rocket);
    }
}


#define Terminal_Width  (80ull * 2)
#define Terminal_Height (25ull * 2)
#define Terminal_Size   (Terminal_Width * Terminal_Height)

#define Terminal_FontWidth  (6)
#define Terminal_FontHeight (Terminal_FontWidth * 2)

#define Display_Width  (Terminal_Width)
#define Display_Height (Terminal_Height * 2)
#define Display_Size   (Display_Width * Display_Height)

void TerminalCursor_resetColor() {
    printf("\033[0m");
}

void TerminalCursor_setColor(Color foreground, Color background) {
    printf(
        "\033[38;2;%d;%d;%d;48;2;%d;%d;%dm", // background
        foreground.r,
        foreground.g,
        foreground.b,
        background.r,
        background.g,
        background.b
    );
}

// Set locale to UTF-8
void Terminal_ensureLocaleUTF8() {
    static const char* const locales[]  = { "en_US.UTF-8", "C.UTF-8", ".UTF-8", "" };
    static const int         localesNum = sizeof(locales) / sizeof(locales[0]);

    const char* settedLocale = NULL;

    for (int i = 0; i < localesNum; ++i) {
        settedLocale = setlocale(LC_ALL, locales[i]);
        if (settedLocale) {
            printf("Successfully set locale to: %s\n", settedLocale);
            break;
        }
    }

    if (!settedLocale) {
        printf("Warning: Failed to set UTF-8 locale. Using system default.\n");
        settedLocale = setlocale(LC_ALL, "");
        if (!settedLocale) {
            perror("Failed to set locale\n");
            // Not exiting here, as the program might still work
        }
    }

#ifdef _WIN32
    // Set console output to UTF-8 on Windows
    if (!SetConsoleOutputCP(CP_UTF8)) {
        perror("Failed to set console output to UTF-8\n");
        // Not exiting here, as the program might still work
    }
#endif
}

// Enable ANSI escape sequence processing
void Terminal_enableANSI() {
#ifdef _WIN32
    HANDLE hOut   = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
    SetConsoleMode(hOut, dwMode);
#else
    // Not needed for Linux as it supports ANSI escape sequences by default
#endif
}

// Print diagnostic information for debugging
void Terminal_printDiagnosticInformation() {
    printf("Current locale: %s\n", setlocale(LC_ALL, NULL));
#ifdef _WIN32
    printf("Active code page: %d\n", GetACP());
    printf("Output code page: %d\n", GetConsoleOutputCP());
#endif
}

// Clear the terminal screen and move the cursor to the top left
void Terminal_clear() {
#ifdef _WIN32
    HANDLE                     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD                      topLeft  = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO screen   = { 0 };
    DWORD                      written  = { 0 };

    GetConsoleScreenBufferInfo(hConsole, &screen);
    FillConsoleOutputCharacter(hConsole, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, 0, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    // FillConsoleOutputAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(hConsole, topLeft);
#else
    // Clear screen and scrollback buffer
    printf("\033[2J\033[3J\033[H");
#endif
}

void Terminal_bootup() {
    Terminal_ensureLocaleUTF8();
    Terminal_enableANSI();
    Terminal_printDiagnosticInformation();
    printf("Terminal setup complete.\n");
    printf("Press any key to continue...\n");
    getch();
    Terminal_clear();
}

void Terminal_shutdown() {
    TerminalCursor_resetColor();
    Terminal_clear();
}

#define Display_UnitPixel1x2Format            "\033[38;2;%d;%d;%d;48;2;%d;%d;%dm▀"
#define Display_UnitPixel1x2FormatMaxCase     "\033[38;2;255;255;255;48;2;255;255;255m▀"
#define Display_UnitPixel1x2FormatMaxCaseSize (sizeof(Display_UnitPixel1x2FormatMaxCase) / sizeof(Display_UnitPixel1x2FormatMaxCase[0]))
#define Display_BufferSize                    ((usize)Terminal_Size * (usize)Display_UnitPixel1x2FormatMaxCaseSize)

static char  Display_frontBuffer[Display_BufferSize] = { 0 };
static char  Display_backBuffer[Display_BufferSize]  = { 0 };
static char* Display_bufferCurrent                   = Display_frontBuffer;
static usize Display_bufferCurrentSize               = 0;
static char* Display_bufferNext                      = Display_backBuffer;
static usize Display_bufferNextSize                  = 0;

void Display_init() {
#ifdef _WIN32
    HANDLE     hConsoleOutput    = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSizeInitial = (SMALL_RECT){ 0, 0, 1, 1 };

    SetConsoleWindowInfo(hConsoleOutput, TRUE, &windowSizeInitial);

    // void SetConsoleScreenBuffer()
    COORD dwSize = (COORD){ (SHORT)Terminal_Width, (SHORT)Terminal_Height };
    SetConsoleActiveScreenBuffer(hConsoleOutput);
    SetConsoleScreenBufferSize(hConsoleOutput, dwSize);

    // void SetConsoleFontSize()
    CONSOLE_FONT_INFOEX fontInfo = (CONSOLE_FONT_INFOEX){ 0 };
    fontInfo.cbSize              = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsoleOutput, FALSE, &fontInfo);
    fontInfo.dwFontSize.X = Terminal_FontWidth;
    fontInfo.dwFontSize.Y = Terminal_FontHeight;
    SetCurrentConsoleFontEx(hConsoleOutput, FALSE, &fontInfo);

    // void SetConsoleWindowSize()
    SMALL_RECT windowSize = (SMALL_RECT){ 0, 0, (SHORT)Terminal_Width - 1, (SHORT)Terminal_Height - 1 };
    SetConsoleWindowInfo(hConsoleOutput, TRUE, &windowSize);

    // void DisableConsoleCursor()
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
#endif
}

void Display_clear() {
    memset(Display_bufferCurrent, 0, Display_bufferCurrentSize);
}

void Display_swapBuffers() {
    pp_swap(char*, Display_bufferCurrent, Display_bufferNext);
    pp_swap(usize, Display_bufferCurrentSize, Display_bufferNextSize);
}

void Display_setBufferFromColors(const Color colors[Display_Size]) {
    Display_clear();

    usize index = 0;
    for (usize y = 0; y < Display_Height; y += 2) {
        usize x = 0;
        while (x < Display_Width) {
            // Ensure we don't go out of bounds
            if (Display_Height <= (y + 1)) { break; }

            const Color upper = colors[x + y * Display_Width];
            const Color lower = colors[x + (y + 1) * Display_Width];

            // Start a run of characters with the same color
            usize runLength = 1;
            while ((x + runLength) < (usize)Display_Width) {
                const Color nextUpper = colors[x + runLength + y * Display_Width];
                const Color nextLower = colors[x + runLength + (y + 1) * Display_Width];

                if (memcmp(&upper.rgba, &nextUpper.rgba, sizeof(Color)) != 0 ||
                    memcmp(&lower.rgba, &nextLower.rgba, sizeof(Color)) != 0) {
                    break;
                }
                runLength++;
            }

            // Construct ANSI escape sequence once for the run
            const int formattedSize = sprintf(
                Display_bufferNext + index,
                "\033[38;2;%d;%d;%d;48;2;%d;%d;%dm",
                upper.r,
                upper.g,
                upper.b,
                lower.r,
                lower.g,
                lower.b
            );
            index += formattedSize;

            // Append the block character '▀' runLength times
            const char* const blockChar     = "▀"; // Multibyte character
            const usize       blockCharSize = strlen(blockChar);

            // Copy the multibyte character multiple times
            for (usize i = 0; i < runLength; ++i) {
                memcpy(Display_bufferNext + index, blockChar, blockCharSize);
                index += blockCharSize;
            }

            x += runLength;
        }
        // Append newline at the end of each line
        Display_bufferNext[index++] = '\n';
    }
    Display_bufferNext[--index] = '\0';
    Display_bufferNextSize      = index;

    Display_swapBuffers();
}

void Display_render() {
    // Reset cursor position
    printf("\033[H");

#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  written  = 0;
    WriteConsoleA(hConsole, Display_bufferCurrent, (DWORD)Display_bufferCurrentSize, &written, NULL);
#else
    fwrite(Display_bufferCurrent, sizeof(Display_bufferCurrent[0]), Display_bufferCurrentSize, stdout);
    fflush(stdout);
#endif
}


int main() {
    State state[1] = { makeCleared(State) };
    State_init(state, Display_Width, Display_Height);
    Canvas canvas[1] = { makeCleared(Canvas) };
    Canvas_init(canvas, state->width, state->height);
    Canvas_initWithColor(canvas, Color_black);

    Display_init();
    Terminal_bootup();
    srand((u32)time(null)); // NOLINT

    while (true) {
        if (GetAsyncKeyState(VK_ESCAPE)) { break; }

        // Add a new rocket with with 5% chance.
        if (Rand_random_f64() < 0.05) {
            if (state->fireworksCount < Fireworks_max) {
                state->fireworks[state->fireworksCount++] = *Firework_init(
                    &makeCleared(Firework),
                    (i64)(Rand_random_u32() % state->width),
                    state->height,
                    Color_fromOpaque(
                        Rand_random_u8(),
                        Rand_random_u8(),
                        Rand_random_u8()
                    )
                );
            }
        }

        for (i64 i = 0; i < state->fireworksCount; ++i) {
            Firework_update(&state->fireworks[i]);
        }
        // RENDER BEGIN
        Canvas_clear(canvas, Color_black);
        for (i64 i = 0; i < state->fireworksCount; ++i) {
            Firework_render(&state->fireworks[i], canvas);
        }
        // Canvas_Render(canvas);
        Display_setBufferFromColors(FrameBuffer_readData(Canvas_readBuffer(canvas)));
        Display_render();
        // RENDER END

        Sleep(16);
    }

    State_fini(state);
    Canvas_fini(canvas);
    Terminal_shutdown();

    return 0;
}




// void Display_Render() {
//     write(STDOUT_FILENO, Display_bufferCurrent, Display_bufferCurrentSize);
// }

// // Define the display buffer using CHAR_INFO
// static CHAR_INFO Display_buffer[Display_Size];
// static HANDLE    hConsole = NULL;

// // Initialize the console handle and buffer
// void Display_Init() {
// #ifdef _WIN32
//     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//     if (hConsole == INVALID_HANDLE_VALUE) {
//         perror("GetStdHandle failed");
//         exit(EXIT_FAILURE);
//     }

//     // Set up the console screen buffer
//     COORD      bufferSize = { .X = Display_Width, .Y = Display_Height };
//     SMALL_RECT windowSize = { .Left = 0, .Top = 0, .Right = Display_Width - 1, .Bottom = Display_Height - 1 };
//     SetConsoleScreenBufferSize(hConsole, bufferSize);
//     SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

//     // Disable the cursor for better performance
//     CONSOLE_CURSOR_INFO cursorInfo;
//     GetConsoleCursorInfo(hConsole, &cursorInfo);
//     cursorInfo.bVisible = FALSE;
//     SetConsoleCursorInfo(hConsole, &cursorInfo);
// #endif
// }

// void Display_SetBufferFromColors(const Color colors[Display_Size]) {
//     for (i32 y = 0; y < Display_Height; y++) {
//         for (i32 x = 0; x < Display_Width; ++x) {
//             i32   index = y * Display_Width + x;
//             Color color = colors[x + y * Display_Width];

//             // Set the character to a space with the desired foreground and background colors
//             Display_buffer[index].Char.AsciiChar = ' ';
//             Display_buffer[index].Attributes     = (color.r << 16) | (color.g << 8) | (color.b);
//         }
//     }
// }

// void Display_Render() {
// #ifdef _WIN32
//     COORD      bufferSize  = { .X = Display_Width, .Y = Display_Height };
//     COORD      bufferCoord = { .X = 0, .Y = 0 };
//     SMALL_RECT writeRegion = { .Left = 0, .Top = 0, .Right = Display_Width - 1, .Bottom = Display_Height - 1 };

//     // Write the buffer to the console
//     if (!WriteConsoleOutputA(
//             hConsole,
//             Display_buffer,
//             bufferSize,
//             bufferCoord,
//             &writeRegion
//         )) {
//         perror("WriteConsoleOutputA failed");
//         exit(EXIT_FAILURE);
//     }
// #else
//     // Existing non-Windows rendering code
//     if (!fwrite(Display_buffer, sizeof(Display_bufferCurrent[0]), Display_bufferCurrentSize, stdout)) {
//         assert(false);
//     }
// #endif
// }

// void Display_Clear() {
//     memset(Display_buffer, 0, sizeof(Display_buffer));
// }




/* fn main() -> Result<()> {
    let(terminal_width, terminal_height) = terminal::size() ? ;
    let width                            = terminal_width;
    let height                           = terminal_height * 2;

    let mut canvas = CrosstermCanvas::new (width, height);
    canvas.set_refresh_limit(120);

    let state = State::new (width as u32, height as u32);

    let input = CrosstermInputState::new ();

    pixel_loop::run(
        60,
        state,
        input,
        canvas,
        | e,
        s,
        input,
        canvas | {
            let width = canvas.width();
            let height = canvas.height();

            if input{.is_key_pressed(KeyboardKey::Q) {
                // @HACK until we refactored PixelLoop to allow for a clean
                // exit.
                let mut stdout = std::io::stdout();
                execute!(
                    stdout,
                    Clear(ClearType::All), // Clear all on screen
                    cursor::MoveTo(0, 0),  // Reset cursor position
                    Print("\x1b[!p"),      // Soft terminal reset (DECSTR)
                    Print("\x1bc"),        // Full terminal reset (RIS)
                )?;
                crossterm::terminal::disable_raw_mode()?;
                std::process::exit(0);
            }}

            // eprintln!("Active fireworks: {}", s.fireworks.len());

            // Remove dead fireworks.
            s.fireworks.retain(|f| !f.is_dead());

            // Add a new rocket with with 5% chance.
            if rand{::random::<f64>() < 0.05 {
                let x = (rand::random::<u32>() % width) as i64;
                let y = height as i64;
                let effect_base_color = Color::from_rgb(
                    rand::random::<u8>(),
                    rand::random::<u8>(),
                    rand::random::<u8>(),
                );

                s.fireworks.push(Firework::new(x, y, effect_base_color));
            }
}

            for{ firework in &mut s.fireworks {
                firework.update();
            }
}

            Ok(()) },
        | e,
        s,
        i,
        canvas,
        dt | {
            // RENDER BEGIN
            canvas.clear_screen(&Color::from_rgb(0, 0, 0));

            for{ firework in &s.fireworks {
                firework.render(canvas)?;
            }
}

            // RENDER END

            canvas.render()?;

            Ok(()) },
    )
        ? ;
    Ok(())
} */
