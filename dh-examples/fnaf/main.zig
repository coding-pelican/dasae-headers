const std = @import("std");
const game = @import("game.zig");
const renderer = @import("renderer.zig");

// ============================================================================
// Game Application
// ============================================================================

pub const Application = struct {
    game_state: game.GameState,
    renderer: renderer.Renderer,
    allocator: std.mem.Allocator,
    last_frame_time: u64,

    pub fn init(allocator: std.mem.Allocator, night: u8) !Application {
        // Initialize renderer first (sets up SDL)
        const rend = try renderer.Renderer.init(allocator);

        // Initialize game state with timestamp seed
        const seed = @as(u64, @intCast(std.time.milliTimestamp()));
        const state = game.GameState.init(night, seed);

        return Application{
            .game_state = state,
            .renderer = rend,
            .allocator = allocator,
            .last_frame_time = @as(u64, @intCast(std.time.milliTimestamp())),
        };
    }

    pub fn deinit(self: *Application) void {
        self.renderer.deinit();
    }

    pub fn run(self: *Application) !void {
        const stdout = std.io.getStdOut().writer();
        try stdout.print("Starting Night {}...\n", .{self.game_state.night_number});
        try stdout.print("\nControls:\n", .{});
        try stdout.print("  TAB     - Toggle Camera\n", .{});
        try stdout.print("  1-8     - Switch Camera Location\n", .{});
        try stdout.print("  A       - Left Door\n", .{});
        try stdout.print("  D       - Right Door\n", .{});
        try stdout.print("  Q       - Left Light\n", .{});
        try stdout.print("  E       - Right Light\n", .{});
        try stdout.print("  ESC     - Quit\n", .{});
        try stdout.print("  R       - Restart (after game over)\n", .{});
        try stdout.print("  ENTER   - Next Night (after winning)\n\n", .{});

        // Main game loop
        var running = true;
        while (running) {
            const current_time = @as(u64, @intCast(std.time.milliTimestamp()));
            const delta_ms = current_time - self.last_frame_time;
            const delta_seconds = @as(f32, @floatFromInt(delta_ms)) / 1000.0;
            self.last_frame_time = current_time;

            // Process input (boundary function)
            const input_result = renderer.processInput(&self.game_state);

            switch (input_result) {
                .quit => {
                    running = false;
                    break;
                },
                .restart => {
                    try stdout.print("Restarting Night {}...\n", .{self.game_state.night_number});
                    self.restartNight();
                },
                .next_night => {
                    const next = self.game_state.night_number + 1;
                    try stdout.print("Starting Night {}...\n", .{next});
                    self.startNight(next);
                },
                .continue_game => {},
            }

            // Update game logic (pure, no I/O)
            game.updateGame(&self.game_state, delta_seconds);

            // Render (reads game state only)
            self.renderer.renderFrame(&self.game_state);

            // Frame rate limiting
            std.time.sleep(std.time.ns_per_ms * 16); // ~60 FPS
        }

        try stdout.print("\nGame ended.\n", .{});
    }

    fn restartNight(self: *Application) void {
        const seed = @as(u64, @intCast(std.time.milliTimestamp()));
        self.game_state = game.GameState.init(self.game_state.night_number, seed);
    }

    fn startNight(self: *Application, night: u8) void {
        const seed = @as(u64, @intCast(std.time.milliTimestamp()));
        self.game_state = game.GameState.init(night, seed);
    }
};

// ============================================================================
// Entry Point
// ============================================================================

pub fn main() !void {
    // Setup allocator
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    // Print banner
    const stdout = std.io.getStdOut().writer();
    try stdout.print("\n", .{});
    try stdout.print("╔════════════════════════════════════════════╗\n", .{});
    try stdout.print("║  FIVE NIGHTS AT FREDDY'S - ZIG EDITION   ║\n", .{});
    try stdout.print("║          Bare Metal Implementation         ║\n", .{});
    try stdout.print("╚════════════════════════════════════════════╝\n", .{});
    try stdout.print("\n", .{});

    // Get starting night from command line args (default: 1)
    var args = try std.process.argsWithAllocator(allocator);
    defer args.deinit();

    _ = args.skip(); // Skip program name

    const night: u8 = if (args.next()) |arg|
        std.fmt.parseInt(u8, arg, 10) catch 1
    else
        1;

    if (night > 20) {
        try stdout.print("Night must be between 1-20\n", .{});
        return;
    }

    // Create and run application
    var app = Application.init(allocator, night) catch |err| {
        try stdout.print("Failed to initialize application: {}\n", .{err});
        try stdout.print("\nMake sure SDL2 and SDL2_ttf are installed:\n", .{});
        try stdout.print("  Ubuntu/Debian: sudo apt install libsdl2-dev libsdl2-ttf-dev\n", .{});
        try stdout.print("  Fedora: sudo dnf install SDL2-devel SDL2_ttf-devel\n", .{});
        try stdout.print("  Arch: sudo pacman -S sdl2 sdl2_ttf\n", .{});
        try stdout.print("  macOS: brew install sdl2 sdl2_ttf\n", .{});
        return err;
    };
    defer app.deinit();

    try app.run();
}
