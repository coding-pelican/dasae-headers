// `clang test_terminal_launcher.c -o test_terminal_launcher -luser32`
// `.\test_terminal_launcher test_terminal_renderer`


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


static const char* Launcher_WindowTitle = "Test Terminal Launcher";

static const char* Terminal_WindowTitle  = NULL;
static const int   Terminal_WindowWidth  = 80;
static const int   Terminal_WindowHeight = 25;


int main(int argc, const char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <program_to_run>\n", argv[0]);
        return 1;
    }
    Terminal_WindowTitle = argv[1];

    assert(Launcher_WindowTitle);
    assert(Terminal_WindowTitle);

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
        "wt --size %d,%d -d . cmd /k .\\%s",
        Terminal_WindowWidth,
        Terminal_WindowHeight,
        Terminal_WindowTitle
    );

    // Create process
    if (!CreateProcessA(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo)) {
        printf("CreateProcess failed (%d).\n", (int)GetLastError());
        return 1;
    }

    // Wait for the process to finish
    WaitForSingleObject(processInfo.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    printf("Windows Terminal executed successfully.\n");
    return 0;
}
