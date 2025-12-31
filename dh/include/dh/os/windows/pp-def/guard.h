/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    guard.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-30 (date of creation)
 * @updated 2025-12-30 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows/pp-def
 * @prefix  (none)
 */
#include "dh/builtin/plat_cfg.h"
#if plat_is_windows
#undef auto
#undef var
#undef let
#endif /* plat_is_windows */
