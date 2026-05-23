#include "dh/core/debug/common.h"

#if plat_is_windows
#include "dh/os/windows/debug.h"
fn_((debug_isDebuggerPresent(void))(bool)) {
    return IsDebuggerPresent();
};

#elif plat_is_linux
#include "dh/mem/common.h"
#include <fcntl.h>
#include <unistd.h>
fn_((debug_isDebuggerPresent(void))(bool)) {
    let fd = open("/proc/self/status", O_RDONLY);
    if (fd < 0) {
        return false;
    }

    var_(buf, A$$(4096, u8)) $undefined;
    let read_len = read(fd, A_ptr(buf), A_len(buf));
    let_ignore = close(fd);
    if (read_len <= 0) return false;

    let bytes = A_slice$((S_const$u8)(buf)$r(0, as$(usize)(read_len)));
    let marker = u8_l("TracerPid:");
    var_(result, bool) = false;
    var_(line_start, usize) = 0;
    while (line_start < bytes.len) {
        var_(line_end, usize) = line_start;
        while (line_end < bytes.len && *S_at((bytes)[line_end]) != u8_c('\n')) {
            line_end++;
        }
        let line = S_slice((bytes)$r(line_start, line_end));
        if (mem_startsWithBytes(line, marker)) {
            var_(idx, usize) = marker.len;
            while (idx < line.len && *S_at((line)[idx]) == u8_c('\t')) idx++;
            while (idx < line.len && *S_at((line)[idx]) == u8_c(' ')) idx++;
            while (idx < line.len) {
                let ch = *S_at((line)[idx]);
                if (ch < u8_c('0') || ch > u8_c('9')) break;
                if (ch != u8_c('0')) {
                    result = true;
                    break;
                }
                idx++;
            }
            break;
        }
        line_start = line_end + 1;
    }
    return result;
};

#elif plat_is_darwin
#include <sys/sysctl.h>
#include <unistd.h>
fn_((debug_isDebuggerPresent(void))(bool)) {
    var mib = A_from$((i32){ CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() });
    var_(info, struct kinfo_proc) $undefined;
    var_(size, usize) = sizeOf$(TypeOf(info));
    if (sysctl(A_ptr(mib), A_len(mib), &info, &size, null, 0) == 0) {
        return (info.kp_proc.p_flag & P_TRACED) != 0;
    }
    return false;
};

#else /* other */
/* Fallback for unsupported platforms - assume no debugger */
fn_((debug_isDebuggerPresent(void))(bool)) {
    return false;
};
#endif /* other */
