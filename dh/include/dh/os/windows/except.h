/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    except.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-27 (date of creation)
 * @updated 2026-04-27 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows exception handling APIs
 * @details Exposes vectored and structured exception handling entry points used
 *          by low-level runtime facilities.
 */
#ifndef os_windows_except__included
#define os_windows_except__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows
#include <errhandlingapi.h>
#endif /* plat_is_windows */
#include "pp-def/unguarded.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_except__included */
