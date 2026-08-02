#include "dh-main.h"
#include "dh/io/PTY.h"
#include "dh/heap/Sys.h"
#include "dh/start/Invoc/Env.h"

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
        try_(TEST_expect(E_eql(
            err.as_any,
            E_cause$io_PTY_InvalidSize().as_any
        )));
        $break_(true);
    }))) eval_(else)({
        $break_(false);
    }) $unscoped(eval);
    try_(TEST_expect(rejected));
} $unguarded(TEST_fn);

$static fn_((test_io_PTY_openSupported(io_PTY_OpenCfg cfg))(io_PTY_OpenE$io_PTY) $scope) {
    return_ok(catch_((io_PTY_open(cfg))(err, {
        if (E_eql(err.as_any, E_cause$io_PTY_Unsupported().as_any)) {
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
        [1] = u8_l("/D"),
        [2] = u8_l("/C"),
        [3] = u8_l("if \"%DH_PTY_SSO%\"==\"inherited\" (exit /b 0) else exit /b 9"),
    });
#elif plat_is_linux
    var_(argv, A$$(4, S_const$u8)) = A_init({
        [0] = u8_l("/bin/sh"),
        [1] = u8_l("-c"),
        [2] = u8_l("test \"$DH_PTY_SSO\" = inherited"),
        [3] = u8_l("test-io-pty"),
    });
#else
    try_(TEST_skipMsg(u8_l("PTY spawn is not supported on this platform")));
#endif

    let gpa = heap_Sys_alctr(&heap);
    let proc = try_(proc_direct());
    var_(env_items, A$$(1, P_const$u8)) = A_init({
        [0] = as$(P_const$u8)("DH_PTY_SSO=inherited"),
    });
    var env_direct = start_Invoc_Env_initVecZ(
        A_len(env_items),
        as$(P_const$P_const$u8)(A_ptr(env_items))
    );
    var cfg = io_PTY_SpawnCfg_default(
        gpa,
        start_Invoc_Env_self(&env_direct),
        (proc_Spawn_Opts){
            .argv = A_ref$((S$S_const$u8)(argv)),
            .env = none(),
            .cwd = union_of((proc_Spawn_CWD_inherit){}),
            .std_in = union_of((proc_Spawn_StdIO_ignore){}),
            .std_out = union_of((proc_Spawn_StdIO_ignore){}),
            .std_err = union_of((proc_Spawn_StdIO_ignore){}),
            .expand_arg0 = proc_ArgExpsn_no_expand,
            .start_suspended = false,
            .create_no_window = true,
        }
    );
    cfg.pty.size = (io_PTY_Size){ .cols = 80, .rows = 24 };

    var session = catch_((io_PTY_spawn(cfg))(err, {
        if (E_eql(err.as_any, E_cause$io_PTY_Unsupported().as_any)) {
            try_(TEST_skipMsg(u8_l("PTY spawn is not supported on this platform")));
        }
        return_err(err);
    }));
    defer_(io_PTY_Session_close(&session, proc));

    try_(io_PTY_Session_resize(&session, (io_PTY_Size){ .cols = 100, .rows = 30 }));
    let term = try_(io_PTY_Session_wait(&session, proc));
    try_(TEST_expect(matches(term, proc_Child_Trm_exited)));
    try_(TEST_expect(union_to((term)(proc_Child_Trm_exited)) == 0));
} $unguarded(TEST_fn);
