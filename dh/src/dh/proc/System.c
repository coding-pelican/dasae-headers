#include "dh/proc/Self.h"
#include "dh/fs/File.h"
#include "dh/start.h"

#if plat_is_windows
#include "dh/sys/api/windows/dll.h"
#endif /* plat_is_windows */

$extern fn_((proc__directGetUserInfo(P$raw ctx, S_const$u8 name))(proc_GetUserInfo_E$proc_UserInfo));
$extern fn_((proc__directGetBaseAddress(P$raw ctx))(proc_GetBaseAddress_E$usize));
$attr($no_return)
$extern fn_((proc__directExit(P$raw ctx, u8 status))(void));
$attr($no_return)
$extern fn_((proc__directAbort(P$raw ctx))(void));

$attr($must_check)
fn_((proc__directGetUserInfo(
    P$raw ctx,
    S_const$u8 name
))(proc_GetUserInfo_E$proc_UserInfo) $guard) {
    claim_assert_nonnull(ctx);
    pp_if_(plat_is_linux)(
        pp_then_(
            let file = catch_((fs_File_open(
                u8_l("/etc/passwd"), fs_File_OpenFlags_default
            ))($ignore, return_err(E_cause$proc_GetUserInfo_SystemResources())));
            defer_(fs_File_close(file));

            typedef enum proc__PasswdState {
                proc__PasswdState_name,
                proc__PasswdState_wait_line,
                proc__PasswdState_password,
                proc__PasswdState_uid,
                proc__PasswdState_gid,
            } proc__PasswdState;
            var_(state, proc__PasswdState) = proc__PasswdState_name;
            var_(name_idx, usize) = 0;
            var_(uid, u32) = 0;
            var_(gid, u32) = 0;
            var_(have_uid, bool) = false;
            var_(have_gid, bool) = false;
            var_(offset, u64) = 0;
            var_(buf, A$$(2048, u8)) $undefined;

            while (true) {
                let n = catch_((fs_File_readPos(
                    file, A_ref$((S$u8)(buf)), offset
                ))($ignore, return_err(E_cause$proc_GetUserInfo_SystemResources())));
                if (n == 0) break;
                offset += n;
                for_(($s(A_prefix$((S_const$u8)(buf)(n))))(byte_ptr)) {
                    let byte = *byte_ptr;
                    switch (state) {
                        case_((proc__PasswdState_name)) {
                            if (byte == ':') {
                                state = name_idx == name.len
                                    ? proc__PasswdState_password
                                    : proc__PasswdState_wait_line;
                            } else if (byte == '\n') {
                                name_idx = 0;
                            } else if (name_idx >= name.len || *S_at((name)[name_idx]) != byte) {
                                state = proc__PasswdState_wait_line;
                            } else {
                                ++name_idx;
                            }
                        } $end(case);
                        case_((proc__PasswdState_wait_line)) {
                            if (byte == '\n') {
                                state = proc__PasswdState_name;
                                name_idx = 0;
                            }
                        } $end(case);
                        case_((proc__PasswdState_password)) {
                            if (byte == ':') {
                                state = proc__PasswdState_uid;
                            } else if (byte == '\n') {
                                return_err(E_cause$proc_GetUserInfo_CorruptDatabase());
                            }
                        } $end(case);
                        case_((proc__PasswdState_uid)) {
                            if (byte == ':') {
                                if (!have_uid) return_err(E_cause$proc_GetUserInfo_CorruptDatabase());
                                state = proc__PasswdState_gid;
                            } else if (byte >= '0' && byte <= '9') {
                                let digit = as$(u32)(byte - '0');
                                uid = orelse_((u32_mulChkd(uid, 10))(
                                    return_err(E_cause$proc_GetUserInfo_CorruptDatabase())
                                ));
                                uid = orelse_((u32_addChkd(uid, digit))(
                                    return_err(E_cause$proc_GetUserInfo_CorruptDatabase())
                                ));
                                have_uid = true;
                            } else {
                                return_err(E_cause$proc_GetUserInfo_CorruptDatabase());
                            }
                        } $end(case);
                        case_((proc__PasswdState_gid)) {
                            if (byte == ':') {
                                if (!have_gid) return_err(E_cause$proc_GetUserInfo_CorruptDatabase());
                                return_ok((proc_UserInfo){ .uid = uid, .gid = gid });
                            } else if (byte >= '0' && byte <= '9') {
                                let digit = as$(u32)(byte - '0');
                                gid = orelse_((u32_mulChkd(gid, 10))(
                                    return_err(E_cause$proc_GetUserInfo_CorruptDatabase())
                                ));
                                gid = orelse_((u32_addChkd(gid, digit))(
                                    return_err(E_cause$proc_GetUserInfo_CorruptDatabase())
                                ));
                                have_gid = true;
                            } else {
                                return_err(E_cause$proc_GetUserInfo_CorruptDatabase());
                            }
                        } $end(case);
                        default_() claim_unreachable; $end(default);
                    }
                } $end(for);
            }
            return_err(E_cause$proc_GetUserInfo_UserNotFound());
        ),
        pp_else_(
            let_ignore = name;
            return_err(E_cause$proc_GetUserInfo_Unsupported());
        )
    );
} $unguarded(fn);

$attr($must_check)
fn_((proc__directGetBaseAddress(
    P$raw ctx
))(proc_GetBaseAddress_E$usize) $guard) {
    claim_assert_nonnull(ctx);
    pp_if_(plat_is_windows)(
        pp_then_(
            let module = GetModuleHandleW(null);
            if (isNull(module)) {
                return_err(E_cause$proc_GetBaseAddress_SystemResources());
            }
            return_ok(as$(usize)(module));
        ),
        pp_else_(pp_if_(plat_is_linux)(
            pp_then_(
                let file = catch_((fs_File_open(
                    u8_l("/proc/self/maps"), fs_File_OpenFlags_default
                ))($ignore, return_err(E_cause$proc_GetBaseAddress_SystemResources())));
                defer_(fs_File_close(file));
                var_(buf, A$$(128, u8)) $undefined;
                let n = catch_((fs_File_readPos(
                    file, A_ref$((S$u8)(buf)), 0
                ))($ignore, return_err(E_cause$proc_GetBaseAddress_SystemResources())));
                if (n == 0) return_err(E_cause$proc_GetBaseAddress_CorruptImage());
                var_(value, usize) = 0;
                var_(have_digit, bool) = false;
                for_(($s(A_prefix$((S_const$u8)(buf)(n))))(byte_ptr)) {
                    let byte = *byte_ptr;
                    if (byte == '-') break;
                    let_(digit, u8) = byte >= '0' && byte <= '9'
                        ? byte - '0'
                        : byte >= 'a' && byte <= 'f'
                            ? byte - 'a' + 10
                            : byte >= 'A' && byte <= 'F'
                                ? byte - 'A' + 10
                                : 0xff;
                    if (digit == 0xff) {
                        return_err(E_cause$proc_GetBaseAddress_CorruptImage());
                    }
                    value = orelse_((usize_mulChkd(value, usize_(16)))(
                        return_err(E_cause$proc_GetBaseAddress_CorruptImage())
                    ));
                    value = orelse_((usize_addChkd(value, digit))(
                        return_err(E_cause$proc_GetBaseAddress_CorruptImage())
                    ));
                    have_digit = true;
                } $end(for);
                if (!have_digit) return_err(E_cause$proc_GetBaseAddress_CorruptImage());
                return_ok(value);
            ),
            pp_else_(
                return_err(E_cause$proc_GetBaseAddress_Unsupported());
            )
        ))
    );
} $unguarded(fn);

$attr($no_return)
fn_((proc__directExit(P$raw ctx, u8 status))(void)) {
    claim_assert_nonnull(ctx);
    start_exit(status);
}

$attr($no_return $branch_cold)
fn_((proc__directAbort(P$raw ctx))(void)) {
    claim_assert_nonnull(ctx);
    start_exit(134);
}
