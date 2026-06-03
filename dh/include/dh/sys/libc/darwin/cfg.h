/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-27 (date of creation)
 * @ingroup dasae-headers(dh)/sys/libc/darwin
 * @prefix  sys_libc_darwin
 */
#ifndef sys_libc_darwin_cfg__included
#define sys_libc_darwin_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "dh/builtin/pp.h"
#include "dh/builtin/cfg/plat.h"

#if plat_is_darwin
#if !defined(sys_libc_darwin_pref_target_ver)
#define sys_libc_darwin_pref_target_ver __comp_int__sys_libc_darwin_pref_target_ver
#endif /* !defined(sys_libc_darwin_pref_target_ver) */

#define sys_libc_darwin_ver_macos_10_12 101200
#define sys_libc_darwin_ver_macos_11_0 110000

#if sys_libc_darwin_pref_target_ver >= sys_libc_darwin_ver_macos_10_12
#define sys_libc_darwin_has_unfair_lock pp_true
#else
#define sys_libc_darwin_has_unfair_lock pp_false
#endif

#if sys_libc_darwin_pref_target_ver >= sys_libc_darwin_ver_macos_11_0
#define sys_libc_darwin_has_ulock_wait2 pp_true
#else
#define sys_libc_darwin_has_ulock_wait2 pp_false
#endif

#define __comp_int__sys_libc_darwin_pref_target_ver sys_libc_darwin_ver_macos_10_12
#endif /* plat_is_darwin */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_libc_darwin_cfg__included */
