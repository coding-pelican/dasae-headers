#include "../src/assert.h"


int main() {
    int x = 5;

    // Test with a passing assertion
    assert(x == 5);

    // Test with a failing assertion without a message
    // This should print the assertion details and abort
    // assert(x != 5);
    // assert(x != NULL);

    // Test with a failing assertion with a message
    // This should print the assertion details, the custom message, and abort
    assertFmt(x != 5, "x was expected to be 10, but got %d", x);
    // assertMessage(x != NULL, "x should never be NULL at this point!");

    return 0;
}
