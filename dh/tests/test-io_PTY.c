#include "dh-main.h"
#include "dh/io/PTY.h"
#include "dh/heap/Sys.h"

TEST_fn_("io/PTY: default configuration is terminal-sized and blocking" $scope) {
    let size = io_PTY_Size_default();
    try_(TEST_expect(size.cols == 80));
    try_(TEST_expect(size.rows == 24));

    let cfg = io_PTY_OpenCfg_default();
    try_(TEST_expect(cfg.size.cols == size.cols));
    try_(TEST_expect(cfg.size.rows == size.rows));
    try_(TEST_expect(!cfg.nonblocking));
    try_(TEST_expect(isNone(cfg.slave_mode)));
} $unscoped(TEST_fn);

TEST_fn_("io/PTY: invalid sizes are rejected before platform open" $guard) {
    var cfg = io_PTY_OpenCfg_default();
    cfg.size = (io_PTY_Size){ .cols = 0, .rows = 24 };

    let rejected = eval_(bool $scope)(catch_((io_PTY_open(cfg))(err, {
        try_(TEST_expect(E_tag$io_PTY_E(err) == E_Tag$io_PTY_InvalidSize));
        $break_(true);
    }))) eval_(else)({
        $break_(false);
    }) $unscoped(eval);
    try_(TEST_expect(rejected));
} $unguarded(TEST_fn);

$static fn_((test_io_PTY_openSupported(io_PTY_OpenCfg cfg))(io_PTY_E$io_PTY) $scope) {
    return_ok(catch_((io_PTY_open(cfg))(err, {
        if (E_tag$io_PTY_E(err) == E_Tag$io_PTY_Unsupported) {
            try_(TEST_skipMsg(u8_l("PTY is not supported on this platform")));
        }
        return_err(err);
    })));
} $unscoped(fn);

TEST_fn_("io/PTY: open resize and close supported PTY" $guard) {
    var cfg = io_PTY_OpenCfg_default();
    cfg.size = (io_PTY_Size){ .cols = 100, .rows = 30 };
    var pty = try_(test_io_PTY_openSupported(cfg));
    defer_(io_PTY_close(&pty));

    try_(io_PTY_resize(&pty, (io_PTY_Size){ .cols = 120, .rows = 40 }));
} $unguarded(TEST_fn);

TEST_fn_("io/PTY: spawn session waits for child termination" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

#if plat_is_windows
    var_(argv, A$$(4, S_const$u8)) = A_init({
        [0] = u8_l("cmd.exe"),
        [1] = u8_l("/c"),
        [2] = u8_l("exit"),
        [3] = u8_l("5"),
    });
#elif plat_is_linux
    var_(argv, A$$(4, S_const$u8)) = A_init({
        [0] = u8_l("/bin/sh"),
        [1] = u8_l("-c"),
        [2] = u8_l("exit 5"),
        [3] = u8_l(""),
    });
#else
    try_(TEST_skipMsg(u8_l("PTY spawn is not supported on this platform")));
#endif

    var cfg = io_PTY_SpawnCfg_default(
        heap_Sys_alctr(&heap),
        (proc_Cmd){
            .argv = A_ref$((S$S_const$u8)(argv)),
            .env = none(),
            .cwd = none(),
            .std_in = proc_StdIO_ignore,
            .std_out = proc_StdIO_ignore,
            .std_err = proc_StdIO_ignore,
            .expand_arg0 = proc_ArgExpansion_no_expand,
            .start_suspended = false,
            .create_no_window = true,
        }
    );
    cfg.pty.size = (io_PTY_Size){ .cols = 80, .rows = 24 };

    var session = catch_((io_PTY_spawn(cfg))(err, {
        if (E_tag$io_PTY_E(err) == E_Tag$io_PTY_Unsupported) {
            try_(TEST_skipMsg(u8_l("PTY spawn is not supported on this platform")));
        }
        return_err(err);
    }));
    defer_(io_PTY_Session_close(&session));

    try_(io_PTY_Session_resize(&session, (io_PTY_Size){ .cols = 100, .rows = 30 }));
    let term = try_(io_PTY_Session_wait(&session));
    try_(TEST_expect(term.tag == proc_Ter_Tag_exited));
    try_(TEST_expect(term.code == 5));
} $unguarded(TEST_fn);
