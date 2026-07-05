#include <dh/prl.h>

/*
 * dasae pacman
 *
 * dh/daterm port of the terminal Pac-Man example.
 * The game state, terminal presentation, and runtime scheduling are kept in
 * separate sections to make the port easier to inspect and test.
 *
 * original source: <https://github.com/coding-pelican/pacman-c>
 */

/*========== Common =========================================================*/

#include <dh/math/vec.h>
#include <dh/ArrList.h>
#include <dh/ArrQue.h>

typedef m_V2i32 pac_V2i;
T_use_prl$(pac_V2i);
#define pac_V2i_zero m_V2i32_zero
#define pac_V2i_unit_x m_V2i32_unit_x
#define pac_V2i_unit_y m_V2i32_unit_y
#define pac_V2i_one m_V2i32_one
#define pac_V2i_left m_V2i32_left
#define pac_V2i_up pac_V2i_of_static(0, -1)
#define pac_V2i_right m_V2i32_right
#define pac_V2i_down pac_V2i_of_static(0, 1)
#define pac_V2i_limit_min m_V2i32_limit_min
#define pac_V2i_limit_max m_V2i32_limit_max
#define pac_V2i_of_static m_V2i32_of_static
$static let pac_V2i_of = m_V2i32_of;
$attr($maybe_unused)
$static let pac_V2i_splat = m_V2i32_splat;
$attr($maybe_unused)
$static let cmp_ord$(pac_V2i) = cmp_ord$(m_V2i32);
$static let cmp_eq$(pac_V2i) = cmp_eq$(m_V2i32);
$attr($maybe_unused)
$static let cmp_ne$(pac_V2i) = cmp_ne$(m_V2i32);
$attr($maybe_unused)
$static let cmp_lt$(pac_V2i) = cmp_lt$(m_V2i32);
$attr($maybe_unused)
$static let cmp_gt$(pac_V2i) = cmp_gt$(m_V2i32);
$attr($maybe_unused)
$static let cmp_le$(pac_V2i) = cmp_le$(m_V2i32);
$attr($maybe_unused)
$static let cmp_ge$(pac_V2i) = cmp_ge$(m_V2i32);
$attr($maybe_unused)
$static let pac_V2i_neg = m_V2i32_neg;
$static let pac_V2i_add = m_V2i32_add;
$attr($maybe_unused)
$static let pac_V2i_sub = m_V2i32_sub;
$attr($maybe_unused)
$static let pac_V2i_mul = m_V2i32_mul;
$attr($maybe_unused)
$static let pac_V2i_div = m_V2i32_div;
$attr($maybe_unused)
$static let pac_V2i_rem = m_V2i32_rem;
$attr($maybe_unused)
$static let pac_V2i_scal = m_V2i32_scal;
$attr($maybe_unused)
$static let pac_V2i_scalInv = m_V2i32_scalInv;
$attr($maybe_unused)
$static let pac_V2i_addS = m_V2i32_addS;
$attr($maybe_unused)
$static let pac_V2i_subS = m_V2i32_subS;
$attr($maybe_unused)
$static let pac_V2i_mulS = m_V2i32_mulS;
$attr($maybe_unused)
$static let pac_V2i_divS = m_V2i32_divS;
$attr($maybe_unused)
$static let pac_V2i_remS = m_V2i32_remS;
$attr($maybe_unused)
$static let pac_V2i_min = m_V2i32_min;
$attr($maybe_unused)
$static let pac_V2i_max = m_V2i32_max;
$static let pac_V2i_clamp = m_V2i32_clamp;
$static let pac_V2i_wrap = m_V2i32_wrap;
$static let pac_V2i_distSq = m_V2i32_distSq;
$attr($maybe_unused)
$static let pac_V2i_dot = m_V2i32_dot;
$attr($maybe_unused)
$static let pac_V2i_cross = m_V2i32_cross;
$attr($maybe_unused)
$static let pac_V2i_det = m_V2i32_det;
$attr($maybe_unused)
$static let pac_V2i_orient = m_V2i32_orient;
$attr($maybe_unused)
$static let pac_V2i_orientDir = m_V2i32_orientDir;
$attr($maybe_unused)
$static let pac_V2i_isCW = m_V2i32_isCW;
$attr($maybe_unused)
$static let pac_V2i_isCollinear = m_V2i32_isCollinear;
$attr($maybe_unused)
$static let pac_V2i_isCCW = m_V2i32_isCCW;
$attr($maybe_unused)
$static let pac_V2i_cw = m_V2i32_cw;
$attr($maybe_unused)
$static let pac_V2i_ccw = m_V2i32_ccw;
$attr($maybe_unused)
$static let pac_V2i_skew = m_V2i32_skew;
$attr($maybe_unused)
$static let pac_V2i_perp = m_V2i32_perp;

/*========== Direction ======================================================*/

typedef enum_((pac_Dir $fits($packed))(
    pac_Dir_up = 0,
    pac_Dir_left,
    pac_Dir_right,
    pac_Dir_down,
    count$pac_Dir
)) pac_Dir;
T_use_prl$(pac_Dir);
claim_assert_static(eqlType$(pac_Dir, u8));
$static fn_((pac_Dir_delta(pac_Dir dir))(pac_V2i)) {
    switch (dir) {
    case_((pac_Dir_up)) return pac_V2i_up $end(case);
    case_((pac_Dir_down)) return pac_V2i_down $end(case);
    case_((pac_Dir_left)) return pac_V2i_left $end(case);
    case_((pac_Dir_right)) return pac_V2i_right $end(case);
    case_((count$pac_Dir)) claim_unreachable $end(case);
    }
};
$static fn_((pac_Dir_reverse(pac_Dir dir))(pac_Dir)) {
    switch (dir) {
    case_((pac_Dir_up)) return pac_Dir_down $end(case);
    case_((pac_Dir_left)) return pac_Dir_right $end(case);
    case_((pac_Dir_right)) return pac_Dir_left $end(case);
    case_((pac_Dir_down)) return pac_Dir_up $end(case);
    case_((count$pac_Dir)) claim_unreachable $end(case);
    }
};

/*========== Board ==========================================================*/

#define pac_Board_w 28
#define pac_Board_h 24
#define pac_Board_size (usize_(pac_Board_w) * usize_(pac_Board_h))

typedef enum_((pac_Tile $fits($packed))(
    pac_Tile_empty = 0,
    pac_Tile_wall,
    pac_Tile_pellet,
    pac_Tile_door,
    pac_Tile_power,
)) pac_Tile;
T_use_prl$(pac_Tile);
claim_assert_static(eqlType$(pac_Tile, u8));

typedef A$$(pac_Board_size, pac_Tile) pac_Board_Cells;
typedef struct pac_Board {
    var_(cells, pac_Board_Cells);
} pac_Board;
$static let_(pac_Board_initial, pac_Board_Cells) = A_init({
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, $listed
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, $listed
    1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, $listed
    1, 4, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 4, 1, $listed
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, $listed
    1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, $listed
    1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1, $listed
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, $listed
    0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, $listed
    0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 1, 1, 1, 3, 3, 1, 1, 1, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, $listed
    1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, $listed
    0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, $listed
    1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, $listed
    0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, $listed
    0, 0, 0, 0, 0, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 0, 0, 0, 0, 0, $listed
    1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, $listed
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, $listed
    1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, $listed
    1, 4, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 4, 1, $listed
    1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, $listed
    1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1, $listed
    1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, $listed
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, $listed
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 $listed
});
$static fn_((pac_Board_reset(pac_Board* self))(void)) { self->cells = pac_Board_initial; };
$static fn_((pac_Board_index(usize x, usize y))(usize)) {
    claim_assert(x < pac_Board_w), claim_assert(y < pac_Board_h);
    return (y * pac_Board_w) + x;
};
$static fn_((pac_Board_at(const pac_Board* self, usize x, usize y))(pac_Tile)) {
    return *A_at((self->cells)[pac_Board_index(x, y)]);
};
$static fn_((pac_Board_atMut(pac_Board* self, usize x, usize y))(pac_Tile*)) {
    return A_at((self->cells)[pac_Board_index(x, y)]);
};
$static fn_((pac_Board_countPellets(const pac_Board* self))(usize)) {
    var_(count, usize) = 0;
    for_(($a(self->cells))(tile)) {
        if (*tile == pac_Tile_pellet || *tile == pac_Tile_power) count += 1;
    } $end(for);
    return count;
};
$static fn_((pac_Board_wrapPos(pac_V2i pos))(pac_V2i)) {
    return pac_V2i_wrap(
        pos,
        pac_V2i_zero,
        pac_V2i_of(i32_(pac_Board_w), i32_(pac_Board_h))
    );
};
$static fn_((pac_Board_containsPos(pac_V2i pos))(bool)) {
    return pos.x >= 0
        && pos.y >= 0
        && pos.x < i32_(pac_Board_w)
        && pos.y < i32_(pac_Board_h);
};
$static fn_((pac_Board_clampPos(pac_V2i pos))(pac_V2i)) {
    return pac_V2i_clamp(
        pos,
        pac_V2i_zero,
        pac_V2i_of(i32_(pac_Board_w - 1), i32_(pac_Board_h - 1))
    );
};

/*========== Game State =====================================================*/

#include <dh/Rand.h>
#include <dh/time/Dur.h>

typedef enum_((pac_Ghost_Type $fits($packed))(
    pac_Ghost_blinky = 0,
    pac_Ghost_inky,
    pac_Ghost_pinky,
    pac_Ghost_clyde,
    count$pac_Ghost_Type
)) pac_Ghost_Type;
T_use_prl$(pac_Ghost_Type);
claim_assert_static(eqlType$(pac_Ghost_Type, u8));

typedef enum_((pac_Ghost_Mode $fits($packed))(
    pac_Ghost_staying = 0,
    pac_Ghost_scattering,
    pac_Ghost_chasing,
    pac_Ghost_frightened,
    pac_Ghost_eaten,
)) pac_Ghost_Mode;
T_use_prl$(pac_Ghost_Mode);
claim_assert_static(eqlType$(pac_Ghost_Mode, u8));

typedef struct pac_Player {
    var_(pos, pac_V2i);
    var_(dir, pac_Dir);
    var_(next_dir, O$pac_Dir);
    var_(anim_frame, u8);
    var_(lives, u8);
    var_(step_elapsed, time_Dur);
} pac_Player;

typedef struct pac_Ghost {
    var_(type, pac_Ghost_Type);
    var_(mode, pac_Ghost_Mode);
    var_(pos, pac_V2i);
    var_(target, pac_V2i);
    var_(dir, pac_Dir);
    var_(anim_frame, u8);
    var_(mode_left, time_Dur);
    var_(frightened_left, time_Dur);
    var_(step_elapsed, time_Dur);
    var_(in_home, bool);
} pac_Ghost;

typedef struct pac_Game {
    var_(board, pac_Board);
    var_(player, pac_Player);
    var_(ghosts, A$$(count$pac_Ghost_Type, pac_Ghost));
    var_(rng, Rand);
    var_(score, u32);
    var_(high_score, u32);
    var_(remaining, usize);
    var_(frightened_epoch, u32);
    var_(flash_left, time_Dur);
    var_(flash_phase_elapsed, time_Dur);
    var_(flash_wall_white, bool);
    var_(reset_round_after_flash, bool);
    var_(is_running, bool);
    var_(is_paused, bool);
    var_(is_game_over, bool);
    var_(is_win, bool);
} pac_Game;

$static let_(pac_Player_init_pos, pac_V2i) = pac_V2i_of_static(13, 18);
$static let_(pac_Ghost_inside, pac_V2i) = pac_V2i_of_static(13, 11);
$static let_(pac_Ghost_outside, pac_V2i) = pac_V2i_of_static(13, 8);
$static let_(pac_Ghost_init_pos, A$$(count$pac_Ghost_Type, pac_V2i)) = A_init({
    [pac_Ghost_blinky] = pac_V2i_of_static(13, 8),
    [pac_Ghost_inky] = pac_V2i_of_static(11, 11),
    [pac_Ghost_pinky] = pac_V2i_of_static(13, 11),
    [pac_Ghost_clyde] = pac_V2i_of_static(15, 11),
});
$static let_(pac_Ghost_init_dir, A$$(count$pac_Ghost_Type, pac_Dir)) = A_init({
    [pac_Ghost_blinky] = pac_Dir_left,
    [pac_Ghost_inky] = pac_Dir_up,
    [pac_Ghost_pinky] = pac_Dir_down,
    [pac_Ghost_clyde] = pac_Dir_up,
});
$static let_(pac_Ghost_stay_ms, A$$(count$pac_Ghost_Type, u32)) = A_init({
    [pac_Ghost_blinky] = 0,
    [pac_Ghost_inky] = 2000,
    [pac_Ghost_pinky] = 8000,
    [pac_Ghost_clyde] = 16000,
});

$static fn_((pac_Ghost_stepDur(const pac_Ghost* self))(time_Dur)) {
    if (self->mode != pac_Ghost_eaten && !time_Dur_isZero(self->frightened_left)) {
        return time_Dur_fromMillis(500);
    }
    switch (self->mode) {
    case_((pac_Ghost_eaten)) return time_Dur_fromMillis(100) $end(case);
    case pac_Ghost_frightened: $fallthrough;
    case pac_Ghost_staying: $fallthrough;
    case pac_Ghost_scattering: $fallthrough;
    case_((pac_Ghost_chasing)) return time_Dur_fromMillis(250) $end(case);
    }
};

$static fn_((pac_Ghost_isFrightened(const pac_Ghost* self))(bool)) {
    return self->mode != pac_Ghost_eaten && !time_Dur_isZero(self->frightened_left);
};

$static fn_((pac_Game_playerCanMove(const pac_Game* self, pac_V2i pos))(bool)) {
    pos = pac_Board_wrapPos(pos);
    let tile = pac_Board_at(&self->board, as$(usize)(pos.x), as$(usize)(pos.y));
    return tile != pac_Tile_wall && tile != pac_Tile_door;
};

$static fn_((pac_Game_ghostCanMove(const pac_Game* self, const pac_Ghost* ghost, pac_V2i pos))(bool)) {
    if (!pac_Board_containsPos(pos)) return false;
    let tile = pac_Board_at(&self->board, as$(usize)(pos.x), as$(usize)(pos.y));
    if (tile == pac_Tile_wall) {
        return false;
    }
    if (tile == pac_Tile_door) {
        return ghost->mode == pac_Ghost_eaten
            || (ghost->mode == pac_Ghost_staying && !ghost->in_home);
    }
    return true;
};

$static fn_((pac_Ghost_reset(pac_Ghost* self, pac_Ghost_Type type))(void)) {
    asg_l((self)({
        .type = type,
        .mode = pac_Ghost_staying,
        .pos = *A_at((pac_Ghost_init_pos)[type]),
        .target = type == pac_Ghost_blinky ? pac_Ghost_outside : pac_Ghost_inside,
        .dir = *A_at((pac_Ghost_init_dir)[type]),
        .anim_frame = 0,
        .mode_left = time_Dur_fromMillis(*A_at((pac_Ghost_stay_ms)[type])),
        .frightened_left = time_Dur_zero,
        .step_elapsed = time_Dur_zero,
        .in_home = type != pac_Ghost_blinky,
    }));
};

$static fn_((pac_Game_reset(pac_Game* self))(pac_Game*)) {
    pac_Board_reset(&self->board);
    asg_l((&self->player)({
        .pos = pac_Player_init_pos,
        .dir = pac_Dir_left,
        .next_dir = none(),
        .anim_frame = 0,
        .lives = 3,
        .step_elapsed = time_Dur_zero,
    }));
    for_(($rf(0), $s(A_ref(self->ghosts)))(i, ghost)) {
        pac_Ghost_reset(ghost, as$(pac_Ghost_Type)(i));
    } $end(for);
    self->rng = Rand_init();
    self->score = 0;
    self->high_score = 0;
    self->remaining = pac_Board_countPellets(&self->board);
    self->frightened_epoch = 0;
    self->flash_left = time_Dur_zero;
    self->flash_phase_elapsed = time_Dur_zero;
    self->flash_wall_white = false;
    self->reset_round_after_flash = false;
    self->is_running = true;
    self->is_paused = false;
    self->is_game_over = false;
    self->is_win = false;
    return self;
};

$static fn_((pac_Game_startFlash(pac_Game* self, u32 iter_count, u32 millis_per_iter))(void)) {
    self->flash_left = time_Dur_fromMillis(u64_(iter_count) * u64_(millis_per_iter));
    self->flash_phase_elapsed = time_Dur_zero;
    self->flash_wall_white = true;
};

$static fn_((pac_Game_resetRound(pac_Game* self))(void)) {
    self->player.pos = pac_Player_init_pos;
    self->player.dir = pac_Dir_left;
    self->player.anim_frame = 0;
    self->player.step_elapsed = time_Dur_zero;
    asg_l((&self->player.next_dir)(none()));
    for_(($rf(0), $s(A_ref(self->ghosts)))(i, ghost)) {
        pac_Ghost_reset(ghost, as$(pac_Ghost_Type)(i));
    } $end(for);
};

$static fn_((pac_Game_tickFlash(pac_Game* self, time_Dur dt))(bool)) {
    if (time_Dur_isZero(self->flash_left)) return false;
    self->flash_left = time_Dur_subSat(self->flash_left, dt);
    self->flash_phase_elapsed = time_Dur_addSat(self->flash_phase_elapsed, dt);
    let phase = time_Dur_fromMillis(250);
    while (time_Dur_ge(self->flash_phase_elapsed, phase)) {
        self->flash_phase_elapsed = time_Dur_subSat(self->flash_phase_elapsed, phase);
        self->flash_wall_white = !self->flash_wall_white;
    }
    if (time_Dur_isZero(self->flash_left)) {
        self->flash_wall_white = false;
        if (self->reset_round_after_flash) {
            self->reset_round_after_flash = false;
            pac_Game_resetRound(self);
        }
        if (self->is_game_over || self->is_win) self->is_running = false;
    }
    return true;
};

$static fn_((pac_Game_setAllFrightened(pac_Game* self))(void)) {
    self->frightened_epoch += 1;
    for_(($s(A_ref(self->ghosts)))(ghost)) {
        if (ghost->mode == pac_Ghost_eaten) continue;
        ghost->frightened_left = time_Dur_fromSecs(8);
        ghost->dir = pac_Dir_reverse(ghost->dir);
        ghost->step_elapsed = time_Dur_zero;
    } $end(for);
};

$static fn_((pac_Game_eatAtPlayer(pac_Game* self))(void)) {
    let pos = self->player.pos;
    let tile = pac_Board_at(&self->board, as$(usize)(pos.x), as$(usize)(pos.y));
    if (tile == pac_Tile_pellet) {
        *pac_Board_atMut(&self->board, as$(usize)(pos.x), as$(usize)(pos.y)) = pac_Tile_empty;
        self->score += 10;
        self->high_score = pri_max(self->high_score, self->score);
        self->remaining -= 1;
    } else if (tile == pac_Tile_power) {
        *pac_Board_atMut(&self->board, as$(usize)(pos.x), as$(usize)(pos.y)) = pac_Tile_empty;
        self->score += 50;
        self->high_score = pri_max(self->high_score, self->score);
        self->remaining -= 1;
        pac_Game_setAllFrightened(self);
    }
    if (self->remaining == 0) {
        pac_Game_startFlash(self, 3, 500);
        self->is_win = true;
    }
};

$static fn_((pac_Game_collide(pac_Game* self))(void)) {
    if (!time_Dur_isZero(self->flash_left)
        || self->reset_round_after_flash
        || self->is_game_over
        || self->is_win) {
        return;
    }
    for_(($s(A_ref(self->ghosts)))(ghost)) {
        if (!pac_V2i_eq(self->player.pos, ghost->pos)) continue;
        if (pac_Ghost_isFrightened(ghost)) {
            pac_Game_startFlash(self, 1, 500);
            ghost->mode = pac_Ghost_eaten;
            ghost->mode_left = time_Dur_fromSecs(8);
            ghost->frightened_left = time_Dur_zero;
            ghost->target = pac_Ghost_inside;
            ghost->step_elapsed = time_Dur_zero;
            self->score += 200;
            self->high_score = pri_max(self->high_score, self->score);
            continue;
        }
        if (ghost->mode == pac_Ghost_eaten) continue;
        return local_({
            pac_Game_startFlash(self, 3, 500);
            if (self->player.lives > 0) self->player.lives -= 1;
            if (self->player.lives == 0) {
                self->is_game_over = true;
            } else {
                self->reset_round_after_flash = true;
            };
        });
    } $end(for);
};

$static fn_((pac_Game_playerStep(pac_Game* self))(void)) {
    if_some((self->player.next_dir)(dir)) {
        let turn_pos = pac_Board_wrapPos(pac_V2i_add(self->player.pos, pac_Dir_delta(dir)));
        if (pac_Game_playerCanMove(self, turn_pos)) {
            self->player.dir = dir;
            asg_l((&self->player.next_dir)(none()));
        }
    };
    pac_Game_eatAtPlayer(self);
    if (!self->is_running) return;
    let next = pac_Board_wrapPos(pac_V2i_add(self->player.pos, pac_Dir_delta(self->player.dir)));
    if (pac_Game_playerCanMove(self, next)) self->player.pos = next;
    pac_Game_collide(self);
};

$static fn_((pac_Ghost_scatterTarget(pac_Ghost_Type type))(pac_V2i)) {
    switch (type) {
    case_((pac_Ghost_blinky)) return pac_V2i_of(25, 0) $end(case);
    case_((pac_Ghost_inky)) return pac_V2i_of(27, 23) $end(case);
    case_((pac_Ghost_pinky)) return pac_V2i_of(2, 0) $end(case);
    case_((pac_Ghost_clyde)) return pac_V2i_of(0, 23) $end(case);
    case_((count$pac_Ghost_Type)) claim_unreachable $end(case);
    }
};

$static fn_((pac_Ghost_chaseTarget(const pac_Game* self, pac_Ghost_Type type))(pac_V2i)) {
    let_ignore = type;
    return self->player.pos;
};

$static fn_((pac_Ghost_eatenPathCanEnter(const pac_Game* self, pac_V2i pos))(bool)) {
    pos = pac_Board_clampPos(pos);
    return pac_Board_at(&self->board, as$(usize)(pos.x), as$(usize)(pos.y)) != pac_Tile_wall;
};

T_use_ArrQue$(pac_V2i);
T_use$((pac_V2i)(
    ArrQue_fixed,
    ArrQue_isEmpty,
    ArrQue_enqueWithin,
    ArrQue_deque
));
$static fn_((pac_Ghost_chooseEatenDir(const pac_Game* self, const pac_Ghost* ghost))(pac_Dir)) {
    $static var_(dist, A$$(pac_Board_size, u16)) $undefined_static;
    $static var_(queue_mem, A$$(pac_Board_size, pac_V2i)) $undefined_static;
    var queue = ArrQue_fixed$pac_V2i(A_ref$((S$pac_V2i)(queue_mem)));
    for_(($s(A_ref(dist)))(d)) *d = u16_limit_max $end(for);

    let target = pac_Board_clampPos(ghost->target);
    let target_idx = pac_Board_index(as$(usize)(target.x), as$(usize)(target.y));
    *A_at((dist)[target_idx]) = 0;
    ArrQue_enqueWithin$pac_V2i(&queue, target);

    while (!ArrQue_isEmpty$pac_V2i(queue)) {
        let pos = unwrap_(ArrQue_deque$pac_V2i(&queue));
        let pos_idx = pac_Board_index(as$(usize)(pos.x), as$(usize)(pos.y));
        let next_dist = u16_(*A_at((dist)[pos_idx]) + 1);
        for_(($r(0, count$pac_Dir))(i)) {
            let dir = as$(pac_Dir)(i);
            let next = pac_Board_clampPos(pac_V2i_add(pos, pac_Dir_delta(dir)));
            if (pac_V2i_eq(next, pos)) continue;
            if (!pac_Ghost_eatenPathCanEnter(self, next)) continue;
            let next_idx = pac_Board_index(as$(usize)(next.x), as$(usize)(next.y));
            if (*A_at((dist)[next_idx]) != u16_limit_max) continue;
            *A_at((dist)[next_idx]) = next_dist;
            ArrQue_enqueWithin$pac_V2i(&queue, next);
        } $end(for);
    }

    let start = pac_Board_clampPos(ghost->pos);
    var_(best, pac_Dir) = ghost->dir;
    var_(best_dist, u16) = u16_limit_max;
    for_(($r(0, count$pac_Dir))(i)) {
        let dir = as$(pac_Dir)(i);
        let next = pac_Board_clampPos(pac_V2i_add(start, pac_Dir_delta(dir)));
        if (pac_V2i_eq(next, start)) continue;
        if (!pac_Ghost_eatenPathCanEnter(self, next)) continue;
        let next_idx = pac_Board_index(as$(usize)(next.x), as$(usize)(next.y));
        let d = *A_at((dist)[next_idx]);
        if (d < best_dist) {
            best_dist = d;
            best = dir;
        }
    } $end(for);
    return best;
};

T_use_ArrList$(pac_Dir);
T_use$((pac_Dir)(
    ArrList_fixed,
    ArrList_len,
    ArrList_at,
    ArrList_appendWithin
));
$static fn_((pac_Ghost_chooseDir(pac_Game* self, const pac_Ghost* ghost))(pac_Dir)) {
    if (ghost->mode == pac_Ghost_eaten) return pac_Ghost_chooseEatenDir(self, ghost);
    if (ghost->mode == pac_Ghost_scattering) {
        var_(dir_mem, A$$(count$pac_Dir, pac_Dir)) $undefined;
        var dirs = ArrList_fixed$pac_Dir(A_ref$((S$pac_Dir)(dir_mem)));
        for_(($r(0, count$pac_Dir))(i)) {
            let dir = as$(pac_Dir)(i);
            if (dir == pac_Dir_reverse(ghost->dir)) continue;
            let next = pac_V2i_add(ghost->pos, pac_Dir_delta(dir));
            if (pac_Game_ghostCanMove(self, ghost, next)) {
                ArrList_appendWithin$pac_Dir(&dirs, dir);
            }
        } $end(for);
        if (ArrList_len$pac_Dir(dirs) == 0) {
            let dir = pac_Dir_reverse(ghost->dir);
            let next = pac_V2i_add(ghost->pos, pac_Dir_delta(dir));
            if (pac_Game_ghostCanMove(self, ghost, next)) {
                ArrList_appendWithin$pac_Dir(&dirs, dir);
            }
        }
        let len = ArrList_len$pac_Dir(dirs);
        if (len > 0) {
            let idx = len > 1 ? Rand_lessThan$usize(&self->rng, len) : usize_(0);
            return *ArrList_at$pac_Dir(dirs, idx);
        }
    }
    var_(best, pac_Dir) = ghost->dir;
    var_(best_score, i32) = 0x7fffffff;
    var_(found, bool) = false;
    for_(($r(0, count$pac_Dir))(i)) {
        let dir = as$(pac_Dir)(i);
        if (dir == pac_Dir_reverse(ghost->dir)) continue;
        let next = pac_V2i_add(ghost->pos, pac_Dir_delta(dir));
        if (!pac_Game_ghostCanMove(self, ghost, next)) continue;
        var_(score, i32) = pac_V2i_distSq(next, ghost->target);
        if (pac_Ghost_isFrightened(ghost)) score = -score;
        if (!found || score < best_score) {
            best_score = score;
            best = dir;
            found = true;
        }
    } $end(for);
    if (!found) {
        let dir = pac_Dir_reverse(ghost->dir);
        let next = pac_V2i_add(ghost->pos, pac_Dir_delta(dir));
        if (pac_Game_ghostCanMove(self, ghost, next)) return dir;
    }
    return best;
};

$static fn_((pac_Ghost_recoverFromEaten(pac_Ghost* self))(void)) {
    self->mode = pac_Ghost_staying;
    self->pos = *A_at((pac_Ghost_init_pos)[self->type == pac_Ghost_blinky ? pac_Ghost_pinky : self->type]);
    self->target = pac_Ghost_inside;
    self->anim_frame = 0;
    self->mode_left = time_Dur_fromSecs(8);
    self->frightened_left = time_Dur_zero;
    self->step_elapsed = time_Dur_zero;
    self->in_home = true;
};

$static fn_((pac_Ghost_updateMode(pac_Ghost* self))(void)) {
    switch (self->mode) {
    case_((pac_Ghost_staying)) {
    } $end(case);
    case_((pac_Ghost_scattering)) {
        if (time_Dur_isZero(self->mode_left)) {
            self->mode = pac_Ghost_chasing;
            self->mode_left = time_Dur_fromSecs(20);
        }
    } $end(case);
    case_((pac_Ghost_chasing)) {
        if (time_Dur_isZero(self->mode_left)) {
            self->mode = pac_Ghost_scattering;
            self->mode_left = time_Dur_fromSecs(7);
        }
    } $end(case);
    case_((pac_Ghost_frightened)) {
        if (time_Dur_isZero(self->mode_left)) {
            self->mode = pac_Ghost_scattering;
            self->mode_left = time_Dur_fromSecs(7);
        }
    } $end(case);
    case_((pac_Ghost_eaten)) {
        if (time_Dur_isZero(self->mode_left) || pac_V2i_eq(self->pos, self->target)) {
            pac_Ghost_recoverFromEaten(self);
        }
    } $end(case);
    }
};

$static fn_((pac_Ghost_updateTarget(pac_Game* self, pac_Ghost* ghost))(void)) {
    if (pac_Ghost_isFrightened(ghost)) {
        ghost->target = self->player.pos;
        return;
    }
    switch (ghost->mode) {
    case_((pac_Ghost_staying)) {
    } $end(case);
    case_((pac_Ghost_scattering)) {
        ghost->target = pac_Ghost_scatterTarget(ghost->type);
    } $end(case);
    case_((pac_Ghost_chasing)) {
        ghost->target = pac_Ghost_chaseTarget(self, ghost->type);
    } $end(case);
    case_((pac_Ghost_frightened)) {
        ghost->target = self->player.pos;
    } $end(case);
    case_((pac_Ghost_eaten)) {
        ghost->target = pac_Ghost_inside;
    } $end(case);
    }
};

$static fn_((pac_Game_ghostAdvance(pac_Game* self, pac_Ghost* ghost))(void)) {
    let next = pac_V2i_add(ghost->pos, pac_Dir_delta(ghost->dir));
    if (pac_Game_ghostCanMove(self, ghost, next)) ghost->pos = next;
    if (pac_V2i_eq(ghost->pos, pac_Ghost_inside)) ghost->in_home = true;
    if (pac_V2i_eq(ghost->pos, pac_Ghost_outside)) ghost->in_home = false;
    pac_Game_collide(self);
};

$static fn_((pac_Game_ghostSteer(pac_Game* self, pac_Ghost* ghost))(void)) {
    if (ghost->mode == pac_Ghost_staying) {
        if (!time_Dur_isZero(ghost->mode_left)) {
            let next = pac_V2i_add(ghost->pos, pac_Dir_delta(ghost->dir));
            if (!pac_Game_ghostCanMove(self, ghost, next)) ghost->dir = pac_Dir_reverse(ghost->dir);
            return;
        }
        if (ghost->in_home) {
            if (ghost->pos.y != ghost->target.y) {
                let next = pac_V2i_add(ghost->pos, pac_Dir_delta(ghost->dir));
                if (!pac_Game_ghostCanMove(self, ghost, next)) ghost->dir = pac_Dir_reverse(ghost->dir);
                return;
            }
            if (ghost->pos.x != ghost->target.x) {
                ghost->dir = ghost->target.x > ghost->pos.x ? pac_Dir_right : pac_Dir_left;
                return;
            }
            ghost->in_home = false;
            ghost->target = pac_Ghost_outside;
            ghost->dir = pac_Dir_up;
            return;
        }
        if (ghost->pos.y != ghost->target.y) {
            return;
        }
        ghost->mode = pac_Ghost_scattering;
        ghost->mode_left = time_Dur_fromSecs(7);
        ghost->dir = Rand_boolean(&self->rng) ? pac_Dir_left : pac_Dir_right;
        return;
    }
    pac_Ghost_updateTarget(self, ghost);
    ghost->dir = pac_Ghost_chooseDir(self, ghost);
};

$static fn_((pac_Game_ghostStep(pac_Game* self, pac_Ghost* ghost))(void)) {
    pac_Ghost_updateMode(ghost);
    pac_Game_ghostSteer(self, ghost);
    pac_Game_ghostAdvance(self, ghost);
};

/*========== Commands/Input =================================================*/

#include <dansi-core/cursor.h>
#include <dansi-core/erase.h>
#include <dansi-core/sgr.h>
#include <dansi-xterm/color.h>
#include <dansi-xterm/screen.h>
#include <dansi-dec/cursor.h>

#include "daterm-context/ANSI.h"
#include "daterm-runtime/Term.h"
#include "daterm-runtime/key.h"

typedef enum_((pac_Cmd $fits($packed))(
    pac_Cmd_quit = 0,
    pac_Cmd_pause,
    pac_Cmd_up,
    pac_Cmd_down,
    pac_Cmd_left,
    pac_Cmd_right
)) pac_Cmd;
T_use_prl$(pac_Cmd);

$static fn_((pac_Game_playerLook(pac_Game* self, pac_Dir dir))(void)) {
    let look_pos = pac_Board_wrapPos(pac_V2i_add(self->player.pos, pac_Dir_delta(dir)));
    if (pac_Game_playerCanMove(self, look_pos)) {
        self->player.dir = dir;
        asg_l((&self->player.next_dir)(none()));
    } else {
        asg_l((&self->player.next_dir)(some(dir)));
    }
};

$static fn_((pac_applyCmd(pac_Game* self, pac_Cmd cmd))(void)) {
    switch (cmd) {
    case_((pac_Cmd_quit)) self->is_running = false $end(case);
    case_((pac_Cmd_pause)) self->is_paused = !self->is_paused $end(case);
    case_((pac_Cmd_up)) pac_Game_playerLook(self, pac_Dir_up) $end(case);
    case_((pac_Cmd_down)) pac_Game_playerLook(self, pac_Dir_down) $end(case);
    case_((pac_Cmd_left)) pac_Game_playerLook(self, pac_Dir_left) $end(case);
    case_((pac_Cmd_right)) pac_Game_playerLook(self, pac_Dir_right) $end(case);
    }
};

$static fn_((pac_cmdFromText(daterm_key_Text text))(O$pac_Cmd) $scope) {
    if_some((text.action)(action)) {
        if (action == daterm_key_Action_release) return_none();
    }
    var_(ch, u32) = text.codepoint;
    if (ch >= 'A' && ch <= 'Z') ch += 'a' - 'A';
    if (text.mods.ctrl && ch == 'c') return_some(pac_Cmd_quit);
    switch (ch) {
    case 'q': return_some(pac_Cmd_quit);
    case 'p': return_some(pac_Cmd_pause);
    case 'w': return_some(pac_Cmd_up);
    case 's': return_some(pac_Cmd_down);
    case 'a': return_some(pac_Cmd_left);
    case 'd': return_some(pac_Cmd_right);
    default: return_none();
    }
} $unscoped(fn);

$static fn_((pac_cmdFromKey(daterm_key_Event key))(O$pac_Cmd) $scope) {
    if_some((key.action)(action)) {
        if (action == daterm_key_Action_release) return_none();
    }
    if (key.code == daterm_key_Code_escape) return_some(pac_Cmd_quit);
    if (key.code == daterm_key_Code_up) return_some(pac_Cmd_up);
    if (key.code == daterm_key_Code_down) return_some(pac_Cmd_down);
    if (key.code == daterm_key_Code_left) return_some(pac_Cmd_left);
    if (key.code == daterm_key_Code_right) return_some(pac_Cmd_right);
    return_none();
} $unscoped(fn);

#define pac_Input_events_per_frame 16

$static fn_((pac_pollInput(pac_Game* self, daterm_Term term))(E$void) $scope) {
    for_(($r(0, pac_Input_events_per_frame))(i)) {
        let_ignore = i;
        let event = orelse_((daterm_Term_poll(term))(return_ok({})));
        $suppress_(switch_enum)(match_(event)) {
        patt_((daterm_Event_key)(key)) {
            if_some((pac_cmdFromKey(key))(cmd)) pac_applyCmd(self, cmd);
        } $end(patt);
        patt_((daterm_Event_text)(text)) {
            if_some((pac_cmdFromText(text))(cmd)) pac_applyCmd(self, cmd);
        } $end(patt);
        default_() $do_nothing $end(default);
        } $end(match);
    } $end(for);
    return_ok({});
} $unscoped(fn);

/*========== Async routines =================================================*/

#include <dh/exec/Coop.h>

typedef struct pac_Sys {
    var_(sched, Sched);
    var_(time, time_Awake);
} pac_Sys;
T_use$((Void)(Future, Future_cancel, Sched_async));

typedef struct pac_Player_RoutineCtx {
    var_(sys, pac_Sys);
    var_(game, pac_Game*);
} pac_Player_RoutineCtx;
$static co_fn_(pac_Player_animCo, (pac_Player_RoutineCtx ctx), Void);
co_fn_frame_scope(
    pac_Player_animCo,
    co_locals_({}),
    co_locals_mut_({}),
    co_suspended_({ var_(sleeping, Void); })
);
co_fn_scope(pac_Player_animCo) {
    while ($co_arg(ctx).game->is_running) {
        suspend_((sleeping)(catch_((time_Awake_sleep($co_arg(ctx).sys.time, time_Dur_fromMillis(34)))($ignore, $do_nothing))));
        if (!$co_arg(ctx).game->is_paused) {
            $co_arg(ctx).game->player.anim_frame = as$(u8)(($co_arg(ctx).game->player.anim_frame + 1) % 6);
        }
        catch_((Sched_idle($co_arg(ctx).sys.sched))($ignore, $do_nothing));
    }
    co_return_({});
} $unscoped(co_fn);
co_use_Clsr_((pac_Player_animCo)(pac_Player_RoutineCtx)(Void));

/*========== Tick ===========================================================*/

$static fn_((pac_Game_ghostTick(pac_Game* self, pac_Ghost* ghost, time_Dur dt))(void)) {
    ghost->mode_left = time_Dur_subSat(ghost->mode_left, dt);
    ghost->frightened_left = time_Dur_subSat(ghost->frightened_left, dt);
    pac_Ghost_updateMode(ghost);
    ghost->step_elapsed = time_Dur_addSat(ghost->step_elapsed, dt);
    while (time_Dur_ge(ghost->step_elapsed, pac_Ghost_stepDur(ghost))) {
        let step_dur = pac_Ghost_stepDur(ghost);
        pac_Game_ghostStep(self, ghost);
        if (!self->is_running) return;
        ghost->step_elapsed = time_Dur_subSat(ghost->step_elapsed, step_dur);
    }
};

$static fn_((pac_Game_tick(pac_Game* self, time_Dur dt))(void)) {
    if (pac_Game_tickFlash(self, dt)) return;
    if (self->is_paused || self->is_game_over || self->is_win) return;
    self->player.step_elapsed = time_Dur_addSat(self->player.step_elapsed, dt);
    while (time_Dur_ge(self->player.step_elapsed, time_Dur_fromMillis(200))) {
        pac_Game_playerStep(self);
        if (!self->is_running) return;
        self->player.step_elapsed = time_Dur_subSat(self->player.step_elapsed, time_Dur_fromMillis(200));
    }
    for_(($s(A_ref(self->ghosts)))(ghost)) {
        pac_Game_ghostTick(self, ghost, dt);
        if (!self->is_running) return;
    } $end(for);
};

/*========== Presenter ======================================================*/

#include <dh/io/Buf.h>

#define pac_Frame_w (pac_Board_w + 24)
#define pac_Frame_h pac_Board_h
#define pac_Presenter_buf_len 16384ull

typedef struct pac_RGB {
    u8 r;
    u8 g;
    u8 b;
} pac_RGB;
T_use_prl$(pac_RGB);

typedef struct pac_FrameCell {
    var_(ch, u8);
    var_(fg, pac_RGB);
} pac_FrameCell;
T_use_prl$(pac_FrameCell);

$static let_(pac_FrameCell_blank, pac_FrameCell) = { .ch = ' ', .fg = { 0, 0, 0 } };

typedef struct pac_Frame {
    var_(cells, A$$(pac_Frame_w * pac_Frame_h, pac_FrameCell));
} pac_Frame;

typedef struct pac_Presenter {
    var_(frame, pac_Frame);
    var_(front, pac_Frame);
    var_(front_valid, bool);
    var_(out, struct {
        var_(mem, A$$(pac_Presenter_buf_len, u8));
        var_(writer, io_Buf_Writer);
    });
} pac_Presenter;

typedef struct pac_Presenter_StyleState {
    var_(fg, pac_RGB);
    var_(fg_valid, bool);
} pac_Presenter_StyleState;

typedef struct pac_Presenter_DiffStats {
    var_(changed_cells, usize);
    var_(changed_runs, usize);
} pac_Presenter_DiffStats;
T_use_prl$(pac_Presenter_DiffStats);

$static fn_((pac_rgbEql(pac_RGB lhs, pac_RGB rhs))(bool)) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
};
$static fn_((pac_FrameCell_eql(pac_FrameCell lhs, pac_FrameCell rhs))(bool)) {
    return lhs.ch == rhs.ch && pac_rgbEql(lhs.fg, rhs.fg);
};
$static fn_((pac_Frame_clear(pac_Frame* self))(void)) {
    for_(($s(A_ref(self->cells)))(cell)) *cell = pac_FrameCell_blank $end(for);
};
$static fn_((pac_Frame_atMut(pac_Frame* self, usize x, usize y))(O$P$pac_FrameCell) $scope) {
    if (x >= pac_Frame_w || y >= pac_Frame_h) return_none();
    return_some(A_at((self->cells)[(y * pac_Frame_w) + x]));
} $unscoped(fn);
$static fn_((pac_Frame_put(pac_Frame* self, usize x, usize y, u8 ch, pac_RGB fg))(void)) {
    if_some((pac_Frame_atMut(self, x, y))(cell)) *cell = (pac_FrameCell){ .ch = ch, .fg = fg };
};
$static fn_((pac_Frame_putText(pac_Frame* self, usize x, usize y, S_const$u8 text, pac_RGB fg))(void)) {
    for_(($s(text), $rf(x))(ch, px)) pac_Frame_put(self, px, y, *ch, fg) $end(for);
};
$static fn_((pac_Presenter_formatU32(S$u8 buf, u32 val))(usize)) {
    var_(rev, A$$(10, u8)) $undefined;
    var_(len, usize) = 0;
    do {
        *A_at((rev)[len]) = as$(u8)('0' + val % 10);
        val /= 10;
        len += 1;
    } while (val != 0);
    for_(($rt(len))(i)) *S_at((buf)[i]) = *A_at((rev)[len - i - 1]) $end(for);
    return len;
};
$static fn_((pac_Presenter_init(pac_Presenter* self, io_Writer out))(pac_Presenter*)) {
    self->out.writer = io_Buf_Writer_init(out, A_ref$((S$u8)(self->out.mem)));
    pac_Frame_clear(&self->frame);
    pac_Frame_clear(&self->front);
    self->front_valid = false;
    return self;
};
$static fn_((pac_moveTo(io_Writer out, usize x, usize y))(E$void)) {
    return dansi_cursor_moveToWrite(as$(u16)(y + 1), as$(u16)(x + 1), out);
};
$static fn_((pac_fg(io_Writer out, pac_RGB rgb))(E$void)) {
    return dansi_xterm_color_fg24bitWrite((dansi_xterm_color_RGB8){ rgb.r, rgb.g, rgb.b }, out);
};
$attr($maybe_unused)
$static fn_((pac_bg(io_Writer out, pac_RGB rgb))(E$void)) {
    return dansi_xterm_color_bg24bitWrite((dansi_xterm_color_RGB8){ rgb.r, rgb.g, rgb.b }, out);
};
$static fn_((pac_writeByte(io_Writer out, u8 c))(E$void)) {
    var_(mem, A$$(1, u8)) = A_init({ c });
    return io_Writer_writeBytes(out, A_ref$((S_const$u8)(mem)));
};

$static fn_((pac_Presenter_StyleState_init(void))(pac_Presenter_StyleState)) {
    return (pac_Presenter_StyleState){ .fg = { 0, 0, 0 }, .fg_valid = false };
};
$static fn_((pac_Presenter_StyleState_applyCell(pac_Presenter_StyleState* self, io_Writer out, pac_FrameCell cell))(E$void) $scope) {
    if (!self->fg_valid || !pac_rgbEql(self->fg, cell.fg)) {
        try_(pac_fg(out, cell.fg));
        self->fg = cell.fg;
        self->fg_valid = true;
    }
    return_ok({});
} $unscoped(fn);

$static fn_((pac_tileGlyph(pac_Tile tile))(u8)) {
    switch (tile) {
    case pac_Tile_empty: return ' ';
    case pac_Tile_wall: return '#';
    case pac_Tile_pellet: return '.';
    case pac_Tile_door: return '-';
    case pac_Tile_power: return 'o';
    }
};
$static fn_((pac_tileFG(pac_Tile tile))(pac_RGB)) {
    switch (tile) {
    case pac_Tile_empty: return (pac_RGB){ 0, 0, 0 };
    case pac_Tile_wall: return (pac_RGB){ 70, 120, 255 };
    case pac_Tile_pellet: return (pac_RGB){ 255, 220, 120 };
    case pac_Tile_power: return (pac_RGB){ 255, 120, 140 };
    case pac_Tile_door: return (pac_RGB){ 150, 150, 150 };
    }
};
$static fn_((pac_tileFGForGame(const pac_Game* game, pac_Tile tile))(pac_RGB)) {
    if (tile == pac_Tile_wall && game->flash_wall_white) return (pac_RGB){ 245, 245, 245 };
    return pac_tileFG(tile);
};
$static fn_((pac_playerGlyph(pac_Dir dir, u8 frame))(u8)) {
    let idx = usize_(frame % 6);
    switch (dir) {
    case_((pac_Dir_up)) {
        let frames = u8_l("VV|||V");
        return *S_at((frames)[idx]);
    } $end(case);
    case_((pac_Dir_left)) {
        let frames = u8_l("})>->)");
        return *S_at((frames)[idx]);
    } $end(case);
    case_((pac_Dir_right)) {
        let frames = u8_l("{(<-<(");
        return *S_at((frames)[idx]);
    } $end(case);
    case_((pac_Dir_down)) {
        let frames = u8_l("^^|||^");
        return *S_at((frames)[idx]);
    } $end(case);
    case_((count$pac_Dir)) claim_unreachable $end(case);
    }
};
$static fn_((pac_ghostFG(pac_Ghost_Type type, pac_Ghost_Mode mode, bool is_frightened))(pac_RGB)) {
    if (mode == pac_Ghost_eaten) return (pac_RGB){ 245, 245, 245 };
    if (is_frightened) return (pac_RGB){ 50, 80, 255 };
    switch (type) {
    case pac_Ghost_blinky: return (pac_RGB){ 255, 80, 80 };
    case pac_Ghost_inky: return (pac_RGB){ 80, 220, 255 };
    case pac_Ghost_pinky: return (pac_RGB){ 255, 130, 220 };
    case pac_Ghost_clyde: return (pac_RGB){ 255, 180, 60 };
    case count$pac_Ghost_Type: claim_unreachable;
    }
};
$static fn_((pac_ghostGlyph(pac_Ghost_Mode mode, bool is_frightened))(u8)) {
    if (mode == pac_Ghost_eaten) return '%';
    if (is_frightened) return 'm';
    switch (mode) {
    case pac_Ghost_frightened: return 'm';
    case pac_Ghost_eaten: return '%';
    case pac_Ghost_staying: $fallthrough;
    case pac_Ghost_scattering: $fallthrough;
    case pac_Ghost_chasing: return 'M';
    }
};

$static fn_((pac_Presenter_compose(pac_Presenter* self, const pac_Game* game))(void)) {
    pac_Frame_clear(&self->frame);
    let frame = &self->frame;
    let ox = usize_(2);
    let oy = usize_(0);
    for_(($r(0, pac_Board_h))(y)) {
        for_(($r(0, pac_Board_w))(x)) {
            let tile = pac_Board_at(&game->board, x, y);
            pac_Frame_put(frame, ox + x, oy + y, pac_tileGlyph(tile), pac_tileFGForGame(game, tile));
        } $end(for);
    } $end(for);

    let p = game->player.pos;
    pac_Frame_put(
        frame,
        ox + as$(usize)(p.x),
        oy + as$(usize)(p.y),
        pac_playerGlyph(game->player.dir, game->player.anim_frame),
        (pac_RGB){ 255, 235, 40 }
    );

    for_(($r(0, count$pac_Ghost_Type))(i)) {
        let ghost = A_at((game->ghosts)[i]);
        pac_Frame_put(
            frame,
            ox + as$(usize)(ghost->pos.x),
            oy + as$(usize)(ghost->pos.y),
            pac_ghostGlyph(ghost->mode, pac_Ghost_isFrightened(ghost)),
            pac_ghostFG(ghost->type, ghost->mode, pac_Ghost_isFrightened(ghost))
        );
    } $end(for);

    let sx = ox + pac_Board_w + 3;
    var_(num_mem, A$$(16, u8)) $undefined;
    var num_buf = A_ref$((S$u8)(num_mem));
    pac_Frame_putText(frame, sx, 1, u8_l("HI-SCORE"), (pac_RGB){ 255, 80, 80 });
    let high_len = pac_Presenter_formatU32(num_buf, game->high_score);
    pac_Frame_putText(frame, sx, 2, S_prefix((num_buf)(high_len)).as_const, (pac_RGB){ 240, 240, 240 });
    pac_Frame_putText(frame, sx, 4, u8_l("SCORE"), (pac_RGB){ 240, 240, 240 });
    let score_len = pac_Presenter_formatU32(num_buf, game->score);
    pac_Frame_putText(frame, sx + 7, 4, S_prefix((num_buf)(score_len)).as_const, (pac_RGB){ 240, 240, 240 });
    pac_Frame_putText(frame, sx, 6, u8_l("LIFE"), (pac_RGB){ 255, 235, 40 });
    let lives_len = pac_Presenter_formatU32(num_buf, as$(u32)(game->player.lives));
    pac_Frame_putText(frame, sx + 7, 6, S_prefix((num_buf)(lives_len)).as_const, (pac_RGB){ 240, 240, 240 });
    pac_Frame_putText(frame, sx, 8, u8_l("LEFT"), (pac_RGB){ 240, 240, 240 });
    let left_len = pac_Presenter_formatU32(num_buf, as$(u32)(game->remaining));
    pac_Frame_putText(frame, sx + 7, 8, S_prefix((num_buf)(left_len)).as_const, (pac_RGB){ 240, 240, 240 });
    pac_Frame_putText(frame, sx, 11, u8_l("WASD/Arrows move"), (pac_RGB){ 180, 200, 220 });
    pac_Frame_putText(frame, sx, 12, u8_l("P pause, Q quit"), (pac_RGB){ 180, 200, 220 });
    if (game->is_paused) {
        pac_Frame_putText(frame, sx, 15, u8_l("PAUSED"), (pac_RGB){ 255, 230, 90 });
    }
    if (game->is_game_over || game->is_win) {
        pac_Frame_putText(
            frame,
            sx,
            17,
            game->is_win ? u8_l("YOU WIN") : u8_l("GAME OVER"),
            game->is_win ? (pac_RGB){ 90, 255, 120 } : (pac_RGB){ 255, 90, 90 }
        );
    }
};

$static fn_((pac_Presenter_beginScreen(io_Writer out))(E$void) $scope) {
    try_(dansi_sgr_resetWrite(out));
    try_(dansi_erase_inDisplayWrite(dansi_erase_Area_all, out));
    return pac_moveTo(out, 0, 0);
} $unscoped(fn);

$static fn_((pac_Presenter_homeScreen(io_Writer out))(E$void)) {
    return pac_moveTo(out, 0, 0);
};

$static fn_((pac_Presenter_presentSmallScreen(pac_Presenter* self))(E$void) $scope) {
    let out = io_Buf_writer(&self->out.writer);
    try_(pac_Presenter_beginScreen(out));
    try_(pac_fg(out, (pac_RGB){ 255, 160, 80 }));
    try_(io_Writer_writeBytes(out, u8_l("screen too small for dasae pacman")));
    try_(dansi_sgr_resetWrite(out));
    try_(io_Buf_Writer_flush(&self->out.writer));
    self->front_valid = false;
    return_ok({});
} $unscoped(fn);

$static fn_((pac_Presenter_presentFull(pac_Presenter* self, bool clear))(E$void) $scope) {
    let out = io_Buf_writer(&self->out.writer);
    if (clear) {
        try_(pac_Presenter_beginScreen(out));
    } else {
        try_(pac_Presenter_homeScreen(out));
    }
    var style = pac_Presenter_StyleState_init();
    for_(($r(0, pac_Frame_h))(y)) {
        for_(($r(0, pac_Frame_w))(x)) {
            let cell = *A_at((self->frame.cells)[(y * pac_Frame_w) + x]);
            try_(pac_Presenter_StyleState_applyCell(&style, out, cell));
            try_(pac_writeByte(out, cell.ch));
        } $end(for);
        if (y + 1 < pac_Frame_h) try_(io_Writer_writeBytes(out, u8_l("\r\n")));
    } $end(for);
    try_(dansi_sgr_resetWrite(out));
    try_(io_Buf_Writer_flush(&self->out.writer));
    self->front = self->frame;
    self->front_valid = true;
    return_ok({});
} $unscoped(fn);

$static fn_((pac_Presenter_presentDiff(pac_Presenter* self))(E$void) $scope) {
    let out = io_Buf_writer(&self->out.writer);
    var style = pac_Presenter_StyleState_init();
    for_(($r(0, pac_Frame_h))(y)) {
        var_(x, usize) = 0;
        while (x < pac_Frame_w) {
            let idx = (y * pac_Frame_w) + x;
            let curr = *A_at((self->frame.cells)[idx]);
            let prev = *A_at((self->front.cells)[idx]);
            if (pac_FrameCell_eql(curr, prev)) {
                x += 1;
                continue;
            }
            try_(pac_moveTo(out, x, y));
            while (x < pac_Frame_w) {
                let run_idx = (y * pac_Frame_w) + x;
                let run_curr = *A_at((self->frame.cells)[run_idx]);
                let run_prev = *A_at((self->front.cells)[run_idx]);
                if (pac_FrameCell_eql(run_curr, run_prev)) break;
                try_(pac_Presenter_StyleState_applyCell(&style, out, run_curr));
                try_(pac_writeByte(out, run_curr.ch));
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

$static fn_((pac_Presenter_DiffStats_calc(const pac_Presenter* self))(pac_Presenter_DiffStats)) {
    var stats = (pac_Presenter_DiffStats){};
    for_(($r(0, pac_Frame_h))(y)) {
        var_(was_changed, bool) = false;
        for_(($r(0, pac_Frame_w))(x)) {
            let idx = (y * pac_Frame_w) + x;
            let changed = !pac_FrameCell_eql(*A_at((self->frame.cells)[idx]), *A_at((self->front.cells)[idx]));
            if (changed) stats.changed_cells += 1;
            if (changed && !was_changed) stats.changed_runs += 1;
            was_changed = changed;
        } $end(for);
    } $end(for);
    return stats;
};

$static fn_((pac_Presenter_present(pac_Presenter* self))(E$void) $scope) {
    if (!self->front_valid) return pac_Presenter_presentFull(self, true);
    let stats = pac_Presenter_DiffStats_calc(self);
    if (stats.changed_cells == 0) return_ok({});
    return stats.changed_runs > (pac_Frame_h / 2)
             ? pac_Presenter_presentFull(self, false)
             : pac_Presenter_presentDiff(self);
} $unscoped(fn);

$static fn_((pac_Presenter_render(pac_Presenter* self, const pac_Game* game, daterm_CellSize screen))(E$void)) {
    pac_Presenter_compose(self, game);
    if (screen.cols < pac_Frame_w || screen.rows < pac_Frame_h) {
        return pac_Presenter_presentSmallScreen(self);
    }
    return pac_Presenter_present(self);
};

/*========== Main ===========================================================*/

#include <dh-main.h>
#include <dh/heap/Sys.h>
#include <dh/heap/Arena.h>
#include <dh/time/Clock.h>
#include <dh/exec/Coop.h>
#include <dh/Future.h>
#include <dh/Sched.h>
#include <dh/utf8.h>
#include <dh/ascii.h>
#include <dh/io/Fixed.h>
#include <dh/io/stream.h>

$static fn_((pac_waitForEnter(void))(void)) {
    let out = fs_File_writer(io_handleStdOut());
    catch_((io_Writer_writeBytes(out, u8_l("dasae pacman: press Enter to enter terminal mode\n")))($ignore, $do_nothing));
    var_(read_mem, A$$(128, u8)) $undefined;
    var reader = io_Buf_Reader_init(fs_File_reader(io_handleStdIn()), A_ref$((S$u8)(read_mem)));
    var_(line_mem, A$$(256, u8)) $undefined;
    catch_((io_Buf_Reader_readUntilByte(&reader, ascii_lf_byte, A_ref$((S$u8)(line_mem))))($ignore, $do_nothing));
};

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    pac_waitForEnter();

    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    var arena = heap_Arena_init(heap_Sys_alctr(&heap));
    defer_(heap_Arena_fini(&arena));
    let gpa = heap_Arena_alctr(&arena);

    var ansi = try_(daterm_ANSI_init(daterm_ANSI_Cfg_default(gpa)));
    defer_(daterm_ANSI_fini(&ansi));
    try_(daterm_ANSI_enableRawMode(&ansi));
    defer_(daterm_ANSI_disableRawMode(&ansi));

    var coop = exec_Coop_init(gpa, try_(time_Awake_direct()));
    defer_(exec_Coop_fini(&coop));
    let sched = Sched_coop(&coop);
    let sys = (pac_Sys){ .sched = sched, .time = time_Awake_evented(&coop) };

    let term = daterm_ANSI_term(&ansi);
    let out = daterm_Term_writer(term);
    try_(dansi_xterm_screen_enterAlternateWrite(out));
    defer_(catch_((dansi_xterm_screen_exitAlternateWrite(out))($ignore, $do_nothing)));
    try_(dansi_dec_cursor_hideWrite(out));
    defer_(catch_((dansi_dec_cursor_showWrite(out))($ignore, $do_nothing)));

    $static var_(game_storage, pac_Game) $undefined_static;
    let game = pac_Game_reset(&game_storage);
    $static var_(presenter_storage, pac_Presenter) $undefined_static;
    let presenter = pac_Presenter_init(&presenter_storage, out);

    var player_anim_clsr = clsr_((pac_Player_animCo)((pac_Player_RoutineCtx){ .sys = sys, .game = game }));
    var player_anim_task = Sched_async$Void(sched, player_anim_clsr.as_base);
    defer_(let_ignore = Future_cancel$Void(&player_anim_task, sched));

    let frame_interval = time_Dur_fromSecs$f64(1.0 / 30.0);
    var schedule_from = time_Clock_now(ansi.clock);
    var last_tick = schedule_from;
    while (game->is_running) {
        let now = time_Clock_now(ansi.clock);
        let dt = time_Clock_Inst_durSince(now, last_tick);
        last_tick = now;

        try_(pac_pollInput(game, term));
        pac_Game_tick(game, dt);
        exec_Coop_runUntil(&coop, time_Awake_Inst_addDur(time_Awake_now(sys.time), time_Dur_fromMillis(1)));

        let screen = try_(daterm_Term_queryNativeScreenCells(term));
        try_(pac_Presenter_render(presenter, game, screen));
        try_(daterm_Term_flush(term));

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

TEST_fn_("example-pacman: direction deltas use board coordinates" $scope) {
    try_(TEST_expect(pac_Dir_delta(pac_Dir_up).x == 0));
    try_(TEST_expect(pac_Dir_delta(pac_Dir_up).y == -1));
    try_(TEST_expect(pac_Dir_delta(pac_Dir_down).x == 0));
    try_(TEST_expect(pac_Dir_delta(pac_Dir_down).y == 1));
    try_(TEST_expect(pac_Dir_delta(pac_Dir_left).x == -1));
    try_(TEST_expect(pac_Dir_delta(pac_Dir_right).x == 1));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: player animation is direction-specific" $scope) {
    try_(TEST_expect(pac_playerGlyph(pac_Dir_up, 0) == 'V'));
    try_(TEST_expect(pac_playerGlyph(pac_Dir_down, 0) == '^'));
    try_(TEST_expect(pac_playerGlyph(pac_Dir_left, 2) == '>'));
    try_(TEST_expect(pac_playerGlyph(pac_Dir_right, 2) == '<'));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: up input advances toward lower board rows" $scope) {
    var_(game, pac_Game) $undefined;
    pac_Game_reset(&game);
    game.player.pos = pac_V2i_of(1, 4);
    game.player.dir = pac_Dir_up;
    pac_Game_tick(&game, time_Dur_fromMillis(200));
    try_(TEST_expect(game.player.pos.x == 1));
    try_(TEST_expect(game.player.pos.y == 3));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: movement input turns immediately when open" $scope) {
    var_(game, pac_Game) $undefined;
    pac_Game_reset(&game);
    game.player.pos = pac_V2i_of(1, 4);
    game.player.dir = pac_Dir_right;

    pac_applyCmd(&game, pac_Cmd_up);

    var_(has_next, bool) = false;
    if_some((game.player.next_dir)(next_dir)) {
        let_ignore = next_dir;
        has_next = true;
    };
    try_(TEST_expect(game.player.dir == pac_Dir_up));
    try_(TEST_expect(!has_next));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: movement input buffers when blocked" $scope) {
    var_(game, pac_Game) $undefined;
    pac_Game_reset(&game);
    game.player.pos = pac_V2i_of(1, 1);
    game.player.dir = pac_Dir_right;

    pac_applyCmd(&game, pac_Cmd_up);

    var_(has_next, bool) = false;
    if_some((game.player.next_dir)(next_dir)) {
        has_next = true;
        try_(TEST_expect(next_dir == pac_Dir_up));
    };
    try_(TEST_expect(game.player.dir == pac_Dir_right));
    try_(TEST_expect(has_next));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: scatter steering consumes game rng" $scope) {
    var_(game, pac_Game) $undefined;
    pac_Game_reset(&game);
    game.rng = Rand_initSeed(1234);
    let before = game.rng.state;
    let ghost = A_at((game.ghosts)[0]);
    ghost->mode = pac_Ghost_scattering;
    ghost->pos = pac_V2i_of(13, 4);
    ghost->dir = pac_Dir_up;
    ghost->target = pac_Ghost_scatterTarget(ghost->type);
    let_ignore = pac_Ghost_chooseDir(&game, ghost);
    try_(TEST_expect(game.rng.state != before));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: flash blocks movement then releases pending round reset" $scope) {
    var_(game, pac_Game) $undefined;
    pac_Game_reset(&game);
    game.player.pos = pac_V2i_of(1, 4);
    game.player.dir = pac_Dir_down;
    pac_Game_startFlash(&game, 1, 500);
    game.reset_round_after_flash = true;

    pac_Game_tick(&game, time_Dur_fromMillis(200));
    try_(TEST_expect(game.player.pos.y == 4));
    try_(TEST_expect(game.reset_round_after_flash));

    pac_Game_tick(&game, time_Dur_fromMillis(300));
    try_(TEST_expect(!game.reset_round_after_flash));
    try_(TEST_expect(game.player.pos.x == pac_Player_init_pos.x));
    try_(TEST_expect(game.player.pos.y == pac_Player_init_pos.y));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: collision only consumes one life while flash is pending" $scope) {
    var_(game, pac_Game) $undefined;
    pac_Game_reset(&game);
    game.player.pos = pac_V2i_of(5, 5);
    A_at((game.ghosts)[0])->pos = game.player.pos;
    A_at((game.ghosts)[1])->pos = game.player.pos;

    pac_Game_collide(&game);
    pac_Game_collide(&game);

    try_(TEST_expect(game.player.lives == 2));
    try_(TEST_expect(game.reset_round_after_flash));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: ghost glyphs represent state, not animation" $scope) {
    try_(TEST_expect(pac_ghostGlyph(pac_Ghost_staying, false) == 'M'));
    try_(TEST_expect(pac_ghostGlyph(pac_Ghost_scattering, false) == 'M'));
    try_(TEST_expect(pac_ghostGlyph(pac_Ghost_chasing, false) == 'M'));
    try_(TEST_expect(pac_ghostGlyph(pac_Ghost_staying, true) == 'm'));
    try_(TEST_expect(pac_ghostGlyph(pac_Ghost_eaten, true) == '%'));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: power pellet overlays frightened without stopping base state" $scope) {
    var_(game, pac_Game) $undefined;
    pac_Game_reset(&game);
    game.player.pos = pac_V2i_of(1, 3);
    game.player.dir = pac_Dir_left;

    let ghost = A_at((game.ghosts)[pac_Ghost_clyde]);
    try_(TEST_expect(ghost->mode == pac_Ghost_staying));
    try_(TEST_expect(time_Dur_isZero(ghost->frightened_left)));

    pac_Game_tick(&game, time_Dur_fromMillis(200));

    try_(TEST_expect(game.is_running));
    try_(TEST_expect(ghost->mode == pac_Ghost_staying));
    try_(TEST_expect(pac_Ghost_isFrightened(ghost)));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: eaten ghost returns from upper map through shortest legal path" $scope) {
    var_(game, pac_Game) $undefined;
    pac_Game_reset(&game);
    let ghost = A_at((game.ghosts)[0]);
    ghost->mode = pac_Ghost_eaten;
    ghost->mode_left = time_Dur_fromSecs(8);
    ghost->pos = pac_V2i_of(13, 4);
    ghost->target = pac_Ghost_inside;
    ghost->dir = pac_Dir_left;
    ghost->in_home = false;

    for_(($r(0, 96))(step)) {
        let_ignore = step;
        pac_Game_ghostStep(&game, ghost);
        if (ghost->mode == pac_Ghost_staying) break;
    } $end(for);

    try_(TEST_expect(ghost->mode == pac_Ghost_staying));
    try_(TEST_expect(ghost->in_home));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: ghosts do not choose the player-only tunnel exit" $scope) {
    var_(game, pac_Game) $undefined;
    pac_Game_reset(&game);
    let ghost = A_at((game.ghosts)[0]);
    ghost->mode = pac_Ghost_chasing;
    ghost->pos = pac_V2i_of(0, 11);
    ghost->target = pac_V2i_of(0, 23);
    ghost->dir = pac_Dir_left;
    ghost->in_home = false;

    try_(TEST_expect(!pac_Game_ghostCanMove(&game, ghost, pac_V2i_of(-1, 11))));
    try_(TEST_expect(pac_Ghost_chooseDir(&game, ghost) == pac_Dir_right));
    return_ok({});
} $unscoped(TEST_fn);

TEST_fn_("example-pacman: recovered ghosts leave the house after staying" $scope) {
    var_(game, pac_Game) $undefined;
    pac_Game_reset(&game);
    let ghost = A_at((game.ghosts)[pac_Ghost_clyde]);
    pac_Ghost_recoverFromEaten(ghost);
    game.player.pos = pac_V2i_of(1, 1);

    for_(($r(0, 48))(step)) {
        let_ignore = step;
        pac_Game_ghostTick(&game, ghost, time_Dur_fromMillis(250));
        if (ghost->mode == pac_Ghost_scattering) break;
    } $end(for);

    try_(TEST_expect(ghost->mode == pac_Ghost_scattering));
    try_(TEST_expect(!ghost->in_home));
    return_ok({});
} $unscoped(TEST_fn);
