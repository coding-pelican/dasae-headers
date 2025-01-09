#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <zlib.h>

// -lz

const uint8_t PNG_SIGNATURE[] = { 137, 80, 78, 71, 13, 10, 26, 10 };

struct PNG_IHDR {
    uint32_t width;
    uint32_t height;
    uint8_t  bit_depth;
    uint8_t  color_type;
    uint8_t  compression;
    uint8_t  filter;
    uint8_t  interlace;
};

uint32_t readU32(const uint8_t* data) {
    return ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) | ((uint32_t)data[2] << 8) | (uint32_t)data[3];
}

int rgbToAnsiColorIndexed8(uint8_t r, uint8_t g, uint8_t b) {
    int ansi = 0;
    if (r > 127) {
        ansi |= 1;
    }
    if (g > 127) {
        ansi |= 2;
    }
    if (b > 127) {
        ansi |= 4;
    }
    return ansi;
}

void unfilterScanline(uint8_t* line, uint8_t* prev_line, int len, int bpp, uint8_t filter_type) {
    int i;
    switch (filter_type) {
    case 0:
        break;
    case 1:
        for (i = bpp; i < len; i++) {
            line[i] += line[i - bpp];
        }
        break;
    case 2:
        if (prev_line) {
            for (i = 0; i < len; i++) {
                line[i] += prev_line[i];
            }
        }
        break;
    case 3:
        for (i = 0; i < len; i++) {
            int a = (i < bpp) ? 0 : line[i - bpp];
            int b = prev_line ? prev_line[i] : 0;
            line[i] += (uint8_t)((a + b) / 2);
        }
        break;
    case 4:
        for (i = 0; i < len; i++) {
            int a = (i < bpp) ? 0 : line[i - bpp];
            int b = prev_line ? prev_line[i] : 0;
            int c = (i < bpp || !prev_line) ? 0 : prev_line[i - bpp];

            int p  = a + b - c;
            int pa = abs(p - a);
            int pb = abs(p - b);
            int pc = abs(p - c);

            if (pa <= pb && pa <= pc) {
                line[i] += (uint8_t)a;
            } else if (pb <= pc) {
                line[i] += (uint8_t)b;
            } else {
                line[i] += (uint8_t)c;
            }
        }
        break;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename.png>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // Verify PNG signature
    uint8_t header[8];
    if (fread(header, 1, 8, file) != 8 || memcmp(header, PNG_SIGNATURE, 8) != 0) {
        fprintf(stderr, "Invalid PNG signature\n");
        fclose(file);
        return 1;
    }

    // Read IHDR chunk
    uint8_t ihdr_chunk[25]; // includes length, type, data, and CRC
    if (fread(ihdr_chunk, 1, 25, file) != 25) {
        fprintf(stderr, "Error reading IHDR chunk\n");
        fclose(file);
        return 1;
    }

    struct PNG_IHDR ihdr;
    ihdr.width       = readU32(ihdr_chunk + 8);
    ihdr.height      = readU32(ihdr_chunk + 12);
    ihdr.bit_depth   = ihdr_chunk[16];
    ihdr.color_type  = ihdr_chunk[17];
    ihdr.compression = ihdr_chunk[18];
    ihdr.filter      = ihdr_chunk[19];
    ihdr.interlace   = ihdr_chunk[20];

    printf("Image dimensions: %ux%u\n", ihdr.width, ihdr.height);
    printf("Bit depth: %u\n", ihdr.bit_depth);
    printf("Color type: %u\n", ihdr.color_type);

    // Collect IDAT chunks
    uint8_t* compressed_data     = NULL;
    size_t   compressed_size     = 0;
    size_t   compressed_capacity = 0;
    uint8_t  chunk_header[8];

    while (1) {
        if (fread(chunk_header, 1, 8, file) != 8) {
            break;
        }

        uint32_t chunk_length  = readU32(chunk_header);
        char     chunk_type[5] = { 0 };
        memcpy(chunk_type, chunk_header + 4, 4);

        if (strcmp(chunk_type, "IDAT") == 0) {
            if (compressed_size + chunk_length > compressed_capacity) {
                compressed_capacity = (compressed_size + chunk_length) * 2;
                uint8_t* new_data   = realloc(compressed_data, compressed_capacity);
                if (!new_data) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(compressed_data);
                    fclose(file);
                    return 1;
                }
                compressed_data = new_data;
            }

            if (fread(compressed_data + compressed_size, 1, chunk_length, file) != chunk_length) {
                fprintf(stderr, "Error reading IDAT chunk data\n");
                free(compressed_data);
                fclose(file);
                return 1;
            }
            compressed_size += chunk_length;

            // Skip CRC
            fseek(file, 4, SEEK_CUR);
        } else if (strcmp(chunk_type, "IEND") == 0) {
            break;
        } else {
            // Skip unknown chunk
            fseek(file, chunk_length + 4, SEEK_CUR);
        }
    }

    printf("Compressed size: %zu bytes\n", compressed_size);

    // Calculate dimensions
    int      bytes_per_pixel = (ihdr.color_type == 6) ? 4 : 3;
    int      stride          = ihdr.width * bytes_per_pixel + 1;
    int      output_size     = stride * ihdr.height;
    uint8_t* image_data      = malloc(output_size);
    if (!image_data) {
        fprintf(stderr, "Failed to allocate image buffer\n");
        free(compressed_data);
        fclose(file);
        return 1;
    }

    // Setup zlib stream
    z_stream zstream  = { 0 };
    zstream.next_in   = compressed_data;
    zstream.avail_in  = compressed_size;
    zstream.next_out  = image_data;
    zstream.avail_out = output_size;

    if (inflateInit(&zstream) != Z_OK) {
        fprintf(stderr, "Failed to initialize zlib\n");
        free(compressed_data);
        free(image_data);
        fclose(file);
        return 1;
    }

    // Decompress
    int ret = inflate(&zstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        fprintf(stderr, "Decompression failed: %s\n", zstream.msg);
        inflateEnd(&zstream);
        free(compressed_data);
        free(image_data);
        fclose(file);
        return 1;
    }

    inflateEnd(&zstream);
    free(compressed_data);

    // Display the image
    uint8_t* prev_line = NULL;
    for (uint32_t y = 0; y < ihdr.height; y++) {
        uint8_t* line        = image_data + y * stride;
        uint8_t  filter_type = line[0];

        unfilterScanline(line + 1, prev_line ? prev_line + 1 : NULL, stride - 1, bytes_per_pixel, filter_type);

        for (uint32_t x = 0; x < ihdr.width; x++) {
            int     pixel_pos = 1 + x * bytes_per_pixel;
            uint8_t r         = line[pixel_pos];
            uint8_t g         = line[pixel_pos + 1];
            uint8_t b         = line[pixel_pos + 2];

            int color = rgbToAnsiColorIndexed8(r, g, b);
            printf("\033[4%dm  ", color);
        }
        printf("\033[0m\n");

        prev_line = line;
    }

    free(image_data);
    fclose(file);
    return 0;
}
