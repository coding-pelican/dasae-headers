/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    sample-tetris.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-19 (date of creation)
 * @updated 2025-03-27 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/samples
 * @prefix  tetris
 *
 * @brief   Tetris game
 * @details Controls:
 *   Arrow Left/Right - Move piece
 *   Arrow Down - Move piece down faster
 *   Z - Rotate piece
 */

/* Includes =================================================================*/

#include "dh/main.h"

#include "dh/ptr.h"
#include "dh/sli.h"
#include "dh/Grid.h"
#include "dh/Str.h"

#include "dh/mem.h"
#include "dh/Arr.h"
#include "dh/ArrList.h"
#include "dh/heap/Page.h"

#include "dh/time.h"
#include "dh/Rand.h"

#if plat_is_windows
#include "dh/os/windows.h"
#else /* others */
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif /* others */

/* Templates ================================================================*/

use_Grid$(u8);
use_ArrList$(i32);

/* Constants ================================================================*/

#define tetris_screen_width (80)
#define tetris_screen_height (30)
#define tetris_field_width (12ull)
#define tetris_field_height (18ull)

/* Game Types ===============================================================*/

typedef enum tetris_Tetrominos {
    tetris_Tetrominos_i = 0,
    tetris_Tetrominos_j = 1,
    tetris_Tetrominos_o = 2,
    tetris_Tetrominos_l = 3,
    tetris_Tetrominos_s = 4,
    tetris_Tetrominos_t = 5,
    tetris_Tetrominos_z = 6,
    tetris_Tetrominos_count = 7
} tetris_Tetrominos;

/// Global variables for tetromino definitions
static const A$$(tetris_Tetrominos_count, u16) tetris_tetrominos = A_init({
    [tetris_Tetrominos_i] = lit_num(0b, 0010, 0010, 0010, 0010), /* I: ..X...X...X...X. */
    [tetris_Tetrominos_j] = lit_num(0b, 0000, 0010, 0110, 0010), /* J: ..X..XX...X..... */
    [tetris_Tetrominos_o] = lit_num(0b, 0000, 0000, 0110, 0110), /* O: .....XX..XX..... */
    [tetris_Tetrominos_l] = lit_num(0b, 0000, 0010, 0110, 0100), /* L: ..X..XX..X...... */
    [tetris_Tetrominos_s] = lit_num(0b, 0000, 0100, 0110, 0010), /* S: .X...XX...X..... */
    [tetris_Tetrominos_t] = lit_num(0b, 0000, 0100, 0100, 1100), /* T: .X...X...XX..... */
    [tetris_Tetrominos_z] = lit_num(0b, 0000, 0010, 0110, 0010)  /* Z: ..X...X..XX..... */
});

typedef struct tetris_PlayField {
    Grid$u8 grid;
} tetris_PlayField;
use_E$(tetris_PlayField);

/* Function Prototypes ======================================================*/

static fn_((tetris_Console_bootup(void))(E$void)) $must_check;
static fn_((tetris_Console_shutdown(void))(void));

static fn_((tetris_isKeyPressed(i32 key))(bool));

static fn_((tetris_PlayField_init(mem_Allocator allocator))(E$tetris_PlayField)) $must_check;
static fn_((tetris_PlayField_drawScreen(const tetris_PlayField* field, i32 current_piece, i32 rotation, i32 pos_x, i32 pos_y, i32 score))(E$void)) $must_check;
static fn_((tetris_rotate(i32 px, i32 py, i32 r))(i32));
static fn_((tetris_PlayField_doesPieceFit(const tetris_PlayField* field, i32 tetromino, i32 rotation, i32 pos_x, i32 pos_y))(bool));
static fn_((tetris_PlayField_clearLines(tetris_PlayField* field, ArrList$i32* lines))(E$i32)) $must_check;
static fn_((tetris_PlayField_lockPiece(tetris_PlayField* field, i32 piece, i32 rotation, i32 pos_x, i32 pos_y))(void));

#if plat_is_windows
use_S$(wchar);
static HANDLE tetris_Console_output_handle = null;
static S$wchar tetris_Console_screen_buffer = cleared();
#else  /* others */
struct termios tetris_Console_original = cleared();
#endif /* others */

/* Main Function ============================================================*/

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    /* Initialize random number generator */
    Rand_init();

    /* Initialize heap allocator */
    var allocator = heap_Page_allocator(create$(heap_Page));

    /* Initialize play field */
    var field = try_(tetris_PlayField_init(allocator));
    defer_(mem_Allocator_free(allocator, anySli(field.grid.items)));

    /* Initialize console/screen */
    try_(tetris_Console_bootup());
    defer_(tetris_Console_shutdown());

    /* Initialize line clearing storage */
    var lines = type$(ArrList$i32, try_(ArrList_initCap(typeInfo$(i32), allocator, 4)));
    defer_(ArrList_fini(lines.base));

    /* Game state variables */
    bool is_gameover = false;
    i32 current_piece = as$(i32)(Rand_range$i64(0, tetris_Tetrominos_count - 1));
    i32 current_rotation = 0;
    i32 current_x = (tetris_field_width / 2) - 2;
    i32 current_y = 0;
    i32 speed = 20;
    i32 speed_count = 0;
    i32 score = 0;
    i32 piece_count = 0;
    bool force_downs = false;
    bool rotate_holds = true;

    /* Main game loop */
    while (!is_gameover) {
        /* Timing - sleep for 50ms (20fps) */
        time_sleep(time_Duration_fromMillis(50));
        speed_count++;
        force_downs = (speed_count == speed);

        /* Handle Input */
        let key_left = tetris_isKeyPressed(1);
        let key_right = tetris_isKeyPressed(2);
        let key_down = tetris_isKeyPressed(3);
        let key_rotate = tetris_isKeyPressed(4);

        /* Game Logic - Movement */
        if (key_right
            && tetris_PlayField_doesPieceFit(&field, current_piece, current_rotation, current_x + 1, current_y)) {
            current_x++;
        }
        if (key_left
            && tetris_PlayField_doesPieceFit(&field, current_piece, current_rotation, current_x - 1, current_y)) {
            current_x--;
        }
        if (key_down
            && tetris_PlayField_doesPieceFit(&field, current_piece, current_rotation, current_x, current_y + 1)) {
            current_y++;
        }

        /* Rotation with anti-spinning latch */
        if (key_rotate) {
            if (rotate_holds && tetris_PlayField_doesPieceFit(&field, current_piece, current_rotation + 1, current_x, current_y)) {
                current_rotation++;
            }
            rotate_holds = false;
        } else {
            rotate_holds = true;
        }

        /* Force piece down */
        if (force_downs) {
            speed_count = 0;
            piece_count++;

            /* Update difficulty every 50 pieces */
            if (piece_count % 50 == 0 && 10 <= speed) { speed--; }

            /* Check if piece can move down */
            if (tetris_PlayField_doesPieceFit(&field, current_piece, current_rotation, current_x, current_y + 1)) {
                current_y++;
            } else {
                /* Lock piece in place */
                tetris_PlayField_lockPiece(&field, current_piece, current_rotation, current_x, current_y);

                /* Check for lines and clear them */
                let linesCleared = try_(tetris_PlayField_clearLines(&field, &lines));

                /* Update score */
                score += 25; /* Points for placing piece */
                if (linesCleared > 0) {
                    /* Points for clearing lines - 2^lines * 100 */
                    score += (1 << linesCleared) * 100;
                }

                /* Pick new piece */
                current_x = (tetris_field_width / 2) - 2;
                current_y = 0;
                current_rotation = 0;
                current_piece = as$(i32)(Rand_range$i64(0, tetris_Tetrominos_count - 1));

                /* Check if game over - can't place new piece */
                is_gameover = !tetris_PlayField_doesPieceFit(&field, current_piece, current_rotation, current_x, current_y);
            }
        }

        /* Draw everything */
        try_(tetris_PlayField_drawScreen(&field, current_piece, current_rotation, current_x, current_y, score));
    }

/* Game over message */
#if plat_is_windows
    DWORD dwBytesWritten = 0;
    COORD coord = { 0, tetris_screen_height };
    WriteConsoleOutputCharacterW(tetris_Console_output_handle, L"Game Over!", 10, coord, &dwBytesWritten);
    time_sleep(time_Duration_fromMillis(2000));
#else
    printf("\nGame Over! Score: %d\n", score);
    time_sleep(time_Duration_fromMillis(2000));
#endif

    return_ok({});
} $unguarded_(fn);

/* Function Implementations =================================================*/

static fn_((tetris_Console_bootup(void))(E$void) pp_if_(plat_is_windows)(pp_then_($guard), pp_else_($scope))) {
#if plat_is_windows
    /* Set up Windows console */
    var screen_size = tetris_screen_width * tetris_screen_height;

    /* Use safe allocation with proper type information */
    var allocator = heap_Page_allocator(create$(heap_Page));
    var screen_buffer = try_(mem_Allocator_alloc(allocator, typeInfo$(wchar), screen_size));
    tetris_Console_screen_buffer = meta_castS$(S$wchar, screen_buffer);
    errdefer_($ignore, mem_Allocator_free(allocator, meta_sliToAny(screen_buffer)));

    /* Initialize buffer with spaces */
    for_(($s(tetris_Console_screen_buffer))(cell) {
        *cell = L' ';
    });

    /* Create console buffer with proper error handling */
    tetris_Console_output_handle = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        0,
        null,
        CONSOLE_TEXTMODE_BUFFER,
        null
    );
    if (tetris_Console_output_handle == INVALID_HANDLE_VALUE) {
        return_err(E_Unexpected());
    }
    errdefer_($ignore, CloseHandle(tetris_Console_output_handle));

    if (!SetConsoleActiveScreenBuffer(tetris_Console_output_handle)) {
        return_err(E_Unexpected());
    }
#else
    /* Set up Unix terminal */
    struct termios new_termios = cleared();

    /* Get current terminal settings */
    tcgetattr(STDIN_FILENO, &tetris_Console_original);

    /* Copy settings */
    new_termios = tetris_Console_original;

    /* Disable canonical mode and echo */
    new_termios.c_lflag &= ~(ICANON | ECHO);

    /* Apply new settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    /* Clear screen */
    printf("\033[2J\033[H");
#endif

    return_ok({});
} $un_(pp_if_(plat_is_windows)(pp_then_(guarded), pp_else_(scoped)))(fn);

static fn_((tetris_Console_shutdown(void))(void)) {
#if plat_is_windows
    var allocator = heap_Page_allocator(create$(heap_Page));
    mem_Allocator_free(allocator, anySli(tetris_Console_screen_buffer));
    CloseHandle(tetris_Console_output_handle);
#else  /* others */
    /* Restore original terminal settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &tetris_Console_original);
#endif /* others */
}

static fn_((tetris_isKeyPressed(i32 key))(bool)) {
#if plat_is_windows
    /* 1 = right, 2 = left, 3 = down, 4 = rotate (Z) */
    static const A$$(5, i8) s_key_map = A_init({ 0, VK_LEFT, VK_RIGHT, VK_DOWN, 'Z' });
    return (GetAsyncKeyState(A_getAt(s_key_map, key)) & 0x8000) != 0;
#else  /* others */
    /* Non-blocking read from stdin */
    static bool s_is_initialized = false;
    if (!s_is_initialized) {
        fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
        s_is_initialized = true;
    }

    i8 c = 0;
    if (read(STDIN_FILENO, &c, 1) < 1) { return false; }

    /* 1 = left, 2 = right, 3 = down, 4 = rotate (z) */
    if (key == 1 && c == 'a') { return true; }
    if (key == 2 && c == 'd') { return true; }
    if (key == 3 && c == 's') { return true; }
    if (key == 4 && c == 'z') { return true; }

    return false;
#endif /* others */
}

static fn_((tetris_PlayField_init(mem_Allocator allocator))(E$tetris_PlayField) $scope) {
    /* Allocate grid for field with proper type information */
    var items = try_(mem_Allocator_alloc(allocator, typeInfo$(u8), tetris_field_width * tetris_field_height));

    /* Create grid from slice with proper type casting */
    let field = Grid_fromS$(Grid$u8, meta_castS$(S$u8, items), tetris_field_width, tetris_field_height);

    /* Initialize field - borders are 9, inside is 0 */
    for_(($r(0, tetris_field_height))(y) {
        for_(($r(0, tetris_field_width))(x) {
            if (x == 0 || x == tetris_field_width - 1 || y == tetris_field_height - 1) {
                Grid_setAt(field, x, y, 9); /* Border */
            } else {
                Grid_setAt(field, x, y, 0); /* Empty */
            }
        });
    });

    return_ok({ .grid = field });
} $unscoped_(fn);

static fn_((tetris_PlayField_drawScreen(const tetris_PlayField* field, i32 current_piece, i32 rotation, i32 pos_x, i32 pos_y, i32 score))(E$void) $scope) {
#if plat_is_windows
    /* Clear screen buffer */
    for_(($s(tetris_Console_screen_buffer))(cell) {
        *cell = L' ';
    });

    /* Draw field */
    for_(($r(0, field->grid.height))(y) {
        for_(($r(0, field->grid.width))(x) {
            let cell = Grid_getAt(field->grid, x, y);
            wchar c = L' ';

            if (cell == 0) {
                c = L' '; /* Empty */
            } else if (cell == 9) {
                c = L'#'; /* Wall */
            } else if (cell == 8) {
                c = L'='; /* Line to be removed */
            } else {
                c = L'A' + cell - 1; /* Tetromino */
            }

            Sli_setAt(tetris_Console_screen_buffer, (y + 2) * tetris_screen_width + (x + 2), c);
        });
    });

    /* Draw current piece */
    for_(($r(0, 4))(py) {
        for_(($r(0, 4))(px) {
            let pi = tetris_rotate(px, py, rotation);
            let bit = 1 << pi;

            if (A_getAt(tetris_tetrominos, current_piece) & bit) {
                Sli_setAt(tetris_Console_screen_buffer, (pos_y + py + 2) * tetris_screen_width + (pos_x + px + 2), L'A' + current_piece);
            }
        });
    });

    /* Draw score */
    wchar score_text_buf[32] = { 0 };
    let score_text_len = swprintf(score_text_buf, 32, L"SCORE: %8d", score);
    let score_text = (S$$(wchar))Sli_from(score_text_buf, score_text_len);
    for_(($rf(0), $s(score_text))(i, typo) {
        Sli_setAt(tetris_Console_screen_buffer, 2 * tetris_screen_width + field->grid.width + 6 + i, *typo);
    });

    /* Output to console */
    DWORD dwBytesWritten = 0;
    WriteConsoleOutputCharacterW(tetris_Console_output_handle, tetris_Console_screen_buffer.ptr, tetris_Console_screen_buffer.len, (COORD){ 0, 0 }, &dwBytesWritten);
#else /* others */
    /* Move cursor to top-left */
    printf("\033[H");

    /* Draw top border */
    printf("+");
    for (usize i = 0; i < field->grid.width; ++i) {
        printf("-");
    }
    printf("+\n");

    /* Draw field */
    for (i32 y = 0; y < as$(i32)(field->grid.height); ++y) {
        printf("|");
        for (i32 x = 0; x < as$(i32)(field->grid.width); ++x) {
            u8 c = ' ';

            /* Check if current position has the active piece */
            bool is_current_piece = false;
            if (pos_x <= x && x < pos_x + 4
                && pos_y <= y && y < pos_y + 4) {
                let px = x - pos_x;
                let py = y - pos_y;
                let pi = tetris_rotate(px, py, rotation);
                let bit = 1 << pi;

                if (A_getAt(tetris_tetrominos, current_piece) & bit) {
                    is_current_piece = true;
                }
            }

            if (is_current_piece) {
                c = 'A' + current_piece;
            } else {
                u8 cell = Grid_getAt(field->grid, x, y);
                if (cell == 0) {
                    c = ' '; /* Empty */
                } else if (cell == 9) {
                    c = '#'; /* Wall */
                } else if (cell == 8) {
                    c = '='; /* Line to be removed */
                } else {
                    c = 'A' + cell - 1; /* Tetromino */
                }
            }

            printf("%c", c);
        }
        printf("|\n");
    }

    /* Draw bottom border */
    printf("+");
    for (usize i = 0; i < field->grid.width; ++i) {
        printf("-");
    }
    printf("+\n");

    /* Draw score */
    printf("SCORE: %8d\n", score);
#endif /* others */

    return_ok({});
} $unscoped_(fn);

static fn_((tetris_rotate(i32 px, i32 py, i32 r))(i32) $scope) {
    switch (r % 4) {
        case_(0 /* 0 degrees */, return_(py * 4 + px));
        case_(1 /* 90 degrees */, return_(12 + py - (px * 4)));
        case_(2 /* 180 degrees */, return_(15 - (py * 4) - px));
        case_(3 /* 270 degrees */, return_(3 - py + (px * 4)));
        default_(claim_unreachable);
    }
} $unscoped_(fn);

static fn_((tetris_PlayField_doesPieceFit(const tetris_PlayField* field, i32 tetromino, i32 rotation, i32 pos_x, i32 pos_y))(bool)) {
    for_(($r(0, 4))(py) {
        for_(($r(0, 4))(px) {
            /* Get index into piece */
            let pi = tetris_rotate(px, py, rotation);
            let bit = 1 << pi;

            /* Check if we're in bounds */
            if (0 <= pos_x + px && pos_x + px < field->grid.width
                && 0 <= pos_y + py && pos_y + py < field->grid.height) {
                /* In bounds, do collision check */
                if ((A_getAt(tetris_tetrominos, tetromino) & bit)
                    && Grid_getAt(field->grid, pos_x + px, pos_y + py) != 0) {
                    return false; /* Collision detected */
                }
            } else if (A_getAt(tetris_tetrominos, tetromino) & bit) {
                /* Out of bounds with a filled tetromino cell */
                return false;
            }
        });
    });
    return true;
}

static fn_((tetris_PlayField_clearLines(tetris_PlayField* field, ArrList$i32* lines))(E$i32) $scope) {
    ArrList_clearRetainingCap(lines->base);

    /* Check for complete lines */
    for_(($r(0, field->grid.height - 1))(y) {
        bool is_line = true;

        /* Check if every cell in the line is filled */
        for_(($r(1, field->grid.width - 1))(x) {
            is_line &= Grid_getAt(field->grid, x, y) != 0;
        });

        if (is_line) {
            /* Mark line for removal */
            for_(($r(1, field->grid.width - 1))(x) {
                Grid_setAt(field->grid, x, y, 8);
            });

            /* Add to lines list */
            var line_y = as$(i32)(y);
            try_(ArrList_append(lines->base, meta_refPtr(&line_y)));
        }
    });

    /* If we have lines to clear, animate and remove them */
    if (0 < lines->items.len) {
        /* First draw the screen with the lines marked */
        time_sleep(time_Duration_fromMillis(400));

        /* Then clear and move lines down */
        for_(($s(lines->items))(line) {
            let y = *line;
            for_(($r(1, (field->grid.width)))(x) {
                /* Move all lines above down by one */
                for_($rev($r(0, y))(k) {
                    Grid_setAt(field->grid, x, k, Grid_getAt(field->grid, x, k - 1));
                });
                /* Clear top line */
                Grid_setAt(field->grid, x, 0, 0);
            });
        });
    }

    return_ok(as$(i32)(lines->items.len));
} $unscoped_(fn);

static fn_((tetris_PlayField_lockPiece(tetris_PlayField* field, i32 piece, i32 rotation, i32 pos_x, i32 pos_y))(void)) {
    for_(($r(0, 4))(py) {
        for_(($r(0, 4))(px) {
            let pi = tetris_rotate(px, py, rotation);
            let bit = 1 << pi;

            if (A_getAt(tetris_tetrominos, piece) & bit) {
                Grid_setAt(field->grid, pos_x + px, pos_y + py, piece + 1);
            }
        });
    });
}
