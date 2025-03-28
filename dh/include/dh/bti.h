/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    bti.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)
 * @prefix  bti
 *
 * @brief   Built-in configuration and compiler-specific macros
 * @details Core configuration module that provides architecture, platform,
 *          compiler, and language specific configurations. Serves as the
 *          central point for all built-in definitions and compiler-related
 *          functionality used throughout the project.
 */

#ifndef BTI_INCLUDED
#define BTI_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

/* Core Configuration Layer =================================================*/

#include "bti/common.h" /* Common macros and utilities */
#include "bti/arch.h"   /* Architecture detection and configuration */
#include "bti/plat.h"   /* Platform/OS specific definitions */
#include "bti/lang.h"   /* C/C++ language configuration */

/* Compiler Layer ===========================================================*/

#include "bti/comp_cfg.h"  /* Compiler detection and configuration */
#include "bti/comp_attr.h" /* Compiler-specific attributes */
#include "bti/comp_ops.h"  /* Compiler built-in operations */

/* Feature Support Layer ====================================================*/

#include "bti/type.h"   /* Type information and traits */
#include "bti/field.h"  /* Field relationship utilities */
#include "bti/mem.h"    /* Memory operations and management */
#include "bti/SrcLoc.h" /* Source location utilities */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_INCLUDED */
