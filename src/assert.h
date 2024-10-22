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


/* Test Code
#include "assert.h"


int main() {
    int x = 5;

    // Test with a passing assertion
    Assert(x == 5);

    // Test with a failing assertion without a message
    // This should print the assertion details and abort
    // Assert(x != 5);
    // Assert(x != NULL);

    // Test with a failing assertion with a message
    // This should print the assertion details, the custom message, and abort
    AssertMessage(x != 5, "x was expected to be 10, but got %d", x);
    // AssertMessage(x != NULL, "x should never be NULL at this point!");

    return 0;
}
*/


#ifndef ASSERT_INCLUDED
#define ASSERT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#ifdef NDEBUG

#  define Assert(_Expression)             ((void)0)
#  define AssertMessage(_Expression, ...) ((void)0)

#else


#  include <stdio.h>
#  include <stdlib.h>
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


/* Assert without additional message */
#  define Assert(_Expression)                                                 \
      do {                                                                    \
          if (!(_Expression)) {                                               \
              (void)fprintf(                                                  \
                  stderr,                                                     \
                  "Assertion failed: %s\nFile: %s\nLine: %d\nFunction: %s\n", \
                  #_Expression,                                               \
                  __FILE__,                                                   \
                  __LINE__,                                                   \
                  __func__                                                    \
              );                                                              \
              DEBUG_BREAK();                                                  \
              abort();                                                        \
          }                                                                   \
      } while (0)

/* Assert with additional formatted message */
#  define AssertMessage(_Expression, ...)                 \
      do {                                                \
          if (!(_Expression)) {                           \
              (void)fprintf(                              \
                  stderr,                                 \
                  "Assertion failed: %s\n",               \
                  #_Expression                            \
              );                                          \
              (void)fprintf(                              \
                  stderr,                                 \
                  __VA_ARGS__                             \
              );                                          \
              (void)fprintf(                              \
                  stderr,                                 \
                  "\nFile: %s\nLine: %d\nFunction: %s\n", \
                  __FILE__,                               \
                  __LINE__,                               \
                  __func__                                \
              );                                          \
              DEBUG_BREAK();                              \
              abort();                                    \
          }                                               \
      } while (0)

#endif /* NDEBUG */


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* ASSERT_INCLUDED */
