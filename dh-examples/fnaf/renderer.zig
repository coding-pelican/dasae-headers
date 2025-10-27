const std = @import("std");
const game = @import("game.zig");

// SDL2 bindings via C import
const c = @cImport({
    @cInclude("SDL2/SDL.h");
    @cInclude("SDL2/SDL_ttf.h");
});

// ============================================================================
// Rendering Constants
// ============================================================================

const SCREEN_WIDTH: i32 = 1280;
const SCREEN_HEIGHT: i32 = 720;
const TARGET_FPS: u32 = 60;
const FRAME_TIME_MS: u32 = 1000 / TARGET_FPS;

// UI Colors
const COLOR_BLACK = c.SDL_Color{ .r = 0, .g = 0, .b = 0, .a = 255 };
const COLOR_WHITE = c.SDL_Color{ .r = 255, .g = 255, .b = 255, .a = 255 };
const COLOR_GREEN = c.SDL_Color{ .r = 0, .g = 255, .b = 0, .a = 255 };
const COLOR_RED = c.SDL_Color{ .r = 255, .g = 0, .b = 0, .a = 255 };
const COLOR_YELLOW = c.SDL_Color{ .r = 255, .g = 255, .b = 0, .a = 255 };

// ============================================================================
// Asset Management
// ============================================================================

pub const Texture = struct {
    ptr: *c.SDL_Texture,
    width: i32,
    height: i32,

    pub fn destroy(self: *Texture) void {
        c.SDL_DestroyTexture(self.ptr);
    }
};

pub const AssetManager = struct {
    allocator: std.mem.Allocator,
    renderer: *c.SDL_Renderer,
    font: *c.TTF_Font,

    // Placeholder textures (in real implementation, load actual images)
    office_texture: Texture,
    camera_textures: [11]?Texture, // One per Location

    pub fn init(allocator: std.mem.Allocator, renderer: *c.SDL_Renderer) !AssetManager {
        // Initialize TTF
        if (c.TTF_Init() == -1) {
            return error.TTFInitFailed;
        }

        // Load font (fallback to system font or create texture-based text)
        const font = c.TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 24) orelse {
            // If font not found, try to continue without it (will handle in rendering)
            std.debug.print("Warning: Could not load font\n", .{});
            return error.FontLoadFailed;
        };

        // Create placeholder textures
        var assets = AssetManager{
            .allocator = allocator,
            .renderer = renderer,
            .font = font,
            .office_texture = try createPlaceholderTexture(renderer, 1280, 720, COLOR_BLACK),
            .camera_textures = [_]?Texture{null} ** 11,
        };

        // Create placeholder camera textures
        for (&assets.camera_textures, 0..) |*tex, i| {
            const gray_value: u8 = @intCast(20 + i * 20);
            const color = c.SDL_Color{ .r = gray_value, .g = gray_value, .b = gray_value, .a = 255 };
            tex.* = try createPlaceholderTexture(renderer, 1280, 720, color);
        }

        return assets;
    }

    pub fn deinit(self: *AssetManager) void {
        self.office_texture.destroy();
        for (&self.camera_textures) |*tex| {
            if (tex.*) |*t| {
                t.destroy();
            }
        }
        c.TTF_CloseFont(self.font);
        c.TTF_Quit();
    }
};

fn createPlaceholderTexture(renderer: *c.SDL_Renderer, width: i32, height: i32, color: c.SDL_Color) !Texture {
    const texture = c.SDL_CreateTexture(
        renderer,
        c.SDL_PIXELFORMAT_RGBA8888,
        c.SDL_TEXTUREACCESS_TARGET,
        width,
        height,
    ) orelse return error.TextureCreationFailed;

    // Set as render target and clear with color
    _ = c.SDL_SetRenderTarget(renderer, texture);
    _ = c.SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    _ = c.SDL_RenderClear(renderer);
    _ = c.SDL_SetRenderTarget(renderer, null);

    return Texture{
        .ptr = texture,
        .width = width,
        .height = height,
    };
}

// ============================================================================
// Renderer
// ============================================================================

pub const Renderer = struct {
    window: *c.SDL_Window,
    renderer: *c.SDL_Renderer,
    assets: AssetManager,
    allocator: std.mem.Allocator,

    pub fn init(allocator: std.mem.Allocator) !Renderer {
        // Initialize SDL
        if (c.SDL_Init(c.SDL_INIT_VIDEO | c.SDL_INIT_AUDIO) < 0) {
            std.debug.print("SDL Init Error: {s}\n", .{c.SDL_GetError()});
            return error.SDLInitFailed;
        }

        // Create window
        const window = c.SDL_CreateWindow(
            "Five Nights at Freddy's - Zig Edition",
            c.SDL_WINDOWPOS_CENTERED,
            c.SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            c.SDL_WINDOW_SHOWN,
        ) orelse {
            std.debug.print("Window Creation Error: {s}\n", .{c.SDL_GetError()});
            return error.WindowCreationFailed;
        };

        // Create renderer
        const renderer = c.SDL_CreateRenderer(
            window,
            -1,
            c.SDL_RENDERER_ACCELERATED | c.SDL_RENDERER_PRESENTVSYNC,
        ) orelse {
            std.debug.print("Renderer Creation Error: {s}\n", .{c.SDL_GetError()});
            return error.RendererCreationFailed;
        };

        // Load assets
        const assets = AssetManager.init(allocator, renderer) catch |err| {
            std.debug.print("Asset loading failed: {}\n", .{err});
            return err;
        };

        return Renderer{
            .window = window,
            .renderer = renderer,
            .assets = assets,
            .allocator = allocator,
        };
    }

    pub fn deinit(self: *Renderer) void {
        self.assets.deinit();
        c.SDL_DestroyRenderer(self.renderer);
        c.SDL_DestroyWindow(self.window);
        c.SDL_Quit();
    }

    /// Main rendering function - reads game state and renders frame
    pub fn renderFrame(self: *Renderer, state: *const game.GameState) void {
        // Clear screen
        _ = c.SDL_SetRenderDrawColor(self.renderer, 0, 0, 0, 255);
        _ = c.SDL_RenderClear(self.renderer);

        // Render based on game phase
        switch (state.phase) {
            .active => {
                if (state.controls.viewing_camera) |camera_location| {
                    self.renderCameraView(state, camera_location);
                } else {
                    self.renderOfficeView(state);
                }
                self.renderUI(state);
            },
            .game_over_jumpscare => {
                self.renderJumpscare();
            },
            .survived_night => {
                self.renderVictoryScreen(state);
            },
        }

        // Present
        c.SDL_RenderPresent(self.renderer);
    }

    fn renderOfficeView(self: *Renderer, state: *const game.GameState) void {
        // Render office background
        _ = c.SDL_RenderCopy(self.renderer, self.assets.office_texture.ptr, null, null);

        // Render door indicators
        self.renderDoorControls(state);

        // Draw office text
        self.drawTextCentered("OFFICE VIEW", 640, 50, COLOR_WHITE);
    }

    fn renderCameraView(self: *Renderer, state: *const game.GameState, location: game.Location) void {
        // Get camera texture
        const camera_index = @intFromEnum(location);
        if (self.assets.camera_textures[camera_index]) |texture| {
            _ = c.SDL_RenderCopy(self.renderer, texture.ptr, null, null);
        }

        // Draw location name
        const location_name = @tagName(location);
        self.drawTextCentered(location_name, 640, 50, COLOR_GREEN);

        // Draw animatronics at this location
        self.renderAnimatronicsAtLocation(state, location);

        // Camera static effect overlay
        self.renderCameraStatic();
    }

    fn renderAnimatronicsAtLocation(self: *Renderer, state: *const game.GameState, location: game.Location) void {
        var y_offset: i32 = 300;

        for (state.animatronics) |anim| {
            if (anim.current_location == location) {
                const name = @tagName(anim.character_type);
                self.drawText(name, 100, y_offset, COLOR_YELLOW);
                y_offset += 30;
            }
        }
    }

    fn renderDoorControls(self: *Renderer, state: *const game.GameState) void {
        // Left door
        const left_color = if (state.controls.left_door_closed) COLOR_RED else COLOR_GREEN;
        self.drawFilledRect(50, 300, 100, 50, left_color);
        self.drawText("LEFT", 60, 310, COLOR_BLACK);
        self.drawText("DOOR", 60, 330, COLOR_BLACK);

        // Right door
        const right_color = if (state.controls.right_door_closed) COLOR_RED else COLOR_GREEN;
        self.drawFilledRect(1130, 300, 100, 50, right_color);
        self.drawText("RIGHT", 1140, 310, COLOR_BLACK);
        self.drawText("DOOR", 1140, 330, COLOR_BLACK);

        // Left light
        if (state.controls.left_light_on) {
            self.drawFilledRect(50, 370, 100, 30, COLOR_YELLOW);
            self.drawText("LIGHT ON", 55, 378, COLOR_BLACK);
        }

        // Right light
        if (state.controls.right_light_on) {
            self.drawFilledRect(1130, 370, 100, 30, COLOR_YELLOW);
            self.drawText("LIGHT ON", 1135, 378, COLOR_BLACK);
        }
    }

    fn renderCameraStatic(self: *Renderer) void {
        // Simple static effect - random noise rectangles
        var prng = std.Random.DefaultPrng.init(@intCast(std.time.milliTimestamp()));
        const random = prng.random();

        var i: usize = 0;
        while (i < 100) : (i += 1) {
            const x = random.intRangeAtMost(i32, 0, SCREEN_WIDTH);
            const y = random.intRangeAtMost(i32, 0, SCREEN_HEIGHT);
            const w = random.intRangeAtMost(i32, 2, 10);
            const h = random.intRangeAtMost(i32, 2, 10);
            const alpha = random.intRangeAtMost(u8, 50, 150);

            _ = c.SDL_SetRenderDrawBlendMode(self.renderer, c.SDL_BLENDMODE_BLEND);
            _ = c.SDL_SetRenderDrawColor(self.renderer, 255, 255, 255, alpha);
            const rect = c.SDL_Rect{ .x = x, .y = y, .w = w, .h = h };
            _ = c.SDL_RenderFillRect(self.renderer, &rect);
        }
    }

    fn renderUI(self: *Renderer, state: *const game.GameState) void {
        // Power indicator (top right)
        const power_text = std.fmt.allocPrintZ(
            self.allocator,
            "Power: {d:.0}%",
            .{state.power.remaining},
        ) catch "Power: ERR";
        defer self.allocator.free(power_text);

        const power_color = if (state.power.remaining < 20.0) COLOR_RED else COLOR_GREEN;
        self.drawText(power_text, 1050, 20, power_color);

        // Time indicator (top left)
        const hour = state.time.hour;
        const time_text = std.fmt.allocPrintZ(
            self.allocator,
            "{d} AM",
            .{if (hour == 0) 12 else hour},
        ) catch "TIME ERR";
        defer self.allocator.free(time_text);

        self.drawText(time_text, 50, 20, COLOR_WHITE);

        // Night number
        const night_text = std.fmt.allocPrintZ(
            self.allocator,
            "Night {d}",
            .{state.night_number},
        ) catch "Night ?";
        defer self.allocator.free(night_text);

        self.drawText(night_text, 600, 20, COLOR_WHITE);

        // Camera button hint
        if (state.controls.viewing_camera == null) {
            self.drawTextCentered("[TAB] Open Camera", 640, 680, COLOR_WHITE);
        } else {
            self.drawTextCentered("[TAB] Close Camera | [1-9] Switch Camera", 640, 680, COLOR_WHITE);
        }
    }

    fn renderJumpscare(self: *Renderer) void {
        // Red flash effect
        _ = c.SDL_SetRenderDrawColor(self.renderer, 255, 0, 0, 255);
        _ = c.SDL_RenderClear(self.renderer);

        self.drawTextCentered("GAME OVER", 640, 300, COLOR_WHITE);
        self.drawTextCentered("[ESC] Quit | [R] Restart", 640, 360, COLOR_WHITE);
    }

    fn renderVictoryScreen(self: *Renderer, state: *const game.GameState) void {
        _ = c.SDL_SetRenderDrawColor(self.renderer, 0, 100, 0, 255);
        _ = c.SDL_RenderClear(self.renderer);

        const text = std.fmt.allocPrintZ(
            self.allocator,
            "6 AM - NIGHT {d} COMPLETE!",
            .{state.night_number},
        ) catch "NIGHT COMPLETE!";
        defer self.allocator.free(text);

        self.drawTextCentered(text, 640, 300, COLOR_WHITE);
        self.drawTextCentered("[ESC] Quit | [ENTER] Next Night", 640, 360, COLOR_WHITE);
    }

    // ========================================================================
    // Text Rendering Helpers
    // ========================================================================

    fn drawText(self: *Renderer, text: [*c]const u8, x: i32, y: i32, color: c.SDL_Color) void {
        const surface = c.TTF_RenderText_Solid(self.assets.font, text, color) orelse return;
        defer c.SDL_FreeSurface(surface);

        const texture = c.SDL_CreateTextureFromSurface(self.renderer, surface) orelse return;
        defer c.SDL_DestroyTexture(texture);

        const rect = c.SDL_Rect{
            .x = x,
            .y = y,
            .w = surface.*.w,
            .h = surface.*.h,
        };

        _ = c.SDL_RenderCopy(self.renderer, texture, null, &rect);
    }

    fn drawTextCentered(self: *Renderer, text: [*c]const u8, x: i32, y: i32, color: c.SDL_Color) void {
        const surface = c.TTF_RenderText_Solid(self.assets.font, text, color) orelse return;
        defer c.SDL_FreeSurface(surface);

        const texture = c.SDL_CreateTextureFromSurface(self.renderer, surface) orelse return;
        defer c.SDL_DestroyTexture(texture);

        const rect = c.SDL_Rect{
            .x = x - @divTrunc(surface.*.w, 2),
            .y = y - @divTrunc(surface.*.h, 2),
            .w = surface.*.w,
            .h = surface.*.h,
        };

        _ = c.SDL_RenderCopy(self.renderer, texture, null, &rect);
    }

    fn drawFilledRect(self: *Renderer, x: i32, y: i32, w: i32, h: i32, color: c.SDL_Color) void {
        _ = c.SDL_SetRenderDrawColor(self.renderer, color.r, color.g, color.b, color.a);
        const rect = c.SDL_Rect{ .x = x, .y = y, .w = w, .h = h };
        _ = c.SDL_RenderFillRect(self.renderer, &rect);
    }
};

// ============================================================================
// Input Processing (Boundary)
// ============================================================================

pub const InputResult = enum {
    continue_game,
    quit,
    restart,
    next_night,
};

pub fn processInput(state: *game.GameState) InputResult {
    var event: c.SDL_Event = undefined;

    while (c.SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            c.SDL_QUIT => return .quit,

            c.SDL_KEYDOWN => {
                const key = event.key.keysym.sym;

                switch (key) {
                    c.SDLK_ESCAPE => return .quit,

                    c.SDLK_r => {
                        if (state.phase == .game_over_jumpscare) {
                            return .restart;
                        }
                    },

                    c.SDLK_RETURN => {
                        if (state.phase == .survived_night) {
                            return .next_night;
                        }
                    },

                    // Camera toggle
                    c.SDLK_TAB => {
                        if (state.controls.viewing_camera) |_| {
                            game.processPlayerAction(state, .{ .toggle_camera = null });
                        } else {
                            game.processPlayerAction(state, .{ .toggle_camera = .show_stage });
                        }
                    },

                    // Camera switching (1-9)
                    c.SDLK_1 => game.processPlayerAction(state, .{ .toggle_camera = .show_stage }),
                    c.SDLK_2 => game.processPlayerAction(state, .{ .toggle_camera = .dining_area }),
                    c.SDLK_3 => game.processPlayerAction(state, .{ .toggle_camera = .pirate_cove }),
                    c.SDLK_4 => game.processPlayerAction(state, .{ .toggle_camera = .west_hall }),
                    c.SDLK_5 => game.processPlayerAction(state, .{ .toggle_camera = .east_hall }),
                    c.SDLK_6 => game.processPlayerAction(state, .{ .toggle_camera = .supply_closet }),
                    c.SDLK_7 => game.processPlayerAction(state, .{ .toggle_camera = .restrooms }),
                    c.SDLK_8 => game.processPlayerAction(state, .{ .toggle_camera = .kitchen }),

                    // Door controls
                    c.SDLK_a => game.processPlayerAction(state, .toggle_left_door),
                    c.SDLK_d => game.processPlayerAction(state, .toggle_right_door),

                    // Light controls
                    c.SDLK_q => game.processPlayerAction(state, .toggle_left_light),
                    c.SDLK_e => game.processPlayerAction(state, .toggle_right_light),

                    else => {},
                }
            },

            else => {},
        }
    }

    return .continue_game;
}
