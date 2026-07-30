/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ver.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-25 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dal-project/da/foundation/cfg
 * @prefix  ver
 *
 * @brief   dh DSL semantic-version configuration and self-version detection.
 * @details Defines the compile-time version representation used by dh and
 *          derives dh's own version from raw build-provided constants. Build
 *          tools only provide those constants; this header owns all defaults,
 *          detection, packing, and string composition.
 */
#pragma once
#ifndef foundation_cfg_ver__included
#define foundation_cfg_ver__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../pp.h"

/*========== Macros and Declarations ========================================*/

/*--- Self-version Queries ---*/

#define ver_self_core_sep dh__str__ver_core_sep
#define ver_self_core_major dh__num__ver_core_major
#define ver_self_core_minor dh__num__ver_core_minor
#define ver_self_core_patch dh__num__ver_core_patch
#define ver_self_core_num dh__val__ver_core_num
#define ver_self_core_str dh__str__ver_core_str

#define ver_self_label_delim dh__str__ver_label_delim
#define ver_self_label_prefix_as_num dh__num__ver_label_prefix_as_num
#define ver_self_label_prefix_as_str dh__str__ver_label_prefix_as_str
#define ver_self_label_sep dh__str__ver_label_sep
#define ver_self_label_suffix_as_num dh__num__ver_label_suffix_as_num
#define ver_self_label_suffix_as_str dh__str__ver_label_suffix_as_str

#define ver_self_build_delim dh__str__ver_build_delim
#define ver_self_build_as_str dh__str__ver_build_as_str
#define ver_self_num dh__val__ver_num
#define ver_self_str dh__str__ver_str
#define ver_self_str_with_build dh__str__ver_str_with_build

/*--- Version Utilities ---*/

#define ver_core_calc(_major, _minor, _patch...) \
    __val__ver_core_calc(_major, _minor, _patch)
#define ver_calc(_major, _minor, _patch, _label_prefix_as_num, _label_suffix_as_num...) \
    __val__ver_calc(_major, _minor, _patch, _label_prefix_as_num, _label_suffix_as_num)

#define ver_core_strfy(_major, _minor, _patch, _core_sep...) \
    __str__ver_core_strfy(_major, _minor, _patch, _core_sep)
#define ver_strfy( \
    _major, _minor, _patch, _core_sep, \
    _label_delim, _label_prefix_as_str, _label_sep, _label_suffix_as_str... \
) \
    __str__ver_strfy( \
        _major, _minor, _patch, _core_sep, \
        _label_delim, _label_prefix_as_str, _label_sep, _label_suffix_as_str \
    )
#define ver_strfyWithBuild( \
    _major, _minor, _patch, _core_sep, \
    _label_delim, _label_prefix_as_str, _label_sep, _label_suffix_as_str, \
    _build_delim, _build_as_str... \
) \
    __str__ver_strfyWithBuild( \
        _major, _minor, _patch, _core_sep, \
        _label_delim, _label_prefix_as_str, _label_sep, _label_suffix_as_str, \
        _build_delim, _build_as_str \
    )

#define ver_core_sep_default ver_core_sep_some
#define ver_core_sep_some "."
#define ver_core_major_default ver_core_major_none
#define ver_core_major_none 0
#define ver_core_minor_default ver_core_minor_none
#define ver_core_minor_none 0
#define ver_core_patch_default ver_core_patch_none
#define ver_core_patch_none 0

#define ver_label_delim_default ver_label_delim_none
#define ver_label_delim_none ""
#define ver_label_delim_some "-"
#define ver_label_prefix_as_num_default ver_label_prefix_as_num_none
#define ver_label_prefix_as_num_none ver_label_prefix_as_num_release
#define ver_label_prefix_as_num_alpha 0
#define ver_label_prefix_as_num_beta 1
#define ver_label_prefix_as_num_rc 2
#define ver_label_prefix_as_num_release 3
#define ver_label_prefix_as_str_default ver_label_prefix_as_str_none
#define ver_label_prefix_as_str_none ver_label_prefix_as_str_release
#define ver_label_prefix_as_str_alpha "alpha"
#define ver_label_prefix_as_str_beta "beta"
#define ver_label_prefix_as_str_rc "rc"
#define ver_label_prefix_as_str_release ""
#define ver_label_sep_default ver_label_sep_none
#define ver_label_sep_none ""
#define ver_label_sep_some "."
#define ver_label_suffix_as_num_default ver_label_suffix_as_num_none
#define ver_label_suffix_as_num_none 0
#define ver_label_suffix_as_str_default ver_label_suffix_as_str_none
#define ver_label_suffix_as_str_none ""

#define ver_build_delim_default ver_build_delim_none
#define ver_build_delim_none ""
#define ver_build_delim_some "+"
#define ver_build_as_str_default ver_build_as_str_none
#define ver_build_as_str_none ""

/*========== Macros and Definitions =========================================*/

/*--- Self-version Queries ---*/

/* Raw build contract: dh__NUM__VER_* / dh__STR__VER_*. */
#define dh__str__ver_core_sep ver_core_sep_default
#define dh__num__ver_core_major ver_core_major_default
#if defined(dh__NUM__VER_CORE_MAJOR)
#undef dh__num__ver_core_major
#define dh__num__ver_core_major dh__NUM__VER_CORE_MAJOR
#endif /* defined(dh__NUM__VER_CORE_MAJOR) */
#define dh__num__ver_core_minor ver_core_minor_default
#if defined(dh__NUM__VER_CORE_MINOR)
#undef dh__num__ver_core_minor
#define dh__num__ver_core_minor dh__NUM__VER_CORE_MINOR
#endif /* defined(dh__NUM__VER_CORE_MINOR) */
#define dh__num__ver_core_patch ver_core_patch_default
#if defined(dh__NUM__VER_CORE_PATCH)
#undef dh__num__ver_core_patch
#define dh__num__ver_core_patch dh__NUM__VER_CORE_PATCH
#endif /* defined(dh__NUM__VER_CORE_PATCH) */

#define dh__str__ver_label_delim ver_label_delim_default
#define dh__num__ver_label_prefix_as_num ver_label_prefix_as_num_default
#define dh__str__ver_label_prefix_as_str ver_label_prefix_as_str_default
#if defined(dh__NUM__VER_LABEL_PREFIX) && defined(dh__STR__VER_LABEL_PREFIX)
#undef dh__str__ver_label_delim
#define dh__str__ver_label_delim ver_label_delim_some
#undef dh__num__ver_label_prefix_as_num
#define dh__num__ver_label_prefix_as_num dh__NUM__VER_LABEL_PREFIX
#undef dh__str__ver_label_prefix_as_str
#define dh__str__ver_label_prefix_as_str dh__STR__VER_LABEL_PREFIX
#endif /* defined(dh__NUM__VER_LABEL_PREFIX) && defined(dh__STR__VER_LABEL_PREFIX) */

#define dh__str__ver_label_sep ver_label_sep_default
#define dh__num__ver_label_suffix_as_num ver_label_suffix_as_num_default
#define dh__str__ver_label_suffix_as_str ver_label_suffix_as_str_default
#if defined(dh__NUM__VER_LABEL_SUFFIX) && defined(dh__STR__VER_LABEL_SUFFIX)
#undef dh__str__ver_label_sep
#define dh__str__ver_label_sep ver_label_sep_some
#undef dh__num__ver_label_suffix_as_num
#define dh__num__ver_label_suffix_as_num dh__NUM__VER_LABEL_SUFFIX
#undef dh__str__ver_label_suffix_as_str
#define dh__str__ver_label_suffix_as_str dh__STR__VER_LABEL_SUFFIX
#endif /* defined(dh__NUM__VER_LABEL_SUFFIX) && defined(dh__STR__VER_LABEL_SUFFIX) */

#define dh__str__ver_build_delim ver_build_delim_default
#define dh__str__ver_build_as_str ver_build_as_str_default
#if defined(dh__STR__VER_BUILD)
#undef dh__str__ver_build_delim
#define dh__str__ver_build_delim ver_build_delim_some
#undef dh__str__ver_build_as_str
#define dh__str__ver_build_as_str dh__STR__VER_BUILD
#endif /* defined(dh__STR__VER_BUILD) */

#define dh__val__ver_core_num ver_core_calc(ver_self_core_major, ver_self_core_minor, ver_self_core_patch)
#define dh__str__ver_core_str ver_core_strfy(ver_self_core_major, ver_self_core_minor, ver_self_core_patch, ver_self_core_sep)
#define dh__val__ver_num ver_calc( \
    ver_self_core_major, ver_self_core_minor, ver_self_core_patch, \
    ver_self_label_prefix_as_num, ver_self_label_suffix_as_num \
)
#define dh__str__ver_str ver_strfy( \
    ver_self_core_major, ver_self_core_minor, ver_self_core_patch, ver_self_core_sep, \
    ver_self_label_delim, ver_self_label_prefix_as_str, ver_self_label_sep, ver_self_label_suffix_as_str \
)
#define dh__str__ver_str_with_build ver_strfyWithBuild( \
    ver_self_core_major, ver_self_core_minor, ver_self_core_patch, ver_self_core_sep, \
    ver_self_label_delim, ver_self_label_prefix_as_str, ver_self_label_sep, ver_self_label_suffix_as_str, \
    ver_self_build_delim, ver_self_build_as_str \
)

/*--- Version Utilities ---*/

#define __val__ver_core_calc(_major, _minor, _patch...) ( \
    (((_major) & 0xFFUL) << 24u) \
    | (((_minor) & 0xFFUL) << 16u) \
    | (((_patch) & 0xFFUL) << 8u) \
)
#define __val__ver_calc(_major, _minor, _patch, _label_prefix_as_num, _label_suffix_as_num...) ( \
    __val__ver_core_calc(_major, _minor, _patch) \
    | (((_label_prefix_as_num) & 0x03UL) << 6u) \
    | (((_label_suffix_as_num) & 0x3FUL) << 0u) \
)

#define __str__ver_core_strfy(_major, _minor, _patch, _core_sep...) pp_expand( \
    pp_strfy(_major) _core_sep pp_strfy(_minor) _core_sep pp_strfy(_patch) \
)
#define __str__ver_strfy( \
    _major, _minor, _patch, _core_sep, \
    _label_delim, _label_prefix_as_str, _label_sep, _label_suffix_as_str... \
) \
    pp_expand( \
        ver_core_strfy(_major, _minor, _patch, _core_sep) \
            _label_delim _label_prefix_as_str _label_sep _label_suffix_as_str \
    )
#define __str__ver_strfyWithBuild( \
    _major, _minor, _patch, _core_sep, \
    _label_delim, _label_prefix_as_str, _label_sep, _label_suffix_as_str, \
    _build_delim, _build_as_str... \
) \
    pp_expand(ver_strfy( \
        _major, _minor, _patch, _core_sep, \
        _label_delim, _label_prefix_as_str, _label_sep, _label_suffix_as_str \
    ) _build_delim _build_as_str)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_cfg_ver__included */
