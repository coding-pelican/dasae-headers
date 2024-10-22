#define DH_IMPL
#include "test_impl_guard.h"
#include "color.h"
#include <stdio.h>


void PrintColorChannel() {
    printf("Red: %d\n", Color_red.r);
    printf("Green: %d\n", Color_red.g);
    printf("Blue: %d\n", Color_red.b);
}
