#include "dh-main.h"
#include "dh/start/Invoc.h"

TEST_fn_("start/Invoc: classic startup inputs expose injectable capabilities" $guard) {
    var_(argv, A$$(1, P_const$u8)) = A_init({
        [0] = as$(P_const$u8)("program"),
    });
    var_(envp, A$$(1, P_const$u8)) = A_init({
        [0] = as$(P_const$u8)("KEY=value"),
    });
    var_(invoc, start_Invoc) $undefined;
    start_Invoc_initArgsEnv(
        &invoc,
        A_len(argv), as$(P_const$P_const$u8)(A_ptr(argv)),
        A_len(envp), as$(P_const$P_const$u8)(A_ptr(envp))
    );
    defer_(start_Invoc_fini(&invoc));

    try_(TEST_expect(proc_Args_isValid(start_Invoc_args(&invoc))));
    try_(TEST_expect(proc_Env_isValid(start_Invoc_env(&invoc))));
    try_(TEST_expect(proc_Preopens_isValid(start_Invoc_preopens(&invoc))));
} $unguarded(TEST_fn);
