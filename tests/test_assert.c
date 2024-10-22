#include "../src/assert.h"


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
