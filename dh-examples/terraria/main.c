const std = @import("std");

// ============================================================================
// CORE DATA DEFINITIONS
// ============================================================================

const Vec2 = struct {
    x: f32,
    y: f32,

    pub fn add(self: Vec2, other: Vec2) Vec2 {
        return .{ .x = self.x + other.x, .y = self.y + other.y };
    }

    pub fn scale(self: Vec2, scalar: f32) Vec2 {
        return .{ .x = self.x * scalar, .y = self.y * scalar };
    }

    pub fn zero() Vec2 {
        return .{ .x = 0, .y = 0 };
    }
};

const Vec2i = struct {
    x: i32,
    y: i32,
};

// Background tiles (walls, decorative)
const BackgroundTile = enum(u8) {
    none = 0,
    stone_wall = 1,
    dirt_wall = 2,
    wood_wall = 3,
    glass_wall = 4,
};

// Foreground block types (solid terrain)
const BlockTile = enum(u8) {
    air = 0,
    stone = 1,
    dirt = 2,
    grass = 3,
    wood = 4,
    leaves = 5,
    sand = 6,
    coal_ore = 7,
    iron_ore = 8,
    gold_ore = 9,
    diamond_ore = 10,
    bedrock = 11,
};

// Entity tiles (furniture, interactive objects)
const EntityTileType = enum(u16) {
    none = 0,
    // Passable (can walk through, can stand on)
    platform = 1,
    crafting_table = 2,
    chair = 3,
    table = 4,
    // Impenetrable (solid collision)
    chest = 10,
    furnace = 11,
    anvil = 12,
    // Interactive impenetrable
    door_closed = 20,
    door_open = 21,
    // Lighting
    torch = 30,
    lantern = 31,
};

const EntityTile = struct {
    tile_type: EntityTileType,
    origin: Vec2i, // For multi-tile objects, points to top-left tile
    is_origin: bool, // True if this is the origin tile
    metadata: u32, // Generic storage for tile-specific data
};

const TileProperties = struct {
    size: Vec2i, // Width x Height in tiles
    solid: bool, // Can entities pass through?
    platform: bool, // Can stand on top but pass through from below?
    interactive: bool, // Can be interacted with?
    light_level: u8, // Light emission (0-15)
};

const ItemType = enum(u16) {
    none = 0,
    // Blocks
    stone = 1,
    dirt = 2,
    grass = 3,
    wood = 4,
    sand = 5,
    // Furniture
    platform = 50,
    crafting_table = 51,
    chair = 52,
    table = 53,
    chest = 54,
    door = 55,
    torch = 56,
    // Tools
    wooden_pickaxe = 100,
    stone_pickaxe = 101,
    iron_pickaxe = 102,
    diamond_pickaxe = 103,
    wooden_axe = 110,
    wooden_hammer = 120,
    // Resources
    coal = 200,
    iron_ore = 201,
    gold_ore = 202,
    diamond = 203,
};

const ItemStack = struct {
    item_type: ItemType,
    count: u16,
};

const EntityType = enum(u8) {
    player,
    zombie,
    skeleton,
    item_drop,
};

const Entity = struct {
    id: u32,
    entity_type: EntityType,
    position: Vec2,
    velocity: Vec2,
    size: Vec2, // Size in world units (tiles)
    health: i16,
    max_health: i16,
    alive: bool,
    grounded: bool,
    on_platform: bool, // Standing on a platform

    // Entity-specific data
    item_stack: ItemStack, // For item_drop entities
};

const WORLD_WIDTH = 500;
const WORLD_HEIGHT = 200;
const INVENTORY_SIZE = 40;
const HOTBAR_SIZE = 10;
const MAX_ENTITIES = 1000;

// ============================================================================
// CENTRALIZED GAME DATABASE
// ============================================================================

const GameDatabase = struct {
    // World layers
    background_tiles: [WORLD_HEIGHT][WORLD_WIDTH]BackgroundTile,
    block_tiles: [WORLD_HEIGHT][WORLD_WIDTH]BlockTile,
    entity_tiles: [WORLD_HEIGHT][WORLD_WIDTH]EntityTile,
    block_health: [WORLD_HEIGHT][WORLD_WIDTH]u8, // Mining progress

    // Player data
    player_entity_id: u32,
    inventory: [INVENTORY_SIZE]ItemStack,
    selected_hotbar_slot: u8,

    // Entity system
    entities: [MAX_ENTITIES]Entity,
    entity_count: u32,
    next_entity_id: u32,

    // World state
    time_of_day: f32, // 0.0 to 1.0
    tick_count: u64,

    // Input state
    input: InputState,

    allocator: std.mem.Allocator,
};

const InputState = struct {
    move_left: bool,
    move_right: bool,
    jump: bool,
    down: bool, // For dropping through platforms
    mine: bool,
    place: bool,
    interact: bool,
    cursor_world_pos: Vec2,
    hotbar_selection: ?u8,
};

// ============================================================================
// PHYSICS CONSTANTS
// ============================================================================

const GRAVITY = 25.0;
const PLAYER_SPEED = 7.0;
const JUMP_VELOCITY = 12.0;
const TERMINAL_VELOCITY = 30.0;

// ============================================================================
// TILE PROPERTIES LOOKUP
// ============================================================================

fn getTileProperties(tile_type: EntityTileType) TileProperties {
    return switch (tile_type) {
        .none => .{ .size = .{ .x = 1, .y = 1 }, .solid = false, .platform = false, .interactive = false, .light_level = 0 },
        // Passable platforms
        .platform => .{ .size = .{ .x = 1, .y = 1 }, .solid = false, .platform = true, .interactive = false, .light_level = 0 },
        .crafting_table => .{ .size = .{ .x = 2, .y = 1 }, .solid = false, .platform = true, .interactive = true, .light_level = 0 },
        .chair => .{ .size = .{ .x = 1, .y = 2 }, .solid = false, .platform = true, .interactive = false, .light_level = 0 },
        .table => .{ .size = .{ .x = 3, .y = 2 }, .solid = false, .platform = true, .interactive = false, .light_level = 0 },
        // Solid objects
        .chest => .{ .size = .{ .x = 2, .y = 2 }, .solid = true, .platform = false, .interactive = true, .light_level = 0 },
        .furnace => .{ .size = .{ .x = 2, .y = 2 }, .solid = true, .platform = false, .interactive = true, .light_level = 3 },
        .anvil => .{ .size = .{ .x = 2, .y = 1 }, .solid = true, .platform = false, .interactive = true, .light_level = 0 },
        // Doors
        .door_closed => .{ .size = .{ .x = 1, .y = 3 }, .solid = true, .platform = false, .interactive = true, .light_level = 0 },
        .door_open => .{ .size = .{ .x = 2, .y = 3 }, .solid = false, .platform = false, .interactive = true, .light_level = 0 },
        // Lighting
        .torch => .{ .size = .{ .x = 1, .y = 1 }, .solid = false, .platform = false, .interactive = false, .light_level = 10 },
        .lantern => .{ .size = .{ .x = 1, .y = 1 }, .solid = false, .platform = false, .interactive = false, .light_level = 12 },
    };
}

// ============================================================================
// INITIALIZATION
// ============================================================================

pub fn createGameDatabase(allocator: std.mem.Allocator) !*GameDatabase {
    var db = try allocator.create(GameDatabase);

    db.* = GameDatabase{
        .background_tiles = std.mem.zeroes([WORLD_HEIGHT][WORLD_WIDTH]BackgroundTile),
        .block_tiles = undefined,
        .entity_tiles = std.mem.zeroes([WORLD_HEIGHT][WORLD_WIDTH]EntityTile),
        .block_health = std.mem.zeroes([WORLD_HEIGHT][WORLD_WIDTH]u8),
        .player_entity_id = 0,
        .inventory = std.mem.zeroes([INVENTORY_SIZE]ItemStack),
        .selected_hotbar_slot = 0,
        .entities = undefined,
        .entity_count = 0,
        .next_entity_id = 0,
        .time_of_day = 0.25, // Start at morning
        .tick_count = 0,
        .input = std.mem.zeroes(InputState),
        .allocator = allocator,
    };

    // Initialize empty world
    for (&db.block_tiles) |*row| {
        for (row) |*block| {
            block.* = .air;
        }
    }

    // Generate world
    generateWorld(db);

    // Create player entity
    db.player_entity_id = createEntity(db, .{
        .id = 0,
        .entity_type = .player,
        .position = .{ .x = @as(f32, @floatFromInt(WORLD_WIDTH)) / 2.0, .y = 50.0 },
        .velocity = Vec2.zero(),
        .size = .{ .x = 0.8, .y = 1.8 }, // Slightly less than 1x2 tiles
        .health = 100,
        .max_health = 100,
        .alive = true,
        .grounded = false,
        .on_platform = false,
        .item_stack = .{ .item_type = .none, .count = 0 },
    });

    // Give player starting items
    db.inventory[0] = .{ .item_type = .wooden_pickaxe, .count = 1 };
    db.inventory[1] = .{ .item_type = .wooden_hammer, .count = 1 };
    db.inventory[2] = .{ .item_type = .wooden_axe, .count = 1 };
    db.inventory[3] = .{ .item_type = .platform, .count = 50 };
    db.inventory[4] = .{ .item_type = .wood, .count = 100 };
    db.inventory[5] = .{ .item_type = .torch, .count = 20 };
    db.inventory[6] = .{ .item_type = .door, .count = 3 };
    db.inventory[7] = .{ .item_type = .crafting_table, .count = 1 };
    db.inventory[8] = .{ .item_type = .chest, .count = 2 };

    return db;
}

fn generateWorld(db: *GameDatabase) void {
    const surface_height = WORLD_HEIGHT / 2;

    // Generate terrain
    var x: usize = 0;
    while (x < WORLD_WIDTH) : (x += 1) {
        const height_variation = @sin(@as(f32, @floatFromInt(x)) * 0.05) * 10.0;
        const height = surface_height + @as(i32, @intFromFloat(height_variation));

        var y: usize = 0;
        while (y < WORLD_HEIGHT) : (y += 1) {
            const iy = @as(i32, @intCast(y));

            // Generate block layer
            if (iy > height + 30) {
                db.block_tiles[y][x] = .bedrock;
            } else if (iy > height + 15) {
                const rand = hash2D(x, y) % 100;
                if (rand < 3) {
                    db.block_tiles[y][x] = .diamond_ore;
                } else if (rand < 10) {
                    db.block_tiles[y][x] = .gold_ore;
                } else {
                    db.block_tiles[y][x] = .stone;
                }
                // Add stone wall background
                if (db.background_tiles[y][x] == .none) {
                    db.background_tiles[y][x] = .stone_wall;
                }
            } else if (iy > height + 5) {
                const rand = hash2D(x, y) % 100;
                if (rand < 8) {
                    db.block_tiles[y][x] = .iron_ore;
                } else if (rand < 15) {
                    db.block_tiles[y][x] = .coal_ore;
                } else {
                    db.block_tiles[y][x] = .stone;
                }
                // Add dirt wall background near surface
                if (iy > height + 10 and db.background_tiles[y][x] == .none) {
                    db.background_tiles[y][x] = .dirt_wall;
                }
            } else if (iy > height) {
                db.block_tiles[y][x] = .dirt;
            } else if (iy == height) {
                db.block_tiles[y][x] = .grass;
            }
        }

        // Add trees
        if (x % 7 == 3 and x > 2 and x < WORLD_WIDTH - 2) {
            const tree_height = 5 + (hash2D(x, 0) % 3);
            const base_y = @as(usize, @intCast(height - 1));

            if (base_y > tree_height + 3) {
                // Trunk
                var ty: usize = 1;
                while (ty <= tree_height) : (ty += 1) {
                    db.block_tiles[base_y - ty][x] = .wood;
                }

                // Leaves
                const leaf_start = base_y - tree_height;
                if (leaf_start > 2) {
                    for ([_]i32{ -2, -1, 0, 1, 2 }) |dx| {
                        for ([_]i32{ -1, 0, 1, 2 }) |dy| {
                            const lx = @as(i32, @intCast(x)) + dx;
                            const ly = @as(i32, @intCast(leaf_start)) + dy;

                            if (lx >= 0 and lx < WORLD_WIDTH and ly >= 0 and ly < WORLD_HEIGHT) {
                                const ulx = @as(usize, @intCast(lx));
                                const uly = @as(usize, @intCast(ly));

                                if (db.block_tiles[uly][ulx] == .air) {
                                    db.block_tiles[uly][ulx] = .leaves;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

fn hash2D(x: usize, y: usize) u32 {
    var h = @as(u32, @intCast(x)) *% 374761393 +% @as(u32, @intCast(y)) *% 668265263;
    h = (h ^% (h >> 13)) *% 1274126177;
    return h ^% (h >> 16);
}

// ============================================================================
// ENTITY TILE MANAGEMENT
// ============================================================================

fn placeEntityTile(db: *GameDatabase, x: i32, y: i32, tile_type: EntityTileType) bool {
    const props = getTileProperties(tile_type);

    // Check if space is available
    var dy: i32 = 0;
    while (dy < props.size.y) : (dy += 1) {
        var dx: i32 = 0;
        while (dx < props.size.x) : (dx += 1) {
            const tx = x + dx;
            const ty = y + dy;

            if (tx < 0 or tx >= WORLD_WIDTH or ty < 0 or ty >= WORLD_HEIGHT) return false;

            const utx = @as(usize, @intCast(tx));
            const uty = @as(usize, @intCast(ty));

            // Check if blocked by terrain or other entity tiles
            if (db.block_tiles[uty][utx] != .air) return false;
            if (db.entity_tiles[uty][utx].tile_type != .none) return false;
        }
    }

    // Place the multi-tile object
    dy = 0;
    while (dy < props.size.y) : (dy += 1) {
        var dx: i32 = 0;
        while (dx < props.size.x) : (dx += 1) {
            const tx = x + dx;
            const ty = y + dy;
            const utx = @as(usize, @intCast(tx));
            const uty = @as(usize, @intCast(ty));

            db.entity_tiles[uty][utx] = .{
                .tile_type = tile_type,
                .origin = .{ .x = x, .y = y },
                .is_origin = (dx == 0 and dy == 0),
                .metadata = 0,
            };
        }
    }

    return true;
}

fn removeEntityTile(db: *GameDatabase, x: i32, y: i32) ?EntityTileType {
    if (x < 0 or x >= WORLD_WIDTH or y < 0 or y >= WORLD_HEIGHT) return null;

    const ux = @as(usize, @intCast(x));
    const uy = @as(usize, @intCast(y));

    const tile = db.entity_tiles[uy][ux];
    if (tile.tile_type == .none) return null;

    // Find the origin and remove all parts
    const origin = tile.origin;
    const props = getTileProperties(tile.tile_type);
    const removed_type = tile.tile_type;

    var dy: i32 = 0;
    while (dy < props.size.y) : (dy += 1) {
        var dx: i32 = 0;
        while (dx < props.size.x) : (dx += 1) {
            const tx = origin.x + dx;
            const ty = origin.y + dy;

            if (tx >= 0 and tx < WORLD_WIDTH and ty >= 0 and ty < WORLD_HEIGHT) {
                const utx = @as(usize, @intCast(tx));
                const uty = @as(usize, @intCast(ty));
                db.entity_tiles[uty][utx] = std.mem.zeroes(EntityTile);
            }
        }
    }

    return removed_type;
}

// ============================================================================
// ENTITY MANAGEMENT
// ============================================================================

fn createEntity(db: *GameDatabase, entity: Entity) u32 {
    if (db.entity_count >= MAX_ENTITIES) return 0;

    const id = db.next_entity_id;
    db.next_entity_id += 1;

    var new_entity = entity;
    new_entity.id = id;

    db.entities[db.entity_count] = new_entity;
    db.entity_count += 1;

    return id;
}

fn getEntityById(db: *GameDatabase, id: u32) ?*Entity {
    var i: usize = 0;
    while (i < db.entity_count) : (i += 1) {
        if (db.entities[i].id == id) {
            return &db.entities[i];
        }
    }
    return null;
}

fn removeDeadEntities(db: *GameDatabase) void {
    var write_idx: usize = 0;
    var read_idx: usize = 0;

    while (read_idx < db.entity_count) : (read_idx += 1) {
        if (db.entities[read_idx].alive) {
            if (write_idx != read_idx) {
                db.entities[write_idx] = db.entities[read_idx];
            }
            write_idx += 1;
        }
    }

    db.entity_count = @intCast(write_idx);
}

// ============================================================================
// GAME LOGIC UPDATE
// ============================================================================

pub fn updateGame(db: *GameDatabase, delta_time: f32) void {
    // Update time
    db.time_of_day += delta_time * 0.01;
    if (db.time_of_day > 1.0) db.time_of_day -= 1.0;
    db.tick_count += 1;

    // Process input
    processInput(db);

    // Update physics for all entities
    updatePhysics(db, delta_time);

    // Update entity AI
    updateEntityAI(db, delta_time);

    // Clean up dead entities
    removeDeadEntities(db);
}

fn processInput(db: *GameDatabase) void {
    const player = getEntityById(db, db.player_entity_id) orelse return;

    // Movement
    if (db.input.move_left) {
        player.velocity.x = -PLAYER_SPEED;
    } else if (db.input.move_right) {
        player.velocity.x = PLAYER_SPEED;
    } else {
        player.velocity.x *= 0.8;
    }

    // Jumping
    if (db.input.jump and (player.grounded or player.on_platform)) {
        player.velocity.y = -JUMP_VELOCITY;
    }

    // Drop through platforms
    if (db.input.down and player.on_platform) {
        player.position.y += 0.1;
        player.on_platform = false;
    }

    // Mining/interaction
    if (db.input.mine) {
        handleMining(db);
    }

    // Placing
    if (db.input.place) {
        handleBlockPlacement(db);
    }

    // Interaction
    if (db.input.interact) {
        handleInteraction(db);
    }

    // Hotbar selection
    if (db.input.hotbar_selection) |slot| {
        if (slot < HOTBAR_SIZE) {
            db.selected_hotbar_slot = slot;
        }
    }
}

fn handleMining(db: *GameDatabase) void {
    const cursor_pos = db.input.cursor_world_pos;
    const block_x = @as(i32, @intFromFloat(@floor(cursor_pos.x)));
    const block_y = @as(i32, @intFromFloat(@floor(cursor_pos.y)));

    if (block_x < 0 or block_x >= WORLD_WIDTH or block_y < 0 or block_y >= WORLD_HEIGHT) return;

    const ux = @as(usize, @intCast(block_x));
    const uy = @as(usize, @intCast(block_y));

    const player = getEntityById(db, db.player_entity_id) orelse return;

    // Check distance
    const dx = cursor_pos.x - player.position.x;
    const dy = cursor_pos.y - player.position.y;
    const distance = @sqrt(dx * dx + dy * dy);
    if (distance > 5.0) return;

    const selected_item = db.inventory[db.selected_hotbar_slot];
    const is_hammer = selected_item.item_type == .wooden_hammer;

    // Check what layer to mine
    if (is_hammer) {
        // Hammer removes entity tiles and background
        if (db.entity_tiles[uy][ux].tile_type != .none) {
            const removed = removeEntityTile(db, block_x, block_y);
            if (removed) |tile_type| {
                const item_type = entityTileToItem(tile_type);
                if (item_type != .none) {
                    createItemDrop(db, cursor_pos, item_type, 1);
                }
            }
        } else if (db.background_tiles[uy][ux] != .none) {
            db.background_tiles[uy][ux] = .none;
        }
    } else {
        // Pickaxe/axe removes blocks
        const block = db.block_tiles[uy][ux];
        if (block == .air or block == .bedrock) return;

        const mining_power = getMiningPower(selected_item.item_type);
        db.block_health[uy][ux] += mining_power;

        const block_hardness = getBlockHardness(block);
        if (db.block_health[uy][ux] >= block_hardness) {
            const item_type = blockToItem(block);
            if (item_type != .none) {
                createItemDrop(db, cursor_pos, item_type, 1);
            }

            db.block_tiles[uy][ux] = .air;
            db.block_health[uy][ux] = 0;
        }
    }
}

fn handleBlockPlacement(db: *GameDatabase) void {
    const cursor_pos = db.input.cursor_world_pos;
    const block_x = @as(i32, @intFromFloat(@floor(cursor_pos.x)));
    const block_y = @as(i32, @intFromFloat(@floor(cursor_pos.y)));

    if (block_x < 0 or block_x >= WORLD_WIDTH or block_y < 0 or block_y >= WORLD_HEIGHT) return;

    const player = getEntityById(db, db.player_entity_id) orelse return;

    // Check distance
    const dx = cursor_pos.x - player.position.x;
    const dy = cursor_pos.y - player.position.y;
    const distance = @sqrt(dx * dx + dy * dy);
    if (distance > 5.0) return;

    // Get selected item
    const selected_item = &db.inventory[db.selected_hotbar_slot];
    if (selected_item.count == 0) return;

    // Try to place as entity tile first
    const entity_tile = itemToEntityTile(selected_item.item_type);
    if (entity_tile != .none) {
        if (placeEntityTile(db, block_x, block_y, entity_tile)) {
            selected_item.count -= 1;
            if (selected_item.count == 0) {
                selected_item.item_type = .none;
            }
        }
    } else {
        // Try to place as block
        const ux = @as(usize, @intCast(block_x));
        const uy = @as(usize, @intCast(block_y));

        if (db.block_tiles[uy][ux] != .air) return;
        if (db.entity_tiles[uy][ux].tile_type != .none) return;

        const block_type = itemToBlock(selected_item.item_type);
        if (block_type != .air) {
            db.block_tiles[uy][ux] = block_type;
            selected_item.count -= 1;
            if (selected_item.count == 0) {
                selected_item.item_type = .none;
            }
        }
    }
}

fn handleInteraction(db: *GameDatabase) void {
    const cursor_pos = db.input.cursor_world_pos;
    const block_x = @as(i32, @intFromFloat(@floor(cursor_pos.x)));
    const block_y = @as(i32, @intFromFloat(@floor(cursor_pos.y)));

    if (block_x < 0 or block_x >= WORLD_WIDTH or block_y < 0 or block_y >= WORLD_HEIGHT) return;

    const ux = @as(usize, @intCast(block_x));
    const uy = @as(usize, @intCast(block_y));

    const tile = &db.entity_tiles[uy][ux];
    if (tile.tile_type == .none) return;

    // Find origin tile for interaction
    const origin_tile = &db.entity_tiles[@intCast(tile.origin.y)][@intCast(tile.origin.x)];

    switch (tile.tile_type) {
        .door_closed => {
            // Open door - change type and update all parts
            const props = getTileProperties(.door_open);
            removeEntityTile(db, tile.origin.x, tile.origin.y);
            _ = placeEntityTile(db, tile.origin.x, tile.origin.y, .door_open);
        },
        .door_open => {
            // Close door
            removeEntityTile(db, tile.origin.x, tile.origin.y);
            _ = placeEntityTile(db, tile.origin.x, tile.origin.y, .door_closed);
        },
        .chest => {
            // Would open chest inventory
            origin_tile.metadata ^= 1; // Toggle open state
        },
        .crafting_table => {
            // Would open crafting interface
        },
        else => {},
    }
}

fn createItemDrop(db: *GameDatabase, pos: Vec2, item_type: ItemType, count: u16) void {
    _ = createEntity(db, .{
        .id = 0,
        .entity_type = .item_drop,
        .position = pos,
        .velocity = .{
            .x = @as(f32, @floatFromInt(hash2D(@intFromFloat(pos.x), @intFromFloat(pos.y)) % 100 - 50)) * 0.01,
            .y = -2.0,
        },
        .size = .{ .x = 0.3, .y = 0.3 },
        .health = 1,
        .max_health = 1,
        .alive = true,
        .grounded = false,
        .on_platform = false,
        .item_stack = .{ .item_type = item_type, .count = count },
    });
}

// ============================================================================
// PHYSICS SYSTEM
// ============================================================================

fn updatePhysics(db: *GameDatabase, delta_time: f32) void {
    var i: usize = 0;
    while (i < db.entity_count) : (i += 1) {
        var entity = &db.entities[i];
        if (!entity.alive) continue;

        // Apply gravity
        entity.velocity.y += GRAVITY * delta_time;
        if (entity.velocity.y > TERMINAL_VELOCITY) {
            entity.velocity.y = TERMINAL_VELOCITY;
        }

        // Move and collide
        moveWithCollision(db, entity, delta_time);

        // Pick up items (player only)
        if (entity.entity_type == .player) {
            pickUpItems(db, entity);
        }
    }
}

fn moveWithCollision(db: *GameDatabase, entity: *Entity, delta_time: f32) void {
    const move_x = entity.velocity.x * delta_time;
    const move_y = entity.velocity.y * delta_time;

    // Horizontal movement
    const new_x = entity.position.x + move_x;
    const h_collision = checkCollision(db, .{ .x = new_x, .y = entity.position.y }, entity.size, false);
    if (!h_collision.collided) {
        entity.position.x = new_x;
    } else {
        entity.velocity.x = 0;
    }

    // Vertical movement
    const new_y = entity.position.y + move_y;
    const v_collision = checkCollision(db, .{ .x = entity.position.x, .y = new_y }, entity.size, entity.velocity.y > 0);

    if (!v_collision.collided) {
        entity.position.y = new_y;
        entity.grounded = false;
        entity.on_platform = false;
    } else {
        if (entity.velocity.y > 0) {
            entity.grounded = v_collision.is_solid;
            entity.on_platform = v_collision.is_platform;
        }
        entity.velocity.y = 0;
    }
}

const CollisionResult = struct {
    collided: bool,
    is_solid: bool,
    is_platform: bool,
};

fn checkCollision(db: *GameDatabase, pos: Vec2, size: Vec2, check_platforms: bool) CollisionResult {
    const min_x = @as(i32, @intFromFloat(@floor(pos.x - size.x / 2)));
    const max_x = @as(i32, @intFromFloat(@ceil(pos.x + size.x / 2)));
    const min_y = @as(i32, @intFromFloat(@floor(pos.y - size.y / 2)));
    const max_y = @as(i32, @intFromFloat(@ceil(pos.y + size.y / 2)));

    var result = CollisionResult{ .collided = false, .is_solid = false, .is_platform = false };

    var y = min_y;
    while (y <= max_y) : (y += 1) {
        var x = min_x;
        while (x <= max_x) : (x += 1) {
            if (x < 0 or x >= WORLD_WIDTH or y < 0 or y >= WORLD_HEIGHT) {
                result.collided = true;
                result.is_solid = true;
                return result;
            }

            const ux = @as(usize, @intCast(x));
            const uy = @as(usize, @intCast(y));

            // Check block tiles
            if (db.block_tiles[uy][ux] != .air) {
                result.collided = true;
                result.is_solid = true;
                return result;
            }

            // Check entity tiles
            const entity_tile = db.entity_tiles[uy][ux];
            if (entity_tile.tile_type != .none) {
                const props = getTileProperties(entity_tile.tile_type);

                if (props.solid) {
                    result.collided = true;
                    result.is_solid = true;
                    return result;
                }

                // Platform collision (only from above)
                if (props.platform and check_platforms) {
                    // Check if entity bottom is near platform top
                    const entity_bottom = pos.y + size.y / 2;
                    const platform_top = @as(f32, @floatFromInt(y));

                    if (entity_bottom >= platform_top and entity_bottom <= platform_top + 0.5) {
                        result.collided = true;
                        result.is_platform = true;
                        return result;
                    }
                }
            }
        }
    }

    return result;
}

fn pickUpItems(db: *GameDatabase, player: *Entity) void {
    var i: usize = 0;
    while (i < db.entity_count) : (i += 1) {
        var item = &db.entities[i];
        if (!item.alive or item.entity_type != .item_drop) continue;

        const dx = item.position.x - player.position.x;
        const dy = item.position.y - player.position.y;
        const distance = @sqrt(dx * dx + dy * dy);

        if (distance < 1.5) {
            if (addToInventory(db, item.item_stack)) {
                item.alive = false;
            }
        }
    }
}

fn addToInventory(db: *GameDatabase, stack: ItemStack) bool {
    // Try to stack with existing items
    var i: usize = 0;
    while (i < INVENTORY_SIZE) : (i += 1) {
        if (db.inventory[i].item_type == stack.item_type) {
            const space = 64 - db.inventory[i].count;
            if (space >= stack.count) {
                db.inventory[i].count += stack.count;
                return true;
            }
        }
    }

    // Find empty slot
    i = 0;
    while (i < INVENTORY_SIZE) : (i += 1) {
        if (db.inventory[i].item_type == .none or db.inventory[i].count == 0) {
            db.inventory[i] = stack;
            return true;
        }
    }

    return false;
}

fn updateEntityAI(db: *GameDatabase, delta_time: f32) void {
    _ = delta_time;

    var i: usize = 0;
    while (i < db.entity_count) : (i += 1) {
        const entity = &db.entities[i];
        if (!entity.alive) continue;

        switch (entity.entity_type) {
            .zombie, .skeleton => {
                const player = getEntityById(db, db.player_entity_id) orelse continue;
                const dx = player.position.x - entity.position.x;

                if (@abs(dx) > 1.0) {
                    entity.velocity.x = if (dx > 0) 3.0 else -3.0;

                    if (entity.grounded and @abs(entity.velocity.x) < 0.1) {
                        entity.velocity.y = -8.0;
                    }
                }
            },
            else => {},
        }
    }
}

// ============================================================================
// CONVERSION FUNCTIONS
// ============================================================================

fn getMiningPower(item_type: ItemType) u8 {
    return switch (item_type) {
        .wooden_pickaxe, .wooden_axe => 2,
        .stone_pickaxe => 3,
        .iron_pickaxe => 5,
        .diamond_pickaxe => 8,
        else => 1,
    };
}

fn getBlockHardness(block: BlockTile) u8 {
    return switch (block) {
        .air => 0,
        .dirt, .sand, .leaves => 5,
        .grass, .wood => 10,
        .stone, .coal_ore => 20,
        .iron_ore => 30,
        .gold_ore => 35,
        .diamond_ore => 50,
        .bedrock => 255,
    };
}

fn blockToItem(block: BlockTile) ItemType {
    return switch (block) {
        .stone => .stone,
        .dirt => .dirt,
        .grass => .dirt,
        .wood => .wood,
        .sand => .sand,
        .coal_ore => .coal,
        .iron_ore => .iron_ore,
        .gold_ore => .gold_ore,
        .diamond_ore => .diamond,
        else => .none,
    };
}

fn itemToBlock(item: ItemType) BlockTile {
    return switch (item) {
        .stone => .stone,
        .dirt => .dirt,
        .grass => .grass,
        .wood => .wood,
        .sand => .sand,
        else => .air,
    };
}

fn itemToEntityTile(item: ItemType) EntityTileType {
    return switch (item) {
        .platform => .platform,
        .crafting_table => .crafting_table,
        .chair => .chair,
        .table => .table,
        .chest => .chest,
        .door => .door_closed,
        .torch => .torch,
        else => .none,
    };
}

fn entityTileToItem(tile: EntityTileType) ItemType {
    return switch (tile) {
        .platform => .platform,
        .crafting_table => .crafting_table,
        .chair => .chair,
        .table => .table,
        .chest => .chest,
        .door_closed, .door_open => .door,
        .torch => .torch,
        else => .none,
    };
}

// ============================================================================
// PUBLIC INTERFACE
// ============================================================================

pub fn setInput(db: *GameDatabase, input: InputState) void {
    db.input = input;
}

pub fn getPlayerPosition(db: *const GameDatabase) Vec2 {
    const player = getEntityByIdConst(db, db.player_entity_id) orelse return Vec2.zero();
    return player.position;
}

fn getEntityByIdConst(db: *const GameDatabase, id: u32) ?*const Entity {
    var i: usize = 0;
    while (i < db.entity_count) : (i += 1) {
        if (db.entities[i].id == id) {
            return &db.entities[i];
        }
    }
    return null;
}

pub fn getBackgroundTile(db: *const GameDatabase, x: usize, y: usize) BackgroundTile {
    if (x >= WORLD_WIDTH or y >= WORLD_HEIGHT) return .none;
    return db.background_tiles[y][x];
}

pub fn getBlockTile(db: *const GameDatabase, x: usize, y: usize) BlockTile {
    if (x >= WORLD_WIDTH or y >= WORLD_HEIGHT) return .bedrock;
    return db.block_tiles[y][x];
}

pub fn getEntityTile(db: *const GameDatabase, x: usize, y: usize) EntityTile {
    if (x >= WORLD_WIDTH or y >= WORLD_HEIGHT) return std.mem.zeroes(EntityTile);
    return db.entity_tiles[y][x];
}

pub fn getInventorySlot(db: *const GameDatabase, slot: usize) ItemStack {
    if (slot >= INVENTORY_SIZE) return .{ .item_type = .none, .count = 0 };
    return db.inventory[slot];
}

pub fn getSelectedHotbarSlot(db: *const GameDatabase) u8 {
    return db.selected_hotbar_slot;
}

pub fn getTimeOfDay(db: *const GameDatabase) f32 {
    return db.time_of_day;
}

pub fn getEntities(db: *const GameDatabase) []const Entity {
    return db.entities[0..db.entity_count];
}

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    const game = try createGameDatabase(allocator);
    defer allocator.destroy(game);

    std.debug.print("Terraria-inspired game with dual-layer system initialized!\n", .{});
    std.debug.print("World size: {}x{}\n", .{ WORLD_WIDTH, WORLD_HEIGHT });
    std.debug.print("Features:\n", .{});
    std.debug.print("- Background layer (walls)\n", .{});
    std.debug.print("- Block layer (terrain)\n", .{});
    std.debug.print("- Entity tile layer (furniture, doors, platforms)\n", .{});
    std.debug.print("- Multi-tile objects support\n", .{});
    std.debug.print("- Platform mechanics (can stand on, pass through)\n", .{});
    std.debug.print("- Interactive tiles (doors, chests)\n", .{});

    // Simulate placing some furniture
    _ = placeEntityTile(game, 250, 95, .platform);
    _ = placeEntityTile(game, 251, 95, .platform);
    _ = placeEntityTile(game, 252, 95, .platform);
    _ = placeEntityTile(game, 253, 95, .platform);
    _ = placeEntityTile(game, 248, 98, .door_closed);
    _ = placeEntityTile(game, 251, 96, .crafting_table);
    _ = placeEntityTile(game, 254, 98, .chest);

    // Simulate a few ticks
    var tick: u32 = 0;
    while (tick < 10) : (tick += 1) {
        game.input = .{
            .move_left = false,
            .move_right = tick % 2 == 0,
            .jump = tick == 5,
            .down = false,
            .mine = false,
            .place = false,
            .interact = tick == 7, // Interact with door on tick 7
            .cursor_world_pos = .{ .x = 248, .y = 99 },
            .hotbar_selection = null,
        };

        updateGame(game, 0.016);

        const pos = getPlayerPosition(game);
        std.debug.print("Tick {}: Player at {d:.2}, {d:.2}", .{ tick, pos.x, pos.y });

        const player = getEntityById(game, game.player_entity_id).?;
        if (player.on_platform) {
            std.debug.print(" [ON PLATFORM]", .{});
        } else if (player.grounded) {
            std.debug.print(" [GROUNDED]", .{});
        }
        std.debug.print("\n", .{});
    }

    std.debug.print("\nGame core with advanced tile system ready!\n", .{});
}
