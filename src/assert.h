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


/* Assert expression. */
#define Assert(_Expression) Assert_(_Expression)
/* Assert expression with formatted message. */
#define AssertFmt(_Expression, _Formatted, ...) AssertFmt_(_Expression, _Formatted, __VA_ARGS__)

/* Assert that expression is true. */
#define AssertTrue(_Expression) AssertTrue_(_Expression)
/* Assert that expression is false. */
#define AssertFalse(_Expression) AssertFalse_(_Expression)
/* Assert that two expressions are equal.  */
#define AssertEqual(_Expression1, _Expression2) AssertEqual_(_Expression1, _Expression2)
/* Assert that two expressions are not equal.  */
#define AssertNotEqual(_Expression1, _Expression2) AssertNotEqual_(_Expression1, _Expression2)
/* Assert that expressions is null.  */
#define AssertNull(_Expression) AssertNull_(_Expression)
/* Assert that expressions is not null.  */
#define AssertNotNull(_Expression) AssertNotNull_(_Expression)

/* Assert that expression is true with formatted message. */
#define AssertTrueFmt(_Expression, _Formatted, ...) AssertTrueFmt_(_Expression, _Formatted, __VA_ARGS__)
/* Assert that expression is false with formatted message. */
#define AssertFalseFmt(_Expression, _Formatted, ...) AssertFalseFmt_(_Expression, _Formatted, __VA_ARGS__)
/* Assert that two expressions are equal with formatted message. */
#define AssertEqualFmt(_Expression1, _Expression2, _Formatted, ...) AssertEqualFmt_(_Expression1, _Expression2, _Formatted, __VA_ARGS__)
/* Assert that two expressions are not equal with formatted message. */
#define AssertNotEqualFmt(_Expression1, _Expression2, _Formatted, ...) AssertNotEqualFmt_(_Expression1, _Expression2, _Formatted, __VA_ARGS__)
/* Assert that expressions is null with formatted message. */
#define AssertNullFmt(_Expression, _Formatted, ...) AssertNullFmt_(_Expression, _Formatted, __VA_ARGS__)
/* Assert that expressions is not null with formatted message. */
#define AssertNotNullFmt(_Expression, _Formatted, ...) AssertNotNullFmt_(_Expression, _Formatted, __VA_ARGS__)


#ifdef NDEBUG

#  include "common.h" /* For `UnusedValue()` */

#  define Assert_(_Expression)                     UnusedValue(0)
#  define AssertFmt_(_Expression, _Formatted, ...) UnusedValue(0)

#  define AssertTrue_(_Expression)                    UnusedValue(0)
#  define AssertFalse_(_Expression)                   UnusedValue(0)
#  define AssertEqual_(_Expression1, _Expression2)    UnusedValue(0)
#  define AssertNotEqual_(_Expression1, _Expression2) UnusedValue(0)
#  define AssertNull_(_Expression)                    UnusedValue(0)
#  define AssertNotNull_(_Expression)                 UnusedValue(0)

#  define AssertTrueFmt_(_Expression, _Formatted, ...)                    UnusedValue(0)
#  define AssertFalseFmt_(_Expression, _Formatted, ...)                   UnusedValue(0)
#  define AssertEqualFmt_(_Expression1, _Expression2, _Formatted, ...)    UnusedValue(0)
#  define AssertNotEqualFmt_(_Expression1, _Expression2, _Formatted, ...) UnusedValue(0)
#  define AssertNullFmt_(_Expression, _Formatted, ...)                    UnusedValue(0)
#  define AssertNotNullFmt_(_Expression, _Formatted, ...)                 UnusedValue(0)

#else

#  include <stdbool.h> /* For `bool`, `true`, `false` */
#  include <stdio.h> /* For `fprintf()`, `stderr` */
#  include <stdlib.h> /* For `about()`, `NULL` */

#  if defined(_MSC_VER)
/* Microsoft Visual Studio */
#    define DEBUG_BREAK() __debugbreak()
#  elif defined(__GNUC__) || defined(__clang__)
/* GCC or Clang */
#    define DEBUG_BREAK() __builtin_trap()
#  else
/* Fallback using signal */
#    include <signal.h>
#    define DEBUG_BREAK() raise(SIGTRAP)
#  endif

#  define Assert_(_Expression)                                                   \
      do {                                                                       \
          if (!(_Expression)) {                                                  \
              (void)fprintf(                                                     \
                  stderr,                                                        \
                  "Assertion failed: %s, in function %s, at file %s, line %d\n", \
                  #_Expression,                                                  \
                  __func__,                                                      \
                  __FILE__,                                                      \
                  __LINE__                                                       \
              );                                                                 \
              DEBUG_BREAK();                                                     \
              abort();                                                           \
          }                                                                      \
      } while (0)

#  define AssertFmt_(_Expression, _Formatted, ...)         \
      do {                                                 \
          if (!(_Expression)) {                            \
              (void)fprintf(                               \
                  stderr,                                  \
                  "Assertion failed: %s\n",                \
                  #_Expression                             \
              );                                           \
              (void)fprintf(                               \
                  stderr,                                  \
                  _Formatted,                              \
                  __VA_ARGS__                              \
              );                                           \
              (void)fprintf(                               \
                  stderr,                                  \
                  "in function %s, at file %s, line %d\n", \
                  __func__,                                \
                  __FILE__,                                \
                  __LINE__                                 \
              );                                           \
              DEBUG_BREAK();                               \
              abort();                                     \
          }                                                \
      } while (0)

#  define AssertTrue_(_Expression)                    AssertFmt_((_Expression) == true, "%s is not true", #_Expression)
#  define AssertFalse_(_Expression)                   AssertFmt_((_Expression) == false, "%s is not false", #_Expression)
#  define AssertEqual_(_Expression1, _Expression2)    AssertFmt_((_Expression1) == (_Expression2), "%s is not equal to %s", #_Expression1, #_Expression2)
#  define AssertNotEqual_(_Expression1, _Expression2) AssertFmt_((_Expression1) != (_Expression2), "%s is equal to %s", #_Expression1, #_Expression2)
#  define AssertNull_(_Expression)                    AssertFmt_((_Expression) == NULL, "%s is not NULL", #_Expression)
#  define AssertNotNull_(_Expression)                 AssertFmt_((_Expression) != NULL, "%s is NULL", #_Expression)

#  define AssertTrueFmt_(_Expression, _Formatted, ...)                    AssertFmt_((_Expression) == true, _Formatted, __VA_ARGS__)
#  define AssertFalseFmt_(_Expression, _Formatted, ...)                   AssertFmt_((_Expression) == false, _Formatted, __VA_ARGS__)
#  define AssertEqualFmt_(_Expression1, _Expression2, _Formatted, ...)    AssertFmt_((_Expression1) == (_Expression2), _Formatted, __VA_ARGS__)
#  define AssertNotEqualFmt_(_Expression1, _Expression2, _Formatted, ...) AssertFmt_((_Expression1) != (_Expression2), _Formatted, __VA_ARGS__)
#  define AssertNullFmt_(_Expression, _Formatted, ...)                    AssertFmt_((_Expression) == NULL, _Formatted, __VA_ARGS__)
#  define AssertNotNullFmt_(_Expression, _Formatted, ...)                 AssertFmt_((_Expression) != NULL, _Formatted, __VA_ARGS__)

#endif /* NDEBUG */


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* ASSERT_INCLUDED */
