#include <stdio.h>

#ifdef _WIN32
#  include <windows.h>
#else // !_WIN32
#  include <stdlib.h>
#  include <string.h>
#  include <unistd.h>
#endif // _WIN32


int supportsANSIEscapeSequences() {
#ifdef _WIN32
    // Windows-specific code
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return 0; // Failed to get console handle
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return 0; //  Failed to get console mode
    }

    return (dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0; //  Check if ANSI escape sequences are supported
#else // !_WIN32
    // Linux/Unix-specific code
    if (!isatty(STDOUT_FILENO)) {
        return 0; // Not a terminal
    }

    char* term = getenv("TERM");
    if (term == NULL) {
        return 0; // TERM not set
    }

    // List of terminal types known to support ANSI escape sequences
    const char*  supported_terms[]    = { "xterm", "xterm-color", "xterm-256color", "screen", "linux", "cygwin" };
    const size_t supported_term_count = sizeof(supported_terms) / sizeof(supported_terms[0]);
    for (size_t i = 0; i < supported_term_count; ++i) {
        if (strcmp(term, supported_terms[i]) == 0) {
            return 1; // Supported terminal type found
        }
    }

    return 0; // Unknown terminal type
#endif // _WIN32
}


int main() {
    if (supportsANSIEscapeSequences()) {
        printf("This terminal supports ANSI escape sequences.\n");
        // Use ANSI escape sequences here
    } else {
        printf("This terminal may not support ANSI escape sequences.\n");
        // Use alternative output methods
    }
    return 0;
}
