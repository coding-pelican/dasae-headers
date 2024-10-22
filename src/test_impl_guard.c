#define DH_IMPL
#include "test_impl_guard.h"
#include "color.h"
#include <stdio.h>


void printColorChannel() {
    printf("Red: %d\n", Color_Red.r);
    printf("Green: %d\n", Color_Red.g);
    printf("Blue: %d\n", Color_Red.b);
}
