/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    plat.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dal-project/da/foundation/cfg
 * @prefix  plat
 *
 * @brief   Native platform facts selected by the compiler
 * @details Identifies the target operating environment without implying that
 *          DH ships a native implementation for it.  Native implementation
 *          selection belongs to the sys layer.
 */
#pragma once
#ifndef foundation_cfg_plat__included
#define foundation_cfg_plat__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp.h"

/*========== Macros and Declarations ========================================*/

/*--- Platform Type ---*/

#define plat_type __comp_enum__plat_type
#define plat_type_unknown __comp_enum__plat_type_unknown
#define plat_type_windows __comp_enum__plat_type_windows
#define plat_type_linux __comp_enum__plat_type_linux
#define plat_type_android __comp_enum__plat_type_android
#define plat_type_darwin __comp_enum__plat_type_darwin
#define plat_type_freebsd __comp_enum__plat_type_freebsd
#define plat_type_netbsd __comp_enum__plat_type_netbsd
#define plat_type_openbsd __comp_enum__plat_type_openbsd
#define plat_type_dragonfly __comp_enum__plat_type_dragonfly
#define plat_type_solaris __comp_enum__plat_type_solaris
#define plat_type_illumos __comp_enum__plat_type_illumos
#define plat_type_haiku __comp_enum__plat_type_haiku
#define plat_type_serenity __comp_enum__plat_type_serenity
#define plat_type_wasi __comp_enum__plat_type_wasi
#define plat_type_emscripten __comp_enum__plat_type_emscripten

#define plat_is_unknown __comp_bool__plat_is_unknown
#define plat_is_windows __comp_bool__plat_is_windows
#define plat_is_linux __comp_bool__plat_is_linux
#define plat_is_android __comp_bool__plat_is_android
#define plat_is_darwin __comp_bool__plat_is_darwin
#define plat_is_freebsd __comp_bool__plat_is_freebsd
#define plat_is_netbsd __comp_bool__plat_is_netbsd
#define plat_is_openbsd __comp_bool__plat_is_openbsd
#define plat_is_dragonfly __comp_bool__plat_is_dragonfly
#define plat_is_solaris __comp_bool__plat_is_solaris
#define plat_is_illumos __comp_bool__plat_is_illumos
#define plat_is_haiku __comp_bool__plat_is_haiku
#define plat_is_serenity __comp_bool__plat_is_serenity
#define plat_is_wasi __comp_bool__plat_is_wasi
#define plat_is_emscripten __comp_bool__plat_is_emscripten

/*--- Platform Name ---*/

#define plat_name __comp_str__plat_name
#define plat_name_unknown __comp_str__plat_name_unknown
#define plat_name_windows __comp_str__plat_name_windows
#define plat_name_linux __comp_str__plat_name_linux
#define plat_name_android __comp_str__plat_name_android
#define plat_name_darwin __comp_str__plat_name_darwin
#define plat_name_freebsd __comp_str__plat_name_freebsd
#define plat_name_netbsd __comp_str__plat_name_netbsd
#define plat_name_openbsd __comp_str__plat_name_openbsd
#define plat_name_dragonfly __comp_str__plat_name_dragonfly
#define plat_name_solaris __comp_str__plat_name_solaris
#define plat_name_illumos __comp_str__plat_name_illumos
#define plat_name_haiku __comp_str__plat_name_haiku
#define plat_name_serenity __comp_str__plat_name_serenity
#define plat_name_wasi __comp_str__plat_name_wasi
#define plat_name_emscripten __comp_str__plat_name_emscripten

/*--- Platform Relations ---*/

#define plat_based_linux __comp_bool__plat_based_linux
#define plat_based_bsd __comp_bool__plat_based_bsd
#define plat_based_unix __comp_bool__plat_based_unix
#define plat_is_posix __comp_bool__plat_is_posix

/*========== Macros and Definitions =========================================*/

/*--- Platform Type ---*/

#define __comp_enum__plat_type plat_type_unknown
#define __comp_enum__plat_type_unknown 0
#define __comp_enum__plat_type_windows 1
#define __comp_enum__plat_type_linux 2
#define __comp_enum__plat_type_android 3
#define __comp_enum__plat_type_darwin 4
#define __comp_enum__plat_type_freebsd 5
#define __comp_enum__plat_type_netbsd 6
#define __comp_enum__plat_type_openbsd 7
#define __comp_enum__plat_type_dragonfly 8
#define __comp_enum__plat_type_solaris 9
#define __comp_enum__plat_type_illumos 10
#define __comp_enum__plat_type_haiku 11
#define __comp_enum__plat_type_serenity 12
#define __comp_enum__plat_type_wasi 13
#define __comp_enum__plat_type_emscripten 14

/* Ordered from more-specific environments to their host-kernel families. */
#if defined(__wasi__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_wasi
#elif defined(__EMSCRIPTEN__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_emscripten
#elif defined(_WIN32) || defined(_WIN64)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_windows
#elif defined(__ANDROID__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_android
#elif defined(__APPLE__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_darwin
#elif defined(__FreeBSD__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_freebsd
#elif defined(__NetBSD__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_netbsd
#elif defined(__OpenBSD__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_openbsd
#elif defined(__DragonFly__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_dragonfly
#elif defined(__illumos__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_illumos
#elif defined(__sun) && defined(__SVR4)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_solaris
#elif defined(__HAIKU__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_haiku
#elif defined(__serenity__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_serenity
#elif defined(__linux__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_linux
#endif

#define __comp_bool__plat_is_unknown pp_Tok_eql(plat_type, plat_type_unknown)
#define __comp_bool__plat_is_windows pp_Tok_eql(plat_type, plat_type_windows)
#define __comp_bool__plat_is_linux pp_Tok_eql(plat_type, plat_type_linux)
#define __comp_bool__plat_is_android pp_Tok_eql(plat_type, plat_type_android)
#define __comp_bool__plat_is_darwin pp_Tok_eql(plat_type, plat_type_darwin)
#define __comp_bool__plat_is_freebsd pp_Tok_eql(plat_type, plat_type_freebsd)
#define __comp_bool__plat_is_netbsd pp_Tok_eql(plat_type, plat_type_netbsd)
#define __comp_bool__plat_is_openbsd pp_Tok_eql(plat_type, plat_type_openbsd)
#define __comp_bool__plat_is_dragonfly pp_Tok_eql(plat_type, plat_type_dragonfly)
#define __comp_bool__plat_is_solaris pp_Tok_eql(plat_type, plat_type_solaris)
#define __comp_bool__plat_is_illumos pp_Tok_eql(plat_type, plat_type_illumos)
#define __comp_bool__plat_is_haiku pp_Tok_eql(plat_type, plat_type_haiku)
#define __comp_bool__plat_is_serenity pp_Tok_eql(plat_type, plat_type_serenity)
#define __comp_bool__plat_is_wasi pp_Tok_eql(plat_type, plat_type_wasi)
#define __comp_bool__plat_is_emscripten pp_Tok_eql(plat_type, plat_type_emscripten)

/*--- Platform Name ---*/

#define __comp_str__plat_name pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(plat_name_windows)), \
        pp_case_((plat_type_linux)(plat_name_linux)), \
        pp_case_((plat_type_android)(plat_name_android)), \
        pp_case_((plat_type_darwin)(plat_name_darwin)), \
        pp_case_((plat_type_freebsd)(plat_name_freebsd)), \
        pp_case_((plat_type_netbsd)(plat_name_netbsd)), \
        pp_case_((plat_type_openbsd)(plat_name_openbsd)), \
        pp_case_((plat_type_dragonfly)(plat_name_dragonfly)), \
        pp_case_((plat_type_solaris)(plat_name_solaris)), \
        pp_case_((plat_type_illumos)(plat_name_illumos)), \
        pp_case_((plat_type_haiku)(plat_name_haiku)), \
        pp_case_((plat_type_serenity)(plat_name_serenity)), \
        pp_case_((plat_type_wasi)(plat_name_wasi)), \
        pp_case_((plat_type_emscripten)(plat_name_emscripten)), \
        pp_default_(()(plat_name_unknown)) \
    ) pp_end \
)
#define __comp_str__plat_name_unknown "Unknown"
#define __comp_str__plat_name_windows "Windows"
#define __comp_str__plat_name_linux "Linux"
#define __comp_str__plat_name_android "Android"
#define __comp_str__plat_name_darwin "Darwin"
#define __comp_str__plat_name_freebsd "FreeBSD"
#define __comp_str__plat_name_netbsd "NetBSD"
#define __comp_str__plat_name_openbsd "OpenBSD"
#define __comp_str__plat_name_dragonfly "DragonFly BSD"
#define __comp_str__plat_name_solaris "Solaris"
#define __comp_str__plat_name_illumos "illumos"
#define __comp_str__plat_name_haiku "Haiku"
#define __comp_str__plat_name_serenity "SerenityOS"
#define __comp_str__plat_name_wasi "WASI"
#define __comp_str__plat_name_emscripten "Emscripten"

/*--- Platform Relations ---*/

#define __comp_bool__plat_based_linux \
    pp_or(plat_is_linux, plat_is_android)
#define __comp_bool__plat_based_bsd pp_or( \
    pp_or(plat_is_freebsd, plat_is_netbsd), \
    pp_or(plat_is_openbsd, plat_is_dragonfly) \
)
#define __comp_bool__plat_based_unix pp_or( \
    pp_or3(plat_based_linux, plat_is_darwin, plat_based_bsd), \
    pp_or4(plat_is_solaris, plat_is_illumos, plat_is_haiku, plat_is_serenity) \
)
#define __comp_bool__plat_is_posix plat_based_unix

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_cfg_plat__included */
