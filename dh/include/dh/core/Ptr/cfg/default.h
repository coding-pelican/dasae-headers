/**
 * @file cfg/default.h
 * Default configuration settings
 */
#ifndef CORE_PTR_CFG_DEFAULT_INCLUDED
#define CORE_PTR_CFG_DEFAULT_INCLUDED (1)

/*========== Includes =======================================================*/

#include "dh/builtin/pltf_cfg.h"
#include "dh/debug/cfg.h"

/*========== Macros and Definitions =========================================*/

#if BUILTIN_PLTF_64BIT
#define CORE_PTR_DEFAULT_IMPL CORE_PTR_IMPL_BITFIELD
#endif

#if DEBUG_ENABLED
#define CORE_PTR_DEFAULT_DEBUG_CHECKS (1)
#define CORE_PTR_DEFAULT_DEBUG_TRACE  (1)
#endif

#endif // CORE_PTR_CFG_DEFAULT_INCLUDED
