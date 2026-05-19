/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-16 (date of creation)
 * @updated 2026-05-16 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/fs
 * @prefix  fs
 */
#ifndef fs_base__included
#define fs_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/posix.h"

/*========== Macros and Declarations ========================================*/

typedef posix_fd_t fs_Handle;
typedef u64 fs_INode;
typedef u64 fs_NLink;
typedef posix_mode_t fs_Mode;
typedef u32 fs_BlockSize;

typedef struct fs_Dir fs_Dir;
T_decl_E$(fs_Dir);

typedef struct fs_File fs_File;
T_decl_E$(fs_File);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_base__included */
