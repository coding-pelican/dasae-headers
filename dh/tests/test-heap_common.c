#include "dh-main.h"
#include "dh/heap/common.h"

TEST_fn_("heap/common: Geom validates and aligns lengths" $scope) {
    let_(geometry, heap_Geom) = {
        .page_size = 4096,
        .reserve_align = 65536,
        .commit_align = 4096,
        .map_align = 65536,
    };

    try_(TEST_expect(heap_Geom_isValid(geometry)));
    try_(TEST_expect(heap_Geom_alignPageWith(geometry, 1) == 4096));
    try_(TEST_expect(heap_Geom_alignPageWith(geometry, 4097) == 8192));
    try_(TEST_expect(heap_Geom_alignReserveWith(geometry, 1) == 65536));
    try_(TEST_expect(heap_Geom_alignReserveWith(geometry, 65537) == 131072));
    try_(TEST_expect(heap_Geom_alignCommitWith(geometry, 0) == 4096));
} $unscoped(TEST_fn);

TEST_fn_("heap/common: Geom builds vendor-neutral default geometry" $scope) {
    let geometry = heap_Geom_from(16384);

    try_(TEST_expect(heap_Geom_isValid(geometry)));
    try_(TEST_expect(geometry.page_size == 16384));
    try_(TEST_expect(geometry.reserve_align == 16384));
    try_(TEST_expect(geometry.commit_align == 16384));
    try_(TEST_expect(geometry.map_align == 16384));
} $unscoped(TEST_fn);
