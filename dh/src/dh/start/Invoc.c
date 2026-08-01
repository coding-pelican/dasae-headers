#include "dh/start/Invoc.h"
#include "dh/mem/common.h"

#if plat_is_windows
#include "dh/sys/api/windows/proc.h"
#endif /* plat_is_windows */

$static fn_((start_Invoc__preopens(void))(proc_Preopens)) {
    return catch_((proc_Preopens_direct())($ignore, proc_Preopens_empty));
};

fn_((start_Invoc_initArgs(
    start_Invoc* self,
    usize argc, P_const$P_const$u8 argv
))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(argv);
    *self = (start_Invoc){
        .args = start_Invoc_Args_initVecZ(argc, argv),
        .env = start_Invoc_Env_initEmpty(),
        .preopens = start_Invoc__preopens(),
        .owned_env_block = none(),
    };
};
fn_((start_Invoc_initArgsEnv(
    start_Invoc* self,
    usize argc, P_const$P_const$u8 argv,
    usize envc, P_const$P_const$u8 envp
))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(argv);
    claim_assert_nonnull(envp);
    *self = (start_Invoc){
        .args = start_Invoc_Args_initVecZ(argc, argv),
        .env = start_Invoc_Env_initVecZ(envc, envp),
        .preopens = start_Invoc__preopens(),
        .owned_env_block = none(),
    };
};
fn_((start_Invoc_initArgsEnvZ(
    start_Invoc* self,
    usize argc, P_const$P_const$u8 argv,
    P_const$P_const$u8 envp
))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(argv);
    claim_assert_nonnull(envp);
    start_Invoc_initArgsEnv(
        self,
        argc, argv,
        mem_lenZ0(u_anyP(envp)), envp
    );
};
#if plat_is_windows
fn_((start_Invoc_initWin32(start_Invoc* self))(void)) {
    claim_assert_nonnull(self);
    let cmd_line_ptr = ensureNonnull(as$(P_const$u16)(GetCommandLineW()));
    var_(command_line_len, usize) = 0;
    while (*P_at((cmd_line_ptr)[command_line_len]) != 0) ++command_line_len;

    let env_ptr = as$(P_const$u16)(GetEnvironmentStringsW());
    var_(env, start_Invoc_Env) = start_Invoc_Env_initEmpty();
    var_(owned_env_block, O$S_const$u16) = none();
    if (env_ptr != null) {
        var_(len, usize) = 0;
        while (!(*P_at((env_ptr)[len]) == 0 && *P_at((env_ptr)[len + 1]) == 0)) ++len;
        len += 2;
        let block = P_prefix$((S_const$u16)(env_ptr)(len));
        env = start_Invoc_Env_initWTF16(block);
        asg_l((&owned_env_block)(some(block)));
    }
    *self = (start_Invoc){
        .args = start_Invoc_Args_initWTF16(
            P_prefix$((S_const$u16)(cmd_line_ptr)(command_line_len))
        ),
        .env = env,
        .preopens = start_Invoc__preopens(),
        .owned_env_block = owned_env_block,
    };
};
#endif /* plat_is_windows */
#if in_analysis || plat_is_linux
fn_((start_Invoc_initLinux(start_Invoc* self, P$raw initial_stack))(void)) {
    claim_assert_nonnull(self);
    let words = ensureNonnull(as$(P$usize)(initial_stack));
    let argc = *P_at((words)[0]);
    let argv = as$(P_const$P_const$u8)(P_at((words)[1]));
    let envp = P_at((argv)[argc + 1]);
    start_Invoc_initArgsEnvZ(
        self,
        argc, argv,
        envp
    );
};
#endif /* in_analysis || plat_is_linux */
fn_((start_Invoc_fini(start_Invoc* self))(void)) {
    claim_assert_nonnull(self);
#if plat_is_windows
    if_some((self->owned_env_block)(env_block)) {
        claim_assert(FreeEnvironmentStringsW(as$(LPWCH)(constCast(env_block.ptr))));
        asg_l((&self->owned_env_block)(none()));
    }
#endif /* plat_is_windows */
};

fn_((start_Invoc_args(start_Invoc* self))(proc_Args)) {
    claim_assert_nonnull(self);
    return start_Invoc_Args_self(&self->args);
};
fn_((start_Invoc_env(start_Invoc* self))(proc_Env)) {
    claim_assert_nonnull(self);
    return start_Invoc_Env_self(&self->env);
};
fn_((start_Invoc_preopens(start_Invoc* self))(proc_Preopens)) {
    claim_assert_nonnull(self);
    return proc_Preopens_ensureValid(self->preopens);
};
