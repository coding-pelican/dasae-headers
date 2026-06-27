#include <dh/prl.h>

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
$static let_(tetris_RGB_screen_too_small, tetris_RGB) = { 255, 120, 35 };
$static let_(tetris_RGB_screen_too_small_text, tetris_RGB) = { 255, 220, 180 };
$static fn_((tetris_RGB_eql(tetris_RGB lhs, tetris_RGB rhs))(bool));
$static fn_((tetris_RGB_dimmedGray(tetris_RGB self))(tetris_RGB));

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

#include <dh/Rand.h>

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

#include <dh/time/Dur.h>
#include <dh/ascii.h>
#include "daterm-runtime/key.h"
#include "daterm-runtime/Term.h"

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
$static fn_((tetris_rotationNext(tetris_Rotation rotation, i32 dir))(tetris_Rotation));
$static fn_((tetris_kickIndex(tetris_Rotation from, tetris_Rotation to))(usize));
$static fn_((tetris_tryRotate(tetris_Self* self, i32 dir))(bool));
$static fn_((tetris_lock(tetris_Self* self))(void));
$static fn_((tetris_hardDrop(tetris_Self* self))(void));
$static fn_((tetris_hold(tetris_Self* self))(void));
$static fn_((tetris_tick(tetris_Self* self, time_Dur dt))(void));
$static fn_((tetris_cmdFromKey(daterm_key_Event key))(E$O$tetris_Cmd)) $must_check;
$static fn_((tetris_cmdFromText(daterm_key_Text text))(E$O$tetris_Cmd)) $must_check;
$static fn_((tetris_applyCmd(tetris_Self* self, tetris_Cmd cmd))(void));
$static fn_((tetris_pollInput(tetris_Self* self, daterm_Term term))(E$void)) $must_check;
$static fn_((tetris_ghostY(const tetris_Self* self))(i32));
$static fn_((tetris_activeCellAt(const tetris_Self* self, usize x, usize y))(O$tetris_Piece));
$static fn_((tetris_ghostCellAt(const tetris_Self* self, i32 ghost_y, usize x, usize y))(bool));

/*========== Frame / Presenter ============================================*/

#include <dh/io/Buf.h>

#define tetris_Frame_width 72ull
#define tetris_Frame_height (tetris_Board_visible_h + 4ull)
#define tetris_Frame_area (tetris_Frame_width * tetris_Frame_height)
#define tetris_Presenter_buf_len 16384ull

typedef A$$(8, S_const$u8) tetris_Presenter_HelpLines;
$static let_(tetris_Presenter_help_lines, tetris_Presenter_HelpLines) = A_init({
    u8_l("left/right : move"),
    u8_l("down       : soft drop"),
    u8_l("space      : hard drop"),
    u8_l("up/x       : rotate cw"),
    u8_l("z          : rotate ccw"),
    u8_l("c          : hold"),
    u8_l("p          : pause"),
    u8_l("q/esc      : quit"),
});

typedef struct tetris_Presenter_Layout {
    var_(header_y, usize);
    var_(header_value_y, usize);
    var_(title_x, usize);
    var_(score_x, usize);
    var_(level_x, usize);
    var_(lines_x, usize);
    var_(board_cell_w, usize);
    var_(board_wall_x, usize);
    var_(board_cell_x, usize);
    var_(board_wall_r, usize);
    var_(board_top_y, usize);
    var_(board_cell_y, usize);
    var_(board_bottom_y, usize);
    var_(panel_x, usize);
    var_(panel_name_x, usize);
    var_(panel_piece_x, usize);
    var_(hold_y, usize);
    var_(hold_piece_y, usize);
    var_(next_y, usize);
    var_(next_piece_y, usize);
    var_(next_piece_stride_y, usize);
    var_(help_x, usize);
    var_(help_y, usize);
    var_(status_x, usize);
    var_(status_y, usize);
    var_(game_over_hint_x, usize);
    var_(game_over_hint_y, usize);
} tetris_Presenter_Layout;
T_use_prl$(tetris_Presenter_Layout);

typedef struct tetris_Presenter_MessageLine {
    var_(text, S_const$u8);
    var_(fg, tetris_RGB);
} tetris_Presenter_MessageLine;
T_use_prl$(tetris_Presenter_MessageLine);

typedef A$$(5, tetris_Presenter_MessageLine) tetris_Presenter_MessageLines;

typedef enum_((tetris_SmallScreen_SimPolicy $fits($packed))(
    tetris_small_screen_sim_policy_pause = 0,
    tetris_small_screen_sim_policy_continue,
    count$tetris_SmallScreen_SimPolicy,
)) tetris_SmallScreen_SimPolicy;
T_use_prl$(tetris_SmallScreen_SimPolicy);
claim_assert_static(eqlType$(tetris_SmallScreen_SimPolicy, u8));

typedef enum_((tetris_SmallScreen_RenderPolicy $fits($packed))(
    tetris_small_screen_render_policy_clear = 0,
    tetris_small_screen_render_policy_dim,
    count$tetris_SmallScreen_RenderPolicy,
)) tetris_SmallScreen_RenderPolicy;
T_use_prl$(tetris_SmallScreen_RenderPolicy);
claim_assert_static(eqlType$(tetris_SmallScreen_RenderPolicy, u8));

typedef struct tetris_SmallScreen_Cfg {
    var_(sim_policy, tetris_SmallScreen_SimPolicy);
    var_(render_policy, tetris_SmallScreen_RenderPolicy);
} tetris_SmallScreen_Cfg;
T_use_prl$(tetris_SmallScreen_Cfg);
$static let_(tetris_SmallScreen_Cfg_default, tetris_SmallScreen_Cfg) = {
    .sim_policy = tetris_small_screen_sim_policy_pause,
    .render_policy = tetris_small_screen_render_policy_dim,
};
$static fn_((tetris_SmallScreen_Cfg_fromArgs(S$S_const$u8 args))(tetris_SmallScreen_Cfg));
$static fn_((tetris_SmallScreen_Cfg_shouldTick(tetris_SmallScreen_Cfg self, bool is_small))(bool));

typedef struct tetris_FrameCell {
    var_(ch, u32);
    var_(fg, O$tetris_RGB);
    var_(bg, O$tetris_RGB);
} tetris_FrameCell;
T_use_prl$(tetris_FrameCell);
$static let_(tetris_FrameCell_blank, tetris_FrameCell) = {
    .ch = ' ',
    .fg = none(),
    .bg = none(),
};
$static let_(tetris_Glyph_upper_half, u32) = 0x2580;
$static let_(tetris_Glyph_lower_half, u32) = 0x2584;
$static let_(tetris_Glyph_full_block, u32) = 0x2588;

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
$static fn_((tetris_Frame_putMiniBlock(
    tetris_Frame* self, usize x, usize y, tetris_Piece piece, bool top, bool bottom
))(void));
$static fn_((tetris_Frame_putGhost(tetris_Frame* self, usize x, usize y))(void));
$static fn_((tetris_FrameCell_eql(tetris_FrameCell lhs, tetris_FrameCell rhs))(bool));
$static fn_((tetris_FrameCell_dimmedGray(tetris_FrameCell self))(tetris_FrameCell));

typedef struct tetris_Presenter {
    var_(frame, tetris_Frame);
    var_(front, tetris_Frame);
    var_(front_valid, bool);
    var_(out, struct {
        var_(mem, A$$(tetris_Presenter_buf_len, u8));
        var_(writer, io_Buf_Writer);
    });
} tetris_Presenter;

typedef struct tetris_Presenter_StyleState {
    var_(fg, O$tetris_RGB);
    var_(bg, O$tetris_RGB);
} tetris_Presenter_StyleState;

typedef struct tetris_Presenter_DiffStats {
    var_(changed_cells, usize);
    var_(changed_runs, usize);
} tetris_Presenter_DiffStats;
T_use_prl$(tetris_Presenter_DiffStats);

$static fn_((tetris_Presenter_init(tetris_Presenter* self, io_Writer out))(tetris_Presenter*));
$static fn_((tetris_Presenter_HelpLines_width(void))(usize));
$static fn_((tetris_Presenter_Layout_centeredX(usize left, usize width, S_const$u8 text))(usize));
$static fn_((tetris_Presenter_Layout_calc(void))(tetris_Presenter_Layout));
$static fn_((tetris_Presenter_MessageLines_firstY(tetris_Presenter_MessageLines lines, daterm_CellSize screen))(usize));
$static fn_((tetris_Presenter_composeBoardBorder(tetris_Frame* frame, tetris_Presenter_Layout layout, usize y))(void));
$static fn_((tetris_Presenter_composeHelp(tetris_Frame* frame, tetris_Presenter_Layout layout))(void));
$static fn_((tetris_Presenter_composeMiniPiece(tetris_Frame* frame, O$tetris_Piece piece, usize col, usize row))(void));
$static fn_((tetris_Presenter_compose(tetris_Presenter* self, const tetris_Self* game))(void));
$static fn_((tetris_Presenter_composeU32(tetris_Frame* frame, usize x, usize y, u32 val))(void));
$static fn_((tetris_Presenter_present(tetris_Presenter* self))(E$void)) $must_check;
$static fn_((tetris_Presenter_presentFull(tetris_Presenter* self, bool clear))(E$void)) $must_check;
$static fn_((tetris_Presenter_presentDiff(tetris_Presenter* self))(E$void)) $must_check;
$static fn_((tetris_Presenter_presentSmallScreen(tetris_Presenter* self, daterm_CellSize screen, tetris_SmallScreen_Cfg cfg))(E$void)) $must_check;
$static fn_((tetris_Presenter_render(tetris_Presenter* self, const tetris_Self* game, daterm_CellSize screen, tetris_SmallScreen_Cfg cfg))(E$void)) $must_check;
$static fn_((tetris_Presenter_DiffStats_calc(const tetris_Presenter* self))(tetris_Presenter_DiffStats));
$static fn_((tetris_Presenter_DiffStats_prefersFull(tetris_Presenter_DiffStats self))(bool));
$static fn_((tetris_Presenter_smallScreenBgCell(const tetris_Presenter* self, usize x, usize y, tetris_SmallScreen_RenderPolicy policy))(tetris_FrameCell));
$static fn_((tetris_Presenter_smallScreenBorderCell(tetris_FrameCell under, daterm_CellSize screen, usize x, usize y))(tetris_FrameCell));
$static fn_((tetris_Presenter_smallScreenTextCell(tetris_FrameCell under, u16 cols, usize x, usize y, usize text_y, S_const$u8 text, tetris_RGB fg))(tetris_FrameCell));
$static fn_((tetris_Presenter_formatU32(S$u8 buf, u32 val))(usize));
$static fn_((tetris_Presenter_writeGlyph(u32 glyph, io_Writer out))(E$void)) $must_check;
$static fn_((tetris_Presenter_moveToCell(usize x, usize y, io_Writer out))(E$void)) $must_check;
$static fn_((tetris_Presenter_beginScreen(io_Writer out))(E$void)) $must_check;
$static fn_((tetris_Presenter_homeScreen(io_Writer out))(E$void)) $must_check;
$static fn_((tetris_Presenter_StyleState_init(void))(tetris_Presenter_StyleState));
$static fn_((tetris_Presenter_StyleState_applyFg(tetris_Presenter_StyleState* self, io_Writer out, O$tetris_RGB next))(E$void)) $must_check;
$static fn_((tetris_Presenter_StyleState_applyBg(tetris_Presenter_StyleState* self, io_Writer out, O$tetris_RGB next))(E$void)) $must_check;
$static fn_((tetris_Presenter_StyleState_applyCell(tetris_Presenter_StyleState* self, io_Writer out, tetris_FrameCell cell))(E$void)) $must_check;

/*========== Main ===========================================================*/

#include <dh-main.h>
#include <dh/heap/Sys.h>
#include <dh/heap/Arena.h>
#include "daterm-context/ANSI.h"
#include <dansi-xterm/screen.h>
#include <dansi-dec/cursor.h>
#include <dh/time/Clock.h>

$static fn_((tetris_Main_waitForEnter(void))(void));

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let small_screen_cfg = tetris_SmallScreen_Cfg_fromArgs(args);

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
    try_(dansi_xterm_screen_enterAlternateWrite(out));
    defer_(catch_((dansi_xterm_screen_exitAlternateWrite(out))($ignore, $do_nothing)));
    try_(dansi_dec_cursor_hideWrite(out));
    defer_(catch_((dansi_dec_cursor_showWrite(out))($ignore, $do_nothing)));

    $static var_(game_storage, tetris_Self) $undefined_static;
    let game = tetris_init(&game_storage);

    $static var_(presenter_storage, tetris_Presenter) $undefined_static;
    let presenter = tetris_Presenter_init(&presenter_storage, out);

    let frame_interval = time_Dur_fromSecs$f64(1.0 / 30.0);
    var schedule_from = time_Clock_now(ansi.clock);
    var last_tick = schedule_from;
    while (game->is_running) {
        let now = time_Clock_now(ansi.clock);
        let dt = time_Clock_Inst_durSince(now, last_tick);
        last_tick = now;
        let screen = try_(daterm_Term_queryNativeScreenCells(term));
        let is_small_screen = screen.cols < tetris_Frame_width || screen.rows < tetris_Frame_height;

        try_(tetris_pollInput(game, term));
        if (tetris_SmallScreen_Cfg_shouldTick(small_screen_cfg, is_small_screen)) {
            tetris_tick(game, dt);
        }
        try_(tetris_Presenter_render(presenter, game, screen, small_screen_cfg));

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

fn_((tetris_Main_waitForEnter(void))(void)) {
    let out = fs_File_writer(io_handleStdOut());
    catch_((io_Writer_writeBytes(out, u8_l("example-tetris: press Enter to enter terminal mode\n")))($ignore, $do_nothing));

    var_(read_mem, A$$(128, u8)) $undefined;
    var reader = io_Buf_Reader_init(fs_File_reader(io_handleStdIn()), A_ref$((S$u8)(read_mem)));
    var_(line_mem, A$$(256, u8)) $undefined;
    catch_((io_Buf_Reader_readUntilByte(&reader, ascii_lf_byte, A_ref$((S$u8)(line_mem))))($ignore, $do_nothing));
};

/*========== RGB ============================================================*/

fn_((tetris_RGB_eql(tetris_RGB lhs, tetris_RGB rhs))(bool)) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
};

fn_((tetris_RGB_dimmedGray(tetris_RGB self))(tetris_RGB)) {
    let gray = as$(u8)((as$(u16)(self.r) * 30u + as$(u16)(self.g) * 59u + as$(u16)(self.b) * 11u) / 200u);
    return (tetris_RGB){ gray, gray, gray };
};

/*========== Piece ==========================================================*/

fn_((tetris_Piece_covers(tetris_Piece self, tetris_Rotation rotation, usize x, usize y))(bool)) {
    claim_assert(x < 4);
    claim_assert(y < 4);
    let mask = *A_at((*A_at((tetris_Piece_masks)[self]))[rotation]);
    let bit_idx = (3 - y) * 4 + (3 - x);
    return ((mask >> bit_idx) & 1u) != 0;
};

fn_((tetris_Piece_color(tetris_Piece self))(tetris_RGB)) {
    return *A_at((tetris_Piece_colors)[self]);
};

fn_((tetris_Piece_name(tetris_Piece self))(S_const$u8)) {
    return *A_at((tetris_Piece_names)[self]);
};

/*========== Board ==========================================================*/

fn_((tetris_Board_clear(tetris_Board* self))(void)) {
    for_(($s(A_ref(self->cells)))(cell)) asg_l((cell)(none())) $end(for);
};

fn_((tetris_Board_at(const tetris_Board* self, usize x, usize y))(const O$tetris_Piece*)) {
    claim_assert(x < tetris_Board_w);
    claim_assert(y < tetris_Board_h);
    return A_at((self->cells)[y * tetris_Board_w + x]);
};

fn_((tetris_Board_atMut(tetris_Board* self, usize x, usize y))(O$tetris_Piece*)) {
    claim_assert(x < tetris_Board_w);
    claim_assert(y < tetris_Board_h);
    return A_at((self->cells)[y * tetris_Board_w + x]);
};

fn_((tetris_Board_clearLines(tetris_Board* self))(usize)) {
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
};

/*========== Queue ==========================================================*/

fn_((tetris_PieceQ_init(tetris_PieceQ* self))(void)) {
    asg_l((self)({
        .len = 0,
        .rng = Rand_init(),
    }));
    tetris_PieceQ_pushBag(self);
    tetris_PieceQ_pushBag(self);
};

fn_((tetris_PieceQ_pushBag(tetris_PieceQ* self))(void)) {
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
};

fn_((tetris_PieceQ_pop(tetris_PieceQ* self))(tetris_Piece)) {
    if (self->len < tetris_PieceQ_Bag_len) { tetris_PieceQ_pushBag(self); }
    let piece = *A_at((self->items)[0]);
    for_(($r(1, self->len))(i)) {
        *A_at((self->items)[i - 1]) = *A_at((self->items)[i]);
    } $end(for);
    self->len -= 1;
    if (self->len < tetris_PieceQ_Bag_len) { tetris_PieceQ_pushBag(self); }
    return piece;
};

fn_((tetris_PieceQ_peek(const tetris_PieceQ* self, usize index))(tetris_Piece)) {
    claim_assert(index < self->len);
    return *A_at((self->items)[index]);
};

/*========== Game ===========================================================*/

#include <dh/utf8.h>
#include <dh/io/stream.h>
#include <dansi-core/erase.h>
#include <dansi-core/cursor.h>
#include <dansi-xterm/color.h>
#include <dh/io/Fixed.h>

fn_((tetris_gravityForLevel(u32 level))(time_Dur)) {
    $static let_(gravity_ms, A$$(10, u32)) = A_init({ 800, 717, 633, 550, 467, 383, 300, 217, 133, 100 });
    if (level == 0) { level = 1; }
    if (level <= A_len(gravity_ms)) return time_Dur_fromMillis(*A_at((gravity_ms)[level - 1]));
    if (19u <= level) return time_Dur_fromMillis(50);
    return time_Dur_fromMillis(100u - (level - 10u) * 6u);
};

fn_((tetris_init(tetris_Self* self))(tetris_Self*)) {
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
};

fn_((tetris_spawn(tetris_Self* self))(void)) {
    self->current = tetris_PieceQ_pop(&self->queue);
    self->rotation = tetris_Rotation_0;
    self->x = tetris_spawn_x;
    self->y = tetris_spawn_y;
    self->gravity_elapsed = time_Dur_zero;
    self->lock_elapsed = time_Dur_zero;
    self->can_hold = true;
    self->is_game_over = !tetris_pieceFits(self, self->current, self->rotation, self->x, self->y);
};

fn_((tetris_pieceFits(const tetris_Self* self, tetris_Piece piece, tetris_Rotation rotation, i32 x, i32 y))(bool)) {
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
};

fn_((tetris_isGrounded(const tetris_Self* self))(bool)) {
    return !tetris_pieceFits(self, self->current, self->rotation, self->x, self->y + 1);
};

fn_((tetris_tryMove(tetris_Self* self, i32 dx, i32 dy))(bool)) {
    if (!tetris_pieceFits(self, self->current, self->rotation, self->x + dx, self->y + dy)) {
        return false;
    }
    self->x += dx;
    self->y += dy;
    if (dx != 0 || dy == 0) { self->lock_elapsed = time_Dur_zero; }
    return true;
};

fn_((tetris_rotationNext(tetris_Rotation rotation, i32 dir))(tetris_Rotation)) {
    let value = (as$(i32)(rotation) + dir + as$(i32)(count$tetris_Rotation)) % as$(i32)(count$tetris_Rotation);
    return as$(tetris_Rotation)(value);
};

fn_((tetris_kickIndex(tetris_Rotation from, tetris_Rotation to))(usize)) {
    if (from == tetris_Rotation_0 && to == tetris_Rotation_r) { return 0; }
    if (from == tetris_Rotation_r && to == tetris_Rotation_0) { return 1; }
    if (from == tetris_Rotation_r && to == tetris_Rotation_2) { return 2; }
    if (from == tetris_Rotation_2 && to == tetris_Rotation_r) { return 3; }
    if (from == tetris_Rotation_2 && to == tetris_Rotation_l) { return 4; }
    if (from == tetris_Rotation_l && to == tetris_Rotation_2) { return 5; }
    if (from == tetris_Rotation_l && to == tetris_Rotation_0) { return 6; }
    claim_assert(from == tetris_Rotation_0 && to == tetris_Rotation_l);
    return 7;
};

fn_((tetris_tryRotate(tetris_Self* self, i32 dir))(bool)) {
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
        let ny = self->y - test->y;
        if (tetris_pieceFits(self, self->current, to, nx, ny)) {
            self->x = nx;
            self->y = ny;
            self->rotation = to;
            self->lock_elapsed = time_Dur_zero;
            return true;
        }
    } $end(for);
    return false;
};

fn_((tetris_lock(tetris_Self* self))(void)) {
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
};

fn_((tetris_hardDrop(tetris_Self* self))(void)) {
    var_(dropped, u32) = 0;
    while (tetris_tryMove(self, 0, 1)) {
        dropped += 1;
    }
    self->score += dropped * 2u;
    tetris_lock(self);
};

fn_((tetris_hold(tetris_Self* self))(void)) {
    if (!self->can_hold) return;
    if_none(self->hold) {
        self->hold = some$((O$tetris_Piece)(self->current));
        tetris_spawn(self);
    } else_some((held)) {
        self->hold = some$((O$tetris_Piece)(self->current));
        self->current = held;
        self->rotation = tetris_Rotation_0;
        self->x = tetris_spawn_x;
        self->y = tetris_spawn_y;
        self->gravity_elapsed = time_Dur_zero;
        self->lock_elapsed = time_Dur_zero;
        self->is_game_over = !tetris_pieceFits(self, self->current, self->rotation, self->x, self->y);
    };
    self->can_hold = false;
};

fn_((tetris_tick(tetris_Self* self, time_Dur dt))(void)) {
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
};

fn_((tetris_cmdFromKey(daterm_key_Event key))(E$O$tetris_Cmd) $scope) {
    if_some((key.action)(action)) {
        if (action == daterm_key_Action_release) return_ok(none());
        if (action == daterm_key_Action_repeat
            && key.code != daterm_key_Code_left
            && key.code != daterm_key_Code_right
            && key.code != daterm_key_Code_down
            && key.code != daterm_key_Code_keypad_4
            && key.code != daterm_key_Code_keypad_6
            && key.code != daterm_key_Code_keypad_2) {
            return_ok(none());
        }
    }
    $suppress_(switch_enum)(switch (key.code)) {
    case_((daterm_key_Code_escape)) return_ok(some(tetris_cmd_quit)) $end(case);
    case daterm_key_Code_left: $fallthrough;
    case_((daterm_key_Code_keypad_4)) return_ok(some(tetris_cmd_move_left)) $end(case);
    case daterm_key_Code_right: $fallthrough;
    case_((daterm_key_Code_keypad_6)) return_ok(some(tetris_cmd_move_right)) $end(case);
    case daterm_key_Code_down: $fallthrough;
    case_((daterm_key_Code_keypad_2)) return_ok(some(tetris_cmd_soft_drop)) $end(case);
    case daterm_key_Code_up: $fallthrough;
    case_((daterm_key_Code_keypad_8)) return_ok(some(tetris_cmd_rotate_cw)) $end(case);
    default_() $do_nothing $end(default);
    }
    return_ok(none());
} $unscoped(fn);

fn_((tetris_cmdFromText(daterm_key_Text text))(E$O$tetris_Cmd) $scope) {
    if_some((text.action)(action)) {
        if (action != daterm_key_Action_press) return_ok(none());
    }
    let len = try_((utf8_codepointSeqLen(text.codepoint)));
    if (len != utf8_SeqLen_1) return_ok(none());
    let ch = ascii_toLower(intCast$((u8)(text.codepoint)));
    if (ch == 'q' || (text.mods.ctrl && ch == 'c')) return_ok(some(tetris_cmd_quit));
    if (ch == 'p') return_ok(some(tetris_cmd_toggle_pause));
    if (ch == ' ') return_ok(some(tetris_cmd_hard_drop));
    if (ch == 'z') return_ok(some(tetris_cmd_rotate_ccw));
    if (ch == 'x') return_ok(some(tetris_cmd_rotate_cw));
    if (ch == 'c') return_ok(some(tetris_cmd_hold));
    return_ok(none());
} $unscoped(fn);

fn_((tetris_applyCmd(tetris_Self* self, tetris_Cmd cmd))(void)) {
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
};

fn_((tetris_pollInput(tetris_Self* self, daterm_Term term))(E$void) $scope) {
    while (true) {
        let event = orelse_((daterm_Term_poll(term))(return_ok({})));
        $suppress_(switch_enum)(match_(event)) {
        patt_((daterm_Event_key)(key)) {
            let cmd = try_(tetris_cmdFromKey(key));
            if_some((cmd)(value)) tetris_applyCmd(self, value);
        } $end(patt);
        patt_((daterm_Event_text)(text)) {
            let cmd = try_(tetris_cmdFromText(text));
            if_some((cmd)(value)) tetris_applyCmd(self, value);
        } $end(patt);
        default_() $do_nothing $end(default);
        } $end(match);
    }
} $unscoped(fn);

fn_((tetris_ghostY(const tetris_Self* self))(i32)) {
    var_(y, i32) = self->y;
    while (tetris_pieceFits(self, self->current, self->rotation, self->x, y + 1)) {
        y += 1;
    }
    return y;
};

fn_((tetris_activeCellAt(const tetris_Self* self, usize x, usize y))(O$tetris_Piece)) {
    for_(($r(0, 4))(py)) {
        for_(($r(0, 4))(px)) {
            if (!tetris_Piece_covers(self->current, self->rotation, px, py)) { continue; }
            if (self->x + as$(i32)(px) == as$(i32)(x) && self->y + as$(i32)(py) == as$(i32)(y)) {
                return some$((O$tetris_Piece)(self->current));
            }
        } $end(for);
    } $end(for);
    return none$((O$tetris_Piece));
};

fn_((tetris_ghostCellAt(const tetris_Self* self, i32 ghost_y, usize x, usize y))(bool)) {
    for_(($r(0, 4))(py)) {
        for_(($r(0, 4))(px)) {
            if (!tetris_Piece_covers(self->current, self->rotation, px, py)) { continue; }
            if (self->x + as$(i32)(px) == as$(i32)(x) && ghost_y + as$(i32)(py) == as$(i32)(y)) {
                return true;
            }
        } $end(for);
    } $end(for);
    return false;
};

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

fn_((tetris_Frame_putMiniBlock(
    tetris_Frame* self,
    usize x,
    usize y,
    tetris_Piece piece,
    bool top,
    bool bottom
))(void)) {
    if (!top && !bottom) return;
    let glyph = top && bottom
                  ? tetris_Glyph_full_block
              : top
                  ? tetris_Glyph_upper_half
                  : tetris_Glyph_lower_half;
    if_some((tetris_Frame_atMut(self, x, y))(cell)) {
        *cell = with_((tetris_FrameCell_blank)(
            (.ch)(glyph),
            (.fg)(some(tetris_Piece_color(piece))),
        ));
    };
};

fn_((tetris_Frame_putGhost(tetris_Frame* self, usize x, usize y))(void)) {
    let ghost = with_((tetris_FrameCell_blank)(
        (.fg)(some(tetris_RGB_ghost)),
    ));
    let left = with_((ghost)(
        (.ch)('['),
    ));
    if_some((tetris_Frame_atMut(self, x, y))(cell)) *cell = left;
    let right = with_((ghost)(
        (.ch)(']'),
    ));
    if_some((tetris_Frame_atMut(self, x + 1, y))(cell)) *cell = right;
};

fn_((tetris_FrameCell_eql(tetris_FrameCell lhs, tetris_FrameCell rhs))(bool)) {
    if (lhs.ch != rhs.ch) return false;

    if (isSome(lhs.fg) != isSome(rhs.fg)) return false;
    if_some((lhs.fg)(lhs_fg)) {
        if_some((rhs.fg)(rhs_fg)) {
            if (!tetris_RGB_eql(lhs_fg, rhs_fg)) return false;
        };
    };

    if (isSome(lhs.bg) != isSome(rhs.bg)) return false;
    if_some((lhs.bg)(lhs_bg)) {
        if_some((rhs.bg)(rhs_bg)) {
            if (!tetris_RGB_eql(lhs_bg, rhs_bg)) return false;
        };
    };

    return true;
};

fn_((tetris_FrameCell_dimmedGray(tetris_FrameCell self))(tetris_FrameCell)) {
    if_some((self.fg)(fg)) asg_l((&self.fg)(some(tetris_RGB_dimmedGray(fg))));
    if_some((self.bg)(bg)) asg_l((&self.bg)(some(tetris_RGB_dimmedGray(bg))));
    return self;
};

fn_((tetris_SmallScreen_Cfg_fromArgs(S$S_const$u8 args))(tetris_SmallScreen_Cfg)) {
    var cfg = tetris_SmallScreen_Cfg_default;
    for_(($s(args))(arg)) {
        let flag_prefix = u8_l("--");
        if (!mem_startsWithBytes(*arg, flag_prefix)) continue;

        let flag_chunk = S_suffix((*arg)(flag_prefix.len));
        let opt_delim = u8_l("=");
        let opt_idx = orelse_((mem_findFirstSeqBytes(flag_chunk, opt_delim))({
            io_stream_eprintln(u8_l("invalid flag format: {:s}"), *arg), start_exit(1);
        }));

        let flag = S_prefix((flag_chunk)(opt_idx));
        let opt = S_suffix((flag_chunk)(opt_idx + opt_delim.len));
        if (mem_eqlBytes(flag, u8_l("small-screen-sim"))) {
            if (mem_eqlBytes(opt, u8_l("pause"))) {
                cfg.sim_policy = tetris_small_screen_sim_policy_pause;
            } else if (mem_eqlBytes(opt, u8_l("continue"))) {
                cfg.sim_policy = tetris_small_screen_sim_policy_continue;
            } else {
                io_stream_eprintln(u8_l("invalid `small-screen-sim` option: {:s}"), opt), start_exit(1);
            }
        } else if (mem_eqlBytes(flag, u8_l("small-screen-render"))) {
            if (mem_eqlBytes(opt, u8_l("clear"))) {
                cfg.render_policy = tetris_small_screen_render_policy_clear;
            } else if (mem_eqlBytes(opt, u8_l("dim"))) {
                cfg.render_policy = tetris_small_screen_render_policy_dim;
            } else {
                io_stream_eprintln(u8_l("invalid `small-screen-render` option: {:s}"), opt), start_exit(1);
            }
        } else {
            io_stream_eprintln(u8_l("invalid flag: {:s}"), flag), start_exit(1);
        }
    } $end(for);
    return cfg;
};

fn_((tetris_SmallScreen_Cfg_shouldTick(tetris_SmallScreen_Cfg self, bool is_small))(bool)) {
    return !is_small || self.sim_policy == tetris_small_screen_sim_policy_continue;
};

fn_((tetris_Presenter_init(tetris_Presenter* self, io_Writer out))(tetris_Presenter*)) {
    self->out.writer = io_Buf_Writer_init(out, A_ref$((S$u8)(self->out.mem)));
    tetris_Frame_clear(&self->frame);
    tetris_Frame_clear(&self->front);
    self->front_valid = false;
    return self;
};

fn_((tetris_Presenter_HelpLines_width(void))(usize)) {
    var_(width, usize) = 0;
    for_(($a(tetris_Presenter_help_lines))(line)) {
        if (width < line->len) { width = line->len; }
    } $end(for);
    return width;
};

fn_((tetris_Presenter_Layout_centeredX(usize left, usize width, S_const$u8 text))(usize)) {
    return text.len < width ? left + (width - text.len) / 2 : left;
};

fn_((tetris_Presenter_Layout_calc(void))(tetris_Presenter_Layout)) {
    let board_cell_w = 2ull;
    let board_cell_h = 1ull;
    let board_wall_w = 1ull;
    let board_inner_w = tetris_Board_w * board_cell_w;
    let board_outer_w = board_inner_w + board_wall_w * 2ull;
    let mini_piece_w = 4ull;
    let mini_piece_h = 2ull;
    var_(panel_w, usize) = mini_piece_w;
    if (panel_w < u8_l("(empty)").len) { panel_w = u8_l("(empty)").len; }

    let help_w = tetris_Presenter_HelpLines_width();
    let content_col_count = 3ull;
    let gutter_count = content_col_count + 1ull;
    let content_w = board_outer_w + panel_w + help_w;
    let gap = tetris_Frame_width <= content_w ? 1ull : (tetris_Frame_width - content_w) / gutter_count;
    let board_wall_x = gap;
    let board_cell_x = board_wall_x + board_wall_w;
    let board_wall_r = board_cell_x + board_inner_w;
    let panel_x = board_wall_r + board_wall_w + gap;
    let help_x = panel_x + panel_w + gap;
    let header_h = 1ull;
    let label_h = 1ull;
    let row_gap = 1ull;
    let section_gap_h = row_gap;
    let board_top_y = header_h + row_gap;
    let board_cell_y = board_top_y + board_cell_h;
    let board_bottom_y = board_cell_y + tetris_Board_visible_h * board_cell_h;
    let hold_y = board_cell_y;
    let hold_piece_y = hold_y + label_h + row_gap;
    let next_y = hold_piece_y + mini_piece_h + section_gap_h;
    let next_piece_y = next_y + label_h + row_gap;
    let next_piece_stride_y = mini_piece_h + row_gap;
    let status_y = board_cell_y + tetris_Board_visible_h / 2ull - 1ull;
    let header_field_w = 12ull;
    let score_x = panel_x;
    let level_x = score_x + header_field_w;
    let lines_x = level_x + header_field_w;
    let game_over_text = u8_l("GAME OVER");
    let game_over_hint = u8_l("press q or esc");
    claim_assert(board_bottom_y < tetris_Frame_height);
    claim_assert(
        next_piece_y
            + (tetris_PieceQ_preview_len - 1) * next_piece_stride_y
            + mini_piece_h
        <= tetris_Frame_height
    );

    return (tetris_Presenter_Layout){
        .header_y = 0,
        .header_value_y = label_h,
        .title_x = board_wall_x,
        .score_x = score_x,
        .level_x = level_x,
        .lines_x = lines_x,
        .board_cell_w = board_cell_w,
        .board_wall_x = board_wall_x,
        .board_cell_x = board_cell_x,
        .board_wall_r = board_wall_r,
        .board_top_y = board_top_y,
        .board_cell_y = board_cell_y,
        .board_bottom_y = board_bottom_y,
        .panel_x = panel_x,
        .panel_name_x = panel_x + u8_l("hold ").len,
        .panel_piece_x = panel_x + (panel_w - mini_piece_w) / 2,
        .hold_y = hold_y,
        .hold_piece_y = hold_piece_y,
        .next_y = next_y,
        .next_piece_y = next_piece_y,
        .next_piece_stride_y = next_piece_stride_y,
        .help_x = help_x,
        .help_y = board_cell_y,
        .status_x = tetris_Presenter_Layout_centeredX(board_cell_x, board_inner_w, game_over_text),
        .status_y = status_y,
        .game_over_hint_x = tetris_Presenter_Layout_centeredX(board_cell_x, board_inner_w, game_over_hint),
        .game_over_hint_y = status_y + label_h + row_gap,
    };
};

fn_((tetris_Presenter_composeU32(tetris_Frame* frame, usize x, usize y, u32 val))(void)) {
    var_(buf_mem, A$$(10, u8)) $undefined;
    let buf = A_ref$((S$u8)(buf_mem));
    let len = tetris_Presenter_formatU32(buf, val);
    tetris_Frame_putText(frame, x, y, S_prefix((buf)(len)).as_const);
};

fn_((tetris_Presenter_MessageLines_firstY(tetris_Presenter_MessageLines lines, daterm_CellSize screen))(usize)) {
    let rows = as$(usize)(screen.rows);
    return rows < A_len(lines) ? 0 : (rows - A_len(lines)) / 2;
};

fn_((tetris_Presenter_composeBoardBorder(tetris_Frame* frame, tetris_Presenter_Layout layout, usize y))(void)) {
    tetris_Frame_putChar(frame, layout.board_wall_x, y, '+');
    for_(($r(layout.board_wall_x + 1, layout.board_wall_r))(x)) {
        tetris_Frame_putChar(frame, x, y, '-');
    } $end(for);
    tetris_Frame_putChar(frame, layout.board_wall_r, y, '+');
};

fn_((tetris_Presenter_composeHelp(tetris_Frame* frame, tetris_Presenter_Layout layout))(void)) {
    for_(($a(tetris_Presenter_help_lines), $rf(layout.help_y))(line, y)) {
        tetris_Frame_putText(frame, layout.help_x, y, *line);
    } $end(for);
};

fn_((tetris_Presenter_composeMiniPiece(tetris_Frame* frame, O$tetris_Piece piece, usize col, usize row))(void)) {
    if_none(piece) {
        tetris_Frame_putText(frame, col, row, u8_l("(empty)"));
    } else_some((piece_value)) {
        for_(($r(0, 2))(py)) {
            for_(($r(0, 4))(px)) {
                tetris_Frame_putMiniBlock(
                    frame,
                    col + px,
                    row + py,
                    piece_value,
                    tetris_Piece_covers(piece_value, tetris_Rotation_0, px, py * 2),
                    tetris_Piece_covers(piece_value, tetris_Rotation_0, px, py * 2 + 1)
                );
            } $end(for);
        } $end(for);
    };
};

fn_((tetris_Presenter_compose(tetris_Presenter* self, const tetris_Self* game))(void)) {
    let frame = &self->frame;
    let layout = tetris_Presenter_Layout_calc();
    tetris_Frame_clear(frame);
    let ghost_y = tetris_ghostY(game);

    tetris_Frame_putText(frame, layout.title_x, layout.header_y, u8_l("Dasae Tetris"));
    tetris_Frame_putText(frame, layout.score_x, layout.header_y, u8_l("score"));
    tetris_Frame_putText(frame, layout.level_x, layout.header_y, u8_l("level"));
    tetris_Frame_putText(frame, layout.lines_x, layout.header_y, u8_l("lines"));
    tetris_Presenter_composeU32(frame, layout.score_x, layout.header_value_y, game->score);
    tetris_Presenter_composeU32(frame, layout.level_x, layout.header_value_y, game->level);
    tetris_Presenter_composeU32(frame, layout.lines_x, layout.header_value_y, game->lines);

    tetris_Presenter_composeBoardBorder(frame, layout, layout.board_top_y);
    for_(($r(tetris_Board_hidden_rows, tetris_Board_h))(y)) {
        let row = y - tetris_Board_hidden_rows + layout.board_cell_y;
        tetris_Frame_putChar(frame, layout.board_wall_x, row, '|');
        tetris_Frame_putChar(frame, layout.board_wall_r, row, '|');
        for_(($r(0, tetris_Board_w))(x)) {
            let active = tetris_activeCellAt(game, x, y);
            let cell = isSome(active) ? active : *tetris_Board_at(&game->board, x, y);
            if_some((cell)(piece)) {
                tetris_Frame_putBlock(frame, layout.board_cell_x + x * layout.board_cell_w, row, piece);
            } else {
                if (isNone(active) && tetris_ghostCellAt(game, ghost_y, x, y)) {
                    tetris_Frame_putGhost(frame, layout.board_cell_x + x * layout.board_cell_w, row);
                }
            }
        } $end(for);
    } $end(for);
    tetris_Presenter_composeBoardBorder(frame, layout, layout.board_bottom_y);

    tetris_Frame_putText(frame, layout.panel_x, layout.hold_y, u8_l("hold"));
    if_some((game->hold)(held)) {
        tetris_Frame_putText(frame, layout.panel_name_x, layout.hold_y, tetris_Piece_name(held));
    } else_none {
        tetris_Frame_putText(frame, layout.panel_name_x, layout.hold_y, u8_l("-"));
    };
    tetris_Presenter_composeMiniPiece(
        frame,
        game->hold,
        isSome(game->hold) ? layout.panel_piece_x : layout.panel_x,
        layout.hold_piece_y
    );

    tetris_Frame_putText(frame, layout.panel_x, layout.next_y, u8_l("next"));
    for_(($r(0, tetris_PieceQ_preview_len))(i)) {
        tetris_Presenter_composeMiniPiece(
            frame,
            some$((O$tetris_Piece)(tetris_PieceQ_peek(&game->queue, i))),
            layout.panel_piece_x,
            layout.next_piece_y + i * layout.next_piece_stride_y
        );
    } $end(for);

    tetris_Presenter_composeHelp(frame, layout);

    if (game->is_paused) {
        tetris_Frame_putTextFg(frame, layout.status_x, layout.status_y, u8_l("PAUSED"), tetris_RGB_pause);
    }
    if (game->is_game_over) {
        tetris_Frame_putTextFg(frame, layout.status_x, layout.status_y, u8_l("GAME OVER"), tetris_RGB_game_over);
        tetris_Frame_putText(frame, layout.game_over_hint_x, layout.game_over_hint_y, u8_l("press q or esc"));
    }
};

fn_((tetris_Presenter_formatU32(S$u8 buf, u32 val))(usize)) {
    var_(rev, A$$(10, u8)) $undefined;
    var_(len, usize) = 0;
    do {
        *A_at((rev)[len]) = as$(u8)('0' + val % 10);
        val /= 10;
        len += 1;
    } while (val != 0);

    for_(($rt(len))(i)) {
        *S_at((buf)[i]) = *A_at((rev)[len - i - 1]);
    } $end(for);
    return len;
};

fn_((tetris_Presenter_StyleState_init(void))(tetris_Presenter_StyleState)) {
    return (tetris_Presenter_StyleState){
        .fg = none(),
        .bg = none(),
    };
};

fn_((tetris_Presenter_writeGlyph(u32 glyph, io_Writer out))(E$void) $scope) {
    var_(mem, A$$(4, u8)) $undefined;
    let bytes = catch_((utf8_encodeWithin(glyph, A_ref$((S$u8)(mem))))(
        $ignore, claim_unreachable
    ));
    try_(io_Writer_writeBytes(out, bytes.as_const));
    return_ok({});
} $unscoped(fn);

fn_((tetris_Presenter_moveToCell(
    usize x, usize y, io_Writer out
))(E$void)) {
    return dansi_cursor_moveToWrite(as$(u16)(y + 1), as$(u16)(x + 1), out);
};

fn_((tetris_Presenter_homeScreen(io_Writer out))(E$void) $scope) {
    try_(dansi_cursor_moveToWrite(1, 1, out));
    return_ok({});
} $unscoped(fn);

fn_((tetris_Presenter_beginScreen(io_Writer out))(E$void) $scope) {
    try_(tetris_Presenter_homeScreen(out));
    try_(dansi_erase_inDisplayWrite(dansi_erase_Area_all, out));
    return_ok({});
} $unscoped(fn);

fn_((tetris_Presenter_StyleState_applyFg(tetris_Presenter_StyleState* self, io_Writer out, O$tetris_RGB next))(E$void) $scope) {
    if_some((self->fg)(curr_rgb)) {
        if_some((next)(next_rgb)) {
            if (tetris_RGB_eql(curr_rgb, next_rgb)) return_ok({});
        }
    } else_none {
        if_none((next)) return_ok({});
    };

    if_none(next) {
        try_(dansi_color_resetFGWrite(out));
        asg_l((&self->fg)(none()));
        return_ok({});
    } else_some((next_rgb)) {
        try_(dansi_xterm_color_fg24bitWrite(
            (dansi_xterm_color_RGB8){
                .r = next_rgb.r,
                .g = next_rgb.g,
                .b = next_rgb.b,
            },
            out
        ));
        asg_l((&self->fg)(some(next_rgb)));
        return_ok({});
    };
    return_ok({});
} $unscoped(fn);

fn_((tetris_Presenter_StyleState_applyBg(tetris_Presenter_StyleState* self, io_Writer out, O$tetris_RGB next))(E$void) $scope) {
    if_some((self->bg)(curr_rgb)) {
        if_some((next)(next_rgb)) {
            if (tetris_RGB_eql(curr_rgb, next_rgb)) return_ok({});
        }
    } else_none {
        if_none((next)) return_ok({});
    };

    if_none(next) {
        try_(dansi_color_resetBGWrite(out));
        asg_l((&self->bg)(none()));
        return_ok({});
    } else_some(next_rgb) {
        try_(dansi_xterm_color_bg24bitWrite(
            (dansi_xterm_color_RGB8){
                .r = next_rgb.r,
                .g = next_rgb.g,
                .b = next_rgb.b,
            },
            out
        ));
        asg_l((&self->bg)(some(next_rgb)));
        return_ok({});
    };
    return_ok({});
} $unscoped(fn);

fn_((tetris_Presenter_StyleState_applyCell(tetris_Presenter_StyleState* self, io_Writer out, tetris_FrameCell cell))(E$void) $scope) {
    try_(tetris_Presenter_StyleState_applyFg(self, out, cell.fg));
    try_(tetris_Presenter_StyleState_applyBg(self, out, cell.bg));
    return_ok({});
} $unscoped(fn);

fn_((tetris_Presenter_DiffStats_calc(const tetris_Presenter* self))(tetris_Presenter_DiffStats)) {
    var stats = (tetris_Presenter_DiffStats){};
    for_(($rt(tetris_Frame_height))(y)) {
        var_(in_run, bool) = false;
        for_(($rt(tetris_Frame_width))(x)) {
            let idx = y * tetris_Frame_width + x;
            let changed = !tetris_FrameCell_eql(
                *S_at((tetris_Frame_cells(&self->frame))[idx]),
                *S_at((tetris_Frame_cells(&self->front))[idx])
            );
            if (changed) {
                stats.changed_cells += 1;
                if (!in_run) {
                    stats.changed_runs += 1;
                    in_run = true;
                }
            } else {
                in_run = false;
            }
        } $end(for);
    } $end(for);
    return stats;
};

fn_((tetris_Presenter_DiffStats_prefersFull(tetris_Presenter_DiffStats self))(bool)) {
    return self.changed_cells * 3ull >= tetris_Frame_area || self.changed_runs * 2ull >= tetris_Frame_height;
};

fn_((tetris_Presenter_smallScreenBgCell(
    const tetris_Presenter* self,
    usize x,
    usize y,
    tetris_SmallScreen_RenderPolicy policy
))(tetris_FrameCell)) {
    if (policy == tetris_small_screen_render_policy_dim && x < tetris_Frame_width && y < tetris_Frame_height) {
        return tetris_FrameCell_dimmedGray(*S_at((tetris_Frame_cells(&self->frame))[y * tetris_Frame_width + x]));
    }
    return tetris_FrameCell_blank;
};

fn_((tetris_Presenter_smallScreenBorderCell(tetris_FrameCell under, daterm_CellSize screen, usize x, usize y))(tetris_FrameCell)) {
    if (screen.cols == 0 || screen.rows == 0) { return under; }
    if (x != 0 && x + 1 != screen.cols && y != 0 && y + 1 != screen.rows) { return under; }

    var ch = '-';
    if (x == 0 || x + 1 == screen.cols) { ch = '|'; }
    if ((x == 0 || x + 1 == screen.cols) && (y == 0 || y + 1 == screen.rows)) { ch = '+'; }
    return with_((under)(
        (.ch)(as$(u8)(ch)),
        (.fg)(some(tetris_RGB_screen_too_small)),
        (.bg)(none$((O$tetris_RGB))),
    ));
};

fn_((tetris_Presenter_smallScreenTextCell(
    tetris_FrameCell under,
    u16 cols,
    usize x,
    usize y,
    usize text_y,
    S_const$u8 text,
    tetris_RGB fg
))(tetris_FrameCell)) {
    if (y != text_y || cols == 0) { return under; }

    let col_count = as$(usize)(cols);
    let text_x = text.len < col_count ? (col_count - text.len) / 2 : 0;
    if (x < text_x || text_x + text.len <= x) { return under; }
    return with_((under)(
        (.ch)(*S_at((text)[x - text_x])),
        (.fg)(some(fg)),
        (.bg)(none$((O$tetris_RGB))),
    ));
};

fn_((tetris_Presenter_presentSmallScreen(tetris_Presenter* self, daterm_CellSize screen, tetris_SmallScreen_Cfg cfg))(E$void) $scope) {
    let out = io_Buf_writer(&self->out.writer);
    try_(tetris_Presenter_beginScreen(out));

    var_(required_mem, A$$(48, u8)) $undefined;
    var required_writer = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(required_mem))));
    try_(io_Writer_writeBytes(io_Fixed_writer(&required_writer), u8_l("required: ")));
    var_(num_mem, A$$(10, u8)) $undefined;
    var num_buf = A_ref$((S$u8)(num_mem));
    let req_cols_len = tetris_Presenter_formatU32(num_buf, as$(u32)(tetris_Frame_width));
    try_(io_Writer_writeBytes(io_Fixed_writer(&required_writer), S_prefix((num_buf)(req_cols_len)).as_const));
    try_(io_Writer_writeByte(io_Fixed_writer(&required_writer), 'x'));
    let req_rows_len = tetris_Presenter_formatU32(num_buf, as$(u32)(tetris_Frame_height));
    try_(io_Writer_writeBytes(io_Fixed_writer(&required_writer), S_prefix((num_buf)(req_rows_len)).as_const));
    let required = io_Fixed_written(required_writer.stream).as_const;

    var_(current_mem, A$$(48, u8)) $undefined;
    var current_writer = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(current_mem))));
    try_(io_Writer_writeBytes(io_Fixed_writer(&current_writer), u8_l("current: ")));
    let curr_cols_len = tetris_Presenter_formatU32(num_buf, screen.cols);
    try_(io_Writer_writeBytes(io_Fixed_writer(&current_writer), S_prefix((num_buf)(curr_cols_len)).as_const));
    try_(io_Writer_writeByte(io_Fixed_writer(&current_writer), 'x'));
    let curr_rows_len = tetris_Presenter_formatU32(num_buf, screen.rows);
    try_(io_Writer_writeBytes(io_Fixed_writer(&current_writer), S_prefix((num_buf)(curr_rows_len)).as_const));
    let current = io_Fixed_written(current_writer.stream).as_const;

    let status = cfg.sim_policy == tetris_small_screen_sim_policy_pause
                   ? u8_l("game paused until size is restored")
                   : u8_l("game is still running");
    let message_lines = (tetris_Presenter_MessageLines)A_init({
        { u8_l("terminal too small"), tetris_RGB_screen_too_small },
        { required, tetris_RGB_screen_too_small_text },
        { current, tetris_RGB_screen_too_small_text },
        { status, tetris_RGB_screen_too_small_text },
        { u8_l("resize the terminal"), tetris_RGB_screen_too_small_text },
    });
    let first_y = tetris_Presenter_MessageLines_firstY(message_lines, screen);
    var style = tetris_Presenter_StyleState_init();

    for_(($rt(screen.rows))(y)) {
        for_(($rt(screen.cols))(x)) {
            var cell = tetris_Presenter_smallScreenBgCell(self, x, y, cfg.render_policy);
            for_(($a(message_lines), $rf(first_y))(line, text_y)) {
                cell = tetris_Presenter_smallScreenTextCell(cell, screen.cols, x, y, text_y, line->text, line->fg);
            } $end(for);
            cell = tetris_Presenter_smallScreenBorderCell(cell, screen, x, y);
            try_(tetris_Presenter_StyleState_applyCell(&style, out, cell));
            try_(tetris_Presenter_writeGlyph(cell.ch, out));
        } $end(for);
        try_(tetris_Presenter_StyleState_applyFg(&style, out, none$((O$tetris_RGB))));
        try_(tetris_Presenter_StyleState_applyBg(&style, out, none$((O$tetris_RGB))));
        if (y + 1 < as$(usize)(screen.rows)) {
            try_(io_Writer_writeBytes(out, u8_l("\r\n")));
        }
    } $end(for);
    try_(dansi_sgr_resetWrite(out));
    try_(io_Buf_Writer_flush(&self->out.writer));
    self->front_valid = false;
    return_ok({});
} $unscoped(fn);

fn_((tetris_Presenter_presentFull(tetris_Presenter* self, bool clear))(E$void) $scope) {
    let out = io_Buf_writer(&self->out.writer);
    if (clear) {
        try_(tetris_Presenter_beginScreen(out));
    } else {
        try_(tetris_Presenter_homeScreen(out));
    }

    var style = tetris_Presenter_StyleState_init();

    for_(($rt(tetris_Frame_height))(y)) {
        let rows = S_slice((tetris_Frame_cells(&self->frame))$r(
            tetris_Frame_width * y,
            tetris_Frame_width * (y + 1)
        ));
        for_(($s(rows))(cell)) {
            try_(tetris_Presenter_StyleState_applyCell(&style, out, *cell));
            try_(tetris_Presenter_writeGlyph(cell->ch, out));
        } $end(for);
        try_(tetris_Presenter_StyleState_applyFg(&style, out, none$((O$tetris_RGB))));
        try_(tetris_Presenter_StyleState_applyBg(&style, out, none$((O$tetris_RGB))));
        if (y + 1 < tetris_Frame_height) {
            try_(io_Writer_writeBytes(out, u8_l("\r\n")));
        }
    } $end(for);
    try_(dansi_sgr_resetWrite(out));
    try_(io_Buf_Writer_flush(&self->out.writer));
    self->front = self->frame;
    self->front_valid = true;
    return_ok({});
} $unscoped(fn);

fn_((tetris_Presenter_presentDiff(tetris_Presenter* self))(E$void) $scope) {
    let out = io_Buf_writer(&self->out.writer);
    var style = tetris_Presenter_StyleState_init();

    for_(($rt(tetris_Frame_height))(y)) {
        var_(x, usize) = 0;
        while (x < tetris_Frame_width) {
            let idx = y * tetris_Frame_width + x;
            let curr = *S_at((tetris_Frame_cells(&self->frame))[idx]);
            let prev = *S_at((tetris_Frame_cells(&self->front))[idx]);
            if (tetris_FrameCell_eql(curr, prev)) {
                x += 1;
                continue;
            }

            try_(tetris_Presenter_moveToCell(x, y, out));
            while (x < tetris_Frame_width) {
                let run_idx = y * tetris_Frame_width + x;
                let run_curr = *S_at((tetris_Frame_cells(&self->frame))[run_idx]);
                let run_prev = *S_at((tetris_Frame_cells(&self->front))[run_idx]);
                if (tetris_FrameCell_eql(run_curr, run_prev)) break;

                try_(tetris_Presenter_StyleState_applyCell(&style, out, run_curr));
                try_(tetris_Presenter_writeGlyph(run_curr.ch, out));
                x += 1;
            }
        }
    } $end(for);

    try_(dansi_sgr_resetWrite(out));
    try_(io_Buf_Writer_flush(&self->out.writer));
    self->front = self->frame;
    self->front_valid = true;
    return_ok({});
} $unscoped(fn);

fn_((tetris_Presenter_present(tetris_Presenter* self))(E$void) $scope) {
    if (!self->front_valid) return tetris_Presenter_presentFull(self, true);

    let stats = tetris_Presenter_DiffStats_calc(self);
    if (stats.changed_cells == 0) return_ok({});
    return tetris_Presenter_DiffStats_prefersFull(stats)
             ? tetris_Presenter_presentFull(self, false)
             : tetris_Presenter_presentDiff(self);
} $unscoped(fn);

fn_((tetris_Presenter_render(
    tetris_Presenter* self,
    const tetris_Self* game,
    daterm_CellSize screen,
    tetris_SmallScreen_Cfg cfg
))(E$void)) {
    tetris_Presenter_compose(self, game);
    if (screen.cols < tetris_Frame_width || screen.rows < tetris_Frame_height) {
        return tetris_Presenter_presentSmallScreen(self, screen, cfg);
    }
    return tetris_Presenter_present(self);
};

TEST_fn_("example-tetris: gravity remains saturated at high levels" $scope) {
    try_(TEST_expect(time_Dur_eq(
        tetris_gravityForLevel(19), time_Dur_fromMillis(50)
    )));
    try_(TEST_expect(time_Dur_eq(
        tetris_gravityForLevel(u32_limit_max), time_Dur_fromMillis(50)
    )));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-tetris: gravity tick advances an active piece" $scope) {
    var_(game, tetris_Self) = cleared();
    tetris_init(&game);
    let y = game.y;
    game.gravity_interval = time_Dur_zero;

    tetris_tick(&game, time_Dur_fromMillis(1));

    try_(TEST_expect(game.y == y + 1));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-tetris: SRS upward kick uses board coordinates" $scope) {
    var_(game, tetris_Self) = cleared();
    tetris_Board_clear(&game.board);
    game.current = tetris_Piece_t;
    game.rotation = tetris_Rotation_0;
    game.x = 3;
    game.y = 2;
    *tetris_Board_atMut(&game.board, 4, 4) = some$((O$tetris_Piece)(tetris_Piece_i));
    *tetris_Board_atMut(&game.board, 3, 4) = some$((O$tetris_Piece)(tetris_Piece_i));

    try_(TEST_expect(tetris_tryRotate(&game, 1)));
    try_(TEST_expect(game.rotation == tetris_Rotation_r));
    try_(TEST_expect(game.x == 2));
    try_(TEST_expect(game.y == 1));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-tetris: repeats only drive continuous movement" $scope) {
    let repeat = some$((O$daterm_key_Action)(daterm_key_Action_repeat));
    let left = try_(tetris_cmdFromKey((daterm_key_Event){
        .code = daterm_key_Code_left,
        .mods = daterm_input_modsNone(),
        .action = repeat,
    }));
    try_(TEST_expect(isSome(left) && unwrap_(left) == tetris_cmd_move_left));

    let rotate = try_(tetris_cmdFromKey((daterm_key_Event){
        .code = daterm_key_Code_up,
        .mods = daterm_input_modsNone(),
        .action = repeat,
    }));
    try_(TEST_expect(isNone(rotate)));

    let hard_drop = try_(tetris_cmdFromText((daterm_key_Text){
        .codepoint = ' ',
        .mods = daterm_input_modsNone(),
        .action = repeat,
    }));
    try_(TEST_expect(isNone(hard_drop)));

    var_(game, tetris_Self) = cleared();
    tetris_init(&game);
    tetris_applyCmd(&game, tetris_cmd_toggle_pause);
    try_(TEST_expect(game.is_paused));
    tetris_applyCmd(&game, tetris_cmd_toggle_pause);
    try_(TEST_expect(!game.is_paused));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-tetris: presenter renders a complete frame" $scope) {
    $static var_(game, tetris_Self) $undefined_static;
    $static var_(presenter, tetris_Presenter) $undefined_static;
    $static var_(out_mem, A$$(65536, u8)) $undefined_static;
    var out = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(out_mem))));
    tetris_init(&game);
    tetris_Presenter_init(&presenter, io_Fixed_writer(&out));

    try_(tetris_Presenter_render(
        &presenter,
        &game,
        (daterm_CellSize){
            .cols = as$(u16)(tetris_Frame_width),
            .rows = as$(u16)(tetris_Frame_height),
        },
        tetris_SmallScreen_Cfg_default
    ));

    try_(TEST_expect(io_Fixed_written(out.stream).len != 0));
    try_(TEST_expect(!tetris_SmallScreen_Cfg_shouldTick(
        tetris_SmallScreen_Cfg_default, true
    )));
    return_ok({});
} $unscoped(TEST_fn);
