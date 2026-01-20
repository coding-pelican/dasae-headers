#include "dh/io/Buf.h"
#include "dh/io/common.h"
#include "dh/mem/common.h"

/* --- Buffered Reader ---*/

fn_((io_Buf_Reader_init(io_Reader inner, S$u8 buf))(io_Buf_Reader)) {
    return (io_Buf_Reader){
        .inner = inner,
        .buf = buf,
        .start = 0,
        .end = 0,
    };
};

fn_((io_Buf_Reader_fill(io_Buf_Reader* self))(E$void) $scope) {
    // Move remaining data to start
    if (self->start < self->end) {
        let remaining = self->end - self->start;
        prim_memmoveS(
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
} $unscoped_(fn);

fn_((io_Buf_Reader_peekByte(io_Buf_Reader* self))(E$u8) $scope) {
    // HOT PATH: data in buf
    if (self->start < self->end) {
        return_ok(*S_at((self->buf)[self->start]));
    }
    // COLD PATH: refill buf
    try_(io_Buf_Reader_fill(self));
    if (self->end <= self->start) {
        return_err(io_Err_UnexpectedEof());
    }
    return_ok(*S_at((self->buf)[self->start]));
} $unscoped_(fn);

fn_((io_Buf_Reader_readUntilByte(io_Buf_Reader* self, u8 delim, S$u8 out_buf))(E$S$u8) $scope) {
    var_(written, usize) = 0;
    while (written < out_buf.len) {
        // Ensure data in buf
        if (self->end <= self->start) {
            try_(io_Buf_Reader_fill(self));
            if (self->end <= self->start) {
                return_err(io_Err_UnexpectedEof());
            }
        }
        // Search for delimiter in current buf
        let search_end = self->end;
        for_(($r(self->start, search_end))(i) {
            if (*S_at((self->buf)[i]) == delim) {
                // Found delimiter
                let copy_len = i - self->start;
                let total_len = written + copy_len;
                if (out_buf.len < total_len) {
                    return_err(io_Err_BufferTooSmall());
                }
                prim_memcpyS(
                    S_prefix((S_suffix((out_buf)(written)))(copy_len)),
                    S_prefix((S_suffix((self->buf)(self->start)))(copy_len)).as_const
                );
                self->start = i + 1; // Skip delimiter
                return_ok(S_slice((out_buf)$r(0, total_len)));
            }
        });
        // Delimiter not found, copy all available data
        let copy_len = self->end - self->start;
        let total_len = written + copy_len;
        if (out_buf.len < total_len) {
            return_err(io_Err_BufferTooSmall());
        }
        prim_memcpyS(
            S_prefix((S_suffix((out_buf)(written)))(copy_len)),
            S_prefix((S_suffix((self->buf)(self->start)))(copy_len)).as_const
        );
        written += copy_len;
        self->start = self->end;
    }
    return_err(io_Err_BufferTooSmall());
} $unscoped_(fn);

fn_((io_Buf_Reader_skipUntilByte(io_Buf_Reader* self, u8 delim))(E$void) $scope) {
    while (true) {
        // Ensure data in buf
        if (self->end <= self->start) {
            try_(io_Buf_Reader_fill(self));
            if (self->end <= self->start) {
                return_err(io_Err_UnexpectedEof());
            }
        }
        // Search for delimiter in current buf
        for_(($r(self->start, self->end))(i) {
            if (*S_at((self->buf)[i]) == delim) {
                self->start = i + 1; // Skip delimiter
                return_ok({});
            }
        });
        // Delimiter not found, skip all buffered data
        self->start = self->end;
    }
} $unscoped_(fn);

fn_((io_Buf_Reader_skip(io_Buf_Reader* self, usize n))(E$void) $scope) {
    var_(remaining, usize) = n;
    while (remaining > 0) {
        // Use buffered data first
        if (self->start < self->end) {
            let available = self->end - self->start;
            let to_skip = prim_min(available, remaining);
            self->start += to_skip;
            remaining -= to_skip;
            continue;
        }
        // Buffer empty, need to read more
        if (remaining >= self->buf.len) {
            // Large skip, use inner reader directly
            try_(io_Reader_skip(self->inner, remaining));
            return_ok({});
        }
        try_(io_Buf_Reader_fill(self));
        if (self->end <= self->start) {
            return_err(io_Err_UnexpectedEof());
        }
    }
    return_ok({});
} $unscoped_(fn);

$static fn_((Reader_VT_read(P$raw ctx, S$u8 output))(E$usize) $scope) {
    let self = ptrAlignCast$((io_Buf_Reader*)(ctx));
    // HOT PATH: data in buf
    if (self->start < self->end) {
        let available = self->end - self->start;
        let to_copy = prim_min(available, output.len);
        prim_memcpyS(
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
    let to_copy = prim_min(available, output.len);
    prim_memcpyS(
        S_prefix((output)(to_copy)),
        S_prefix((S_suffix((self->buf)(self->start)))(to_copy)).as_const
    );
    self->start += to_copy;
    return_ok(to_copy);
} $unscoped_(fn);

fn_((io_Buf_reader(io_Buf_Reader* self))(io_Reader)) {
    return (io_Reader){
        .ctx = ptrCast$((P$raw)(self)),
        .read = Reader_VT_read,
    };
};

/* --- Buffered Writer ---*/

fn_((io_Buf_Writer_init(io_Writer inner, S$u8 buf))(io_Buf_Writer)) {
    return (io_Buf_Writer){
        .inner = inner,
        .buf = buf,
        .used = 0,
    };
};

fn_((io_Buf_Writer_flush(io_Buf_Writer* self))(E$void) $scope) {
    if (self->used == 0) {
        return_ok({});
    }
    let data = prefix$S(self->buf, self->used).as_const;
    try_(io_Writer_writeBytes(self->inner, data));
    self->used = 0;
    return_ok({});
} $unscoped_(fn);

$static fn_((Writer_VT_write(P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    let self = ptrAlignCast$((io_Buf_Writer*)(ctx));
    // If bytes fit in remaining buf space, just buf them
    let remaining = self->buf.len - self->used;
    if (remaining >= bytes.len) {
        prim_memcpyS(S_prefix((S_suffix((self->buf)(self->used)))(bytes.len)), bytes);
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
    prim_memcpyS(S_prefix((self->buf)(bytes.len)), bytes);
    self->used = bytes.len;
    return_ok(bytes.len);
} $unscoped_(fn);

fn_((io_Buf_writer(io_Buf_Writer* self))(io_Writer)) {
    return (io_Writer){
        .ctx = ptrCast$((P$raw)(self)),
        .write = Writer_VT_write,
    };
};
