/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Self.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-17 (date of creation)
 * @updated 2026-05-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd
 */
#pragma once
#ifndef thrd_Self__included
#define thrd_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../clsr.h"
#include "../mem/Alctr.h"

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
    var_(gpa, mem_Alctr);
    var_(stack_size, usize);
} thrd_SpawnCfg;
#define thrd_SpawnCfg_default_stack_size (usize_(16) * 1024 * 1024)
$attr($inline_always)
$static fn_((thrd_SpawnCfg_default(mem_Alctr gpa))(thrd_SpawnCfg));
errset_((thrd_spawn_E)() $union_errset_(thrd_E, mem_E));
T_use_E$($set(thrd_spawn_E)(thrd_Self));
$attr($must_check)
$extern fn_((thrd_spawn(
    thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type
))(thrd_spawn_E$thrd_Self));
#define T_use_thrd_spawn$(_T...) __stmt__T_use_thrd_spawn$(_T)
$attr($must_check)
$extern fn_((thrd_spawnOwned(
    thrd_SpawnCfg cfg, Clsr$raw* clsr, TypeInfo ret_type, u_P$raw owned_clsr
))(thrd_spawn_E$thrd_Self));
#define T_use_thrd_spawnOwned$(_T...) __stmt__T_use_thrd_spawnOwned$(_T)
$extern fn_((thrd_detach(thrd_Self self))(void));
$extern fn_((thrd_join(thrd_Self self))(Clsr$raw*));
#define T_use_thrd_join$(_T...) __stmt__T_use_thrd_join$(_T)

/*========== Macros and Definitions =========================================*/

#define __stmt__T_use_thrd_spawn$(_T...) \
    $attr($inline_always $static $must_check) \
    fn_((tpl$(thrd_spawn, _T)(thrd_SpawnCfg cfg, P$$(Clsr$(_T)) clsr))(thrd_spawn_E$thrd_Self)) { \
        return thrd_spawn(cfg, clsr->as_raw, typeInfo$(_T)); \
    }
#define __stmt__T_use_thrd_spawnOwned$(_T...) \
    $attr($inline_always $static $must_check) \
    fn_((tpl$(thrd_spawnOwned, _T)(thrd_SpawnCfg cfg, P$$(Clsr$(_T)) clsr, u_P$raw owned_clsr))(thrd_spawn_E$thrd_Self)) { \
        return thrd_spawnOwned(cfg, clsr->as_raw, typeInfo$(_T), owned_clsr); \
    }
#define __stmt__T_use_thrd_join$(_T...) \
    $attr($inline_always $static) \
    fn_((tpl$(thrd_join, _T)(thrd_Self self))(P$$(Clsr$(_T)))) { \
        return as$(Clsr$(_T)*)(thrd_join(self)); \
    }

#if on_analysis_active_only || on_comptime
fn_((thrd_SpawnCfg_default(mem_Alctr gpa))(thrd_SpawnCfg)) {
    return (thrd_SpawnCfg){
        .gpa = mem_Alctr_ensureValid(gpa),
        .stack_size = thrd_SpawnCfg_default_stack_size,
    };
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Self__included */
