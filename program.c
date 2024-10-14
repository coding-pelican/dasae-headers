// On Windows:
// ```shell
// clang test_terminal_program.c -o test_terminal_program.exe '-Wl,-subsystem,windows'
// ```
// On Linux/macOS:
// ```shell
// clang test_terminal_program.c -o test_terminal_program
// ```

#include <math.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#  include <windows.h>
static inline void std_sleep(int seconds) { Sleep(seconds * 1000); }
#else
#  include <unistd.h>
static inline void std_sleep(int seconds) { sleep(seconds); }
#endif

#if !defined(__cplusplus)
#  define nullptr ((void*)0)
#endif

#define unused(x) (void)(x)

void runProgram() {
    time_t start_time = time(nullptr);
    while (1) {
        time_t current_time = time(nullptr);
        double elapsed_time = difftime(current_time, start_time);

        // Example: Write to a log file every 5 seconds
        if (fmod(elapsed_time, 5.0) < 0.1) {
            FILE* log_file = fopen("program_log.txt", "a");
            int   result   = 0;
            if (log_file != nullptr) {
                result = fprintf(log_file, "Program running for %.0f seconds\n", elapsed_time);
                if (result < 0) {
                    perror("fprintf failed");
                }
                result = fclose(log_file);
                if (result < 0) {
                    perror("fclose failed");
                }
            }
        }

        std_sleep(1); // Sleep for 1 second
    }
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) { // NOLINT
    unused(hInstance);
    unused(hPrevInstance);
    unused(lpCmdLine);
    unused(nShowCmd);
    runProgram();
    return 0;
}
#else
int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (0 < pid) {
        printf("Started detached process with PID: %d\n", pid);
        exit(0);
    } else {
        setsid();
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        runProgram();
    }

    return 0;
}
#endif
