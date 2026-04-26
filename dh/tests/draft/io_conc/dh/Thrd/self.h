/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    self.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-04-27 (date of creation)
 * @updated 2026-04-27 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd
 */
#ifndef Thrd_self__included
#define Thrd_self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "dh/mem/Alctr.h"
#include "../exec/common.h"

/*========== Macros and Declarations ========================================*/

struct Thrd {
    var_(handle, Thrd_Handle);
    var_(closure, Closure$raw*);
    var_(inner, P$raw);
};
T_impl_E$(Thrd);
$extern fn_((Thrd_handle(Thrd self))(Thrd_Handle));
$attr($must_check)
$extern fn_((Thrd_yield(void))(E$void));

$extern fn_((Thrd_currId(void))(Thrd_Id));
$attr($must_check)
$extern fn_((Thrd_cpuCount(void))(E$usize));

$attr($must_check)
$extern fn_((Thrd_getName(Thrd self, Thrd_NameBuf* buf_ptr))(E$O$S_const$u8));
$attr($must_check)
$extern fn_((Thrd_setName(Thrd self, S_const$u8 name))(E$void));

typedef struct Thrd_SpawnCfg {
    var_(gpa, O$mem_Alctr);
    var_(stack_size, usize);
} Thrd_SpawnCfg;
#define Thrd_SpawnCfg_default_stack_size (usize_(16) * 1024 * 1024)
static const Thrd_SpawnCfg Thrd_SpawnCfg_default = {
    .gpa = none(),
    .stack_size = Thrd_SpawnCfg_default_stack_size,
};
$attr($must_check)
$extern fn_((Thrd_spawn(Thrd_SpawnCfg cfg, Closure$raw* closure, TypeInfo ret_type))(E$Thrd));
#define T_use_Thrd_spawn$(_T...) __stmt__T_use_Thrd_spawn$(_T)
$extern fn_((Thrd_detach(Thrd self))(void));
$extern fn_((Thrd_join(Thrd self))(Closure$raw*));
#define T_use_Thrd_join$(_T...) __stmt__T_use_Thrd_join$(_T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Thrd_self__included */
