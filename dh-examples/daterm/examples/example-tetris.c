/* TODO: Apply double buffering */
/* TODO: Avoid calling clear frequently */
/* TODO: Do not directly manipulate the cursor */
#include <dh-main.h>
#include <dh/ascii.h>
#include <dh/utf8.h>
#include <dh/Rand.h>
#include <dh/heap/Sys.h>
#include <dh/heap/Arena.h>
#include <dh/io/Buf.h>
#include <dh/io/common.h>
#include <dh/mem/common.h>
#include <dh/time/Dur.h>
#include "daterm.h"

/*========== RGB ============================================================*/

typedef struct tetris_RGB {
    u8 r;
    u8 g;
    u8 b;
} tetris_RGB;
T_use_prl$(tetris_RGB);
$static let_(tetris_RGB_ghost, tetris_RGB) = { 90, 95, 105 };
$static let_(tetris_RGB_pause, tetris_RGB) = { 255, 230, 120 };
$static let_(tetris_RGB_game_over, tetris_RGB) = { 255, 95, 95 };
$static fn_((tetris_RGB_eql(tetris_RGB lhs, tetris_RGB rhs))(bool));

/*========== Piece ==========================================================*/

typedef enum_((tetris_Piece $fits($packed))(
    tetris_Piece_i = 0,
    tetris_Piece_j,
    tetris_Piece_l,
    tetris_Piece_o,
    tetris_Piece_s,
    tetris_Piece_t,
    tetris_Piece_z,
    count$tetris_Piece,
)) tetris_Piece;
T_use_prl$(tetris_Piece);
claim_assert_static(eqlType$(tetris_Piece, u8));

typedef enum_((tetris_Rotation $fits($packed))(
    tetris_Rotation_0 = 0,
    tetris_Rotation_r,
    tetris_Rotation_2,
    tetris_Rotation_l,
    count$tetris_Rotation,
)) tetris_Rotation;
T_use_prl$(tetris_Rotation);
claim_assert_static(eqlType$(tetris_Rotation, u8));

typedef A$$(count$tetris_Rotation, u16) tetris_Piece_Masks;
$static let_(tetris_Piece_masks, A$$(count$tetris_Piece, tetris_Piece_Masks)) = A_init({
    [tetris_Piece_i] = A_init({
        [tetris_Rotation_0] = n_(0b, 0000, 1111, 0000, 0000),
        [tetris_Rotation_r] = n_(0b, 0010, 0010, 0010, 0010),
        [tetris_Rotation_2] = n_(0b, 0000, 0000, 1111, 0000),
        [tetris_Rotation_l] = n_(0b, 0100, 0100, 0100, 0100),
    }),
    [tetris_Piece_j] = A_init({
        [tetris_Rotation_0] = n_(0b, 1000, 1110, 0000, 0000),
        [tetris_Rotation_r] = n_(0b, 0110, 0100, 0100, 0000),
        [tetris_Rotation_2] = n_(0b, 0000, 1110, 0010, 0000),
        [tetris_Rotation_l] = n_(0b, 0100, 0100, 1100, 0000),
    }),
    [tetris_Piece_l] = A_init({
        [tetris_Rotation_0] = n_(0b, 0010, 1110, 0000, 0000),
        [tetris_Rotation_r] = n_(0b, 0100, 0100, 0110, 0000),
        [tetris_Rotation_2] = n_(0b, 0000, 1110, 1000, 0000),
        [tetris_Rotation_l] = n_(0b, 1100, 0100, 0100, 0000),
    }),
    [tetris_Piece_o] = A_init({
        [tetris_Rotation_0] = n_(0b, 0110, 0110, 0000, 0000),
        [tetris_Rotation_r] = n_(0b, 0110, 0110, 0000, 0000),
        [tetris_Rotation_2] = n_(0b, 0110, 0110, 0000, 0000),
        [tetris_Rotation_l] = n_(0b, 0110, 0110, 0000, 0000),
    }),
    [tetris_Piece_s] = A_init({
        [tetris_Rotation_0] = n_(0b, 0110, 1100, 0000, 0000),
        [tetris_Rotation_r] = n_(0b, 0100, 0110, 0010, 0000),
        [tetris_Rotation_2] = n_(0b, 0000, 0110, 1100, 0000),
        [tetris_Rotation_l] = n_(0b, 1000, 1100, 0100, 0000),
    }),
    [tetris_Piece_t] = A_init({
        [tetris_Rotation_0] = n_(0b, 0100, 1110, 0000, 0000),
        [tetris_Rotation_r] = n_(0b, 0100, 0110, 0100, 0000),
        [tetris_Rotation_2] = n_(0b, 0000, 1110, 0100, 0000),
        [tetris_Rotation_l] = n_(0b, 0100, 1100, 0100, 0000),
    }),
    [tetris_Piece_z] = A_init({
        [tetris_Rotation_0] = n_(0b, 1100, 0110, 0000, 0000),
        [tetris_Rotation_r] = n_(0b, 0010, 0110, 0100, 0000),
        [tetris_Rotation_2] = n_(0b, 0000, 1100, 0110, 0000),
        [tetris_Rotation_l] = n_(0b, 0100, 1100, 1000, 0000),
    }),
});
$static fn_((tetris_Piece_covers(tetris_Piece self, tetris_Rotation rotation, usize x, usize y))(bool));

$static let_(tetris_Piece_colors, A$$(count$tetris_Piece, tetris_RGB)) = A_init({
    [tetris_Piece_i] = { 0, 220, 220 },
    [tetris_Piece_j] = { 55, 90, 220 },
    [tetris_Piece_l] = { 240, 150, 35 },
    [tetris_Piece_o] = { 235, 220, 60 },
    [tetris_Piece_s] = { 80, 200, 90 },
    [tetris_Piece_t] = { 160, 85, 210 },
    [tetris_Piece_z] = { 225, 60, 75 },
});
$static fn_((tetris_Piece_color(tetris_Piece self))(tetris_RGB));


$static let_(tetris_Piece_names, A$$(count$tetris_Piece, S_const$u8)) = A_init({
    [tetris_Piece_i] = u8_l("I"),
    [tetris_Piece_j] = u8_l("J"),
    [tetris_Piece_l] = u8_l("L"),
    [tetris_Piece_o] = u8_l("O"),
    [tetris_Piece_s] = u8_l("S"),
    [tetris_Piece_t] = u8_l("T"),
    [tetris_Piece_z] = u8_l("Z"),
});
$static fn_((tetris_Piece_name(tetris_Piece self))(S_const$u8));

/*========== Board ==========================================================*/

#define tetris_Board_w 10ull
#define tetris_Board_h 22ull
#define tetris_Board_hidden_rows 2ull
#define tetris_Board_visible_h (tetris_Board_h - tetris_Board_hidden_rows)

typedef struct tetris_Board {
    var_(cells, A$$(tetris_Board_w * tetris_Board_h, O$tetris_Piece));
} tetris_Board;
$static fn_((tetris_Board_clear(tetris_Board* self))(void));
$static fn_((tetris_Board_at(const tetris_Board* self, usize x, usize y))(const O$tetris_Piece*));
$static fn_((tetris_Board_atMut(tetris_Board* self, usize x, usize y))(O$tetris_Piece*));
$static fn_((tetris_Board_clearLines(tetris_Board* self))(usize));

/*========== Queue ==========================================================*/


#define tetris_PieceQ_Bag_len 7ull
typedef A$$(tetris_PieceQ_Bag_len, tetris_Piece) tetris_PieceQ_Bag;
#define tetris_PieceQ_Items_cap 14ull
typedef A$$(tetris_PieceQ_Items_cap, tetris_Piece) tetris_PieceQ_Items;
#define tetris_PieceQ_preview_len 5ull

typedef struct tetris_PieceQ {
    var_(items, tetris_PieceQ_Items);
    var_(len, usize);
    var_(rng, Rand);
} tetris_PieceQ;

$static fn_((tetris_PieceQ_init(tetris_PieceQ* self))(void));
$static fn_((tetris_PieceQ_pushBag(tetris_PieceQ* self))(void));
$static fn_((tetris_PieceQ_pop(tetris_PieceQ* self))(tetris_Piece));
$static fn_((tetris_PieceQ_peek(const tetris_PieceQ* self, usize index))(tetris_Piece));

/*========== Game ===========================================================*/

#define tetris_spawn_x 3
#define tetris_spawn_y 0
#define tetris_lock_delay time_Dur_fromMillis_static(500)

typedef enum_((tetris_Cmd $fits($packed))(
    tetris_cmd_quit,
    tetris_cmd_toggle_pause,
    tetris_cmd_move_left,
    tetris_cmd_move_right,
    tetris_cmd_soft_drop,
    tetris_cmd_hard_drop,
    tetris_cmd_rotate_cw,
    tetris_cmd_rotate_ccw,
    tetris_cmd_hold,
)) tetris_Cmd;
T_use_prl$(tetris_Cmd);

typedef struct tetris_Kick {
    i32 x;
    i32 y;
} tetris_Kick;

typedef A$$(5, tetris_Kick) tetris_KickTests;
typedef A$$(8, tetris_KickTests) tetris_KickTable;
typedef A$$(5, u32) tetris_LineScores;

$static let_(tetris_kicks_jlstz, tetris_KickTable) = A_init({
    A_init({ { 0, 0 }, { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } }),
    A_init({ { 0, 0 }, { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } }),
    A_init({ { 0, 0 }, { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } }),
    A_init({ { 0, 0 }, { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } }),
    A_init({ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 } }),
    A_init({ { 0, 0 }, { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } }),
    A_init({ { 0, 0 }, { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } }),
    A_init({ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 } }),
});

$static let_(tetris_kicks_i, tetris_KickTable) = A_init({
    A_init({ { 0, 0 }, { -2, 0 }, { 1, 0 }, { -2, -1 }, { 1, 2 } }),
    A_init({ { 0, 0 }, { 2, 0 }, { -1, 0 }, { 2, 1 }, { -1, -2 } }),
    A_init({ { 0, 0 }, { -1, 0 }, { 2, 0 }, { -1, 2 }, { 2, -1 } }),
    A_init({ { 0, 0 }, { 1, 0 }, { -2, 0 }, { 1, -2 }, { -2, 1 } }),
    A_init({ { 0, 0 }, { 2, 0 }, { -1, 0 }, { 2, 1 }, { -1, -2 } }),
    A_init({ { 0, 0 }, { -2, 0 }, { 1, 0 }, { -2, -1 }, { 1, 2 } }),
    A_init({ { 0, 0 }, { 1, 0 }, { -2, 0 }, { 1, -2 }, { -2, 1 } }),
    A_init({ { 0, 0 }, { -1, 0 }, { 2, 0 }, { -1, 2 }, { 2, -1 } }),
});

$static let tetris_line_scores = A_from$((u32){ 0, 100, 300, 500, 800 });

typedef struct tetris_Self {
    var_(board, tetris_Board);
    var_(queue, tetris_PieceQ);
    var_(current, tetris_Piece);
    var_(hold, O$tetris_Piece);
    var_(rotation, tetris_Rotation);
    var_(x, i32);
    var_(y, i32);
    var_(score, u32);
    var_(lines, u32);
    var_(level, u32);
    var_(gravity_interval, time_Dur);
    var_(gravity_elapsed, time_Dur);
    var_(lock_elapsed, time_Dur);
    var_(can_hold, bool);
    var_(is_game_over, bool);
    var_(is_paused, bool);
    var_(is_running, bool);
} tetris_Self;
$static fn_((tetris_gravityForLevel(u32 level))(time_Dur));

$static fn_((tetris_init(tetris_Self* self))(tetris_Self*));
$static fn_((tetris_spawn(tetris_Self* self))(void));
$static fn_((tetris_pieceFits(const tetris_Self* self, tetris_Piece piece, tetris_Rotation rotation, i32 x, i32 y))(bool));
$static fn_((tetris_isGrounded(const tetris_Self* self))(bool));
$static fn_((tetris_tryMove(tetris_Self* self, i32 dx, i32 dy))(bool));
$static fn_((tetris_tryRotate(tetris_Self* self, i32 dir))(bool));
$static fn_((tetris_lock(tetris_Self* self))(void));
$static fn_((tetris_hardDrop(tetris_Self* self))(void));
$static fn_((tetris_hold(tetris_Self* self))(void));
$static fn_((tetris_tick(tetris_Self* self, time_Dur dt))(void));
$static fn_((tetris_cmdFromKey(dansi_Event_Key key))(E$O$tetris_Cmd)) $must_check;
$static fn_((tetris_applyCmd(tetris_Self* self, tetris_Cmd cmd))(void));
$static fn_((tetris_pollInput(tetris_Self* self, daterm_Term term))(E$void)) $must_check;
$static fn_((tetris_ghostY(const tetris_Self* self))(i32));
$static fn_((tetris_activeCellAt(const tetris_Self* self, usize x, usize y))(O$tetris_Piece));
$static fn_((tetris_ghostCellAt(const tetris_Self* self, i32 ghost_y, usize x, usize y))(bool));

/*========== Frame / Presenter ============================================*/

#define tetris_Frame_width 72ull
#define tetris_Frame_height 34ull
#define tetris_Presenter_buf_len 16384ull

typedef struct tetris_FrameCell {
    var_(ch, u8);
    var_(fg, O$tetris_RGB);
    var_(bg, O$tetris_RGB);
} tetris_FrameCell;
T_use_prl$(tetris_FrameCell);
$static let_(tetris_FrameCell_blank, tetris_FrameCell) = {
    .ch = ' ',
    .fg = none(),
    .bg = none(),
};

typedef struct tetris_Frame {
    var_(cells, A$$(tetris_Frame_width * tetris_Frame_height, tetris_FrameCell));
} tetris_Frame;
$static fn_((tetris_Frame_clear(tetris_Frame* self))(void));
$static fn_((tetris_Frame_cells(const tetris_Frame* self))(S_const$tetris_FrameCell));
$static fn_((tetris_Frame_cellsMut(tetris_Frame* self))(S$tetris_FrameCell));
$static fn_((tetris_Frame_at(const tetris_Frame* self, usize x, usize y))(O$P_const$tetris_FrameCell));
$static fn_((tetris_Frame_atMut(tetris_Frame* self, usize x, usize y))(O$P$tetris_FrameCell));
$static fn_((tetris_Frame_putChar(tetris_Frame* self, usize x, usize y, u8 ch))(void));
$static fn_((tetris_Frame_putText(tetris_Frame* self, usize x, usize y, S_const$u8 text))(void));
$static fn_((tetris_Frame_putTextFg(tetris_Frame* self, usize x, usize y, S_const$u8 text, tetris_RGB fg))(void));
$static fn_((tetris_Frame_putBlock(tetris_Frame* self, usize x, usize y, tetris_Piece piece))(void));
$static fn_((tetris_Frame_putGhost(tetris_Frame* self, usize x, usize y))(void));

$static let_(tetris_Presenter_digit_pairs, S_const$u8) = u8_l(
    "00010203040506070809"
    "10111213141516171819"
    "20212223242526272829"
    "30313233343536373839"
    "40414243444546474849"
    "50515253545556575859"
    "60616263646566676869"
    "70717273747576777879"
    "80818283848586878889"
    "90919293949596979899"
);
typedef struct tetris_Presenter {
    var_(frame, tetris_Frame);
    var_(out, struct {
        var_(mem, A$$(tetris_Presenter_buf_len, u8));
        var_(writer, io_Buf_Writer);
    });
} tetris_Presenter;

typedef struct tetris_Presenter_StyleState {
    var_(fg, O$tetris_RGB);
    var_(bg, O$tetris_RGB);
} tetris_Presenter_StyleState;

$static fn_((tetris_Presenter_init(tetris_Presenter* self, io_Writer out))(tetris_Presenter*));
$static fn_((tetris_Presenter_compose(tetris_Presenter* self, const tetris_Self* game))(void));
$static fn_((tetris_Presenter_present(tetris_Presenter* self))(E$void)) $must_check;
$static fn_((tetris_Presenter_render(tetris_Presenter* self, const tetris_Self* game))(E$void)) $must_check;
$static fn_((tetris_Presenter_formatU8(S$u8 buf, u8 val))(usize));
$static fn_((tetris_Presenter_writeRGB(S$u8 buf, S_const$u8 prefix, tetris_RGB color))(usize));
$static fn_((tetris_Presenter_StyleState_init(void))(tetris_Presenter_StyleState));
$static fn_((tetris_Presenter_StyleState_applyFg(tetris_Presenter_StyleState* self, io_Writer out, O$tetris_RGB next))(E$void)) $must_check;
$static fn_((tetris_Presenter_StyleState_applyBg(tetris_Presenter_StyleState* self, io_Writer out, O$tetris_RGB next))(E$void)) $must_check;
$static fn_((tetris_Presenter_StyleState_applyCell(tetris_Presenter_StyleState* self, io_Writer out, tetris_FrameCell cell))(E$void)) $must_check;

/*========== Main ===========================================================*/

$static fn_((tetris_Main_waitForEnter(void))(void));

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    tetris_Main_waitForEnter();

    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);

    var ansi = try_(daterm_ANSI_init(daterm_ANSI_Cfg_default(gpa)));
    defer_(daterm_ANSI_fini(&ansi));
    try_(daterm_ANSI_enableRawMode(&ansi));
    defer_(daterm_ANSI_disableRawMode(&ansi));

    let term = daterm_ANSI_term(&ansi);
    let out = daterm_Term_writer(term);
    try_(dansi_screen_enterAlternateWrite(out));
    defer_(catch_((dansi_screen_exitAlternateWrite(out))($ignore, $do_nothing)));
    try_(dansi_cursor_hideWrite(out));
    defer_(catch_((dansi_cursor_showWrite(out))($ignore, $do_nothing)));

    $static var_(game_storage, tetris_Self) $undefined_static;
    let game = tetris_init(&game_storage);

    $static var_(presenter_storage, tetris_Presenter) $undefined_static;
    let presenter = tetris_Presenter_init(&presenter_storage, out);

    let frame_interval = time_Dur_fromSecs$f64(1.0 / 60.0);
    var schedule_from = time_Clock_now(ansi.clock);
    var last_tick = schedule_from;
    while (game->is_running) {
        let now = time_Clock_now(ansi.clock);
        let dt = time_Clock_Inst_durSince(now, last_tick);
        last_tick = now;

        try_(tetris_pollInput(game, term));
        tetris_tick(game, dt);
        try_(tetris_Presenter_render(presenter, game));

        let due_at = time_Clock_Inst_addDur(schedule_from, frame_interval);
        let done_at = time_Clock_now(ansi.clock);
        if_some((time_Clock_Inst_durSinceChkd(due_at, done_at))(wait_dur)) {
            catch_((time_Clock_sleep(ansi.clock, wait_dur))($ignore, $do_nothing));
            schedule_from = due_at;
        } else_none {
            schedule_from = done_at;
        }
    }

    return_ok({});
} $unguarded(fn);

/*========== Main ===========================================================*/

$static fn_((tetris_Main_waitForEnter(void))(void)) {
    let out = fs_File_writer(io_getStdOut());
    catch_((io_Writer_writeBytes(out, u8_l("example-tetris: press Enter to enter terminal mode\n")))($ignore, $do_nothing));

    var_(read_mem, A$$(128, u8)) $undefined;
    var reader = io_Buf_Reader_init(fs_File_reader(io_getStdIn()), A_ref$((S$u8)(read_mem)));
    var_(line_mem, A$$(256, u8)) $undefined;
    catch_((io_Buf_Reader_readUntilByte(&reader, ascii_lf_byte, A_ref$((S$u8)(line_mem))))($ignore, $do_nothing));
}

/*========== RGB ============================================================*/

$static fn_((tetris_RGB_eql(tetris_RGB lhs, tetris_RGB rhs))(bool)) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

/*========== Piece ==========================================================*/

$static fn_((tetris_Piece_covers(tetris_Piece self, tetris_Rotation rotation, usize x, usize y))(bool)) {
    claim_assert(x < 4);
    claim_assert(y < 4);
    let mask = *A_at((*A_at((tetris_Piece_masks)[self]))[rotation]);
    let bit_idx = (3 - y) * 4 + (3 - x);
    return ((mask >> bit_idx) & 1u) != 0;
}

$static fn_((tetris_Piece_color(tetris_Piece self))(tetris_RGB)) {
    return *A_at((tetris_Piece_colors)[self]);
}

$static fn_((tetris_Piece_name(tetris_Piece self))(S_const$u8)) {
    return *A_at((tetris_Piece_names)[self]);
}

/*========== Board ==========================================================*/

$static fn_((tetris_Board_clear(tetris_Board* self))(void)) {
    for_(($s(A_ref(self->cells)))(cell)) asg_l((cell)(none())) $end(for);
};

$static fn_((tetris_Board_at(const tetris_Board* self, usize x, usize y))(const O$tetris_Piece*)) {
    claim_assert(x < tetris_Board_w);
    claim_assert(y < tetris_Board_h);
    return A_at((self->cells)[y * tetris_Board_w + x]);
};

$static fn_((tetris_Board_atMut(tetris_Board* self, usize x, usize y))(O$tetris_Piece*)) {
    claim_assert(x < tetris_Board_w);
    claim_assert(y < tetris_Board_h);
    return A_at((self->cells)[y * tetris_Board_w + x]);
};

$static fn_((tetris_Board_clearLines(tetris_Board* self))(usize)) {
    var_(cleared, usize) = 0;
    var_(y, usize) = tetris_Board_h;
    while (0 < y) {
        y -= 1;
        var_(full, bool) = true;
        for_(($r(0, tetris_Board_w))(x)) {
            if (isNone(*tetris_Board_at(self, x, y))) {
                full = false;
                break;
            }
        } $end(for);
        if (!full) { continue; }

        var_(pull_y, usize) = y;
        while (0 < pull_y) {
            for_(($r(0, tetris_Board_w))(x)) {
                *tetris_Board_atMut(self, x, pull_y) = *tetris_Board_at(self, x, pull_y - 1);
            } $end(for);
            pull_y -= 1;
        }
        for_(($r(0, tetris_Board_w))(x)) {
            *tetris_Board_atMut(self, x, 0) = none$((O$tetris_Piece));
        } $end(for);
        cleared += 1;
        y += 1;
    }
    return cleared;
}

/*========== Queue ==========================================================*/

$static fn_((tetris_PieceQ_init(tetris_PieceQ* self))(void)) {
    asg_l((self)({
        .len = 0,
        .rng = Rand_init(),
    }));
    tetris_PieceQ_pushBag(self);
    tetris_PieceQ_pushBag(self);
}

$static fn_((tetris_PieceQ_pushBag(tetris_PieceQ* self))(void)) {
    var_(bag, tetris_PieceQ_Bag) = A_init({
        tetris_Piece_i,
        tetris_Piece_j,
        tetris_Piece_l,
        tetris_Piece_o,
        tetris_Piece_s,
        tetris_Piece_t,
        tetris_Piece_z,
    });

    for_(($r(0, tetris_PieceQ_Bag_len - 1))(step)) {
        let i = tetris_PieceQ_Bag_len - 1 - step;
        let j = Rand_lessThan$usize(&self->rng, i + 1);
        let tmp = *A_at((bag)[i]);
        *A_at((bag)[i]) = *A_at((bag)[j]);
        *A_at((bag)[j]) = tmp;
    } $end(for);

    for_(($a(bag))(piece)) {
        claim_assert(self->len < tetris_PieceQ_Items_cap);
        *A_at((self->items)[self->len]) = *piece;
        self->len += 1;
    } $end(for);
}

$static fn_((tetris_PieceQ_pop(tetris_PieceQ* self))(tetris_Piece)) {
    if (self->len < tetris_PieceQ_Bag_len) { tetris_PieceQ_pushBag(self); }
    let piece = *A_at((self->items)[0]);
    for_(($r(1, self->len))(i)) {
        *A_at((self->items)[i - 1]) = *A_at((self->items)[i]);
    } $end(for);
    self->len -= 1;
    if (self->len < tetris_PieceQ_Bag_len) { tetris_PieceQ_pushBag(self); }
    return piece;
}

$static fn_((tetris_PieceQ_peek(const tetris_PieceQ* self, usize index))(tetris_Piece)) {
    claim_assert(index < self->len);
    return *A_at((self->items)[index]);
}

/*========== Game ===========================================================*/

$static fn_((tetris_gravityForLevel(u32 level))(time_Dur)) {
    $static let_(gravity_ms, A$$(10, u32)) = A_init({ 800, 717, 633, 550, 467, 383, 300, 217, 133, 100 });
    if (level == 0) { level = 1; }
    if (level <= A_len(gravity_ms)) { return time_Dur_fromMillis(*A_at((gravity_ms)[level - 1])); }
    let fast = 100u - (level - 10u) * 6u;
    return time_Dur_fromMillis(fast < 50u ? 50u : fast);
}

$static fn_((tetris_init(tetris_Self* self))(tetris_Self*)) {
    asg_l((self)({
        .hold = none(),
        .score = 0,
        .lines = 0,
        .level = 1,
        .gravity_interval = tetris_gravityForLevel(1),
        .gravity_elapsed = time_Dur_zero,
        .lock_elapsed = time_Dur_zero,
        .can_hold = false,
        .is_game_over = false,
        .is_paused = false,
        .is_running = true,
    }));
    tetris_Board_clear(&self->board);
    tetris_PieceQ_init(&self->queue);
    tetris_spawn(self);
    return self;
}

$static fn_((tetris_spawn(tetris_Self* self))(void)) {
    self->current = tetris_PieceQ_pop(&self->queue);
    self->rotation = tetris_Rotation_0;
    self->x = tetris_spawn_x;
    self->y = tetris_spawn_y;
    self->gravity_elapsed = time_Dur_zero;
    self->lock_elapsed = time_Dur_zero;
    self->can_hold = true;
    self->is_game_over = !tetris_pieceFits(self, self->current, self->rotation, self->x, self->y);
}

$static fn_((tetris_pieceFits(const tetris_Self* self, tetris_Piece piece, tetris_Rotation rotation, i32 x, i32 y))(bool)) {
    for_(($r(0, 4))(py)) {
        for_(($r(0, 4))(px)) {
            if (!tetris_Piece_covers(piece, rotation, px, py)) { continue; }
            let bx = x + as$(i32)(px);
            let by = y + as$(i32)(py);
            if (bx < 0 || by < 0) { return false; }
            if (as$(usize)(bx) >= tetris_Board_w || as$(usize)(by) >= tetris_Board_h) { return false; }
            if (isSome(*tetris_Board_at(&self->board, as$(usize)(bx), as$(usize)(by)))) { return false; }
        } $end(for);
    } $end(for);
    return true;
}

$static fn_((tetris_isGrounded(const tetris_Self* self))(bool)) {
    return !tetris_pieceFits(self, self->current, self->rotation, self->x, self->y + 1);
}

$static fn_((tetris_tryMove(tetris_Self* self, i32 dx, i32 dy))(bool)) {
    if (!tetris_pieceFits(self, self->current, self->rotation, self->x + dx, self->y + dy)) {
        return false;
    }
    self->x += dx;
    self->y += dy;
    if (dx != 0 || dy == 0) { self->lock_elapsed = time_Dur_zero; }
    return true;
}

$static fn_((tetris_rotationNext(tetris_Rotation rotation, i32 dir))(tetris_Rotation)) {
    let value = (as$(i32)(rotation) + dir + as$(i32)(count$tetris_Rotation)) % as$(i32)(count$tetris_Rotation);
    return as$(tetris_Rotation)(value);
}

$static fn_((tetris_kickIndex(tetris_Rotation from, tetris_Rotation to))(usize)) {
    if (from == tetris_Rotation_0 && to == tetris_Rotation_r) { return 0; }
    if (from == tetris_Rotation_r && to == tetris_Rotation_0) { return 1; }
    if (from == tetris_Rotation_r && to == tetris_Rotation_2) { return 2; }
    if (from == tetris_Rotation_2 && to == tetris_Rotation_r) { return 3; }
    if (from == tetris_Rotation_2 && to == tetris_Rotation_l) { return 4; }
    if (from == tetris_Rotation_l && to == tetris_Rotation_2) { return 5; }
    if (from == tetris_Rotation_l && to == tetris_Rotation_0) { return 6; }
    claim_assert(from == tetris_Rotation_0 && to == tetris_Rotation_l);
    return 7;
}

$static fn_((tetris_tryRotate(tetris_Self* self, i32 dir))(bool)) {
    let from = self->rotation;
    let to = tetris_rotationNext(from, dir);
    if (self->current == tetris_Piece_o) {
        self->rotation = to;
        self->lock_elapsed = time_Dur_zero;
        return true;
    }

    let kick_idx = tetris_kickIndex(from, to);
    let tests = self->current == tetris_Piece_i ? *A_at((tetris_kicks_i)[kick_idx]) : *A_at((tetris_kicks_jlstz)[kick_idx]);
    for_(($a(tests))(test)) {
        let nx = self->x + test->x;
        let ny = self->y + test->y;
        if (tetris_pieceFits(self, self->current, to, nx, ny)) {
            self->x = nx;
            self->y = ny;
            self->rotation = to;
            self->lock_elapsed = time_Dur_zero;
            return true;
        }
    } $end(for);
    return false;
}

$static fn_((tetris_lock(tetris_Self* self))(void)) {
    for_(($r(0, 4))(py)) {
        for_(($r(0, 4))(px)) {
            if (!tetris_Piece_covers(self->current, self->rotation, px, py)) { continue; }
            let bx = self->x + as$(i32)(px);
            let by = self->y + as$(i32)(py);
            if (bx < 0 || by < 0) { continue; }
            *tetris_Board_atMut(&self->board, as$(usize)(bx), as$(usize)(by)) = some$((O$tetris_Piece)(self->current));
        } $end(for);
    } $end(for);

    let cleared = tetris_Board_clearLines(&self->board);
    self->score += *A_at((tetris_line_scores)[cleared]) * self->level;
    self->lines += as$(u32)(cleared);
    self->level = self->lines / 10u + 1u;
    self->gravity_interval = tetris_gravityForLevel(self->level);
    tetris_spawn(self);
}

$static fn_((tetris_hardDrop(tetris_Self* self))(void)) {
    var_(dropped, u32) = 0;
    while (tetris_tryMove(self, 0, 1)) {
        dropped += 1;
    }
    self->score += dropped * 2u;
    tetris_lock(self);
}

$static fn_((tetris_hold(tetris_Self* self))(void)) {
    if (!self->can_hold) { return; }
    if_none(self->hold) {
        self->hold = some$((O$tetris_Piece)(self->current));
        tetris_spawn(self);
    } else_some(held) {
        self->hold = some$((O$tetris_Piece)(self->current));
        self->current = held;
        self->rotation = tetris_Rotation_0;
        self->x = tetris_spawn_x;
        self->y = tetris_spawn_y;
        self->gravity_elapsed = time_Dur_zero;
        self->lock_elapsed = time_Dur_zero;
        self->is_game_over = !tetris_pieceFits(self, self->current, self->rotation, self->x, self->y);
    }
    self->can_hold = false;
}

$static fn_((tetris_tick(tetris_Self* self, time_Dur dt))(void)) {
    if (self->is_paused || self->is_game_over) { return; }

    self->gravity_elapsed = time_Dur_addSat(self->gravity_elapsed, dt);
    self->lock_elapsed = tetris_isGrounded(self) ? time_Dur_addSat(self->lock_elapsed, dt) : time_Dur_zero;

    if (time_Dur_lt(self->gravity_elapsed, self->gravity_interval)
        && time_Dur_lt(self->lock_elapsed, tetris_lock_delay)) {
        return;
    }

    self->gravity_elapsed = time_Dur_zero;
    if (tetris_tryMove(self, 0, 1)) { return; }
    if (time_Dur_ge(self->lock_elapsed, tetris_lock_delay)) {
        tetris_lock(self);
    }
}

$static fn_((tetris_cmdFromKey(dansi_Event_Key key))(E$O$tetris_Cmd) $scope) {
    $suppress_(switch_enum)(switch (key.code)) {
    case_((dansi_Event_KeyCode_esc)) return_ok(some(tetris_cmd_quit)) $end(case);
    case_((dansi_Event_KeyCode_left)) return_ok(some(tetris_cmd_move_left)) $end(case);
    case_((dansi_Event_KeyCode_right)) return_ok(some(tetris_cmd_move_right)) $end(case);
    case_((dansi_Event_KeyCode_down)) return_ok(some(tetris_cmd_soft_drop)) $end(case);
    case_((dansi_Event_KeyCode_up)) return_ok(some(tetris_cmd_rotate_cw)) $end(case);
    case_((dansi_Event_KeyCode_char)) {
        let len = try_((utf8_codepointSeqLen(key.codepoint)));
        if (len != utf8_SeqLen_1) return_ok(none());
        let ch = ascii_toLower(intCast$((u8)(key.codepoint)));
        if (ch == 'q' || (key.mods.ctrl && ch == 'c')) return_ok(some(tetris_cmd_quit));
        if (ch == 'p') return_ok(some(tetris_cmd_toggle_pause));
        if (ch == ' ') return_ok(some(tetris_cmd_hard_drop));
        if (ch == 'z') return_ok(some(tetris_cmd_rotate_ccw));
        if (ch == 'x') return_ok(some(tetris_cmd_rotate_cw));
        if (ch == 'c') return_ok(some(tetris_cmd_hold));
    } $end(case);
    default_() $do_nothing $end(default);
    }
    return_ok(none());
} $unscoped(fn);

$static fn_((tetris_applyCmd(tetris_Self* self, tetris_Cmd cmd))(void)) {
    $suppress_(switch_enum)(switch (cmd)) {
    case_((tetris_cmd_quit)) {
        self->is_running = false;
        return;
    } $end(case);
    case_((tetris_cmd_toggle_pause)) {
        self->is_paused = !self->is_paused;
        return;
    } $end(case);
    default_() $do_nothing $end(default);
    };

    if (self->is_paused || self->is_game_over) return;
    $suppress_(switch_enum)(switch (cmd)) {
    case_((tetris_cmd_move_left)) let_ignore = tetris_tryMove(self, -1, 0) $end(case);
    case_((tetris_cmd_move_right)) let_ignore = tetris_tryMove(self, 1, 0) $end(case);
    case_((tetris_cmd_soft_drop)) {
        if (tetris_tryMove(self, 0, 1)) self->score += 1;
    } $end(case);
    case_((tetris_cmd_hard_drop)) tetris_hardDrop(self) $end(case);
    case_((tetris_cmd_rotate_cw)) let_ignore = tetris_tryRotate(self, 1) $end(case);
    case_((tetris_cmd_rotate_ccw)) let_ignore = tetris_tryRotate(self, -1) $end(case);
    case_((tetris_cmd_hold)) tetris_hold(self) $end(case);
    default_() $do_nothing $end(default);
    };
}

$static fn_((tetris_pollInput(tetris_Self* self, daterm_Term term))(E$void) $scope) {
    while (true) {
        let event = orelse_((daterm_Term_poll(term))(return_ok({})));
        $suppress_(switch_enum)(match_(event)) {
        pattern_((daterm_Event_key)(key)) {
            let cmd = try_(tetris_cmdFromKey(key));
            if_some((cmd)(value)) tetris_applyCmd(self, value);
        } $end(pattern);
        default_() $do_nothing $end(default);
        } $end(match);
    }
} $unscoped(fn);

$static fn_((tetris_ghostY(const tetris_Self* self))(i32)) {
    var_(y, i32) = self->y;
    while (tetris_pieceFits(self, self->current, self->rotation, self->x, y + 1)) {
        y += 1;
    }
    return y;
}

$static fn_((tetris_activeCellAt(const tetris_Self* self, usize x, usize y))(O$tetris_Piece)) {
    for_(($r(0, 4))(py)) {
        for_(($r(0, 4))(px)) {
            if (!tetris_Piece_covers(self->current, self->rotation, px, py)) { continue; }
            if (self->x + as$(i32)(px) == as$(i32)(x) && self->y + as$(i32)(py) == as$(i32)(y)) {
                return some$((O$tetris_Piece)(self->current));
            }
        } $end(for);
    } $end(for);
    return none$((O$tetris_Piece));
}

$static fn_((tetris_ghostCellAt(const tetris_Self* self, i32 ghost_y, usize x, usize y))(bool)) {
    for_(($r(0, 4))(py)) {
        for_(($r(0, 4))(px)) {
            if (!tetris_Piece_covers(self->current, self->rotation, px, py)) { continue; }
            if (self->x + as$(i32)(px) == as$(i32)(x) && ghost_y + as$(i32)(py) == as$(i32)(y)) {
                return true;
            }
        } $end(for);
    } $end(for);
    return false;
}

/*========== Frame / Presenter ============================================*/

fn_((tetris_Frame_clear(tetris_Frame* self))(void)) {
    for_(($s(tetris_Frame_cellsMut(self)))(cell)) *cell = tetris_FrameCell_blank $end(for);
};

fn_((tetris_Frame_cells(const tetris_Frame* self))(S_const$tetris_FrameCell)) {
    return A_ref$((S_const$tetris_FrameCell)(ensureNonnull(self)->cells));
};

fn_((tetris_Frame_cellsMut(tetris_Frame* self))(S$tetris_FrameCell)) {
    return A_ref$((S$tetris_FrameCell)(ensureNonnull(self)->cells));
};

$attr($maybe_unused)
fn_((tetris_Frame_at(const tetris_Frame* self, usize x, usize y))(O$P_const$tetris_FrameCell $scope)) {
    if (tetris_Frame_width <= x || tetris_Frame_height <= y) return_none();
    return_some(S_at((tetris_Frame_cells(self))[y * tetris_Frame_width + x]));
} $unscoped(fn);

fn_((tetris_Frame_atMut(tetris_Frame* self, usize x, usize y))(O$P$tetris_FrameCell) $scope) {
    if (tetris_Frame_width <= x || tetris_Frame_height <= y) return_none();
    return_some(S_at((tetris_Frame_cellsMut(self))[y * tetris_Frame_width + x]));
} $unscoped(fn);

fn_((tetris_Frame_putChar(tetris_Frame* self, usize x, usize y, u8 ch))(void)) {
    *orelse_((tetris_Frame_atMut(self, x, y))(return)) = with_((tetris_FrameCell_blank)(
        (.ch)(ch),
    ));
};

fn_((tetris_Frame_putText(tetris_Frame* self, usize x, usize y, S_const$u8 text))(void)) {
    for_(($s(text), $rf(x))(ch, px)) {
        tetris_Frame_putChar(self, px, y, *ch);
    } $end(for);
};

fn_((tetris_Frame_putTextFg(tetris_Frame* self, usize x, usize y, S_const$u8 text, tetris_RGB fg))(void)) {
    for_(($s(text), $rf(x))(ch, px)) {
        if_some((tetris_Frame_atMut(self, px, y))(cell)) {
            *cell = with_((tetris_FrameCell_blank)(
                (.ch)(*ch),
                (.fg)(some(fg)),
            ));
        };
    } $end(for);
};

fn_((tetris_Frame_putBlock(tetris_Frame* self, usize x, usize y, tetris_Piece piece))(void)) {
    let block = with_((tetris_FrameCell_blank)(
        (.bg)(some(tetris_Piece_color(piece))),
    ));
    if_some((tetris_Frame_atMut(self, x, y))(cell)) *cell = block;
    if_some((tetris_Frame_atMut(self, x + 1, y))(cell)) *cell = block;
};

fn_((tetris_Frame_putGhost(tetris_Frame* self, usize x, usize y))(void)) {
    let ghost = with_((tetris_FrameCell_blank)(
        (.fg)(some(tetris_RGB_ghost)),
    ));
    if_some((tetris_Frame_atMut(self, x, y))(cell)) *cell = ghost;
    let left = with_((ghost)(
        (.ch)('['),
    ));
    if_some((tetris_Frame_atMut(self, x, y))(cell)) *cell = left;
    let right = with_((ghost)(
        (.ch)(']'),
    ));
    if_some((tetris_Frame_atMut(self, x + 1, y))(cell)) *cell = right;
};

$static fn_((tetris_Presenter_init(tetris_Presenter* self, io_Writer out))(tetris_Presenter*)) {
    self->out.writer = io_Buf_Writer_init(out, A_ref$((S$u8)(self->out.mem)));
    tetris_Frame_clear(&self->frame);
    return self;
};

$static fn_((tetris_Presenter_composeMiniPiece(tetris_Frame* frame, O$tetris_Piece piece, usize col, usize row))(void)) {
    if_none(piece) {
        tetris_Frame_putText(frame, col, row, u8_l("(empty)"));
    } else_some(piece_value) {
        for_(($r(0, 4))(py)) {
            for_(($r(0, 4))(px)) {
                if (tetris_Piece_covers(piece_value, tetris_Rotation_0, px, py)) {
                    tetris_Frame_putBlock(frame, col + px * 2, row + py, piece_value);
                }
            } $end(for);
        } $end(for);
    }
}

$static fn_((tetris_Presenter_compose(tetris_Presenter* self, const tetris_Self* game))(void)) {
    let frame = &self->frame;
    tetris_Frame_clear(frame);
    let ghost_y = tetris_ghostY(game);

    tetris_Frame_putText(frame, 0, 0, u8_l("Dasae Tetris"));
    tetris_Frame_putText(frame, 15, 0, u8_l("score"));
    tetris_Frame_putText(frame, 27, 0, u8_l("level"));
    tetris_Frame_putText(frame, 37, 0, u8_l("lines"));

    tetris_Frame_putText(frame, 1, 2, u8_l("+--------------------+"));
    for_(($r(tetris_Board_hidden_rows, tetris_Board_h))(y)) {
        let row = y - tetris_Board_hidden_rows + 3;
        tetris_Frame_putChar(frame, 1, row, '|');
        tetris_Frame_putChar(frame, 22, row, '|');
        for_(($r(0, tetris_Board_w))(x)) {
            let active = tetris_activeCellAt(game, x, y);
            let cell = isSome(active) ? active : *tetris_Board_at(&game->board, x, y);
            if_some((cell)(piece)) {
                tetris_Frame_putBlock(frame, 2 + x * 2, row, piece);
            } else {
                if (isNone(active) && tetris_ghostCellAt(game, ghost_y, x, y)) {
                    tetris_Frame_putGhost(frame, 2 + x * 2, row);
                }
            }
        } $end(for);
    } $end(for);
    tetris_Frame_putText(frame, 1, tetris_Board_visible_h + 3, u8_l("+--------------------+"));

    tetris_Frame_putText(frame, 29, 3, u8_l("hold"));
    if_some((game->hold)(held)) {
        tetris_Frame_putText(frame, 34, 3, tetris_Piece_name(held));
    } else {
        tetris_Frame_putText(frame, 34, 3, u8_l("-"));
    }
    tetris_Presenter_composeMiniPiece(frame, game->hold, 29, 5);

    tetris_Frame_putText(frame, 29, 11, u8_l("next"));
    for_(($r(0, tetris_PieceQ_preview_len))(i)) {
        tetris_Presenter_composeMiniPiece(
            frame,
            some$((O$tetris_Piece)(tetris_PieceQ_peek(&game->queue, i))),
            29,
            13 + i * 4
        );
    } $end(for);

    tetris_Frame_putText(frame, 47, 3, u8_l("left/right : move"));
    tetris_Frame_putText(frame, 47, 4, u8_l("down       : soft drop"));
    tetris_Frame_putText(frame, 47, 5, u8_l("space      : hard drop"));
    tetris_Frame_putText(frame, 47, 6, u8_l("up/x       : rotate cw"));
    tetris_Frame_putText(frame, 47, 7, u8_l("z          : rotate ccw"));
    tetris_Frame_putText(frame, 47, 8, u8_l("c          : hold"));
    tetris_Frame_putText(frame, 47, 9, u8_l("p          : pause"));
    tetris_Frame_putText(frame, 47, 10, u8_l("q/esc      : quit"));

    if (game->is_paused) {
        tetris_Frame_putTextFg(frame, 7, 12, u8_l("PAUSED"), tetris_RGB_pause);
    }
    if (game->is_game_over) {
        tetris_Frame_putTextFg(frame, 7, 12, u8_l("GAME OVER"), tetris_RGB_game_over);
        tetris_Frame_putText(frame, 5, 14, u8_l("press q or esc"));
    }
}

$static fn_((tetris_Presenter_formatU8(S$u8 buf, u8 val))(usize)) {
    if (val >= 100) {
        let hi = val / 100;
        let lo = val % 100;
        *S_at((buf)[0]) = as$(u8)('0' + hi);
        *S_at((buf)[1]) = *S_at((tetris_Presenter_digit_pairs)[lo * 2]);
        *S_at((buf)[2]) = *S_at((tetris_Presenter_digit_pairs)[lo * 2 + 1]);
        return 3;
    }
    if (val >= 10) {
        *S_at((buf)[0]) = *S_at((tetris_Presenter_digit_pairs)[val * 2]);
        *S_at((buf)[1]) = *S_at((tetris_Presenter_digit_pairs)[val * 2 + 1]);
        return 2;
    }
    *S_at((buf)[0]) = as$(u8)('0' + val);
    return 1;
}

$static fn_((tetris_Presenter_writeRGB(S$u8 buf, S_const$u8 prefix, tetris_RGB color))(usize)) {
    var rem = buf;
    rem = S_suffix((mem_copyBytes(rem, prefix))(prefix.len));
    rem = S_suffix((rem)(tetris_Presenter_formatU8(rem, color.r)));
    rem = S_suffix((rem)((*S_at((rem)[0]) = ';', 1)));
    rem = S_suffix((rem)(tetris_Presenter_formatU8(rem, color.g)));
    rem = S_suffix((rem)((*S_at((rem)[0]) = ';', 1)));
    rem = S_suffix((rem)(tetris_Presenter_formatU8(rem, color.b)));
    rem = S_suffix((rem)((*S_at((rem)[0]) = 'm', 1)));
    return buf.len - rem.len;
}

$static fn_((tetris_Presenter_StyleState_init(void))(tetris_Presenter_StyleState)) {
    return (tetris_Presenter_StyleState){
        .fg = none(),
        .bg = none(),
    };
}

$static fn_((tetris_Presenter_StyleState_applyFg(tetris_Presenter_StyleState* self, io_Writer out, O$tetris_RGB next))(E$void) $scope) {
    if_some((self->fg)(curr_rgb)) {
        if_some((next)(next_rgb)) {
            if (tetris_RGB_eql(curr_rgb, next_rgb)) { return_ok({}); }
        }
    } else_none {
        if_none(next) { return_ok({}); }
    }

    if_none(next) {
        try_(io_Writer_writeBytes(out, u8_l("\x1B[39m")));
        asg_l((&self->fg)(none()));
        return_ok({});
    } else_some(next_rgb) {
        var_(seq_mem, A$$(24, u8)) $undefined;
        let seq = A_ref$((S$u8)(seq_mem));
        let len = tetris_Presenter_writeRGB(seq, u8_l("\x1B[38;2;"), next_rgb);
        try_(io_Writer_writeBytes(out, S_prefix((seq)(len)).as_const));
        asg_l((&self->fg)(some(next_rgb)));
        return_ok({});
    }
    return_ok({});
} $unscoped(fn);

$static fn_((tetris_Presenter_StyleState_applyBg(tetris_Presenter_StyleState* self, io_Writer out, O$tetris_RGB next))(E$void) $scope) {
    if_some((self->bg)(curr_rgb)) {
        if_some((next)(next_rgb)) {
            if (tetris_RGB_eql(curr_rgb, next_rgb)) { return_ok({}); }
        }
    } else_none {
        if_none(next) { return_ok({}); }
    }

    if_none(next) {
        try_(io_Writer_writeBytes(out, u8_l("\x1B[49m")));
        asg_l((&self->bg)(none()));
        return_ok({});
    } else_some(next_rgb) {
        var_(seq_mem, A$$(24, u8)) $undefined;
        let seq = A_ref$((S$u8)(seq_mem));
        let len = tetris_Presenter_writeRGB(seq, u8_l("\x1B[48;2;"), next_rgb);
        try_(io_Writer_writeBytes(out, S_prefix((seq)(len)).as_const));
        asg_l((&self->bg)(some(next_rgb)));
        return_ok({});
    }
    return_ok({});
} $unscoped(fn);

$static fn_((tetris_Presenter_StyleState_applyCell(tetris_Presenter_StyleState* self, io_Writer out, tetris_FrameCell cell))(E$void) $scope) {
    try_(tetris_Presenter_StyleState_applyFg(self, out, cell.fg));
    try_(tetris_Presenter_StyleState_applyBg(self, out, cell.bg));
    return_ok({});
} $unscoped(fn);

$static fn_((tetris_Presenter_present(tetris_Presenter* self))(E$void) $scope) {
    let out = io_Buf_writer(&self->out.writer);
    try_(io_Writer_writeBytes(out, u8_l("\x1B[H\x1B[2J")));

    var style = tetris_Presenter_StyleState_init();

    for_(($rt(tetris_Frame_height))(y)) {
        let rows = S_slice((tetris_Frame_cells(&self->frame))$r(
            tetris_Frame_width * y,
            tetris_Frame_width * (y + 1)
        ));
        for_(($s(rows))(cell)) {
            try_(tetris_Presenter_StyleState_applyCell(&style, out, *cell));
            try_(io_Writer_writeByte(out, cell->ch));
        } $end(for);
        try_(tetris_Presenter_StyleState_applyFg(&style, out, none$((O$tetris_RGB))));
        try_(tetris_Presenter_StyleState_applyBg(&style, out, none$((O$tetris_RGB))));
        try_(io_Writer_writeByte(out, '\n'));
        try_(io_Buf_Writer_flush(&self->out.writer));
    } $end(for);
    try_(io_Writer_writeBytes(out, u8_l("\x1B[0m")));
    try_(io_Buf_Writer_flush(&self->out.writer));
    return_ok({});
} $unscoped(fn);

$static fn_((tetris_Presenter_render(tetris_Presenter* self, const tetris_Self* game))(E$void)) {
    tetris_Presenter_compose(self, game);
    return tetris_Presenter_present(self);
}
