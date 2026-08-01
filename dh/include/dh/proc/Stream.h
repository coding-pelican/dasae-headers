/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Stream.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc
 *
 * @brief   Standard-stream disposition for a spawned child process
 */
#pragma once
#ifndef proc_Stream__included
#define proc_Stream__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../fs/File.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Stream)(variant_((proc_Stream $fits($packed))(
    (proc_Stream_inherit, Void),
    (proc_Stream_file, fs_File),
    (proc_Stream_ignore, Void),
    (proc_Stream_pipe, Void),
    (proc_Stream_close, Void)
))));
$attr($inline_always)
$static fn_((proc_Stream_fromFile(fs_File file))(proc_Stream));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_Stream_fromFile(fs_File file))(proc_Stream)) {
    return union_of$((proc_Stream)(proc_Stream_file)(file));
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Stream__included */
