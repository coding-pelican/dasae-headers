/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    assert.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-22 (date of creation)
 * @updated 2024-10-22 (date of last update)
 * @version v1.0.0
 * @ingroup SOME_GROUP
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef ASSERT_INCLUDED
#define ASSERT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/* assert expression. */
#define assert(_expr) assert_(_expr)
/* assert expression with formatted message. */
#define assertFmt(_expr, _fmt, ...) assertFmt_(_expr, _fmt, __VA_ARGS__)

/* assert that expression is true. */
#define assertTrue(_expr) assertTrue_(_expr)
/* assert that expression is false. */
#define assertFalse(_expr) assertFalse_(_expr)
/* assert that two expressions are equal.  */
#define assertEq(_expr1, _expr2) assertEq_(_expr1, _expr2)
/* assert that two expressions are not equal.  */
#define assertNe(_expr1, _expr2) assertNe_(_expr1, _expr2)
/* assert that expressions is null.  */
#define assertNull(_expr) assertNull_(_expr)
/* assert that expressions is not null.  */
#define assertNotNull(_expr) assertNotNull_(_expr)

/* assert that expression is true with formatted message. */
#define assertTrueFmt(_expr, _fmt, ...) assertTrueFmt_(_expr, _fmt, __VA_ARGS__)
/* assert that expression is false with formatted message. */
#define assertFalseFmt(_expr, _fmt, ...) assertFalseFmt_(_expr, _fmt, __VA_ARGS__)
/* assert that two expressions are equal with formatted message. */
#define assertEqFmt(_expr1, _expr2, _fmt, ...) assertEqFmt_(_expr1, _expr2, _fmt, __VA_ARGS__)
/* assert that two expressions are not equal with formatted message. */
#define assertNeFmt(_expr1, _expr2, _fmt, ...) assertNeFmt_(_expr1, _expr2, _fmt, __VA_ARGS__)
/* assert that expressions is null with formatted message. */
#define assertNullFmt(_expr, _fmt, ...) assertNullFmt_(_expr, _fmt, __VA_ARGS__)
/* assert that expressions is not null with formatted message. */
#define assertNotNullFmt(_expr, _fmt, ...) assertNotNullFmt_(_expr, _fmt, __VA_ARGS__)


#ifndef NDEBUG

#  include "common.h" /* For `ignore_return` */
#  include "primitive_types.h"
#  include <stdio.h> /* For `fprintf()`, `stderr` */
#  include <stdlib.h> /* For `about()` */

#  if defined(_MSC_VER)
/* Microsoft Visual Studio */
#    define assert_debugBreak() __debugbreak()
#  elif defined(__GNUC__) || defined(__clang__)
/* GCC or Clang */
#    define assert_debugBreak() __builtin_trap()
#  else
/* Fallback using signal */
#    include <signal.h>
#    define assert_debugBreak() raise(SIGTRAP)
#  endif

#  define assert_(_expr)                                                         \
      do {                                                                       \
          if (!(_expr)) {                                                        \
              ignore_return fprintf(                                             \
                  stderr,                                                        \
                  "Assertion failed: %s, in function %s, at file %s, line %d\n", \
                  #_expr,                                                        \
                  __func__,                                                      \
                  __FILE__,                                                      \
                  __LINE__                                                       \
              );                                                                 \
              assert_debugBreak();                                               \
              abort();                                                           \
          }                                                                      \
      } while (false)

#  define assertFmt_(_expr, _fmt, ...)                     \
      do {                                                 \
          if (!(_expr)) {                                  \
              ignore_return fprintf(                       \
                  stderr,                                  \
                  "Assertion failed: %s\n",                \
                  #_expr                                   \
              );                                           \
              ignore_return fprintf(                       \
                  stderr,                                  \
                  _fmt,                                    \
                  __VA_ARGS__                              \
              );                                           \
              ignore_return fprintf(                       \
                  stderr,                                  \
                  "in function %s, at file %s, line %d\n", \
                  __func__,                                \
                  __FILE__,                                \
                  __LINE__                                 \
              );                                           \
              assert_debugBreak();                         \
              abort();                                     \
          }                                                \
      } while (false)

#  define assertTrue_(_expr)        assertFmt_((_expr) == true, "%s is not true", #_expr)
#  define assertFalse_(_expr)       assertFmt_((_expr) == false, "%s is not false", #_expr)
#  define assertEq_(_expr1, _expr2) assertFmt_((_expr1) == (_expr2), "%s is not equal to %s", #_expr1, #_expr2)
#  define assertNe_(_expr1, _expr2) assertFmt_((_expr1) != (_expr2), "%s is equal to %s", #_expr1, #_expr2)
#  define assertNull_(_expr)        assertFmt_((_expr) == null, "%s is not null", #_expr)
#  define assertNotNull_(_expr)     assertFmt_((_expr) != null, "%s is null", #_expr)

#  define assertTrueFmt_(_expr, _fmt, ...)        assertFmt_((_expr) == true, _fmt, __VA_ARGS__)
#  define assertFalseFmt_(_expr, _fmt, ...)       assertFmt_((_expr) == false, _fmt, __VA_ARGS__)
#  define assertEqFmt_(_expr1, _expr2, _fmt, ...) assertFmt_((_expr1) == (_expr2), _fmt, __VA_ARGS__)
#  define assertNeFmt_(_expr1, _expr2, _fmt, ...) assertFmt_((_expr1) != (_expr2), _fmt, __VA_ARGS__)
#  define assertNullFmt_(_expr, _fmt, ...)        assertFmt_((_expr) == null, _fmt, __VA_ARGS__)
#  define assertNotNullFmt_(_expr, _fmt, ...)     assertFmt_((_expr) != null, _fmt, __VA_ARGS__)

#else

#  include "common.h" /* For `unused()` */

#  define assert_(_expr)               unused(0)
#  define assertFmt_(_expr, _fmt, ...) unused(0)

#  define assertTrue_(_expr)        unused(0)
#  define assertFalse_(_expr)       unused(0)
#  define assertEq_(_expr1, _expr2) unused(0)
#  define assertNe_(_expr1, _expr2) unused(0)
#  define assertNull_(_expr)        unused(0)
#  define assertNotNull_(_expr)     unused(0)

#  define assertTrueFmt_(_expr, _fmt, ...)        unused(0)
#  define assertFalseFmt_(_expr, _fmt, ...)       unused(0)
#  define assertEqFmt_(_expr1, _expr2, _fmt, ...) unused(0)
#  define assertNeFmt_(_expr1, _expr2, _fmt, ...) unused(0)
#  define assertNullFmt_(_expr, _fmt, ...)        unused(0)
#  define assertNotNullFmt_(_expr, _fmt, ...)     unused(0)

#endif /* NDEBUG */


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* ASSERT_INCLUDED */
