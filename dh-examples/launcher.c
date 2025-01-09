// build `clang -xc launcher.c ../src/*.c -o launcher -O3 -static -luser32`
// run with `.\launcher <program_to_run:game_of_life> <width:160> <height:50>`


#include "dh/core.h"
#include "dh/debug/assert.h"
#include "../dh-terminal/terminal.h"

#include <stdio.h>
#include <stdlib.h>


static const char* const Launcher_window_title = "Test Terminal Launcher";

static const char* Terminal_window_title  = null;
static i32         Terminal_window_width  = 80;
static i32         Terminal_window_height = 25;


i32 main(i32 argc, const char* argv[]) {
    if (argc < 2) {
        printf(
            "[%s] Usage: %s <program_to_run> <width> <height>\n",
            Launcher_window_title,
            argv[0]
        );
        return 1;
    }
    Terminal_window_title = argv[1];
    debug_assert(Launcher_window_title);
    debug_assert(Terminal_window_title);
    printf("[%s] Terminal: %s\n", Launcher_window_title, Terminal_window_title);

    if (2 < argc) {
        Terminal_window_width  = atoi(argv[2]);
        Terminal_window_height = atoi(argv[3]);
    }
    debug_assert(0 < Terminal_window_width);
    debug_assert(0 < Terminal_window_height);
    printf(
        "[%s] Terminal size: %dx%d\n",
        Launcher_window_title,
        Terminal_window_width,
        Terminal_window_height
    );

    STARTUPINFO         startup_info = { 0 };
    PROCESS_INFORMATION process_info = { 0 };

    ZeroMemory(&startup_info, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);
    ZeroMemory(&process_info, sizeof(process_info));

    // Prepare the command string
    char command[1024] = { 0 };
    (void)snprintf(
        command,
        sizeof(command),
        "wt --size %d,%d -d . cmd /k .\\%s %d %d",
        Terminal_window_width,
        Terminal_window_height,
        Terminal_window_title,
        Terminal_window_width,
        Terminal_window_height
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
            &startup_info,
            &process_info
        )) {
        printf(
            "[%s] CreateProcess failed (%d).\n",
            Launcher_window_title,
            (i32)GetLastError()
        );
        return 1;
    }

    // Wait for the process to finish
    WaitForSingleObject(process_info.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    printf("[%s] Windows Terminal executed successfully.\n", Launcher_window_title);
    return 0;
}
