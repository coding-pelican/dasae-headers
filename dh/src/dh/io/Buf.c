#include "dh/io/Buf.h"
#include "dh/mem/common.h"

/*---------- Buffered Reader ------------------------------------------------*/

fn_((io_Buf_Reader_init(io_Reader inner, S$u8 buf))(io_Buf_Reader)) {
    return (io_Buf_Reader){
        .inner = inner,
        .buf = buf,
        .start = 0,
        .end = 0,
    };
};

fn_((io_Buf_Reader_fill(io_Buf_Reader* self))(io_ReadE$void) $scope) {
    // Move remaining data to start
    if (self->start < self->end) {
        let remaining = self->end - self->start;
        mem_moveBytes(
            S_prefix((self->buf)(remaining)),
            S_slice((self->buf)$r(self->start, self->end)).as_const
        );
        self->start = 0;
        self->end = remaining;
    } else {
        self->start = 0;
        self->end = 0;
    }
    // Read more data
    let available_space = self->buf.len - self->end;
    if (available_space > 0) {
        let read_buf = S_suffix((self->buf)(self->end));
        let bytes_read = try_(io_Reader_read(self->inner, read_buf));
        self->end += bytes_read;
    }
    return_ok({});
} $unscoped(fn);

fn_((io_Buf_Reader_require(io_Buf_Reader* self, usize min_len))(io_Buf_ReadE$void) $scope) {
    claim_assert_nonnull(self);
    if (self->buf.len < min_len) {
        return_err(E_cause$TooSmallBuffer());
    }
    while ((self->end - self->start) < min_len) {
        let before = self->end - self->start;
        try_(io_Buf_Reader_fill(self));
        let after = self->end - self->start;
        if (after == before) {
            return_err(E_cause$UnexpectedEOF());
        }
    }
    return_ok({});
} $unscoped(fn);

fn_((io_Buf_Reader_ready(io_Buf_Reader self))(S_const$u8)) {
    return S_slice((self.buf)$r(self.start, self.end)).as_const;
};

fn_((io_Buf_Reader_readyMut(io_Buf_Reader self))(S$u8)) {
    return S_prefix((S_suffix((self.buf)(self.start)))(self.end - self.start));
};

$static fn_((Reader_VT_read(P$raw ctx, S$u8 output))(io_ReadE$usize) $scope) {
    let self = ptrAlignCast$((io_Buf_Reader*)(ctx));
    // HOT PATH: data in buf
    if (self->start < self->end) {
        let available = self->end - self->start;
        let to_copy = pri_min(available, output.len);
        mem_copyBytes(
            S_prefix((output)(to_copy)),
            S_prefix((S_suffix((self->buf)(self->start)))(to_copy)).as_const
        );
        self->start += to_copy;
        return_ok(to_copy);
    }
    // COLD PATH: buf empty
    // If output is larger than buf, read directly
    if (self->buf.len <= output.len) {
        return io_Reader_read(self->inner, output);
    }
    // Otherwise, refill buf and retry
    try_(io_Buf_Reader_fill(self));
    if (self->end <= self->start) {
        return_ok(0); // EOF
    }
    let available = self->end - self->start;
    let to_copy = pri_min(available, output.len);
    mem_copyBytes(
        S_prefix((output)(to_copy)),
        S_prefix((S_suffix((self->buf)(self->start)))(to_copy)).as_const
    );
    self->start += to_copy;
    return_ok(to_copy);
} $unscoped(fn);

fn_((io_Buf_reader(io_Buf_Reader* self))(io_Reader)) {
    return (io_Reader){
        .ctx = ptrCast$((P$raw)(self)),
        .readFn = Reader_VT_read,
    };
};

/*--- Reader cursor ---*/

fn_((io_Buf_Reader_peekByte(io_Buf_Reader* self))(io_Buf_ReadE$u8) $scope) {
    try_(io_Buf_Reader_require(self, 1));
    return_ok(*S_at((self->buf)[self->start]));
} $unscoped(fn);

fn_((io_Buf_Reader_peekBytes(io_Buf_Reader* self, usize len))(io_Buf_ReadE$S_const$u8) $scope) {
    try_(io_Buf_Reader_require(self, len));
    return_ok(S_prefix((S_suffix((self->buf)(self->start)))(len)).as_const);
} $unscoped(fn);

fn_((io_Buf_Reader_takeByte(io_Buf_Reader* self))(io_Buf_ReadE$u8) $scope) {
    let byte = try_(io_Buf_Reader_peekByte(self));
    self->start += 1;
    return_ok(byte);
} $unscoped(fn);

fn_((io_Buf_Reader_takeBytes(io_Buf_Reader* self, usize len))(io_Buf_ReadE$S_const$u8) $scope) {
    let bytes = try_(io_Buf_Reader_peekBytes(self, len));
    self->start += len;
    return_ok(bytes);
} $unscoped(fn);

fn_((io_Buf_Reader_skip(io_Buf_Reader* self, usize len))(io_Buf_ReadE$void) $scope) {
    var_(remaining, usize) = len;
    while (remaining > 0) {
        if (self->start < self->end) {
            let available = self->end - self->start;
            let to_skip = pri_min(available, remaining);
            self->start += to_skip;
            remaining -= to_skip;
            continue;
        }
        if (remaining >= self->buf.len) {
            try_(io_Reader_skip(self->inner, remaining));
            return_ok({});
        }
        try_(io_Buf_Reader_fill(self));
        if (self->end <= self->start) {
            return_err(E_cause$UnexpectedEOF());
        }
    }
    return_ok({});
} $unscoped(fn);

fn_((io_Buf_Reader_drop(io_Buf_Reader* self, usize len))(void)) {
    claim_assert_nonnull(self);
    claim_assert(len <= (self->end - self->start));
    self->start += len;
};

fn_((io_Buf_Reader_dropAll(io_Buf_Reader* self))(void)) {
    claim_assert_nonnull(self);
    self->start = self->end;
};

/*--- Reader delimited read ---*/

fn_((io_Buf_Reader_readUntilByte(io_Buf_Reader* self, u8 delim, S$u8 out_buf))(io_Buf_ReadE$S$u8) $scope) {
    var_(delims, A$$(1, u8)) = A_init({ delim });
    return io_Buf_Reader_readUntilAny(self, A_ref$((S_const$u8)(delims)), out_buf);
} $unscoped(fn);

fn_((io_Buf_Reader_readUntilSeq(io_Buf_Reader* self, S_const$u8 delim, S$u8 out_buf))(io_Buf_ReadE$S$u8) $scope) {
    claim_assert_nonnullS(delim);
    claim_assert(delim.len != 0);
    if (self->buf.len < delim.len) {
        return_err(E_cause$TooSmallBuffer());
    }

    var_(written, usize) = 0;
    while (true) {
        if (self->end <= self->start) {
            try_(io_Buf_Reader_fill(self));
            if (self->end <= self->start) {
                return_err(E_cause$UnexpectedEOF());
            }
        }

        let readable = S_slice((self->buf)$r(self->start, self->end)).as_const;
        if_some((mem_findFirstSeqBytes(readable, delim))(delim_idx)) {
            let copy_len = delim_idx;
            let total_len = written + copy_len;
            if (out_buf.len < total_len) {
                return_err(E_cause$TooSmallBuffer());
            }
            mem_copyBytes(
                S_prefix((S_suffix((out_buf)(written)))(copy_len)),
                S_prefix((readable)(copy_len))
            );
            self->start += delim_idx + delim.len;
            return_ok(S_slice((out_buf)$r(0, total_len)));
        }

        let preserve_len = pri_min(delim.len - 1, readable.len);
        let copy_len = readable.len - preserve_len;
        if (copy_len == 0) {
            let before = self->end - self->start;
            try_(io_Buf_Reader_fill(self));
            let after = self->end - self->start;
            if (after == before) {
                return_err(E_cause$UnexpectedEOF());
            }
            continue;
        }

        let total_len = written + copy_len;
        if (out_buf.len < total_len) {
            return_err(E_cause$TooSmallBuffer());
        }
        mem_copyBytes(
            S_prefix((S_suffix((out_buf)(written)))(copy_len)),
            S_prefix((readable)(copy_len))
        );
        written += copy_len;
        self->start += copy_len;
    }
} $unscoped(fn);

fn_((io_Buf_Reader_readUntilAny(io_Buf_Reader* self, S_const$u8 delims, S$u8 out_buf))(io_Buf_ReadE$S$u8) $scope) {
    claim_assert_nonnullS(delims);
    claim_assert(delims.len != 0);
    var_(written, usize) = 0;
    while (true) {
        if (self->end <= self->start) {
            try_(io_Buf_Reader_fill(self));
            if (self->end <= self->start) {
                return_err(E_cause$UnexpectedEOF());
            }
        }
        let readable = S_slice((self->buf)$r(self->start, self->end)).as_const;
        if_some((mem_findFirstAnyBytes(readable, delims))(delim_idx)) {
            let copy_len = delim_idx;
            let total_len = written + copy_len;
            if (out_buf.len < total_len) {
                return_err(E_cause$TooSmallBuffer());
            }
            mem_copyBytes(
                S_prefix((S_suffix((out_buf)(written)))(copy_len)),
                S_prefix((readable)(copy_len))
            );
            self->start += delim_idx + 1;
            return_ok(S_slice((out_buf)$r(0, total_len)));
        }
        let copy_len = self->end - self->start;
        let total_len = written + copy_len;
        if (out_buf.len < total_len) {
            return_err(E_cause$TooSmallBuffer());
        }
        mem_copyBytes(
            S_prefix((S_suffix((out_buf)(written)))(copy_len)),
            S_prefix((S_suffix((self->buf)(self->start)))(copy_len)).as_const
        );
        written += copy_len;
        self->start = self->end;
    }
} $unscoped(fn);

/*--- Reader delimited skip ---*/

fn_((io_Buf_Reader_skipUntilByte(io_Buf_Reader* self, u8 delim))(io_Buf_ReadE$void) $scope) {
    var_(delims, A$$(1, u8)) = A_init({ delim });
    return io_Buf_Reader_skipUntilAny(self, A_ref$((S_const$u8)(delims)));
} $unscoped(fn);

fn_((io_Buf_Reader_skipUntilSeq(io_Buf_Reader* self, S_const$u8 delim))(io_Buf_ReadE$void) $scope) {
    claim_assert_nonnullS(delim);
    claim_assert(delim.len != 0);
    if (self->buf.len < delim.len) {
        return_err(E_cause$TooSmallBuffer());
    }

    while (true) {
        if (self->end <= self->start) {
            try_(io_Buf_Reader_fill(self));
            if (self->end <= self->start) {
                return_err(E_cause$UnexpectedEOF());
            }
        }

        let readable = S_slice((self->buf)$r(self->start, self->end)).as_const;
        if_some((mem_findFirstSeqBytes(readable, delim))(delim_idx)) {
            self->start += delim_idx + delim.len;
            return_ok({});
        }

        let preserve_len = pri_min(delim.len - 1, readable.len);
        let skip_len = readable.len - preserve_len;
        if (skip_len == 0) {
            let before = self->end - self->start;
            try_(io_Buf_Reader_fill(self));
            let after = self->end - self->start;
            if (after == before) {
                return_err(E_cause$UnexpectedEOF());
            }
            continue;
        }
        self->start += skip_len;
    }
} $unscoped(fn);

fn_((io_Buf_Reader_skipUntilAny(io_Buf_Reader* self, S_const$u8 delims))(io_Buf_ReadE$void) $scope) {
    claim_assert_nonnullS(delims);
    claim_assert(delims.len != 0);
    while (true) {
        if (self->end <= self->start) {
            try_(io_Buf_Reader_fill(self));
            if (self->end <= self->start) {
                return_err(E_cause$UnexpectedEOF());
            }
        }
        let readable = S_slice((self->buf)$r(self->start, self->end)).as_const;
        if_some((mem_findFirstAnyBytes(readable, delims))(delim_idx)) {
            self->start += delim_idx + 1;
            return_ok({});
        }
        self->start = self->end;
    }
} $unscoped(fn);

/*---------- Buffered Writer ------------------------------------------------*/

fn_((io_Buf_Writer_init(io_Writer inner, S$u8 buf))(io_Buf_Writer)) {
    return (io_Buf_Writer){
        .inner = inner,
        .buf = buf,
        .used = 0,
    };
};

fn_((io_Buf_Writer_pending(io_Buf_Writer self))(S_const$u8)) {
    return S_prefix((self.buf)(self.used)).as_const;
};

fn_((io_Buf_Writer_pendingMut(io_Buf_Writer self))(S$u8)) {
    return S_prefix((S_suffix((self.buf)(self.used)))(self.buf.len - self.used));
};

fn_((io_Buf_Writer_flush(io_Buf_Writer* self))(io_WriteE$void) $scope) {
    if (self->used == 0) return_ok({});
    let data = S_prefix((self->buf)(self->used)).as_const;
    try_(io_Writer_writeBytes(self->inner, data));
    self->used = 0;
    return_ok({});
} $unscoped(fn);

$static fn_((Writer_VT_write(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize) $scope) {
    let self = ptrAlignCast$((io_Buf_Writer*)(ctx));
    // If bytes fit in remaining buf space, just buf them
    let remaining = self->buf.len - self->used;
    if (remaining >= bytes.len) {
        mem_copyBytes(S_prefix((S_suffix((self->buf)(self->used)))(bytes.len)), bytes);
        self->used += bytes.len;
        return_ok(bytes.len);
    }
    // Buffer is full or will be full - flush first
    try_(io_Buf_Writer_flush(self));
    // If bytes are larger than buf, write directly
    if (self->buf.len < bytes.len) {
        return io_Writer_write(self->inner, bytes);
    }
    // Otherwise, buf the bytes
    mem_copyBytes(S_prefix((self->buf)(bytes.len)), bytes);
    self->used = bytes.len;
    return_ok(bytes.len);
} $unscoped(fn);

fn_((io_Buf_writer(io_Buf_Writer* self))(io_Writer)) {
    return (io_Writer){
        .ctx = ptrCast$((P$raw)(self)),
        .writeFn = Writer_VT_write,
    };
};
