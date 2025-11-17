#ifndef subframes__included
#define subframes__included 1

#include "dh/io/stream.h"
#include "dh/log.h"

#include "dh/mem.h"
#include "dh/heap/Fixed.h"
#include "dh/heap/Arena.h"
#include "dh/ArrList.h"

#include "dh/time.h"
#include "dh/Rand.h"

#include "engine.h"

#define window_res_width__320x200  /* template value */ (320)
#define window_res_height__320x200 /* template value */ (200)
#define window_res_width__160x100  /* template value */ (160)
#define window_res_height__160x100 /* template value */ (100)
#define window_res_width__80x50    /* template value */ (80)
#define window_res_height__80x50   /* template value */ (50)
#define window_res_width__40x25    /* template value */ (40)
#define window_res_height__40x25   /* template value */ (25)

#if debug_comp_enabled
#define window_res_width  (window_res_width__80x50)
#define window_res_height (window_res_height__80x50)
#else /* !debug_comp_enabled */
#define window_res_width  (window_res_width__80x50)
#define window_res_height (window_res_height__80x50)
#endif /* debug_comp_enabled */
#define window_res_size (as$(usize)(window_res_width) * window_res_height)

/* (1.0 / render_target_fps__62_50) ~16ms => ~60 FPS, Assume 62.5 FPS for simplicity */
#define render_target_fps__125_0 /* template value */ (125.0)
#define render_target_fps__62_50 /* template value */ (62.50)
#define render_target_fps__50_00 /* template value */ (50.00)
#define render_target_fps__31_25 /* template value */ (31.25)

#define render_target_fps (render_target_fps__31_25)
#define render_target_spf (1.0 / render_target_fps)

#define update_target_fps (480.0f)
#define update_target_spf (1.0f / update_target_fps)

#define state_player_speed       (1000.0f)
#define state_gravity            (100.0f)
#define state_collision_damping  (0.8f)
#define state_objects_cap_inital (512)

#define state_vec2f_threshold lit$((m_V2f32){ .x = 1e-4f, .y = 1e-4f })

#define state_enable_fps_limits 0

/*
 * World Scale = ratio of physical screen size to world space size
 * World Space Size = Window Resolution Size (dimensions correspond to units of physical screen size)
 */

T_use_E$($set(mem_Err)(P$m_V2f32));
T_use$((m_V2f32)(
    ArrList,
    ArrList_init,
    ArrList_fini,
    ArrList_addBack
));

T_use_E$($set(mem_Err)(P$Color));
T_use$((Color)(
    ArrList,
    ArrList_init,
    ArrList_fini,
    ArrList_addBack
));

#endif /* subframes__included */
