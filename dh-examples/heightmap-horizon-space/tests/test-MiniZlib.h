#include "dh/core.h"
#include <string.h>

/* --------------------------------------------------------------------------
   Minimal inflate (zlib/deflate) routine. This is a simplified approach:
   - It does not handle every possible corner case
   - It is adequate for many standard PNGs
   - For brevity, error handling is minimal
   In production, you might prefer an established library or a more robust
   inflate implementation.
-------------------------------------------------------------------------- */

typedef struct MiniZlibInflator {
    const u8* input;
    usize     input_len;
    usize     bit_pos;
    u8*       output;
    usize     output_size;
    usize     output_pos;
} MiniZlibInflator;

/* Read bits from the input stream */
static $inline u32 MiniZlib_readBits(MiniZlibInflator* self, i32 num_bits) {
    u32 result = 0;
    for (i32 i = 0; i < num_bits; ++i) {
        // If we run out of input, bail out
        if (self->bit_pos >= self->input_len * 8) {
            // error or return partial
            return 0;
        }
        // read the current bit
        u32 bit = (self->input[self->bit_pos >> 3] >> (self->bit_pos & 7)) & 1;
        self->bit_pos++;
        // shift it into result
        result |= bit << i;
    }
    return result;
}

/* A very minimal "uncompressed" block reader (Type=0) */
static $inline i32 MiniZlib_inflateUncompressedBlock(MiniZlibInflator* self) {
    // align to byte boundary
    self->bit_pos = (self->bit_pos + 7) & ~7;
    if ((self->bit_pos >> 3) + 4 > self->input_len) {
        return -1; // error
    }
    u32 len = self->input[self->bit_pos >> 3]
            | (self->input[(self->bit_pos >> 3) + 1] << 8);
    u32 nlen = self->input[(self->bit_pos >> 3) + 2]
             | (self->input[(self->bit_pos >> 3) + 3] << 8);
    self->bit_pos += 32;
    if ((len & 0xFFFF) != (~nlen & 0xFFFF)) {
        return -1; // error
    }
    if (((self->bit_pos >> 3) + len) > self->input_len) {
        return -1; // error
    }
    if (self->output_pos + len > self->output_size) {
        return -1; // not enough output buffer
    }
    memcpy(self->output + self->output_pos, self->input + (self->bit_pos >> 3), len);
    self->output_pos += len;
    self->bit_pos += (len << 3);
    return 0;
}

/* Stub for compressed blocks (Type=1/2).
   In a real-world scenario, you’d implement the Huffman codes, etc.
   This example only handles 'uncompressed' blocks for brevity.
*/
static $inline i32 MiniZlib_inflateCompressedBlock(MiniZlibInflator* self, i32 type) {
    $unused(self);
    $unused(type);
    // *** You would handle Type=1 and Type=2 here with Huffman decoding ***
    $ignore fprintf(stderr, "Compressed block not supported in this minimal example.\n");
    return -1;
}

/* Minimal inflate: expects zlib header + raw DEFLATE blocks (no dictionary). */
static $inline i32 MiniZlib_inflate(const u8* in, usize in_len, u8* out, usize out_size, usize* out_len) {
    MiniZlibInflator inf;
    memset(&inf, 0, sizeof(inf));
    inf.input       = in;
    inf.input_len   = in_len;
    inf.output      = out;
    inf.output_size = out_size;
    inf.output_pos  = 0;
    inf.bit_pos     = 0;

    // Skip zlib header (2 bytes) and possibly check Adler at end (skipped here).
    // In real PNG data, there's often a ZLIB header; but partial IDAT might skip.
    // For simplicity, just skip 2 bytes if present:
    if (in_len < 2) {
        return -1;
    }
    usize pos = 0; // track our reading within 'in'
    pos += 2;      // skip zlib header
    inf.bit_pos = pos * 8;

    i32 final_block = 0;
    while (!final_block) {
        final_block    = (i32)MiniZlib_readBits(&inf, 1);
        i32 block_type = (i32)MiniZlib_readBits(&inf, 2);
        i32 res        = 0;
        if (block_type == 0) {
            res = MiniZlib_inflateUncompressedBlock(&inf);
        } else {
            res = MiniZlib_inflateCompressedBlock(&inf, block_type);
        }
        if (res < 0) {
            return -1;
        }
    }

    *out_len = inf.output_pos;
    return 0;
}
