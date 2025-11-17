#include "dh/prl.h"
#include "dh/math/vec.h"
#include "dh/mem/Allocator.h"

#include "dh/main.h"
#include "dh/heap/Page.h"
#include "dh/io/stream.h"
#include "dh/time.h"

#include "engine.h"

// ============================================================================
// CORE DATA DEFINITIONS
// ============================================================================

// Background tiles (walls, decorative)
typedef enum BgTile : u8 {
    BgTile_none       = 0,
    BgTile_stone_wall = 1,
    BgTile_dirt_wall  = 2,
    BgTile_wood_wall  = 3,
    BgTile_glass_wall = 4,
} BgTile;

// Foreground block types (solid terrain)
typedef enum FgTile : u8 {
    FgTile_air         = 0,
    FgTile_stone       = 1,
    FgTile_dirt        = 2,
    FgTile_grass       = 3,
    FgTile_wood        = 4,
    FgTile_leaves      = 5,
    FgTile_sand        = 6,
    FgTile_coal_ore    = 7,
    FgTile_iron_ore    = 8,
    FgTile_gold_ore    = 9,
    FgTile_diamond_ore = 10,
    FgTile_bedrock     = 11,
} FgTile;

// Entity tiles (furniture, interactive objects)
typedef enum EntityTileType : u8 {
    EntityTileType_none           = 0,
    // Passable (can walk through, can stand on)
    EntityTileType_platform       = 1,
    EntityTileType_crafting_table = 2,
    EntityTileType_chair          = 3,
    EntityTileType_table          = 4,
    // Tree system
    EntityTileType_tree_trunk     = 5, // 나무 기둥 (엔티티)
    EntityTileType_wood_plank     = 6, // 나무 판자 (블럭)
    // Impenetrable (solid collision)
    EntityTileType_chest          = 10,
    EntityTileType_furnace        = 11,
    EntityTileType_anvil          = 12,
    // Interactive impenetrable
    EntityTileType_door_closed    = 20,
    EntityTileType_door_open      = 21,
    // Lighting
    EntityTileType_torch          = 30,
    EntityTileType_lantern        = 31,
} EntityTileType;
typedef struct EntityTile {
    EntityTileType type;
    m_V2i32        origin;    // For multi-tile objects, points to top-left tile
    bool           is_origin; // True if this is the origin tile
    u32            metadata;  // Generic storage for tile-specific data
} EntityTile;

typedef struct TileProps {
    m_V2i32 size;        // Width x Height in tiles
    bool    obstacle;    // Can entities pass through?
    bool    platform;    // Can stand on top but pass through from below?
    bool    interactive; // Can be interacted with?
    u8      light_level; // Light emission (0-15)
} TileProps;

typedef enum ItemType : u16 {
    ItemType_none            = 0,
    // Blocks
    ItemType_stone           = 1,
    ItemType_dirt            = 2,
    ItemType_grass           = 3,
    ItemType_wood            = 4,
    ItemType_sand            = 5,
    // Furniture
    ItemType_platform        = 50,
    ItemType_crafting_table  = 51,
    ItemType_chair           = 52,
    ItemType_table           = 53,
    ItemType_chest           = 54,
    ItemType_door            = 55,
    ItemType_torch           = 56,
    // Tools
    ItemType_wooden_pickaxe  = 100,
    ItemType_stone_pickaxe   = 101,
    ItemType_iron_pickaxe    = 102,
    ItemType_diamond_pickaxe = 103,
    ItemType_wooden_axe      = 110,
    ItemType_wooden_hammer   = 120,
    // Resources
    ItemType_coal            = 200,
    ItemType_iron_ore        = 201,
    ItemType_gold_ore        = 202,
    ItemType_diamond         = 203,
} ItemType;
typedef struct ItemStack {
    ItemType item_type;
    u16      count;
} ItemStack;

typedef u32 EntityId;
typedef enum EntityType : u8 {
    EntityType_player    = 0,
    EntityType_item_drop = 1,
    EntityType_zombie    = 2,
    EntityType_skeleton  = 3,
} EntityType;
typedef struct Entity {
    EntityId   id;
    EntityType entity_type;
    m_V2f32    position;
    m_V2f32    velocity;
    m_V2f32    size; // Size in world units (tiles)
    i16        health;
    i16        max_health;
    bool       alive;
    bool       grounded;
    bool       on_platform; // Standing on a platform

    // Entity-specific data
    ItemStack item_stack; // For item_drop entities
} Entity;

#define Game_world_width           (500)
#define Game_world_height          (200)
#define Game_player_size_x         (1.25f)
#define Game_player_size_y         (2.625f)
#define Game_player_health_default (100)
#define Game_inventory_size        (40)
#define Game_hotbar_size           (10)
#define Game_max_entities          (1000)

// Rendering constants
#define Render_screen_width  (120)
#define Render_screen_height (40)
#define Render_tile_size     (1) // Each tile = 1 pixel for simplicity

// ============================================================================
// INPUT STATE
// ============================================================================

typedef struct InputState {
    m_V2f32 cursor_world_pos;
    O$u8  hotbar_selection;
    union {
        u8 states;
        struct {
            u8 move_left  : 1;
            u8 move_right : 1;
            u8 jump       : 1;
            u8 down       : 1; // For dropping through platforms
            u8 mine       : 1;
            u8 place      : 1;
            u8 interact   : 1;
        };
    };
} InputState;

// ============================================================================
// CENTRALIZED GAME DATABASE
// ============================================================================

typedef struct WorldLayer {
    BgTile     bg_tiles[Game_world_height][Game_world_width];
    FgTile     fg_tiles[Game_world_height][Game_world_width];
    EntityTile entity_tiles[Game_world_height][Game_world_width];
    u8         block_health[Game_world_height][Game_world_width]; // Mining progress
} WorldLayer;

typedef struct WorldState {
    f32 time_of_day; // 0.0 to 1.0
    u64 tick_count;
} WorldState;

typedef struct PlayerData {
    EntityId  entity_id;
    ItemStack inventory[Game_inventory_size];
    u8        selected_hotbar_slot;
} PlayerData;

typedef struct EntitySystem {
    Entity   data[Game_max_entities];
    u32      count;
    EntityId next_id;
} EntitySystem;

typedef struct Camera {
    m_V2f32 position; // Camera center position in world coordinates
    f32     zoom;     // Zoom level (1.0 = normal)
} Camera;

typedef struct Renderer {
    engine_Window*     window;
    engine_Canvas*     canvas;
    engine_Input*      input;
    engine_core_VT100* core;
    Camera             camera;
    mem_Allocator      allocator;
} Renderer;

typedef struct GameState {
    struct {
        WorldLayer layer;
        WorldState state;
    } world;
    struct {
        PlayerData data;
        InputState input;
    } player;
    EntitySystem entities;
    Renderer     renderer;
} GameState;

// ============================================================================
// PHYSICS CONSTANTS
// ============================================================================

#define Game_gravity           (90.0f)
#define Game_player_speed_x    (18.0f)
#define Game_jump_velocity     (18.0f) // 점프력 증가
#define Game_terminal_velocity (30.0f)

// ============================================================================
// TILE COLOR MAPPING
// ============================================================================

// Background tile colors
fn_((Render_getBgTileColor(BgTile tile))(Color)) {
    switch (tile) {
    case BgTile_none:
        return Color_transparent;
    case BgTile_stone_wall:
        return literal_Color_fromOpaque(100, 100, 100); // Gray
    case BgTile_dirt_wall:
        return literal_Color_fromOpaque(139, 69, 19); // Brown
    case BgTile_wood_wall:
        return literal_Color_fromOpaque(160, 82, 45); // Saddle brown
    case BgTile_glass_wall:
        return literal_Color_fromOpaque(173, 216, 230); // Light blue
    default:
        return Color_black;
    }
}

// Foreground tile colors
fn_((Render_getFgTileColor(FgTile tile))(Color)) {
    switch (tile) {
    case FgTile_air:
        return Color_transparent;
    case FgTile_stone:
        return literal_Color_fromOpaque(128, 128, 128); // Gray
    case FgTile_dirt:
        return literal_Color_fromOpaque(139, 69, 19); // Brown
    case FgTile_grass:
        return literal_Color_fromOpaque(34, 139, 34); // Forest green
    case FgTile_wood:
        return literal_Color_fromOpaque(160, 82, 45); // Saddle brown
    case FgTile_leaves:
        return literal_Color_fromOpaque(0, 128, 0); // Green
    case FgTile_sand:
        return literal_Color_fromOpaque(238, 203, 173); // Peach puff
    case FgTile_coal_ore:
        return literal_Color_fromOpaque(64, 64, 64); // Dark gray
    case FgTile_iron_ore:
        return literal_Color_fromOpaque(192, 192, 192); // Silver
    case FgTile_gold_ore:
        return literal_Color_fromOpaque(255, 215, 0); // Gold
    case FgTile_diamond_ore:
        return literal_Color_fromOpaque(0, 191, 255); // Deep sky blue
    case FgTile_bedrock:
        return literal_Color_fromOpaque(25, 25, 25); // Very dark gray
    default:
        return Color_black;
    }
}

fn_((FgTile_isSolid(FgTile tile))(bool)) {
    switch (tile) {
    case FgTile_air:
    case FgTile_leaves: // 나무 잎은 통과 가능
        return false;
    case FgTile_dirt:
    case FgTile_grass:
    case FgTile_stone:
    case FgTile_wood:
    case FgTile_sand:
    case FgTile_coal_ore:
    case FgTile_iron_ore:
    case FgTile_gold_ore:
    case FgTile_diamond_ore:
    case FgTile_bedrock:
        return true;
    default:
        claim_unreachable;
    }
}

// Entity tile colors
fn_((Render_getEntityTileColor(EntityTileType tile))(Color)) {
    switch (tile) {
    case EntityTileType_none:
        return Color_transparent;
    case EntityTileType_platform:
        return literal_Color_fromOpaque(160, 82, 45); // Saddle brown
    case EntityTileType_crafting_table:
        return literal_Color_fromOpaque(139, 69, 19); // Brown
    case EntityTileType_chair:
        return literal_Color_fromOpaque(101, 67, 33); // Dark brown
    case EntityTileType_table:
        return literal_Color_fromOpaque(139, 69, 19); // Brown
    // Tree system
    case EntityTileType_tree_trunk:
        return literal_Color_fromOpaque(160, 82, 45); // Saddle brown (나무 기둥)
    case EntityTileType_wood_plank:
        return literal_Color_fromOpaque(222, 184, 135); // Burlywood (나무 판자)
    case EntityTileType_chest:
        return literal_Color_fromOpaque(139, 69, 19); // Brown
    case EntityTileType_furnace:
        return literal_Color_fromOpaque(105, 105, 105); // Dim gray
    case EntityTileType_anvil:
        return literal_Color_fromOpaque(192, 192, 192); // Silver
    case EntityTileType_door_closed:
    case EntityTileType_door_open:
        return literal_Color_fromOpaque(101, 67, 33); // Dark brown
    case EntityTileType_torch:
        return literal_Color_fromOpaque(255, 69, 0); // Red orange
    case EntityTileType_lantern:
        return literal_Color_fromOpaque(255, 215, 0); // Gold
    default:
        return Color_black;
    }
}

// Entity colors
fn_((Render_getEntityColor(EntityType entity_type))(Color)) {
    switch (entity_type) {
    case EntityType_player:
        return literal_Color_fromOpaque(0, 0, 255); // Blue
    case EntityType_item_drop:
        return literal_Color_fromOpaque(255, 255, 0); // Yellow
    case EntityType_zombie:
        return literal_Color_fromOpaque(0, 128, 0); // Green
    case EntityType_skeleton:
        return literal_Color_fromOpaque(255, 255, 255); // White
    default:
        return Color_white;
    }
}

// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================

fn_((Game_init(mem_Allocator allocator))(GameState*));
fn_((Game_fini(GameState* self, mem_Allocator allocator))(void));
fn_((Game_genWorld(GameState* self))(void));

fn_((Game_placeEntityTile(GameState* self, i32 x, i32 y, EntityTileType tile_type))(bool));
T_use_O$(EntityTileType);
fn_((Game_removeEntityTile(GameState* self, i32 x, i32 y))(O$EntityTileType));
fn_((Game_destroyTreeAbove(GameState* self, usize x, usize y))(void));

T_use_O$(EntityId);
fn_((Game_appendEntity(GameState* self, Entity entity))(O$EntityId));
T_use$((Entity)(P, O));
T_use_O$(P_const$Entity);
fn_((Game_idxEntityById(const GameState* self, EntityId id))(O$P_const$Entity));
T_use_O$(P$Entity);
fn_((Game_idxMutEntityById(GameState* self, EntityId id))(O$P$Entity));
fn_((Game_removeDeadEntities(GameState* self))(void));

fn_((Item_miningPower(ItemType item_type))(u8));
fn_((Item_blockHardness(FgTile block))(u8));
fn_((Item_blockToItem(FgTile block))(ItemType));
fn_((Item_itemToBlock(ItemType item))(FgTile));
fn_((Item_itemToEntityTile(ItemType item))(EntityTileType));
fn_((Item_entityTileToItem(EntityTileType tile))(ItemType));

fn_((Game_createItemDrop(GameState* self, m_V2f32 pos, ItemType item_type, u16 count))(void));
fn_((Game_addToInventory(GameState* self, ItemStack stack))(bool));
fn_((Game_pickUpItems(GameState* self, Entity* player))(void));

fn_((Game_processInput(GameState* self))(void));
fn_((Game_handleMining(GameState* self))(void));
fn_((Game_handleBlockPlacement(GameState* self))(void));
fn_((Game_handleInteraction(GameState* self))(void));

fn_((Game_updatePhysics(GameState* self, f32 delta_time))(void));
fn_((Game_updateEntityAI(GameState* self, f32 delta_time))(void));
fn_((Game_update(GameState* self, f32 delta_time))(void));

fn_((Game_setInput(GameState* self, InputState input))(void));
fn_((Game_getPlayerPosition(GameState* self))(m_V2f32));
fn_((Game_getBackgroundTile(GameState* self, usize x, usize y))(BgTile));
fn_((Game_getFgTile(GameState* self, usize x, usize y))(FgTile));
fn_((Game_getEntityTile(GameState* self, usize x, usize y))(EntityTile));
fn_((Game_getInventorySlot(GameState* self, usize slot))(ItemStack));
fn_((Game_getSelectedHotbarSlot(GameState* self))(u8));
fn_((Game_getTimeOfDay(GameState* self))(f32));
fn_((Game_getEntityCount(GameState* self))(u32));
fn_((Game_getEntity(GameState* self, usize index))(O$Entity));

// Rendering functions
fn_((Render_init(mem_Allocator allocator))(Renderer*));
fn_((Render_fini(Renderer* self, mem_Allocator allocator))(void));
fn_((Render_worldToScreen(const Camera* camera, m_V2f32 world_pos))(m_V2i32));
fn_((Render_screenToWorld(const Camera* camera, m_V2i32 screen_pos))(m_V2f32));
fn_((Render_updateCamera(Renderer* self, const GameState* game))(void));
fn_((Render_world(GameState* self))(void));
fn_((Render_entities(GameState* self))(void));
fn_((Render_ui(GameState* self))(void));
fn_((Render_hotbar(GameState* self))(void));
fn_((Render_selectedItemInfo(GameState* self))(void));
fn_((Render_cursor(GameState* self))(void));
fn_((Render_playerStatus(GameState* self))(void));
fn_((Game_render(GameState* self))(void));

// ============================================================================
// TILE PROPERTIES LOOKUP
// ============================================================================

fn_((EntityTile_props(EntityTileType tile_type))(TileProps)) {
    switch (tile_type) {
    case EntityTileType_none:
        return (TileProps){
            .size        = { .x = 1, .y = 1 },
            .obstacle    = false,
            .platform    = false,
            .interactive = false,
            .light_level = 0,
        };
    // Passable platforms
    case EntityTileType_platform:
        return (TileProps){
            .size        = { .x = 1, .y = 1 },
            .obstacle    = false,
            .platform    = true,
            .interactive = false,
            .light_level = 0,
        };
    case EntityTileType_crafting_table:
        return (TileProps){
            .size        = { .x = 2, .y = 1 },
            .obstacle    = false,
            .platform    = true,
            .interactive = true,
            .light_level = 0,
        };
    case EntityTileType_chair:
        return (TileProps){
            .size        = { .x = 1, .y = 2 },
            .obstacle    = false,
            .platform    = true,
            .interactive = false,
            .light_level = 0,
        };
    case EntityTileType_table:
        return (TileProps){
            .size        = { .x = 3, .y = 2 },
            .obstacle    = false,
            .platform    = true,
            .interactive = false,
            .light_level = 0,
        };
    // Tree system
    case EntityTileType_tree_trunk:
        return (TileProps){
            .size        = { .x = 1, .y = 1 },
            .obstacle    = false,
            .platform    = false,
            .interactive = true, // 채굴 가능
            .light_level = 0,
        };
    case EntityTileType_wood_plank:
        return (TileProps){
            .size        = { .x = 1, .y = 1 },
            .obstacle    = true,
            .platform    = false,
            .interactive = true, // 채굴 가능
            .light_level = 0,
        };
    case EntityTileType_chest:
        return (TileProps){
            .size        = { .x = 2, .y = 2 },
            .obstacle    = false,
            .platform    = false,
            .interactive = true,
            .light_level = 0,
        };
    // Solid objects
    case EntityTileType_furnace:
        return (TileProps){
            .size        = { .x = 2, .y = 2 },
            .obstacle    = false,
            .platform    = false,
            .interactive = true,
            .light_level = 3,
        };
    case EntityTileType_anvil:
        return (TileProps){
            .size        = { .x = 2, .y = 1 },
            .obstacle    = false,
            .platform    = false,
            .interactive = true,
            .light_level = 0,
        };
    // Doors
    case EntityTileType_door_closed:
        return (TileProps){
            .size        = { .x = 1, .y = 3 },
            .obstacle    = true,
            .platform    = false,
            .interactive = true,
            .light_level = 0,
        };
    case EntityTileType_door_open:
        return (TileProps){
            .size        = { .x = 1, .y = 3 },
            .obstacle    = false,
            .platform    = false,
            .interactive = true,
            .light_level = 0,
        };
    // Lighting
    case EntityTileType_torch:
        return (TileProps){
            .size        = { .x = 1, .y = 1 },
            .obstacle    = false,
            .platform    = false,
            .interactive = false,
            .light_level = 10,
        };
    case EntityTileType_lantern:
        return (TileProps){
            .size        = { .x = 1, .y = 1 },
            .obstacle    = false,
            .platform    = false,
            .interactive = false,
            .light_level = 12,
        };
    default:
        claim_unreachable;
    };
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

fn_((hash2D(usize x, usize y))(u32)) {
    let h1 = (as$(u32)(x) * lit_n$(u32)(374, 761, 393)) + (as$(u32)(y) * lit_n$(u32)(668, 265, 263));
    let h2 = (h1 ^ (h1 >> 13)) * lit_n$(u32)(1, 274, 126, 177);
    return h2 ^ (h2 >> 16);
}

// ============================================================================
// INITIALIZATION
// ============================================================================

fn_((Game_init(mem_Allocator allocator))(GameState*)) {
    let self = u_castP$((GameState*)(
        catch_((mem_Allocator_create(allocator, typeInfo$(GameState)))(
            $ignore, claim_unreachable
        ))
    ));

    // Initialize memory to zero
    self->player.data.entity_id            = 0;
    self->player.data.selected_hotbar_slot = 0;
    self->entities.count                   = 0;
    self->entities.next_id                 = 0;
    self->world.state.time_of_day          = 0.25f; // Start at morning
    self->world.state.tick_count           = 0;

    // Initialize empty world
    for_(($r(0, Game_world_height))(y) {
        for_(($r(0, Game_world_width))(x) {
            self->world.layer.fg_tiles[y][x]     = FgTile_air;
            self->world.layer.bg_tiles[y][x]     = BgTile_none;
            self->world.layer.entity_tiles[y][x] = (EntityTile){ 0 };
            self->world.layer.block_health[y][x] = 0;
        });
    });

    // Initialize inventory
    for_(($r(0, Game_inventory_size))(i) {
        self->player.data.inventory[i] = ((ItemStack){ .item_type = ItemType_none, .count = 0 });
    });

    // Initialize input state
    self->player.input = (InputState){ .states = 0, .hotbar_selection = none$((O$u8)) };

    // Generate world
    Game_genWorld(self);

    // Create player entity
    let entity = (Entity){
        .id          = 0,
        .entity_type = EntityType_player,
        .position    = { .x = as$(f32)(Game_world_width) / 2.0f, .y = 50.0f },
        .velocity    = m_V2f32_zero,
        .size        = { .x = Game_player_size_x, .y = Game_player_size_y },
        .health      = Game_player_health_default,
        .max_health  = Game_player_health_default,
        .alive       = true,
        .grounded    = false,
        .on_platform = false,
        .item_stack  = { .item_type = ItemType_none, .count = 0 },
    };
    self->player.data.entity_id = unwrap_(Game_appendEntity(self, entity));

    // Give player starting items
    self->player.data.inventory[0] = (ItemStack){ .item_type = ItemType_wooden_pickaxe, .count = 1 };
    self->player.data.inventory[1] = (ItemStack){ .item_type = ItemType_wooden_hammer, .count = 1 };
    self->player.data.inventory[2] = (ItemStack){ .item_type = ItemType_wooden_axe, .count = 1 };
    self->player.data.inventory[3] = (ItemStack){ .item_type = ItemType_platform, .count = 50 };
    self->player.data.inventory[4] = (ItemStack){ .item_type = ItemType_wood, .count = 100 };
    self->player.data.inventory[5] = (ItemStack){ .item_type = ItemType_torch, .count = 20 };
    self->player.data.inventory[6] = (ItemStack){ .item_type = ItemType_door, .count = 3 };
    self->player.data.inventory[7] = (ItemStack){ .item_type = ItemType_crafting_table, .count = 1 };
    self->player.data.inventory[8] = (ItemStack){ .item_type = ItemType_chest, .count = 2 };

    // Initialize renderer
    self->renderer = *Render_init(allocator);

    return self;
}

fn_((Game_fini(GameState* self, mem_Allocator allocator))(void)) {
    Render_fini(&self->renderer, allocator);
    mem_Allocator_destroy(allocator, u_anyP(self));
}

fn_((Game_genWorld(GameState* self))(void)) {
    const usize surface_height = Game_world_height / 2;
    // Generate terrain
    for_(($r(0, Game_world_width))(x) {
        let height_variation = flt_sin(as$(f32)(x) * 0.05f) * 10.0f;
        let height           = as$(i32)(surface_height) + as$(i32)(height_variation);

        for_(($r(0, Game_world_height))(y) {
            if_(let iy = as$(i32)(y), (iy > height + 30)) {
                self->world.layer.fg_tiles[y][x] = FgTile_bedrock;
            } else if (iy > height + 15) {
                const u32 rand = hash2D(x, y) % 100;
                if (rand < 3) {
                    self->world.layer.fg_tiles[y][x] = FgTile_diamond_ore;
                } else if (rand < 10) {
                    self->world.layer.fg_tiles[y][x] = FgTile_gold_ore;
                } else {
                    self->world.layer.fg_tiles[y][x] = FgTile_stone;
                }
                // Add stone wall background
                if (self->world.layer.bg_tiles[y][x] == BgTile_none) {
                    self->world.layer.bg_tiles[y][x] = BgTile_stone_wall;
                }
            } else if (iy > height + 5) {
                const u32 rand = hash2D(x, y) % 100;
                if (rand < 8) {
                    self->world.layer.fg_tiles[y][x] = FgTile_iron_ore;
                } else if (rand < 15) {
                    self->world.layer.fg_tiles[y][x] = FgTile_coal_ore;
                } else {
                    self->world.layer.fg_tiles[y][x] = FgTile_stone;
                }
                // Add dirt wall background near surface
                if (iy > height + 10 && self->world.layer.bg_tiles[y][x] == BgTile_none) {
                    self->world.layer.bg_tiles[y][x] = BgTile_dirt_wall;
                }
            } else if (iy > height) {
                self->world.layer.fg_tiles[y][x] = FgTile_dirt;
            } else if (iy == height) {
                self->world.layer.fg_tiles[y][x] = FgTile_grass;
            }
        });
        // Add trees
        if ((x % 7 == 3) && (2 < x && x < (Game_world_width - 2))) {
            let tree_height = 5 + (hash2D(x, 0) % 3);
            let base_y      = as$(usize)(height - 1);
            if (base_y > (tree_height + 3)) {
                // Trunk as entity tiles (나무 기둥을 엔티티 타일로)
                for_(($r(1, tree_height + 1))(ty) {
                    let trunk_y                                = base_y - ty;
                    self->world.layer.entity_tiles[trunk_y][x] = ((EntityTile){
                        .type      = EntityTileType_tree_trunk,
                        .origin    = { .x = as$(i32)(x), .y = as$(i32)(trunk_y) },
                        .is_origin = true,
                        .metadata  = as$(u32)(ty), // 트리 높이 정보 저장
                    });
                });
                // Leaves
                const usize leaf_start = base_y - tree_height;
                if (leaf_start > 2) {
                    let leaf_offsets_x = ((i32[]){ -2, -1, 0, 1, 2 });
                    let leaf_offsets_y = ((i32[]){ -1, 0, 1, 2 });

                    for_(($r(0, 5))(dx_idx) {
                        for_(($r(0, 4))(dy_idx) {
                            let lx = as$(i32)(x) + leaf_offsets_x[dx_idx];
                            let ly = as$(i32)(leaf_start) + leaf_offsets_y[dy_idx];

                            if ((0 <= lx) && (lx < Game_world_width) && (0 <= ly) && (ly < Game_world_height)) {
                                let ulx = as$(usize)(lx);
                                let uly = as$(usize)(ly);

                                if (self->world.layer.fg_tiles[uly][ulx] == FgTile_air) {
                                    self->world.layer.fg_tiles[uly][ulx] = FgTile_leaves;
                                }
                            }
                        });
                    });
                }
            }
        }
    });
}

// ============================================================================
// ENTITY TILE MANAGEMENT
// ============================================================================

fn_((Game_placeEntityTile(GameState* self, i32 x, i32 y, EntityTileType tile_type))(bool)) {
    const TileProps props = EntityTile_props(tile_type);
    // Check if space is available
    for_(($r(0, props.size.y))(dy) {
        for_(($r(0, props.size.x))(dx) {
            let tx = x + dx;
            let ty = y + dy;
            if ((tx < 0 || Game_world_width <= tx) || (ty < 0 || Game_world_height <= ty)) { return false; }

            // Check if blocked by terrain or other entity tiles
            let utx = as$(usize)(tx);
            let uty = as$(usize)(ty);
            if (self->world.layer.fg_tiles[uty][utx] != FgTile_air) { return false; }
            if (self->world.layer.entity_tiles[uty][utx].type != EntityTileType_none) { return false; }
        });
    });
    // Place the multi-tile object
    for_(($r(0, props.size.y))(dy) {
        for_(($r(0, props.size.x))(dx) {
            let tx  = x + dx;
            let ty  = y + dy;
            let utx = as$(usize)(tx);
            let uty = as$(usize)(ty);

            self->world.layer.entity_tiles[uty][utx] = ((EntityTile){
                .type      = tile_type,
                .origin    = { .x = x, .y = y },
                .is_origin = (dx == 0 && dy == 0),
                .metadata  = 0,
            });
        });
    });
    return true;
}

fn_((Game_removeEntityTile(GameState* self, i32 x, i32 y))(O$EntityTileType) $scope) {
    if (x < 0 || Game_world_width <= x || y < 0 || Game_world_height <= y) { return_none(); }

    let ux = as$(usize)(x);
    let uy = as$(usize)(y);

    let tile = self->world.layer.entity_tiles[uy][ux];
    if (tile.type == EntityTileType_none) { return_none(); }

    // Find the origin and remove all parts
    let origin       = tile.origin;
    let props        = EntityTile_props(tile.type);
    let removed_type = tile.type;

    for_(($r(0, props.size.y))(dy) {
        for_(($r(0, props.size.x))(dx) {
            let tx = origin.x + dx;
            let ty = origin.y + dy;
            if ((0 <= tx && tx < Game_world_width) && (0 <= ty && ty < Game_world_height)) {
                self->world.layer.entity_tiles[ty][tx] = ((EntityTile){ 0 });
            }
        });
    });

    // 나무 기둥 파괴 시 위쪽 부분 자동 파괴
    if (removed_type == EntityTileType_tree_trunk) {
        Game_destroyTreeAbove(self, as$(usize)(x), as$(usize)(y));
    }

    return_some(removed_type);
} $unscoped_(fn);

fn_((Game_destroyTreeAbove(GameState* self, usize x, usize y))(void)) {
    // 위쪽으로 올라가면서 나무 기둥과 잎을 파괴
    for_($rev($r(0, y))(check_y) {
        let entity_tile = &self->world.layer.entity_tiles[check_y][x];
        let fg_tile     = self->world.layer.fg_tiles[check_y][x];

        // 나무 기둥 파괴
        if (entity_tile->type == EntityTileType_tree_trunk) {
            let item_type = Item_entityTileToItem(entity_tile->type);
            if (item_type != ItemType_none) {
                Game_createItemDrop(self, (m_V2f32){ .x = as$(f32)(x) + 0.5f, .y = as$(f32)(check_y) + 0.5f }, item_type, 1);
            }
            *entity_tile = ((EntityTile){ 0 });
        }

        // 나무 잎 파괴
        if (fg_tile == FgTile_leaves) {
            let item_type = Item_blockToItem(fg_tile);
            if (item_type != ItemType_none) {
                Game_createItemDrop(self, (m_V2f32){ .x = as$(f32)(x) + 0.5f, .y = as$(f32)(check_y) + 0.5f }, item_type, 1);
            }
            self->world.layer.fg_tiles[check_y][x] = FgTile_air;
        }
    });
}

// ============================================================================
// ENTITY MANAGEMENT
// ============================================================================

fn_((Game_appendEntity(GameState* self, Entity entity))(O$EntityId) $scope) {
    if (Game_max_entities <= self->entities.count) {
        return_none();
    }

    let id = self->entities.next_id;
    self->entities.next_id += 1;

    entity.id = id;

    self->entities.data[self->entities.count] = entity;
    self->entities.count += 1;

    return_some(id);
} $unscoped_(fn);


fn_((Game_idxEntityById(const GameState* self, EntityId id))(O$P_const$Entity) $scope) {
    for_(($r(0, self->entities.count))(i) {
        if (self->entities.data[i].id != id) { continue; }
        return_some(&self->entities.data[i]);
    });
    return_none();
} $unscoped_(fn);

fn_((Game_idxMutEntityById(GameState* self, EntityId id))(O$P$Entity) $scope) {
    for_(($r(0, self->entities.count))(i) {
        if (self->entities.data[i].id != id) { continue; }
        return_some(&self->entities.data[i]);
    });
    return_none();
} $unscoped_(fn);

fn_((Game_removeDeadEntities(GameState* self))(void)) {
    usize write_idx = 0;
    for_(($r(0, self->entities.count))(read_idx) {
        if (self->entities.data[read_idx].alive) {
            if (write_idx != read_idx) {
                self->entities.data[write_idx] = self->entities.data[read_idx];
            }
            write_idx++;
        }
    });
    self->entities.count = write_idx;
}

// ============================================================================
// CONVERSION FUNCTIONS
// ============================================================================

fn_((Item_miningPower(ItemType item_type))(u8)) {
    switch (item_type) {
    case ItemType_wooden_pickaxe:
    case ItemType_wooden_axe:
        return 2;
    case ItemType_stone_pickaxe:
        return 3;
    case ItemType_iron_pickaxe:
        return 5;
    case ItemType_diamond_pickaxe:
        return 8;
    default:
        return 1;
    }
}

fn_((Item_blockHardness(FgTile block))(u8)) {
    switch (block) {
    case FgTile_air:
        return 0;
    case FgTile_dirt:
    case FgTile_sand:
    case FgTile_leaves:
        return 5;
    case FgTile_grass:
    case FgTile_wood:
        return 10;
    case FgTile_stone:
    case FgTile_coal_ore:
        return 20;
    case FgTile_iron_ore:
        return 30;
    case FgTile_gold_ore:
        return 35;
    case FgTile_diamond_ore:
        return 50;
    case FgTile_bedrock:
        return 255;
    default:
        return 1;
    }
}

fn_((Item_blockToItem(FgTile block))(ItemType)) {
    switch (block) {
    case FgTile_stone:
        return ItemType_stone;
    case FgTile_dirt:
        return ItemType_dirt;
    case FgTile_grass:
        return ItemType_grass;
    case FgTile_wood:
        return ItemType_wood;
    case FgTile_sand:
        return ItemType_sand;
    case FgTile_coal_ore:
        return ItemType_coal;
    case FgTile_iron_ore:
        return ItemType_iron_ore;
    case FgTile_gold_ore:
        return ItemType_gold_ore;
    case FgTile_diamond_ore:
        return ItemType_diamond;
    default:
        return ItemType_none;
    }
}

fn_((Item_itemToBlock(ItemType item))(FgTile)) {
    switch (item) {
    case ItemType_stone:
        return FgTile_stone;
    case ItemType_dirt:
        return FgTile_dirt;
    case ItemType_grass:
        return FgTile_grass;
    case ItemType_wood:
        return FgTile_wood;
    case ItemType_sand:
        return FgTile_sand;
    default:
        return FgTile_air;
    }
}

fn_((Item_itemToEntityTile(ItemType item))(EntityTileType)) {
    switch (item) {
    case ItemType_platform:
        return EntityTileType_platform;
    case ItemType_crafting_table:
        return EntityTileType_crafting_table;
    case ItemType_chair:
        return EntityTileType_chair;
    case ItemType_table:
        return EntityTileType_table;
    case ItemType_chest:
        return EntityTileType_chest;
    case ItemType_door:
        return EntityTileType_door_closed;
    case ItemType_torch:
        return EntityTileType_torch;
    default:
        return EntityTileType_none;
    }
}

fn_((Item_entityTileToItem(EntityTileType tile))(ItemType)) {
    switch (tile) {
    case EntityTileType_platform:
        return ItemType_platform;
    case EntityTileType_crafting_table:
        return ItemType_crafting_table;
    case EntityTileType_chair:
        return ItemType_chair;
    case EntityTileType_table:
        return ItemType_table;
    case EntityTileType_chest:
        return ItemType_chest;
    case EntityTileType_door_closed:
    case EntityTileType_door_open:
        return ItemType_door;
    case EntityTileType_torch:
        return ItemType_torch;
    default:
        return ItemType_none;
    }
}

// ============================================================================
// ITEM AND INVENTORY MANAGEMENT
// ============================================================================

fn_((Game_createItemDrop(GameState* self, m_V2f32 pos, ItemType item_type, u16 count))(void)) {
    let pos_x_hash = as$(u32)(pos.x);
    let pos_y_hash = as$(u32)(pos.y);
    let rand_x     = as$(f32)(((hash2D(pos_x_hash, pos_y_hash) % 100) - 50) * 0.01f);

    let entity = ((Entity){
        .id          = 0,
        .entity_type = EntityType_item_drop,
        .position    = pos,
        .velocity    = { .x = rand_x, .y = -2.0f },
        .size        = { .x = 0.3f, .y = 0.3f },
        .health      = 1,
        .max_health  = 1,
        .alive       = true,
        .grounded    = false,
        .on_platform = false,
        .item_stack  = { .item_type = item_type, .count = count },
    });
    Game_appendEntity(self, entity);
}

fn_((Game_addToInventory(GameState* self, ItemStack stack))(bool)) {
    // Try to stack with existing items
    for_(($r(0, Game_inventory_size))(i) {
        if (self->player.data.inventory[i].item_type == stack.item_type) {
            const u16 space = 64 - self->player.data.inventory[i].count;
            if (space >= stack.count) {
                self->player.data.inventory[i].count += stack.count;
                return true;
            }
        }
    });
    // Find empty slot
    for_(($r(0, Game_inventory_size))(i) {
        if (self->player.data.inventory[i].item_type == ItemType_none || self->player.data.inventory[i].count == 0) {
            self->player.data.inventory[i] = stack;
            return true;
        }
    });
    return false;
}

fn_((Game_pickUpItems(GameState* self, Entity* player))(void)) {
    for_(($r(0, self->entities.count))(i) {
        let item = &self->entities.data[i];
        if (!item->alive || item->entity_type != EntityType_item_drop) { continue; }

        let dx       = item->position.x - player->position.x;
        let dy       = item->position.y - player->position.y;
        let distance = flt_sqrt((dx * dx) + (dy * dy));
        if (distance < 1.5f) {
            if (Game_addToInventory(self, item->item_stack)) {
                item->alive = false;
            }
        }
    });
}

// ============================================================================
// COLLISION SYSTEM
// ============================================================================

typedef struct CollisionResult {
    bool collided;
    bool is_obstacle;
    bool is_platform;
} CollisionResult;

fn_((Game_checkCollision(GameState* self, m_V2f32 pos, m_V2f32 size, bool check_platforms))(CollisionResult) $scope) {
    let min_x = as$(i32)(flt_floor(pos.x - (size.x / 2.0f)));
    let max_x = as$(i32)(flt_ceil(pos.x + (size.x / 2.0f)));
    let min_y = as$(i32)(flt_floor(pos.y - (size.y / 2.0f)));
    let max_y = as$(i32)(flt_ceil(pos.y + (size.y / 2.0f)));

    for_(($r(min_y, max_y + 1))(y) {
        for_(($r(min_x, max_x + 1))(x) {
            if ((x < 0 || Game_world_width <= x) || (y < 0 || Game_world_height <= y)) {
                return_({
                    .collided    = true,
                    .is_obstacle = true,
                    .is_platform = false,
                });
            }

            // Check block tiles
            let ux      = as$(usize)(x);
            let uy      = as$(usize)(y);
            let fg_tile = self->world.layer.fg_tiles[uy][ux];
            if (FgTile_isSolid(fg_tile)) {
                return_({
                    .collided    = true,
                    .is_obstacle = true,
                    .is_platform = false,
                });
            }

            // Check entity tiles
            let entity_tile = self->world.layer.entity_tiles[uy][ux];
            if (entity_tile.type != EntityTileType_none) {
                let props = EntityTile_props(entity_tile.type);
                if (props.obstacle) {
                    return_({
                        .collided    = true,
                        .is_obstacle = true,
                        .is_platform = false,
                    });
                }
                // Platform collision (only from above)
                if (props.platform && check_platforms) {
                    // Check if entity bottom is near platform top
                    let entity_bottom = pos.y + (size.y / 2.0f);
                    let platform_top  = as$(f32)(y);

                    if (platform_top <= entity_bottom
                        && entity_bottom <= (platform_top + 0.5f)) {
                        return_({
                            .collided    = true,
                            .is_platform = true,
                            .is_obstacle = false,
                        });
                    }
                }
            }
        });
    });

    return_({
        .collided    = false,
        .is_obstacle = false,
        .is_platform = false,
    });
} $unscoped_(fn);

fn_((Game_moveWithCollision(GameState* self, Entity* entity, f32 delta_time))(void)) {
    let move_x = entity->velocity.x * delta_time;
    let move_y = entity->velocity.y * delta_time;

    // Horizontal movement
    let new_x       = entity->position.x + move_x;
    let h_collision = Game_checkCollision(self, (m_V2f32){ .x = new_x, .y = entity->position.y }, entity->size, false);

    if (!h_collision.collided) {
        entity->position.x = new_x;
    } else {
        // 1칸 높이 블럭 오르기 시도 (플레이어만)
        if (entity->entity_type == EntityType_player && entity->grounded) {
            let step_up_y         = entity->position.y - 1.0f; // 1칸 위로
            let step_up_collision = Game_checkCollision(self, (m_V2f32){ .x = new_x, .y = step_up_y }, entity->size, false);

            if (!step_up_collision.collided) {
                // 1칸 높이 블럭을 오를 수 있음
                entity->position.x = new_x;
                entity->position.y = step_up_y;
            } else {
                entity->velocity.x = 0.0f; // 충돌 시 속도만 0으로
            }
        } else {
            entity->velocity.x = 0.0f; // 충돌 시 속도만 0으로
        }
    }

    // Vertical movement
    let new_y       = entity->position.y + move_y;
    let v_collision = Game_checkCollision(
        self,
        (m_V2f32){ .x = entity->position.x, .y = new_y },
        entity->size,
        entity->velocity.y > 0.0f
    );

    if (!v_collision.collided) {
        entity->position.y  = new_y;
        entity->grounded    = false;
        entity->on_platform = false;
    } else {
        if (entity->velocity.y > 0.0f) {
            entity->grounded    = v_collision.is_obstacle;
            entity->on_platform = v_collision.is_platform;
        } else {
            entity->velocity.y = 0.0f;
        }
    }
}

// ============================================================================
// PHYSICS SYSTEM
// ============================================================================

fn_((Game_updatePhysics(GameState* self, f32 delta_time))(void)) {
    for_(($r(0, self->entities.count))(i) {
        let entity = &self->entities.data[i];
        if (!entity->alive) { continue; }
        // Apply gravity
        entity->velocity.y += Game_gravity * delta_time;
        if (entity->velocity.y > Game_terminal_velocity) {
            entity->velocity.y = Game_terminal_velocity;
        }
        // Move and collide
        Game_moveWithCollision(self, entity, delta_time);
        // Pick up items (player only)
        if (entity->entity_type == EntityType_player) {
            Game_pickUpItems(self, entity);
        }
    });
}

// ============================================================================
// INPUT PROCESSING
// ============================================================================

typedef variant_((Shape)(
    (Shape_circle, struct { f32 radius; }),
    (Shape_rect, struct { f32 width; f32 height; })
)) Shape;

fn_((Game_processInput(GameState* self))(void)) {
    let player = orelse_((Game_idxMutEntityById(self, self->player.data.entity_id))(return));


    player->velocity.x = expr_(f32 $scope) if (self->player.input.move_left) {
        $break_(-Game_player_speed_x);
    } else if (self->player.input.move_right) {
        $break_(Game_player_speed_x);
    } else {
        $break_(player->velocity.x * 0.8f);
    } $unscoped_(expr);

    // Movement
    player->velocity.x = expr_(f32 $scope) if (self->player.input.move_left) {
        $break_(-Game_player_speed_x);
    } else if (self->player.input.move_right) {
        $break_(Game_player_speed_x);
    } else {
        $break_(player->velocity.x * 0.8f);
    } $unscoped_(expr);

    // Jumping
    if (self->player.input.jump && (player->grounded || player->on_platform)) {
        player->velocity.y = -Game_jump_velocity;
    }

    // Drop through platforms
    if (self->player.input.down && player->on_platform) {
        player->position.y += 0.2f; // 더 확실하게 통과하도록
        player->on_platform = false;
        player->velocity.y  = 0.0f; // 통과 시 수직 속도 초기화
    }

    // Mining/interaction
    if (self->player.input.mine) { Game_handleMining(self); }

    // Placing
    if (self->player.input.place) { Game_handleBlockPlacement(self); }

    // Interaction
    if (self->player.input.interact) { Game_handleInteraction(self); }

    // Hotbar selection
    if_some((self->player.input.hotbar_selection)(slot)) {
        if (slot < Game_hotbar_size) {
            self->player.data.selected_hotbar_slot = slot;
        }
    }
}

fn_((Game_handleMining(GameState* self))(void)) {
    let cursor_pos = self->player.input.cursor_world_pos;
    let block_x    = (i32)flt_floor(cursor_pos.x);
    let block_y    = (i32)flt_floor(cursor_pos.y);
    if ((block_x < 0 || Game_world_width <= block_x)
        || (block_y < 0 || Game_world_height <= block_y)) { return; }

    // Check distance
    let player   = orelse_((Game_idxEntityById(self, self->player.data.entity_id))(return));
    let dx       = cursor_pos.x - player->position.x;
    let dy       = cursor_pos.y - player->position.y;
    let distance = flt_sqrt((dx * dx) + (dy * dy));
    if (distance > 5.0f) { return; }

    let ux = (usize)block_x;
    let uy = (usize)block_y;

    // Check what layer to mine
    let selected_item = self->player.data.inventory[self->player.data.selected_hotbar_slot];
    let is_hammer     = selected_item.item_type == ItemType_wooden_hammer;
    if (is_hammer) {
        // Hammer removes entity tiles and background
        if (self->world.layer.entity_tiles[uy][ux].type != EntityTileType_none) {
            if_some((Game_removeEntityTile(self, block_x, block_y))(removed_tile_type)) {
                if_(let item_type = Item_entityTileToItem(removed_tile_type), item_type != ItemType_none) {
                    Game_createItemDrop(self, cursor_pos, item_type, 1);
                }
            }
        } else if (self->world.layer.bg_tiles[uy][ux] != BgTile_none) {
            self->world.layer.bg_tiles[uy][ux] = BgTile_none;
        }
    } else {
        // Pickaxe/axe removes blocks
        let block = self->world.layer.fg_tiles[uy][ux];
        if (block == FgTile_air || block == FgTile_bedrock) { return; }

        self->world.layer.block_health[uy][ux] += Item_miningPower(selected_item.item_type);
        if (self->world.layer.block_health[uy][ux] >= Item_blockHardness(block)) {
            if_(let item_type = Item_blockToItem(block), item_type != ItemType_none) {
                Game_createItemDrop(self, cursor_pos, item_type, 1);
            }
            self->world.layer.fg_tiles[uy][ux]     = FgTile_air;
            self->world.layer.block_health[uy][ux] = 0;
        }
    }
}

fn_((Game_handleBlockPlacement(GameState* self))(void)) {
    let cursor_pos = self->player.input.cursor_world_pos;
    let block_x    = (i32)flt_floor(cursor_pos.x);
    let block_y    = (i32)flt_floor(cursor_pos.y);
    if ((block_x < 0 || Game_world_width <= block_x)
        || (block_y < 0 || Game_world_height <= block_y)) { return; }

    // Check distance
    let player   = orelse_((Game_idxEntityById(self, self->player.data.entity_id))(return));
    let dx       = cursor_pos.x - player->position.x;
    let dy       = cursor_pos.y - player->position.y;
    let distance = flt_sqrt((dx * dx) + (dy * dy));
    if (distance > 5.0f) { return; }

    // Get selected item
    let selected_item = &self->player.data.inventory[self->player.data.selected_hotbar_slot];
    if (selected_item->count == 0) { return; }

    // Try to place as entity tile first
    if_(let entity_tile = Item_itemToEntityTile(selected_item->item_type), entity_tile != EntityTileType_none) {
        if (Game_placeEntityTile(self, block_x, block_y, entity_tile)) {
            selected_item->count -= 1;
            if (selected_item->count == 0) {
                selected_item->item_type = ItemType_none;
            }
        }
    } else {
        // Try to place as block
        let ux = (usize)block_x;
        let uy = (usize)block_y;
        if (self->world.layer.fg_tiles[uy][ux] != FgTile_air) { return; }
        if (self->world.layer.entity_tiles[uy][ux].type != EntityTileType_none) { return; }
        if_(let block_type = Item_itemToBlock(selected_item->item_type), block_type != FgTile_air) {
            self->world.layer.fg_tiles[uy][ux] = block_type;
            selected_item->count -= 1;
            if (selected_item->count == 0) {
                selected_item->item_type = ItemType_none;
            }
        }
    }
}

fn_((Game_handleInteraction(GameState* self))(void)) {
    let cursor_pos = self->player.input.cursor_world_pos;
    let block_x    = (i32)flt_floor(cursor_pos.x);
    let block_y    = (i32)flt_floor(cursor_pos.y);

    if ((block_x < 0 || Game_world_width <= block_x)
        || (block_y < 0 || Game_world_height <= block_y)) { return; }

    let ux = (usize)block_x;
    let uy = (usize)block_y;

    let tile = &self->world.layer.entity_tiles[uy][ux];
    if (tile->type == EntityTileType_none) { return; }

    // Find origin tile for interaction
    let origin_tile = &self->world.layer.entity_tiles[tile->origin.y][tile->origin.x];

    switch (tile->type) {
    case EntityTileType_door_closed: {
        // Open door - change type and update all parts
        let_ignore = Game_removeEntityTile(self, tile->origin.x, tile->origin.y);
        let_ignore = Game_placeEntityTile(self, tile->origin.x, tile->origin.y, EntityTileType_door_open);
        break;
    }
    case EntityTileType_door_open: {
        // Close door
        let_ignore = Game_removeEntityTile(self, tile->origin.x, tile->origin.y);
        let_ignore = Game_placeEntityTile(self, tile->origin.x, tile->origin.y, EntityTileType_door_closed);
        break;
    }
    case EntityTileType_chest: {
        // Would open chest inventory
        origin_tile->metadata ^= 1; // Toggle open state
        break;
    }
    case EntityTileType_crafting_table: {
        // Would open crafting interface
        break;
    }
    default:
        break;
    }
}

// ============================================================================
// ENTITY AI
// ============================================================================

fn_((Game_updateEntityAI(GameState* self, f32 delta_time))(void)) {
    let_ignore = delta_time;
    for_(($r(0, self->entities.count))(i) {
        let entity = &self->entities.data[i];
        if (!entity->alive) { continue; }

        switch (entity->entity_type) {
        case EntityType_zombie:
        case EntityType_skeleton: {
            let player = orelse_((Game_idxEntityById(self, self->player.data.entity_id))(return));
            let dx     = player->position.x - entity->position.x;
            if (1.0f < flt_abs(dx)) {
                entity->velocity.x = (0.0f < dx) ? 3.0f : -3.0f;
                if (entity->grounded && flt_abs(entity->velocity.x) < 0.1f) {
                    entity->velocity.y = -8.0f;
                }
            }
        } break;
        default:
            continue;
        }
    });
}

// ============================================================================
// GAME LOGIC UPDATE
// ============================================================================

fn_((Game_update(GameState* self, f32 delta_time))(void)) {
    self->world.state.time_of_day += delta_time * 0.01f;
    if (self->world.state.time_of_day > 1.0f) {
        self->world.state.time_of_day -= 1.0f;
    }
    self->world.state.tick_count += 1;
    Game_processInput(self);
    Game_updatePhysics(self, delta_time);
    Game_updateEntityAI(self, delta_time);
    Game_removeDeadEntities(self);
}

// ============================================================================
// PUBLIC INTERFACE
// ============================================================================

fn_((Game_setInput(GameState* self, InputState input))(void)) {
    self->player.input = input;
}

fn_((Game_getPlayerPosition(GameState* self))(m_V2f32)) {
    return expr_(m_V2f32 $scope) if_some((Game_idxEntityById(self, self->player.data.entity_id))(player)) {
        $break_(player->position);
    } else {
        $break_(m_V2f32_zero);
    } $unscoped_(expr);
}

fn_((Game_getBackgroundTile(GameState* self, usize x, usize y))(BgTile)) {
    return expr_(BgTile $scope) if (Game_world_width <= x || Game_world_height <= y) {
        $break_(BgTile_none);
    } else {
        $break_(self->world.layer.bg_tiles[y][x]);
    } $unscoped_(expr);
}

fn_((Game_getFgTile(GameState* self, usize x, usize y))(FgTile)) {
    return expr_(FgTile $scope) if (Game_world_width <= x || Game_world_height <= y) {
        $break_(FgTile_bedrock);
    } else {
        $break_(self->world.layer.fg_tiles[y][x]);
    } $unscoped_(expr);
}

fn_((Game_getEntityTile(GameState* self, usize x, usize y))(EntityTile)) {
    return expr_(EntityTile $scope) if (Game_world_width <= x || Game_world_height <= y) {
        $break_((EntityTile){ .type = EntityTileType_none, .origin = m_V2i32_zero, .is_origin = false, .metadata = 0 });
    } else {
        $break_(self->world.layer.entity_tiles[y][x]);
    } $unscoped_(expr);
}

fn_((Game_getInventorySlot(GameState* self, usize slot))(ItemStack)) {
    return expr_(ItemStack $scope) if (Game_inventory_size <= slot) {
        $break_((ItemStack){ .item_type = ItemType_none, .count = 0 });
    } else {
        $break_(self->player.data.inventory[slot]);
    } $unscoped_(expr);
}

fn_((Game_getSelectedHotbarSlot(GameState* self))(u8)) {
    return self->player.data.selected_hotbar_slot;
}

fn_((Game_getTimeOfDay(GameState* self))(f32)) {
    return self->world.state.time_of_day;
}

fn_((Game_getEntityCount(GameState* self))(u32)) {
    return self->entities.count;
}

fn_((Game_getEntity(GameState* self, usize index))(O$Entity)) {
    return expr_(O$Entity $scope) if (self->entities.count <= index) {
        $break_(none());
    } else {
        $break_(some(self->entities.data[index]));
    } $unscoped_(expr);
}

// ============================================================================
// RENDERING SYSTEM
// ============================================================================

fn_((Render_init(mem_Allocator allocator))(Renderer*)) {
    let self = u_castP$((Renderer*)(
        catch_((mem_Allocator_create(allocator, typeInfo$(Renderer)))(
            $ignore, claim_unreachable
        ))
    ));

    // Create window
    let window_config = (engine_Window_Config){
        .allocator     = some(allocator),
        .rect_size     = { .x = Render_screen_width, .y = Render_screen_height },
        .default_color = some(Color_black),
        .title         = some(u8_l("Terraria-like Game")),
    };
    self->window = catch_((engine_Window_init(window_config))(
        $ignore, claim_unreachable
    ));

    // Create canvas
    let canvas_config = (engine_Canvas_Config){
        .allocator     = some(allocator),
        .width         = Render_screen_width,
        .height        = Render_screen_height,
        .default_color = some(Color_black),
        .type          = some(engine_CanvasType_rgba),
    };
    self->canvas = catch_((engine_Canvas_init(canvas_config))(
        $ignore, claim_unreachable
    ));

    // Add canvas to window
    engine_Window_appendView(
        self->window,
        (engine_CanvasView_Config){
            .canvas      = self->canvas,
            .pos         = { .x = 0, .y = 0 },
            .size        = { .x = Render_screen_width, .y = Render_screen_height },
            .scale       = { .x = 1.0f, .y = 1.0f },
            .resizable_x = false,
            .resizable_y = false,
            .visible     = true,
        }
    );

    // Create input system
    self->input = catch_((engine_Input_init(allocator))(
        $ignore, claim_unreachable
    ));

    // Bind engine core (Vt100 terminal backend)
    self->core = catch_((engine_core_VT100_init((engine_core_VT100_Config){
        .allocator = some(allocator),
        .window    = self->window,
        .input     = self->input,
    }))(
        $ignore, claim_unreachable
    ));

    // Initialize camera
    self->camera = (Camera){
        .position = { .x = Game_world_width / 2.0f, .y = Game_world_height / 2.0f },
        .zoom     = 1.0f,
    };

    self->allocator = allocator;

    return self;
}

fn_((Render_fini(Renderer* self, mem_Allocator allocator))(void)) {
    if (self->core) { engine_core_VT100_fini(self->core); }
    if (self->input) { engine_Input_fini(self->input); }
    if (self->canvas) { engine_Canvas_fini(self->canvas); }
    if (self->window) { engine_Window_fini(self->window); }
    mem_Allocator_destroy(allocator, u_anyP(self));
}

fn_((Render_worldToScreen(const Camera* camera, m_V2f32 world_pos))(m_V2i32)) {
    let screen_x = as$(i32)(world_pos.x - camera->position.x) * camera->zoom + (Render_screen_width / 2.0f);
    let screen_y = as$(i32)(world_pos.y - camera->position.y) * camera->zoom + (Render_screen_height / 2.0f);
    return (m_V2i32){ .x = as$(i32)(screen_x), .y = as$(i32)(screen_y) };
}

fn_((Render_screenToWorld(const Camera* camera, m_V2i32 screen_pos))(m_V2f32)) {
    let world_x = (as$(f32)(screen_pos.x) - (Render_screen_width / 2.0f)) / camera->zoom + camera->position.x;
    let world_y = (as$(f32)(screen_pos.y) - (Render_screen_height / 2.0f)) / camera->zoom + camera->position.y;
    return (m_V2f32){ .x = world_x, .y = world_y };
}

fn_((Render_updateCamera(Renderer* self, const GameState* game))(void)) {
    // Follow player
    if_some((Game_idxEntityById(game, game->player.data.entity_id))(player)) {
        self->camera.position = player->position;
    }
}

fn_((Render_world(GameState* self))(void)) {
    let canvas = self->renderer.canvas;
    let camera = &self->renderer.camera;

    // Clear canvas with sky color
    let sky_color = literal_Color_fromOpaque(135, 206, 235); // Sky blue
    engine_Canvas_clear(canvas, some$((O$Color)sky_color));

    // Calculate visible world bounds
    let world_min = Render_screenToWorld(camera, (m_V2i32){ .x = 0, .y = 0 });
    let world_max = Render_screenToWorld(camera, (m_V2i32){ .x = Render_screen_width, .y = Render_screen_height });

    var min_x = as$(i32)(flt_floor(world_min.x));
    var max_x = as$(i32)(flt_ceil(world_max.x));
    var min_y = as$(i32)(flt_floor(world_min.y));
    var max_y = as$(i32)(flt_ceil(world_max.y));

    // Clamp to world bounds
    min_x = (min_x < 0) ? 0 : min_x;
    max_x = (max_x >= Game_world_width) ? (Game_world_width - 1) : max_x;
    min_y = (min_y < 0) ? 0 : min_y;
    max_y = (max_y >= Game_world_height) ? (Game_world_height - 1) : max_y;

    // Render background tiles
    for_(($r(min_y, $incl(max_y)))(y) {
        for_(($r(min_x, $incl(max_x)))(x) {
            let bg_tile = self->world.layer.bg_tiles[y][x];
            if (bg_tile != BgTile_none) {
                let screen_pos = Render_worldToScreen(camera, (m_V2f32){ .x = as$(f32)(x), .y = as$(f32)(y) });
                if ((0 <= screen_pos.x && screen_pos.x < Render_screen_width) && (0 <= screen_pos.y && screen_pos.y < Render_screen_height)) {
                    let color = Render_getBgTileColor(bg_tile);
                    engine_Canvas_drawPixel(canvas, screen_pos.x, screen_pos.y, color);
                }
            }
        });
    });

    // Render foreground tiles
    for_(($r(min_y, $incl(max_y)))(y) {
        for_(($r(min_x, $incl(max_x)))(x) {
            let fg_tile = self->world.layer.fg_tiles[y][x];
            if (fg_tile != FgTile_air) {
                let screen_pos = Render_worldToScreen(camera, (m_V2f32){ .x = as$(f32)(x), .y = as$(f32)(y) });
                if ((0 <= screen_pos.x && screen_pos.x < Render_screen_width) && (0 <= screen_pos.y && screen_pos.y < Render_screen_height)) {
                    let color = Render_getFgTileColor(fg_tile);
                    engine_Canvas_drawPixel(canvas, screen_pos.x, screen_pos.y, color);
                }
            }
        });
    });

    // Render entity tiles
    for_(($r(min_y, $incl(max_y)))(y) {
        for_(($r(min_x, $incl(max_x)))(x) {
            let entity_tile = self->world.layer.entity_tiles[y][x];
            if (entity_tile.type != EntityTileType_none && entity_tile.is_origin) {
                let props = EntityTile_props(entity_tile.type);
                let color = Render_getEntityTileColor(entity_tile.type);

                // 타일 크기에 따라 렌더링
                for_(($r(0, props.size.y))(dy) {
                    for_(($r(0, props.size.x))(dx) {
                        let tile_x     = x + dx;
                        let tile_y     = y + dy;
                        let screen_pos = Render_worldToScreen(camera, (m_V2f32){ .x = as$(f32)(tile_x), .y = as$(f32)(tile_y) });
                        if ((0 <= screen_pos.x && screen_pos.x < Render_screen_width) && (0 <= screen_pos.y && screen_pos.y < Render_screen_height)) {
                            engine_Canvas_drawPixel(canvas, screen_pos.x, screen_pos.y, color);
                        }
                    });
                });
            }
        });
    });
}

fn_((Render_player(GameState* self))(void)) {
    let canvas = self->renderer.canvas;
    let camera = &self->renderer.camera;

    // 플레이어 엔티티 찾기
    let player_entity = orelse_((Game_idxMutEntityById(self, self->player.data.entity_id))(return));

    let color = literal_Color_fromOpaque(255, 0, 0); // 빨간색 플레이어

    // AABB 계산 (월드 좌표계에서)
    let player_left   = player_entity->position.x - (player_entity->size.x / 2.0f);
    let player_right  = player_entity->position.x + (player_entity->size.x / 2.0f);
    let player_bottom = player_entity->position.y;                         // 발 부분
    let player_top    = player_entity->position.y + player_entity->size.y; // 머리 부분

    // AABB를 화면 좌표로 변환
    let screen_left   = Render_worldToScreen(camera, (m_V2f32){ .x = player_left, .y = player_bottom });
    let screen_right  = Render_worldToScreen(camera, (m_V2f32){ .x = player_right, .y = player_bottom });
    let screen_bottom = Render_worldToScreen(camera, (m_V2f32){ .x = player_entity->position.x, .y = player_bottom });
    let screen_top    = Render_worldToScreen(camera, (m_V2f32){ .x = player_entity->position.x, .y = player_top });

    // 화면 경계 내에서 AABB 렌더링
    let min_x = prim_min(screen_left.x, screen_right.x);
    let max_x = prim_max(screen_left.x, screen_right.x);
    let min_y = prim_min(screen_bottom.y, screen_top.y);
    let max_y = prim_max(screen_bottom.y, screen_top.y);

    for_(($r(min_y, $incl(max_y)))(y) {
        for_(($r(min_x, $incl(max_x)))(x) {
            engine_Canvas_drawPixel(canvas, as$(i32)(x), as$(i32)(y), color);
        });
    });
}

fn_((Render_entities(GameState* self))(void)) {
    let canvas = self->renderer.canvas;
    let camera = &self->renderer.camera;

    for_(($r(0, self->entities.count))(i) {
        let entity = &self->entities.data[i];
        if (!entity->alive) { continue; }

        // 플레이어는 별도로 렌더링하므로 스킵
        if (entity->entity_type == EntityType_player) { continue; }

        let color = Render_getEntityColor(entity->entity_type);

        // 엔티티 크기에 따라 렌더링
        let size_pixels_x = as$(i32)(entity->size.x* Render_tile_size);
        let size_pixels_y = as$(i32)(entity->size.y* Render_tile_size);

        for_(($r(0, size_pixels_y))(dy) {
            for_(($r(0, size_pixels_x))(dx) {
                let world_pos = ((m_V2f32){
                    .x = entity->position.x - (entity->size.x / 2.0f) + (as$(f32)(dx) / Render_tile_size),
                    .y = entity->position.y - (entity->size.y / 2.0f) + (as$(f32)(dy) / Render_tile_size)
                });
                let screen_pos = Render_worldToScreen(camera, world_pos);
                if ((0 <= screen_pos.x && screen_pos.x < Render_screen_width) && (0 <= screen_pos.y && screen_pos.y < Render_screen_height)) {
                    engine_Canvas_drawPixel(canvas, screen_pos.x, screen_pos.y, color);
                }
            });
        });
    });
}

fn_((Render_ui(GameState* self))(void)) {
    // 1. 핫바 UI (화면 하단)
    Render_hotbar(self);
    // 2. 선택된 아이템 정보 (화면 상단)
    Render_selectedItemInfo(self);
    // 3. 마우스 커서 위치 표시
    Render_cursor(self);
    // 4. 플레이어 상태 정보 (화면 좌상단)
    Render_playerStatus(self);
}

fn_((Render_hotbar(GameState* self))(void)) {
    let canvas        = self->renderer.canvas;
    let hotbar_height = 5;
    let hotbar_y      = Render_screen_height - hotbar_height;
    let slot_size     = 3;

    // 핫바 배경
    engine_Canvas_drawRect(canvas, 0, hotbar_y, Render_screen_width - 1, Render_screen_height - 1, literal_Color_fromOpaque(50, 50, 50));
    // 핫바 슬롯들
    for_(($r(0, Game_hotbar_size))(i) {
        let slot_x = as$(i32)(i* slot_size);
        let slot_y = hotbar_y + 1;
        // 슬롯 배경
        let slot_bg_color = literal_Color_fromOpaque(30, 30, 30);
        engine_Canvas_drawRect(canvas, slot_x, slot_y, slot_x + slot_size - 1, slot_y + slot_size - 1, slot_bg_color);
        // 아이템이 있으면 표시
        let item = self->player.data.inventory[i];
        if (item.item_type != ItemType_none && item.count > 0) {
            let item_color = literal_Color_fromOpaque(150, 150, 150);
            engine_Canvas_drawRect(canvas, slot_x + 1, slot_y + 1, slot_x + slot_size - 2, slot_y + slot_size - 2, item_color);
        }
        // 선택된 슬롯 강조
        if (i == self->player.data.selected_hotbar_slot) {
            let selection_color = literal_Color_fromOpaque(255, 255, 0);
            engine_Canvas_drawRect(canvas, slot_x, slot_y, slot_x + slot_size - 1, slot_y + slot_size - 1, selection_color);
        }
    });
}

fn_((Render_selectedItemInfo(GameState* self))(void)) {
    let canvas        = self->renderer.canvas;
    let selected_slot = self->player.data.selected_hotbar_slot;
    let item          = self->player.data.inventory[selected_slot];

    if (item.item_type != ItemType_none && item.count > 0) {
        // 아이템 정보 배경
        let info_x      = 1;
        let info_y      = 1;
        let info_width  = 20;
        let info_height = 4;
        engine_Canvas_drawRect(canvas, info_x, info_y, info_x + info_width, info_y + info_height, literal_Color_fromOpaque(0, 0, 0));
        // 아이템 타입 표시 (간단한 텍스트 대신 색상으로)
        let item_color = literal_Color_fromOpaque(200, 200, 200);
        engine_Canvas_drawRect(canvas, info_x + 1, info_y + 1, info_x + 3, info_y + 3, item_color);
        // 아이템 개수 표시 (간단한 점으로)
        for (usize i = 0; i < item.count && i < 10; ++i) {
            let count_x = info_x + 5 + as$(i32)(i);
            let count_y = info_y + 2;
            engine_Canvas_drawPixel(canvas, count_x, count_y, literal_Color_fromOpaque(255, 255, 0));
        }
    }
}

fn_((Render_cursor(GameState* self))(void)) {
    let canvas = self->renderer.canvas;
    let input  = self->renderer.input;

    // 마우스 위치 가져오기
    let mouse_pos = engine_Mouse_getPos(input->mouse);
    let mouse_x   = mouse_pos.x;
    let mouse_y   = mouse_pos.y;

    // 커서 표시 (십자가 모양)
    let cursor_color = literal_Color_fromOpaque(255, 255, 255);
    if (0 <= mouse_x && mouse_x < Render_screen_width && 0 <= mouse_y && mouse_y < Render_screen_height) {
        // 수평선
        engine_Canvas_drawPixel(canvas, mouse_x - 1, mouse_y, cursor_color);
        engine_Canvas_drawPixel(canvas, mouse_x + 1, mouse_y, cursor_color);
        // 수직선
        engine_Canvas_drawPixel(canvas, mouse_x, mouse_y - 1, cursor_color);
        engine_Canvas_drawPixel(canvas, mouse_x, mouse_y + 1, cursor_color);
        // 중심점
        engine_Canvas_drawPixel(canvas, mouse_x, mouse_y, literal_Color_fromOpaque(255, 0, 0));
    }
}

fn_((Render_playerStatus(GameState* self))(void)) {
    let canvas = self->renderer.canvas;

    if_some((Game_idxEntityById(self, self->player.data.entity_id))(player)) {
        let status_x = 1;
        let status_y = 6;

        // 체력 바
        let health_percent = as$(f32)(player->health) / as$(f32)(player->max_health);
        let health_width   = as$(i32)(health_percent * 20.0f);
        engine_Canvas_drawRect(canvas, status_x, status_y, status_x + 20, status_y + 2, literal_Color_fromOpaque(100, 0, 0));
        engine_Canvas_drawRect(canvas, status_x, status_y, status_x + health_width, status_y + 2, Color_red);
    }
}

fn_((Game_render(GameState* self))(void)) {
    // Process window events
    let_ignore = engine_Window_update(self->renderer.window);

    Render_updateCamera(&self->renderer, self);
    Render_world(self);
    Render_entities(self);
    Render_player(self);
    Render_ui(self);

    // Present to screen
    engine_Window_present(self->renderer.window);
}

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================

fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    let allocator = heap_Page_allocator(&(heap_Page){});
    let game      = Game_init(allocator);
    defer_(Game_fini(game, allocator));

    io_stream_print(u8_l("Terraria-like game with dual-layer system initialized!\n"));
    io_stream_print(u8_l("World size: {:d}x{:d}\n"), Game_world_width, Game_world_height);
    io_stream_print(u8_l("Features:\n"));
    io_stream_print(u8_l("- Background layer (walls)\n"));
    io_stream_print(u8_l("- Block layer (terrain)\n"));
    io_stream_print(u8_l("- Entity tile layer (furniture, doors, platforms)\n"));
    io_stream_print(u8_l("- Multi-tile objects support\n"));
    io_stream_print(u8_l("- Platform mechanics (can stand on, pass through)\n"));
    io_stream_print(u8_l("- Interactive tiles (doors, chests)\n"));

    // Simulate placing some furniture
    Game_placeEntityTile(game, 250, 95, EntityTileType_platform);
    Game_placeEntityTile(game, 251, 95, EntityTileType_platform);
    Game_placeEntityTile(game, 252, 95, EntityTileType_platform);
    Game_placeEntityTile(game, 253, 95, EntityTileType_platform);
    Game_placeEntityTile(game, 254, 95, EntityTileType_platform);
    Game_placeEntityTile(game, 255, 95, EntityTileType_platform);
    Game_placeEntityTile(game, 256, 95, EntityTileType_platform);
    Game_placeEntityTile(game, 257, 95, EntityTileType_platform);
    Game_placeEntityTile(game, 258, 95, EntityTileType_platform);
    Game_placeEntityTile(game, 259, 95, EntityTileType_platform);
    Game_placeEntityTile(game, 248, 98, EntityTileType_door_closed);
    Game_placeEntityTile(game, 251, 96, EntityTileType_crafting_table);
    Game_placeEntityTile(game, 254, 98, EntityTileType_chest);

    // Game loop
    io_stream_print(u8_l("Starting game loop... Press ESC to quit\n"));

    // Initialize timing variables
    let target_fps        = 60.0;
    let target_frame_time = time_Duration_fromSecs$f64(1.0 / target_fps);
    var prev_frame_time   = time_Instant_now();

    var running = true;
    while (running) {
        // 1) Capture the start of the frame
        let curr_frame_time = time_Instant_now();

        // 2) Compute delta time
        let elapsed_time = time_Instant_durationSince(curr_frame_time, prev_frame_time);
        let dt           = time_Duration_asSecs$f64(elapsed_time);

        // Handle input
        if (engine_Keyboard_pressed(game->renderer.input->keyboard, engine_KeyCode_esc)) {
            running = false;
        }

        // Simple input handling (for demonstration)
        game->player.input = (InputState){
            .move_left        = engine_Keyboard_held(game->renderer.input->keyboard, engine_KeyCode_a),
            .move_right       = engine_Keyboard_held(game->renderer.input->keyboard, engine_KeyCode_d),
            .jump             = engine_Keyboard_held(game->renderer.input->keyboard, engine_KeyCode_space),
            .down             = engine_Keyboard_held(game->renderer.input->keyboard, engine_KeyCode_s),
            .mine             = engine_Mouse_held(game->renderer.input->mouse, engine_MouseButton_left),
            .place            = engine_Mouse_held(game->renderer.input->mouse, engine_MouseButton_right),
            .interact         = engine_Keyboard_pressed(game->renderer.input->keyboard, engine_KeyCode_e),
            .cursor_world_pos = { .x = 250.0f, .y = 100.0f }, // TODO: Convert mouse position to world coordinates
            .hotbar_selection = none(),
        };

        Game_update(game, as$(f32)(dt));
        Game_render(game);

        // Display FPS
        let fps = (0.0 < dt) ? (1.0 / dt) : 9999.0;
        io_stream_print(u8_l("\033[H\033[40;37m")); // Move cursor to top left
        io_stream_print(u8_l("\rFPS: {:6.2f}"), fps);

        // 3) Measure how long the update+render actually took
        let now        = time_Instant_now();
        let frame_used = time_Instant_durationSince(now, curr_frame_time);

        // 4) Sleep for remaining time to maintain target FPS
        if_some((time_Duration_subChkd(target_frame_time, frame_used))(leftover)) {
            time_sleep(leftover);
        }

        prev_frame_time = curr_frame_time;
    }

    return_ok({});
} $unguarded_(fn);

TEST_only(
    static GameState s_game = cleared();

    TEST_fn_("FgTile solid properties" $scope) {
        // Test that leaves are not solid (passable)
        try_(TEST_expect(!FgTile_isSolid(FgTile_leaves)));

        // Test that air is not solid
        try_(TEST_expect(!FgTile_isSolid(FgTile_air)));

        // Test that solid blocks are solid
        try_(TEST_expect(FgTile_isSolid(FgTile_wood)));
        try_(TEST_expect(FgTile_isSolid(FgTile_stone)));
        try_(TEST_expect(FgTile_isSolid(FgTile_dirt)));
        try_(TEST_expect(FgTile_isSolid(FgTile_grass)));
        try_(TEST_expect(FgTile_isSolid(FgTile_sand)));
        try_(TEST_expect(FgTile_isSolid(FgTile_coal_ore)));
        try_(TEST_expect(FgTile_isSolid(FgTile_iron_ore)));
        try_(TEST_expect(FgTile_isSolid(FgTile_gold_ore)));
        try_(TEST_expect(FgTile_isSolid(FgTile_diamond_ore)));
        try_(TEST_expect(FgTile_isSolid(FgTile_bedrock)));

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Collision system with leaves" $scope) {
        // Create a test game state
        s_game = cleared$(GameState);

        // Set up a world with leaves at position (10, 10)
        s_game.world.layer.fg_tiles[10][10] = FgTile_leaves;

        // Test collision at the same position
        let test_pos         = (m_V2f32){ .x = 10.0f, .y = 10.0f };
        let test_size        = (m_V2f32){ .x = 1.0f, .y = 1.0f };
        let collision_result = Game_checkCollision(&s_game, test_pos, test_size, false);

        // Leaves should not cause collision (should be passable)
        try_(TEST_expect(!collision_result.collided));

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Collision system with solid blocks" $scope) {
        // Create a test game state
        s_game = cleared$(GameState);

        // Set up a world with wood at position (10, 10)
        s_game.world.layer.fg_tiles[10][10] = FgTile_wood;

        // Test collision at the same position
        let test_pos         = (m_V2f32){ .x = 10.0f, .y = 10.0f };
        let test_size        = (m_V2f32){ .x = 1.0f, .y = 1.0f };
        let collision_result = Game_checkCollision(&s_game, test_pos, test_size, false);

        // Wood should cause collision (should be solid)
        try_(TEST_expect(collision_result.collided));
        try_(TEST_expect(collision_result.is_obstacle));

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Collision system boundary checks" $scope) {
        // Create a test game state
        s_game = cleared$(GameState);

        // Test collision at world boundaries
        let test_size = (m_V2f32){ .x = 1.0f, .y = 1.0f };

        // Test negative coordinates (should collide)
        let neg_pos       = (m_V2f32){ .x = -1.0f, .y = -1.0f };
        let neg_collision = Game_checkCollision(&s_game, neg_pos, test_size, false);
        try_(TEST_expect(neg_collision.collided));

        // Test coordinates beyond world bounds (should collide)
        let out_pos       = (m_V2f32){ .x = 500.0f, .y = 200.0f };
        let out_collision = Game_checkCollision(&s_game, out_pos, test_size, false);
        try_(TEST_expect(out_collision.collided));

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Tree destruction mechanics" $guard) {
        s_game = cleared$(GameState);

        // Set up a tree structure
        let tree_x = 10;
        let tree_y = 50;

        // Create tree trunk entity tile
        s_game.world.layer.entity_tiles[tree_y][tree_x] = (EntityTile){
            .type      = EntityTileType_tree_trunk,
            .origin    = { .x = tree_x, .y = tree_y },
            .is_origin = true,
            .metadata  = 1
        };

        // Create leaves above
        s_game.world.layer.fg_tiles[tree_y - 1][tree_x] = FgTile_leaves;
        s_game.world.layer.fg_tiles[tree_y - 2][tree_x] = FgTile_leaves;

        // Record initial entity count
        let initial_count = s_game.entities.count;

        // Remove the tree trunk
        let removed_type = Game_removeEntityTile(&s_game, tree_x, tree_y);

        // Should return the removed type
        try_(TEST_expect(isSome(removed_type)));
        if_some(removed_type, type) {
            try_(TEST_expect(type == EntityTileType_tree_trunk));
        }

        // Should create items (entities should increase)
        try_(TEST_expect(s_game.entities.count > initial_count));

        // Tree trunk should be removed
        try_(TEST_expect(s_game.world.layer.entity_tiles[tree_y][tree_x].type == EntityTileType_none));

        return_ok({});
    } $unguarded_TEST_fn;

    TEST_fn_("Entity tile placement and removal" $guard) {
        s_game = cleared$(GameState);

        let x = 20;
        let y = 30;

        // Initially should be empty
        try_(TEST_expect(s_game.world.layer.entity_tiles[y][x].type == EntityTileType_none));

        // Place a tree trunk
        s_game.world.layer.entity_tiles[y][x] = (EntityTile){
            .type      = EntityTileType_tree_trunk,
            .origin    = { .x = x, .y = y },
            .is_origin = true,
            .metadata  = 1
        };

        // Should now have the tree trunk
        try_(TEST_expect(s_game.world.layer.entity_tiles[y][x].type == EntityTileType_tree_trunk));

        // Remove it
        let removed = Game_removeEntityTile(&s_game, x, y);
        try_(TEST_expect(isSome(removed)));

        // Should be empty again
        try_(TEST_expect(s_game.world.layer.entity_tiles[y][x].type == EntityTileType_none));

        return_ok({});
    } $unguarded_TEST_fn;

    TEST_fn_("Camera coordinate conversion" $scope) {
        let camera = (Camera){
            .position = { .x = 250.0f, .y = 100.0f },
            .zoom     = 1.0f
        };

        // Test world to screen conversion
        let world_pos  = (m_V2f32){ .x = 250.0f, .y = 100.0f };
        let screen_pos = Render_worldToScreen(&camera, world_pos);

        // Center of world should map to center of screen (assuming 500x200 screen)
        try_(TEST_expect(screen_pos.x == 250));
        try_(TEST_expect(screen_pos.y == 100));

        // Test screen to world conversion
        let screen_pos2 = (m_V2i32){ .x = 250, .y = 100 };
        let world_pos2  = Render_screenToWorld(&camera, screen_pos2);

        // Should convert back to original world position
        try_(TEST_expect(world_pos2.x == 250.0f));
        try_(TEST_expect(world_pos2.y == 100.0f));

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Tile color rendering" $scope) {
        // Test that different tile types have different colors
        let air_color    = Render_getFgTileColor(FgTile_air);
        let wood_color   = Render_getFgTileColor(FgTile_wood);
        let leaves_color = Render_getFgTileColor(FgTile_leaves);

        // Air should be transparent/black
        try_(TEST_expect(air_color.r == 0 && air_color.g == 0 && air_color.b == 0));

        // Wood and leaves should have different colors
        let wood_different = (wood_color.r != leaves_color.r) || (wood_color.g != leaves_color.g) || (wood_color.b != leaves_color.b);
        try_(TEST_expect(wood_different));

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Entity tile color rendering" $scope) {
        // Test that different entity tile types have different colors
        let none_color  = Render_getEntityTileColor(EntityTileType_none);
        let trunk_color = Render_getEntityTileColor(EntityTileType_tree_trunk);
        let plank_color = Render_getEntityTileColor(EntityTileType_wood_plank);

        // None should be black/transparent
        try_(TEST_expect(none_color.r == 0 && none_color.g == 0 && none_color.b == 0));

        // Tree trunk and wood plank should have different colors
        let trunk_different = (trunk_color.r != plank_color.r) || (trunk_color.g != plank_color.g) || (trunk_color.b != plank_color.b);
        try_(TEST_expect(trunk_different));

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Tree system entity types" $scope) {
        // Test tree trunk properties
        let trunk_props = EntityTile_props(EntityTileType_tree_trunk);
        try_(TEST_expect(trunk_props.obstacle == true));
        try_(TEST_expect(trunk_props.interactive == true));
        try_(TEST_expect(trunk_props.size.x == 1));
        try_(TEST_expect(trunk_props.size.y == 1));

        // Test wood plank properties
        let plank_props = EntityTile_props(EntityTileType_wood_plank);
        try_(TEST_expect(plank_props.obstacle == true));
        try_(TEST_expect(plank_props.interactive == true));
        try_(TEST_expect(plank_props.size.x == 1));
        try_(TEST_expect(plank_props.size.y == 1));

        // Test that tree trunk and wood plank are different types
        try_(TEST_expect(EntityTileType_tree_trunk != EntityTileType_wood_plank));

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Tree system item conversion" $scope) {
        // Test that tree trunk converts to appropriate item
        let trunk_item = Item_entityTileToItem(EntityTileType_tree_trunk);
        try_(TEST_expect(trunk_item != ItemType_none));

        // Test that wood plank converts to appropriate item
        let plank_item = Item_entityTileToItem(EntityTileType_wood_plank);
        try_(TEST_expect(plank_item != ItemType_none));

        // Test that they convert to different items
        try_(TEST_expect(trunk_item != plank_item));

        return_ok({});
    } $unscoped_(TEST_fn);

    // ============================================================================
    // BUG REPRODUCTION AND FIX VERIFICATION TESTS
    // ============================================================================

    TEST_fn_("Player collision fix - no teleport to origin" $scope) {
        s_game = cleared$(GameState);

        // Create a player entity
        let player_entity = (Entity){
            .id          = 1,
            .entity_type = EntityType_player,
            .position    = { .x = 10.0f, .y = 10.0f },
            .velocity    = { .x = 5.0f, .y = 0.0f },
            .size        = { .x = 1.0f, .y = 1.0f },
            .health      = 100,
            .max_health  = 100,
            .alive       = true,
            .grounded    = false,
            .on_platform = false,
        };

        // Add player to game
        if_some(Game_appendEntity(&s_game, player_entity), entity_id) {
            s_game.player.data.entity_id = entity_id;
        }

        // Place a solid block in front of player
        s_game.world.layer.fg_tiles[10][15] = FgTile_stone;

        // Simulate movement that would cause collision
        let player     = orelse_((Game_idxMutEntityById(&s_game, s_game.player.data.entity_id))(return_ok({})));
        let original_x = player->position.x;

        // Apply horizontal movement that should collide
        Game_moveWithCollision(&s_game, player, 0.1f);

        // Player should not be teleported to origin (0,0)
        try_(TEST_expect(player->position.x != 0.0f));
        // Player should be stopped by the wall, not teleported
        try_(TEST_expect(player->position.x >= original_x));

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Tree trunk not acting as platform" $scope) {
        s_game = cleared$(GameState);

        // Create a player entity above a tree trunk
        let player_entity = (Entity){
            .id          = 1,
            .entity_type = EntityType_player,
            .position    = { .x = 10.0f, .y = 12.0f }, // Above tree trunk
            .velocity    = { .x = 0.0f, .y = 2.0f },   // Falling down
            .size        = { .x = 1.0f, .y = 1.0f },
            .health      = 100,
            .max_health  = 100,
            .alive       = true,
            .grounded    = false,
            .on_platform = false,
        };

        // Add player to game
        if_some(Game_appendEntity(&s_game, player_entity), entity_id) {
            s_game.player.data.entity_id = entity_id;
        }

        // Place a tree trunk below player
        s_game.world.layer.entity_tiles[10][10] = (EntityTile){
            .type      = EntityTileType_tree_trunk,
            .origin    = { .x = 10, .y = 10 },
            .is_origin = true,
            .metadata  = 1
        };

        // Simulate falling onto tree trunk
        let player     = orelse_((Game_idxMutEntityById(&s_game, s_game.player.data.entity_id))(return_ok({})));
        let original_y = player->position.y;

        // Apply vertical movement (falling)
        Game_moveWithCollision(&s_game, player, 0.1f);

        // Check collision result
        let collision_result = Game_checkCollision(&s_game, player->position, player->size, true);

        // Tree trunk should be obstacle, not a platform
        if (collision_result.collided) {
            try_(TEST_expect(collision_result.is_obstacle == true));
            try_(TEST_expect(collision_result.is_platform == false));
        }

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Player rendering verification" $scope) {
        s_game = cleared$(GameState);

        // Create a player entity
        let player_entity = (Entity){
            .id          = 1,
            .entity_type = EntityType_player,
            .position    = { .x = 250.0f, .y = 100.0f }, // Center of world
            .velocity    = { .x = 0.0f, .y = 0.0f },
            .size        = { .x = 1.0f, .y = 1.0f },
            .health      = 100,
            .max_health  = 100,
            .alive       = true,
            .grounded    = false,
            .on_platform = false,
        };

        // Add player to game
        if_some(Game_appendEntity(&s_game, player_entity), entity_id) {
            s_game.player.data.entity_id = entity_id;
        }

        // Verify player entity exists and is alive
        let player = orelse_((Game_idxMutEntityById(&s_game, s_game.player.data.entity_id))(return_ok({})));
        try_(TEST_expect(player->alive == true));
        try_(TEST_expect(player->entity_type == EntityType_player));
        try_(TEST_expect(player->position.x == 250.0f));
        try_(TEST_expect(player->position.y == 100.0f));

        // Verify player has proper size for rendering
        try_(TEST_expect(player->size.x > 0.0f));
        try_(TEST_expect(player->size.y > 0.0f));

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Leaf collision passability" $scope) {
        s_game = cleared$(GameState);

        // Create a player entity
        let player_entity = (Entity){
            .id          = 1,
            .entity_type = EntityType_player,
            .position    = { .x = 10.0f, .y = 10.0f },
            .velocity    = { .x = 1.0f, .y = 0.0f }, // Moving right
            .size        = { .x = 1.0f, .y = 1.0f },
            .health      = 100,
            .max_health  = 100,
            .alive       = true,
            .grounded    = false,
            .on_platform = false,
        };

        // Add player to game
        if_some(Game_appendEntity(&s_game, player_entity), entity_id) {
            s_game.player.data.entity_id = entity_id;
        }

        // Place leaves in front of player
        s_game.world.layer.fg_tiles[10][12] = FgTile_leaves;

        // Test collision with leaves
        let collision_result = Game_checkCollision(&s_game, (m_V2f32){ .x = 12.0f, .y = 10.0f }, (m_V2f32){ .x = 1.0f, .y = 1.0f }, false);

        // Leaves should not cause collision (should be passable)
        try_(TEST_expect(!collision_result.collided));

        // Simulate movement through leaves
        let player     = orelse_((Game_idxMutEntityById(&s_game, s_game.player.data.entity_id))(return_ok({})));
        let original_x = player->position.x;

        Game_moveWithCollision(&s_game, player, 0.1f);

        // Player should be able to move through leaves
        try_(TEST_expect(player->position.x > original_x));

        return_ok({});
    } $unscoped_(TEST_fn);

    TEST_fn_("Tree destruction cascade effect" $scope) {
        s_game = cleared$(GameState);

        // Set up a tree structure with trunk and leaves above
        let tree_x = 10;
        let tree_y = 50;

        // Create tree trunk entity tile
        s_game.world.layer.entity_tiles[tree_y][tree_x] = (EntityTile){
            .type      = EntityTileType_tree_trunk,
            .origin    = { .x = tree_x, .y = tree_y },
            .is_origin = true,
            .metadata  = 1
        };

        // Create leaves above the trunk
        s_game.world.layer.fg_tiles[tree_y - 1][tree_x] = FgTile_leaves;
        s_game.world.layer.fg_tiles[tree_y - 2][tree_x] = FgTile_leaves;
        s_game.world.layer.fg_tiles[tree_y - 3][tree_x] = FgTile_leaves;

        // Record initial state
        let initial_leaves_count = 0;
        for (usize y = tree_y - 3; y <= tree_y - 1; ++y) {
            if (s_game.world.layer.fg_tiles[y][tree_x] == FgTile_leaves) {
                // Count leaves
            }
        }

        let initial_entity_count = s_game.entities.count;

        // Remove the tree trunk (should trigger cascade destruction)
        let removed_type = Game_removeEntityTile(&s_game, tree_x, tree_y);

        // Should return the removed type
        try_(TEST_expect(isSome(removed_type)));
        if_some(removed_type, type) {
            try_(TEST_expect(type == EntityTileType_tree_trunk));
        }

        // Should create items (entities should increase)
        try_(TEST_expect(s_game.entities.count > initial_entity_count));

        // Tree trunk should be removed
        try_(TEST_expect(s_game.world.layer.entity_tiles[tree_y][tree_x].type == EntityTileType_none));

        // Leaves above should also be destroyed (cascade effect)
        try_(TEST_expect(s_game.world.layer.fg_tiles[tree_y - 1][tree_x] == FgTile_air));
        try_(TEST_expect(s_game.world.layer.fg_tiles[tree_y - 2][tree_x] == FgTile_air));
        try_(TEST_expect(s_game.world.layer.fg_tiles[tree_y - 3][tree_x] == FgTile_air));

        return_ok({});
    } $unscoped_(TEST_fn);
);
