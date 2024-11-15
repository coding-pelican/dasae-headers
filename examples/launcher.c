// build `clang -xc launcher.c ../src/*.c -o launcher -O3 -static -luser32`
// run with `.\launcher <program_to_run:game_of_life> <width:160> <height:50>`


#include "dh/core.h"
#include "dh/debug/assert.h"
#include "../dh-terminal/terminal.h"

#include <stdio.h>
#include <stdlib.h>


static const char* const Launcher_WindowTitle = "Test Terminal Launcher";

static const char* Terminal_windowTitle  = null;
static int         Terminal_windowWidth  = 80;
static int         Terminal_windowHeight = 25;


int main(int argc, const char* argv[]) {
    if (argc < 2) {
        printf(
            "[%s] Usage: %s <program_to_run> <width> <height>\n",
            Launcher_WindowTitle,
            argv[0]
        );
        return 1;
    }
    Terminal_windowTitle = argv[1];
    debug_assert(Launcher_WindowTitle);
    debug_assert(Terminal_windowTitle);
    printf("[%s] Terminal: %s\n", Launcher_WindowTitle, Terminal_windowTitle);

    if (2 < argc) {
        Terminal_windowWidth  = atoi(argv[2]);
        Terminal_windowHeight = atoi(argv[3]);
    }
    debug_assert(0 < Terminal_windowWidth);
    debug_assert(0 < Terminal_windowHeight);
    printf(
        "[%s] Terminal size: %dx%d\n",
        Launcher_WindowTitle,
        Terminal_windowWidth,
        Terminal_windowHeight
    );

    STARTUPINFO         startupInfo = { 0 };
    PROCESS_INFORMATION processInfo = { 0 };

    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    ZeroMemory(&processInfo, sizeof(processInfo));

    // Prepare the command string
    char command[1024] = { 0 };
    (void)snprintf(
        command,
        sizeof(command),
        "wt --size %d,%d -d . cmd /k .\\%s %d %d",
        Terminal_windowWidth,
        Terminal_windowHeight,
        Terminal_windowTitle,
        Terminal_windowWidth,
        Terminal_windowHeight
    );

    // Create process
    if (!CreateProcessA(
            null,
            command,
            null,
            null,
            false,
            0,
            null,
            null,
            &startupInfo,
            &processInfo
        )) {
        printf(
            "[%s] CreateProcess failed (%d).\n",
            Launcher_WindowTitle,
            (int)GetLastError()
        );
        return 1;
    }

    // Wait for the process to finish
    WaitForSingleObject(processInfo.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    printf("[%s] Windows Terminal executed successfully.\n", Launcher_WindowTitle);
    return 0;
}
