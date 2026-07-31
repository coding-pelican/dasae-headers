#include "dh-main.h"
#include "dh/atom.h"
#include "dh/thrd.h"
#include "dh/clsr.h"
#include "dh/heap/Sys.h"
#include "dh/io/stream.h"

T_use_atom_V$(i32);
T_use$((Void)(thrd_spawn, thrd_join));

#define SharedBuf_capacity 256

T_alias$((SharedBuf)(struct SharedBuf {
    var_(is_active, atom_V$bool);
    var_(write_idx, atom_V$i32);
    var_(read_idx, atom_V$i32);
    var_(data, A$$(SharedBuf_capacity, i32));
}));

$static fn_((SharedBuf_init(void))(SharedBuf)) {
    return (SharedBuf){
        .write_idx = atom_V_init(0),
        .read_idx = atom_V_init(0),
        .is_active = atom_V_init(true),
        .data = A_zero(),
    };
};

$static fn_((producer(SharedBuf* buf))(Void) $scope) {
    var_(value, i32) = 0;
    while (atom_V_load(&buf->is_active, atom_MemOrd_acquire)) {
        let write_idx = atom_V_load(&buf->write_idx, atom_MemOrd_acquire);
        let read_idx = atom_V_load(&buf->read_idx, atom_MemOrd_acquire);
        if ((write_idx - read_idx) < SharedBuf_capacity) {
            *A_at((buf->data)[as$(usize)(write_idx % SharedBuf_capacity)]) = value;
            atom_V_store(&buf->write_idx, write_idx + 1, atom_MemOrd_release);
            io_stream_println(u8_l("Produced: {:d}"), value++);
            continue;
        }
        atom_spinLoopHint();
    }
    return_void();
} $unscoped(fn);
fn_use_Clsr_((producer)(SharedBuf*)(Void));

$static fn_((consumer(SharedBuf* buf))(Void) $scope) {
    var_(consumed_count, i32) = 0;
    while (atom_V_load(&buf->is_active, atom_MemOrd_acquire)) {
        let write_idx = atom_V_load(&buf->write_idx, atom_MemOrd_acquire);
        let read_idx = atom_V_load(&buf->read_idx, atom_MemOrd_acquire);
        if (read_idx < write_idx) {
            let value = *A_at((buf->data)[as$(usize)(read_idx % SharedBuf_capacity)]);
            atom_V_store(&buf->read_idx, read_idx + 1, atom_MemOrd_release);
            io_stream_println(u8_l("Consumed: {:d}"), value);
            consumed_count++;
            if (consumed_count < 100) continue;
            atom_V_store(&buf->is_active, false, atom_MemOrd_release);
            break;
        }
        atom_spinLoopHint();
    }
    return_void();
} $unscoped(fn);
fn_use_Clsr_((consumer)(SharedBuf*)(Void));

fn_((main(proc_Entry entry))(E$void) $guard) {
    let_ignore = entry;

    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    let_(spawn_cfg, thrd_SpawnCfg) = {
        .gpa = heap_Sys_alctr(&heap),
        .stack_size = thrd_SpawnCfg_default_stack_size,
    };
    var buffer = SharedBuf_init();
    var producer_work = clsr_((producer)(&buffer));
    var consumer_work = clsr_((consumer)(&buffer));

    let producer_thrd = try_(thrd_spawn$Void(spawn_cfg, producer_work.as_base));
    let consumer_thrd = try_(thrd_spawn$Void(spawn_cfg, consumer_work.as_base));
    let_ignore = thrd_join$Void(producer_thrd);
    let_ignore = thrd_join$Void(consumer_thrd);

    io_stream_println(u8_l("Processing complete"));
    return_ok({});
} $unguarded(fn);
