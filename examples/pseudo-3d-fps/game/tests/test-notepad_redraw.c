#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

int main() {
    const char *message = "Hello, notepad!";
    int length = strlen(message);
    char buffer[256] = {0};

    HWND notepadWindow = NULL;
    HWND consoleWindow = GetConsoleWindow();

    for (int i = 0; i <= length; i++) {
        strncpy(buffer, message, i);
        buffer[i] = '\0';

        FILE *file = fopen("hello.txt", "w");
        if (file == NULL) {
            printf("Error creating file!");
            return 1;
        }
        fprintf(file, "%s", buffer);
        fclose(file);

        if (notepadWindow == NULL) {
            ShellExecute(NULL, "open", "notepad.exe", "hello.txt", NULL, SW_SHOW);
            Sleep(500);
            notepadWindow = FindWindow("Notepad", NULL);
            // Disable user input to Notepad window
            EnableWindow(notepadWindow, FALSE);
        }

        // Briefly activate Notepad to trigger refresh
        SetForegroundWindow(notepadWindow);
        Sleep(50);

        // Return focus to console
        SetForegroundWindow(consoleWindow);

        Sleep(1000);
    }

    // Re-enable the Notepad window before exiting
    if (notepadWindow) {
        EnableWindow(notepadWindow, TRUE);
    }

    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <windows.h>
// #include <string.h>

// int main() {
//     const char *message = "Hello, notepad!";
//     int length = strlen(message);
//     char buffer[256] = {0};

//     HWND notepadWindow = NULL;
//     HWND consoleWindow = GetConsoleWindow();

//     // Set the console window to always-on-top to maintain focus
//     SetWindowPos(consoleWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

//     // Allow our program to force focus changes
//     AllowSetForegroundWindow(ASFW_ANY);

//     for (int i = 0; i <= length; i++) {
//         strncpy(buffer, message, i);
//         buffer[i] = '\0';

//         FILE *file = fopen("hello.txt", "w");
//         if (file == NULL) {
//             printf("Error creating file!");
//             return 1;
//         }
//         fprintf(file, "%s", buffer);
//         fclose(file);

//         if (notepadWindow == NULL) {
//             ShellExecute(NULL, "open", "notepad.exe", "hello.txt", NULL, SW_SHOW);
//             Sleep(500);
//             notepadWindow = FindWindow("Notepad", NULL);
//             // Make sure Notepad is not topmost
//             SetWindowPos(notepadWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
//         }

//         // Briefly activate Notepad to trigger refresh
//         SetForegroundWindow(notepadWindow);
//         Sleep(50);

//         // Force focus back to console
//         SetForegroundWindow(consoleWindow);
//         SetActiveWindow(consoleWindow);

//         Sleep(1000);
//     }

//     // Remove always-on-top setting when done
//     SetWindowPos(consoleWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <windows.h>
// #include <string.h>

// int main() {
//     const char* message     = "Hello, notepad!";
//     int         length      = strlen(message);
//     char        buffer[256] = { 0 }; // Buffer to store partial message

//     // Get handles to both windows
//     HWND notepadWindow = NULL;
//     HWND consoleWindow = GetConsoleWindow();

//     for (int i = 0; i <= length; i++) {
//         // Copy partial string to buffer
//         strncpy(buffer, message, i);
//         buffer[i] = '\0'; // Ensure null termination

//         // Write to file
//         FILE* file = fopen("hello.txt", "w");
//         if (file == NULL) {
//             printf("Error creating file!");
//             return 1;
//         }
//         fprintf(file, "%s", buffer);
//         fclose(file);

//         // If notepad isn't open yet, open it
//         if (notepadWindow == NULL) {
//             ShellExecute(NULL, "open", "notepad.exe", "hello.txt", NULL, SW_SHOW);
//             Sleep(500); // Wait for notepad to open
//             notepadWindow = FindWindow("Notepad", NULL);
//         }

//         // Briefly activate Notepad to trigger refresh
//         SetForegroundWindow(notepadWindow);
//         Sleep(20); // Brief pause to ensure window activation

//         // Return focus to console window
//         SetForegroundWindow(consoleWindow);

//         // Wait one second
//         Sleep(1000);
//     }

//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <windows.h>
// #include <string.h>

// int main() {
//     const char* message     = "Hello, notepad!";
//     int         length      = strlen(message);
//     char        buffer[256] = { 0 }; // Buffer to store partial message

//     // Get handle to notepad
//     HWND notepadWindow = NULL;

//     for (int i = 0; i <= length; i++) {
//         // Copy partial string to buffer
//         strncpy(buffer, message, i);
//         buffer[i] = '\0'; // Ensure null termination

//         // Write to file
//         FILE* file = fopen("hello.txt", "w");
//         if (file == NULL) {
//             printf("Error creating file!");
//             return 1;
//         }
//         fprintf(file, "%s", buffer);
//         fclose(file);

//         // If notepad isn't open yet, open it
//         if (notepadWindow == NULL) {
//             ShellExecute(NULL, "open", "notepad.exe", "hello.txt", NULL, SW_SHOW);
//             Sleep(500); // Wait for notepad to open
//             notepadWindow = FindWindow("Notepad", NULL);
//         } else {
//             // Refresh notepad content
//             SendMessage(notepadWindow, WM_COMMAND, 0x000F, 0); // Send FILE_REVERT command
//         }

//         // Wait one second
//         Sleep(1000);
//     }

//     return 0;
// }
