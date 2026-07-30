/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Preopens.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc_Preopens
 *
 * @brief   Borrowed named resources inherited at process startup
 */
#pragma once
#ifndef proc_Preopens__included
#define proc_Preopens__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../fs/Dir.h"
#include "../fs/File/self.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Preopens_Resrc)(variant_((proc_Preopen_Resrc $fits($packed))(
    (proc_Preopens_Resrc_file, fs_File),
    (proc_Preopens_Resrc_dir, fs_Dir)
))));

T_alias$((proc_Preopens)(Void));
T_use_prl$(proc_Preopens);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Preopens__included */
