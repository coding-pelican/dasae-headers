// ============================================
// SimpleOMP를 사용한 파티클 시뮬레이션
// pthread + atomic 기반, OpenMP 불필요
// ============================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdatomic.h>

// SimpleOMP 핵심 부분만 포함 (헤더 파일 없이도 작동)
#ifndef SOMP_MAX_THREADS
#define SOMP_MAX_THREADS 16
#endif

static int global_num_threads = 4;

void somp_set_num_threads(int n) {
    if (n > 0 && n <= SOMP_MAX_THREADS) {
        global_num_threads = n;
    }
}

int somp_get_num_threads(void) {
    return global_num_threads;
}

// ============================================
// 파티클 구조체
// ============================================

typedef struct {
    float x, y;
    float vx, vy;
    float mass;
} Particle;

// ============================================
// 시뮬레이션 설정
// ============================================

#define NUM_PARTICLES (1 << 20)  // 2^20 = 1,048,576
#define BOUNDARY_RADIUS 500.0f
#define GRAVITY 9.81f
#define DAMPING 0.98f
#define DELTA_TIME (1.0f / 30.0f)

// Spatial hashing
#define CELL_SIZE 10.0f
#define GRID_WIDTH 200
#define GRID_HEIGHT 200
#define MAX_PARTICLES_PER_CELL 32

typedef struct {
    atomic_int count;
    int indices[MAX_PARTICLES_PER_CELL];
} Cell;

Particle* particles = NULL;
Cell* grid = NULL;

// ============================================
// 유틸리티
// ============================================

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

float gaussian_random(float mean, float stddev) {
    static int has_spare = 0;
    static float spare;
    
    if (has_spare) {
        has_spare = 0;
        return mean + stddev * spare;
    }
    
    has_spare = 1;
    float u, v, s;
    do {
        u = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        v = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
        s = u * u + v * v;
    } while (s >= 1.0f || s == 0.0f);
    
    s = sqrtf(-2.0f * logf(s) / s);
    spare = v * s;
    return mean + stddev * u * s;
}

// ============================================
// SimpleOMP Parallel For 구현
// ============================================

typedef void (*somp_loop_func_t)(int, void*);

typedef struct {
    int start;
    int end;
    somp_loop_func_t func;
    void* user_data;
} somp_thread_data_t;

static void* somp_worker(void* arg) {
    somp_thread_data_t* data = (somp_thread_data_t*)arg;
    for (int i = data->start; i < data->end; i++) {
        data->func(i, data->user_data);
    }
    return NULL;
}

void somp_parallel_for(int start, int end, somp_loop_func_t func, void* user_data) {
    int num_threads = global_num_threads;
    pthread_t threads[SOMP_MAX_THREADS];
    somp_thread_data_t data[SOMP_MAX_THREADS];
    
    int chunk = (end - start + num_threads - 1) / num_threads;
    
    for (int t = 0; t < num_threads; t++) {
        data[t].start = start + t * chunk;
        data[t].end = (data[t].start + chunk > end) ? end : data[t].start + chunk;
        data[t].func = func;
        data[t].user_data = user_data;
        
        if (data[t].start < end) {
            pthread_create(&threads[t], NULL, somp_worker, &data[t]);
        }
    }
    
    for (int t = 0; t < num_threads; t++) {
        if (data[t].start < end) {
            pthread_join(threads[t], NULL);
        }
    }
}

// ============================================
// 파티클 초기화
// ============================================

void init_particle_worker(int i, void* data) {
    float* params = (float*)data;
    float centerX = params[0];
    float centerY = params[1];
    float radiusA = params[2];
    float radiusB = params[3];
    
    unsigned int seed = time(NULL) + i;
    
    float r = gaussian_random(0.0f, radiusB / 2.5f);
    float theta = ((float)rand_r(&seed) / RAND_MAX) * 2.0f * M_PI;
    
    if (r > radiusB) r = radiusB;
    if (r < 0) r = -r;
    
    float px = centerX + r * cosf(theta);
    float py = centerY + r * sinf(theta);
    
    float dist = sqrtf(px * px + py * py);
    if (dist > radiusA) {
        float scale = radiusA / dist;
        px *= scale;
        py *= scale;
    }
    
    particles[i].x = px;
    particles[i].y = py;
    particles[i].vx = 0.0f;
    particles[i].vy = 0.0f;
    particles[i].mass = 1.0f;
}

void initialize_particles(float centerX, float centerY, float radiusA, float radiusB) {
    printf("Initializing %d particles...\n", NUM_PARTICLES);
    
    float params[4] = {centerX, centerY, radiusA, radiusB};
    somp_parallel_for(0, NUM_PARTICLES, init_particle_worker, params);
    
    printf("Initialization complete.\n");
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

void build_spatial_grid() {
    // 그리드 초기화
    somp_parallel_for(0, GRID_WIDTH * GRID_HEIGHT, clear_grid_worker, NULL);
    
    // 파티클을 그리드에 삽입
    for (int i = 0; i < NUM_PARTICLES; i++) {
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

void apply_gravity() {
    somp_parallel_for(0, NUM_PARTICLES, apply_gravity_worker, NULL);
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
            
            if (nx < 0 || nx >= GRID_WIDTH || ny < 0 || ny >= GRID_HEIGHT) 
                continue;
            
            int neighbor_idx = ny * GRID_WIDTH + nx;
            int count = atomic_load(&grid[neighbor_idx].count);
            
            for (int k = 0; k < count && k < MAX_PARTICLES_PER_CELL; k++) {
                int j = grid[neighbor_idx].indices[k];
                if (i >= j) continue;
                
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

void handle_collisions() {
    somp_parallel_for(0, NUM_PARTICLES, handle_collisions_worker, NULL);
}

// ============================================
// 위치 업데이트 및 경계 처리
// ============================================

void update_positions_worker(int i, void* data) {
    particles[i].x += particles[i].vx * DELTA_TIME;
    particles[i].y += particles[i].vy * DELTA_TIME;
    
    float dist = sqrtf(particles[i].x * particles[i].x + 
                       particles[i].y * particles[i].y);
    
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

void update_positions() {
    somp_parallel_for(0, NUM_PARTICLES, update_positions_worker, NULL);
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
        
        build_spatial_grid();
        apply_gravity();
        handle_collisions();
        update_positions();
        
        double frame_time = get_time() - frame_start;
        total_time += frame_time;
        
        if (frame % 30 == 0) {
            printf("Frame %d: %.2f ms (%.1f FPS) | Avg: %.2f ms (%.1f FPS)\n",
                   frame, frame_time * 1000.0, 1.0 / frame_time,
                   (total_time / (frame + 1)) * 1000.0, (frame + 1) / total_time);
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
        printf("Target 30 FPS not achieved (%.2f FPS)\n", 
               num_frames / total_time);
    }
}

// ============================================
// 메인
// ============================================

int main(int argc, char** argv) {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║  Particle Simulation (SimpleOMP)      ║\n");
    printf("║  pthread + atomic (No OpenMP!)        ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");
    
    printf("Particles: 2^20 = %d\n", NUM_PARTICLES);
    printf("Target FPS: 30\n");
    
    // 스레드 수 설정
    int num_threads = 4;
    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }
    somp_set_num_threads(num_threads);
    
    // 메모리 할당
    printf("\nAllocating memory...\n");
    particles = (Particle*)malloc(NUM_PARTICLES * sizeof(Particle));
    grid = (Cell*)calloc(GRID_WIDTH * GRID_HEIGHT, sizeof(Cell));
    
    if (!particles || !grid) {
        printf("ERROR: Failed to allocate memory!\n");
        return 1;
    }
    
    printf("Memory allocated: %.2f MB\n", 
           (NUM_PARTICLES * sizeof(Particle)) / (1024.0 * 1024.0));
    
    // 초기화
    srand(time(NULL));
    initialize_particles(0.0f, 0.0f, BOUNDARY_RADIUS, 200.0f);
    
    // 시뮬레이션 실행
    int num_frames = 100;
    if (argc > 2) {
        num_frames = atoi(argv[2]);
    }
    
    simulate(num_frames);
    
    // 결과 샘플 출력
    printf("\nSample particles:\n");
    for (int i = 0; i < 3; i++) {
        printf("Particle %d: pos(%.2f, %.2f) vel(%.2f, %.2f)\n",
               i, particles[i].x, particles[i].y,
               particles[i].vx, particles[i].vy);
    }
    
    // 정리
    free(particles);
    free(grid);
    
    printf("\n=== Program Complete ===\n");
    return 0;
}
