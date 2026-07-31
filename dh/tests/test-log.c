#include "dh-main.h"
#include "dh/log.h"

typedef struct test_log_Output {
    var_(buf, S$u8);
    var_(used, usize);
} test_log_Output;

$attr($must_check)
$static fn_((test_log_Output_write(
    P$raw ctx,
    S_const$u8 bytes
))(io_WriteE$usize) $scope) {
    let self = ptrCast$((test_log_Output*)(ensureNonnull(ctx)));
    let remaining = S_suffix((self->buf)(self->used));
    if (remaining.len < bytes.len) return_err(E_cause$TooSmallBuffer());
    pri_memcpyS(S_prefix((remaining)(bytes.len)), bytes);
    self->used += bytes.len;
    return_ok(bytes.len);
} $unscoped(fn);

$static fn_((test_log_Output_writer(test_log_Output* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return io_Writer_ensureValid((io_Writer){
        .ctx = self,
        .writeFn = test_log_Output_write,
    });
};

TEST_fn_("log: writer provider filters levels and formats default scope" $scope) {
    T_use_A$(64, u8);
    var_(storage, A$64$u8) $undefined;
    var output = (test_log_Output){
        .buf = A_ref$((S$u8)storage),
        .used = 0,
    };
    var provider = log_Writer_init(test_log_Output_writer(&output), log_Level_info);
    let logger = log_Writer_self(&provider);

    log_debug(logger, u8_l("filtered {:u}"), 1u);
    log_info(logger, u8_l("ready {:u}"), 2u);

    try_(TEST_expect(mem_eqlBytes(
        S_prefix((output.buf)(output.used)).as_const,
        u8_l("info: ready 2" io_nl)
    )));
} $unscoped(TEST_fn);

TEST_fn_("log: writer provider carries explicit scope" $scope) {
    T_use_A$(64, u8);
    var_(storage, A$64$u8) $undefined;
    var output = (test_log_Output){
        .buf = A_ref$((S$u8)storage),
        .used = 0,
    };
    var provider = log_Writer_init(test_log_Output_writer(&output), log_Level_debug);
    let logger = log_Writer_self(&provider);

    log_scopedWarn(logger, u8_l("proc"), u8_l("missing {:s}"), u8_l("child"));

    try_(TEST_expect(mem_eqlBytes(
        S_prefix((output.buf)(output.used)).as_const,
        u8_l("warning(proc): missing child" io_nl)
    )));
} $unscoped(TEST_fn);

TEST_fn_("log: noop provider is an enabled policy, not a missing provider" $scope) {
    try_(TEST_expect(log_isValid(log_Self_noop)));
    try_(TEST_expect(!log_enabled(
        log_Self_noop,
        log_Level_err,
        log_scope_default
    )));
    log_err(log_Self_noop, u8_l("discarded by policy"));
} $unscoped(TEST_fn);
