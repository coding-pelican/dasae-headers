#include "dh-main.h"
#include "dh/mem/ring.h"

TEST_fn_("mem/ring: absolute and head-relative indices wrap" $scope) {
    try_(TEST_expect(mem_ring_idx(4, 0) == 0));
    try_(TEST_expect(mem_ring_idx(4, 3) == 3));
    try_(TEST_expect(mem_ring_idx(4, 4) == 0));
    try_(TEST_expect(mem_ring_idx(4, 9) == 1));

    try_(TEST_expect(mem_ring_idxFrom(2, 5, 0) == 2));
    try_(TEST_expect(mem_ring_idxFrom(2, 5, 2) == 4));
    try_(TEST_expect(mem_ring_idxFrom(2, 5, 3) == 0));
    try_(TEST_expect(mem_ring_idxFrom(2, 5, 5) == 2));
    return_ok({});
} $unscoped(TEST_fn)

TEST_fn_("mem/ring: every logical slot maps from head modulo capacity" $scope) {
    for_(($r(1, 9))(cap)) {
        for_(($rt(cap))(head)) {
            for_(($r(0, cap + 1))(idx)) {
                try_(TEST_expect(mem_ring_idxFrom(head, cap, idx) == ((head + idx) % cap)));
            } $end(for);
        } $end(for);
    } $end(for);
} $unscoped(TEST_fn)
