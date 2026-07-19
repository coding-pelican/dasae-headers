#include "Mtx.h"
#include "Self.h"

/*========== Internal Declarations ==========================================*/

pp_if_(pp_not(thrd_Mtx_has_specialized))(pp_then_(
    $attr($inline_always)
    $static fn_((thrd_Mtx__default_init(void))(thrd_Mtx));
    $attr($inline_always)
    $static fn_((thrd_Mtx__default_fini(P$$(thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((thrd_Mtx__default_lock(P$$(thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((thrd_Mtx__default_tryLock(P$$(thrd_Mtx) self))(bool));
    $attr($inline_always)
    $static fn_((thrd_Mtx__default_unlock(P$$(thrd_Mtx) self))(void));
));
pp_if_(thrd_Mtx_use_pthread)(pp_then_(
    $attr($inline_always)
    $static fn_((thrd_Mtx__pthread_init(void))(thrd_Mtx));
    $attr($inline_always)
    $static fn_((thrd_Mtx__pthread_fini(P$$(thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((thrd_Mtx__pthread_lock(P$$(thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((thrd_Mtx__pthread_tryLock(P$$(thrd_Mtx) self))(bool));
    $attr($inline_always)
    $static fn_((thrd_Mtx__pthread_unlock(P$$(thrd_Mtx) self))(void));
));
pp_if_(pp_and(thrd_Mtx_has_specialized, plat_is_windows))(pp_then_(
    $attr($inline_always)
    $static fn_((thrd_Mtx__windows_init(void))(thrd_Mtx));
    $attr($inline_always)
    $static fn_((thrd_Mtx__windows_fini(P$$(thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((thrd_Mtx__windows_lock(P$$(thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((thrd_Mtx__windows_tryLock(P$$(thrd_Mtx) self))(bool));
    $attr($inline_always)
    $static fn_((thrd_Mtx__windows_unlock(P$$(thrd_Mtx) self))(void));
));
pp_if_(pp_and(thrd_Mtx_has_specialized, plat_is_darwin))(pp_then_(
    $attr($inline_always)
    $static fn_((thrd_Mtx__darwin_init(void))(thrd_Mtx));
    $attr($inline_always)
    $static fn_((thrd_Mtx__darwin_fini(P$$(thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((thrd_Mtx__darwin_lock(P$$(thrd_Mtx) self))(void));
    $attr($inline_always)
    $static fn_((thrd_Mtx__darwin_tryLock(P$$(thrd_Mtx) self))(bool));
    $attr($inline_always)
    $static fn_((thrd_Mtx__darwin_unlock(P$$(thrd_Mtx) self))(void));
));
$static let thrd_Mtx__init = pp_if_(thrd_Mtx_use_pthread)(
    pp_then_(thrd_Mtx__pthread_init),
    pp_else_(pp_if_(thrd_Mtx_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)(
                    thrd_Mtx__windows_init
                )),
                pp_case_((plat_type_darwin)(
                    thrd_Mtx__darwin_init
                ))
            ) pp_end
        )),
        pp_else_(thrd_Mtx__default_init)
    )));
$static let thrd_Mtx__fini = pp_if_(thrd_Mtx_use_pthread)(
    pp_then_(thrd_Mtx__pthread_fini),
    pp_else_(pp_if_(thrd_Mtx_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)(
                    thrd_Mtx__windows_fini
                )),
                pp_case_((plat_type_darwin)(
                    thrd_Mtx__darwin_fini
                ))
            ) pp_end
        )),
        pp_else_(thrd_Mtx__default_fini)
    )));
$static let thrd_Mtx__lock = pp_if_(thrd_Mtx_use_pthread)(
    pp_then_(thrd_Mtx__pthread_lock),
    pp_else_(pp_if_(thrd_Mtx_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)(
                    thrd_Mtx__windows_lock
                )),
                pp_case_((plat_type_darwin)(
                    thrd_Mtx__darwin_lock
                ))
            ) pp_end
        )),
        pp_else_(thrd_Mtx__default_lock)
    )));
$static let thrd_Mtx__tryLock = pp_if_(thrd_Mtx_use_pthread)(
    pp_then_(thrd_Mtx__pthread_tryLock),
    pp_else_(pp_if_(thrd_Mtx_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)(
                    thrd_Mtx__windows_tryLock
                )),
                pp_case_((plat_type_darwin)(
                    thrd_Mtx__darwin_tryLock
                ))
            ) pp_end
        )),
        pp_else_(thrd_Mtx__default_tryLock)
    )));
$static let thrd_Mtx__unlock = pp_if_(thrd_Mtx_use_pthread)(
    pp_then_(thrd_Mtx__pthread_unlock),
    pp_else_(pp_if_(thrd_Mtx_has_specialized)(
        pp_then_(pp_expand(
            pp_switch_ pp_begin(plat_type)(
                pp_case_((plat_type_windows)(
                    thrd_Mtx__windows_unlock
                )),
                pp_case_((plat_type_darwin)(
                    thrd_Mtx__darwin_unlock
                ))
            ) pp_end
        )),
        pp_else_(thrd_Mtx__default_unlock)
    )));

/*========== External Definitions ===========================================*/

fn_((thrd_Mtx_init(void))(thrd_Mtx)) {
    return thrd_Mtx__init();
};

fn_((thrd_Mtx_fini(thrd_Mtx* self))(void)) {
    thrd_Mtx__fini(self);
};

fn_((thrd_Mtx_lock(thrd_Mtx* self))(void)) {
    thrd_Mtx__lock(self);
};

fn_((thrd_Mtx_tryLock(thrd_Mtx* self))(bool)) {
    return thrd_Mtx__tryLock(self);
};

fn_((thrd_Mtx_unlock(thrd_Mtx* self))(void)) {
    thrd_Mtx__unlock(self);
};

fn_((thrd_Mtx_Recur_init(void))(thrd_Mtx_Recur)) {
    return (thrd_Mtx_Recur){
        .inner = thrd_Mtx_init(),
        .thrd_id = thrd_invalid_id,
        .lock_count = 0,
    };
};

fn_((thrd_Mtx_Recur_fini(thrd_Mtx_Recur* self))(void)) {
    thrd_Mtx_fini(&self->inner);
};

fn_((thrd_Mtx_Recur_lock(thrd_Mtx_Recur* self))(void)) {
    let current_id = thrd_currId();
    if (atom_load(&self->thrd_id, atom_MemOrd_unordered) != current_id) {
        thrd_Mtx_lock(&self->inner);
        debug_assert(self->lock_count == 0);
        atom_store(&self->thrd_id, current_id, atom_MemOrd_unordered);
    }
    self->lock_count++;
};

fn_((thrd_Mtx_Recur_tryLock(thrd_Mtx_Recur* self))(bool)) {
    let current_id = thrd_currId();
    if (atom_load(&self->thrd_id, atom_MemOrd_unordered) != current_id) {
        if (!thrd_Mtx_tryLock(&self->inner)) { return false; }
        debug_assert(self->lock_count == 0);
        atom_store(&self->thrd_id, current_id, atom_MemOrd_unordered);
    }
    self->lock_count++;
    return true;
};

fn_((thrd_Mtx_Recur_unlock(thrd_Mtx_Recur* self))(void)) {
    self->lock_count--;
    if (self->lock_count == 0) {
        atom_store(&self->thrd_id, thrd_invalid_id, atom_MemOrd_unordered);
        thrd_Mtx_unlock(&self->inner);
    }
};

/*========== Internal Definitions ===========================================*/

/*--- Pthreads ---*/

#if thrd_Mtx_use_pthread
fn_((thrd_Mtx__pthread_init(void))(thrd_Mtx)) {
    return (thrd_Mtx)thrd_Mtx_init_static();
};

fn_((thrd_Mtx__pthread_fini(thrd_Mtx* self))(void)) {
    pthread_mutex_destroy(&self->impl);
};

fn_((thrd_Mtx__pthread_lock(thrd_Mtx* self))(void)) {
    pthread_mutex_lock(&self->impl);
};

fn_((thrd_Mtx__pthread_tryLock(thrd_Mtx* self))(bool)) {
    return pthread_mutex_trylock(&self->impl) == 0;
};

fn_((thrd_Mtx__pthread_unlock(thrd_Mtx* self))(void)) {
    pthread_mutex_unlock(&self->impl);
};
#endif /* thrd_Mtx_use_pthread */

/*--- Default ---*/

#if !thrd_Mtx_has_specialized
#include "ftx.h"

#define thrd_Mtx__default_unlocked (as$(u32)(0b00))
#define thrd_Mtx__default_locked (as$(u32)(0b01))
#define thrd_Mtx__default_contended (as$(u32)(0b11)) /* must contain the `locked` bit for x86 optimization below */

$static fn_((thrd_Mtx__default_lockSlow(thrd_Mtx* self))(void));

fn_((thrd_Mtx__default_init(void))(thrd_Mtx)) {
    return (thrd_Mtx)thrd_Mtx_init_static();
};

fn_((thrd_Mtx__default_fini(thrd_Mtx* self))(void)) {
    atom_V_store(&self->impl.state, thrd_Mtx__default_unlocked, atom_MemOrd_monotonic);
};

fn_((thrd_Mtx__default_lock(thrd_Mtx* self))(void)) {
    if (!thrd_Mtx_tryLock(self)) {
        thrd_Mtx__default_lockSlow(self);
    }
};

fn_((thrd_Mtx__default_tryLock(thrd_Mtx* self))(bool)) {
    pp_if_(arch_family_is_x86)(
        /*pp_then_*/ (
            // On x86, use `lock bts` instead of `lock cmpxchg` as:
            // - they both seem to mark the cache-line as modified regardless: https://stackoverflow.com/a/63350048
            // - `lock bts` is smaller instruction-wise which makes it better for inlining
            let locked_bit = mem_trailingZeros32(thrd_Mtx__default_locked);
            let prev_bit = atom_V_int_setBit(
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
                thrd_Mtx__default_unlocked, thrd_Mtx__default_locked,
                atom_MemOrd_acquire, atom_MemOrd_monotonic
            ));
        )
    );
};

fn_((thrd_Mtx__default_unlock(thrd_Mtx* self))(void)) {
    // Unlock the mutex and wake up a waiting thread if any.
    //
    // A waiting thread will acquire with `contended` instead of `locked`
    // which ensures that it wakes up another thread on the next unlock().
    //
    // Release barrier ensures the critical section happens before we let go of the lock
    // and that our critical section happens before the next lock holder grabs the lock.
    let state = atom_V_fetchXchg(&self->state, thrd_Mtx__default_unlocked, atom_MemOrd_release);
    debug_assert(state != thrd_Mtx__default_unlocked);
    if (state == thrd_Mtx__default_contended) {
        thrd_ftx_wake(&self->state, 1);
    }
};

$attr($branch_cold)
fn_((thrd_Mtx__default_lockSlow(thrd_Mtx* self))(void)) {
    // Avoid doing an atomic swap below if we already know the state is contended.
    // An atomic swap unconditionally stores which marks the cache-line as modified unnecessarily.
    if (atom_V_load(&self->state, atom_MemOrd_monotonic) == thrd_Mtx__default_contended) {
        thrd_ftx_wait(&self->state, thrd_Mtx__default_contended);
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
    while (atom_V_fetchXchg(&self->state, thrd_Mtx__default_contended, atom_MemOrd_acquire) != thrd_Mtx__default_unlocked) {
        thrd_ftx_wait(&self->state, thrd_Mtx__default_contended);
    }
};
#endif /* !thrd_Mtx_has_specialized */

/*--- Windows ---*/

#if thrd_Mtx_has_specialized && plat_is_windows
fn_((thrd_Mtx__windows_init(void))(thrd_Mtx)) {
    return (thrd_Mtx)thrd_Mtx_init_static();
};

fn_((thrd_Mtx__windows_fini(thrd_Mtx* self))(void)) {
    let_ignore = self;
};

fn_((thrd_Mtx__windows_lock(thrd_Mtx* self))(void)) {
    AcquireSRWLockExclusive(&self->impl.inner);
};

fn_((thrd_Mtx__windows_tryLock(thrd_Mtx* self))(bool)) {
    return TryAcquireSRWLockExclusive(&self->impl.inner) != false;
};

fn_((thrd_Mtx__windows_unlock(thrd_Mtx* self))(void)) {
    ReleaseSRWLockExclusive(&self->impl.inner);
};
#endif /* thrd_Mtx_has_specialized && plat_is_windows */

/*--- Darwin ---*/

#if thrd_Mtx_has_specialized && plat_is_darwin
fn_((thrd_Mtx__darwin_init(void))(thrd_Mtx)) {
    return (thrd_Mtx)thrd_Mtx_init_static();
};

fn_((thrd_Mtx__darwin_fini(thrd_Mtx* self))(void)) {
    let_ignore = self;
};

fn_((thrd_Mtx__darwin_lock(thrd_Mtx* self))(void)) {
    os_unfair_lock_lock(&self->impl.inner);
};

fn_((thrd_Mtx__darwin_tryLock(thrd_Mtx* self))(bool)) {
    return os_unfair_lock_trylock(&self->impl.inner);
};

fn_((thrd_Mtx__darwin_unlock(thrd_Mtx* self))(void)) {
    os_unfair_lock_unlock(&self->impl.inner);
};
#endif /* thrd_Mtx_has_specialized && plat_is_darwin */
