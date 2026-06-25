/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    plat.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2026-06-03 (date of last update)
 * @ingroup dal-project/da/foundation/cfg
 * @prefix  plat
 *
 * @brief   Platform detection and configuration
 * @details Detects operating system and platform-specific features.
 *          Focus: Windows, Linux, Darwin, and WASI.
 */
#pragma once
#ifndef foundation_cfg_plat__included
#define foundation_cfg_plat__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../pp.h"

/*========== Macros and Declarations ========================================*/

/* --- Platform Detection --- */

#define plat_type __comp_enum__plat_type
#define plat_type_unknown __comp_enum__plat_type_unknown
#define plat_type_windows __comp_enum__plat_type_windows
#define plat_type_linux __comp_enum__plat_type_linux
#define plat_type_darwin __comp_enum__plat_type_darwin
#define plat_type_wasi __comp_enum__plat_type_wasi

#define plat_is_windows __comp_bool__plat_is_windows
#define plat_is_linux __comp_bool__plat_is_linux
#define plat_is_darwin __comp_bool__plat_is_darwin
#define plat_is_wasi __comp_bool__plat_is_wasi

#define plat_name __comp_str__plat_name
#define plat_name_unknown __comp_str__plat_name_unknown
#define plat_name_windows __comp_str__plat_name_windows
#define plat_name_linux __comp_str__plat_name_linux
#define plat_name_darwin __comp_str__plat_name_darwin
#define plat_name_wasi __comp_str__plat_name_wasi

#define plat_based_unix __comp_bool__plat_based_unix
#define plat_is_posix __comp_bool__plat_is_posix

/* --- Data Model --- */

#define plat_data_model __comp_enum__plat_data_model
#define plat_data_model_unknown __comp_enum__plat_data_model_unknown
#define plat_data_model_lp64 __comp_enum__plat_data_model_lp64
#define plat_data_model_lp32 __comp_enum__plat_data_model_lp32
#define plat_data_model_ilp64 __comp_enum__plat_data_model_ilp64
#define plat_data_model_ilp32 __comp_enum__plat_data_model_ilp32
#define plat_data_model_llp64 __comp_enum__plat_data_model_llp64
/* Alias: for this project, `llp32` is kept as a naming handle for the
 * int=32, long=32, pointer=32 family that currently maps to `ilp32`. */
#define plat_data_model_llp32 plat_data_model_ilp32

#define plat_data_model_is_lp64 __comp_bool__plat_data_model_is_lp64
#define plat_data_model_is_lp32 __comp_bool__plat_data_model_is_lp32
#define plat_data_model_is_ilp64 __comp_bool__plat_data_model_is_ilp64
#define plat_data_model_is_ilp32 __comp_bool__plat_data_model_is_ilp32
#define plat_data_model_is_llp64 __comp_bool__plat_data_model_is_llp64
#define plat_data_model_is_llp32 plat_data_model_is_ilp32

#define plat_bits_unit_unknown __comp_enum__plat_bits_unit_unknown
#define plat_bits_unit_64bit __comp_enum__plat_bits_unit_64bit
#define plat_bits_unit_32bit __comp_enum__plat_bits_unit_32bit
#define plat_bits_unit_16bit __comp_enum__plat_bits_unit_16bit

#define plat_bits_unknown __comp_int__plat_bits_unknown
#define plat_bits_64 __comp_int__plat_bits_64
#define plat_bits_32 __comp_int__plat_bits_32
#define plat_bits_16 __comp_int__plat_bits_16

#define plat_int_unit __comp_enum__plat_int_unit
#define plat_long_unit __comp_enum__plat_long_unit
#define plat_ptr_unit __comp_enum__plat_ptr_unit

#define plat_int_bits __comp_int__plat_int_bits
#define plat_long_bits __comp_int__plat_long_bits
#define plat_ptr_bits __comp_int__plat_ptr_bits

#define plat_int_is_64bit __comp_bool__plat_int_is_64bit
#define plat_int_is_32bit __comp_bool__plat_int_is_32bit
#define plat_int_is_16bit __comp_bool__plat_int_is_16bit
#define plat_long_is_64bit __comp_bool__plat_long_is_64bit
#define plat_long_is_32bit __comp_bool__plat_long_is_32bit
#define plat_ptr_is_64bit __comp_bool__plat_ptr_is_64bit
#define plat_ptr_is_32bit __comp_bool__plat_ptr_is_32bit

#define plat_long_needs_distinct_int_cases __comp_bool__plat_long_needs_distinct_int_cases

/* --- Calling Conventions --- */

#define plat_callconv_cdecl __comp_attr__plat_callconv_cdecl
#define plat_callconv_stdcall __comp_attr__plat_callconv_stdcall
#define plat_callconv_fastcall __comp_attr__plat_callconv_fastcall
#define plat_callconv_vectorcall __comp_attr__plat_callconv_vectorcall
/// Fiber entry trampoline; no prologue/epilogue.
#define plat_callconv_naked __comp_attr__plat_callconv_naked
/// Fiber yield / context-switch boundary: callee does not preserve platform callee-saved GPRs.
#define plat_callconv_preserve_none __comp_attr__plat_callconv_preserve_none
#define plat_callconv_preserve_all __comp_attr__plat_callconv_preserve_all

/*========== Macros and Definitions =========================================*/

/* --- Platform Detection --- */

/* Default: unknown platform */
#define __comp_enum__plat_type plat_type_unknown
#define __comp_enum__plat_type_unknown 0
#define __comp_enum__plat_type_windows 1
#define __comp_enum__plat_type_linux 2
#define __comp_enum__plat_type_darwin 3
#define __comp_enum__plat_type_wasi 4

#define __comp_bool__plat_is_windows pp_Tok_eql(plat_type, plat_type_windows)
#define __comp_bool__plat_is_linux pp_Tok_eql(plat_type, plat_type_linux)
#define __comp_bool__plat_is_darwin pp_Tok_eql(plat_type, plat_type_darwin)
#define __comp_bool__plat_is_wasi pp_Tok_eql(plat_type, plat_type_wasi)

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

/* --- Data Model --- */

#define __comp_enum__plat_data_model plat_data_model_unknown
#define __comp_enum__plat_data_model_unknown 0
#define __comp_enum__plat_data_model_lp64 1
#define __comp_enum__plat_data_model_lp32 2
#define __comp_enum__plat_data_model_ilp64 3
#define __comp_enum__plat_data_model_ilp32 4
#define __comp_enum__plat_data_model_llp64 5

#if (__SIZEOF_LONG__ == 8) && (__SIZEOF_POINTER__ == 8)
#if (__SIZEOF_INT__ == 8)
#undef __comp_enum__plat_data_model
#define __comp_enum__plat_data_model plat_data_model_ilp64
#else
#undef __comp_enum__plat_data_model
#define __comp_enum__plat_data_model plat_data_model_lp64
#endif
#elif (__SIZEOF_LONG__ == 4) && (__SIZEOF_POINTER__ == 8)
#undef __comp_enum__plat_data_model
#define __comp_enum__plat_data_model plat_data_model_llp64
#elif (__SIZEOF_LONG__ == 4) && (__SIZEOF_POINTER__ == 4)
#if (__SIZEOF_INT__ == 2)
#undef __comp_enum__plat_data_model
#define __comp_enum__plat_data_model plat_data_model_lp32
#else
#undef __comp_enum__plat_data_model
#define __comp_enum__plat_data_model plat_data_model_ilp32
#endif
#endif

#define __comp_bool__plat_data_model_is_lp64 pp_Tok_eql(plat_data_model, plat_data_model_lp64)
#define __comp_bool__plat_data_model_is_lp32 pp_Tok_eql(plat_data_model, plat_data_model_lp32)
#define __comp_bool__plat_data_model_is_ilp32 pp_Tok_eql(plat_data_model, plat_data_model_ilp32)
#define __comp_bool__plat_data_model_is_ilp64 pp_Tok_eql(plat_data_model, plat_data_model_ilp64)
#define __comp_bool__plat_data_model_is_llp64 pp_Tok_eql(plat_data_model, plat_data_model_llp64)

#define __comp_enum__plat_bits_unit_unknown 0
#define __comp_enum__plat_bits_unit_64bit 1
#define __comp_enum__plat_bits_unit_32bit 2
#define __comp_enum__plat_bits_unit_16bit 3

#define __comp_int__plat_bits_unknown 0
#define __comp_int__plat_bits_64 64
#define __comp_int__plat_bits_32 32
#define __comp_int__plat_bits_16 16

#define __comp_enum__plat_int_unit pp_expand( \
    pp_switch_ pp_begin(plat_data_model)( \
        pp_case_((plat_data_model_lp32)(plat_bits_unit_16bit)), \
        pp_case_((plat_data_model_ilp64)(plat_bits_unit_64bit)), \
        pp_default_(plat_bits_unit_32bit) \
    ) pp_end \
)
#define __comp_enum__plat_long_unit pp_expand( \
    pp_switch_ pp_begin(plat_data_model)( \
        pp_case_((plat_data_model_lp64)(plat_bits_unit_64bit)), \
        pp_case_((plat_data_model_ilp64)(plat_bits_unit_64bit)), \
        pp_default_(plat_bits_unit_32bit) \
    ) pp_end \
)
#define __comp_enum__plat_ptr_unit pp_expand( \
    pp_switch_ pp_begin(plat_data_model)( \
        pp_case_((plat_data_model_lp32)(plat_bits_unit_32bit)), \
        pp_case_((plat_data_model_ilp32)(plat_bits_unit_32bit)), \
        pp_default_(plat_bits_unit_64bit) \
    ) pp_end \
)

#define __comp_int__plat_int_bits pp_expand( \
    pp_switch_ pp_begin(plat_int_unit)( \
        pp_case_((plat_bits_unit_64bit)(plat_bits_64)), \
        pp_case_((plat_bits_unit_32bit)(plat_bits_32)), \
        pp_case_((plat_bits_unit_16bit)(plat_bits_16)), \
        pp_default_(plat_bits_unknown) \
    ) pp_end \
)
#define __comp_int__plat_long_bits pp_expand( \
    pp_switch_ pp_begin(plat_long_unit)( \
        pp_case_((plat_bits_unit_64bit)(plat_bits_64)), \
        pp_case_((plat_bits_unit_32bit)(plat_bits_32)), \
        pp_default_(plat_bits_unknown) \
    ) pp_end \
)
#define __comp_int__plat_ptr_bits pp_expand( \
    pp_switch_ pp_begin(plat_ptr_unit)( \
        pp_case_((plat_bits_unit_64bit)(plat_bits_64)), \
        pp_case_((plat_bits_unit_32bit)(plat_bits_32)), \
        pp_default_(plat_bits_unknown) \
    ) pp_end \
)

#define __comp_bool__plat_int_is_64bit pp_Tok_eql(plat_int_unit, plat_bits_unit_64bit)
#define __comp_bool__plat_int_is_32bit pp_Tok_eql(plat_int_unit, plat_bits_unit_32bit)
#define __comp_bool__plat_int_is_16bit pp_Tok_eql(plat_int_unit, plat_bits_unit_16bit)
/* C requires `long` to provide at least 32 bits of range, so this layer only
 * models `long` as 32-bit or 64-bit and intentionally has no `*_long_is_16bit`. */
#define __comp_bool__plat_long_is_64bit pp_Tok_eql(plat_long_unit, plat_bits_unit_64bit)
#define __comp_bool__plat_long_is_32bit pp_Tok_eql(plat_long_unit, plat_bits_unit_32bit)
#define __comp_bool__plat_ptr_is_64bit pp_Tok_eql(plat_ptr_unit, plat_bits_unit_64bit)
#define __comp_bool__plat_ptr_is_32bit pp_Tok_eql(plat_ptr_unit, plat_bits_unit_32bit)

#define __comp_bool__plat_long_needs_distinct_int_cases \
    pp_or(pp_or(plat_data_model_is_llp64, plat_data_model_is_llp32), plat_is_darwin)

/* --- Calling Conventions --- */

#if plat_type == plat_type_windows
#define __comp_attr__plat_callconv_cdecl __cdecl
#define __comp_attr__plat_callconv_stdcall __stdcall
#define __comp_attr__plat_callconv_fastcall __fastcall
#define __comp_attr__plat_callconv_vectorcall __vectorcall
#else /* plat_type != plat_type_windows */
/* Linux/Unix typically ignores these or they are default */
#define __comp_attr__plat_callconv_cdecl
#define __comp_attr__plat_callconv_stdcall
#define __comp_attr__plat_callconv_fastcall
#define __comp_attr__plat_callconv_vectorcall
#endif

#define __comp_attr__plat_callconv_naked comp_naked
#define __comp_attr__plat_callconv_preserve_none comp_preserve_none
#define __comp_attr__plat_callconv_preserve_all comp_preserve_all

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_cfg_plat__included */
