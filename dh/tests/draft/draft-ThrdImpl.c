/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    mockup-ThrdImpl.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-21 (date of creation)
 * @updated 2025-05-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 *
 * @brief   Mockup thread implementation
 * @details Provides mockup thread implementation for testing purposes.
 */

// #ifndef THRD_INCLUDED
// #define THRD_INCLUDED (1)


// #include "dh/core.h"
// #include "dh/err.h"
// #include "dh/Arr.h"
// #include "dh/Sli.h"
// #include "dh/Str.h"
// #include "dh/time.h"

// // Constants
// #define Thrd_max_name_len (15)
// #define Thrd_use_pthread  (1)

// #if Thrd_use_pthread
// #include <pthread.h>
// #include <semaphore.h>
// #else  /* !Thrd_use_pthread */
// #endif /* !Thrd_use_pthread */


// // Error handling types
// config_ErrSet(Thrd_Err,
//     OutOfMemory,
//     SystemResources,
//     ThreadQuotaExceeded,
//     TooManyThreads,
//     NameTooLong,
//     PermissionDenied,
//     Unsupported,
//     InvalidParameter,
//     CpuCount,
//     Other
// );

// // Read-write lock type
// typedef struct Thrd_RwLock {
//     pthread_rwlock_t impl;
// } Thrd_RwLock;
// // Semaphore type
// typedef struct Thrd_Sem {
// #ifdef __APPLE__
//     dispatch_semaphore_t impl;
// #else
//     sem_t impl;
// #endif
// } Thrd_Sem;
// // Reset event type
// typedef struct Thrd_ResetEvent {
//     Thrd_Mtx  mutex;
//     Thrd_Cond cond;
//     bool      signaled;
// } Thrd_ResetEvent;
// // Wait group type
// typedef struct Thrd_WaitGroup {
//     Thrd_Mtx  mutex;
//     Thrd_Cond cond;
//     usize     count;
// } Thrd_WaitGroup;


// typedef struct Thrd_FnCtx {
//     u8 data[];
// } Thrd_FnCtx;
// typedef struct Thrd_FnRet {
//     u8 data[];
// } Thrd_FnRet;
// typedef fn_((*Thrd_WorkFn)(Thrd_FnCtx* ctx), Thrd_FnRet*);
// typedef A$$(Thrd_max_name_len, u8) Thrd_NameBuf;
// use_O$(Thrd_NameBuf);
// // Thread implementation
// struct Thrd {
//     Thrd_Handle handle;
// };
// use_E$(Thrd);
// // Thread spawn configuration
// typedef struct Thrd_SpawnCfg {
//     usize             stack_size;
//     O$mem_Allocator allocator;
// } Thrd_SpawnCfg;
// #define Thrd_SpawnCfg_default_stack_size (16ull * 1024ull * 1024ull)
// static const Thrd_SpawnCfg Thrd_SpawnCfg_default = {
//     .stack_size = Thrd_SpawnCfg_default_stack_size,
//     .allocator  = none()
// };
// // Thread pool type
// typedef struct Thrd_Pool Thrd_Pool;



// // Error sets
// typedef E$void  Thrd_YieldError;
// typedef E$void  Thrd_SetNameError;
// typedef E$usize Thrd_CpuCountError;
// typedef E$void  Thrd_GetNameError;
// typedef E$Thrd  Thrd_SpawnError;


// // RwLock functions
// Thrd_RwLock Thrd_RwLock_init(void);
// void        Thrd_RwLock_deinit(Thrd_RwLock* rwlock);
// void        Thrd_RwLock_readLock(Thrd_RwLock* rwlock);
// void        Thrd_RwLock_writeLock(Thrd_RwLock* rwlock);
// bool        Thrd_RwLock_tryReadLock(Thrd_RwLock* rwlock);
// bool        Thrd_RwLock_tryWriteLock(Thrd_RwLock* rwlock);
// void        Thrd_RwLock_unlock(Thrd_RwLock* rwlock);

// // Semaphore functions
// Thrd_Sem Thrd_Sem_init(usize initial_value);
// void     Thrd_Sem_deinit(Thrd_Sem* sem);
// void     Thrd_Sem_wait(Thrd_Sem* sem);
// bool     Thrd_Sem_timedWait(Thrd_Sem* sem, uint64_t ns);
// void     Thrd_Sem_post(Thrd_Sem* sem);

// // Reset event functions
// Thrd_ResetEvent Thrd_ResetEvent_init(void);
// void            Thrd_ResetEvent_deinit(Thrd_ResetEvent* event);
// void            Thrd_ResetEvent_wait(Thrd_ResetEvent* event);
// bool            Thrd_ResetEvent_timedWait(Thrd_ResetEvent* event, uint64_t ns);
// void            Thrd_ResetEvent_signal(Thrd_ResetEvent* event);
// void            Thrd_ResetEvent_reset(Thrd_ResetEvent* event);

// // Wait group functions
// Thrd_WaitGroup Thrd_WaitGroup_init(void);
// void           Thrd_WaitGroup_deinit(Thrd_WaitGroup* wg);
// void           Thrd_WaitGroup_add(Thrd_WaitGroup* wg, usize count);
// void           Thrd_WaitGroup_wait(Thrd_WaitGroup* wg);
// void           Thrd_WaitGroup_done(Thrd_WaitGroup* wg);

// #endif /* THRD_INCLUDED */





// // RwLock implementation
// Thrd_RwLock Thrd_RwLock_init(void) {
//     Thrd_RwLock rwlock;
//     pthread_rwlock_init(&rwlock.impl, null);
//     return rwlock;
// }

// void Thrd_RwLock_deinit(Thrd_RwLock* rwlock) {
//     pthread_rwlock_destroy(&rwlock->impl);
// }

// void Thrd_RwLock_readLock(Thrd_RwLock* rwlock) {
//     pthread_rwlock_rdlock(&rwlock->impl);
// }

// void Thrd_RwLock_writeLock(Thrd_RwLock* rwlock) {
//     pthread_rwlock_wrlock(&rwlock->impl);
// }

// bool Thrd_RwLock_tryReadLock(Thrd_RwLock* rwlock) {
//     return pthread_rwlock_tryrdlock(&rwlock->impl) == 0;
// }

// bool Thrd_RwLock_tryWriteLock(Thrd_RwLock* rwlock) {
//     return pthread_rwlock_trywrlock(&rwlock->impl) == 0;
// }

// void Thrd_RwLock_unlock(Thrd_RwLock* rwlock) {
//     pthread_rwlock_unlock(&rwlock->impl);
// }

// // Semaphore implementation
// Thrd_Sem Thrd_Sem_init(usize initial_value) {
//     Thrd_Sem sem;
// #ifdef __APPLE__
//     sem.impl = dispatch_semaphore_create(initial_value);
// #else
//     sem_init(&sem.impl, 0, initial_value);
// #endif
//     return sem;
// }

// void Thrd_Sem_deinit(Thrd_Sem* sem) {
// #ifdef __APPLE__
// // dispatch_release(sem->impl); // Not needed in ARC environment
// #else
//     sem_destroy(&sem->impl);
// #endif
// }

// void Thrd_Sem_wait(Thrd_Sem* sem) {
// #ifdef __APPLE__
//     dispatch_semaphore_wait(sem->implm, DISPATCH_TIME_FOREVER);
// #else
//     sem_wait(&sem->impl);
// #endif
// }

// bool Thrd_Sem_timedWait(Thrd_Sem* sem, uint64_t ns) {
// #ifdef __APPLE__
//     return dispatch_semaphore_wait(sem->implm, dispatch_time(DISPATCH_TIME_NOW, ns)) == 0;
// #else
//     struct timespec ts;
//     clock_gettime(CLOCK_REALTIME, &ts);

//     uint64_t sec  = ns / 1000000000;
//     uint64_t nsec = ns % 1000000000;

//     ts.tv_sec += sec;
//     ts.tv_nsec += ns if (ts.tv_ >= 1000000000) {
//         ts.tv_sec += 1;
//         ts.tv_nsec -= 1000000000;
//     }

//     return sem_timedwait(&sem->implm, &ts) == 0;
// #endif
// }

// void Thrd_Sem_post(Thrd_Semaphosem) {
// #ifdef __APPLE__
//     dispatch_semaphore_signal(sem->impl);
// #else
//     sem_post(&sem->impl);
// #endif
// }

// // Reset event implementa
// Thrd_ResetEvent Thrd_ResetEvent_init(void) {
//     Thrd_ResetEvent event;
//     pthread_mutex_init(&event.mutex, null);
//     pthread_cond_init(&event.cond, null);
//     event.signaled = false;
//     return event void Thrd_ResetEvent_deinit(Thrd_ResetEvent * event) {
//         pthread_cond_destroy(&event->cond);
//         pthread_mutex_destroy(&event->mutex);
//     }

//     void Thrd_ResetEvent_wait(Thrd_ResetEvent * event) {
//         pthread_mutex_lock(&event->mutex);
//         while (!event->signaled) {
//             pthread_cond_wait(&event->cond, &event->mutex);
//         }
//         pthread_mutex_unlock(&event->mutex);
//     }

//     bool Thrd_ResetEvent_timedWait(Thrd_ResetEvent * event, uint64_t ns) {
//         struct timespec ts;
//         clock_gettime(CLOCK_REALTIME, &ts);

//         uint64_t sec  = ns / 1000000000;
//         uint64_t nsec = ns % 1000000000;

//         ts.tv_sec += sec;
//         ts.tv_nsec += n

//             if (ts.tv_nsec >= 1000000000) {
//             ts.tv_sec += 1;
//             ts.tv_nsec -= 1000000000;
//         }

//         pthread_mutex_lock(&event->mutex);
//         bool result = true;

//         while (!event->signaled) {
//             int res = pthread_cond_timedwait(&event->cond, &event->mutex, &ts);
//             if (res == ETIMEDOUT) {
//                 result = false;
//                 break;
//             }
//         }

//         pthread_mutex_unlock(&event->mutex);
//         return result;
//     }

//     void Thrd_ResetEvent_signal(Thrd_ResetEvent * event) {
//         pthread_mutex_lock(&event->mutex);
//         event->signaled = true;
//         pthread_cond_broadcast(&event->cond);
//         pthread_mutex_unlock(&event->mutex);
//     }

//     void Thrd_ResetEvent_reset(Thrd_ResetEvent * event) {
//         pthread_mutex_lock(&event->mutex);
//         event->signaled = false;
//         pthread_mutex_unlock(&event->mutex);
//     }

//     // Wait group implementation
//     Thrd_WaitGroup Thrd_WaitGroup_init(void) {
//         Thrd_WaitGroup wg;
//         pthread_mutex_init(&wg.mutex, null);
//         pthread_cond_init(&wg.cond, null);
//         wg.count = 0;
//         return wg;
//     }

//     void Thrd_WaitGroup_deinit(Thrd_WaitGroup * wg) {
//         pthread_cond_destroy(&wg->cond);
//         pthread_mutex_destroy(&wg->mutex);
//     }

//     void Thrd_WaitGroup_add(Thrd_WaitGroup * wg, usize count) {
//         pthread_mutex_lock(&wg->mutex);
//         wg->count += count;
//         pthread_mutex_unlock(&wg->mutex);
//     }

//     void Thrd_WaitGroup_wait(Thrd_WaitGroup * wg) {
//         pthread_mutex_lock(&wg->mutex);
//         while (wg->count > 0) {
//             pthread_cond_wait(&wg->cond, &wg->mutex);
//         }
//         pthread_mutex_unlock(&wg->mutex);
//     }

//     void Thrd_WaitGroup_done(Thrd_WaitGroup * wg) {
//         pthread_mutex_lock(&wg->mutex);
//         if (wg->count > 0) {
//             wg->count--;
//             if (wg->count == 0) {
//                 pthread_cond_broadcast(&wg->cond);
//             }
//         }
//         pthread_mutex_unlock(&wg->mutex);
//     }
