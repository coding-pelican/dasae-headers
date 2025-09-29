#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

// Map dimensions
#define MAP_WIDTH  40
#define MAP_HEIGHT 20
#define MIN_TEMP   -50.0
#define MAX_TEMP   50.0
#define MAX_PLATES 8

// ANSI color codes
#define RESET       "\033[0m"
#define CLEAR       "\033[2J\033[H"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"

// Terrain types (now determined by altitude + conditions)
typedef enum {
    TERRAIN_DEEP_OCEAN,
    TERRAIN_OCEAN,
    TERRAIN_SHALLOW_WATER,
    TERRAIN_LAKE, // New: inland water
    TERRAIN_BEACH,
    TERRAIN_GRASSLAND,
    TERRAIN_FOREST,
    TERRAIN_HILL,
    TERRAIN_MOUNTAIN,
    TERRAIN_PEAK, // New: highest mountains
    TERRAIN_SNOW,
    TERRAIN_DESERT,
    TERRAIN_TUNDRA,
    TERRAIN_VOLCANIC,
    TERRAIN_COUNT
} TerrainType;

// Life forms
typedef enum {
    LIFE_NONE,
    LIFE_BACTERIA,
    LIFE_ALGAE,
    LIFE_MOSS,
    LIFE_GRASS,
    LIFE_SHRUB,
    LIFE_TREE,
    LIFE_ANIMAL_SMALL,
    LIFE_ANIMAL_LARGE,
    LIFE_COUNT
} LifeForm;

// Atmospheric composition
typedef struct {
    float nitrogen;
    float oxygen;
    float co2;
    float methane;
    float water_vapor;
    float argon;
    float pressure;
} Atmosphere;

// Ocean/Wind vector
typedef struct {
    float vx;
    float vy;
    float strength;
} FlowVector;

// Tectonic plate
typedef struct {
    int   id;
    float vx, vy;
    float rotation;
    int   type;    // 0=oceanic, 1=continental
    float density; // Affects subduction
} TectonicPlate;

// Planet configuration
typedef struct {
    float axial_tilt;            // Degrees (0-90)
    float rotation_speed;        // Hours per day
    float sea_level;             // Global sea level (-1000 to 1000 meters)
    float sea_level_change_rate; // Meters per step
} PlanetConfig;

// Tile structure
typedef struct {
    TerrainType terrain;
    float       altitude;      // Actual altitude in meters (-10000 to 10000)
    float       base_altitude; // Bedrock altitude (changes slowly)
    float       sediment;      // Sediment layer on top
    float       water_depth;   // For lakes and oceans
    float       temperature;
    float       moisture;
    float       nutrients;
    LifeForm    life;
    int         age;

    // Advanced fields
    int        plate_id;
    float      magma_heat;
    FlowVector ocean_current;
    FlowVector wind;
    float      salinity;
    float      ice_coverage;
    int        is_lake;        // Flag for inland water bodies
    float      drainage_basin; // Water accumulation
} Tile;

// World structure
typedef struct {
    Tile tiles[MAP_HEIGHT][MAP_WIDTH];
    int  cursor_x, cursor_y;
    int  time_step;
    int  day_cycle; // Current hour of day

    // Climate system
    float      global_temp;
    Atmosphere atmosphere;
    float      solar_constant;
    float      albedo;

    // Planet configuration
    PlanetConfig planet;

    // Tectonic system
    TectonicPlate plates[MAX_PLATES];
    int           num_plates;

    // View modes
    int view_mode;
    int paused;
} World;

// Function prototypes
void  init_world(World* world);
void  generate_initial_terrain(World* world);
void  generate_tectonic_plates(World* world);
void  determine_terrain_from_altitude(World* world);
void  simulate_step(World* world);
void  simulate_plate_tectonics(World* world);
void  simulate_ocean_currents(World* world);
void  simulate_wind_patterns(World* world);
void  simulate_atmosphere(World* world);
void  simulate_water_cycle(World* world);
void  update_climate(World* world);
void  render_world(World* world);
void  render_hex_tile(int x, int y, Tile* tile, World* world);
void  get_hex_neighbors(int x, int y, int neighbors[6][2], int* count);
void  move_cursor(World* world, char direction);
void  print_info_panel(World* world);
void  set_color_256(int color);
void  evolve_life(World* world);
float perlin_noise(float x, float y);
float calculate_greenhouse_effect(Atmosphere* atm);
void  apply_erosion(World* world);
float get_latitude(int y);
float clamp(float value, float min, float max);
void  adjust_sea_level(World* world, float change);

// Terminal state
static struct termios old_tio, new_tio;

// Safe math functions
float safe_divide(float a, float b) {
    if (fabs(b) < 0.0001) {
        return 0.0;
    }
    return a / b;
}

float clamp(float value, float min, float max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    if (isnan(value)) {
        return min; // Safety check for NaN
    }
    return value;
}

int main() {
    World world;
    char  input;
    int   running     = 1;
    int   frame_count = 0;

    // Initialize
    srand(time(NULL));
    init_terminal();
    init_world(&world);
    generate_tectonic_plates(&world);
    generate_initial_terrain(&world);
    determine_terrain_from_altitude(&world);

    printf(CLEAR HIDE_CURSOR);
    printf("Welcome to SimEarth Terminal Edition v2!\n");
    printf("New: Altitude system, sea levels, lakes, planetary axis\n");
    printf("Controls: WASD=move, Space=pause, 1-9=views, +=raise/-=lower sea, T=volcano, Q=quit\n");
    printf("Press any key to start...\n");
    getchar();

    // Main game loop
    while (running) {
        // Handle input
        if (kbhit()) {
            input = getchar();
            switch (input) {
            case 'q':
            case 'Q':
                running = 0;
                break;
            case ' ':
                world.paused = !world.paused;
                break;
            case 'w':
            case 'W':
                move_cursor(&world, 'N');
                break;
            case 's':
            case 'S':
                move_cursor(&world, 'S');
                break;
            case 'a':
            case 'A':
                move_cursor(&world, 'W');
                break;
            case 'd':
            case 'D':
                move_cursor(&world, 'E');
                break;
            case 'e':
            case 'E':
                move_cursor(&world, 'Q'); // NW
                break;
            case 'r':
            case 'R':
                move_cursor(&world, 'C'); // NE
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                world.view_mode = input - '1';
                break;
            case 't':
            case 'T':
                // Trigger volcanic activity
                world.tiles[world.cursor_y][world.cursor_x].magma_heat = 1.0;
                world.tiles[world.cursor_y][world.cursor_x].altitude += 500;
                break;
            case '+':
            case '=':
                adjust_sea_level(&world, 10.0);
                break;
            case '-':
            case '_':
                adjust_sea_level(&world, -10.0);
                break;
            }
        }

        // Simulate
        if (!world.paused && frame_count % 10 == 0) {
            simulate_step(&world);
        }

        // Render
        if (frame_count % 2 == 0) {
            render_world(&world);
        }

        frame_count++;
        usleep(50000); // 50ms delay
    }

    // Cleanup
    printf(CLEAR SHOW_CURSOR);
    restore_terminal();
    printf("Thanks for playing!\n");

    return 0;
}

float get_latitude(int y) {
    return (y - MAP_HEIGHT / 2.0) / (MAP_HEIGHT / 2.0); // -1 to 1
}

void init_world(World* world) {
    world->cursor_x    = MAP_WIDTH / 2;
    world->cursor_y    = MAP_HEIGHT / 2;
    world->time_step   = 0;
    world->day_cycle   = 0;
    world->global_temp = 15.0;
    world->view_mode   = 0;
    world->paused      = 0;

    // Initialize planet configuration
    world->planet.axial_tilt            = 23.5; // Earth-like tilt
    world->planet.rotation_speed        = 24.0; // 24 hour days
    world->planet.sea_level             = 0.0;  // Start at 0 meters
    world->planet.sea_level_change_rate = 0.0;

    // Initialize atmosphere (early Earth-like)
    world->atmosphere.nitrogen    = 0.78;
    world->atmosphere.oxygen      = 0.01;
    world->atmosphere.co2         = 0.10;
    world->atmosphere.methane     = 0.05;
    world->atmosphere.water_vapor = 0.05;
    world->atmosphere.argon       = 0.01;
    world->atmosphere.pressure    = 1.0;

    // Climate parameters
    world->solar_constant = 1361.0;
    world->albedo         = 0.3;

    // Initialize plates
    world->num_plates = 0;

    // Initialize all tiles
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile                   = &world->tiles[y][x];
            tile->terrain                = TERRAIN_OCEAN;
            tile->altitude               = -1000.0; // Start underwater
            tile->base_altitude          = -1000.0;
            tile->sediment               = 0.0;
            tile->water_depth            = 0.0;
            tile->temperature            = 15.0;
            tile->moisture               = 0.5;
            tile->nutrients              = 0.5;
            tile->life                   = LIFE_NONE;
            tile->age                    = 0;
            tile->plate_id               = 0;
            tile->magma_heat             = 0.0;
            tile->ocean_current.vx       = 0.0;
            tile->ocean_current.vy       = 0.0;
            tile->ocean_current.strength = 0.0;
            tile->wind.vx                = 0.0;
            tile->wind.vy                = 0.0;
            tile->wind.strength          = 0.0;
            tile->salinity               = 0.035;
            tile->ice_coverage           = 0.0;
            tile->is_lake                = 0;
            tile->drainage_basin         = 0.0;
        }
    }
}

float perlin_noise(float x, float y) {
    int n     = (int)(x * 57 + y * 131);
    n         = (n << 13) ^ n;
    float ret = (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
    return ret;
}

void generate_tectonic_plates(World* world) {
    world->num_plates = 5 + rand() % 3;

    // Initialize plates
    for (int i = 0; i < world->num_plates; i++) {
        world->plates[i].id       = i;
        world->plates[i].vx       = ((rand() % 100) / 100.0 - 0.5) * 0.01;
        world->plates[i].vy       = ((rand() % 100) / 100.0 - 0.5) * 0.01;
        world->plates[i].rotation = ((rand() % 100) / 100.0 - 0.5) * 0.001;
        world->plates[i].type     = (rand() % 3 == 0) ? 1 : 0;
        world->plates[i].density  = world->plates[i].type ? 2.7 : 3.0; // Continental vs oceanic
    }

    // Place plate centers
    int plate_centers[MAX_PLATES][2];
    for (int i = 0; i < world->num_plates; i++) {
        plate_centers[i][0] = rand() % MAP_WIDTH;
        plate_centers[i][1] = rand() % MAP_HEIGHT;
    }

    // Assign tiles to plates
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            float min_dist      = 999999;
            int   nearest_plate = 0;

            for (int p = 0; p < world->num_plates; p++) {
                int dx = abs(x - plate_centers[p][0]);
                if (dx > MAP_WIDTH / 2) {
                    dx = MAP_WIDTH - dx;
                }
                int   dy   = y - plate_centers[p][1];
                float dist = sqrt(dx * dx + dy * dy);

                if (dist < min_dist) {
                    min_dist      = dist;
                    nearest_plate = p;
                }
            }

            world->tiles[y][x].plate_id = nearest_plate;
        }
    }
}

void generate_initial_terrain(World* world) {
    // Generate base altitudes based on plates
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile*          tile  = &world->tiles[y][x];
            TectonicPlate* plate = &world->plates[tile->plate_id];

            // Base altitude from plate type
            float base_alt = (plate->type == 1) ? 500.0 : -2000.0;

            // Add noise for variation (seamless on x-axis)
            float angle = (x / (float)MAP_WIDTH) * 2 * 3.14159;
            float nx    = cos(angle) * 5;
            float nz    = sin(angle) * 5;
            float ny    = y / 10.0;

            float noise_factor = 0;
            noise_factor += perlin_noise(nx, ny) * 2000;
            noise_factor += perlin_noise(nx * 2, ny * 2) * 1000;
            noise_factor += perlin_noise(nx * 4 + nz, ny * 4) * 500;

            tile->altitude      = base_alt + noise_factor;
            tile->base_altitude = tile->altitude;

            // Check for plate boundaries
            int neighbors[6][2];
            int count;
            get_hex_neighbors(x, y, neighbors, &count);

            int at_boundary = 0;
            for (int i = 0; i < count; i++) {
                if (world->tiles[neighbors[i][1]][neighbors[i][0]].plate_id != tile->plate_id) {
                    at_boundary = 1;
                    break;
                }
            }

            // Plate boundaries create interesting features
            if (at_boundary) {
                if (plate->type == 1) {
                    tile->altitude += 1000; // Mountains at continental boundaries
                    tile->base_altitude = tile->altitude;
                    tile->magma_heat    = 0.1;
                } else {
                    tile->altitude -= 1000; // Trenches at oceanic boundaries
                    tile->base_altitude = tile->altitude;
                }
            }

            // Clamp altitudes to reasonable range
            tile->altitude      = clamp(tile->altitude, -10000, 10000);
            tile->base_altitude = tile->altitude;

            // Set initial temperature based on latitude and altitude
            float latitude    = get_latitude(y);
            float lat_temp    = 30.0 - fabs(latitude) * 45.0;
            float alt_cooling = tile->altitude * 0.0065; // 6.5°C per 1000m
            tile->temperature = clamp(lat_temp - alt_cooling, MIN_TEMP, MAX_TEMP);

            // Initialize water depth for submerged areas
            if (tile->altitude < world->planet.sea_level) {
                tile->water_depth = world->planet.sea_level - tile->altitude;
            }

            // Seed initial life in shallow waters
            if (tile->water_depth > 0 && tile->water_depth < 200 && rand() % 10 == 0) {
                tile->life = LIFE_ALGAE;
            }
        }
    }
}

void determine_terrain_from_altitude(World* world) {
    // First pass: determine water bodies
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile    = &world->tiles[y][x];
            tile->is_lake = 0;

            // Calculate effective altitude (base + sediment)
            float effective_altitude = tile->base_altitude + tile->sediment;
            tile->altitude           = effective_altitude;

            // Determine water depth
            if (effective_altitude < world->planet.sea_level) {
                tile->water_depth = world->planet.sea_level - effective_altitude;
            } else {
                tile->water_depth = tile->drainage_basin; // Lakes from accumulated water
                if (tile->water_depth > 10) {
                    tile->is_lake = 1;
                }
            }
        }
    }

    // Second pass: check for inland water (lakes)
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = &world->tiles[y][x];

            if (tile->water_depth > 0 && tile->altitude > world->planet.sea_level - 50) {
                // Check if connected to ocean
                int connected_to_ocean = 0;
                int neighbors[6][2];
                int count;
                get_hex_neighbors(x, y, neighbors, &count);

                for (int i = 0; i < count; i++) {
                    Tile* neighbor = &world->tiles[neighbors[i][1]][neighbors[i][0]];
                    if (neighbor->altitude < world->planet.sea_level - 100) {
                        connected_to_ocean = 1;
                        break;
                    }
                }

                if (!connected_to_ocean && tile->water_depth > 5) {
                    tile->is_lake = 1;
                }
            }
        }
    }

    // Third pass: assign terrain types
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = &world->tiles[y][x];
            float alt  = tile->altitude;
            float sea  = world->planet.sea_level;

            // Water terrains
            if (tile->water_depth > 0) {
                if (tile->is_lake) {
                    tile->terrain  = TERRAIN_LAKE;
                    tile->moisture = 1.0;
                    tile->salinity = 0.001; // Fresh water
                } else if (tile->water_depth > 2000) {
                    tile->terrain  = TERRAIN_DEEP_OCEAN;
                    tile->moisture = 1.0;
                } else if (tile->water_depth > 200) {
                    tile->terrain  = TERRAIN_OCEAN;
                    tile->moisture = 1.0;
                } else {
                    tile->terrain  = TERRAIN_SHALLOW_WATER;
                    tile->moisture = 1.0;
                }
            }
            // Land terrains based on altitude
            else if (alt < sea + 10) {
                tile->terrain  = TERRAIN_BEACH;
                tile->moisture = clamp(tile->moisture, 0.6, 1.0);
            } else if (alt < sea + 300) {
                // Lowlands - terrain depends on climate
                if (tile->temperature > 35 && tile->moisture < 0.2) {
                    tile->terrain = TERRAIN_DESERT;
                } else if (tile->temperature < -5) {
                    tile->terrain = TERRAIN_TUNDRA;
                } else if (tile->moisture > 0.5) {
                    tile->terrain = TERRAIN_FOREST;
                } else {
                    tile->terrain = TERRAIN_GRASSLAND;
                }
            } else if (alt < sea + 1000) {
                tile->terrain  = TERRAIN_HILL;
                tile->moisture = clamp(tile->moisture * 0.8, 0, 1.0);
            } else if (alt < sea + 2500) {
                tile->terrain  = TERRAIN_MOUNTAIN;
                tile->moisture = clamp(tile->moisture * 0.6, 0, 1.0);
            } else {
                // High peaks
                if (tile->temperature < -10 || alt > sea + 3500) {
                    tile->terrain = TERRAIN_SNOW;
                } else {
                    tile->terrain = TERRAIN_PEAK;
                }
                tile->moisture = clamp(tile->moisture * 0.4, 0, 1.0);
            }

            // Volcanic override
            if (tile->magma_heat > 0.5) {
                tile->terrain = TERRAIN_VOLCANIC;
            }

            // Ensure moisture is clamped
            tile->moisture = clamp(tile->moisture, 0.0, 1.0);
        }
    }
}

void get_hex_neighbors(int x, int y, int neighbors[6][2], int* count) {
    *count = 0;

    if (y % 2 == 0) {
        int offsets[6][2] = { { -1, 0 }, { 1, 0 }, { -1, -1 }, { 0, -1 }, { -1, 1 }, { 0, 1 } };
        for (int i = 0; i < 6; i++) {
            int nx = x + offsets[i][0];
            int ny = y + offsets[i][1];
            nx     = (nx + MAP_WIDTH) % MAP_WIDTH;

            if (ny >= 0 && ny < MAP_HEIGHT) {
                neighbors[*count][0] = nx;
                neighbors[*count][1] = ny;
                (*count)++;
            }
        }
    } else {
        int offsets[6][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 1, -1 }, { 0, 1 }, { 1, 1 } };
        for (int i = 0; i < 6; i++) {
            int nx = x + offsets[i][0];
            int ny = y + offsets[i][1];
            nx     = (nx + MAP_WIDTH) % MAP_WIDTH;

            if (ny >= 0 && ny < MAP_HEIGHT) {
                neighbors[*count][0] = nx;
                neighbors[*count][1] = ny;
                (*count)++;
            }
        }
    }
}

void simulate_plate_tectonics(World* world) {
    if (world->time_step % 100 != 0) {
        return;
    }

    // Update plate movements
    for (int p = 0; p < world->num_plates; p++) {
        TectonicPlate* plate = &world->plates[p];

        // Small random changes to plate motion
        plate->vx += ((rand() % 100) / 100.0 - 0.5) * 0.0005;
        plate->vy += ((rand() % 100) / 100.0 - 0.5) * 0.0005;
        plate->vx = clamp(plate->vx, -0.02, 0.02);
        plate->vy = clamp(plate->vy, -0.02, 0.02);
    }

    // Process plate boundaries
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = &world->tiles[y][x];
            int   neighbors[6][2];
            int   count;
            get_hex_neighbors(x, y, neighbors, &count);

            for (int i = 0; i < count; i++) {
                Tile* neighbor = &world->tiles[neighbors[i][1]][neighbors[i][0]];
                if (neighbor->plate_id != tile->plate_id) {
                    TectonicPlate* p1 = &world->plates[tile->plate_id];
                    TectonicPlate* p2 = &world->plates[neighbor->plate_id];

                    float dx          = neighbors[i][0] - x;
                    float dy          = neighbors[i][1] - y;
                    float convergence = (p1->vx - p2->vx) * dx + (p1->vy - p2->vy) * dy;

                    if (convergence > 0.005) {
                        // Converging plates
                        if (p1->type == 1 && p2->type == 1) {
                            // Continental collision
                            tile->base_altitude += 5;
                            neighbor->base_altitude += 5;
                            tile->magma_heat = clamp(tile->magma_heat + 0.02, 0, 1);
                        } else if (p1->density > p2->density) {
                            // Subduction
                            tile->base_altitude -= 2;
                            neighbor->base_altitude += 3;
                            neighbor->magma_heat = clamp(neighbor->magma_heat + 0.05, 0, 1);
                        } else {
                            tile->base_altitude += 3;
                            neighbor->base_altitude -= 2;
                            tile->magma_heat = clamp(tile->magma_heat + 0.05, 0, 1);
                        }
                    } else if (convergence < -0.005) {
                        // Diverging plates
                        tile->base_altitude -= 3;
                        tile->magma_heat = clamp(tile->magma_heat + 0.1, 0, 1);
                    }

                    // Clamp altitudes
                    tile->base_altitude     = clamp(tile->base_altitude, -10000, 10000);
                    neighbor->base_altitude = clamp(neighbor->base_altitude, -10000, 10000);
                }
            }

            // Volcanic eruptions
            if (tile->magma_heat > 0.8 && rand() % 100 < 5) {
                tile->terrain = TERRAIN_VOLCANIC;
                tile->base_altitude += 50;
                tile->sediment += 10;
                tile->magma_heat      = 1.0;
                world->atmosphere.co2 = clamp(world->atmosphere.co2 + 0.0005, 0, 0.5);
            }

            // Cool magma
            tile->magma_heat *= 0.995;
        }
    }
}

void simulate_water_cycle(World* world) {
    // Water accumulation in low areas (lakes)
    float new_drainage[MAP_HEIGHT][MAP_WIDTH];
    memset(new_drainage, 0, sizeof(new_drainage));

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = &world->tiles[y][x];

            // Rain based on moisture and temperature
            if (tile->altitude > world->planet.sea_level && tile->moisture > 0.3 && tile->temperature > 0) {
                new_drainage[y][x] += tile->moisture * 0.1;
            }

            // Water flows downhill
            if (tile->drainage_basin > 0.1) {
                int neighbors[6][2];
                int count;
                get_hex_neighbors(x, y, neighbors, &count);

                int   lowest_idx = -1;
                float lowest_alt = tile->altitude;

                for (int i = 0; i < count; i++) {
                    Tile* neighbor = &world->tiles[neighbors[i][1]][neighbors[i][0]];
                    if (neighbor->altitude < lowest_alt) {
                        lowest_alt = neighbor->altitude;
                        lowest_idx = i;
                    }
                }

                if (lowest_idx >= 0) {
                    // Transfer water to lowest neighbor
                    float transfer = tile->drainage_basin * 0.2;
                    new_drainage[neighbors[lowest_idx][1]][neighbors[lowest_idx][0]] += transfer;
                    new_drainage[y][x] -= transfer;
                }
            }

            // Evaporation
            if (tile->water_depth > 0 && tile->temperature > 0) {
                float evap_rate = 0.01 * (tile->temperature / 30.0);
                world->atmosphere.water_vapor += evap_rate * 0.00001;
                new_drainage[y][x] -= evap_rate;
            }
        }
    }

    // Apply drainage changes
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            world->tiles[y][x].drainage_basin += new_drainage[y][x];
            world->tiles[y][x].drainage_basin = clamp(world->tiles[y][x].drainage_basin, 0, 100);
        }
    }
}

void simulate_ocean_currents(World* world) {
    FlowVector new_currents[MAP_HEIGHT][MAP_WIDTH];

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = &world->tiles[y][x];

            if (tile->water_depth > 10) {
                float temp_gradient_x = 0, temp_gradient_y = 0;
                int   neighbors[6][2];
                int   count;
                get_hex_neighbors(x, y, neighbors, &count);

                int water_neighbors = 0;
                for (int i = 0; i < count; i++) {
                    Tile* neighbor = &world->tiles[neighbors[i][1]][neighbors[i][0]];
                    if (neighbor->water_depth > 10) {
                        float temp_diff = neighbor->temperature - tile->temperature;
                        temp_gradient_x += temp_diff * (neighbors[i][0] - x);
                        temp_gradient_y += temp_diff * (neighbors[i][1] - y);
                        water_neighbors++;
                    }
                }

                if (water_neighbors > 0) {
                    temp_gradient_x /= water_neighbors;
                    temp_gradient_y /= water_neighbors;
                }

                // Coriolis effect with planetary axis
                float latitude           = get_latitude(y);
                float effective_latitude = latitude * cos(world->planet.axial_tilt * 3.14159 / 180);
                float coriolis           = sin(effective_latitude * 3.14159 / 2) * 0.3;

                // Update current
                new_currents[y][x].vx = tile->ocean_current.vx * 0.9;
                new_currents[y][x].vy = tile->ocean_current.vy * 0.9;
                new_currents[y][x].vx += temp_gradient_x * 0.005;
                new_currents[y][x].vy += temp_gradient_y * 0.005;

                float temp_vx = new_currents[y][x].vx;
                new_currents[y][x].vx += coriolis * new_currents[y][x].vy;
                new_currents[y][x].vy -= coriolis * temp_vx;

                // Trade winds
                if (fabs(latitude) < 0.3) {
                    new_currents[y][x].vx -= 0.01;
                } else if (fabs(latitude) < 0.7) {
                    new_currents[y][x].vx += 0.01;
                }

                float speed = sqrt(new_currents[y][x].vx * new_currents[y][x].vx + new_currents[y][x].vy * new_currents[y][x].vy);
                speed       = clamp(speed, 0, 0.5);
                if (speed > 0.5) {
                    new_currents[y][x].vx *= 0.5 / speed;
                    new_currents[y][x].vy *= 0.5 / speed;
                }
                new_currents[y][x].strength = speed;
            } else {
                new_currents[y][x].vx       = 0;
                new_currents[y][x].vy       = 0;
                new_currents[y][x].strength = 0;
            }
        }
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            world->tiles[y][x].ocean_current = new_currents[y][x];
        }
    }
}

void simulate_wind_patterns(World* world) {
    FlowVector new_winds[MAP_HEIGHT][MAP_WIDTH];

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = &world->tiles[y][x];

            // Pressure based on temperature and altitude
            float alt_pressure = world->atmosphere.pressure * exp(-tile->altitude / 8000.0);
            float temp_kelvin  = 273.15 + tile->temperature;
            float pressure     = alt_pressure * safe_divide(temp_kelvin, 288.15);
            pressure           = clamp(pressure, 0.1, 2.0);

            float pressure_gradient_x = 0, pressure_gradient_y = 0;
            int   neighbors[6][2];
            int   count;
            get_hex_neighbors(x, y, neighbors, &count);

            for (int i = 0; i < count; i++) {
                Tile* neighbor       = &world->tiles[neighbors[i][1]][neighbors[i][0]];
                float n_alt_pressure = world->atmosphere.pressure * exp(-neighbor->altitude / 8000.0);
                float n_temp_kelvin  = 273.15 + neighbor->temperature;
                float n_pressure     = n_alt_pressure * safe_divide(n_temp_kelvin, 288.15);
                n_pressure           = clamp(n_pressure, 0.1, 2.0);

                float pressure_diff = pressure - n_pressure;
                pressure_gradient_x += pressure_diff * (neighbors[i][0] - x);
                pressure_gradient_y += pressure_diff * (neighbors[i][1] - y);
            }

            // Coriolis with axial tilt
            float latitude           = get_latitude(y);
            float effective_latitude = latitude * cos(world->planet.axial_tilt * 3.14159 / 180);
            float coriolis           = sin(effective_latitude * 3.14159 / 2) * 0.2;

            new_winds[y][x].vx = tile->wind.vx * 0.85;
            new_winds[y][x].vy = tile->wind.vy * 0.85;
            new_winds[y][x].vx += pressure_gradient_x * 0.02;
            new_winds[y][x].vy += pressure_gradient_y * 0.02;

            float temp_vx = new_winds[y][x].vx;
            new_winds[y][x].vx += coriolis * new_winds[y][x].vy;
            new_winds[y][x].vy -= coriolis * temp_vx;

            // Prevailing winds
            if (fabs(latitude) < 0.3) {
                new_winds[y][x].vx -= 0.05;
            } else if (fabs(latitude) < 0.6) {
                new_winds[y][x].vx += 0.05;
            } else {
                new_winds[y][x].vx -= 0.02;
            }

            // Mountains affect wind
            if (tile->altitude > world->planet.sea_level + 1000) {
                float reduction = 1.0 - (tile->altitude - world->planet.sea_level) / 5000.0;
                reduction       = clamp(reduction, 0.1, 1.0);
                new_winds[y][x].vx *= reduction;
                new_winds[y][x].vy *= reduction;
            }

            float speed              = sqrt(new_winds[y][x].vx * new_winds[y][x].vx + new_winds[y][x].vy * new_winds[y][x].vy);
            speed                    = clamp(speed, 0, 1.0);
            new_winds[y][x].strength = speed;

            // Wind carries moisture
            if (tile->water_depth > 0 && speed > 0.1) {
                for (int i = 0; i < count; i++) {
                    float wind_component = new_winds[y][x].vx * (neighbors[i][0] - x) + new_winds[y][x].vy * (neighbors[i][1] - y);
                    if (wind_component > 0) {
                        world->tiles[neighbors[i][1]][neighbors[i][0]].moisture += clamp(tile->moisture * 0.001 * wind_component, 0, 0.01);
                    }
                }
            }
        }
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            world->tiles[y][x].wind     = new_winds[y][x];
            world->tiles[y][x].moisture = clamp(world->tiles[y][x].moisture, 0.0, 1.0);
        }
    }
}

float calculate_greenhouse_effect(Atmosphere* atm) {
    float greenhouse_factor = 1.0;
    greenhouse_factor += log(1 + atm->co2 * 100) * 5.0;
    greenhouse_factor += atm->methane * 80.0;
    greenhouse_factor += atm->water_vapor * 20.0;
    return clamp(greenhouse_factor, 0, 100);
}

void simulate_atmosphere(World* world) {
    float o2_production = 0, co2_consumption = 0;
    float methane_production = 0;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = &world->tiles[y][x];

            if (tile->life >= LIFE_ALGAE && tile->life <= LIFE_TREE) {
                float photosynthesis_rate = 0.00001 * (tile->life - LIFE_ALGAE + 1);

                if (tile->temperature > 0 && tile->temperature < 40) {
                    float temp_factor = 1.0 - fabs(tile->temperature - 25) / 25.0;
                    photosynthesis_rate *= temp_factor;
                    photosynthesis_rate *= (1 + world->atmosphere.co2);

                    o2_production += photosynthesis_rate;
                    co2_consumption += photosynthesis_rate;
                }
            }

            if (tile->life != LIFE_NONE) {
                co2_consumption -= 0.000005;
                o2_production -= 0.000005;
            }

            if (tile->terrain == TERRAIN_SHALLOW_WATER && tile->temperature > 10) {
                methane_production += 0.0000001;
            }

            if (tile->terrain == TERRAIN_VOLCANIC && tile->magma_heat > 0.5) {
                world->atmosphere.co2         = clamp(world->atmosphere.co2 + 0.00001, 0, 0.5);
                world->atmosphere.water_vapor = clamp(world->atmosphere.water_vapor + 0.00001, 0, 0.2);
            }
        }
    }

    float area                = MAP_WIDTH * MAP_HEIGHT;
    world->atmosphere.oxygen  = clamp(world->atmosphere.oxygen + o2_production / area, 0, 0.5);
    world->atmosphere.co2     = clamp(world->atmosphere.co2 + co2_consumption / area, 0.0001, 0.5);
    world->atmosphere.methane = clamp(world->atmosphere.methane + methane_production / area, 0, 0.1);

    world->atmosphere.co2 *= 0.9999;    // Chemical weathering
    world->atmosphere.methane *= 0.999; // Oxidation

    // Normalize
    float total = world->atmosphere.nitrogen + world->atmosphere.oxygen + world->atmosphere.co2 + world->atmosphere.methane + world->atmosphere.water_vapor + world->atmosphere.argon;

    if (total > 0.01) {
        world->atmosphere.nitrogen /= total;
        world->atmosphere.oxygen /= total;
        world->atmosphere.co2 /= total;
        world->atmosphere.methane /= total;
        world->atmosphere.water_vapor /= total;
        world->atmosphere.argon /= total;
    }
}

void update_climate(World* world) {
    float greenhouse   = calculate_greenhouse_effect(&world->atmosphere);
    world->global_temp = clamp(-18.0 + greenhouse, MIN_TEMP, MAX_TEMP);

    // Day/night cycle
    world->day_cycle = (world->day_cycle + 1) % (int)world->planet.rotation_speed;
    float day_factor = sin(world->day_cycle * 2 * 3.14159 / world->planet.rotation_speed);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = &world->tiles[y][x];

            // Base temperature from latitude with axial tilt effect
            float latitude           = get_latitude(y);
            float season_factor      = sin(world->time_step * 0.01) * world->planet.axial_tilt / 90.0;
            float effective_latitude = latitude - season_factor;
            float lat_temp           = 30.0 - fabs(effective_latitude) * 45.0;

            // Altitude cooling
            float alt_above_sea = fmax(0, tile->altitude - world->planet.sea_level);
            float alt_cooling   = alt_above_sea * 0.0065;

            float base_temp = world->global_temp + lat_temp - alt_cooling;

            // Day/night variation (less in water)
            if (tile->water_depth > 0) {
                base_temp += day_factor * 2;
                base_temp = (base_temp + tile->temperature * 4) / 5;
            } else {
                base_temp += day_factor * 5;
            }

            // Ocean currents
            if (tile->ocean_current.strength > 0.1) {
                if (tile->ocean_current.vy > 0 && y < MAP_HEIGHT / 2) {
                    base_temp += 3 * tile->ocean_current.strength;
                } else if (tile->ocean_current.vy < 0 && y > MAP_HEIGHT / 2) {
                    base_temp += 3 * tile->ocean_current.strength;
                }
            }

            tile->temperature = clamp(base_temp, MIN_TEMP, MAX_TEMP);

            // Ice coverage
            if (tile->temperature < -5) {
                tile->ice_coverage = clamp(tile->ice_coverage + 0.01, 0, 1);
                world->albedo      = clamp(world->albedo + 0.00001, 0, 0.9);
            } else if (tile->temperature > 0) {
                tile->ice_coverage = clamp(tile->ice_coverage - 0.02, 0, 1);
            }
        }
    }
}

void apply_erosion(World* world) {
    if (world->time_step % 20 != 0) {
        return;
    }

    float erosion[MAP_HEIGHT][MAP_WIDTH];
    memset(erosion, 0, sizeof(erosion));

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = &world->tiles[y][x];

            if (tile->altitude > world->planet.sea_level) {
                float erosion_rate = (tile->altitude - world->planet.sea_level) * 0.00001;
                erosion_rate *= (tile->moisture + 0.1);
                erosion_rate *= (1 + tile->wind.strength);

                int neighbors[6][2];
                int count;
                get_hex_neighbors(x, y, neighbors, &count);

                int lower_neighbors = 0;
                for (int i = 0; i < count; i++) {
                    Tile* neighbor = &world->tiles[neighbors[i][1]][neighbors[i][0]];
                    if (neighbor->altitude < tile->altitude) {
                        erosion[neighbors[i][1]][neighbors[i][0]] += erosion_rate / count;
                        lower_neighbors++;
                    }
                }

                if (lower_neighbors > 0) {
                    erosion[y][x] -= erosion_rate;
                }
            }
        }
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            world->tiles[y][x].sediment += erosion[y][x];
            world->tiles[y][x].sediment = clamp(world->tiles[y][x].sediment, 0, 1000);
        }
    }
}

void evolve_life(World* world) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Tile* tile = &world->tiles[y][x];

            float min_oxygen = 0.001;
            if (tile->life == LIFE_ANIMAL_SMALL) {
                min_oxygen = 0.1;
            }
            if (tile->life == LIFE_ANIMAL_LARGE) {
                min_oxygen = 0.15;
            }

            if (world->atmosphere.oxygen < min_oxygen && (tile->life == LIFE_ANIMAL_SMALL || tile->life == LIFE_ANIMAL_LARGE)) {
                tile->life = LIFE_NONE;
                continue;
            }

            if (tile->life == LIFE_NONE) {
                if (tile->water_depth > 0 && tile->water_depth < 200 && tile->temperature > 5 && rand() % 100 < 2) {
                    tile->life = LIFE_ALGAE;
                } else if (tile->terrain == TERRAIN_GRASSLAND && tile->moisture > 0.3 && world->atmosphere.oxygen > 0.01 && rand() % 100 < 1) {
                    tile->life = LIFE_GRASS;
                }
            } else {
                tile->age++;

                if (tile->age > 50 && rand() % 100 < 5) {
                    // Evolution logic
                    if (tile->life == LIFE_ALGAE && world->atmosphere.oxygen > 0.02) {
                        tile->life = LIFE_MOSS;
                        tile->age  = 0;
                    } else if (tile->life == LIFE_MOSS && tile->moisture > 0.2) {
                        tile->life = LIFE_GRASS;
                        tile->age  = 0;
                    } else if (tile->life == LIFE_GRASS && tile->moisture > 0.4 && world->atmosphere.oxygen > 0.1) {
                        tile->life = LIFE_SHRUB;
                        tile->age  = 0;
                    } else if (tile->life == LIFE_SHRUB && tile->moisture > 0.5 && world->atmosphere.oxygen > 0.15) {
                        tile->life = LIFE_TREE;
                        tile->age  = 0;
                    }
                }

                // Death conditions
                if ((tile->temperature < -30 || tile->temperature > 50) && rand() % 100 < 20) {
                    tile->life = LIFE_NONE;
                    tile->age  = 0;
                } else if (tile->water_depth > 2000) {
                    // Too deep for most life
                    if (tile->life != LIFE_BACTERIA && tile->life != LIFE_ALGAE) {
                        tile->life = LIFE_NONE;
                    }
                }
            }

            // Life spreads
            if (tile->life != LIFE_NONE && rand() % 100 < 3) {
                int neighbors[6][2];
                int count;
                get_hex_neighbors(x, y, neighbors, &count);

                if (count > 0) {
                    int   n        = rand() % count;
                    Tile* neighbor = &world->tiles[neighbors[n][1]][neighbors[n][0]];
                    if (neighbor->life == LIFE_NONE && neighbor->water_depth < 2000 && neighbor->terrain != TERRAIN_VOLCANIC) {

                        // Check if suitable habitat
                        int suitable = 0;
                        if ((tile->life == LIFE_ALGAE || tile->life == LIFE_BACTERIA) && neighbor->water_depth > 0) {
                            suitable = 1;
                        } else if (neighbor->water_depth == 0 && neighbor->altitude < world->planet.sea_level + 3000) {
                            suitable = 1;
                        }

                        if (suitable) {
                            neighbor->life = tile->life;
                            neighbor->age  = 0;
                        }
                    }
                }
            }
        }
    }
}

void adjust_sea_level(World* world, float change) {
    world->planet.sea_level += change;
    world->planet.sea_level = clamp(world->planet.sea_level, -2000, 2000);
    determine_terrain_from_altitude(world);
}

void simulate_step(World* world) {
    world->time_step++;

    simulate_ocean_currents(world);
    simulate_wind_patterns(world);

    if (world->time_step % 5 == 0) {
        simulate_water_cycle(world);
        simulate_atmosphere(world);
        update_climate(world);
        determine_terrain_from_altitude(world);
        evolve_life(world);
    }

    if (world->time_step % 20 == 0) {
        apply_erosion(world);
    }

    simulate_plate_tectonics(world);

    // Sea level changes from ice
    float total_ice = 0;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            total_ice += world->tiles[y][x].ice_coverage;
        }
    }

    // More ice = lower sea level
    world->planet.sea_level_change_rate = (0.5 - total_ice / (MAP_WIDTH * MAP_HEIGHT)) * 0.1;
    world->planet.sea_level += world->planet.sea_level_change_rate;
    world->planet.sea_level = clamp(world->planet.sea_level, -2000, 2000);
}

void set_color_256(int color) {
    printf("\033[38;5;%dm", color);
}

void render_hex_tile(int x, int y, Tile* tile, World* world) {
    int  color  = 15;
    char symbol = ' ';

    if (world->view_mode == 0) { // Terrain view
        switch (tile->terrain) {
        case TERRAIN_DEEP_OCEAN:
            color  = 17;
            symbol = '~';
            break;
        case TERRAIN_OCEAN:
            color  = 21;
            symbol = '~';
            break;
        case TERRAIN_SHALLOW_WATER:
            color  = 39;
            symbol = '~';
            break;
        case TERRAIN_LAKE:
            color  = 45;
            symbol = '~';
            break;
        case TERRAIN_BEACH:
            color  = 229;
            symbol = '.';
            break;
        case TERRAIN_GRASSLAND:
            color  = 40;
            symbol = '"';
            break;
        case TERRAIN_FOREST:
            color  = 28;
            symbol = 'T';
            break;
        case TERRAIN_HILL:
            color  = 94;
            symbol = '^';
            break;
        case TERRAIN_MOUNTAIN:
            color  = 244;
            symbol = 'A';
            break;
        case TERRAIN_PEAK:
            color  = 255;
            symbol = 'A';
            break;
        case TERRAIN_SNOW:
            color  = 255;
            symbol = '*';
            break;
        case TERRAIN_DESERT:
            color  = 178;
            symbol = '~';
            break;
        case TERRAIN_TUNDRA:
            color  = 252;
            symbol = '_';
            break;
        case TERRAIN_VOLCANIC:
            color  = 196;
            symbol = 'V';
            break;
        }

        if (tile->life != LIFE_NONE) {
            switch (tile->life) {
            case LIFE_ALGAE:
                color  = 22;
                symbol = '~';
                break;
            case LIFE_MOSS:
                color  = 64;
                symbol = '.';
                break;
            case LIFE_GRASS:
                color  = 82;
                symbol = '"';
                break;
            case LIFE_SHRUB:
                color  = 70;
                symbol = '*';
                break;
            case LIFE_TREE:
                color  = 28;
                symbol = 'T';
                break;
            case LIFE_ANIMAL_SMALL:
                color  = 130;
                symbol = 'a';
                break;
            case LIFE_ANIMAL_LARGE:
                color  = 94;
                symbol = 'A';
                break;
            default:
                break;
            }
        }
    } else if (world->view_mode == 1) { // Temperature
        float temp = tile->temperature;
        if (temp < -20) {
            color  = 21;
            symbol = 'F';
        } else if (temp < 0) {
            color  = 39;
            symbol = 'C';
        } else if (temp < 10) {
            color  = 46;
            symbol = 'c';
        } else if (temp < 20) {
            color  = 226;
            symbol = 'w';
        } else if (temp < 30) {
            color  = 208;
            symbol = 'W';
        } else {
            color  = 196;
            symbol = 'H';
        }
    } else if (world->view_mode == 2) { // Moisture
        int level = (int)(tile->moisture * 9);
        color     = 232 + level * 2;
        symbol    = '0' + level;
    } else if (world->view_mode == 3) { // Life
        if (tile->life == LIFE_NONE) {
            color  = 237;
            symbol = '.';
        } else {
            color  = 46 + tile->life * 20;
            symbol = '0' + tile->life;
        }
    } else if (world->view_mode == 4) { // Plates
        color  = 16 + (tile->plate_id * 30) % 230;
        symbol = '0' + tile->plate_id;
        if (tile->magma_heat > 0.5) {
            color  = 196;
            symbol = 'V';
        }
    } else if (world->view_mode == 5) { // Ocean currents
        if (tile->ocean_current.strength > 0.01) {
            float angle = atan2(tile->ocean_current.vy, tile->ocean_current.vx);
            if (angle < -2.36) {
                symbol = '<';
            } else if (angle < -1.57) {
                symbol = '\\';
            } else if (angle < -0.78) {
                symbol = 'v';
            } else if (angle < 0) {
                symbol = '/';
            } else if (angle < 0.78) {
                symbol = '>';
            } else if (angle < 1.57) {
                symbol = '/';
            } else if (angle < 2.36) {
                symbol = '^';
            } else {
                symbol = '\\';
            }

            if (tile->temperature > 20) {
                color = 196;
            } else if (tile->temperature > 10) {
                color = 226;
            } else {
                color = 39;
            }
        } else {
            color  = 17;
            symbol = '.';
        }
    } else if (world->view_mode == 6) { // Winds
        if (tile->wind.strength > 0.01) {
            float angle = atan2(tile->wind.vy, tile->wind.vx);
            if (angle < -2.36) {
                symbol = '<';
            } else if (angle < -1.57) {
                symbol = '\\';
            } else if (angle < -0.78) {
                symbol = 'v';
            } else if (angle < 0) {
                symbol = '/';
            } else if (angle < 0.78) {
                symbol = '>';
            } else if (angle < 1.57) {
                symbol = '/';
            } else if (angle < 2.36) {
                symbol = '^';
            } else {
                symbol = '\\';
            }

            int strength = (int)(tile->wind.strength * 5);
            color        = 250 - strength * 10;
        } else {
            color  = 237;
            symbol = '.';
        }
    } else if (world->view_mode == 7) { // Atmosphere/O2
        int o2 = (int)(world->atmosphere.oxygen * 100);
        if (o2 < 5) {
            color  = 52;
            symbol = 'L';
        } else if (o2 < 10) {
            color  = 226;
            symbol = 'M';
        } else if (o2 < 15) {
            color  = 82;
            symbol = 'G';
        } else {
            color  = 46;
            symbol = 'H';
        }

        if (tile->life >= LIFE_ALGAE && tile->life <= LIFE_TREE) {
            symbol = 'O';
        }
    } else if (world->view_mode == 8) { // Altitude view
        float alt_meters = tile->altitude;
        float sea        = world->planet.sea_level;

        if (alt_meters < sea - 2000) {
            color  = 17;
            symbol = 'D';
        } else if (alt_meters < sea - 500) {
            color  = 21;
            symbol = 'd';
        } else if (alt_meters < sea) {
            color  = 39;
            symbol = '-';
        } else if (alt_meters < sea + 500) {
            color  = 40;
            symbol = '.';
        } else if (alt_meters < sea + 1000) {
            color  = 226;
            symbol = '^';
        } else if (alt_meters < sea + 2000) {
            color  = 94;
            symbol = 'A';
        } else if (alt_meters < sea + 3000) {
            color  = 244;
            symbol = 'M';
        } else {
            color  = 255;
            symbol = 'P';
        }

        if (tile->is_lake) {
            color  = 45;
            symbol = 'L';
        }
    }

    if (x == world->cursor_x && y == world->cursor_y) {
        printf("\033[7m");
    }

    set_color_256(color);
    printf("%c", symbol);

    if (x == world->cursor_x && y == world->cursor_y) {
        printf("\033[27m");
    }
}

void render_world(World* world) {
    printf(CLEAR);

    printf("=== SimEarth v2 === Step:%d Day:%d ", world->time_step, world->day_cycle);
    if (world->paused) {
        printf("[PAUSED]");
    }
    printf("\n");

    const char* modes[] = { "Terrain", "Temperature", "Moisture", "Life",
                            "Plates", "Currents", "Winds", "O2", "Altitude" };
    printf("View:%s | Temp:%.1f°C | Sea:%.0fm | Tilt:%.1f° | O2:%.1f%%\n", modes[world->view_mode], world->global_temp, world->planet.sea_level, world->planet.axial_tilt, world->atmosphere.oxygen * 100);
    printf("\n");

    for (int y = MAP_HEIGHT - 1; y >= 0; y--) {
        if (y % 2 == 1) {
            printf(" ");
        }
        for (int x = 0; x < MAP_WIDTH; x++) {
            render_hex_tile(x, y, &world->tiles[y][x], world);
            printf(" ");
        }
        printf(RESET "\n");
    }

    print_info_panel(world);
}

void print_info_panel(World* world) {
    printf("\n=== World Statistics ===\n");
    printf("Atmosphere: N2:%.1f%% O2:%.1f%% CO2:%.3f%% CH4:%.4f%%\n", world->atmosphere.nitrogen * 100, world->atmosphere.oxygen * 100, world->atmosphere.co2 * 100, world->atmosphere.methane * 100);

    Tile* t = &world->tiles[world->cursor_y][world->cursor_x];
    printf("\n=== Cursor (%d,%d) ===\n", world->cursor_x, world->cursor_y);
    printf("Altitude: %.0fm | Water: %.0fm | Plate: %d\n", t->altitude, t->water_depth, t->plate_id);
    printf("Temp: %.1f°C | Moisture: %.0f%% | Ice: %.0f%%\n", t->temperature, t->moisture * 100, t->ice_coverage * 100);

    if (t->is_lake) {
        printf("[LAKE] ");
    }
    if (t->ocean_current.strength > 0.01) {
        printf("Current:%.2f ", t->ocean_current.strength);
    }
    if (t->wind.strength > 0.01) {
        printf("Wind:%.2f ", t->wind.strength);
    }
    if (t->magma_heat > 0.01) {
        printf("Magma:%.0f%%", t->magma_heat * 100);
    }
    printf("\n");

    const char* life_names[] = { "None", "Bacteria", "Algae", "Moss", "Grass",
                                 "Shrub", "Tree", "Small Animal", "Large Animal" };
    if (t->life != LIFE_NONE) {
        printf("Life: %s (age:%d)\n", life_names[t->life], t->age);
    }

    printf("\nControls: WASD/ER=move | 1-9=views | +/-=sea level | T=volcano | Space=pause\n");
}





// void init_terminal() {
//     tcgetattr(STDIN_FILENO, &old_tio);
//     new_tio = old_tio;
//     new_tio.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
//     fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
// }

// void restore_terminal() {
//     tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
// }

#include "dh/prl.h"

#if bti_plat_windows
#include "dh/os/windows/common.h"

fn_(engine_utils_getch(void), u8) {
    let console_input_handle = GetStdHandle(STD_INPUT_HANDLE);
    if (console_input_handle == INVALID_HANDLE_VALUE) { return 0; }

    DWORD original_console_mode = 0;
    if (!GetConsoleMode(console_input_handle, &original_console_mode)) { return 0; }

    let new_console_mode = original_console_mode & (~ENABLE_ECHO_INPUT) & (~ENABLE_LINE_INPUT);
    if (!SetConsoleMode(console_input_handle, new_console_mode)) { return 0; }

    u8    character_buffer = 0;
    DWORD characters_read  = 0;
    if (!ReadConsoleA(console_input_handle, &character_buffer, 1, &characters_read, null)
        || characters_read != 1) {
        character_buffer = 0;
    }
    SetConsoleMode(console_input_handle, original_console_mode);
    return character_buffer;
}
fn_(engine_utils_kbhit(void), bool) {
    let console_input_handle = GetStdHandle(STD_INPUT_HANDLE);
    if (console_input_handle == INVALID_HANDLE_VALUE) { return false; }

    DWORD total_events = 0;
    if (!GetNumberOfConsoleInputEvents(console_input_handle, &total_events)) { return false; }
    if (total_events <= 1) { return false; }

    INPUT_RECORD event_buffer[128] = {};
    DWORD        events_read       = 0;
    if (!PeekConsoleInput(console_input_handle, event_buffer, 128, &events_read)) { return false; }
    for (DWORD event_index = 0; event_index < events_read; ++event_index) {
        if (event_buffer[event_index].EventType == KEY_EVENT
            && event_buffer[event_index].Event.KeyEvent.bKeyDown) {
            return true;
        }
    }
    return false;
}

#else /* bti_plat_unix */
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

fn_(engine_utils_getch(void), u8) {
    struct termios original_terminal_attrs = {};
    if (tcgetattr(STDIN_FILENO, &original_terminal_attrs) < 0) { return 0; }

    let new_terminal_attrs = original_terminal_attrs;
    new_terminal_attrs.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_attrs) < 0) { return 0; }

    u8 character_buffer = 0;
    if (read(STDIN_FILENO, &character_buffer, 1) < 0) { character_buffer = 0; }
    tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_attrs);
    return character_buffer;
}
fn_(engine_utils_kbhit(void), bool) {
    struct termios original_terminal_attrs = {};
    if (tcgetattr(STDIN_FILENO, &original_terminal_attrs) < 0) { return false; }

    let new_terminal_attrs = original_terminal_attrs;
    new_terminal_attrs.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_attrs) < 0) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_attrs);
        return false;
    }

    let original_file_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (original_file_flags < 0) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_attrs);
        return false;
    }
    if (fcntl(STDIN_FILENO, F_SETFL, original_file_flags | O_NONBLOCK) < 0) {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_attrs);
        return false;
    }

    let character_from_input = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_attrs);
    fcntl(STDIN_FILENO, F_SETFL, original_file_flags);
    if (character_from_input != EOF) {
        ungetc(character_from_input, stdin);
        return true;
    }
    return false;
}
#endif /* bti_plat_unix */
