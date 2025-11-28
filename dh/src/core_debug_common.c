#include "dh/core/debug/common.h"

#if plat_is_windows
#include "dh/os/windows.h"
fn_((debug_isDebuggerPresent(void))(bool)) {
    return IsDebuggerPresent();
}
#elif plat_is_linux
#include <stdio.h> /* TODO: Use io or fs instead of this */
fn_((debug_isDebuggerPresent(void))(bool)) {
    /* Check /proc/self/status for TracerPid */
    FILE* f = fopen("/proc/self/status", "r");
    if (f == null) {
        return false;
    }

    char line[256];
    bool result = false;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "TracerPid:", 10) == 0) {
            i32 pid = 0;
            sscanf(line + 10, "%d", &pid);
            result = pid != 0;
            break;
        }
    }

    fclose(f);
    return result;
}
#elif plat_is_darwin
#include <sys/sysctl.h>
#include <unistd.h>
fn_((debug_isDebuggerPresent(void))(bool)) {
    i32 mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    struct kinfo_proc info;
    usize size = sizeof(info);
    if (sysctl(mib, 4, &info, &size, null, 0) == 0) {
        return (info.kp_proc.p_flag & P_TRACED) != 0;
    }
    return false;
}
#else  /* other */
/* Fallback for unsupported platforms - assume no debugger */
fn_((debug_isDebuggerPresent(void))(bool)) {
    return false;
}
#endif /* other */
