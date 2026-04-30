#include "dh/heap/Smp.h"
#include "dh/meta.h"

/*========== Internal Declarations ==========================================*/

$static fn_((heap_Smp__cpuCount(heap_Smp* self))(u32));

typedef struct heap_Smp__CacheEntry {
    var_(inst_addr, usize); /// Key: allocator instance address
    var_(slot_idx, u32); /// Value: last successful slot index
    var_(lru_age, u32); /// LRU(Least Recently Used) age
} heap_Smp__CacheEntry;
#define heap_Smp__cache_size /*:usize*/ (arch_cache_line_bytes / sizeOf$(heap_Smp__CacheEntry))
$static $thrd_local var_(heap_Smp__cache, A$$(heap_Smp__cache_size, heap_Smp__CacheEntry)) = A_zero();

typedef struct heap_Smp__Locked {
    var_(meta, heap_Smp_ThrdMeta*);
    var_(idx, u32);
} heap_Smp__Locked;
$static fn_((heap_Smp__lockThrdMeta(heap_Smp* self))(heap_Smp__Locked));
$static fn_((heap_Smp__updateCache(usize addr, u32 idx))(void));

$static fn_((heap_Smp__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Smp__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Smp__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Smp__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

$static fn_((heap_Smp__sizeClassIdx(usize len, mem_Align align))(usize));
$static fn_((heap_Smp__slotSize(usize class_idx))(usize));

/*========== External Definitions ===========================================*/

let_(heap_Smp_vtbl, mem_Alctr_VTbl) = {
    .alloc = heap_Smp__alloc,
    .resize = heap_Smp__resize,
    .remap = heap_Smp__remap,
    .free = heap_Smp__free,
};

fn_((heap_Smp_alctr(heap_Smp* self))(mem_Alctr)) {
    claim_assert_nonnull(self);
    return mem_Alctr_ensureValid((mem_Alctr){
        .ctx = self,
        .vtbl = &heap_Smp_vtbl,
    });
};

fn_((heap_Smp_from(mem_Alctr backing_alctr, S$heap_Smp_ThrdMeta thrd_metas))(heap_Smp)) {
    backing_alctr = mem_Alctr_ensureValid(backing_alctr);
    claim_assert_nonnullS(thrd_metas);
    claim_assert(thrd_metas.len <= heap_Smp_max_thrd_count);
    return (heap_Smp){
        .backing_alctr = backing_alctr,
        .thrd_metas = thrd_metas,
        .cpu_count = 0,
    };
};

fn_((heap_Smp_createOnHeap(mem_Alctr backing_alctr, usize thrd_meta_count))(mem_E$P$heap_Smp) $scope) {
    let thrd_meta_arr_type = u_typeInfoA(thrd_meta_count, typeInfo$(heap_Smp_ThrdMeta));
    let record_field_types = typeInfosFrom(typeInfo$(heap_Smp), thrd_meta_arr_type);
    let record = try_(mem_Alctr_create($trace backing_alctr, u_typeInfoRecord(record_field_types)));
    let record_fields = u_fieldPtrsMut(record, record_field_types, A_ref$((S$u_P$raw)l0$((A$$(2, u_P$raw)))));
    let smp = u_castP$((P$heap_Smp)(*S_at((record_fields)[0])));
    smp->backing_alctr = backing_alctr;
    smp->thrd_metas = u_castS$((S$heap_Smp_ThrdMeta)(u_prefixP(*S_at((record_fields)[1]), thrd_meta_count)));
    smp->cpu_count = 0;
    return_ok(smp);
} $unscoped(fn);

fn_((heap_Smp_destroyOnHeap(P$heap_Smp* self))(void)) {
    let thrd_meta_arr_type = u_typeInfoA((*self)->thrd_metas.len, typeInfo$(heap_Smp_ThrdMeta));
    let record_field_types = typeInfosFrom(typeInfo$(heap_Smp), thrd_meta_arr_type);
    let record = u_recordPtrMut(u_anyP(*self), record_field_types, 0);
    *self = (mem_Alctr_destroy($trace(*self)->backing_alctr, record), null);
};

/*========== Internal Definitions ===========================================*/

fn_((heap_Smp__cpuCount(heap_Smp* self))(u32)) {
    let cpu_count = atom_load(&self->cpu_count, atom_MemOrd_unordered);
    if (cpu_count != 0) { return cpu_count; }
    let max_count = heap_Smp_max_thrd_count;
    let n = int_min(intCast$((u32)catch_((Thrd_cpuCount())($ignore, max_count))), max_count);
    return expr_(u32 $scope)(if_some((atom_cmpXchgStrong(
        &self->cpu_count, 0, n, atom_MemOrd_monotonic, atom_MemOrd_monotonic
    ))(other)) {
        $break_(other);
    } else_none {
        $break_(n);
    }) $unscoped(expr);
};

fn_((heap_Smp__lockThrdMeta(heap_Smp* self))(heap_Smp__Locked) $scope) {
    let self_addr = ptrToInt(self);
    for_(($s(A_ref(heap_Smp__cache)))(entry)) {
        if (entry->inst_addr == self_addr) {
            let idx = entry->slot_idx;
            let meta = S_at((self->thrd_metas)[idx]);
            entry->lru_age = 0;
            if (Thrd_Mtx_tryLock(&meta->mtx)) {
                return_({
                    .meta = meta,
                    .idx = idx,
                });
            }
            break;
        }
        entry->lru_age = u32_addSat(entry->lru_age, 1);
    } $end(for);
    var_(idx, u32) = 0;
    let cpu_count = heap_Smp__cpuCount(self);
    while (true) {
        let meta = S_at((self->thrd_metas)[idx]);
        if (Thrd_Mtx_tryLock(&meta->mtx)) {
            heap_Smp__updateCache(self_addr, idx);
            return_({
                .meta = meta,
                .idx = idx,
            });
        }
        idx = (idx + 1) % cpu_count;
    }
} $unscoped(fn);

fn_((heap_Smp__updateCache(usize addr, u32 idx))(void)) {
    var_(oldest_idx, usize) = 0;
    var_(max_age, u32) = 0;
    for_(($s(A_ref(heap_Smp__cache)), $rf(0))(entry, i)) {
        if (entry->inst_addr == 0 || entry->inst_addr == addr) {
            entry->inst_addr = addr;
            entry->slot_idx = idx;
            entry->lru_age = 0;
            return;
        }
        if (entry->lru_age > max_age) {
            max_age = entry->lru_age;
            oldest_idx = i;
        }
    } $end(for);
    asg_l((A_at((heap_Smp__cache)[oldest_idx]))({
        .inst_addr = addr,
        .slot_idx = idx,
        .lru_age = 0,
    }));
};

fn_((heap_Smp__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $guard) {
    let self = ptrAlignCast$((heap_Smp*)(ctx));
    let class_idx = heap_Smp__sizeClassIdx(len, align);
    if ($branch_unlikely(class_idx >= heap_Smp_size_class_count)) {
        return_some(orelse_((mem_Alctr_rawAlloc($trace self->backing_alctr, len, align))(return_none())));
    }
    let slot_size = heap_Smp__slotSize(class_idx);
    claim_assert(heap_Smp_slab_len % slot_size == 0);
    var_(search_count, u8) = 0;
    let locked = heap_Smp__lockThrdMeta(self);
    var locked_meta = locked.meta;
    var locked_idx = locked.idx;
    loop_labeled(outer, while (true)) {
        let top_free_ptr = *A_at((locked_meta->frees)[class_idx]);
        if ($branch_likely(top_free_ptr != 0)) blk_defer {
            defer_(Thrd_Mtx_unlock(&locked_meta->mtx));
            let node = intToPtr$((usize*)(top_free_ptr));
            *node = *A_at((locked_meta->frees)[class_idx]);
            *A_at((locked_meta->frees)[class_idx]) = ptrToInt(node);
            return_some(intToPtr$((u8*)(top_free_ptr)));
        } blk_deferral;

        let next_addr = *A_at((locked_meta->next_addrs)[class_idx]);
        if ($branch_likely((next_addr % heap_Smp_slab_len) != 0)) blk_defer {
            defer_(Thrd_Mtx_unlock(&locked_meta->mtx));
            *A_at((locked_meta->next_addrs)[class_idx]) = next_addr + slot_size;
            return_some(intToPtr$((u8*)(next_addr)));
        } blk_deferral;

        if ($branch_likely(search_count >= heap_Smp_max_alloc_search)) blk_defer {
            defer_(Thrd_Mtx_unlock(&locked_meta->mtx));
            let ptr = orelse_((mem_Alctr_rawAlloc(
                $trace self->backing_alctr,
                heap_Smp_slab_len,
                mem_alignToLog2(heap_Smp_slab_len)
            ))(return_none()));
            *A_at((locked_meta->next_addrs)[class_idx]) = ptrToInt(ptr) + slot_size;
            return_some(ptr);
        } blk_deferral;

        Thrd_Mtx_unlock(&locked_meta->mtx);
        let cpu_count = heap_Smp__cpuCount(self);
        claim_assert(cpu_count != 0);
        var_(idx, u32) = locked_idx;
        while (true) {
            idx = (idx + 1) % cpu_count;
            locked_meta = S_at((self->thrd_metas)[idx]);
            if (Thrd_Mtx_tryLock(&locked_meta->mtx)) {
                locked_idx = idx;
                search_count += 1;
                loop_continue_(outer);
            }
        }
    } $end(loop_labeled);
} $unguarded(fn);

fn_((heap_Smp__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let self = ptrAlignCast$((heap_Smp*)(ctx));
    let old_class_idx = heap_Smp__sizeClassIdx(buf.len, buf_align);
    let new_class_idx = heap_Smp__sizeClassIdx(new_len, buf_align);
    if (heap_Smp_size_class_count <= old_class_idx) {
        if (new_class_idx < heap_Smp_size_class_count) return false;
        return mem_Alctr_rawResize($trace self->backing_alctr, buf, buf_align, new_len);
    }
    return old_class_idx == new_class_idx;
};

fn_((heap_Smp__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Smp*)(ctx));
    let old_class_idx = heap_Smp__sizeClassIdx(buf.len, buf_align);
    let new_class_idx = heap_Smp__sizeClassIdx(new_len, buf_align);
    if (heap_Smp_size_class_count <= old_class_idx) {
        if (new_class_idx < heap_Smp_size_class_count) return_none();
        return mem_Alctr_rawRemap($trace self->backing_alctr, buf, buf_align, new_len);
    }
    return_(expr_(O$P$u8 $scope)(
        old_class_idx == new_class_idx
            ? $break_(some(buf.ptr))
            : $break_(none())
    ) $unscoped(expr));
} $unscoped(fn);

fn_((heap_Smp__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void) $guard) {
    let self = ptrAlignCast$((heap_Smp*)(ctx));
    let class_idx = heap_Smp__sizeClassIdx(buf.len, buf_align);
    if ($branch_unlikely(class_idx >= heap_Smp_size_class_count)) {
        return_void(mem_Alctr_rawFree($trace self->backing_alctr, buf, buf_align));
    }
    let node = ptrAlignCast$((usize*)(buf.ptr));
    let locked = heap_Smp__lockThrdMeta(self);
    let locked_meta = locked.meta;
    defer_(Thrd_Mtx_unlock(&locked_meta->mtx));
    *node = *A_at((locked_meta->frees)[class_idx]);
    *A_at((locked_meta->frees)[class_idx]) = ptrToInt(node);
} $unguarded(fn);

fn_((heap_Smp__sizeClassIdx(usize len, mem_Align align))(usize)) {
    return int_max(int_max(int_bits$(usize) - int_leadingZeros(len - 1), align), heap_Smp_min_size_class)
         - heap_Smp_min_size_class;
};

fn_((heap_Smp__slotSize(usize class_idx))(usize)) {
    return as$(usize)(1) << (class_idx + heap_Smp_min_size_class);
};
