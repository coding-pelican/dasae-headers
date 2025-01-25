/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-13 (date of creation)
 * @updated 2025-01-15 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/scope
 * @prefix  scope
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef SCOPE_COMMON_INCLUDED
#define SCOPE_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

#define scope_with(_Init_Statement...) \
    SYN__scope_with(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init_Statement)

#define scope_with_fini(_Init_Statement, _Fini_Statement...) \
    SYN__scope_with_fini(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init_Statement, _Fini_Statement)

#define scope_var(_Init_Statement...) \
    SYN__scope_var(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init_Statement)

#define scope_let(_Init_Statement...) \
    SYN__scope_let(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init_Statement)

#define scope_if(_Init_Statement, _Condition) \
    SYN__scope_if(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init_Statement, _Condition)

#define scope_else(_Init_Statement...) \
    SYN__scope_else(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init_Statement)

#define scope_switch(_Init_Statement, _Condition) \
    SYN__scope_switch(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init_Statement, _Condition)

#define scope_while(_Init_Statement, _Condition) \
    SYN__scope_while(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init_Statement, _Condition)

#define scope_va_list(_Init_Statement) \
    /* TODO: Implement scope_va_list */

/*========== Macros Implementation ==========================================*/

// NOLINTBEGIN
#define SYN__scope_with(_run_once, _init_once, _Init_Statement...)               \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (_Init_Statement; _init_once; _init_once = false)

#define SYN__scope_with_fini(_run_once, _init_once, _Init_Statement, _Fini_Statement...)          \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false, _Fini_Statement) \
        for (_Init_Statement; _init_once; _init_once = false)

#define SYN__scope_var(_run_once, _init_once, _Init_Statement...)                \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (var _Init_Statement; _init_once; _init_once = false)

#define SYN__scope_let(_run_once, _init_once, _Init_Statement...)                \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (let _Init_Statement; _init_once; _init_once = false)

#define SYN__scope_if(_run_once, _init_once, _Init_Statement, _Condition) \
    SYN__scope_with(_run_once, _init_once, _Init_Statement) if (_Condition)

#define SYN__scope_else(_run_once, _init_once, _Init_Statement...) \
    else SYN__scope_with(_run_once, _init_once, _Init_Statement)

#define SYN__scope_switch(_run_once, _init_once, _Init_Statement, _Condition) \
    SYN__scope_with(_run_once, _init_once, _Init_Statement) switch (_Condition)

#define SYN__scope_while(_run_once, _init_once, _Init_Statement, _Condition) \
    SYN__scope_with(_run_once, _init_once, _Init_Statement) while (_Condition)
// NOLINTEND

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SCOPE_COMMON_INCLUDED */
