#include "dh-main.h"
#include "dh/atom.h"

TEST_fn_("builtin/atom: fetch min and max preserve fetch-before-update semantics" $scope) {
    volatile i32 signed_value = 7;
    try_(TEST_expect(atom_pri_fetchMin(&signed_value, -3, atom_MemOrd_seq_cst) == 7));
    try_(TEST_expect(atom_load(&signed_value, atom_MemOrd_seq_cst) == -3));
    try_(TEST_expect(atom_pri_fetchMin(&signed_value, 4, atom_MemOrd_acq_rel) == -3));
    try_(TEST_expect(atom_load(&signed_value, atom_MemOrd_acquire) == -3));
    try_(TEST_expect(atom_pri_fetchMax(&signed_value, 5, atom_MemOrd_release) == -3));
    try_(TEST_expect(atom_load(&signed_value, atom_MemOrd_acquire) == 5));

    volatile u32 unsigned_value = u32_(0x80000000);
    try_(TEST_expect(atom_pri_fetchMin(&unsigned_value, u32_(1), atom_MemOrd_monotonic) == u32_(0x80000000)));
    try_(TEST_expect(atom_pri_fetchMax(&unsigned_value, u32_limit_max, atom_MemOrd_seq_cst) == u32_(1)));
    try_(TEST_expect(atom_load(&unsigned_value, atom_MemOrd_seq_cst) == u32_limit_max));
    return_ok({});
} $unscoped(TEST_fn)

TEST_fn_("builtin/atom: min fetch and max fetch preserve update-after semantics" $scope) {
    volatile i32 signed_value = 7;
    try_(TEST_expect(atom_pri_minFetch(&signed_value, -3, atom_MemOrd_seq_cst) == -3));
    try_(TEST_expect(atom_load(&signed_value, atom_MemOrd_seq_cst) == -3));
    try_(TEST_expect(atom_pri_minFetch(&signed_value, 4, atom_MemOrd_acq_rel) == -3));
    try_(TEST_expect(atom_load(&signed_value, atom_MemOrd_acquire) == -3));
    try_(TEST_expect(atom_pri_maxFetch(&signed_value, 5, atom_MemOrd_release) == 5));
    try_(TEST_expect(atom_load(&signed_value, atom_MemOrd_acquire) == 5));

    volatile u32 unsigned_value = u32_(0x80000000);
    try_(TEST_expect(atom_pri_minFetch(&unsigned_value, u32_(1), atom_MemOrd_monotonic) == u32_(1)));
    try_(TEST_expect(atom_pri_maxFetch(&unsigned_value, u32_limit_max, atom_MemOrd_seq_cst) == u32_limit_max));
    try_(TEST_expect(atom_load(&unsigned_value, atom_MemOrd_seq_cst) == u32_limit_max));
    return_ok({});
} $unscoped(TEST_fn)

TEST_fn_("atom: lock-free queries expose builtin results" $scope) {
    volatile u8 byte = 0;
    try_(TEST_expect(atom_alwaysLockFreeSize(sizeOf$(u8))));
    try_(TEST_expect(atom_alwaysLockFree$(u8)));
    try_(TEST_expect(atom_isLockFree(&byte)));

    var_(wrapped, atom_V$$(u32)) = atom_V_init(0u);
    try_(TEST_expect(atom_V_alwaysLockFree$(u32)));
    try_(TEST_expect(atom_V_isLockFree(&wrapped)));
    return_ok({});
} $unscoped(TEST_fn)

TEST_fn_("atom: wrapped fetch min and max support floating-point values" $scope) {
    var_(value, atom_V$$(f32)) = atom_V_init(f32_(3.5));
    try_(TEST_expect(atom_V_pri_fetchMin(&value, f32_(1.25), atom_MemOrd_acq_rel) == f32_(3.5)));
    try_(TEST_expect(atom_V_load(&value, atom_MemOrd_acquire) == f32_(1.25)));
    try_(TEST_expect(atom_V_pri_fetchMax(&value, f32_(4.75), atom_MemOrd_seq_cst) == f32_(1.25)));
    try_(TEST_expect(atom_V_load(&value, atom_MemOrd_seq_cst) == f32_(4.75)));

    try_(TEST_expect(atom_V_pri_fetchMin(&value, f32_nan, atom_MemOrd_seq_cst) == f32_(4.75)));
    try_(TEST_expect(atom_V_load(&value, atom_MemOrd_seq_cst) == f32_(4.75)));
    atom_V_store(&value, f32_nan, atom_MemOrd_seq_cst);
    try_(TEST_expect(f32_isNaN(atom_V_pri_fetchMax(&value, f32_(2.0), atom_MemOrd_seq_cst))));
    try_(TEST_expect(atom_V_load(&value, atom_MemOrd_seq_cst) == f32_(2.0)));
    return_ok({});
} $unscoped(TEST_fn)

TEST_fn_("atom: wrapped min fetch and max fetch support floating-point values" $scope) {
    var_(value, atom_V$$(f32)) = atom_V_init(f32_(3.5));
    try_(TEST_expect(atom_V_pri_minFetch(&value, f32_(1.25), atom_MemOrd_acq_rel) == f32_(1.25)));
    try_(TEST_expect(atom_V_load(&value, atom_MemOrd_acquire) == f32_(1.25)));
    try_(TEST_expect(atom_V_pri_maxFetch(&value, f32_(4.75), atom_MemOrd_seq_cst) == f32_(4.75)));
    try_(TEST_expect(atom_V_load(&value, atom_MemOrd_seq_cst) == f32_(4.75)));

    try_(TEST_expect(atom_V_pri_minFetch(&value, f32_nan, atom_MemOrd_seq_cst) == f32_(4.75)));
    try_(TEST_expect(atom_V_load(&value, atom_MemOrd_seq_cst) == f32_(4.75)));
    atom_V_store(&value, f32_nan, atom_MemOrd_seq_cst);
    try_(TEST_expect(atom_V_pri_maxFetch(&value, f32_(2.0), atom_MemOrd_seq_cst) == f32_(2.0)));
    try_(TEST_expect(atom_V_load(&value, atom_MemOrd_seq_cst) == f32_(2.0)));
    return_ok({});
} $unscoped(TEST_fn)

TEST_fn_("atom: spin loop hint is callable" $scope) {
    atom_spinLoopHint();
    return_ok({});
} $unscoped(TEST_fn)

TEST_fn_("atom: lock preserves single-owner state transitions" $scope) {
    var lock = atom_SpinLock_init();
    try_(TEST_expect(!atom_SpinLock_isLocked(&lock)));
    try_(TEST_expect(atom_SpinLock_tryLock(&lock)));
    try_(TEST_expect(atom_SpinLock_isLocked(&lock)));
    try_(TEST_expect(!atom_SpinLock_tryLock(&lock)));
    atom_SpinLock_unlock(&lock);
    try_(TEST_expect(!atom_SpinLock_isLocked(&lock)));

    atom_SpinLock_lock(&lock);
    try_(TEST_expect(atom_SpinLock_isLocked(&lock)));
    atom_SpinLock_unlock(&lock);
    return_ok({});
} $unscoped(TEST_fn)
