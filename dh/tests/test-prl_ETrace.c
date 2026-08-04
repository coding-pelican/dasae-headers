#include "dh-main.h"

#if ETrace_enabled
TEST_fn_("prl/ETrace: captures and resets frames" $scope) {
    ETrace_enable();
    ETrace_reset();

    ETrace_captureFrame();
    ETrace_captureFrame();
    try_(TEST_expect(ETrace_depth() == 2));

    ETrace_reset();
    try_(TEST_expect(ETrace_depth() == 0));
} $unscoped(TEST_fn);

TEST_fn_("prl/ETrace: disable suppresses capture without clearing frames" $scope) {
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
    ETrace_enable();
    ETrace_reset();
    ETrace_captureFrame();
    try_(TEST_expect(ETrace_depth() == 1));
} $unscoped(TEST_fn);

TEST_fn_("prl/ETrace - TEST runner resets ETrace before each case: verify" $scope) {
    ETrace_enable();
    try_(TEST_expect(ETrace_depth() == 0));
} $unscoped(TEST_fn);
#else /* !ETrace_enabled */
TEST_fn_("prl/ETrace: disabled surface remains inert" $scope) {
    ETrace_enable();
    ETrace_captureFrame();
    try_(TEST_expect(!ETrace_isEnabled()));
    try_(TEST_expect(ETrace_depth() == 0));

    ETrace_disable();
    ETrace_reset();
    ETrace_print();
    try_(TEST_expect(!ETrace_isEnabled()));
    try_(TEST_expect(ETrace_depth() == 0));
} $unscoped(TEST_fn);
#endif /* ETrace_enabled */
