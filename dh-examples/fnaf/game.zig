const std = @import("std");

// ============================================================================
// Core Data Definitions
// ============================================================================

/// Room/Camera locations in the pizzeria
pub const Location = enum(u8) {
    office, // Player's office
    show_stage, // Main stage
    dining_area,
    pirate_cove,
    west_hall,
    west_hall_corner,
    east_hall,
    east_hall_corner,
    supply_closet,
    restrooms,
    kitchen,

    pub fn canViewFromCamera(self: Location) bool {
        return self != .office;
    }
};

/// Animatronic character types
pub const AnimatronicType = enum(u8) {
    freddy,
    bonnie,
    chica,
    foxy,
};

/// Door/Light controls
pub const DoorSide = enum(u8) {
    left,
    right,
};

/// Game phase within a night
pub const GamePhase = enum(u8) {
    active,
    game_over_jumpscare,
    survived_night,
};

/// Time representation (0-6 representing 12AM-6AM)
pub const GameTime = struct {
    hour: u8, // 0=12AM, 1=1AM, ..., 6=6AM
    progress: f32, // 0.0 to 1.0 within current hour

    pub fn isNightOver(self: GameTime) bool {
        return self.hour >= 6;
    }

    pub fn advance(self: *GameTime, delta_seconds: f32) void {
        // Each hour is roughly 90 seconds (540 seconds total for 6 hours)
        const seconds_per_hour: f32 = 90.0;
        self.progress += delta_seconds / seconds_per_hour;

        while (self.progress >= 1.0) {
            self.progress -= 1.0;
            self.hour += 1;
        }
    }
};

/// Power system state
pub const PowerState = struct {
    remaining: f32, // 0.0 to 100.0
    usage_rate: f32, // Percentage per second

    pub fn init() PowerState {
        return .{
            .remaining = 100.0,
            .usage_rate = 0.0,
        };
    }

    pub fn update(self: *PowerState, delta_seconds: f32) void {
        self.remaining -= self.usage_rate * delta_seconds;
        if (self.remaining < 0.0) {
            self.remaining = 0.0;
        }
    }

    pub fn isDepleted(self: PowerState) bool {
        return self.remaining <= 0.0;
    }
};

/// Individual animatronic state
pub const AnimatronicState = struct {
    character_type: AnimatronicType,
    current_location: Location,
    ai_level: u8, // 0-20, affects movement frequency
    move_timer: f32, // Time until next movement opportunity
    is_at_door: bool, // Special flag for door proximity

    pub fn init(char_type: AnimatronicType, ai_level: u8) AnimatronicState {
        const start_location: Location = switch (char_type) {
            .freddy, .bonnie, .chica => .show_stage,
            .foxy => .pirate_cove,
        };

        return .{
            .character_type = char_type,
            .current_location = start_location,
            .ai_level = ai_level,
            .move_timer = 0.0,
            .is_at_door = false,
        };
    }
};

/// Player control state
pub const PlayerControls = struct {
    viewing_camera: ?Location, // null when viewing office
    left_door_closed: bool,
    right_door_closed: bool,
    left_light_on: bool,
    right_light_on: bool,

    pub fn init() PlayerControls {
        return .{
            .viewing_camera = null,
            .left_door_closed = false,
            .right_door_closed = false,
            .left_light_on = false,
            .right_light_on = false,
        };
    }

    pub fn isViewingCamera(self: PlayerControls) bool {
        return self.viewing_camera != null;
    }

    /// Calculate current power usage based on active systems
    pub fn calculatePowerUsage(self: PlayerControls) f32 {
        var usage: f32 = 0.1; // Base usage

        if (self.isViewingCamera()) {
            usage += 0.1; // Camera system
        }

        if (self.left_door_closed) usage += 0.1;
        if (self.right_door_closed) usage += 0.1;
        if (self.left_light_on) usage += 0.1;
        if (self.right_light_on) usage += 0.1;

        return usage;
    }
};

// ============================================================================
// Central Game State (Database)
// ============================================================================

pub const GameState = struct {
    phase: GamePhase,
    time: GameTime,
    power: PowerState,
    controls: PlayerControls,
    animatronics: [4]AnimatronicState,
    night_number: u8,
    random_state: u64, // Simple RNG state

    pub fn init(night: u8, seed: u64) GameState {
        // AI levels increase with night number
        const base_ai = night * 2;

        return .{
            .phase = .active,
            .time = .{ .hour = 0, .progress = 0.0 },
            .power = PowerState.init(),
            .controls = PlayerControls.init(),
            .animatronics = [4]AnimatronicState{
                AnimatronicState.init(.freddy, @min(base_ai, 20)),
                AnimatronicState.init(.bonnie, @min(base_ai + 1, 20)),
                AnimatronicState.init(.chica, @min(base_ai + 1, 20)),
                AnimatronicState.init(.foxy, @min(base_ai, 20)),
            },
            .night_number = night,
            .random_state = seed,
        };
    }

    /// Simple LCG random number generator
    fn nextRandom(self: *GameState) u32 {
        self.random_state = self.random_state *% 6364136223846793005 +% 1442695040888963407;
        return @truncate(self.random_state >> 32);
    }

    fn randomFloat(self: *GameState) f32 {
        return @as(f32, @floatFromInt(self.nextRandom() % 10000)) / 10000.0;
    }

    fn randomRange(self: *GameState, min: u32, max: u32) u32 {
        if (min >= max) return min;
        return min + (self.nextRandom() % (max - min + 1));
    }
};

// ============================================================================
// Game Logic Systems
// ============================================================================

/// Update game state by delta time
pub fn updateGame(state: *GameState, delta_seconds: f32) void {
    if (state.phase != .active) {
        return; // Game over or won, no updates
    }

    // Update time
    state.time.advance(delta_seconds);

    // Check for night completion
    if (state.time.isNightOver()) {
        state.phase = .survived_night;
        return;
    }

    // Update power
    state.power.usage_rate = state.controls.calculatePowerUsage();
    state.power.update(delta_seconds);

    // Power outage leads to Freddy's attack
    if (state.power.isDepleted()) {
        state.phase = .game_over_jumpscare;
        return;
    }

    // Update animatronics
    for (&state.animatronics) |*anim| {
        updateAnimatronic(state, anim, delta_seconds);
    }

    // Check for jumpscare conditions
    if (checkJumpscareConditions(state)) {
        state.phase = .game_over_jumpscare;
        return;
    }
}

/// Update individual animatronic AI
fn updateAnimatronic(state: *GameState, anim: *AnimatronicState, delta_seconds: f32) void {
    // Decrease move timer
    anim.move_timer -= delta_seconds;

    if (anim.move_timer <= 0.0) {
        // Check if animatronic attempts to move
        const move_chance = @as(f32, @floatFromInt(anim.ai_level)) / 20.0;
        const roll = state.randomFloat();

        if (roll < move_chance) {
            attemptMove(state, anim);
        }

        // Reset timer (3-5 seconds between movement opportunities)
        const min_time: u32 = 3;
        const max_time: u32 = 5;
        anim.move_timer = @as(f32, @floatFromInt(state.randomRange(min_time, max_time)));
    }
}

/// Attempt to move animatronic to next location
fn attemptMove(state: *GameState, anim: *AnimatronicState) void {
    const next_loc = getNextLocation(state, anim);

    // Bonnie goes left, Chica goes right (generally)
    if (next_loc) |loc| {
        // Check if blocked by camera (being watched reduces movement)
        if (state.controls.viewing_camera == anim.current_location) {
            const block_chance = 0.7; // 70% chance to be deterred
            if (state.randomFloat() < block_chance) {
                return; // Movement blocked
            }
        }

        anim.current_location = loc;

        // Check if arrived at door
        anim.is_at_door = switch (anim.character_type) {
            .bonnie => loc == .west_hall_corner,
            .chica => loc == .east_hall_corner,
            .freddy => loc == .east_hall_corner,
            .foxy => false, // Foxy has different mechanic
        };
    }
}

/// Determine next location for animatronic movement
fn getNextLocation(state: *GameState, anim: *AnimatronicState) ?Location {
    const current = anim.current_location;

    return switch (anim.character_type) {
        .bonnie => switch (current) {
            .show_stage => .dining_area,
            .dining_area => if (state.randomFloat() < 0.5) .west_hall else .supply_closet,
            .supply_closet => .west_hall,
            .west_hall => .west_hall_corner,
            .west_hall_corner => null, // At door
            else => null,
        },
        .chica => switch (current) {
            .show_stage => .dining_area,
            .dining_area => if (state.randomFloat() < 0.5) .east_hall else .restrooms,
            .restrooms => .east_hall,
            .east_hall => .east_hall_corner,
            .east_hall_corner => null, // At door
            else => null,
        },
        .freddy => switch (current) {
            .show_stage => .dining_area,
            .dining_area => .restrooms,
            .restrooms => .kitchen,
            .kitchen => .east_hall,
            .east_hall => .east_hall_corner,
            .east_hall_corner => null, // At door
            else => null,
        },
        .foxy => switch (current) {
            .pirate_cove => .west_hall,
            .west_hall => .west_hall_corner,
            .west_hall_corner => null, // At door
            else => null,
        },
    };
}

/// Check if any animatronic can perform jumpscare
fn checkJumpscareConditions(state: *GameState) bool {
    for (state.animatronics) |anim| {
        if (!anim.is_at_door) continue;

        const door_closed = switch (anim.character_type) {
            .bonnie, .foxy => state.controls.left_door_closed,
            .chica, .freddy => state.controls.right_door_closed,
        };

        // If at door and door is not closed, jumpscare occurs
        if (!door_closed) {
            // Additional check: if player is viewing camera, delay attack slightly
            if (!state.controls.isViewingCamera()) {
                return true;
            }
        }
    }

    return false;
}

// ============================================================================
// Player Input Interface (Boundary)
// ============================================================================

pub const PlayerAction = union(enum) {
    toggle_camera: ?Location, // null to close camera
    toggle_left_door,
    toggle_right_door,
    toggle_left_light,
    toggle_right_light,
};

/// Process player input (boundary function)
pub fn processPlayerAction(state: *GameState, action: PlayerAction) void {
    if (state.phase != .active) return;

    switch (action) {
        .toggle_camera => |location| {
            state.controls.viewing_camera = location;
        },
        .toggle_left_door => {
            state.controls.left_door_closed = !state.controls.left_door_closed;
        },
        .toggle_right_door => {
            state.controls.right_door_closed = !state.controls.right_door_closed;
        },
        .toggle_left_light => {
            state.controls.left_light_on = !state.controls.left_light_on;
        },
        .toggle_right_light => {
            state.controls.right_light_on = !state.controls.right_light_on;
        },
    }
}

// ============================================================================
// Query Functions (For Rendering/UI)
// ============================================================================

/// Get animatronic at specific location
pub fn getAnimatronicsAtLocation(state: *const GameState, location: Location) []const AnimatronicType {
    var buffer: [4]AnimatronicType = undefined;
    var count: usize = 0;

    for (state.animatronics) |anim| {
        if (anim.current_location == location) {
            buffer[count] = anim.character_type;
            count += 1;
        }
    }

    return buffer[0..count];
}

/// Check if animatronic is visible at door
pub fn isAnimatronicAtDoor(state: *const GameState, side: DoorSide) ?AnimatronicType {
    const target_location: Location = switch (side) {
        .left => .west_hall_corner,
        .right => .east_hall_corner,
    };

    for (state.animatronics) |anim| {
        if (anim.is_at_door and anim.current_location == target_location) {
            return anim.character_type;
        }
    }

    return null;
}

/// Get current power percentage (for UI display)
pub fn getPowerPercentage(state: *const GameState) f32 {
    return state.power.remaining;
}

/// Get current hour (0-6)
pub fn getCurrentHour(state: *const GameState) u8 {
    return state.time.hour;
}

// ============================================================================
// Module exports for external use
// ============================================================================
// This module provides pure game logic without rendering or I/O
