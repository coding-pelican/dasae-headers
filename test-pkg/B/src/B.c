#include "B.h"
#include "B-stub.h"
#include "A.h"
#include "other/AA.h"

$static var_(pair, O$$(Pair)) = none();

fn_((B_init(void))(void)) {
    A_update();
    asg_lit((&pair)(some({ .key = 0, .value = 0.0 })));
}

fn_((B_added(void))(f64)) {
    let unwrapped_pair = unwrap_(pair);
    return B_stub_add(unwrapped_pair.key, unwrapped_pair.value);
}

fn_((B_len(void))(usize)) {
    return AA_getLen(A_name());
}

fn_((B_next(void))(Pair)) {
    A_update();
    asg_lit((&pair)(some({ .key = A_id(), .value = A_value() })));
    return unwrap_(pair);
}
