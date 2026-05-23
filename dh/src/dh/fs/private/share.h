#ifndef dh_src_fs_private_share__included
#define dh_src_fs_private_share__included 1

#include "dh/fs/common.h"
#include "dh/mem/common.h"

#define fs__path_max (usize_(1024))

$attr($inline_always)
$static fn_((fs__pathZ(S_const$u8 path, u8* out, usize out_len))(bool)) {
    if (path.len + 1 > out_len) return false;
    let out_buf = P_prefix$((S$u8)(out)(out_len));
    mem_copyBytes(S_prefix((out_buf)(path.len)), path);
    *S_at((out_buf)[path.len]) = 0;
    return true;
};

#if plat_is_windows
$attr($inline_always)
$static fn_((fs__trimWinNtPrefix(S$u8 path))(S$u8)) {
    if (path.len < 4) return path;
    if (
        *S_at((path)[0]) == u8_c('\\') &&
        *S_at((path)[1]) == u8_c('\\') &&
        *S_at((path)[2]) == u8_c('?') &&
        *S_at((path)[3]) == u8_c('\\')
    ) {
        return S_suffix((path)(4));
    }
    return path;
};
#endif /* plat_is_windows */

#endif /* dh_src_fs_private_share__included */
