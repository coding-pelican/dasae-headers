#pragma once
#ifndef dansi_test_support__included
#define dansi_test_support__included 1

#include <dh/io/Reader.h>
#include <dh/mem/common.h>

typedef struct dansi_test_ChunkReader {
    var_(bytes, S_const$u8);
    var_(pos, usize);
    var_(chunk, usize);
} dansi_test_ChunkReader;

$static fn_((dansi_test_ChunkReader__read(P$raw ctx, S$u8 out))(io_ReadE$usize) $scope) {
    let self = ptrAlignCast$((dansi_test_ChunkReader*)(ctx));
    let available = self->bytes.len - self->pos;
    if (available == 0) return_ok(0);
    let to_read = pri_min(pri_min(available, out.len), self->chunk);
    mem_copyBytes(
        S_prefix((out)(to_read)),
        S_prefix((S_suffix((self->bytes)(self->pos)))(to_read))
    );
    self->pos += to_read;
    return_ok(to_read);
} $unscoped(fn);

$static fn_((dansi_test_ChunkReader_reader(dansi_test_ChunkReader* self))(io_Reader)) {
    return (io_Reader){
        .ctx = ptrCast$((P$raw)(self)),
        .readFn = dansi_test_ChunkReader__read,
    };
};

#endif /* dansi_test_support__included */
