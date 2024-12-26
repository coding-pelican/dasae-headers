/**
 * @file cfg/template.h
 * Configuration template for customization
 */
#ifndef CORE_PTR_CFG_TEMPLATE_INCLUDED
#define CORE_PTR_CFG_TEMPLATE_INCLUDED (1)

/*========== Includes =======================================================*/

#include "dh/builtin/pltf_cfg.h"
#include "dh/debug/cfg.h"

/*========== Macros and Definitions =========================================*/

// Select Ptr implementation for 64-bit systems
// Options: CORE_PTR_IMPL_BITFIELD, CORE_PTR_IMPL_DOUBLE
// #define CORE_PTR_IMPL CORE_PTR_IMPL_BITFIELD

// #define CORE_PTR_DEBUG_CHECKS (1)
// #define CORE_PTR_DEBUG_TRACE  (1)

#endif // CORE_PTR_CFG_TEMPLATE_INCLUDED
