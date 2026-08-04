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

struct proc_Child {
#ifdef _WIN32
    HANDLE process;
    HANDLE thread;
    HANDLE job;
#else
    pid_t pid;
#endif
};

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

static HANDLE proc__createKillOnCloseJob(void) {
    HANDLE job = CreateJobObjectA(NULL, NULL);
    if (!job) { return NULL; }
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
    memset(&info, 0, sizeof(info));
    info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation, &info, sizeof(info))) {
        CloseHandle(job);
        return NULL;
    }
    return job;
}

static bool proc__assignJobAndResume(HANDLE job, PROCESS_INFORMATION* process) {
    if (!job || !process) { return false; }
    if (!AssignProcessToJobObject(job, process->hProcess)) {
        TerminateProcess(process->hProcess, 1);
        return false;
    }
    if (ResumeThread(process->hThread) == (DWORD)-1) {
        TerminateProcess(process->hProcess, 1);
        return false;
    }
    return true;
}
#endif
#ifndef _WIN32
static int proc__statusToExitCode(int status) {
    if (WIFEXITED(status)) { return WEXITSTATUS(status); }
    if (WIFSIGNALED(status)) { return 128 + WTERMSIG(status); }
    return -1;
}

enum { proc__max_active_children = 1024 };
static volatile sig_atomic_t proc__cleanup_installed = 0;
static volatile sig_atomic_t proc__active_child_count = 0;
static pid_t proc__active_child_pgids[proc__max_active_children];

static void proc__signalMask(int how, sigset_t* old_mask) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGHUP);
    (void)sigprocmask(how, &mask, old_mask);
}

static void proc__terminateAll(int sig) {
    const sig_atomic_t count = proc__active_child_count;
    for (sig_atomic_t i = 0; i < count; ++i) {
        const pid_t pgid = proc__active_child_pgids[i];
        if (pgid > 0) { (void)kill(-pgid, sig); }
    }
}

static void proc__cleanupAll(void) {
    sigset_t old_mask;
    proc__signalMask(SIG_BLOCK, &old_mask);
    proc__terminateAll(SIGTERM);
    proc__terminateAll(SIGKILL);
    const sig_atomic_t count = proc__active_child_count;
    for (sig_atomic_t i = 0; i < count; ++i) {
        if (proc__active_child_pgids[i] > 0) {
            (void)waitpid(proc__active_child_pgids[i], NULL, WNOHANG);
        }
    }
    proc__active_child_count = 0;
    (void)sigprocmask(SIG_SETMASK, &old_mask, NULL);
}

static void proc__handleSignal(int sig) {
    proc__terminateAll(sig);
    _exit(128 + sig);
}

static void proc__installCleanup(void) {
    if (proc__cleanup_installed) { return; }
    proc__cleanup_installed = 1;
    (void)atexit(proc__cleanupAll);
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = proc__handleSignal;
    sigemptyset(&action.sa_mask);
    (void)sigaction(SIGINT, &action, NULL);
    (void)sigaction(SIGTERM, &action, NULL);
    (void)sigaction(SIGHUP, &action, NULL);
}

static bool proc__registerChild(pid_t pgid) {
    proc__installCleanup();
    sigset_t old_mask;
    proc__signalMask(SIG_BLOCK, &old_mask);
    const bool ok = proc__active_child_count < proc__max_active_children;
    if (ok) { proc__active_child_pgids[proc__active_child_count++] = pgid; }
    (void)sigprocmask(SIG_SETMASK, &old_mask, NULL);
    return ok;
}

static void proc__unregisterChild(pid_t pgid) {
    sigset_t old_mask;
    proc__signalMask(SIG_BLOCK, &old_mask);
    for (sig_atomic_t i = 0; i < proc__active_child_count; ++i) {
        if (proc__active_child_pgids[i] == pgid) {
            --proc__active_child_count;
            proc__active_child_pgids[i] = proc__active_child_pgids[proc__active_child_count];
            proc__active_child_pgids[proc__active_child_count] = 0;
            break;
        }
    }
    (void)sigprocmask(SIG_SETMASK, &old_mask, NULL);
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

proc_Child* proc_spawn(const char** argv, bool show_output) {
    if (!argv || !argv[0]) { return NULL; }
#ifdef _WIN32
    char* const cmd_line = proc__buildCommandLine(argv);
    if (!cmd_line) { return NULL; }

    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);
    if (!show_output) {
        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    }
    HANDLE job = proc__createKillOnCloseJob();
    if (!job) {
        free(cmd_line);
        return NULL;
    }

    const BOOL success = CreateProcessA(
        NULL,
        cmd_line,
        NULL,
        NULL,
        !show_output,
        CREATE_NEW_PROCESS_GROUP | CREATE_SUSPENDED,
        NULL,
        NULL,
        &si,
        &pi
    );
    free(cmd_line);
    if (!success) {
        if (job) { CloseHandle(job); }
        return NULL;
    }
    if (!proc__assignJobAndResume(job, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(job);
        return NULL;
    }

    proc_Child* child = (proc_Child*)malloc(sizeof(*child));
    if (!child) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        if (job) { CloseHandle(job); }
        return NULL;
    }
    child->process = pi.hProcess;
    child->thread = pi.hThread;
    child->job = job;
    return child;
#else
    const pid_t pid = fork();
    if (pid == -1) { return NULL; }
    if (pid == 0) {
        (void)setpgid(0, 0);
#ifdef __linux__
        (void)prctl(PR_SET_PDEATHSIG, SIGTERM);
        if (getppid() == 1) { _exit(128 + SIGTERM); }
#endif
        if (!show_output) {
            freopen("/dev/null", "w", stdout);
            freopen("/dev/null", "w", stderr);
        }
        char** const exec_argv = proc__makeMutableArgv(argv);
        if (!exec_argv) { exit(1); }
        execvp(exec_argv[0], exec_argv);
        proc__freeMutableArgv(exec_argv);
        exit(1);
    }

    (void)setpgid(pid, pid);
    if (!proc__registerChild(pid)) {
        (void)kill(-pid, SIGTERM);
        (void)waitpid(pid, NULL, 0);
        return NULL;
    }
    proc_Child* child = (proc_Child*)malloc(sizeof(*child));
    if (!child) {
        (void)kill(-pid, SIGTERM);
        (void)waitpid(pid, NULL, 0);
        proc__unregisterChild(pid);
        return NULL;
    }
    child->pid = pid;
    return child;
#endif
}

int proc_wait(proc_Child** child) {
    if (!child || !*child) { return -1; }
    proc_Child* value = *child;
#ifdef _WIN32
    WaitForSingleObject(value->process, INFINITE);
    DWORD exit_code = 0;
    GetExitCodeProcess(value->process, &exit_code);
    CloseHandle(value->process);
    CloseHandle(value->thread);
    if (value->job) { CloseHandle(value->job); }
    free(value);
    *child = NULL;
    return (int)exit_code;
#else
    int status = 0;
    for (;;) {
        pid_t waited = waitpid(value->pid, &status, 0);
        if (waited == value->pid) { break; }
        if (waited == -1 && errno == EINTR) { continue; }
        proc__unregisterChild(value->pid);
        free(value);
        *child = NULL;
        return -1;
    }
    int code = proc__statusToExitCode(status);
    proc__unregisterChild(value->pid);
    free(value);
    *child = NULL;
    return code;
#endif
}

int proc_waitAny(proc_Child** children, int count, int* index_out) {
    if (!children || count <= 0 || !index_out) { return -1; }
#ifdef _WIN32
    HANDLE handles[MAXIMUM_WAIT_OBJECTS];
    if (count > (int)MAXIMUM_WAIT_OBJECTS) { return -1; }
    for (int i = 0; i < count; ++i) {
        if (!children[i]) { return -1; }
        handles[i] = children[i]->process;
    }
    DWORD waited = WaitForMultipleObjects((DWORD)count, handles, FALSE, INFINITE);
    if (waited < WAIT_OBJECT_0 || waited >= WAIT_OBJECT_0 + (DWORD)count) {
        return -1;
    }
    int index = (int)(waited - WAIT_OBJECT_0);
    *index_out = index;
    return proc_wait(&children[index]);
#else
    for (;;) {
        int status = 0;
        pid_t waited = waitpid(-1, &status, 0);
        if (waited == -1 && errno == EINTR) { continue; }
        if (waited == -1) { return -1; }
        for (int i = 0; i < count; ++i) {
            if (children[i] && children[i]->pid == waited) {
                int code = proc__statusToExitCode(status);
                proc__unregisterChild(children[i]->pid);
                free(children[i]);
                children[i] = NULL;
                *index_out = i;
                return code;
            }
        }
    }
#endif
}

int proc_run(const char** argv, bool show_output) {
    if (!argv || !argv[0]) { return -1; }
    proc_Child* child = proc_spawn(argv, show_output);
    return child ? proc_wait(&child) : -1;
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

    HANDLE job = proc__createKillOnCloseJob();
    if (!job) {
        free(cmd_line);
        CloseHandle(hRead);
        CloseHandle(hWrite);
        return -1;
    }

    const BOOL success = CreateProcessA(
        NULL,
        cmd_line,
        NULL,
        NULL,
        TRUE,
        CREATE_NEW_PROCESS_GROUP | CREATE_SUSPENDED,
        NULL,
        NULL,
        &si,
        &pi
    );
    free(cmd_line);
    if (!success) {
        CloseHandle(hWrite);
        CloseHandle(hRead);
        CloseHandle(job);
        return -1;
    }
    if (!proc__assignJobAndResume(job, &pi)) {
        CloseHandle(hWrite);
        CloseHandle(hRead);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(job);
        return -1;
    }
    CloseHandle(hWrite);

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
    HANDLE job = proc__createKillOnCloseJob();
    if (!job) {
        free(cmd_line);
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
        CREATE_NEW_PROCESS_GROUP | CREATE_SUSPENDED,
        NULL,
        NULL,
        &si,
        &pi
    );
    free(cmd_line);
    if (!success) {
        CloseHandle(hWrite);
        CloseHandle(job);
        return CloseHandle(hRead), NULL;
    }
    if (!proc__assignJobAndResume(job, &pi)) {
        CloseHandle(hWrite);
        CloseHandle(hRead);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(job);
        return NULL;
    }
    CloseHandle(hWrite);
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
            if (job) { CloseHandle(job); }
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
    if (job) { CloseHandle(job); }
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
        (void)setpgid(0, 0);
#ifdef __linux__
        (void)prctl(PR_SET_PDEATHSIG, SIGTERM);
        if (getppid() == 1) { _exit(128 + SIGTERM); }
#endif
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
        (void)setpgid(pid, pid);
        if (!proc__registerChild(pid)) {
            (void)kill(-pid, SIGTERM);
            (void)waitpid(pid, NULL, 0);
            close(pipefd[0]);
            close(pipefd[1]);
            return NULL;
        }
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
                proc__unregisterChild(pid);
                return NULL;
            }
            output = temp;
            memcpy(output + total_size, buffer, bytes_count);
            total_size += bytes_count;
        }
        if (output) { output[total_size] = '\0'; }
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
        proc__unregisterChild(pid);
        return output;
    }
#endif
}
