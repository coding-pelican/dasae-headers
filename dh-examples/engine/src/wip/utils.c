#include "engine-wip/utils.h"
#include "dh/scope/common.h"

#if bti_plat_windows

#include "dh/os/windows.h"

char engine_utils_getch(void) {
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD  mode   = 0;
    char   buf    = 0;

    // Get the console mode
    GetConsoleMode(handle, &mode);
    // Disable echo and line input
    SetConsoleMode(handle, mode & (~ENABLE_ECHO_INPUT) & (~ENABLE_LINE_INPUT));
    // Read a single character
    if_(DWORD read = 0,
        !ReadConsoleA(
            handle,
            &buf,
            1,
            &read,
            null
        ) || read != 1) {
        buf = 0;
    };
    // Restore the console mode
    SetConsoleMode(handle, mode);

    return buf;
}

#else /* bti_plat_unix */

#include <termios.h>
#include <unistd.h>

char engine_utils_getch(void) {
    struct termios old = { 0 };
    struct termios new = { 0 };
    char buf           = 0;

    // Get the current terminal settings
    if (tcgetattr(0, &old) < 0) { return 0; }
    // Copy the old settings to new settings
    new = old;
    // Disable canonical mode and echo
    new.c_lflag &= ~(ICANON | ECHO);
    // Set the new settings
    if (tcsetattr(0, TCSANOW, &new) < 0) { return 0; }
    // Read a single character
    if (read(0, &buf, 1) < 0) { buf = 0; }
    // Restore the old settings
    tcsetattr(0, TCSANOW, &old);

    return buf;
}

#endif /* bti_plat_unix */
