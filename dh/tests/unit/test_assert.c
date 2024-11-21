#include "dh/debug/assert.h"


int main(void) {
    int x = 5;

    // Test with a passing assertion
    debug_assert(x == 5);

    // Test with a failing assertion without a message
    // This should print the assertion details and abort
    // debug_assert(x != 5);
    // debug_assert(x != NULL);

    // Test with a failing assertion with a message
    // This should print the assertion details, the custom message, and abort
    debug_assert_fmt(x != 5, "x was expected to be 10, but got %d", x);
    // debug_assert_fmt(x != NULL, "x should never be NULL at this point!");

    return 0;
}
