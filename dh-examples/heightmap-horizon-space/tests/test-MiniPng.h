#include "dh/core.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "test-MiniZlib.h"

/* PNG signature is 8 bytes */
static const u8 MiniPng_sig[8] = {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
};

/* Filter recon helpers for each byte */
$inline_always u8 MiniPng_paeth(u8 a, u8 b, u8 c) {
    i32 p  = a + b - c;
    i32 pa = abs(p - a);
    i32 pb = abs(p - b);
    i32 pc = abs(p - c);
    if (pa <= pb && pa <= pc) {
        return a;
    }
    if (pb <= pc) {
        return b;
    }
    return c;
}

/* Decode one scanline with the specified PNG filter type */
static $inline void MiniPng_filterScanline(
    u8*       scanline,
    const u8* prev_scanline,
    i32       bytes_per_pixel,
    i32       filter_type,
    i32       row_bytes
) {
    switch (filter_type) {
    case 0: // None
        break;
    case 1: // Sub
        for (i32 i = bytes_per_pixel; i < row_bytes; i++) {
            scanline[i] += scanline[i - bytes_per_pixel];
        }
        break;
    case 2: // Up
        if (prev_scanline) {
            for (i32 i = 0; i < row_bytes; i++) {
                scanline[i] += prev_scanline[i];
            }
        }
        break;
    case 3: // Average
        for (i32 i = 0; i < row_bytes; i++) {
            u8 up   = prev_scanline ? prev_scanline[i] : 0;
            u8 left = (i >= bytes_per_pixel) ? scanline[i - bytes_per_pixel] : 0;
            u8 avg  = (u8)(((i32)up + (i32)left) / 2);
            scanline[i] += avg;
        }
        break;
    case 4: // Paeth
        for (i32 i = 0; i < row_bytes; i++) {
            u8 up     = prev_scanline ? prev_scanline[i] : 0;
            u8 left   = (i >= bytes_per_pixel) ? scanline[i - bytes_per_pixel] : 0;
            u8 upLeft = (prev_scanline && i >= bytes_per_pixel) ? prev_scanline[i - bytes_per_pixel] : 0;
            scanline[i] += MiniPng_paeth(left, up, upLeft);
        }
        break;
    default:
        // Unsupported filter
        break;
    }
}

/* Load a PNG from file (8-bit only).
   - out_w, out_h: store image width/height
   - out_channels: store channels (1=gray, 3=RGB, 4=RGBA)
   - returns newly allocated buffer with raw pixels (row-major, top-down).
   - returns null on error.

   This code handles color type=0 (gray), 2 (RGB), and 6 (RGBA) at 8-bit depth only.
*/
static $inline u8* MiniPng_load8bit(const char* filename, i32* out_w, i32* out_h, i32* out_channels) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        $ignore fprintf(stderr, "Cannot open file: %s\n", filename);
        return null;
    }

    /* Read signature */
    u8 sig[8];
    if (fread(sig, 1, 8, fp) != 8) {
        $ignore fclose(fp);
        return null;
    }
    if (memcmp(sig, MiniPng_sig, 8) != 0) {
        $ignore fclose(fp);
        $ignore fprintf(stderr, "Not a PNG file.\n");
        return null;
    }

    i32   width      = 0;
    i32   height     = 0;
    i32   bit_depth  = 0;
    i32   color_type = 0;
    u8*   idat_data  = null;
    usize idat_size  = 0;
    i32   done       = 0;

    while (!done) {
        // Each chunk: length (4 bytes), type (4 bytes), data (length), CRC (4 bytes)
        u8 length_bytes[4];
        if (fread(length_bytes, 1, 4, fp) != 4) {
            break; // no more chunks
        }
        u32 length = (length_bytes[0] << 24) | (length_bytes[1] << 16) | (length_bytes[2] << 8) | (length_bytes[3] << 0);

        u8 type_bytes[5] = { 0 };
        if (fread(type_bytes, 1, 4, fp) != 4) {
            break; // chunk type
        }
        type_bytes[4] = '\0';

        // Allocate chunk data
        u8* chunk_data = (u8*)malloc(length);
        if (chunk_data == null) {
            $ignore fclose(fp);
            return null;
        }

        // Read chunk data
        if (fread(chunk_data, 1, length, fp) != length) {
            free(chunk_data);
            break;
        }

        // Read CRC ($ignore in this simplified code)
        u8 crc_bytes[4];
        $ignore fread(crc_bytes, 1, 4, fp);

        if (!strcmp((char*)type_bytes, "IHDR")) {
            // Parse IHDR
            if (length == 13) {
                width      = (chunk_data[0] << 24) | (chunk_data[1] << 16) | (chunk_data[2] << 8) | (chunk_data[3] << 0);
                height     = (chunk_data[4] << 24) | (chunk_data[5] << 16) | (chunk_data[6] << 8) | (chunk_data[7] << 0);
                bit_depth  = chunk_data[8];
                color_type = chunk_data[9];
                // We only support 8-bit depth, color types 0,2,6
                if (bit_depth != 8 || (color_type != 0 && color_type != 2 && color_type != 6)) {
                    $ignore fprintf(stderr, "Unsupported PNG format (bit_depth=%d, color_type=%d)\n", bit_depth, color_type);
                    free(chunk_data);
                    $ignore fclose(fp);
                    return null;
                }
            }
        } else if (!strcmp((char*)type_bytes, "IDAT")) {
            // Accumulate IDAT
            u8* temp = (u8*)realloc(idat_data, idat_size + length);
            if (!temp) {
                free(chunk_data);
                $ignore fclose(fp);
                return null;
            }
            idat_data = temp;
            memcpy(idat_data + idat_size, chunk_data, length);
            idat_size += length;
        } else if (!strcmp((char*)type_bytes, "IEND")) {
            // end of PNG
            done = 1;
        }

        free(chunk_data);
    }

    $ignore fclose(fp);

    // We now have width, height, bit_depth=8, color_type in {0,2,6},
    // and IDAT data in idat_data (size = idat_size).
    if (width <= 0 || height <= 0 || !idat_data) {
        free(idat_data);
        return null;
    }

    // Each row has a 1-byte filter type + actual pixel data
    // for color_type=0 => 1 channel
    // for color_type=2 => 3 channels (RGB)
    // for color_type=6 => 4 channels (RGBA)
    i32 channels = 0;
    switch (color_type) {
    case 0:
        channels = 1;
        break; // gray
    case 2:
        channels = 3;
        break; // rgb
    case 6:
        channels = 4;
        break; // rgba
    default:
        free(idat_data);
        $ignore fprintf(stderr, "Unsupported PNG format (color_type=%d)\n", color_type);
        return null;
    }
    usize raw_len_per_scanline = ((usize)channels * width);
    usize raw_len_total        = (raw_len_per_scanline + 1) * height;

    // Decompress IDAT
    u8* decomp_data = (u8*)malloc(raw_len_total * 4); // some over-alloc
    if (!decomp_data) {
        free(idat_data);
        return null;
    }
    memset(decomp_data, 0, raw_len_total * 4);
    usize out_len = 0;

    if (MiniZlib_inflate(idat_data, idat_size, decomp_data, raw_len_total * 4, &out_len) != 0) {
        free(idat_data);
        free(decomp_data);
        $ignore fprintf(stderr, "Inflate failed (or block type not supported).\n");
        return null;
    }
    free(idat_data);

    if (out_len < raw_len_total) {
        // Not enough data
        free(decomp_data);
        $ignore fprintf(stderr, "Inflated data too short.\n");
        return null;
    }

    // Now we decode the scanline filters
    u8* image = (u8*)malloc((isize)width * height * channels);
    if (!image) {
        free(decomp_data);
        return null;
    }

    u8*   prev_line    = null;
    u8*   current_line = decomp_data;
    usize src_offset   = 0;
    $unused(src_offset);
    for (i32 y = 0; y < height; y++) {
        i32 filter_type = current_line[0];
        u8* scanline    = current_line + 1;
        MiniPng_filterScanline(scanline, prev_line, channels, filter_type, width * channels);
        memcpy(image + (usize)(y * width * channels), scanline, (isize)width * channels);

        prev_line = scanline;
        current_line += (width * channels + 1);
        src_offset += (width * channels + 1);
    }

    free(decomp_data);

    *out_w        = width;
    *out_h        = height;
    *out_channels = channels;
    return image;
}
