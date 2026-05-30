/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Self.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-27 (date of creation)
 * @updated 2026-04-27 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd
 */
#ifndef thrd_self__included
#define thrd_self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "dh/mem/Alctr.h"
#include "../exec/common.h"

/*========== Macros and Declarations ========================================*/

errset_((thrd_E)(thrd_Unsupported, thrd_SystemResources));
T_useBy$(($spec(E, $set(thrd_E)))(usize, O$S_const$u8));

struct thrd_Self {
    var_(handle, thrd_Handle);
    var_(clsr, Clsr$raw*);
    var_(inner, P$raw);
};
$extern fn_((thrd_handle(thrd_Self self))(thrd_Handle));
$attr($must_check)
$extern fn_((thrd_yield(void))(thrd_E$void));

$extern fn_((thrd_currId(void))(thrd_Id));
$attr($must_check)
$extern fn_((thrd_cpuCount(void))(thrd_E$usize));

$attr($must_check)
$extern fn_((thrd_getName(thrd_Self self, thrd_NameBuf* buf_ptr))(thrd_E$O$S_const$u8));
$attr($must_check)
$extern fn_((thrd_setName(thrd_Self self, S_const$u8 name))(thrd_E$void));

typedef struct thrd_SpawnCfg {
    var_(gpa, O$mem_Alctr);
    var_(stack_size, usize);
} thrd_SpawnCfg;
#define thrd_SpawnCfg_default_stack_size (usize_(16) * 1024 * 1024)
static const thrd_SpawnCfg thrd_SpawnCfg_default = {
    .gpa = none(),
    .stack_size = thrd_SpawnCfg_default_stack_size,
};
errset_((thrd_spawn_E)() $union_errset_(thrd_E, mem_E));
T_use_E$($set(thrd_spawn_E)(thrd_Self));
$attr($must_check)
$extern fn_((thrd_spawn(thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type))(thrd_spawn_E$thrd_Self));
#define T_use_thrd_spawn$(_T...) __stmt__T_use_thrd_spawn$(_T)
$extern fn_((thrd_detach(thrd_Self self))(void));
$extern fn_((thrd_join(thrd_Self self))(Clsr$raw*));
#define T_use_thrd_join$(_T...) __stmt__T_use_thrd_join$(_T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_self__included */
