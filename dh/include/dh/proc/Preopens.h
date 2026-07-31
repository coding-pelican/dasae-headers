/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Preopens.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc_Preopens
 *
 * @brief   Concrete named resources inherited by the current process
 */
#pragma once
#ifndef proc_Preopens__included
#define proc_Preopens__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "std.h"
#include "../fs/Dir.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Preopens_Resrc)(variant_((proc_Preopens_Resrc $fits($packed))(
    (proc_Preopens_Resrc_file, fs_File),
    (proc_Preopens_Resrc_dir, fs_Dir)
))));
T_use_O$(proc_Preopens_Resrc);

/// Platform preopened-resource catalog.
///
/// Windows and POSIX derive only the three standard streams and therefore
/// carry no storage. WASI can extend this concrete type with its descriptor map.
T_alias$((proc_Preopens)(struct proc_Preopens {}));
T_use_prl$(proc_Preopens);
$extern fn_((proc_Preopens_direct(void))(proc_Preopens));
$extern fn_((proc_Preopens_by(
    proc_Preopens self,
    S_const$u8 name
))(O$proc_Preopens_Resrc));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Preopens__included */
