#include "dh/main.h"
#include "dh/core.h"
#include "dh/heap/Classic.h"
#include "dh/log.h"

#include "dh/debug.h"

#include "dh/math/vec.h"
#include "dh/mem/Allocator.h"
#include "dh/ArrList.h"
#include "dh/meta/common.h"
#include "dh/Mat.h"

#include "dh/Random.h"

#include "dh/scope/return.h"
#include "engine.h"
#include "engine/color.h"
#include "engine/platform_backend.h"

#include "dh/Str.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "../libs/stb_truetype.h"

use_Grid$(u8);
use_Grid$(f32);
use_Sli$(Color);
use_Grid$(Color);

#define window_res_width__320x200  /* template value */ (320)
#define window_res_height__320x200 /* template value */ (200)
#define window_res_width__160x100  /* template value */ (160)
#define window_res_height__160x100 /* template value */ (100)
#define window_res_width__80x50    /* template value */ (80)
#define window_res_height__80x50   /* template value */ (50)
#define window_res_width__40x25    /* template value */ (40)
#define window_res_height__40x25   /* template value */ (25)

#define window_res_width  (window_res_width__320x200)
#define window_res_height (window_res_height__320x200)
#define window_res_size   (as$(usize, window_res_width) * window_res_height)

/* (1.0 / target_fps__62_50) ~16ms => ~60 FPS, Assume 62.5 FPS for simplicity */
#define target_fps__62_50 /* template value */ (62.50)
#define target_fps__50_00 /* template value */ (50.00)
#define target_fps__31_25 /* template value */ (31.25)

#define target_fps (target_fps__62_50)
#define target_spf (1.0 / target_fps)
#define font_size  (18)

// ========== Font Data Structures ==========
typedef struct KoreanFont {
    stbtt_fontinfo info;
    Sli$u8         font_buffer;
    f32            scale;
    i32            ascent;
    i32            descent;
    i32            line_gap;
    mem_Allocator  allocator;
} KoreanFont;
use_Opt$(KoreanFont);
use_Err$(KoreanFont);
use_Sli$(KoreanFont);
use_Grid$(KoreanFont);

typedef struct Glyph {
    Grid$u8 bitmap; // alpha values
    i32     width;
    i32     height;
    i32     x_offset;
    i32     y_offset;
    i32     advance;
} Glyph;
use_Opt$(Glyph);
use_Err$(Glyph);
use_Sli$(Glyph);
use_Grid$(Glyph);
use_ArrList$(Glyph);

// ========== Font Errors / Declarations (unchanged) ==========
config_ErrSet(
    FontSystemErr,
    FailedToOpenFontFile,
    FailedToLoadFontInfo,
    FailedToInitFontInfo,
    FailedToLoadGlyph,
    FailedToRenderGlyph
);

// ========== FontSystem_init / fini (unchanged, except we read .ttf) ==========
must_check Err$KoreanFont FontSystem_init(mem_Allocator allocator, const char* filename, f32 size_pixels) {
    scope_reserveReturn(Err$KoreanFont) {
        FILE* font_file = fopen(filename, "rb");
        if (!font_file) {
            return_err(FontSystemErr_err(FontSystemErrCode_FailedToOpenFontFile));
        }
        defer(ignore fclose(font_file));

        ignore fseek(font_file, 0, SEEK_END);
        long   file_size = ftell(font_file);
        ignore fseek(font_file, 0, SEEK_SET);

        let font_buffer = meta_cast$(Sli$u8, try(mem_Allocator_alloc(allocator, typeInfo(u8), file_size)));
        errdefer(mem_Allocator_free(allocator, anySli(font_buffer)));
        if (fread(font_buffer.ptr, 1, (size_t)file_size, font_file) != (size_t)file_size) {
            return_err(FontSystemErr_err(FontSystemErrCode_FailedToLoadFontInfo));
        }

        KoreanFont font = {
            .font_buffer = font_buffer,
            .allocator   = allocator
        };

        if (!stbtt_InitFont(&font.info, font_buffer.ptr, 0)) {
            return_err(FontSystemErr_err(FontSystemErrCode_FailedToInitFontInfo));
        }

        font.scale = stbtt_ScaleForPixelHeight(&font.info, size_pixels);

        stbtt_GetFontVMetrics(&font.info, &font.ascent, &font.descent, &font.line_gap);
        return_ok(font);
    }
    scope_returnReserved;
}

void FontSystem_fini(KoreanFont* font) {
    if (font->font_buffer.ptr) {
        mem_Allocator_free(font->allocator, anySli(font->font_buffer));
    }
}

// ========== Updated FontSystem_renderGlyph using stbtt_GetGlyphBitmapBox ==========
// 1) Use bounding-box approach with stbtt_GetGlyphBitmapBox
// 2) Allocate raw bytes with typeInfo(u8)
// 3) Remove the old bounding logic based on x0,y0 from stbtt_GetGlyphBox
must_check Err$Glyph FontSystem_renderGlyph(const KoreanFont* font, i32 codepoint, f32 scale_factor) {
    scope_reserveReturn(Err$Glyph) {
        // 1) Find glyph index
        i32 glyph_index = stbtt_FindGlyphIndex(&font->info, codepoint);
        if (glyph_index == 0) {
            return_err(FontSystemErr_err(FontSystemErrCode_FailedToLoadGlyph));
        }

        // 2) Horizontal metrics (for advance)
        i32 advance = 0;
        i32 lsb     = 0;
        stbtt_GetGlyphHMetrics(&font->info, glyph_index, &advance, &lsb);

        // 3) bounding box in pixel coords
        f32 final_scale = font->scale * scale_factor;
        i32 ix0         = 0;
        i32 iy0         = 0;
        i32 ix1         = 0;
        i32 iy1         = 0;
        stbtt_GetGlyphBitmapBox(
            &font->info, glyph_index, final_scale, final_scale, &ix0, &iy0, &ix1, &iy1
        );

        i32 w = ix1 - ix0;
        i32 h = iy1 - iy0;
        if (w <= 0 || h <= 0) {
            // blank or empty glyph, return a zero-bitmap
            Glyph gBlank = {
                .bitmap   = Grid_fromSli$(Grid$u8, meta_cast$(Sli$u8, try(mem_Allocator_alloc(font->allocator, typeInfo(u8), 1))), 1, 1),
                .width    = 0,
                .height   = 0,
                .x_offset = 0,
                .y_offset = 0,
                .advance  = (i32)((f32)advance * final_scale)
            };
            return_ok(gBlank);
        }

        // 4) Allocate memory for the bitmap
        usize  size = (usize)w * h;
        Sli$u8 mem  = meta_cast$(
            Sli$u8,
            try(mem_Allocator_alloc(font->allocator, typeInfo(u8), size))
        );
        let bitmap = Grid_fromSli$(Grid$u8, mem, w, h);

        // 5) Render via stbtt_GetGlyphBitmap
        i32 outW     = 0;
        i32 outH     = 0;
        u8* stb_data = stbtt_GetGlyphBitmap(
            &font->info,
            final_scale,
            final_scale,
            glyph_index,
            &outW,
            &outH,
            0,
            0
        );
        if (!stb_data) {
            mem_Allocator_free(font->allocator, anySli(bitmap.items));
            return_err(FontSystemErr_err(FontSystemErrCode_FailedToRenderGlyph));
        }
        defer(stbtt_FreeBitmap(stb_data, null));

        // 6) copy
        if (outW != w || outH != h) {
            log_warn("Warning: bounding box mismatch: box(%d,%d), out(%d,%d)\n", w, h, outW, outH);
        }
        memcpy(bitmap.items.ptr, stb_data, (usize)outW * outH);

        // 7) Build final glyph
        Glyph glyph = {
            .bitmap   = bitmap,
            .width    = w,
            .height   = h,
            .x_offset = ix0, // subpixel offset from bounding box
            .y_offset = iy0,
            .advance  = (i32)((f32)advance * final_scale)
        };
        return_ok(glyph);
    }
    scope_returnReserved;
}

// ========== Render glyph to canvas (unchanged) ==========
void FontSystem_renderGlyphToCanvas(const Glyph* glyph, engine_Canvas* canvas, i32 x, i32 y, Color color) {
    for (i32 dy = 0; dy < glyph->height; ++dy) {
        for (i32 dx = 0; dx < glyph->width; ++dx) {
            u8 alpha = *Grid_at(glyph->bitmap, dx, dy);
            if (alpha <= 0) { continue; }
            // scale color
            Color pixel_color = Color_fromOpaque(
                (u8)((u32)color.r * alpha / 255),
                (u8)((u32)color.g * alpha / 255),
                (u8)((u32)color.b * alpha / 255)
            );
            engine_Canvas_drawPixel(
                canvas,
                x + glyph->x_offset + dx,
                y + glyph->y_offset + dy,
                pixel_color
            );
        }
    }
}

// ========== Remove utf8_decode() and use StrUtf8Iter instead ==========
// This function now takes Str_const instead of const char* for text input.
must_check Err$void renderKoreanText(Str_const text, engine_Canvas* canvas, i32 x, i32 y, usize size_pixels) {
    scope_reserveReturn(Err$void) {
        var        allocator = heap_Classic_allocator(&(heap_Classic){});
        // 1) Init font
        KoreanFont font      = try(FontSystem_init(allocator, "../assets/Galmuri9.ttf", size_pixels));
        defer(FontSystem_fini(&font));

        i32 cursor_x = x;
        i32 cursor_y = y;

        // 2) Create an iterator over code points
        StrUtf8Iter iter = StrUtf8_iter(text);
        Opt$u32     cp   = { 0 };
        // 3) For each codepoint
        while (StrUtf8Iter_next(&iter, &cp)) {
            if (isNone(cp)) {
                // Invalid codepoint or end
                continue;
            }
            i32 codepoint = (i32)cp.value;

            // 4) Render glyph
            scope_with(let glyph = try(FontSystem_renderGlyph(&font, codepoint, 1.0f))) {
                FontSystem_renderGlyphToCanvas(
                    &glyph,
                    canvas,
                    cursor_x,
                    cursor_y,
                    Color_fromOpaque(255, 255, 255)
                );
                // move X
                cursor_x += glyph.advance;

                // release glyph memory
                mem_Allocator_free(allocator, anySli(glyph.bitmap.items));
            }
        }
        return_ok({});
    }
    scope_returnReserved;
}

// ========== Example usage in dh_main() loop ==========
// We call `renderKoreanText(Str_l("안녕하세요."), canvas, x, y)` instead of passing `const char*`.
Err$void dh_main(i32 argc, const char* argv[]) {
    unused(argc), unused(argv);
    scope_reserveReturn(Err$void) {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("main-debug.log", "w"), debug_file) {
            log_initWithFile(debug_file);
            // Configure logging behavior
            log_setLevel(log_Level_debug);
            log_showTimestamp(true);
            log_showLevel(true);
            log_showLocation(false);
            log_showFunction(true);
        }
        defer(log_fini());

        // Initialize platform with terminal backend
        let window = try(engine_Window_create(
            &(engine_PlatformParams){
                .backend_type  = engine_RenderBackendType_vt100,
                .window_title  = "Korean Font Test",
                .width         = window_res_width,
                .height        = window_res_height,
                .default_color = Color_blue,
            }
        ));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        // Create canvases
        let game_canvas = catch (engine_Canvas_create(window_res_width, window_res_height, engine_CanvasType_rgba), err, {
            log_error("Failed to create canvas: %s\n", err);
            return_err(err);
        });
        defer(engine_Canvas_destroy(game_canvas));
        log_info("canvas created\n");

        engine_Canvas_clearDefault(game_canvas);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, window_res_width, window_res_height);
        log_info("canvas views added\n");

        // Initialize game state
        // var allocator = heap_Classic_allocator(&(heap_Classic){});
        log_info("game state created\n");
        ignore getchar();

        // Initialize timing variables
        let target_frame_time = time_Duration_fromSecs_f64(target_spf);
        var prev_frame_time   = time_Instant_now();
        log_info("game loop started\n");

        // Game loop
        bool is_running = true;
        while (is_running) {
            // 1) Capture the start of the frame (and capture the end-of-frame time of prev iteration’s dt includes sleep)
            let curr_frame_time = time_Instant_now();

            // 2) Compute how long since last frame (purely for your dt usage)
            let elapsed = time_Instant_durationSince(curr_frame_time, prev_frame_time);
            let dt      = time_Duration_asSecs_f64(elapsed);

            // 3) Process input/events
            try(engine_Window_processEvents(window));

            // 4) Update game state
            if (engine_Key_pressed(engine_KeyCode_esc)) {
                log_debug("esc pressed\n");
                is_running = false;
            }
            if (engine_Mouse_pressed(engine_MouseButton_left)) {
                log_debug("mouse left pressed\n");
                let        pos           = engine_Mouse_getPosition();
                static i32 pressed_count = 0;
                Str_const  pressed_str   = Str_l("pressed_str");
                switch (pressed_count) {
                case 0:
                    pressed_str = Str_l("안녕하세요.");
                    break;
                case 1:
                    pressed_str = Str_l("안녕하세요, 한글.");
                    break;
                case 2:
                    pressed_str = Str_l("안녕하세요, 한글 테스트입니다.");
                    break;
                case 3:
                    pressed_str = Str_l("안녕하세요, 한글 테스트입니다람쥐썬더!!");
                    break;
                case 4:
                default:
                    pressed_str = Str_l("잘 출력되어라 제바아아아알.");
                    break;
                }
                try(renderKoreanText(pressed_str, game_canvas, pos.x, pos.y, font_size));
                pressed_count++;
                try(renderKoreanText(Str_l("안녕하세요, 스트로 처리중!"), game_canvas, 20, 50, font_size));
            }

            // 5) Render all views
            engine_Window_present(window);

            // 6) (Optional) Display instantaneous FPS
            const f64 fps = (0.0 < dt) ? (1.0 / dt) : 9999.0;
            printf("\033[H"); // Move cursor to top left
            printf("\rFPS: %6.2f\n", fps);
            debug_only(
                // log frame every 1s
                static f64 total_game_time_for_timestamp = 0.0;
                static f64 logging_after_duration        = 0.0;
                total_game_time_for_timestamp += dt;
                logging_after_duration += dt;
                if (1.0 < logging_after_duration) {
                    logging_after_duration = 0.0;
                    log_debug("[t=%6.2f] dt: %6.2f, fps %6.2f\n", total_game_time_for_timestamp, dt, 1.0 / dt);
                }
            );

            // 7) Measure how long the update+render actually took
            let now        = time_Instant_now();
            let frame_used = time_Instant_durationSince(now, curr_frame_time);

            // 8) Subtract from our target; clamp to zero if negative
            if_some(time_Duration_chkdSub(target_frame_time, frame_used), leftover) {
                time_sleep(leftover);
            }
            prev_frame_time = curr_frame_time;
        }
        return_void();
    }
    scope_returnReserved;
}
