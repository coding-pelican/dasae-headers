#include "dal-c-ext/proc.h"
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#ifdef __linux__
#include <sys/prctl.h>
#endif
#endif

#ifdef _WIN32
static char* proc__buildCommandLine(const char** argv) {
    size_t cmd_len = 0;
    for (int i = 0; argv[i]; ++i) {
        cmd_len += strlen(argv[i]) + 3;
    }

    char* const cmd_line = (char*)malloc(cmd_len + 1);
    if (!cmd_line) { return NULL; }

    cmd_line[0] = '\0';
    for (int i = 0; argv[i]; ++i) {
        if (i > 0) { strcat(cmd_line, " "); }
        strcat(cmd_line, "\"");
        strcat(cmd_line, argv[i]);
        strcat(cmd_line, "\"");
    }
    return cmd_line;
}
#endif
#ifndef _WIN32
static volatile sig_atomic_t proc__pending_signal = 0;
static volatile sig_atomic_t proc__active_child_pgid = 0;

static void proc__handleSignal(int sig) {
    proc__pending_signal = sig;
    if (proc__active_child_pgid > 0) {
        (void)kill(-(pid_t)proc__active_child_pgid, sig);
    }
}

static void proc__freeMutableArgv(char** argv) {
    if (!argv) { return; }
    for (size_t i = 0; argv[i] != NULL; ++i) {
        free(argv[i]);
    }
    free(argv);
}

static char** proc__makeMutableArgv(const char** argv) {
    if (!argv) { return NULL; }

    size_t count = 0;
    while (argv[count] != NULL) { ++count; }

    char** result = (char**)calloc(count + 1, sizeof(char*));
    if (!result) { return NULL; }

    for (size_t i = 0; i < count; ++i) {
        const size_t len = strlen(argv[i]);
        result[i] = (char*)malloc(len + 1);
        if (!result[i]) {
            proc__freeMutableArgv(result);
            return NULL;
        }
        memcpy(result[i], argv[i], len + 1);
    }
    result[count] = NULL;
    return result;
}
#endif

int proc_run(const char** argv, bool show_output) {
    if (!argv || !argv[0]) { return -1; }
#ifdef _WIN32
    char* const cmd_line = proc__buildCommandLine(argv);
    if (!cmd_line) { return -1; }

    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);
    if (!show_output) {
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    }
    HANDLE job = CreateJobObjectA(NULL, NULL);
    if (job) {
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
        memset(&info, 0, sizeof(info));
        info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation, &info, sizeof(info))) {
            CloseHandle(job);
            job = NULL;
        }
    }

    const BOOL success = CreateProcessA(
        NULL,
        cmd_line,
        NULL,
        NULL,
        !show_output,
        CREATE_NEW_PROCESS_GROUP,
        NULL,
        NULL,
        &si,
        &pi
    );
    free(cmd_line);
    if (!success) {
        if (job) { CloseHandle(job); }
        return -1;
    }
    if (job) {
        (void)AssignProcessToJobObject(job, pi.hProcess);
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exit_code = 0;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    if (job) { CloseHandle(job); }
    return (int)exit_code;
#else
    const pid_t pid = fork();
    if (pid == -1) { return -1; }
    if (pid == 0) {
        // Child process. Use a process group so dh-c can terminate the whole
        // build subtree (make + compiler workers) when dh-c is interrupted.
        (void)setpgid(0, 0);
#ifdef __linux__
        // If dh-c is killed by an outer supervisor before it can forward a signal,
        // terminate the direct child (usually make) instead of leaving a build tree
        // behind. make normally forwards/cleans up its compiler workers.
        (void)prctl(PR_SET_PDEATHSIG, SIGTERM);
        if (getppid() == 1) { _exit(128 + SIGTERM); }
#endif
        if (!show_output) {
            // Redirect output to /dev/null
            freopen("/dev/null", "w", stdout);
            freopen("/dev/null", "w", stderr);
        }
        char** const exec_argv = proc__makeMutableArgv(argv);
        if (!exec_argv) { exit(1); }
        execvp(exec_argv[0], exec_argv);
        proc__freeMutableArgv(exec_argv);
        exit(1); // execvp failed
    } else {
        // Parent process
        (void)setpgid(pid, pid);

        struct sigaction old_int = { 0 };
        struct sigaction old_term = { 0 };
        struct sigaction action = { 0 };
        action.sa_handler = proc__handleSignal;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        proc__pending_signal = 0;
        proc__active_child_pgid = (sig_atomic_t)pid;
        (void)sigaction(SIGINT, &action, &old_int);
        (void)sigaction(SIGTERM, &action, &old_term);

        int status = 0;
        for (;;) {
            pid_t waited = waitpid(pid, &status, 0);
            if (waited == pid) { break; }
            if (waited == -1 && errno == EINTR && proc__pending_signal != 0) {
                const int sig = (int)proc__pending_signal;
                (void)kill(-pid, sig);
                (void)waitpid(pid, &status, 0);
                proc__active_child_pgid = 0;
                (void)sigaction(SIGINT, &old_int, NULL);
                (void)sigaction(SIGTERM, &old_term, NULL);
                return 128 + sig;
            }
            if (waited == -1 && errno != EINTR) {
                proc__active_child_pgid = 0;
                (void)sigaction(SIGINT, &old_int, NULL);
                (void)sigaction(SIGTERM, &old_term, NULL);
                return -1;
            }
        }

        proc__active_child_pgid = 0;
        (void)sigaction(SIGINT, &old_int, NULL);
        (void)sigaction(SIGTERM, &old_term, NULL);
        if (WIFEXITED(status)) { return WEXITSTATUS(status); }
        if (WIFSIGNALED(status)) { return 128 + WTERMSIG(status); }
        return -1;
    }
#endif
}

int proc_runMergedOutput(const char** argv, bool show_output) {
    if (!argv || !argv[0]) { return -1; }
#ifdef _WIN32
    if (!show_output) {
        return proc_run(argv, false);
    }

    HANDLE hRead = NULL;
    HANDLE hWrite = NULL;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) { return -1; }
    (void)SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

    char* const cmd_line = proc__buildCommandLine(argv);
    if (!cmd_line) {
        CloseHandle(hRead);
        CloseHandle(hWrite);
        return -1;
    }

    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;

    HANDLE job = CreateJobObjectA(NULL, NULL);
    if (job) {
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
        memset(&info, 0, sizeof(info));
        info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation, &info, sizeof(info))) {
            CloseHandle(job);
            job = NULL;
        }
    }

    const BOOL success = CreateProcessA(
        NULL,
        cmd_line,
        NULL,
        NULL,
        TRUE,
        CREATE_NEW_PROCESS_GROUP,
        NULL,
        NULL,
        &si,
        &pi
    );
    free(cmd_line);
    CloseHandle(hWrite);
    if (!success) {
        CloseHandle(hRead);
        if (job) { CloseHandle(job); }
        return -1;
    }
    if (job) {
        (void)AssignProcessToJobObject(job, pi.hProcess);
    }

    char buffer[2048];
    DWORD bytes_read = 0;
    while (ReadFile(hRead, buffer, sizeof(buffer), &bytes_read, NULL) && bytes_read > 0) {
        (void)fwrite(buffer, 1, bytes_read, stdout);
        (void)fflush(stdout);
    }
    CloseHandle(hRead);

    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD exit_code = 0;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    if (job) { CloseHandle(job); }
    return (int)exit_code;
#else
    return proc_run(argv, show_output);
#endif
}

char* proc_output(const char** argv) {
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
    char* const cmd_line = proc__buildCommandLine(argv);
    if (!cmd_line) {
        CloseHandle(hRead);
        CloseHandle(hWrite);
        return NULL;
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
    char buffer[2048] = {};
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
        char** const exec_argv = proc__makeMutableArgv(argv);
        if (!exec_argv) { exit(1); }
        execvp(exec_argv[0], exec_argv);
        proc__freeMutableArgv(exec_argv);
        exit(1);
    } else {
        // Parent process
        close(pipefd[1]);
        char buffer[2048] = {};
        size_t total_size = 0;
        char* output = NULL;
        ssize_t bytes_read = 0;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            const size_t bytes_count = (size_t)bytes_read;
            buffer[bytes_count] = '\0';
            char* const temp = (char*)realloc(output, total_size + bytes_count + 1);
            if (!temp) {
                free(output);
                close(pipefd[0]);
                waitpid(pid, NULL, 0);
                return NULL;
            }
            output = temp;
            memcpy(output + total_size, buffer, bytes_count);
            total_size += bytes_count;
        }
        if (output) { output[total_size] = '\0'; }
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
        return output;
    }
#endif
}
