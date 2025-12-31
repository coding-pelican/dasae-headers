#include "dh/Thrd/Mtx.h"

/*========== Internal Declarations ==========================================*/

pp_if_(pp_not(Thrd_Mtx_has_specialized))(pp_then_(
    $attr($inline_always)
    $static fn_((Thrd_Mtx__default_init(void))(Thrd_Mtx));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__default_fini(P$$(Thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__default_lock(P$$(Thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__default_tryLock(P$$(Thrd_Mtx) self))(bool));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__default_unlock(P$$(Thrd_Mtx) self))(void));
));
pp_if_(Thrd_Mtx_use_pthread)(pp_then_(
    $attr($inline_always)
    $static fn_((Thrd_Mtx__pthread_init(void))(Thrd_Mtx));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__pthread_fini(P$$(Thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__pthread_lock(P$$(Thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__pthread_tryLock(P$$(Thrd_Mtx) self))(bool));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__pthread_unlock(P$$(Thrd_Mtx) self))(void));
));
pp_if_(pp_and(Thrd_Mtx_has_specialized, plat_is_windows))(pp_then_(
    $attr($inline_always)
    $static fn_((Thrd_Mtx__windows_init(void))(Thrd_Mtx));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__windows_fini(P$$(Thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__windows_lock(P$$(Thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__windows_tryLock(P$$(Thrd_Mtx) self))(bool));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__windows_unlock(P$$(Thrd_Mtx) self))(void));
));
pp_if_(pp_and(Thrd_Mtx_has_specialized, plat_is_darwin))(pp_then_(
    $attr($inline_always)
    $static fn_((Thrd_Mtx__darwin_init(void))(Thrd_Mtx));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__darwin_fini(P$$(Thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__darwin_lock(P$$(Thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__darwin_tryLock(P$$(Thrd_Mtx) self))(bool));
    $attr($inline_always)
    $static fn_((Thrd_Mtx__darwin_unlock(P$$(Thrd_Mtx) self))(void));
));
$static let Thrd_Mtx__init = pp_if_(Thrd_Mtx_use_pthread)(
    pp_then_(Thrd_Mtx__pthread_init),
    pp_else_(pp_if_(Thrd_Mtx_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)(
                    Thrd_Mtx__windows_init
                )),
                pp_case_((plat_type_darwin)(
                    Thrd_Mtx__darwin_init
                ))
            ) pp_end
        )),
        pp_else_(Thrd_Mtx__default_init)
    )));
$static let Thrd_Mtx__fini = pp_if_(Thrd_Mtx_use_pthread)(
    pp_then_(Thrd_Mtx__pthread_fini),
    pp_else_(pp_if_(Thrd_Mtx_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)(
                    Thrd_Mtx__windows_fini
                )),
                pp_case_((plat_type_darwin)(
                    Thrd_Mtx__darwin_fini
                ))
            ) pp_end
        )),
        pp_else_(Thrd_Mtx__default_fini)
    )));
$static let Thrd_Mtx__lock = pp_if_(Thrd_Mtx_use_pthread)(
    pp_then_(Thrd_Mtx__pthread_lock),
    pp_else_(pp_if_(Thrd_Mtx_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)(
                    Thrd_Mtx__windows_lock
                )),
                pp_case_((plat_type_darwin)(
                    Thrd_Mtx__darwin_lock
                ))
            ) pp_end
        )),
        pp_else_(Thrd_Mtx__default_lock)
    )));
$static let Thrd_Mtx__tryLock = pp_if_(Thrd_Mtx_use_pthread)(
    pp_then_(Thrd_Mtx__pthread_tryLock),
    pp_else_(pp_if_(Thrd_Mtx_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)(
                    Thrd_Mtx__windows_tryLock
                )),
                pp_case_((plat_type_darwin)(
                    Thrd_Mtx__darwin_tryLock
                ))
            ) pp_end
        )),
        pp_else_(Thrd_Mtx__default_tryLock)
    )));
$static let Thrd_Mtx__unlock = pp_if_(Thrd_Mtx_use_pthread)(
    pp_then_(Thrd_Mtx__pthread_unlock),
    pp_else_(pp_if_(Thrd_Mtx_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)(
                    Thrd_Mtx__windows_unlock
                )),
                pp_case_((plat_type_darwin)(
                    Thrd_Mtx__darwin_unlock
                ))
            ) pp_end
        )),
        pp_else_(Thrd_Mtx__default_unlock)
    )));

/*========== External Definitions ===========================================*/

fn_((Thrd_Mtx_init(void))(Thrd_Mtx)) {
    return Thrd_Mtx__init();
};

fn_((Thrd_Mtx_fini(Thrd_Mtx* self))(void)) {
    Thrd_Mtx__fini(self);
};

fn_((Thrd_Mtx_lock(Thrd_Mtx* self))(void)) {
    Thrd_Mtx__lock(self);
};

fn_((Thrd_Mtx_tryLock(Thrd_Mtx* self))(bool)) {
    return Thrd_Mtx__tryLock(self);
};

fn_((Thrd_Mtx_unlock(Thrd_Mtx* self))(void)) {
    Thrd_Mtx__unlock(self);
};

fn_((Thrd_Mtx_Recur_init(void))(Thrd_Mtx_Recur)) {
    return (Thrd_Mtx_Recur){
        .inner = Thrd_Mtx_init(),
        .thrd_id = Thrd_invalid_id,
        .lock_count = 0,
    };
};

fn_((Thrd_Mtx_Recur_fini(Thrd_Mtx_Recur* self))(void)) {
    Thrd_Mtx_fini(&self->inner);
};

fn_((Thrd_Mtx_Recur_lock(Thrd_Mtx_Recur* self))(void)) {
    let current_id = Thrd_currentId();
    if (atom_load(&self->thrd_id, atom_MemOrd_unordered) != current_id) {
        Thrd_Mtx_lock(&self->inner);
        debug_assert(self->lock_count == 0);
        atom_store(&self->thrd_id, current_id, atom_MemOrd_unordered);
    }
    self->lock_count++;
};

fn_((Thrd_Mtx_Recur_tryLock(Thrd_Mtx_Recur* self))(bool)) {
    let current_id = Thrd_currentId();
    if (atom_load(&self->thrd_id, atom_MemOrd_unordered) != current_id) {
        if (!Thrd_Mtx_tryLock(&self->inner)) { return false; }
        debug_assert(self->lock_count == 0);
        atom_store(&self->thrd_id, current_id, atom_MemOrd_unordered);
    }
    self->lock_count++;
    return true;
};

fn_((Thrd_Mtx_Recur_unlock(Thrd_Mtx_Recur* self))(void)) {
    self->lock_count--;
    if (self->lock_count == 0) {
        atom_store(&self->thrd_id, Thrd_invalid_id, atom_MemOrd_unordered);
        Thrd_Mtx_unlock(&self->inner);
    }
};

/*========== Internal Definitions ===========================================*/

/* --- Pthreads --- */

#if Thrd_Mtx_use_pthread
fn_((Thrd_Mtx__pthread_init(void))(Thrd_Mtx)) {
    return (Thrd_Mtx){ .impl = PTHREAD_MUTEX_INITIALIZER };
};

fn_((Thrd_Mtx__pthread_fini(Thrd_Mtx* self))(void)) {
    pthread_mutex_destroy(&self->impl);
};

fn_((Thrd_Mtx__pthread_lock(Thrd_Mtx* self))(void)) {
    pthread_mutex_lock(&self->impl);
};

fn_((Thrd_Mtx__pthread_tryLock(Thrd_Mtx* self))(bool)) {
    return pthread_mutex_trylock(&self->impl) == 0;
};

fn_((Thrd_Mtx__pthread_unlock(Thrd_Mtx* self))(void)) {
    pthread_mutex_unlock(&self->impl);
};
#endif /* Thrd_Mtx_use_pthread */

/* --- Default --- */

#if !Thrd_Mtx_has_specialized
#include "dh/Thrd/Ftx.h"

#define Thrd_Mtx__default_unlocked (as$(u32)(0b00))
#define Thrd_Mtx__default_locked (as$(u32)(0b01))
#define Thrd_Mtx__default_contended (as$(u32)(0b11)) /* must contain the `locked` bit for x86 optimization below */

$static fn_((Thrd_Mtx__default_lockSlow(Thrd_Mtx* self))(void));

fn_((Thrd_Mtx__default_init(void))(Thrd_Mtx)) {
    return (Thrd_Mtx){ .impl.state = atom_V_init(Thrd_Mtx__default_unlocked) };
};

fn_((Thrd_Mtx__default_fini(Thrd_Mtx* self))(void)) {
    atom_V_store(&self->impl.state, Thrd_Mtx__default_unlocked, atom_MemOrd_monotonic);
};

fn_((Thrd_Mtx__default_lock(Thrd_Mtx* self))(void)) {
    if (!Thrd_Mtx_tryLock(self)) {
        Thrd_Mtx__default_lockSlow(self);
    }
};

fn_((Thrd_Mtx__default_tryLock(Thrd_Mtx* self))(bool)) {
    pp_if_(arch_is_x86_family)(
        /*pp_then_*/ (
            // On x86, use `lock bts` instead of `lock cmpxchg` as:
            // - they both seem to mark the cache-line as modified regardless: https://stackoverflow.com/a/63350048
            // - `lock bts` is smaller instruction-wise which makes it better for inlining
            let locked_bit = mem_trailingZeros32(Thrd_Mtx__default_locked);
            let prev_bit = atom_V_bitSet(
                &self->state,
                locked_bit,
                atom_MemOrd_acquire
            );
            return prev_bit == 0;
        ),
        /*pp_else_*/ (
            // Acquire barrier ensures grabbing the lock happens before the critical section
            // and that the previous lock holder's critical section happens before we grab the lock.
            return isNone(atom_V_cmpXchgWeak(
                &self->state,
                Thrd_Mtx__default_unlocked, Thrd_Mtx__default_locked,
                atom_MemOrd_acquire, atom_MemOrd_monotonic
            ));
        )
    );
};

fn_((Thrd_Mtx__default_unlock(Thrd_Mtx* self))(void)) {
    // Unlock the mutex and wake up a waiting thread if any.
    //
    // A waiting thread will acquire with `contended` instead of `locked`
    // which ensures that it wakes up another thread on the next unlock().
    //
    // Release barrier ensures the critical section happens before we let go of the lock
    // and that our critical section happens before the next lock holder grabs the lock.
    let state = atom_V_fetchXchg(&self->state, Thrd_Mtx__default_unlocked, atom_MemOrd_release);
    debug_assert(state != Thrd_Mtx__default_unlocked);
    if (state == Thrd_Mtx__default_contended) {
        Thrd_Ftx_wake(&self->state, 1);
    }
};

$attr($branch_cold)
fn_((Thrd_Mtx__default_lockSlow(Thrd_Mtx* self))(void)) {
    // Avoid doing an atomic swap below if we already know the state is contended.
    // An atomic swap unconditionally stores which marks the cache-line as modified unnecessarily.
    if (atom_V_load(&self->state, atom_MemOrd_monotonic) == Thrd_Mtx__default_contended) {
        Thrd_Ftx_wait(&self->state, Thrd_Mtx__default_contended);
    }
    // Try to acquire the lock while also telling the existing lock holder that there are threads waiting.
    //
    // Once we sleep on the Futex, we must acquire the mutex using `contended` rather than `locked`.
    // If not, threads sleeping on the Futex wouldn't see the state change in unlock and potentially deadlock.
    // The downside is that the last mutex unlocker will see `contended` and do an unnecessary Futex wake
    // but this is better than having to wake all waiting threads on mutex unlock.
    //
    // Acquire barrier ensures grabbing the lock happens before the critical section
    // and that the previous lock holder's critical section happens before we grab the lock.
    while (atom_V_fetchXchg(&self->state, Thrd_Mtx__default_contended, atom_MemOrd_acquire) != Thrd_Mtx__default_unlocked) {
        Thrd_Ftx_wait(&self->state, Thrd_Mtx__default_contended);
    }
};
#endif /* !Thrd_Mtx_has_specialized */

/* --- Windows --- */

#if Thrd_Mtx_has_specialized && plat_is_windows
fn_((Thrd_Mtx__windows_init(void))(Thrd_Mtx)) {
    return (Thrd_Mtx){ .impl.inner = SRWLOCK_INIT };
};

fn_((Thrd_Mtx__windows_fini(Thrd_Mtx* self))(void)) {
    let_ignore = self;
};

fn_((Thrd_Mtx__windows_lock(Thrd_Mtx* self))(void)) {
    AcquireSRWLockExclusive(&self->impl.inner);
};

fn_((Thrd_Mtx__windows_tryLock(Thrd_Mtx* self))(bool)) {
    return TryAcquireSRWLockExclusive(&self->impl.inner) != false;
};

fn_((Thrd_Mtx__windows_unlock(Thrd_Mtx* self))(void)) {
    ReleaseSRWLockExclusive(&self->impl.inner);
};
#endif /* Thrd_Mtx_has_specialized && plat_is_windows */

/* --- Darwin --- */

#if Thrd_Mtx_has_specialized && plat_is_darwin
fn_((Thrd_Mtx__darwin_init(void))(Thrd_Mtx)) {
    return (Thrd_Mtx){ .impl.inner = OS_UNFAIR_LOCK_INIT };
};

fn_((Thrd_Mtx__darwin_fini(Thrd_Mtx* self))(void)) {
    let_ignore = self;
};

fn_((Thrd_Mtx__darwin_lock(Thrd_Mtx* self))(void)) {
    os_unfair_lock_lock(&self->impl.inner);
};

fn_((Thrd_Mtx__darwin_tryLock(Thrd_Mtx* self))(bool)) {
    return os_unfair_lock_trylock(&self->impl.inner);
};

fn_((Thrd_Mtx__darwin_unlock(Thrd_Mtx* self))(void)) {
    os_unfair_lock_unlock(&self->impl.inner);
};
#endif /* Thrd_Mtx_has_specialized && plat_is_darwin */
