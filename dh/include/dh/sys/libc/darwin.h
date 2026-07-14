/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    darwin.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-27 (date of creation)
 * @updated 2026-07-14 (date of last update)
 * @ingroup dasae-headers(dh)/sys/libc
 * @prefix  sys_libc_darwin
 */
#pragma once
#ifndef sys_libc_darwin__included
#define sys_libc_darwin__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "darwin/common.h"
#include "darwin/cfg.h"
#include "darwin/time.h"
#include "darwin/sync.h"
#include "darwin/signal.h"
#include "darwin/termios.h"
#include "darwin/mem.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_libc_darwin__included */
