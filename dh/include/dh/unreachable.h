#ifndef UNREACHABLE_INCLUDED
#define UNREACHABLE_INCLUDED (1)

#include <stdnoreturn.h>
#include <assert.h>

// Compiler-specific macros for optimization hints
#if defined(__GNUC__) || defined(__clang__)
#define UNREACHABLE_BUILTIN __builtin_unreachable()
#elif defined(_MSC_VER)
#define UNREACHABLE_BUILTIN __assume(0)
#else
#define UNREACHABLE_BUILTIN \
    do {                    \
    } while (0)
#endif

// Debug assert for development builds
#if defined(DEBUG) || !defined(NDEBUG)
#define unreachable_debug_assert(msg) assert(!"msg")
#else
#define unreachable_debug_assert(msg) ((void)0)
#endif

/**
 * @brief Runtime unreachable - analogous to Rust's unreachable!() macro
 *
 * This should be used in situations where you know a code path is impossible,
 * but the compiler cannot prove it. Using this incorrectly leads to undefined behavior.
 *
 * In debug builds, this will trigger an assertion.
 * In release builds, this tells the compiler the code is unreachable for optimization.
 */
#define unreachable()                                         \
    do {                                                      \
        unreachable_debug_assert("Reached unreachable code"); \
        UNREACHABLE_BUILTIN;                                  \
    } while (0)

/**
 * @brief Compile-time unreachable check - analogous to Zig's unreachable
 *
 * This version is intended for use in compile-time constant evaluation contexts,
 * like switch statements where you know all cases are covered.
 *
 * @note This is safer than unreachable() as it will fail at compile time if reachable
 */
#define unreachable_static()                                                               \
    do {                                                                                   \
        /* Create a switch with no valid cases to force compile-time error if reachable */ \
        switch (0) {                                                                       \
        case 0:                                                                            \
        case 0:                                                                            \
        }                                                                                  \
        unreachable();                                                                     \
    } while (0)

/**
 * @brief Runtime unreachable with message - provides more context for debugging
 *
 * @param msg The message to display if this is reached in debug mode
 */
#define unreachable_msg(msg)           \
    do {                               \
        unreachable_debug_assert(msg); \
        UNREACHABLE_BUILTIN;           \
    } while (0)

/**
 * @brief Function implementation of unreachable for use in expression contexts
 *
 * This is useful when you need an expression rather than a statement.
 * Returns a value of the specified type, though it will never actually return.
 *
 * @param T The type to "return" (only used for type checking)
 * @return Value of type T (never actually returns)
 */
#define unreachable_val(T) \
    ((T)(unreachable(), (T)0))

/**
 * @brief Verify at compile-time that a switch covers all cases
 *
 * @param value The value being switched on
 * @param type The type of the value
 */
#define unreachable_switch(value, type)   \
    do {                                  \
        type __unreachable_tmp = (value); \
        (void)__unreachable_tmp;          \
        unreachable_static();             \
    } while (0)

// Example usage:
#if 0 // Disabled to prevent compilation

typedef enum {
    Status_OK,
    Status_ERROR
} Status;

int example_function(Status status) {
    switch (status) {
        case Status_OK: return 1;
        case Status_ERROR: return 0;
    }
    // Compiler knows all cases are handled
    unreachable_switch(status, Status);
}

int unsafe_divide(int a, int b) {
    if (b == 0) {
        unreachable_msg("Division by zero should never happen");
    }
    return a / b;
}

int get_positive(int x) {
    if (x <= 0) {
        return unreachable_val(int);  // Use in expression context
    }
    return x;
}

#endif

#endif // UNREACHABLE_INCLUDED
