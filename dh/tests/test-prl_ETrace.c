#include "dh-main.h"

TEST_fn_("prl/ETrace: captures and resets frames" $scope) {
    if (!ETrace_enabled) try_(TEST_skip());
    ETrace_enable();
    ETrace_reset();

    ETrace_captureFrame();
    ETrace_captureFrame();
    try_(TEST_expect(ETrace_depth() == 2));

    ETrace_reset();
    try_(TEST_expect(ETrace_depth() == 0));
} $unscoped(TEST_fn);

TEST_fn_("prl/ETrace: disable suppresses capture without clearing frames" $scope) {
    if (!ETrace_enabled) try_(TEST_skip());
    ETrace_enable();
    ETrace_reset();

    ETrace_captureFrame();
    try_(TEST_expect(ETrace_depth() == 1));

    ETrace_disable();
    try_(TEST_expect(ETrace_depth() == 1));

    ETrace_captureFrame();
    try_(TEST_expect(ETrace_depth() == 1));

    ETrace_enable();
    ETrace_captureFrame();
    try_(TEST_expect(ETrace_depth() == 2));

    ETrace_reset();
} $unscoped(TEST_fn);

TEST_fn_("prl/ETrace - TEST runner resets ETrace before each case: seed" $scope) {
    if (!ETrace_enabled) try_(TEST_skip());
    ETrace_enable();
    ETrace_reset();
    ETrace_captureFrame();
    try_(TEST_expect(ETrace_depth() == 1));
} $unscoped(TEST_fn);

TEST_fn_("prl/ETrace - TEST runner resets ETrace before each case: verify" $scope) {
    if (!ETrace_enabled) try_(TEST_skip());
    ETrace_enable();
    try_(TEST_expect(ETrace_depth() == 0));
} $unscoped(TEST_fn);
