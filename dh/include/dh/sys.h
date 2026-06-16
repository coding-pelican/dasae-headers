/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    sys.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-22 (date of creation)
 * @updated 2026-05-22 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  sys
 */
#pragma once
#ifndef sys__included
#define sys__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "sys/fw.h"
#include "sys/call.h"
#include "sys/libc.h"
#include "sys/api.h"
#include "sys/hosted.h"

#include "sys/posix.h"
#include "sys/win32.h"
#include "sys/wasi.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys__included */
