#include "dh/io/Buf.h"
#include "dh/io/common.h"
#include "dh/io/Reader.h"
#include "dh/io/Writer.h"
#include "dh/mem/common.h"

/* BufReader ========================*/

fn_((io_Buf_Reader_init(io_Reader inner, S$u8 buf))(io_Buf_Reader)) {
    return (io_Buf_Reader){
        .inner = inner,
        .buf = buf,
        .start = 0,
        .end = 0,
    };
}

fn_((io_Buf_Reader_fill(io_Buf_Reader* self))(E$void) $scope) {
    // Move remaining data to start
    if (self->start < self->end) {
        let remaining = self->end - self->start;
        memmove(self->buf.ptr, self->buf.ptr + self->start, remaining);
        self->start = 0;
        self->end = remaining;
    } else {
        self->start = 0;
        self->end = 0;
    }
    // Read more data
    let available_space = self->buf.len - self->end;
    if (available_space > 0) {
        let read_buf = suffix$S(self->buf, self->end);
        let bytes_read = try_(io_Reader_read(self->inner, read_buf));
        self->end += bytes_read;
    }
    return_ok({});
} $unscoped_(fn);

fn_((io_Buf_Reader_peekByte(io_Buf_Reader* self))(E$u8) $scope) {
    // HOT PATH: data in buf
    if (self->start < self->end) {
        return_ok(*at$S(self->buf, self->start));
    }
    // COLD PATH: refill buf
    try_(io_Buf_Reader_fill(self));
    if (self->start >= self->end) {
        return_err(io_Err_UnexpectedEof());
    }
    return_ok(*at$S(self->buf, self->start));
} $unscoped_(fn);

fn_((io_Buf_Reader_readUntilByte(io_Buf_Reader* self, u8 delim, S$u8 out_buf))(E$S$u8) $scope) {
    usize written = 0;
    while (written < out_buf.len) {
        // Ensure data in buf
        if (self->start >= self->end) {
            try_(io_Buf_Reader_fill(self));
            if (self->start >= self->end) {
                return_err(io_Err_UnexpectedEof());
            }
        }
        // Search for delimiter in current buf
        let search_end = self->end;
        for_(($r(self->start, search_end))(i) {
            if (*at$S(self->buf, i) == delim) {
                // Found delimiter
                let copy_len = i - self->start;
                if (written + copy_len > out_buf.len) {
                    return_err(io_Err_BufferTooSmall());
                }
                memcpy(out_buf.ptr + written, self->buf.ptr + self->start, copy_len);
                self->start = i + 1; // Skip delimiter
                return_ok(slice$S(out_buf, $r(0, written + copy_len)));
            }
        });
        // Delimiter not found, copy all available data
        let copy_len = self->end - self->start;
        if (written + copy_len > out_buf.len) {
            return_err(io_Err_BufferTooSmall());
        }
        memcpy(out_buf.ptr + written, self->buf.ptr + self->start, copy_len);
        written += copy_len;
        self->start = self->end;
    }
    return_err(io_Err_BufferTooSmall());
} $unscoped_(fn);

fn_((io_Buf_Reader_skipUntilByte(io_Buf_Reader* self, u8 delim))(E$void) $scope) {
    while (true) {
        // Ensure data in buf
        if (self->start >= self->end) {
            try_(io_Buf_Reader_fill(self));
            if (self->start >= self->end) {
                return_err(io_Err_UnexpectedEof());
            }
        }
        // Search for delimiter in current buf
        for_(($r(self->start, self->end))(i) {
            if (*at$S(self->buf, i) == delim) {
                self->start = i + 1; // Skip delimiter
                return_ok({});
            }
        });
        // Delimiter not found, skip all buffered data
        self->start = self->end;
    }
} $unscoped_(fn);

fn_((io_Buf_Reader_skip(io_Buf_Reader* self, usize n))(E$void) $scope) {
    usize remaining = n;
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
        if (self->start >= self->end) {
            return_err(io_Err_UnexpectedEof());
        }
    }
    return_ok({});
} $unscoped_(fn);

$static fn_((Reader_VT_read(const P$raw ctx, S$u8 output))(E$usize) $scope) {
    let self = as$((io_Buf_Reader*)(ctx));
    // HOT PATH: data in buf
    if (self->start < self->end) {
        let available = self->end - self->start;
        let to_copy = prim_min(available, output.len);
        memcpy(output.ptr, self->buf.ptr + self->start, to_copy);
        self->start += to_copy;
        return_ok(to_copy);
    }
    // COLD PATH: buf empty
    // If output is larger than buf, read directly
    if (output.len >= self->buf.len) {
        return io_Reader_read(self->inner, output);
    }
    // Otherwise, refill buf and retry
    try_(io_Buf_Reader_fill(self));
    if (self->start >= self->end) {
        return_ok(0); // EOF
    }
    let available = self->end - self->start;
    let to_copy = prim_min(available, output.len);
    memcpy(output.ptr, self->buf.ptr + self->start, to_copy);
    self->start += to_copy;
    return_ok(to_copy);
} $unscoped_(fn);

fn_((io_Buf_Reader_reader(io_Buf_Reader* self))(io_Reader)) {
    return (io_Reader){
        .ctx = as$((P$raw)(self)),
        .read = Reader_VT_read,
    };
}

/* BufWriter ========================*/

fn_((io_Buf_Writer_init(io_Writer inner, S$u8 buf))(io_Buf_Writer)) {
    return (io_Buf_Writer){
        .inner = inner,
        .buf = buf,
        .used = 0,
    };
}

fn_((io_Buf_Writer_flush(io_Buf_Writer* self))(E$void) $scope) {
    if (self->used == 0) {
        return_ok({});
    }
    let data = prefix$S(self->buf, self->used).as_const;
    try_(io_Writer_writeBytes(self->inner, data));
    self->used = 0;
    return_ok({});
} $unscoped_(fn);

$static fn_((Writer_VT_write(const P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    let self = as$((io_Buf_Writer*)(ctx));
    // If bytes fit in remaining buf space, just buf them
    let remaining = self->buf.len - self->used;
    if (bytes.len <= remaining) {
        memcpy(self->buf.ptr + self->used, bytes.ptr, bytes.len);
        self->used += bytes.len;
        return_ok(bytes.len);
    }
    // Buffer is full or will be full - flush first
    try_(io_Buf_Writer_flush(self));
    // If bytes are larger than buf, write directly
    if (bytes.len > self->buf.len) {
        return io_Writer_write(self->inner, bytes);
    }
    // Otherwise, buf the bytes
    memcpy(self->buf.ptr, bytes.ptr, bytes.len);
    self->used = bytes.len;
    return_ok(bytes.len);
} $unscoped_(fn);

fn_((io_Buf_Writer_writer(io_Buf_Writer* self))(io_Writer)) {
    return (io_Writer){
        .ctx = as$((P$raw)(self)),
        .write = Writer_VT_write,
    };
}
