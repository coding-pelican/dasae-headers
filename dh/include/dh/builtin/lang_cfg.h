/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    lang_cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2025-01-22 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  lang
 *
 * @brief   Language standards detection configuration
 * @details Detects C/C++ language standards, features, and compilation modes.
 *          Includes standard versions, language extensions, and dialect support.
 */
#ifndef builtin_lang_cfg__included
#define builtin_lang_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pp.h"

/*========== Macros and Declarations ========================================*/

/* Language Mode */

#define lang_mode __comp_enum__lang_mode
#define lang_mode_unknown __comp_enum__lang_mode_unknown
#define lang_mode_c __comp_enum__lang_mode_c
#define lang_mode_cpp __comp_enum__lang_mode_cpp

/* Language Name */

#define lang_name __comp_str__lang_name
#define lang_name_unknown __comp_str__lang_name_unknown
#define lang_name_c __comp_str__lang_name_c
#define lang_name_cpp __comp_str__lang_name_cpp

/* Language Version */

#define lang_version __comp_int__lang_version
#define lang_version_unknown __comp_int__lang_version_unknown

#define lang_version_c __comp_int__lang_version_c
#define lang_version_c90 __comp_int__lang_version_c90
#define lang_version_c99 __comp_int__lang_version_c99
#define lang_version_c11 __comp_int__lang_version_c11
#define lang_version_c17 __comp_int__lang_version_c17
#define lang_version_c23 __comp_int__lang_version_c23

#define lang_version_cpp __comp_int__lang_version_cpp
#define lang_version_cpp98 __comp_int__lang_version_cpp98
#define lang_version_cpp11 __comp_int__lang_version_cpp11
#define lang_version_cpp14 __comp_int__lang_version_cpp14
#define lang_version_cpp17 __comp_int__lang_version_cpp17
#define lang_version_cpp20 __comp_int__lang_version_cpp20
#define lang_version_cpp23 __comp_int__lang_version_cpp23

#define lang_version_in_c90 __comp_bool__lang_version_in_c90
#define lang_version_in_c99 __comp_bool__lang_version_in_c99
#define lang_version_in_c11 __comp_bool__lang_version_in_c11
#define lang_version_in_c17 __comp_bool__lang_version_in_c17
#define lang_version_in_c23 __comp_bool__lang_version_in_c23

#define lang_version_in_cpp98 __comp_bool__lang_version_in_cpp98
#define lang_version_in_cpp11 __comp_bool__lang_version_in_cpp11
#define lang_version_in_cpp14 __comp_bool__lang_version_in_cpp14
#define lang_version_in_cpp17 __comp_bool__lang_version_in_cpp17
#define lang_version_in_cpp20 __comp_bool__lang_version_in_cpp20
#define lang_version_in_cpp23 __comp_bool__lang_version_in_cpp23

/* Language Strict Standards Conformance */

#define lang_is_enabled_strict __comp_bool__lang_is_enabled_strict

/* Language Feature Extensions */

#define lang_has_extensions __comp_bool__lang_has_extensions
#define lang_has_extensions_gnu __comp_bool__lang_has_extensions_gnu
#define lang_has_extensions_ms __comp_bool__lang_has_extensions_ms

/*========== Macros and Implementations =====================================*/

/* Default Values */

#define __comp_enum__lang_mode lang_mode_unknown
#define __comp_enum__lang_mode_unknown 0
#define __comp_enum__lang_mode_c 1
#define __comp_enum__lang_mode_cpp 2

#define __comp_str__lang_name pp_expand( \
    pp_switch_ pp_begin(lang_mode)( \
        pp_case_((lang_mode_c)(lang_name_c)), \
        pp_case_((lang_mode_cpp)(lang_name_cpp)), \
        pp_default_(lang_name_unknown) \
    ) pp_end \
)
#define __comp_str__lang_name_unknown "unknown"
#define __comp_str__lang_name_c "c"
#define __comp_str__lang_name_cpp "cpp"

#define __comp_int__lang_version pp_expand( \
    pp_switch_ pp_begin(lang_mode)( \
        pp_case_((lang_mode_c)(lang_version_c)), \
        pp_case_((lang_mode_cpp)(lang_version_cpp)), \
        pp_default_(lang_version_unknown) \
    ) pp_end \
)
#define __comp_int__lang_version_unknown 0

#define __comp_int__lang_version_c lang_version_unknown
#define __comp_int__lang_version_c90 199001L
#define __comp_int__lang_version_c99 199901L
#define __comp_int__lang_version_c11 201112L
#define __comp_int__lang_version_c17 201710L
#define __comp_int__lang_version_c23 202311L

#define __comp_int__lang_version_cpp lang_version_unknown
#define __comp_int__lang_version_cpp98 199711L
#define __comp_int__lang_version_cpp11 201103L
#define __comp_int__lang_version_cpp14 201402L
#define __comp_int__lang_version_cpp17 201703L
#define __comp_int__lang_version_cpp20 202002L
#define __comp_int__lang_version_cpp23 202302L

#define __comp_bool__lang_version_in_c90 0
#define __comp_bool__lang_version_in_c99 0
#define __comp_bool__lang_version_in_c11 0
#define __comp_bool__lang_version_in_c17 0
#define __comp_bool__lang_version_in_c23 0

#define __comp_bool__lang_version_in_cpp98 0
#define __comp_bool__lang_version_in_cpp11 0
#define __comp_bool__lang_version_in_cpp14 0
#define __comp_bool__lang_version_in_cpp17 0
#define __comp_bool__lang_version_in_cpp20 0
#define __comp_bool__lang_version_in_cpp23 0

#define __comp_bool__lang_is_enabled_strict 0
#define __comp_bool__lang_has_extensions 0
#define __comp_bool__lang_has_extensions_gnu 0
#define __comp_bool__lang_has_extensions_ms 0

/* Language Mode */

#if defined(__STDC__)
#undef __comp_enum__lang_mode
#define __comp_enum__lang_mode lang_mode_c
#endif /* defined(__STDC__) */

#if defined(__cplusplus)
#undef __comp_enum__lang_mode
#define __comp_enum__lang_mode lang_mode_cpp
#endif /* defined(__cplusplus) */

/* Language Version */

#if lang_mode == lang_mode_c && defined(__STDC_VERSION__)
#undef __comp_int__lang_version_c
#define __comp_int__lang_version_c __STDC_VERSION__

#if lang_version_c >= lang_version_c90
#undef __comp_bool__lang_version_in_c90
#define __comp_bool__lang_version_in_c90 1
#endif /* lang_version >= lang_version_c90 */
#if lang_version_c >= lang_version_c99
#undef __comp_bool__lang_version_in_c99
#define __comp_bool__lang_version_in_c99 1
#endif /* lang_version >= lang_version_c99 */
#if lang_version_c >= lang_version_c11
#undef __comp_bool__lang_version_in_c11
#define __comp_bool__lang_version_in_c11 1
#endif /* lang_version >= lang_version_c11 */
#if lang_version_c >= lang_version_c17
#undef __comp_bool__lang_version_in_c17
#define __comp_bool__lang_version_in_c17 1
#endif /* lang_version >= lang_version_c17 */
#if lang_version_c >= lang_version_c23
#undef __comp_bool__lang_version_in_c23
#define __comp_bool__lang_version_in_c23 1
#endif /* lang_version >= lang_version_c23 */
#endif /* lang_mode == lang_mode_c && defined(__STDC_VERSION__) */

#if lang_mode == lang_mode_cpp && defined(__cplusplus)
#undef __comp_int__lang_version_cpp
#define __comp_int__lang_version_cpp __cplusplus

#if lang_version_cpp >= lang_version_cpp98
#undef __comp_bool__lang_version_in_cpp98
#define __comp_bool__lang_version_in_cpp98 1
#endif /* lang_version >= lang_version_cpp98 */
#if lang_version_cpp >= lang_version_cpp11
#undef __comp_bool__lang_version_in_cpp11
#define __comp_bool__lang_version_in_cpp11 1
#endif /* lang_version >= lang_version_cpp11 */
#if lang_version_cpp >= lang_version_cpp14
#undef __comp_bool__lang_version_in_cpp14
#define __comp_bool__lang_version_in_cpp14 1
#endif /* lang_version >= lang_version_cpp14 */
#if lang_version_cpp >= lang_version_cpp17
#undef __comp_bool__lang_version_in_cpp17
#define __comp_bool__lang_version_in_cpp17 1
#endif /* lang_version >= lang_version_cpp17 */
#if lang_version_cpp >= lang_version_cpp20
#undef __comp_bool__lang_version_in_cpp20
#define __comp_bool__lang_version_in_cpp20 1
#endif /* lang_version >= lang_version_cpp20 */
#if lang_version_cpp >= lang_version_cpp23
#undef __comp_bool__lang_version_in_cpp23
#define __comp_bool__lang_version_in_cpp23 1
#endif /* lang_version >= lang_version_cpp23 */
#endif /* lang_mode == lang_mode_cpp && defined(__cplusplus) */

/* Language Strict Standards Conformance */

#if defined(__STRICT_ANSI__)
#undef __comp_bool__lang_is_enabled_strict
#define __comp_bool__lang_is_enabled_strict 1
#endif /* defined(__STRICT_ANSI__) */

/* Language Feature Extensions */

#if defined(__GNUC__)
#undef __comp_bool__lang_has_extensions_gnu
#define __comp_bool__lang_has_extensions_gnu 1
#endif /* defined(__GNUC__) */
#if defined(_MSC_EXTENSIONS)
#undef __comp_bool__lang_has_extensions_ms
#define __comp_bool__lang_has_extensions_ms 1
#endif /* defined(_MSC_EXTENSIONS) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_lang_cfg__included */
