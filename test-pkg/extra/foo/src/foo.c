#include "foo.h"

$static var_(counter, usize) = 0;
$static fn_((fib(usize n))(usize)) { /* NOLINT(misc-no-recursion) */
    return n < 2 ? n : fib(n - 1) + fib(n - 2);
}

fn_((foo_Obj_init(void))(foo_Obj)) {
    let foo = (foo_Obj){ .id = counter, .value = as$(f64)(fib(counter)) };
    counter++;
    return foo;
}

fn_((foo_Obj_id(foo_Obj self))(usize)) {
    return self.id;
}

fn_((foo_Obj_value(foo_Obj self))(f64)) {
    return self.value;
}
