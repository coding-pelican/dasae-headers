/**
 * @file cfg.h
 * Configuration and platform settings
 */
#ifndef CORE_PTR_CFG_INCLUDED
#define CORE_PTR_CFG_INCLUDED (1)

/*========== Includes =======================================================*/

/* Include default configuration first */
#include "cfg/default.h"

/* Include custom configuration if it exists */
#if __has_include("dh/core/ptr/cfg/custom.h")
#include "dh/core/ptr/cfg/custom.h"
#endif

/*========== Macros and Definitions =========================================*/

#define CORE_PTR_IMPL_BITFIELD (1) // Use bitfield in pointer
#define CORE_PTR_IMPL_DOUBLE   (2) // Use double-width structure

#if !defined(CORE_PTR_IMPL)
#define CORE_PTR_IMPL CORE_PTR_DEFAULT_IMPL
#endif

#if !defined(CORE_PTR_DEBUG_CHECKS)
#define CORE_PTR_DEBUG_CHECKS CORE_PTR_DEFAULT_DEBUG_CHECKS
#endif

#if !defined(CORE_PTR_DEBUG_TRACE)
#define CORE_PTR_DEBUG_TRACE CORE_PTR_DEFAULT_DEBUG_TRACE
#endif

/*========== Configuration Validation =======================================*/

#ifdef BUILTIN_PLTF_64BIT
#if CORE_PTR_IMPL != CORE_PTR_IMPL_BITFIELD && CORE_PTR_IMPL != CORE_PTR_IMPL_DOUBLE
#error "Invalid CORE_PTR_IMPL selection for 64-bit"
#endif
#endif

#endif // CORE_PTR_CFG_INCLUDED
