#pragma once
#ifndef B__included
#define B__included 1

#include <dh/prl.h>

T_alias$((Pair)(struct Pair {
    var_(key, usize);
    var_(value, f64);
}));
$extern fn_((B_init(void))(void));
$extern fn_((B_added(void))(f64));
$extern fn_((B_len(void))(usize));
$extern fn_((B_next(void))(Pair));

#endif /* B__included */
