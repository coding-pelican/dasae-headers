#ifndef dal_c_ext_base__included
#define dal_c_ext_base__included 1

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define dal_c_ext_in_c23 1
#else
#define dal_c_ext_in_c23 0
#endif

#include "pp.h"
#include "dsl.h"

#include <stddef.h>
#if !dal_c_ext_in_c23
#include <stdbool.h> /* deprecated in C23; `bool`, `true`, `false` are now keywords. */
#if UNUSED_CODE
#include <iso646.h> /* deprecated in C23; `not`, `and`, `or` are now keywords. */
#endif /* UNUSED_CODE */
#endif /* !dal_c_ext_in_c23 */

#include <stdint.h>
#include <limits.h>
#include <float.h>

#if dal_c_ext_in_c23
#if UNUSED_CODE
#include <stdbit.h>
#include <stdckdint.h>
#include <stdcountof.h>
#endif /* UNUSED_CODE */
#endif /* dal_c_ext_in_c23 */

#if !dal_c_ext_in_c23
#include <stdalign.h> /* deprecated in C23; use `alignof`, `alignas` or `[[alignas]]`. */
#if UNUSED_CODE
/* C11 `stdnoreturn.h` conflicts with Windows when included before `windows.h`.
 * `noreturn` expands to `_Noreturn`; MSVC uses `__declspec(noreturn)`, yielding
 * invalid `__declspec(_Noreturn)`. Known since 2012.
 * Gnulib: https://www.gnu.org/software/gnulib/manual/html_node/stdnoreturn_002eh.html
 * Eric Blake (autoconf 2012-04): https://lists.gnu.org/archive/html/autoconf/2012-04/msg00051.html */
#include <stdnoreturn.h> /* deprecated in C23; use `[[noreturn]]`. */
#endif /* UNUSED_CODE */
#endif /* !dal_c_ext_in_c23 */

#include <stdarg.h>
#if !defined(__STDC_NO_ATOMICS__)
#if UNUSED_CODE
#include <stdatomic.h>
#endif /* UNUSED_CODE */
#endif /* !defined(__STDC_NO_ATOMICS__) */

#endif /* dal_c_ext_base__included */
