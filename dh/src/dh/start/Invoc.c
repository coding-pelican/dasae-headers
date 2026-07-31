#include "dh/start/Invoc.h"
#include "dh/mem/common.h"

#if plat_is_windows
#include "dh/sys/api/windows/proc.h"
#endif /* plat_is_windows */

fn_((start_Invoc_initClassic(start_Invoc* self, usize argc, start_Invoc_ArgVec argv))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(argv);
    *self = union_of$((start_Invoc)(start_Invoc_classic){
        .argc = argc,
        .argv = argv,
    });
};
fn_((start_Invoc_initPosix(start_Invoc* self, usize argc, start_Invoc_ArgVec argv, start_Invoc_ArgVec envp))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(argv);
    claim_assert_nonnull(envp);
    var_(envc, usize) = 0;
    while (*P_at((envp)[envc]) != null) ++envc;
    *self = union_of$((start_Invoc)(start_Invoc_posix){
        .argc = argc,
        .argv = argv,
        .envc = envc,
        .envp = envp,
    });
};
#if plat_is_windows
fn_((start_Invoc_initWin32(start_Invoc* self))(void)) {
    claim_assert_nonnull(self);
    let cmd_line_ptr = as$(P_const$u16)(GetCommandLineW());
    claim_assert_nonnull(cmd_line_ptr);
    var_(command_line_len, usize) = 0;
    while (*P_at((cmd_line_ptr)[command_line_len]) != 0) ++command_line_len;

    let env_ptr = as$(P_const$u16)(GetEnvironmentStringsW());
    var_(env_block, O$S_const$u16) = none();
    if (env_ptr != null) {
        var_(len, usize) = 0;
        while (!(*P_at((env_ptr)[len]) == 0 && *P_at((env_ptr)[len + 1]) == 0)) ++len;
        len += 2;
        asg_l((&env_block)(some(P_prefix$((S_const$u16)(env_ptr)(len)))));
    }
    *self = union_of$((start_Invoc)(start_Invoc_win32){
        .cmd_line = P_prefix$((S_const$u16)(cmd_line_ptr)(command_line_len)),
        .env_block = env_block,
    });
};
#endif /* plat_is_windows */
#if plat_is_linux
fn_((start_Invoc_initLinux(start_Invoc* self, P$raw initial_stack))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(initial_stack);
    let words = as$(P$usize)(initial_stack);
    let argc = *P_at((words)[0]);
    let argv = as$(start_Invoc_ArgVec)(P_at((words)[1]));
    let envp = P_at((argv)[argc + 1]);
    start_Invoc_initPosix(self, argc, argv, envp);
};
#endif /* plat_is_linux */

fn_((start_Invoc_fini(start_Invoc* self))(void)) {
    claim_assert_nonnull(self);
#if plat_is_windows
    if (matches(*self, start_Invoc_win32)) {
        let win32 = union_as((self)(start_Invoc_win32));
        if_some((win32->env_block)(env_block)) {
            claim_assert(FreeEnvironmentStringsW(as$(LPWCH)(constCast(env_block.ptr))));
        }
    }
#endif /* plat_is_windows */
};

fn_((start_Invoc_env(start_Invoc* self))(proc_Env)) {
    claim_assert_nonnull(self);
    if (matches(*self, start_Invoc_posix)) {
        let posix = union_as((self)(start_Invoc_posix));
        return proc_Env_fromPosix(posix->envc, posix->envp);
    }
    if (matches(*self, start_Invoc_win32)) {
        let win32 = union_as((self)(start_Invoc_win32));
        return isSome(win32->env_block)
                 ? proc_Env_fromWin32(unwrap_(win32->env_block))
                 : proc_Env_none;
    }
    return proc_Env_none;
};

fn_((start_Invoc_args(start_Invoc* self))(proc_Args)) {
    claim_assert_nonnull(self);
    if (matches(*self, start_Invoc_classic)) {
        let classic = union_as((self)(start_Invoc_classic));
        return proc_Args_fromPosix(classic->argc, classic->argv);
    }
    if (matches(*self, start_Invoc_posix)) {
        let posix = union_as((self)(start_Invoc_posix));
        return proc_Args_fromPosix(posix->argc, posix->argv);
    }
    let win32 = union_as((self)(start_Invoc_win32));
    return proc_Args_fromWin32(win32->cmd_line);
};
