// ============================================
// SimpleOMP를 사용한 파티클 시뮬레이션
// pthread + atomic 기반, OpenMP 불필요
// ============================================

#include "dh/prl.h"
#include "dh/Thrd.h"

#define mp_max_thrd_count 32

$static var_(mp__thrd_count, usize) = 4;
$static fn_((mp_setThrdCount(usize thrd_count))(void)) { mp__thrd_count = thrd_count; }
$static fn_((mp_getThrdCount(void))(usize)) { return mp__thrd_count; }

typedef fn_(((*)(usize, u_V$raw))(void) $T) mp_LoopFn;
typedef struct mp_LoopData {
    var_(range, R);
    var_(workerFn, mp_LoopFn);
    var_(params, u_V$raw);
} mp_LoopData;

$static Thrd_fn_(mp_worker, ({ mp_LoopData data; }, Void), ($ignore, args)$scope) {
    let data = args->data;
    for_(((data.range))(i) {
        data.workerFn(i, data.params);
    });
    return_({});
} $unscoped_(Thrd_fn);

$static fn_((mp_parallel_for(R range, mp_LoopFn func, u_V$raw user_data))(void)) {
    let thrd_count = mp_getThrdCount();

    var_(data_list_buf, A$$(mp_max_thrd_count, mp_LoopData)) = A_zero();
    let data_list = slice$A(data_list_buf, $r(0, thrd_count));
    var_(workers_buf, A$$(mp_max_thrd_count, Thrd_FnCtx$(mp_worker))) = A_zero();
    let workers = slice$A(workers_buf, $r(0, thrd_count));
    var_(threads_buf, A$$(mp_max_thrd_count, Thrd)) = A_zero();
    let threads = slice$A(threads_buf, $r(0, thrd_count));

    let chunk = (range.end - range.begin + thrd_count - 1) / thrd_count;
    for_(($rf(0), $s(workers), $s(data_list))(t, worker, data) {
        data->range.begin = range.begin + chunk * t;
        data->range.end = ((data->range.begin + chunk) <= range.end) ? (data->range.begin + chunk) : range.end;
        data->workerFn = func;
        data->params = user_data;
        *worker = Thrd_FnCtx_from$((mp_worker)(*data));
    });
    for_(($s(threads), $s(workers))(thread, worker) {
        *thread = catch_((Thrd_spawn(Thrd_SpawnCfg_default, worker->as_raw))(
            $ignore, claim_unreachable
        ));
    });
    for_(($s(threads))(thread) {
        Thrd_join(*thread);
    });
}

// ============================================
// 유틸리티
// ============================================

#include "dh/math/common.h"
#include "dh/math/vec.h"
#include "dh/Rand.h"

fn_((Rand_gaussian$f32(f32 mean, f32 std_dev))(f32)) {
    static O$f32 spare = none();
    if_some((spare)(value)) {
        return mean + std_dev * value;
    }

    f32 u = f32_nan, v = f32_nan, s = f32_nan;
    do {
        u = (Rand_next$f32() * 2.0f - 1.0f);
        v = (Rand_next$f32() * 2.0f - 1.0f);
        s = u * u + v * v;
    } while (s >= 1.0f || s == 0.0f);
    s = sqrtf(-2.0f * logf(s) / s);
    asg$O((&spare)(some(v * s)));
    return mean + std_dev * u * s;
}

// ============================================
// 파티클 구조체
// ============================================

typedef struct Particle {
    m_V2f32 pos;
    m_V2f32 vel;
    f32 mass;
} Particle;
T_use_S$(Particle);

// ============================================
// 시뮬레이션 설정
// ============================================

#define NUM_PARTICLES_LOG2 20u
#define num_particles (1u << NUM_PARTICLES_LOG2) // 2^20 = 1,048,576
#define BOUNDARY_RADIUS 500.0f
#define GRAVITY 9.81f
#define DAMPING 0.98f
#define TARGET_FPS 30.0f
#define DELTA_TIME (1.0f / 30.0f)

// Spatial hashing
#define CELL_SIZE 10.0f
#define GRID_WIDTH 200
#define GRID_HEIGHT 200
#define MAX_PARTICLES_PER_CELL 32

typedef struct Cell {
    usize count;
    usize indices[MAX_PARTICLES_PER_CELL];
} Cell;
T_use_S$(Cell);

// ============================================
// 파티클 초기화
// ============================================

typedef struct State_InitParticlesArgs {
    S$Particle particles;
    f32 center_x;
    f32 center_y;
    f32 radius_a;
    f32 radius_b;
} InitParticleWorkerData;

fn_((init_particle_worker(usize i, u_V$raw data))(void)) {
    let params = u_castV$((InitParticleWorkerData)(data));
    let particles = params.particles;
    let center_x = params.center_x;
    let center_y = params.center_y;
    let radius_a = params.radius_a;
    let radius_b = params.radius_b;

    var r = Rand_gaussian$f32(0.0f, radius_b / 2.5f);
    let theta = Rand_next$f32() * 2.0f * math_pi;

    if (r > radius_b) {
        r = radius_b;
    }
    if (r < 0) {
        r = -r;
    }

    var px = center_x + r * cosf(theta);
    var py = center_y + r * sinf(theta);
    let dist = sqrtf(px * px + py * py);
    if (dist > radius_a) {
        let scale = radius_a / dist;
        px *= scale;
        py *= scale;
    }

    let particle = at$S(particles, i);
    particle->pos.x = px;
    particle->pos.y = py;
    particle->vel.x = 0.0f;
    particle->vel.y = 0.0f;
    particle->mass = 1.0f;
}

fn_((init_particles(S$Particle particles, f32 center_x, f32 center_y, f32 radius_a, f32 radius_b))(void)) {
    mp_parallel_for($r(0, num_particles), init_particle_worker, u_anyV(lit$((InitParticleWorkerData){
                                                                    .particles = particles,
                                                                    .center_x = center_x,
                                                                    .center_y = center_y,
                                                                    .radius_a = radius_a,
                                                                    .radius_b = radius_b,
                                                                })));
}

// ============================================
// Spatial Grid 구축
// ============================================

inline int hash_position(float x, float y) {
    int gx = (int)((x + GRID_WIDTH * CELL_SIZE / 2.0f) / CELL_SIZE);
    int gy = (int)((y + GRID_HEIGHT * CELL_SIZE / 2.0f) / CELL_SIZE);

    gx = (gx < 0) ? 0 : (gx >= GRID_WIDTH ? GRID_WIDTH - 1 : gx);
    gy = (gy < 0) ? 0 : (gy >= GRID_HEIGHT ? GRID_HEIGHT - 1 : gy);

    return gy * GRID_WIDTH + gx;
}

void clear_grid_worker(int i, void* data) {
    atomic_store(&grid[i].count, 0);
}

void State_buildSpatialGrid() {
    // 그리드 초기화
    mp_parallel_for(0, GRID_WIDTH * GRID_HEIGHT, clear_grid_worker, NULL);

    // 파티클을 그리드에 삽입
    for (int i = 0; i < num_particles; i++) {
        int cell_idx = hash_position(particles[i].x, particles[i].y);

        int pos = atomic_fetch_add(&grid[cell_idx].count, 1);
        if (pos < MAX_PARTICLES_PER_CELL) {
            grid[cell_idx].indices[pos] = i;
        }
    }
}

// ============================================
// 중력 적용
// ============================================

void apply_gravity_worker(int i, void* data) {
    float dx = -particles[i].x;
    float dy = -particles[i].y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist > 0.1f) {
        float force = GRAVITY / (dist * dist);
        particles[i].vx += (dx / dist) * force * DELTA_TIME;
        particles[i].vy += (dy / dist) * force * DELTA_TIME;
    }

    particles[i].vx *= DAMPING;
    particles[i].vy *= DAMPING;
}

void State_applyGravity() {
    mp_parallel_for($r(0, num_particles), apply_gravity_worker, NULL);
}

// ============================================
// 충돌 처리
// ============================================

void handle_collisions_worker(int i, void* data) {
    const float COLLISION_RADIUS = 0.5f;

    int cell_idx = hash_position(particles[i].x, particles[i].y);
    int gx = cell_idx % GRID_WIDTH;
    int gy = cell_idx / GRID_WIDTH;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int nx = gx + dx;
            int ny = gy + dy;

            if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT) {
                continue;
            }

            int neighbor_idx = ny * GRID_WIDTH + nx;
            int count = atomic_load(&grid[neighbor_idx].count);

            for (int k = 0; k < count && k < MAX_PARTICLES_PER_CELL; k++) {
                int j = grid[neighbor_idx].indices[k];
                if (i >= j) {
                    continue;
                }

                float dx = particles[j].x - particles[i].x;
                float dy = particles[j].y - particles[i].y;
                float dist_sq = dx * dx + dy * dy;
                float min_dist = COLLISION_RADIUS * 2.0f;

                if (dist_sq < min_dist * min_dist && dist_sq > 0.001f) {
                    float dist = sqrtf(dist_sq);
                    float overlap = min_dist - dist;

                    float nx = dx / dist;
                    float ny = dy / dist;

                    float separation = overlap * 0.5f;
                    particles[i].x -= nx * separation;
                    particles[i].y -= ny * separation;
                }
            }
        }
    }
}

void State_handleCollisions() {
    mp_parallel_for($r(0, num_particles), handle_collisions_worker, NULL);
}

// ============================================
// 위치 업데이트 및 경계 처리
// ============================================

void update_positions_worker(int i, void* data) {
    particles[i].x += particles[i].vx * DELTA_TIME;
    particles[i].y += particles[i].vy * DELTA_TIME;

    float dist = sqrtf(particles[i].x * particles[i].x + particles[i].y * particles[i].y);

    if (dist > BOUNDARY_RADIUS) {
        float nx = particles[i].x / dist;
        float ny = particles[i].y / dist;

        particles[i].x = nx * BOUNDARY_RADIUS;
        particles[i].y = ny * BOUNDARY_RADIUS;

        float dot = particles[i].vx * nx + particles[i].vy * ny;
        particles[i].vx -= 2.0f * dot * nx;
        particles[i].vy -= 2.0f * dot * ny;
        particles[i].vx *= 0.8f;
        particles[i].vy *= 0.8f;
    }
}

void State_updatePositions() {
    mp_parallel_for(0, num_particles, update_positions_worker, NULL);
}

// ============================================
// 시뮬레이션 루프
// ============================================

void simulate(int num_frames) {
    printf("\nStarting simulation for %d frames at 30 FPS...\n", num_frames);
    printf("Using %d threads (SimpleOMP)\n\n", global_num_threads);

    double total_time = 0.0;

    for (int frame = 0; frame < num_frames; frame++) {
        double frame_start = get_time();

        State_buildSpatialGrid();
        State_applyGravity();
        State_handleCollisions();
        State_updatePositions();

        double frame_time = get_time() - frame_start;
        total_time += frame_time;

        if (frame % 30 == 0) {
            printf("Frame %d: %.2f ms (%.1f FPS) | Avg: %.2f ms (%.1f FPS)\n", frame, frame_time * 1000.0, 1.0 / frame_time, (total_time / (frame + 1)) * 1000.0, (frame + 1) / total_time);
        }
    }

    printf("\n=== Simulation Complete ===\n");
    printf("Total frames: %d\n", num_frames);
    printf("Total time: %.2f seconds\n", total_time);
    printf("Average FPS: %.2f\n", num_frames / total_time);
    printf("Average frame time: %.2f ms\n", (total_time / num_frames) * 1000.0);

    if (num_frames / total_time >= 30.0f) {
        printf("✓ SUCCESS: 30 FPS achieved!\n");
    } else {
        printf("Target 30 FPS not achieved (%.2f FPS)\n", num_frames / total_time);
    }
}

// ============================================
// 메인
// ============================================

#include "dh/main.h"
#include "dh/io/stream.h"
#include "dh/fmt/common.h"
#include "dh/heap/Page.h"
#include "dh/Rand.h"

typedef struct State {
    S$Particle particles;
    S$Cell grid;
} State;

fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    io_stream_println(u8_l("╔═══════════════════════════════════════╗"));
    io_stream_println(u8_l("║  Particle Simulation (SimpleOMP)      ║"));
    io_stream_println(u8_l("║  pthread + atomic (No OpenMP!)        ║"));
    io_stream_println(u8_l("╚═══════════════════════════════════════╝\n"));

    io_stream_println(u8_l("Particles: 2^{:u} = {:u}"), num_particles_log2, num_particles);
    io_stream_println(u8_l("Target FPS: {:.1f}"), TARGET_FPS);

    mp_setThrds(expr_(usize $scope) if (1 < args.len) {
        $break_(try_(fmt_parse$usize(*at$S(args, 1), 10)));
    } else {
        $break_(4);
    } $unscoped_(expr));

    io_stream_print(u8_l("\nAllocating memory...\n"));
    var page = lit$((heap_Page){});
    let gpa = heap_Page_allocator(&page);
    T_use_S$(Particle);
    let particles = try_(u_castE$((E$$(S$(Particle)))(mem_Allocator_alloc(gpa, typeInfo$(Particle), num_particles))));
    defer_(mem_Allocator_free(gpa, u_anyS(particles)));
    T_use_S$(Cell);
    let grid = try_(u_castE$((E$$(S$(Cell)))(mem_Allocator_alloc(gpa, typeInfo$(Cell), GRID_WIDTH * GRID_HEIGHT))));
    defer_(mem_Allocator_free(gpa, u_anyS(grid)));
    io_stream_println(u8_l("Memory allocated: {:.2f} MB"), ((usize)num_particles * typeInfo$(Particle).size) / (1024.0 * 1024.0));

    Rand_init();
    init_particles(particles, m_V2f32_zero, m_V2f32_from(BOUNDARY_RADIUS, 200.0f))

        State_simulate(expr_(usize $scope) if (2 < args.len) {
            $break_(try_(fmt_parse$usize(*at$S(args, 2), 10)));
        } else {
            $break_(100);
        } $unscoped_(expr));

    io_stream_println(u8_l("\nSample particles:\n"));
    for_(($r(0, 3), $s(particles))(i, particle) {
        io_stream_println(u8_l("Particle {:uz}: pos({:.2f}, {:.2f}) vel({:.2f}, {:.2f})"), i, particle.x, particle.y, particle.vx, particle.vy);
    });
    io_stream_println(u8_l("\n=== Program Complete ===\n"));
    return_ok({});
} $unguarded_(fn);
