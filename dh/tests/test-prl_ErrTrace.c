#include "dh-main.h"

TEST_fn_("prl/ErrTrace: captures and resets frames" $scope) {
    ETrace_enable();
    ETrace_reset();

    ETrace_captureFrame_debug(srcLoc(), null);
    ETrace_captureFrame_debug(srcLoc(), null);
    try_(TEST_expect(ETrace_depth_debug() == 2));

    ETrace_reset();
    try_(TEST_expect(ETrace_depth_debug() == 0));
} $unscoped(TEST_fn);

TEST_fn_("prl/ErrTrace: disable suppresses capture without clearing frames" $scope) {
    ETrace_enable();
    ETrace_reset();

    ETrace_captureFrame_debug(srcLoc(), null);
    try_(TEST_expect(ETrace_depth_debug() == 1));

    ETrace_disable();
    try_(TEST_expect(ETrace_depth_debug() == 1));

    ETrace_captureFrame_debug(srcLoc(), null);
    try_(TEST_expect(ETrace_depth_debug() == 1));

    ETrace_enable();
    ETrace_captureFrame_debug(srcLoc(), null);
    try_(TEST_expect(ETrace_depth_debug() == 2));

    ETrace_reset();
} $unscoped(TEST_fn);

TEST_fn_("prl/ErrTrace - TEST runner resets ETrace before each case: seed" $scope) {
    ETrace_enable();
    ETrace_reset();
    ETrace_captureFrame_debug(srcLoc(), null);
    try_(TEST_expect(ETrace_depth_debug() == 1));
} $unscoped(TEST_fn);

TEST_fn_("prl/ErrTrace - TEST runner resets ETrace before each case: verify" $scope) {
    ETrace_enable();
    try_(TEST_expect(ETrace_depth_debug() == 0));
} $unscoped(TEST_fn);
