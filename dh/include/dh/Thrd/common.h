/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-23 (date of creation)
 * @updated 2025-05-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/Thrd
 * @prefix  Thrd
 *
 * @brief   Common definitions for thread management
 * @details Defines common definitions for thread management.
 */

#ifndef THRD_COMMON_INCLUDED
#define THRD_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/Arr.h"
#include "dh/sli.h"
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/time/common.h"
#include "dh/mem/Allocator.h"

/*========== Macros and Definitions =========================================*/

// Thread ID type
typedef Thrd_IdImpl     Thrd_Id;
// Thread handle type
typedef Thrd_HandleImpl Thrd_Handle;

// Thread function context type
typedef struct Thrd_FnCtx {
    u8 data[];
} Thrd_FnCtx;
// Thread function return type
typedef struct Thrd_FnRet {
    u8 data[];
} Thrd_FnRet;
// Thread function type
typedef fn_((*Thrd_WorkFn)(Thrd_FnCtx* ctx), Thrd_FnRet*);
// Thread functions
extern fn_(Thrd_sleep(time_Duration duration), void);
extern fn_(Thrd_yield(void), Err$void) $must_check;
extern fn_(Thrd_getCurrentId(void), Thrd_Id);
extern fn_(Thrd_getCpuCount(void), Err$usize) $must_check;

// Thread type
typedef struct Thrd {
    Thrd_Handle handle;
} Thrd;
use_Err$(Thrd);
extern fn_(Thrd_getHandle(Thrd self), Thrd_Handle);

// Thread name buffer type
typedef Arr$$(Thrd_max_name_len, u8) Thrd_NameBuf;
use_Opt$(Thrd_NameBuf);
use_Opt$(Sli_const$u8);
use_Err$(Opt$Sli_const$u8);
extern fn_(Thrd_getName(Thrd self, Thrd_NameBuf* buf_ptr), Err$Opt$Sli_const$u8) $must_check;
extern fn_(Thrd_setName(Thrd self, Sli_const$u8 name), Err$void) $must_check;

// Thread spawn configuration
typedef struct Thrd_SpawnConfig {
    usize             stack_size;
    Opt$mem_Allocator allocator;
} Thrd_SpawnConfig;
#define Thrd_SpawnConfig_default_stack_size (16ull * 1024ull * 1024ull)
static const Thrd_SpawnConfig Thrd_SpawnConfig_default = {
    .stack_size = Thrd_SpawnConfig_default_stack_size,
    .allocator  = none()
};
extern fn_(Thrd_spawn(Thrd_SpawnConfig config, Thrd_WorkFn workFn, Thrd_FnCtx* ctx), Err$Thrd) $must_check;
extern fn_(Thrd_detach(Thrd self), void);
extern fn_(Thrd_join(Thrd self), Thrd_FnRet*);

// Mutex type
typedef struct Thrd_Mtx    Thrd_Mtx;
// Condition variable type
typedef struct Thrd_Cond   Thrd_Cond;
// Read-write lock type
typedef struct Thrd_RwLock Thrd_RwLock;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* THRD_COMMON_INCLUDED */
