#include "dh/debug/common.h"

#if defined(_WIN32) || defined(_WIN64)
#include "dh/os/windows.h"
bool debug_isDebuggerPresent(void) {
    return IsDebuggerPresent();
}
#elif defined(__APPLE__)
#include <sys/sysctl.h>
bool debug_isDebuggerPresent(void) {
    i32               mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    struct kinfo_proc info;
    usize             size = sizeof(info);
    if (sysctl(mib, 4, &info, &size, null, 0) == 0) {
        return (info.kp_proc.p_flag & P_TRACED) != 0;
    }
    return false;
}
#elif defined(__linux__)
#include <stdio.h>
bool debug_isDebuggerPresent(void) {
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
#else  /* others */
/* Fallback for unsupported platforms - assume no debugger */
bool debug_isDebuggerPresent(void) {
    return false;
}
#endif /* others */
