/* Actor.h */
#ifndef Actor__included
#define Actor__included 1

#include "dh/async.h"
use_Co_Ctx$(Void);

#include "dh/mem/Allocator.h"
#include "dh/heap/Arena.h"

// 메시지 타입
typedef enum MessageType : u8 {
    MessageType_none = 0,
    MessageType_increment = 1,
    MessageType_get_value = 2,
    MessageType_print = 3,
    MessageType_broadcast = 4,
    MessageType_stop = 5,
    BoidMsg_neighbor_info = 100, // 이웃 정보
    BoidMsg_update = 101,        // 프레임 업데이트
    BoidMsg_stop = 102,
} MessageType;

// 메시지 구조 (작은 크기로 최적화)
typedef struct Message {
    MessageType tag;
    union {
        u64 u64_val;
        i64 i64_val;
        void* ptr_val;
        A$$(48, u8) inline_data;
    } payload;
    struct Message* next;
} Message;
T_use$((Message)(P, O, E));

// 메일박스 (침입적 연결 리스트)
typedef struct Mailbox {
    Message* head;
    Message* tail;
    usize count;
} Mailbox;

// Actor 구조
typedef struct Actor {
    Co_Ctx* coroutine; // 코루틴 컨텍스트
    Mailbox mailbox;   // 메시지 큐
    P$raw state;       // Actor 상태 (타입 소거)
    u32 id;            // Actor ID
    bool active       : 1;
    bool has_messages : 1;
} Actor;
T_use$((Actor)(P, S));
T_use_E$(P$Actor);

// Actor 시스템 (Arena allocator 사용)
typedef struct ActorSystem {
    mem_Allocator allocator;  // 기본 allocator
    heap_Arena arena_actor;   // Actor 전용 arena
    heap_Arena arena_message; // 메시지 전용 arena
    S$Actor actors;           // Actor 슬라이스
    usize next_id;
} ActorSystem;
T_use_E$(ActorSystem);

// Actor 시스템 초기화
fn_((ActorSystem_init(mem_Allocator base_allocator, usize max_actors))(E$ActorSystem));
// Actor 시스템 종료 (모든 Actor/메시지 한번에 해제)
fn_((ActorSystem_fini(ActorSystem* self))(void));
// Actor 런타임 실행
fn_((ActorSystem_runLoop(ActorSystem* self, bool endless))(void));
// Actor 생성
fn_((ActorSystem_spawn(ActorSystem* self, Co_Ctx* coroutine, P$raw initial_state))(E$P$Actor));

// 메시지 전송
fn_((Actor_send(ActorSystem* sys, Actor* self, Message msg))(E$void));
// 메시지 수신 (시도)
fn_((Actor_tryRecv(ActorSystem* sys, Actor* self))(O$Message));

#endif /* Actor__included */

/* Actor.c */
#include "dh/io/stream.h"

// Actor 시스템 초기화
fn_((ActorSystem_init(mem_Allocator base_allocator, usize max_actors))(E$ActorSystem) $scope) {
    ActorSystem sys = {
        .allocator = base_allocator,
        .arena_actor = heap_Arena_init(base_allocator),
        .arena_message = heap_Arena_init(base_allocator),
        .next_id = 0,
    };
    sys.actors = blk({
        let gpa = heap_Arena_allocator(&sys.arena_actor);
        let actors = u_castS$((S$Actor)(try_(mem_Allocator_alloc(gpa, typeInfo$(InnerType), max_actors))));
        for_(($s(actors))(actor) {
            actor->coroutine = null;
            actor->mailbox = (Mailbox){ 0 };
            actor->state = null;
            actor->id = 0;
            actor->active = false;
            actor->has_messages = false;
        });
        blk_return_(actors);
    });
    return_ok(sys);
} $unscoped_(fn);

// Actor 시스템 종료
fn_((ActorSystem_fini(ActorSystem* self))(void)) {
    heap_Arena_fini(self->arena_message);
    heap_Arena_fini(self->arena_actor);
}

// Actor 런타임
fn_((ActorSystem_runLoop(ActorSystem* self, bool endless))(void)) {
    io_stream_println(u8_l("[ActorSystem] Starting event loop"));
    while_(usize iteration = 0, true) {
        bool any_active = false;
        usize messages_processed = 0;
        // 모든 Actor 순회
        for_(($s(self->actors))(actor) {
            if (!actor->active) { continue; }
            any_active = true;
            if (actor->has_messages) {
                resume_(actor->coroutine);
                messages_processed++;
                if (actor->coroutine->state == Co_State_ready) {
                    actor->active = false;
                    // io_stream_println(u8_l("[Actor {:u}] Completed"), actor->id);
                }
            }
        });
        if (iteration % 1000 == 0 && messages_processed > 0) {
            io_stream_println(
                u8_l("[ActorSystem] Iteration {:zu}: processed {:zu} messages"),
                iteration, messages_processed
            );
        }
        if (!endless && (!any_active || messages_processed == 0)) { break; }
        iteration++;
    }
    io_stream_println(u8_l("[ActorSystem] Event loop finished"));
}

// Actor 생성
fn_((ActorSystem_spawn(ActorSystem* self, Co_Ctx* coroutine, P$raw initial_state))(E$P$Actor) $scope) {
    // 빈 슬롯 찾기
    let free_actor = expr_(P$Actor $scope)(if (self->next_id < self->actors.len) {
        $break_(at$S(self->actors, self->next_id));
    }) expr_(else)({
        return_err(mem_Err_OutOfMemory());
    }) $unscoped_(expr);

    // Actor 초기화
    free_actor->coroutine = coroutine;
    free_actor->mailbox = (Mailbox){ 0 };
    free_actor->state = initial_state;
    free_actor->id = self->next_id++;
    free_actor->active = true;
    free_actor->has_messages = false;

    return_ok(free_actor);
} $unscoped_(fn);

// 메시지 전송 (arena에서 할당)
fn_((Actor_send(ActorSystem* sys, Actor* self, Message msg))(E$void) $scope) {
    let gpa = heap_Arena_allocator(&sys->arena_message);
    // 메시지 복사 (arena에서)
    let new_msg = u_castP$((P$Message)(try_(mem_Allocator_create(gpa, typeInfo$(InnerType)))));
    *new_msg = msg;
    new_msg->next = null;
    // 메일박스에 추가
    if (self->mailbox.tail) {
        self->mailbox.tail->next = new_msg;
        self->mailbox.tail = new_msg;
    } else {
        self->mailbox.head = self->mailbox.tail = new_msg;
    }
    self->mailbox.count++;
    self->has_messages = true;
    return_ok({});
} $unscoped_(fn);

// 메시지 수신
fn_((Actor_tryRecv(ActorSystem* sys, Actor* self))(O$Message) $scope) {
    let_ignore = sys;
    if (!self->mailbox.head) {
        self->has_messages = false;
        return_none();
    }

    let_(msg, Message) = *self->mailbox.head;
    self->mailbox.head = msg.next;
    if (!self->mailbox.head) {
        self->mailbox.tail = null;
        self->has_messages = false;
    }
    self->mailbox.count--;
    return_some(msg);
} $unscoped_(fn);

/* boids_demo.c */
#include "dh/main.h"
#include "dh/time.h"
#include "dh/math.h"

// Boid 상태
typedef struct BoidState {
    P$Actor self;
    u32 id;

    m_V2f32 pos;
    m_V2f32 vel;

    // 이웃 정보 누적 (메시지로 받음)
    m_V2f32 neighbor_sum_pos;
    m_V2f32 neighbor_sum_vel;
    u32 neighbor_count;

    // 분리 힘
    m_V2f32 separation;
} BoidState;
T_use_P$(BoidState);
T_use_S$(BoidState);

// 이웃 정보 메시지
typedef struct NeighborInfo {
    m_V2f32 pos;
    m_V2f32 vel;
} NeighborInfo;

// Boids 파라미터
typedef struct BoidsParams {
    f32 separation_radius; // 분리 반경
    f32 alignment_radius;  // 정렬 반경
    f32 cohesion_radius;   // 응집 반경
    f32 separation_force;  // 분리 힘
    f32 alignment_force;   // 정렬 힘
    f32 cohesion_force;    // 응집 힘
    f32 max_speed;         // 최대 속도
    f32 world_width;
    f32 world_height;
} BoidsParams;

// // Spatial Grid (이웃 찾기 최적화)
// typedef struct {
//     u32 width, height;      // 그리드 크기
//     f32 cell_size;
//     S$S$P$Actor grid; // 2D 그리드
// } SpatialGrid;

// 벡터 계산 헬퍼
$static $inline_always
fn_((m_V2f32_limit(m_V2f32 self, f32 max))(m_V2f32)) {
    let len = m_V2f32_len(self);
    if (len > max) {
        return m_V2f32_scale(self, max / len);
    }
    return self;
}

// // Spatial Grid 초기화
// fn_((SpatialGrid_init(mem_Allocator allocator, f32 world_width, f32 world_height, f32 cell_size))(E$SpatialGrid) $scope) {
//     SpatialGrid grid = {
//         .width = (u32)(world_width / cell_size) + 1,
//         .height = (u32)(world_height / cell_size) + 1,
//         .cell_size = cell_size,
//     };

//     // 그리드 할당
//     grid.grid = meta_cast$(S$S$P$Actor,
//         try_(mem_Allocator_alloc(allocator, typeInfo$(S$P$Actor), grid.width * grid.height)));

//     // 각 셀 초기화
//     for_(($s(grid.grid))(cell) {
//         *cell = (S$P$Actor){ .buf = null, .len = 0 };
//     });

//     return_ok(grid);
// } $unscoped_(fn);

// // Boid를 그리드에 추가
// fn_((SpatialGrid_insert(SpatialGrid* grid, BoidState* boid, P$Actor actor))(void)) {
//     u32 cell_x = (u32)(boid->x / grid->cell_size);
//     u32 cell_y = (u32)(boid->y / grid->cell_size);

//     if (cell_x >= grid->width) cell_x = grid->width - 1;
//     if (cell_y >= grid->height) cell_y = grid->height - 1;

//     u32 cell_idx = cell_y * grid->width + cell_x;
//     // Note: 실제로는 동적 배열이 필요하지만, 데모용으로는 고정 크기 사용
// }

// // 이웃 찾기 (그리드 기반)
// fn_((SpatialGrid_getNeighbors(SpatialGrid* grid, BoidState* boid, f32 radius))(S$P$Actor)) {
//     u32 cell_x = (u32)(boid->x / grid->cell_size);
//     u32 cell_y = (u32)(boid->y / grid->cell_size);

//     // 주변 9개 셀 검사
//     // 실제 구현에서는 동적 배열 반환
//     // 데모에서는 단순화
//     return (S$P$Actor){ .buf = null, .len = 0 };
// }

T_use_S$(P$Actor);
// Boid Actor
async_fn_(boid_actor, (var_(sys, ActorSystem*); var_(self, P$Actor); var_(params, const BoidsParams*); var_(all_boids, S$P$Actor);), Void);

async_fn_scope(boid_actor, {
    var_(msg, Message);
    var_(frame_count, u32);
}) {
    let state = as$(BoidState*)(args->self->state);
    locals->frame_count = 0;

    while (true) {
        if_none(Actor_tryRecv(args->sys, args->self)) {
            suspend_({});
            continue;
        } else_some(msg) {
            locals->msg = msg;
        }

        switch (locals->msg.tag) {
        case BoidMsg_neighbor_info: {
            // 이웃 정보 받음
            let info = *as$(NeighborInfo*)(locals->msg.payload.inline_data.val);

            let diff = m_V2f32_sub(info.pos, state->pos);
            let dist = m_V2f32_len(diff);
            if (dist < 0.001f) { break; } // 자기 자신

            // Separation (분리) - 가까운 이웃 피하기
            if (dist < args->params->separation_radius) {
                m_V2f32_subAsg(&state->separation, m_V2f32_scaleInv(diff, dist));
            }

            // Alignment & Cohesion용 정보 누적
            if (dist < args->params->alignment_radius) {
                m_V2f32_addAsg(&state->neighbor_sum_pos, info.pos);
                m_V2f32_addAsg(&state->neighbor_sum_vel, info.vel);
                state->neighbor_count++;
            }
        } break;

        case BoidMsg_update: {
            // 프레임 업데이트
            if (state->neighbor_count > 0) {
                // Alignment (정렬) - 이웃의 평균 속도
                let avg_vel = m_V2f32_scaleInv(state->neighbor_sum_vel, as$(f32)(state->neighbor_count));
                let align_diff = m_V2f32_sub(avg_vel, state->vel);

                // Cohesion (응집) - 이웃의 평균 위치로
                let avg_pos = m_V2f32_scaleInv(state->neighbor_sum_pos, as$(f32)(state->neighbor_count));
                let cohesion_diff = m_V2f32_sub(avg_pos, state->pos);

                // 힘 적용
                m_V2f32_addAsg(&state->vel, m_V2f32_scale(state->separation, args->params->separation_force));
                m_V2f32_addAsg(&state->vel, m_V2f32_scale(align_diff, args->params->alignment_force));
                m_V2f32_addAsg(&state->vel, m_V2f32_scale(cohesion_diff, args->params->cohesion_force));

                // 속도 제한
                state->vel = m_V2f32_limit(state->vel, args->params->max_speed);
            }

            // 위치 업데이트
            m_V2f32_addAsg(&state->pos, state->vel);

            // 화면 wrap (월드 경계)
            if (state->pos.x < 0) { state->pos.x += args->params->world_width; }
            if (state->pos.x >= args->params->world_width) { state->pos.x -= args->params->world_width; }
            if (state->pos.y < 0) { state->pos.y += args->params->world_height; }
            if (state->pos.y >= args->params->world_height) { state->pos.y -= args->params->world_height; }

            // 다음 프레임을 위해 초기화
            state->neighbor_sum_pos = m_V2f32_zero;
            state->neighbor_sum_vel = m_V2f32_zero;
            state->neighbor_count = 0;
            state->separation = m_V2f32_zero;

            // 모든 다른 Boid에게 내 정보 브로드캐스트
            // (실제로는 그리드 기반 최적화 필요)
            NeighborInfo my_info = {
                .pos = state->pos,
                .vel = state->vel,
            };
            Message neighbor_msg = {
                .tag = BoidMsg_neighbor_info,
                .payload = {},
            };
            prim_memcpy(neighbor_msg.payload.inline_data.val, &my_info, sizeof(NeighborInfo));

            // 이웃에게만 전송 (거리 체크)
            // 데모에서는 단순화: 모든 boid에게 전송 (느릴 수 있음)
            for_(($s(args->all_boids))(other_slot) {
                let other = *other_slot;
                if (other != args->self && other->active) {
                    // 거리 체크 (최적화)
                    let other_state = as$(BoidState*)(other->state);
                    let diff = m_V2f32_sub(other_state->pos, state->pos);
                    let dist = m_V2f32_len(diff);
                    if (dist < args->params->alignment_radius) {
                        catch_((Actor_send(args->sys, other, neighbor_msg))(err, {
                            Err_print(err);
                            ErrTrace_print();
                            areturn_({});
                        }));
                    }
                }
            });

            locals->frame_count++;
        } break;

        case BoidMsg_stop: {
            io_stream_println(
                u8_l("[Boid {:u}] Processed {:u} frames"),
                state->id, locals->frame_count
            );
            areturn_({});
        } break;

        default:
            break;
        }
    }

    areturn_({});
} $unscoped_(async_fn);

#include "dh/heap/Page.h"
#include "dh/Rand.h"

// 메인
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    Rand_init();

    // Page allocator
    var page = (heap_Page){};
    let page_gpa = heap_Page_allocator(&page);

    io_stream_println(u8_l("=== 100K Boids Simulation ===\n"));

    // Boids 파라미터
    BoidsParams params = {
        .separation_radius = 25.0f,
        .alignment_radius = 50.0f,
        .cohesion_radius = 50.0f,
        .separation_force = 0.05f,
        .alignment_force = 0.03f,
        .cohesion_force = 0.02f,
        .max_speed = 4.0f,
        .world_width = 1920.0f,
        .world_height = 1080.0f,
    };

    // Actor 시스템 초기화
    const usize num_boids = lit_num(100);
    // const usize num_boids = lit_num(100, 000);
    io_stream_println(u8_l("Initializing actor system for {:zu} boids..."), num_boids);

    var sys = try_(ActorSystem_init(page_gpa, num_boids));
    defer_(ActorSystem_fini(&sys));

    let actor_gpa = heap_Arena_allocator(&sys.arena_actor);

    // Boid 상태 할당
    io_stream_println(u8_l("Allocating boid states..."));
    let states = u_castS$((S$BoidState)(try_(mem_Allocator_alloc(actor_gpa, typeInfo$(InnerType), num_boids))));
    errdefer_($ignore, mem_Allocator_free(actor_gpa, u_anyS(states)));

    // 코루틴 할당
    io_stream_println(u8_l("Allocating coroutines..."));
    let ctxs = u_castS$((S$$(Co_CtxFn$(boid_actor)))(try_(mem_Allocator_alloc(actor_gpa, typeInfo$(InnerType), num_boids))));
    errdefer_($ignore, mem_Allocator_free(actor_gpa, u_anyS(ctxs)));

    // Actors 슬라이스
    let actors = u_castS$((S$P$Actor)(try_(mem_Allocator_alloc(actor_gpa, typeInfo$(InnerType), num_boids))));

    // Boids 생성
    io_stream_println(u8_l("Creating {:zu} boid actors..."), num_boids);
    var start = time_Instant_now();

    var rng = Rand_default;
    for_(($rf(0), $s(states), $s(ctxs), $s(actors))(i, state, ctx, actor_slot) {
        *state = lit$((BoidState){
            .self = *actor_slot,
            .id = i,
            .pos = m_V2f32_from(
                as$(f32)(Rand_next$u32(&rng) % as$(u32)((params.world_width))),
                as$(f32)(Rand_next$u32(&rng) % as$(u32)((params.world_height)))),
            .vel = m_V2f32_from(
                (as$(f32)(Rand_next$u32(&rng) % 100) / 50.0f - 1.0f) * params.max_speed,
                (as$(f32)(Rand_next$u32(&rng) % 100) / 50.0f - 1.0f) * params.max_speed
            ),
            .neighbor_count = 0,
            .separation = m_V2f32_zero,
        });
        // 랜덤 위치와 속도
        state->id = i;
        state->pos = m_V2f32_from(
            as$(f32)(Rand_next$u32(&rng) % as$(u32)((params.world_width))),
            as$(f32)(Rand_next$u32(&rng) % as$(u32)((params.world_height))));
        state->vel = m_V2f32_from(
            (as$(f32)(Rand_next$u32(&rng) % 100) / 50.0f - 1.0f) * params.max_speed,
            (as$(f32)(Rand_next$u32(&rng) % 100) / 50.0f - 1.0f) * params.max_speed
        );
        state->neighbor_count = 0;
        state->separation = m_V2f32_zero;

        // 코루틴 초기화
        *ctx = Co_CtxFn_init$(boid_actor);
        ctx->args.sys = &sys;
        ctx->args.self = try_(ActorSystem_spawn(&sys, ctx->anyraw, state));
        ctx->args.params = &params;
        ctx->args.all_boids = actors; // 모든 boid 참조
        *actor_slot = ctx->args.self;

        if (i % lit_num(10, 000) == 0) {
            io_stream_println(u8_l("  Created {:zu} boids"), i);
        }
    });

    let create_time = time_Instant_elapsed(start);
    io_stream_println(
        u8_l("Created {:zu} boids in {:f} seconds\n"),
        num_boids,
        time_Duration_asSecs$f64(create_time)
    );

    // 메모리 사용량
    let actor_mem = heap_Arena_queryCap(&sys.arena_actor);
    let msg_mem = heap_Arena_queryCap(&sys.arena_message);
    io_stream_println(u8_l("Memory usage:"));
    io_stream_println(u8_l("  Actors:   {:zu} MB"), actor_mem / lit_n$(usize)(1024) * 1024);
    io_stream_println(u8_l("  Messages: {:zu} MB\n"), msg_mem / lit_n$(usize)(1024) * 1024);

    // 시뮬레이션 실행
    const usize num_frames = 100; // 100 프레임
    io_stream_println(u8_l("Running simulation for {:zu} frames..."), num_frames);
    start = time_Instant_now();

    for_(($r(0, num_frames))(frame) {
        // 모든 boid에게 update 메시지
        Message update_msg = { .tag = BoidMsg_update };
        for_(($s(actors))(actor_slot) {
            let actor = *actor_slot;
            if (actor->active) {
                try_(Actor_send(&sys, actor, update_msg));
            }
        });

        // 한 프레임 처리
        ActorSystem_runLoop(&sys, false);

        if (frame % 10 == 0) {
            io_stream_println(u8_l("  Frame {:zu}/{:zu}"), frame, num_frames);
        }
    });

    let sim_time = time_Instant_elapsed(start);
    io_stream_println(
        u8_l("Completed {:zu} frames in {:f} seconds"),
        num_frames,
        time_Duration_asSecs$f64(sim_time)
    );

    // 성능 메트릭
    let fps = as$(f64)(num_frames) / time_Duration_asSecs$f64(sim_time);
    let boids_per_sec = as$(f64)(num_boids)*fps;

    io_stream_println(u8_l("\n=== Performance ==="));
    io_stream_println(u8_l("FPS: {:f}"), fps);
    io_stream_println(u8_l("Boid updates/sec: {:f}"), boids_per_sec);
    io_stream_println(u8_l("Time per frame: {:f} ms"), as$(f64)(time_Duration_asSecs$f64(sim_time) * 1000.0) / as$(f64)(num_frames));

    // 정지
    io_stream_println(u8_l("\nStopping all boids..."));
    Message stop_msg = { .tag = BoidMsg_stop };
    for_(($s(actors))(actor_slot) {
        let actor = *actor_slot;
        if (actor->active) {
            try_(Actor_send(&sys, actor, stop_msg));
        }
    });
    ActorSystem_runLoop(&sys, false);

    return_ok({});
} $unguarded_(fn);
