/*
 * test-thrd.c
 *
 * Example Windows C implementation of:
 *  - Thrd          (thread abstraction)
 *  - ThrdFtx       (futex-like wait/wake)
 *  - ThrdMtx       (non-recursive mutex using SRWLOCK)
 *  - ThrdMtxRecur  (recursive mutex using CRITICAL_SECTION)
 *  - ThrdCnd       (condition variable)
 *  - ThrdSem       (semaphore)
 *
 * This code is simplified for illustration. In a real project:
 *  - You may add robust error handling or convert to your "Err$..." style.
 *  - You may need to dynamically load RtlWaitOnAddress from ntdll.dll
 *    if using older Windows toolchains that do not provide WaitOnAddress, etc.
 *  - You might unify them into separate headers or a single library.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h> /* for _ReadWriteBarrier if needed */
#include <stdint.h> /* for uint32_t, etc. */
#include <stdio.h>  /* for fprintf, debugging, etc. */
#include <assert.h>

#include "dh/core.h"

/*===========================================================
 * 1) Thrd (thread object)
 *===========================================================*/

/* Forward declarations for our start routine. */
typedef int32_t (*ThrdStartFn)(void* arg);

typedef struct Thrd {
    HANDLE handle;
} Thrd;

/* Example “Err$Thrd” type. Adjust to your own error system. */
typedef enum ThrdSpawnErr {
    ThrdSpawnErr_Success = 0,
    ThrdSpawnErr_QuotaExceeded,
    ThrdSpawnErr_OutOfMemory,
    ThrdSpawnErr_Unexpected
} ThrdSpawnErr;

/* For naming threads, we can store them in a small struct passed to CreateThread. */
typedef struct ThrdStartupInfo {
    ThrdStartFn fn;
    void*       arg;
} ThrdStartupInfo;

/* Windows thread entry point. */
static DWORD WINAPI Thrd_WinThreadFunc(LPVOID lpParam) {
    ThrdStartupInfo* info = (ThrdStartupInfo*)lpParam;
    if (!info) {
        return 0;
    }
    /* The user’s start routine returns int32_t. We forward that as a DWORD. */
    int32_t rc = info->fn(info->arg);

    /* We allocated info dynamically in Thrd_spawn. Free it now. */
    HeapFree(GetProcessHeap(), 0, info);
    return (DWORD)rc;
}

/* Spawns a new thread. Returns an error or the created Thrd. */
ThrdSpawnErr Thrd_spawn(ThrdStartFn fn, void* arg, /* out */ Thrd* out_thrd) {
    if (!out_thrd) {
        return ThrdSpawnErr_Unexpected;
    }
    /* Windows will create the stack automatically. If you want a custom stack size, pass it to CreateThread. */

    ThrdStartupInfo* info = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*info));
    if (!info) {
        return ThrdSpawnErr_OutOfMemory;
    }
    info->fn  = fn;
    info->arg = arg;

    /* CreateThread: 0 stack size => uses default. */
    HANDLE h = CreateThread(
        /* lpThreadAttributes=*/NULL,
        /* dwStackSize=      */ 0,
        /* lpStartAddress=   */ Thrd_WinThreadFunc,
        /* lpParam=          */ info,
        /* dwCreationFlags=  */ 0,
        /* lpThreadId=       */ NULL
    );
    if (!h) {
        /* If CreateThread failed, free the startup info. */
        HeapFree(GetProcessHeap(), 0, info);
        DWORD err = GetLastError();
        switch (err) {
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY:
            return ThrdSpawnErr_OutOfMemory;
        case ERROR_MAX_THRDS_REACHED:
            return ThrdSpawnErr_QuotaExceeded;
        default:
            return ThrdSpawnErr_Unexpected;
        }
    }

    out_thrd->handle = h;
    return ThrdSpawnErr_Success;
}

/* Yields the current thread’s time slice. */
int Thrd_yield(void) {
    /* Windows has SwitchToThread or Sleep(0). */
    if (!SwitchToThread()) {
        /* SwitchToThread returns nonzero if it actually yielded. If it returns zero,
         * we can do Sleep(0) to yield. */
        Sleep(0);
    }
    return 0; /* success */
}

/* Sleeps for a certain number of nanoseconds. Windows Sleep is in ms. For finer granularity, use SleepConditionVariableSRW or others. */
void Thrd_sleep(uint64_t nanos) {
    DWORD ms = (DWORD)(nanos / 1000000ULL);
    if (ms == 0) {
        /* If less than 1 ms, just do a minimal Sleep(0). */
        Sleep(0);
    } else {
        Sleep(ms);
    }
}

/* Detach a thread. We close our handle so we no longer can join it. */
void Thrd_detach(Thrd self) {
    if (self.handle) {
        CloseHandle(self.handle);
        /* self.handle = NULL; */
    }
}

/* Join a thread. This waits until it finishes, then closes the handle. */
void Thrd_join(Thrd self) {
    if (self.handle) {
        WaitForSingleObject(self.handle, INFINITE);
        CloseHandle(self.handle);
    }
}

/* Set the name of a thread. Since Windows 10 1607, we can call SetThreadDescription. */
int Thrd_setName(Thrd self, const char* name) {
    if (!self.handle) {
        return -1;
    }
    /* For older Windows versions, the “official” way was raising an exception with a special structure.
     * For new Windows 10/11, we can do: */
    /* If we must support older Windows, load the function dynamically. */
    HMODULE hKernel = GetModuleHandleA("Kernel32.dll");
    if (hKernel) {
        typedef HRESULT(WINAPI * pSetThreadDescription)(HANDLE, PCWSTR);
        pSetThreadDescription fnSetDesc = (pSetThreadDescription)GetProcAddress(hKernel, "SetThreadDescription");
        if (fnSetDesc) {
            /* Convert `name` from UTF-8 to wide string. Here we do a naive approach. */
            wchar_t wName[128] = { 0 };
            MultiByteToWideChar(CP_UTF8, 0, name, -1, wName, 128);
            HRESULT hr = fnSetDesc(self.handle, wName);
            if (FAILED(hr)) {
                return -1;
            }
            return 0;
        }
    }
    /* If SetThreadDescription is not available, no easy method to do it older than Win10 1607. */
    return -1;
}

/* For “getting” the name: Windows does not offer a direct “get name” API.
 * Since SetThreadDescription sets the name internally, we *can* retrieve it with the same function. */
int Thrd_getName(Thrd self, char* buffer, size_t bufferSize) {
    if (!self.handle || !buffer || bufferSize == 0) {
        return -1;
    }
    HMODULE hKernel = GetModuleHandleA("Kernel32.dll");
    if (hKernel) {
        typedef HRESULT(WINAPI * pGetThreadDescription)(HANDLE, PWSTR*);
        pGetThreadDescription fnGetDesc = (pGetThreadDescription)GetProcAddress(hKernel, "GetThreadDescription");
        if (fnGetDesc) {
            PWSTR   wName = NULL;
            HRESULT hr    = fnGetDesc(self.handle, &wName);
            if (FAILED(hr) || !wName) {
                return -1;
            }
            /* Convert wide to UTF-8. */
            int len = WideCharToMultiByte(CP_UTF8, 0, wName, -1, buffer, (int)bufferSize, NULL, NULL);
            LocalFree(wName);
            if (len <= 0) {
                return -1;
            }
            return 0;
        }
    }
    return -1;
}

/* Current Thread ID. On Windows, GetCurrentThreadId returns a DWORD. */
uint32_t Thrd_getCurrentId(void) {
    return (uint32_t)GetCurrentThreadId();
}

/* Number of logical CPU cores. We can use GetActiveProcessorCount. */
int Thrd_getCpuCount(void) {
    DWORD count = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
    if (count == 0) {
        /* fallback: 1? */
        return 1;
    }
    return (int)count;
}

/*===========================================================
 * 2) ThrdFtx (futex-like) on Windows
 *    Using WaitOnAddress / WakeByAddressSingle / WakeByAddressAll
 *
 *    If your compiler or platform is older, you might need
 *    to dynamically load these from 'kernel32.dll' or 'ntdll.dll'.
 *===========================================================*/

typedef struct ThrdFtx {
    /* The 32-bit memory you wait on. */
    volatile LONG val;
} ThrdFtx;

/* Initialize with some value (default 0). */
static void ThrdFtx_init(ThrdFtx* ftx, LONG initial) {
    ftx->val = initial;
}

/* Wait if *ftx->val == expect. Blocks until changed or woken. */
static int ThrdFtx_wait(ThrdFtx* ftx, LONG expect) {
    /* WaitOnAddress needs pointer to the memory, the “compare” pointer, and size (4). */
    BOOL rc = WaitOnAddress((PVOID)&ftx->val, (PVOID)&expect, sizeof(LONG), INFINITE);
    if (!rc) {
        DWORD err = GetLastError();
        if (err == ERROR_TIMEOUT) {
            return 1; /* or define error code for timed out */
        }
        /* spurious wake or mismatch => ignore */
    }
    return 0;
}

/* Like ThrdFtx_wait but with a timeout in nanoseconds. We approximate. */
static int ThrdFtx_timedWait(ThrdFtx* ftx, LONG expect, uint64_t nanos) {
    DWORD ms = (DWORD)(nanos / 1000000ULL);
    BOOL  rc = WaitOnAddress((PVOID)&ftx->val, (PVOID)&expect, sizeof(LONG), ms);
    if (!rc) {
        DWORD err = GetLastError();
        if (err == ERROR_TIMEOUT) {
            return 1; /* timed out */
        }
    }
    return 0;
}

/* Wake up to “count” waiters (1 or all). */
static void ThrdFtx_wake(ThrdFtx* ftx, int count) {
    if (count <= 1) {
        WakeByAddressSingle((PVOID)&ftx->val);
    } else {
        WakeByAddressAll((PVOID)&ftx->val);
    }
}

/*===========================================================
 * 3) ThrdMtx (non‐recursive mutex)
 *    We'll use an SRWLOCK for a lightweight non‐recursive lock.
 *===========================================================*/

typedef struct ThrdMtx {
    SRWLOCK lock;
} ThrdMtx;

/* Initialize. SRWLOCK can be statically initialized with SRWLOCK_INIT. */
static void ThrdMtx_init(ThrdMtx* mtx) {
    InitializeSRWLock(&mtx->lock);
}

/* Destroy is a no‐op on SRWLOCK. */

/* Lock (exclusive) */
static void ThrdMtx_lock(ThrdMtx* mtx) {
    AcquireSRWLockExclusive(&mtx->lock);
}

/* Try‐lock returns 0 if locked, non-zero if we failed. */
static int ThrdMtx_tryLock(ThrdMtx* mtx) {
    return (int)(!TryAcquireSRWLockExclusive(&mtx->lock));
}

/* Unlock */
static void ThrdMtx_unlock(ThrdMtx* mtx) {
    ReleaseSRWLockExclusive(&mtx->lock);
}

/*===========================================================
 * 4) ThrdMtxRecur (recursive mutex)
 *    We'll use CRITICAL_SECTION for recursion on Windows.
 *===========================================================*/

typedef struct ThrdMtxRecur {
    CRITICAL_SECTION cs;
} ThrdMtxRecur;

static void ThrdMtxRecur_init(ThrdMtxRecur* mtx) {
    InitializeCriticalSection(&mtx->cs);
}

static void ThrdMtxRecur_destroy(ThrdMtxRecur* mtx) {
    DeleteCriticalSection(&mtx->cs);
}

static void ThrdMtxRecur_lock(ThrdMtxRecur* mtx) {
    EnterCriticalSection(&mtx->cs);
}

static int ThrdMtxRecur_tryLock(ThrdMtxRecur* mtx) {
    return (int)(!TryEnterCriticalSection(&mtx->cs));
}

static void ThrdMtxRecur_unlock(ThrdMtxRecur* mtx) {
    LeaveCriticalSection(&mtx->cs);
}

/*===========================================================
 * 5) ThrdCnd (condition variable)
 *    We'll use CONDITION_VARIABLE + SRWLOCK or CRITICAL_SECTION.
 *    Typically you pair ThrdCnd with ThrdMtx (the non‐recursive one).
 *===========================================================*/

typedef struct ThrdCnd {
    CONDITION_VARIABLE cv;
} ThrdCnd;

static void ThrdCnd_init(ThrdCnd* cnd) {
    InitializeConditionVariable(&cnd->cv);
}

/* Wait on the condition. You must hold the associated SRWLock *exclusively*. */
static int ThrdCnd_wait(ThrdCnd* cnd, ThrdMtx* mtx) {
    /* This will release the SRWLOCK, block, then reacquire the SRWLOCK. */
    if (!SleepConditionVariableSRW(&cnd->cv, &mtx->lock, INFINITE, 0)) {
        /* 0 => error. Usually GetLastError()==ERROR_TIMEOUT or something else. */
        return (int)GetLastError();
    }
    return 0;
}

/* Timed wait in nanoseconds. Windows SleepConditionVariableSRW uses milliseconds. */
static int ThrdCnd_timedWait(ThrdCnd* cnd, ThrdMtx* mtx, uint64_t nanos) {
    DWORD ms = (DWORD)(nanos / 1000000ULL);
    if (!SleepConditionVariableSRW(&cnd->cv, &mtx->lock, ms, 0)) {
        DWORD err = GetLastError();
        if (err == ERROR_TIMEOUT) {
            return 1; /* e.g. timed out */
        }
        return -1; /* some other error */
    }
    return 0;
}

/* Signal one thread. */
static void ThrdCnd_signal(ThrdCnd* cnd) {
    WakeConditionVariable(&cnd->cv);
}

/* Signal all waiting threads. */
static void ThrdCnd_broadcast(ThrdCnd* cnd) {
    WakeAllConditionVariable(&cnd->cv);
}

/*===========================================================
 * 6) ThrdSem (semaphore)
 *    We'll use a Win32 semaphore handle.
 *===========================================================*/

typedef struct ThrdSem {
    HANDLE sem;
} ThrdSem;

static int ThrdSem_init(ThrdSem* s, unsigned initialCount) {
    HANDLE h = CreateSemaphoreA(
        NULL,               /* default security attributes */
        (LONG)initialCount, /* initial count */
        LONG_MAX,           /* maximum count, or pick something else */
        NULL                /* unnamed semaphore */
    );
    if (!h) {
        return -1;
    }
    s->sem = h;
    return 0;
}

static void ThrdSem_destroy(ThrdSem* s) {
    if (s->sem) {
        CloseHandle(s->sem);
        s->sem = NULL;
    }
}

static void ThrdSem_post(ThrdSem* s, unsigned releaseCount) {
    ReleaseSemaphore(s->sem, (LONG)releaseCount, NULL);
}

/* Wait indefinitely until the semaphore can be decremented. */
static int ThrdSem_wait(ThrdSem* s) {
    DWORD rc = WaitForSingleObject(s->sem, INFINITE);
    return (rc == WAIT_OBJECT_0) ? 0 : -1;
}

/* Timed wait in nanoseconds. Approximate with ms. */
static int ThrdSem_timedWait(ThrdSem* s, uint64_t nanos) {
    DWORD ms = (DWORD)(nanos / 1000000ULL);
    DWORD rc = WaitForSingleObject(s->sem, ms);
    if (rc == WAIT_TIMEOUT) {
        return 1; /* custom code for “timeout” */
    }
    return (rc == WAIT_OBJECT_0) ? 0 : -1;
}

/*===========================================================
 * Example usage / test code
 *===========================================================*/

#include <stdlib.h>

/* A sample thread function for Thrd. */
static int32_t myThreadFunc(void* arg) {
    int idx = *(int*)arg;
    printf("Hello from Windows Thrd: index = %d\n", idx);
    return 42 + idx; /* return some value */
}

static void example_Thrd(void) {
    Thrd t;
    int  indexVal = 123;
    if (Thrd_spawn(myThreadFunc, &indexVal, &t) == ThrdSpawnErr_Success) {
        /* Optionally set thread name. */
        Thrd_setName(t, "MyWinThread");

        /* Join the thread.  */
        Thrd_join(t); /* ignoring the result code for brevity */
        printf("Joined the thread.\n");
    }
}

static void example_ThrdMtx(void) {
    ThrdMtx mtx;
    ThrdMtx_init(&mtx);

    ThrdMtx_lock(&mtx);
    /* critical section... */
    ThrdMtx_unlock(&mtx);

    /* SRWLock is a static object, so no explicit destroy needed. */
}

static void example_ThrdMtxRecur(void) {
    ThrdMtxRecur mtx;
    ThrdMtxRecur_init(&mtx);

    ThrdMtxRecur_lock(&mtx);
    ThrdMtxRecur_lock(&mtx);
    /* We have locked it twice. Now we must unlock twice. */
    ThrdMtxRecur_unlock(&mtx);
    ThrdMtxRecur_unlock(&mtx);

    ThrdMtxRecur_destroy(&mtx);
}

static void example_ThrdSem(void) {
    ThrdSem sem;
    ThrdSem_init(&sem, 1 /* initial permits */);

    ThrdSem_wait(&sem);
    /* critical region or resource usage */
    ThrdSem_post(&sem, 1);

    ThrdSem_destroy(&sem);
}

int main(void) {
    example_Thrd();
    example_ThrdMtx();
    example_ThrdMtxRecur();
    example_ThrdSem();

    /* For ThrdCnd and ThrdFtx, usage is more specialized. This is just a sample code. */
    ignore fprintf(stdout, "All examples finished.\n");
    return 0;
}
