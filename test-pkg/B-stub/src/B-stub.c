#include "B-stub.h"

fn_((B_stub_add(usize key, f64 value))(f64)) {
    return as$(f64)(key) + value;
}
