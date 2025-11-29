/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    plat_cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2025-11-27 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/foundation
 * @prefix  plat
 *
 * @brief   Platform detection and configuration
 * @details Detects operating system and platform-specific features.
 *          Focus: Windows, Linux, Darwin, and WASI.
 */
#ifndef foundation_plat_cfg__included
#define foundation_plat_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pp.h"

/*========== Macros and Declarations ========================================*/

/* --- Platform Detection --- */

#define plat_type __comp_enum__plat_type
#define plat_type_unknown __comp_enum__plat_type_unknown
#define plat_type_windows __comp_enum__plat_type_windows
#define plat_type_linux __comp_enum__plat_type_linux
#define plat_type_darwin __comp_enum__plat_type_darwin
#define plat_type_wasi __comp_enum__plat_type_wasi

#define plat_name __comp_str__plat_name
#define plat_name_unknown __comp_str__plat_name_unknown
#define plat_name_windows __comp_str__plat_name_windows
#define plat_name_linux __comp_str__plat_name_linux
#define plat_name_darwin __comp_str__plat_name_darwin
#define plat_name_wasi __comp_str__plat_name_wasi

#define plat_based_unix __comp_bool__plat_based_unix
#define plat_is_posix __comp_bool__plat_is_posix

#define plat_is_windows __comp_bool__plat_is_windows
#define plat_is_linux __comp_bool__plat_is_linux
#define plat_is_darwin __comp_bool__plat_is_darwin
#define plat_is_wasi __comp_bool__plat_is_wasi

/* --- Calling Conventions --- */

#define plat_cdecl __comp_attr__plat_cdecl
#define plat_stdcall __comp_attr__plat_stdcall
#define plat_fastcall __comp_attr__plat_fastcall
#define plat_vectorcall __comp_attr__plat_vectorcall

/*========== Macros and Definitions =========================================*/

/* --- Platform Detection --- */

/* Default: unknown platform */
#define __comp_enum__plat_type plat_type_unknown
#define __comp_enum__plat_type_unknown 0
#define __comp_enum__plat_type_windows 1
#define __comp_enum__plat_type_linux 2
#define __comp_enum__plat_type_darwin 3
#define __comp_enum__plat_type_wasi 4

/* Detect WASI (WebAssembly System Interface) */
#if defined(__wasi__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_wasi

/* Detect Windows */
#elif defined(_WIN32) || defined(_WIN64)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_windows

/* Detect Darwin (macOS, iOS, etc.) */
#elif defined(__APPLE__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_darwin

/* Detect Linux */
#elif defined(__linux__)
#undef __comp_enum__plat_type
#define __comp_enum__plat_type plat_type_linux

// /* Detect generic Unix */
// #elif defined(__unix__) || defined(__unix)
// #undef __comp_enum__plat_type
// #define __comp_enum__plat_type plat_type_unix

#else
#warning "Unknown platform detected. Some features may not work as expected."
#endif

#define __comp_str__plat_name pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_windows)(plat_name_windows)), \
        pp_case_((plat_type_linux)(plat_name_linux)), \
        pp_case_((plat_type_darwin)(plat_name_darwin)), \
        pp_case_((plat_type_wasi)(plat_name_wasi)), \
        pp_default_(plat_name_unknown) \
    ) pp_end \
)
#define __comp_str__plat_name_unknown "Unknown"
#define __comp_str__plat_name_windows "Windows"
#define __comp_str__plat_name_linux "Linux"
#define __comp_str__plat_name_darwin "Darwin"
#define __comp_str__plat_name_wasi "WASI"

#define __comp_bool__plat_based_unix pp_expand( \
    pp_switch_ pp_begin(plat_type)( \
        pp_case_((plat_type_linux)(pp_true)), \
        pp_case_((plat_type_darwin)(pp_true)), \
        pp_default_(pp_false) \
    ) pp_end \
)
#define __comp_bool__plat_is_posix plat_based_unix

#define __comp_bool__plat_is_windows pp_Tok_eq(plat_type, plat_type_windows)
#define __comp_bool__plat_is_linux pp_Tok_eq(plat_type, plat_type_linux)
#define __comp_bool__plat_is_darwin pp_Tok_eq(plat_type, plat_type_darwin)
#define __comp_bool__plat_is_wasi pp_Tok_eq(plat_type, plat_type_wasi)

/* --- Calling Conventions --- */

#if plat_type == plat_type_windows
#define __comp_attr__plat_cdecl __cdecl
#define __comp_attr__plat_stdcall __stdcall
#define __comp_attr__plat_fastcall __fastcall
#define __comp_attr__plat_vectorcall __vectorcall
#else /* plat_type != plat_type_windows */
/* Linux/Unix typically ignores these or they are default */
#define __comp_attr__plat_cdecl
#define __comp_attr__plat_stdcall
#define __comp_attr__plat_fastcall
#define __comp_attr__plat_vectorcall
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_plat_cfg__included */
