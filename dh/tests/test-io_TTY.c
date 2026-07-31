#include "dh-main.h"
#include "dh/io/TTY.h"
#include "dh/fs/File.h"

$static fn_((test_io_TTY__stdCfg(void))(io_TTY_Cfg)) {
    var direct = proc_std_Direct_initNative();
    return io_TTY_Cfg_std(proc_std_Direct_self(&direct));
};

TEST_fn_("io/TTY: std config wraps stdio handles" $scope) {
    let cfg = test_io_TTY__stdCfg();
    var tty = io_TTY_init(cfg);

    try_(TEST_expect(fs_File_handle(tty.input_file) == fs_File_handle(cfg.input_file)));
    try_(TEST_expect(fs_File_handle(tty.output_file) == fs_File_handle(cfg.output_file)));
} $unscoped(TEST_fn);

TEST_fn_("io/TTY: mode patch presets expose raw byte and VT intent" $scope) {
    let preserve = io_TTY_ModePatch_preserve();
    try_(TEST_expect(preserve.enable == io_TTY_ModeBit_none));
    try_(TEST_expect(preserve.disable == io_TTY_ModeBit_none));
    try_(TEST_expect(!preserve.set_min_time));

    let raw = io_TTY_ModePatch_rawBytes();
    try_(TEST_expect((raw.disable & io_TTY_ModeBit_echo) != 0));
    try_(TEST_expect((raw.disable & io_TTY_ModeBit_line_input) != 0));
    try_(TEST_expect((raw.disable & io_TTY_ModeBit_signal_input) != 0));
    try_(TEST_expect((raw.disable & io_TTY_ModeBit_output_process) != 0));
    try_(TEST_expect(raw.set_min_time));
    try_(TEST_expect(raw.min_read == 1));
    try_(TEST_expect(raw.timeout_ds == 0));

    let raw_vt = io_TTY_ModePatch_rawVT();
    try_(TEST_expect((raw_vt.enable & io_TTY_ModeBit_vt_input) != 0));
    try_(TEST_expect((raw_vt.enable & io_TTY_ModeBit_vt_output) != 0));
    try_(TEST_expect((raw_vt.enable & io_TTY_ModeBit_output_process) != 0));
    try_(TEST_expect((raw_vt.disable & io_TTY_ModeBit_output_process) == 0));

    let cbreak_vt = io_TTY_ModePatch_cbreakVT();
    try_(TEST_expect((cbreak_vt.enable & io_TTY_ModeBit_signal_input) != 0));
    try_(TEST_expect((cbreak_vt.enable & io_TTY_ModeBit_vt_input) != 0));
    try_(TEST_expect((cbreak_vt.disable & io_TTY_ModeBit_echo) != 0));
    try_(TEST_expect((cbreak_vt.disable & io_TTY_ModeBit_line_input) != 0));
} $unscoped(TEST_fn);

TEST_fn_("io/TTY: stdio TTY queries match file queries" $scope) {
    var tty = io_TTY_init(test_io_TTY__stdCfg());

    try_(TEST_expect(try_(io_TTY_inputIsTTY(&tty)) == try_(fs_File_isTTY(tty.input_file))));
    try_(TEST_expect(try_(io_TTY_outputIsTTY(&tty)) == try_(fs_File_isTTY(tty.output_file))));
} $unscoped(TEST_fn);

TEST_fn_("io/TTY: enter and leave raw VT mode when stdin is a TTY" $guard) {
    var tty = io_TTY_init(test_io_TTY__stdCfg());
    defer_(io_TTY_fini(&tty));

    catch_((io_TTY_enterMode(&tty, io_TTY_ModePatch_rawVT()))(err, {
        if (E_eql(err.as_any, E_cause$io_TTY_Unsupported().as_any)) {
            try_(TEST_skipMsg(u8_l("TTY mode control is not supported on this platform")));
        }
        if (E_eql(err.as_any, E_cause$io_TTY_NotTTY().as_any)) {
            try_(TEST_skipMsg(u8_l("stdio is not an interactive TTY")));
        }
        return_err(err);
    }));
    try_(TEST_expect(io_TTY_isInEnteredMode(&tty)));
    io_TTY_leaveMode(&tty);
    try_(TEST_expect(!io_TTY_isInEnteredMode(&tty)));
} $unguarded(TEST_fn);
