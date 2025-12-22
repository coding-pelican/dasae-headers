#include "dh/main.h"
#include "dh/atom.h"
#include "dh/Thrd.h"
#include "dh/io/stream.h"

// Shared counter between threads
T_use_atom_V$(i32);
T_use_atom_V$(bool);

// Shared buffer state
typedef struct SharedBuf {
    atom_V$(bool) is_active;
    atom_V$(i32) write_idx;
    atom_V$(i32) read_idx;
    A$$(1024, i32) data;
} SharedBuf;
// Initialize shared buffer
fn_((SharedBuf_init(void))(SharedBuf)) {
    return (SharedBuf){
        .write_idx = atom_V_init(0),
        .read_idx = atom_V_init(0),
        .is_active = atom_V_init(true),
        .data = zero$A(),
    };
}

// Producer thread function
$static Thrd_fn_(producer, ({ SharedBuf* buf; }, Void), ($ignore, args)$scope) {
    let buf = args->buf;
    i32 value = 0;
    while (atom_V_load(&buf->is_active, atom_MemOrd_acquire)) {
        let write_idx = atom_V_load(&buf->write_idx, atom_MemOrd_acquire);
        let read_idx = atom_V_load(&buf->read_idx, atom_MemOrd_acquire);
        // Check if buffer is not full
        if ((write_idx - read_idx) < as$(i32)(len$A(buf->data))) {
            // Store value in buffer
            *at$A(buf->data, write_idx % 1024) = value;
            // Increment write index atomically
            atom_V_store(&buf->write_idx, write_idx + 1, atom_MemOrd_release);
            io_stream_println(u8_l("Produced: {:d}"), value++);
            continue;
        }
        // Buffer full, yield to consumer
        atom_spinLoopHint();
    }
    return_void();
} $unscoped_(Thrd_fn);

// Consumer thread function
$static Thrd_fn_(consumer, ({ SharedBuf* buf; }, Void), ($ignore, args)$scope) {
    let buf = args->buf;
    i32 consumed_count = 0;
    while (atom_V_load(&buf->is_active, atom_MemOrd_acquire)) {
        let write_idx = atom_V_load(&buf->write_idx, atom_MemOrd_acquire);
        let read_idx = atom_V_load(&buf->read_idx, atom_MemOrd_acquire);
        // Check if buffer is not empty
        if (read_idx < write_idx) {
            // Read value from buffer
            let value = *at$A(buf->data, read_idx % 1024);
            // Increment read index atomically
            atom_V_store(&buf->read_idx, read_idx + 1, atom_MemOrd_release);
            io_stream_println(u8_l("Consumed: {:d}"), value);
            consumed_count++;
            // Stop after consuming 100 items
            if (consumed_count < 100) { continue; }
            atom_V_store(&buf->is_active, false, atom_MemOrd_release);
            break;
        }
        // Buffer empty, yield to producer
        atom_spinLoopHint();
    }
    return_void();
} $unscoped_(Thrd_fn);

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    var_(buffer, SharedBuf) = SharedBuf_init();
    let producer_thrd = try_(Thrd_spawn(Thrd_SpawnCfg_default, Thrd_FnCtx_from$((producer)(&buffer)).as_raw));
    let consumer_thrd = try_(Thrd_spawn(Thrd_SpawnCfg_default, Thrd_FnCtx_from$((consumer)(&buffer)).as_raw));
    Thrd_join(producer_thrd);
    Thrd_join(consumer_thrd);
    io_stream_println(u8_l("Processing complete"));
    return_ok({});
} $unscoped_(fn);
