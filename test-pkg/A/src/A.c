#include "A.h"
#include "foo.h"
#include "edition/bar.h"

$static var_(is_initialized, bool) = false;
$static var_(foo, foo_Obj) = {};
$static var_(bar, bar_Obj) = {};

fn_((A_update(void))(void)) {
    if (!is_initialized) {
        foo = foo_Obj_init();
        bar = bar_Obj_init();
        is_initialized = true;
        return;
    }
    foo = foo_Obj_init();
}

fn_((A_name(void))(S_const$u8)) {
    return bar_Obj_nameRef(&bar);
}

fn_((A_id(void))(usize)) {
    return foo_Obj_id(foo);
}

fn_((A_value(void))(f64)) {
    return foo_Obj_value(foo);
}
