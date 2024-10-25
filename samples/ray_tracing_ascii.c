// build `clang -xc ray_tracing_ascii.c ../src/*.c -o ray_tracing_ascii -O3 -static`
// run with `./ray_tracing_ascii <width:100> <height:50>`


#include <assert.h>
#include <conio.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NOMINMAX
#include <windows.h>


#define PP_UnusedParam(_param) ((void)(_param))
#define PP_ignoreReturn(_func) (void)_func
#define PtrT(_Type)            _Type*
#define RefT(_Type)            _Type* const
#define PP_swap(_Type, _a, _b)   \
    do {                         \
        PtrT(_Type) __a = &(_a); \
        PtrT(_Type) __b = &(_b); \
        _Type __t       = *__a;  \
        *__a            = *__b;  \
        *__b            = __t;   \
    } while (false)


typedef struct Vec3      Vec3;
typedef struct Direction Direction;

struct Vec3 {
    float x, y, z;
};
void      Vec3_normalize(Vec3* self);
float     Vec3_length(Vec3* self);
void      Vec3_add(Vec3* self, Vec3 v);
void      Vec3_scale(Vec3* self, float s);
Vec3      Vec3_scaled(Vec3* self, float s);
float     Vec3_dot(Vec3* self, Vec3 other);
float     Vec3_dist(Vec3* self, Vec3 other);
Direction Vec3_to_direction(Vec3* self);

struct Direction {
    float ang_v;
    float ang_h;
};
Vec3 Direction_to_unit(Direction* self);


// helper functions
char  ray_char(Vec3* ray, int refl);
bool  ray_done(Vec3* ray);
char* setc(int row, int col);
bool  key_is_pressed();


typedef struct Ball Ball;
typedef struct Game Game;

struct Ball {
    Vec3  center;
    float radius;
};
Vec3 Ball_reflect(Ball* self, Vec3 incoming, Vec3 move);

#define MOVE_ANGLE    (0.01f)
#define MOVE_POSITION (0.03f)

#define RAYSTEP  (0.02f)
#define RAYSTEPS (5000)

struct Game {
    Ball      balls[32];
    int       balls_size;
    Vec3      pos;
    Direction dir;
    float     width, height;
    int       xres, yres;
};
// Game(Vec3 start_pos, Direction start_dir, float width, float height, int xres, int yres) {
//     this->width  = width;
//     this->height = height;
//     this->pos    = start_pos;
//     this->dir    = start_dir;
//     this->xres   = xres;
//     this->yres   = yres;
// }
#define Game(...) ((Game){ __VA_ARGS__ })
void Game_add_ball(Game* self, Ball b);
void Game_make_pic(Game* self);
void Game_start(Game* self);
void Game_move_view(Game* self, int key);
void Game_move_position(Game* self, int key);
bool Game_check_reflections(Game* self, Vec3* ray, Vec3* move);



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
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
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
    // system("cls"); // NOLINT
    HANDLE                     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD                      topLeft  = (COORD){ 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO screen   = (CONSOLE_SCREEN_BUFFER_INFO){ 0 };
    DWORD                      written  = 0;

    GetConsoleScreenBufferInfo(hConsole, &screen);
    FillConsoleOutputCharacterA(hConsole, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(hConsole, topLeft);
#else
    system("clear"); // NOLINT
#endif
    // printf(
    //     "\033[2J" // clear screen
    //     "\033[H" // move cursor to top left
    // );
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
    Terminal_clear();
}

static CHAR_INFO  Display_clearBuffer[640 * 400] = { 0 };
static CHAR_INFO  Display_frontBuffer[640 * 400] = { 0 };
static CHAR_INFO  Display_backBuffer[640 * 400]  = { 0 };
static CHAR_INFO* Display_bufferCurrent          = Display_frontBuffer;
static CHAR_INFO* Display_bufferNext             = Display_backBuffer;
static int        Display_bufferSize             = 640 * 400;

static HANDLE     Display_windowOutputHandle = NULL;
static SHORT      Display_windowWidth        = 80;
static SHORT      Display_windowHeight       = 25;
static COORD      Display_windowSize         = (COORD){ 0, 0 };
static COORD      Display_windowCoord        = (COORD){ 0, 0 };
static SMALL_RECT Display_windowRect         = (SMALL_RECT){ 0, 0, 1, 1 };

void Display_init(int width, int height) {
#ifdef _WIN32
    Display_windowOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    Display_windowRect         = (SMALL_RECT){ 0, 0, 1, 1 };

    SetConsoleWindowInfo(Display_windowOutputHandle, TRUE, &Display_windowRect);

    // void SetConsoleScreenBuffer()
    Display_windowWidth  = (SHORT)width;
    Display_windowHeight = (SHORT)height;
    Display_windowSize   = (COORD){ (SHORT)Display_windowWidth, (SHORT)Display_windowHeight };
    SetConsoleActiveScreenBuffer(Display_windowOutputHandle);
    SetConsoleScreenBufferSize(Display_windowOutputHandle, Display_windowSize);
    Display_bufferSize = Display_windowWidth * Display_windowHeight;

    // void SetConsoleFontSize()
    CONSOLE_FONT_INFOEX fontInfo = (CONSOLE_FONT_INFOEX){ 0 };
    fontInfo.cbSize              = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(Display_windowOutputHandle, FALSE, &fontInfo);
    fontInfo.dwFontSize.X = 8;
    fontInfo.dwFontSize.Y = 16;
    SetCurrentConsoleFontEx(Display_windowOutputHandle, FALSE, &fontInfo);

    // void SetConsoleWindowSize()
    Display_windowRect = (SMALL_RECT){ 0, 0, (SHORT)(Display_windowWidth - 1), (SHORT)(Display_windowHeight - 1) };
    SetConsoleWindowInfo(Display_windowOutputHandle, TRUE, &Display_windowRect);

    // void DisableConsoleCursor()
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(Display_windowOutputHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(Display_windowOutputHandle, &cursorInfo);

    for (int i = 0; i < Display_bufferSize; ++i) {
        Display_clearBuffer[i].Char.UnicodeChar = ' ';
        Display_clearBuffer[i].Attributes       = 0x00;
    }
#endif
}

void Display_clear() {
    memcpy(Display_bufferCurrent, Display_clearBuffer, Display_bufferSize * sizeof(CHAR_INFO));
}

void Display_swapBuffers() {
    PP_swap(CHAR_INFO*, Display_bufferCurrent, Display_bufferNext);
}

void Display_render() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleOutputW(hConsole, Display_bufferCurrent, Display_windowSize, Display_windowCoord, &Display_windowRect);
#endif
}

int main(int argc, char* argv[]) {
    Vec3      start_pos = (Vec3){ 0, 0, 1 };
    Direction start_dir = (Direction){ -0.2f, 0 };

    int width  = 0;
    int height = 0;
    if (argc == 1) {
        // no window sizes given, defaults to 60x30
        width  = 60;
        height = 30;
    } else {
        width  = atoi(argv[1]);
        height = atoi(argv[2]);
    }

    Game game = Game(.pos = start_pos, .dir = start_dir, .width = 2, .height = 2, .xres = width, .yres = height);

    // add some balls and start the "game"
    Ball b = { { 5, 0, 2 }, 2 };
    Game_add_ball(&game, b);
    Ball c = { { 10, 0, 2 }, 2 };
    Game_add_ball(&game, c);
    Ball d = { { 7.5f, 0, 8 }, 4 };
    Game_add_ball(&game, d);

    Terminal_bootup();
    Display_init(width, height);
    Game_start(&game);
    Terminal_shutdown();

    return 0;
}



void Vec3_normalize(Vec3* self) {
    float len = Vec3_length(self);
    self->x /= len;
    self->y /= len;
    self->z /= len;
}

float Vec3_length(Vec3* self) {
    return sqrtf(self->x * self->x + self->y * self->y + self->z * self->z);
}

void Vec3_add(Vec3* self, Vec3 v) {
    self->x += v.x;
    self->y += v.y;
    self->z += v.z;
}

void Vec3_scale(Vec3* self, float s) {
    self->x *= s;
    self->y *= s;
    self->z *= s;
}

Vec3 Vec3_scaled(Vec3* self, float s) {
    return (Vec3){
        .x = self->x * s,
        .y = self->y * s,
        .z = self->z * s
    };
}

float Vec3_dot(Vec3* self, Vec3 other) {
    return self->x * other.x + self->y * other.y + self->z * other.z;
}

float Vec3_dist(Vec3* self, Vec3 other) {
    return sqrtf((self->x - other.x) * (self->x - other.x) + (self->y - other.y) * (self->y - other.y) + (self->z - other.z) * (self->z - other.z));
}

Direction to_direction(Vec3* self) {
    return (Direction){
        .ang_v = atanf(self->z / (self->x * self->x + self->y * self->y)),
        .ang_h = atan2f(self->y, self->x)
    };
}

Vec3 Direction_to_unit(Direction* self) {
    return (Vec3){
        .x = cosf(self->ang_v) * cosf(self->ang_h),
        .y = cosf(self->ang_v) * sinf(self->ang_h),
        .z = sinf(self->ang_v)
    };
}

// determines character to be printed for finished ray
char ray_char(Vec3* ray, int refl) {
    char chars[] = { '.', '-', ',' };
    if (ray->z <= 0 && abs(((int)floorf(ray->x)) - ((int)floorf(ray->y))) % 2 == 0) {
        return '#';
    }
    if (refl > 0) {
        if (refl < 4) {
            return chars[refl - 1];
        }
        return '+';
    }
    return ' ';
}

// ray ends when it hits the floor at z = 0
bool ray_done(Vec3* ray) {
    return ray->z <= 0;
}

// for setting position of cursor in terminal window
char* setc(int row, int col) {
    static char buffer[32] = { 0 };
    int         len        = snprintf(buffer, sizeof(buffer), "\033[%d;%dH", row, col);
    buffer[len]            = '\0';
    return buffer;
}

bool key_is_pressed() {
    return kbhit();
}

Vec3 Ball_reflect(Ball* self, Vec3 incoming, Vec3 move) {
    Vec3_scale(&self->center, -1);
    Vec3_add(&incoming, self->center);
    Vec3_scale(&self->center, -1);
    Vec3_normalize(&incoming);

    Vec3_scale(&incoming, -2 * Vec3_dot(&incoming, move));
    Vec3 new_move = move;

    Vec3_add(&new_move, incoming);

    return new_move;
}

void Game_add_ball(Game* self, Ball b) {
    self->balls[self->balls_size] = b;
    self->balls_size++;
}

void Game_make_pic(Game* self) {
    Display_clear();

    // rays through equidistant points on width*height rectangle with distance 1 from viewer
    Vec3 v1 = Direction_to_unit(&self->dir);
    // v2 points from middle of the rectangle to upper edge
    Vec3 v2 = {
        -tanf(self->dir.ang_v) * v1.x,
        -tanf(self->dir.ang_v) * v1.y,
        cosf(self->dir.ang_v)
    };
    Vec3_scale(&v2, self->height / 2);
    // v3 points from middle of rectangle to left edge
    Vec3 v3 = { -v1.y, v1.x, 0 };
    Vec3_normalize(&v3);
    Vec3_scale(&v3, self->width / 2);

    for (int row = 0; row < self->yres; ++row) {
        for (int col = 0; col < self->xres; ++col) {
            float up_offset   = -((float)row / ((float)self->yres - 1.0f) - 0.5f);
            float left_offset = (float)col / ((float)self->xres - 1.0f) - 0.5f;
            Vec3  move        = v1;

            Vec3_add(&move, Vec3_scaled(&v2, up_offset));
            Vec3_add(&move, Vec3_scaled(&v3, left_offset));
            Vec3_normalize(&move);
            Vec3_scale(&move, RAYSTEP);

            Vec3 ray = self->pos;
            // trace ray
            float dists_to_balls[32]  = { 0 };
            int   dists_to_balls_size = 0;
            for (int i = 0; i < self->balls_size; ++i) {
                dists_to_balls[i] = 0;
            }
            int times_reflected = 0;
            for (int i = 0; i < RAYSTEPS; ++i) {
                if (ray_done(&ray)) {
                    break;
                }

                int ball_index = 0;
                for (Ball* b = self->balls; b < self->balls + self->balls_size; ++b) {
                    float d                    = Vec3_dist(&ray, b->center) - b->radius;
                    dists_to_balls[ball_index] = d;
                    if (d < 0) {
                        move = Ball_reflect(b, ray, move);
                        times_reflected++;
                    }
                    ball_index++;
                }

                // optimization: test if all distances are large enough to make
                // multiple steps at once
                float min_dist = ray.z;
                for (float* f = dists_to_balls; f < dists_to_balls + dists_to_balls_size; ++f) {
                    if (*f < min_dist) {
                        min_dist = *f;
                    }
                }

                if (min_dist > RAYSTEP) {
                    int possible_steps = (int)(min_dist / RAYSTEP);
                    i += possible_steps - 1; // -1 because of default increment
                    Vec3_add(&ray, Vec3_scaled(&move, (float)possible_steps));
                } else {
                    Vec3_add(&ray, move);
                }
            }
            Display_bufferNext[row * self->xres + col].Char.UnicodeChar = (WCHAR)ray_char(&ray, times_reflected);
            Display_bufferNext[row * self->xres + col].Attributes       = (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        }
    }

    Display_swapBuffers();
}

void Game_start(Game* self) {
    int keys[]    = { VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT };
    int keys_size = sizeof(keys) / sizeof(keys[0]);

    while (true) {
        Game_make_pic(self);
        for (int i = 0; i < keys_size; ++i) {
            if ((unsigned)GetAsyncKeyState(keys[i]) & 0x8000u) {
                if ((unsigned)GetAsyncKeyState(VK_LSHIFT) & 0x8000u) {
                    Game_move_view(self, keys[i]);
                } else {
                    Game_move_position(self, keys[i]);
                }
            }
        }
        Display_render();
        Sleep(20);
    }
}

void Game_move_view(Game* self, int key) {
    if (key == VK_UP) {
        self->dir.ang_v += MOVE_ANGLE;
    } else if (key == VK_DOWN) {
        self->dir.ang_v -= MOVE_ANGLE;
    }
    if (key == VK_LEFT) {
        self->dir.ang_h -= MOVE_ANGLE;
    }
    if (key == VK_RIGHT) {
        self->dir.ang_h += MOVE_ANGLE;
    }
}

void Game_move_position(Game* self, int key) {
    Vec3  dir_vect = Direction_to_unit(&self->dir);
    float xmov     = dir_vect.x;
    float ymov     = dir_vect.y;
    float scale    = 1 / sqrtf(xmov * xmov + ymov * ymov);
    xmov *= scale;
    ymov *= scale;
    xmov *= MOVE_POSITION;
    ymov *= MOVE_POSITION;
    if (key == VK_UP) {
        // move forward
        self->pos.x += xmov;
        self->pos.y += ymov;
    } else if (key == VK_DOWN) {
        // move back
        self->pos.x -= xmov;
        self->pos.y -= ymov;
    }
    if (key == VK_LEFT) {
        // move left
        self->pos.x += ymov;
        self->pos.y -= xmov;
    }
    if (key == VK_RIGHT) {
        // move right
        self->pos.x -= ymov;
        self->pos.y += xmov;
    }
}

bool Game_check_reflections(Game* self, Vec3* ray, Vec3* move) {
    // checks if ray has to be reflected on one of the objects, changes dir accordingly
    for (Ball* b = self->balls; b < self->balls + self->balls_size; ++b) {
        if (Vec3_dist(ray, b->center) < b->radius) {
            *move = Ball_reflect(b, *ray, *move);
            return true; // only one reflection
        }
    }
    return false;
}
