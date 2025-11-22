#ifndef B__included
#define B__included 1

#include "dh/prl.h"

typedef struct Pair {
    usize key;
    f64 value;
} Pair;
$extern fn_((B_init(void))(void));
$extern fn_((B_added(void))(f64));
$extern fn_((B_len(void))(usize));
$extern fn_((B_next(void))(Pair));

#endif /* B__included */
