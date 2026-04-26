/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    path.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-25 (date of creation)
 * @updated 2026-04-25 (date of last update)
 * @ingroup dasae-headers(dh)/fs
 * @prefix  fs_path
 */
#ifndef fs_path__included
#define fs_path__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "dh/mem/Alctr.h"

/*========== Macros and Declarations ========================================*/

errset_((fs_path_E)(
    BufferTooSmall,
    BadSyntax,
    PathTooLong
));

$static let_(fs_path_sep, u8) = u8_c(pp_if_(plat_is_windows)(pp_then_('\\'), pp_else_('/')));

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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_path__included */
