/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-29 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  Cfg
 *
 * @brief   Runtime representation of a DH translation configuration
 * @details `dh/builtin/cfg` remains the compile-time source of truth. This
 *          module materializes compiler, environment, C language and target
 *          facts for binary APIs.
 */
#pragma once
#ifndef Cfg__included
#define Cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Target.h"

/*========== Macros and Declarations ========================================*/

T_alias$((Cfg_Comp_Kind)(enum_((Cfg_Comp_Kind $fits($packed))(
    Cfg_Comp_Kind_unknown = comp_type_unknown,
    Cfg_Comp_Kind_clang = comp_type_clang,
    Cfg_Comp_Kind_gcc = comp_type_gcc,
    Cfg_Comp_Kind_msvc = comp_type_msvc
))));
T_alias$((Cfg_Comp)(struct Cfg_Comp {
    var_(kind, Cfg_Comp_Kind);
    var_(ver, u32);
    var_(gnu_compat_ver, u32);
}));
$extern cmp_fn_eql$((Cfg_Comp)(lhs, rhs));
$extern cmp_fn_neq$((Cfg_Comp)(lhs, rhs));

T_alias$((Cfg_Env_Kind)(enum_((Cfg_Env_Kind $fits($packed))(
    Cfg_Env_Kind_unknown = env_type_unknown,
    Cfg_Env_Kind_hosted = env_type_hosted,
    Cfg_Env_Kind_freestanding = env_type_freestanding
))));
T_alias$((Cfg_Env)(struct Cfg_Env {
    var_(kind, Cfg_Env_Kind);
    var_(start_files_linked, bool);
    var_(crt_linked, bool);
    var_(default_libs_linked, bool);
    var_(compiler_rt_linked, bool);
    var_(libc_linked, bool);
    var_(stdlib_linked, bool);
}));
$extern cmp_fn_eql$((Cfg_Env)(lhs, rhs));
$extern cmp_fn_neq$((Cfg_Env)(lhs, rhs));

T_alias$((Cfg_Lang_Kind)(enum_((Cfg_Lang_Kind $fits($packed))(
    Cfg_Lang_Kind_unknown = lang_mode_unknown,
    Cfg_Lang_Kind_c = lang_mode_c
))));
T_alias$((Cfg_Lang)(struct Cfg_Lang {
    var_(kind, Cfg_Lang_Kind);
    var_(ver, u32);
    var_(strict, bool);
    var_(has_gnu_extn, bool);
    var_(has_ms_extn, bool);
}));
$extern cmp_fn_eql$((Cfg_Lang)(lhs, rhs));
$extern cmp_fn_neq$((Cfg_Lang)(lhs, rhs));

T_alias$((Cfg)(struct Cfg {
    var_(comp, Cfg_Comp);
    var_(env, Cfg_Env);
    var_(lang, Cfg_Lang);
    var_(target, Target);
}));
#define Cfg_here_static(/*void*/) ____Cfg_here_static()
$attr($inline_always)
$static fn_((Cfg_here(void))(Cfg));
$extern fn_((Cfg_self(void))(Cfg));

$extern cmp_fn_eql$((Cfg)(lhs, rhs));
$extern cmp_fn_neq$((Cfg)(lhs, rhs));

/*========== Macros and Definitions =========================================*/

#define ____Cfg_here_static() l$((Cfg){ \
    .comp = { \
        .kind = as$(Cfg_Comp_Kind)(comp_type), \
        .ver = u32_(comp_ver), \
        .gnu_compat_ver = u32_(comp_gnu_ver), \
    }, \
    .env = { \
        .kind = as$(Cfg_Env_Kind)(env_type), \
        .start_files_linked = env_start_files_linked, \
        .crt_linked = env_crt_linked, \
        .default_libs_linked = env_default_libs_linked, \
        .compiler_rt_linked = env_compiler_rt_linked, \
        .libc_linked = env_libc_linked, \
        .stdlib_linked = env_stdlib_linked, \
    }, \
    .lang = { \
        .kind = as$(Cfg_Lang_Kind)(lang_mode), \
        .ver = u32_(lang_ver), \
        .strict = lang_enabled_strict, \
        .has_gnu_extn = lang_has_extn_gnu, \
        .has_ms_extn = lang_has_extn_ms, \
    }, \
    .target = Target_here_static(), \
})
#if in_analysis_active_only || in_comptime
fn_((Cfg_here(void))(Cfg)) {
    return Cfg_here_static();
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Cfg__included */
