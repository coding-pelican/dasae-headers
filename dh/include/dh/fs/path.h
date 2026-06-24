/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    path.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-25 (date of creation)
 * @updated 2026-06-20 (date of last update)
 * @ingroup dasae-headers(dh)/fs
 * @prefix  fs_path
 */
#pragma once
#ifndef fs_path__included
#define fs_path__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../mem/Alctr.h"

/*========== Macros and Declarations ========================================*/

errset_((fs_path_E)(
    fs_path_TooSmallBuffer,
    fs_path_BadSyntax,
    fs_path_TooLong
));

#define fs_path_seps __str__fs_path_seps
#define fs_path_sep_slash __str__fs_path_sep_slash
#define fs_path_sep_slash_byte __uint__fs_path_sep_slash_byte
#define fs_path_sep_backslash __str__fs_path_sep_backslash
#define fs_path_sep_backslash_byte __uint__fs_path_sep_backslash_byte

#define fs_path_sep __str__alias__fs_path_sep
#define fs_path_sep_native __str__fs_path_sep_native
#define fs_path_sep_native_byte __uint__fs_path_sep_native_byte
#define fs_path_sep_alt_native __opt_str__fs_path_sep_alt_native
#define fs_path_sep_alt_native_byte __opt_uint__fs_path_sep_alt_native_byte
#define fs_path_sep_windows __str__fs_path_sep_windows
#define fs_path_sep_windows_byte __uint__fs_path_sep_windows_byte
#define fs_path_sep_alt_windows __some_str__fs_path_sep_alt_windows
#define fs_path_sep_alt_windows_byte __some_uint__fs_path_sep_alt_windows_byte
#define fs_path_sep_unix __str__fs_path_sep_unix
#define fs_path_sep_unix_byte __uint__fs_path_sep_unix_byte
#define fs_path_sep_alt_unix __none_str__fs_path_sep_alt_unix
#define fs_path_sep_alt_unix_byte __none_uint__fs_path_sep_alt_unix_byte

$attr($must_check)
$extern fn_((fs_path_isAbs(S_const$u8 path))(bool));
$attr($must_check)
$extern fn_((fs_path_basename(S_const$u8 path))(S_const$u8));
$attr($must_check)
$extern fn_((fs_path_dirname(S_const$u8 path))(S_const$u8));
$attr($must_check)
$extern fn_((fs_path_extension(S_const$u8 path))(S_const$u8));
$attr($must_check)
$extern fn_((fs_path_stem(S_const$u8 path))(S_const$u8));

$attr($must_check)
$extern fn_((fs_path_join2(S_const$u8 lhs, S_const$u8 rhs, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_path_join2Alloc(S_const$u8 lhs, S_const$u8 rhs, mem_Alctr gpa))(E$S$u8));
$attr($must_check)
$extern fn_((fs_path_normalize(S_const$u8 path, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_path_normalizeAlloc(S_const$u8 path, mem_Alctr gpa))(E$S$u8));
$attr($must_check)
$extern fn_((fs_path_resolve(S_const$u8 base, S_const$u8 sub_path, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((fs_path_resolveAlloc(S_const$u8 base, S_const$u8 sub_path, mem_Alctr gpa))(E$S$u8));

/*========== Macros and Definitions =========================================*/

#define __str__fs_path_seps \
    fs_path_sep_slash fs_path_sep_backslash
#define __str__fs_path_sep_slash \
    "/"
#define __uint__fs_path_sep_slash_byte \
    u8_c('/')
#define __str__fs_path_sep_backslash \
    "\\"
#define __uint__fs_path_sep_backslash_byte \
    u8_c('\\')

#define __str__alias__fs_path_sep fs_path_sep_native
#define __str__fs_path_sep_native pp_if_(plat_is_windows)( \
    pp_then_(fs_path_sep_windows), \
    pp_else_(fs_path_sep_unix) \
)
#define __uint__fs_path_sep_native_byte pp_if_(plat_is_windows)( \
    pp_then_(fs_path_sep_windows_byte), \
    pp_else_(fs_path_sep_unix_byte) \
)
#define __str__fs_path_sep_alt_native pp_if_(plat_is_windows)( \
    pp_then_(fs_path_sep_windows), \
    pp_else_(fs_path_sep_unix) \
)
#define __uint__fs_path_sep_alt_native_byte pp_if_(plat_is_windows)( \
    pp_then_(fs_path_sep_windows_byte), \
    pp_else_(fs_path_sep_unix_byte) \
)
#define __str__fs_path_sep_windows \
    fs_path_sep_backslash
#define __uint__fs_path_sep_windows_byte \
    fs_path_sep_backslash_byte
#define __str__fs_path_sep_alt_windows \
    pp_some(fs_path_sep_slash)
#define __uint__fs_path_sep_alt_windows_byte \
    pp_some(fs_path_sep_slash_byte)
#define __str__fs_path_sep_unix \
    fs_path_sep_slash
#define __uint__fs_path_sep_unix_byte \
    fs_path_sep_slash_byte
#define __str__fs_path_sep_alt_unix \
    pp_none()
#define __uint__fs_path_sep_alt_unix_byte \
    pp_none()

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_path__included */
