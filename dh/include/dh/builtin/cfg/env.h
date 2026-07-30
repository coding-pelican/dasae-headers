/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    env.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)/builtin/cfg
 * @prefix  env
 *
 * @brief   Translation and link environment facts
 * @details Describes the environment selected for the current translation:
 *          hosted/freestanding mode and the runtime components supplied by the
 *          build. Compiler identity and compiler syntax remain in `comp.h`.
 */
#pragma once
#ifndef foundation_cfg_env__included
#define foundation_cfg_env__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp.h"

/*========== Macros and Declarations ========================================*/

#define env_type __comp_enum__env_type
#define env_type_unknown __comp_enum__env_type_unknown
#define env_type_hosted __comp_enum__env_type_hosted
#define env_type_freestanding __comp_enum__env_type_freestanding

#define env_is_hosted __comp_bool__env_is_hosted
#define env_is_freestanding __comp_bool__env_is_freestanding

#define env_start_files_linked __comp_bool__env_start_files_linked
#define env_crt_linked __comp_bool__env_crt_linked
#define env_default_libs_linked __comp_bool__env_default_libs_linked
#define env_compiler_rt_linked __comp_bool__env_compiler_rt_linked
#define env_libc_linked __comp_bool__env_libc_linked
#define env_stdlib_linked __comp_bool__env_stdlib_linked

/*========== Macros and Definitions =========================================*/

#define __comp_enum__env_type_unknown 0
#define __comp_enum__env_type_hosted 1
#define __comp_enum__env_type_freestanding 2

#if defined(__STDC_HOSTED__) && (__STDC_HOSTED__ + 0) == 0
#define __comp_enum__env_type env_type_freestanding
#else
#define __comp_enum__env_type env_type_hosted
#endif

#if defined(COMP_FREESTANDING)
#undef __comp_enum__env_type
#define __comp_enum__env_type env_type_freestanding
#elif defined(COMP_HOSTED)
#undef __comp_enum__env_type
#define __comp_enum__env_type env_type_hosted
#endif

#if defined(COMP_HAS_START_FILES)
#define __comp_flag__env_start_files_linked 1
#elif defined(COMP_NO_START_FILES)
#define __comp_flag__env_start_files_linked 0
#else
#define __comp_flag__env_start_files_linked 1
#endif

#if defined(COMP_HAS_CRT)
#define __comp_flag__env_crt_linked 1
#elif defined(COMP_NO_CRT)
#define __comp_flag__env_crt_linked 0
#else
#define __comp_flag__env_crt_linked env_start_files_linked
#endif

#if defined(COMP_HAS_DEFAULT_LIBS)
#define __comp_flag__env_default_libs_linked 1
#elif defined(COMP_NO_DEFAULT_LIBS)
#define __comp_flag__env_default_libs_linked 0
#else
#define __comp_flag__env_default_libs_linked 1
#endif

#if defined(COMP_HAS_COMPILER_RT)
#define __comp_flag__env_compiler_rt_linked 1
#elif defined(COMP_NO_COMPILER_RT) || defined(COMP_NO_DEFAULT_LIBS)
#define __comp_flag__env_compiler_rt_linked 0
#else
#define __comp_flag__env_compiler_rt_linked 1
#endif

#if defined(COMP_HAS_LIBC)
#define __comp_flag__env_libc_linked 1
#elif defined(COMP_NO_LIBC) || defined(COMP_NO_DEFAULT_LIBS) || (env_type == env_type_freestanding)
#define __comp_flag__env_libc_linked 0
#else
#define __comp_flag__env_libc_linked 1
#endif

#if defined(COMP_HAS_STDLIB)
#define __comp_flag__env_stdlib_linked 1
#elif defined(COMP_NO_STDLIB)
#define __comp_flag__env_stdlib_linked 0
#else
#define __comp_flag__env_stdlib_linked (env_start_files_linked && env_default_libs_linked)
#endif

#define __comp_bool__env_is_hosted pp_Tok_eql(env_type, env_type_hosted)
#define __comp_bool__env_is_freestanding pp_Tok_eql(env_type, env_type_freestanding)

#define __comp_bool__env_start_files_linked __comp_flag__env_start_files_linked
#define __comp_bool__env_crt_linked __comp_flag__env_crt_linked
#define __comp_bool__env_default_libs_linked __comp_flag__env_default_libs_linked
#define __comp_bool__env_compiler_rt_linked __comp_flag__env_compiler_rt_linked
#define __comp_bool__env_libc_linked __comp_flag__env_libc_linked
#define __comp_bool__env_stdlib_linked __comp_flag__env_stdlib_linked

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_cfg_env__included */
