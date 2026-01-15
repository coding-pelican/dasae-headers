#include "dal-c-ext/proc.h"
#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#endif

int proc_run(char** argv, bool show_output) {
    if (!argv || !argv[0]) { return -1; }
#ifdef _WIN32
    // Build command line
    size_t cmd_len = 0;
    for (int i = 0; argv[i]; ++i) {
        cmd_len += strlen(argv[i]) + 3; // +3 for quotes and space
    }
    char* const cmd_line = (char*)malloc(cmd_len + 1);
    if (!cmd_line) { return -1; }
    cmd_line[0] = '\0';
    for (int i = 0; argv[i]; ++i) {
        if (i > 0) { strcat(cmd_line, " "); }
        strcat(cmd_line, "\"");
        strcat(cmd_line, argv[i]);
        strcat(cmd_line, "\"");
    }

    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);
    if (!show_output) {
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    }
    const BOOL success = CreateProcessA(
        NULL,
        cmd_line,
        NULL,
        NULL,
        show_output,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );
    free(cmd_line);
    if (!success) { return -1; }
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exit_code = 0;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return (int)exit_code;
#else
    const pid_t pid = fork();
    if (pid == -1) { return -1; }
    if (pid == 0) {
        // Child process
        if (!show_output) {
            // Redirect output to /dev/null
            freopen("/dev/null", "w", stdout);
            freopen("/dev/null", "w", stderr);
        }
        execvp(argv[0], argv);
        exit(1); // execvp failed
    } else {
        // Parent process
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) { return WEXITSTATUS(status); }
        return -1;
    }
#endif
}

char* proc_output(char** argv) {
    if (!argv || !argv[0]) { return NULL; }
#ifdef _WIN32
    // Windows: Use CreateProcess with pipe redirection
    HANDLE hRead = NULL;
    HANDLE hWrite = NULL;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) { return NULL; }

    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    // Build command line
    size_t cmd_len = 0;
    for (int i = 0; argv[i]; ++i) { cmd_len += strlen(argv[i]) + 3; }
    char* const cmd_line = (char*)malloc(cmd_len + 1);
    if (!cmd_line) {
        CloseHandle(hRead);
        CloseHandle(hWrite);
        return NULL;
    }
    cmd_line[0] = '\0';
    for (int i = 0; argv[i]; ++i) {
        if (i > 0) { strcat(cmd_line, " "); }
        strcat(cmd_line, "\"");
        strcat(cmd_line, argv[i]);
        strcat(cmd_line, "\"");
    }
    const BOOL success = CreateProcessA(
        NULL,
        cmd_line,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );
    free(cmd_line);
    CloseHandle(hWrite);
    if (!success) { return CloseHandle(hRead), NULL; }
    // Read output
    char buffer[4096] = {};
    size_t total_size = 0;
    char* output = NULL;
    DWORD bytes_read = 0;
    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytes_read, NULL) && bytes_read > 0) {
        buffer[bytes_read] = '\0';
        char* const temp = (char*)realloc(output, total_size + bytes_read + 1);
        if (!temp) {
            free(output);
            CloseHandle(hRead);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return NULL;
        }
        output = temp;
        memcpy(output + total_size, buffer, bytes_read);
        total_size += bytes_read;
    }
    if (output) { output[total_size] = '\0'; }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hRead);
    return output;
#else
    // Unix: Use pipe and fork
    int pipefd[2] = {};
    if (pipe(pipefd) == -1) { return NULL; }
    const pid_t pid = fork();
    if (pid == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return NULL;
    }
    if (pid == 0) {
        // Child process
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);
        execvp(argv[0], argv);
        exit(1);
    } else {
        // Parent process
        close(pipefd[1]);
        char buffer[4096] = {};
        size_t total_size = 0;
        char* output = NULL;
        ssize_t bytes_read = 0;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            char* const temp = (char*)realloc(output, total_size + bytes_read + 1);
            if (!temp) {
                free(output);
                close(pipefd[0]);
                waitpid(pid, NULL, 0);
                return NULL;
            }
            output = temp;
            memcpy(output + total_size, buffer, (size_t)bytes_read);
            total_size += bytes_read;
        }
        if (output) { output[total_size] = '\0'; }
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
        return output;
    }
#endif
}
