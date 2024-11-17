#include "dh/scope.h"

int get_value() {
    return 1;
}

int main(void) {
    scope_if(int a = get_value(), a == 1) {
        return 0;
    }
}
