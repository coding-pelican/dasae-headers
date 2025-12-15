/* Actor.h */
#include "dh/meta.h"
#ifndef Actor__included
#define Actor__included 1

#include "dh/async.h"
use_Co_Ctx$(Void);

#include "dh/mem/Allocator.h"
#include "dh/heap/Arena.h"

// 메시지 타입
typedef enum_(MessageType $bits(8)) {
    MessageType_none = 0,
    MessageType_increment = 1,
    MessageType_get_value = 2,
    MessageType_print = 3,
    MessageType_broadcast = 4,
    MessageType_stop = 5,
} MessageType;

// 메시지 구조 (작은 크기로 최적화)
typedef struct Message {
    MessageType tag;
    union {
        u64 u64_val;
        i64 i64_val;
        void* ptr_val;
        u8 inline_data[48];
    } payload;
    struct Message* next;
} Message;
T_use$((Message)(P, O, E));
T_use$((P$Message)(E));

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
T_use$((Actor)(P, S, E));
T_use$((P$Actor)(E));
T_use$((S$Actor)(E));
T_use_E$($set(mem_Err)(Actor));

// Actor 시스템 (Arena allocator 사용)
typedef struct ActorSystem {
    mem_Allocator allocator;  // 기본 allocator
    heap_Arena arena_actor;   // Actor 전용 arena
    heap_Arena arena_message; // 메시지 전용 arena
    S$Actor actors;           // Actor 슬라이스
    usize next_id;
} ActorSystem;
T_use$((ActorSystem)(E));

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
#include "dh/Str.h"
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
        let actors = u_castS$((S$Actor)(try_((mem_Allocator_alloc(gpa, typeInfo$(InnerType), max_actors)))));
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
                u8_l("[ActorSystem] Iteration {:uz}: processed {:uz} messages"),
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
    let new_msg = u_castP$((P$Message)(try_((mem_Allocator_create(gpa, typeInfo$(InnerType))))));
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

/* counter_actor.c */
// Counter 상태
typedef struct {
    i32 count;
    P$Actor self;
} CounterState;
// Counter Actor 행동
async_fn_(counter_actor, (var_(sys, ActorSystem*); var_(self, P$Actor);), Void);
async_fn_scope(counter_actor, {
    var_(msg, Message);
}) {
    let state = as$(CounterState*)(args->self->state);
    io_stream_println(u8_l("[Counter {:u}] Started"), args->self->id);
    while (true) {
        // 메시지 대기
        if_none(Actor_tryRecv(args->sys, args->self)) {
            // 메시지 없음, 계속 suspend
            suspend_({});
            continue;
        } else_some(msg) {
            locals->msg = msg;
        }
        // 메시지 처리
        switch (locals->msg.tag) {
        case MessageType_increment: {
            state->count++;
            io_stream_println(
                u8_l("[Counter {:u}] Incremented to {:d}"),
                args->self->id, state->count
            );
        } break;
        case MessageType_get_value: {
            io_stream_println(
                u8_l("[Counter {:u}] Current value: {:d}"),
                args->self->id, state->count
            );
        } break;
        case MessageType_stop: {
            io_stream_println(u8_l("[Counter {:u}] Stopping"), args->self->id);
            areturn_({});
        } break;
        default: {
        } break;
        }
    }
    areturn_({});
} $unscoped_(async_fn);

/* test_million_actors.c */
#include "dh/main.h"
#include "dh/time.h"

// Echo Actor (최소 상태)
typedef struct {
    u32 id;
    u32 ping_count;
} EchoState;
T_use_P$(EchoState);
T_use_S$(EchoState);
T_use_E$(S$EchoState);
async_fn_(echo_actor, (var_(sys, ActorSystem*); var_(self, P$Actor);), Void);
async_fn_scope(echo_actor, {
    var_(msg, Message);
}) {
    let state = as$(EchoState*)(args->self->state);
    while (true) {
        if_none(Actor_tryRecv(args->sys, args->self)) {
            suspend_({});
            continue;
        } else_some(msg) {
            locals->msg = msg;
        }
        if (locals->msg.tag == MessageType_stop) {
            areturn_({});
        }
        state->ping_count++;
    }
    areturn_({});
} $unscoped_(async_fn);

#include "dh/heap/Page.h"

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    // Page allocator (OS virtual memory)
    var page = (heap_Page){};
    let page_gpa = heap_Page_allocator(&page);

    let total_actors = lit_n$(usize)(1, 000, 000);
    io_stream_println(u8_l("=== Million Actors Test ===\n"));

    // Actor 시스템 초기화
    var sys = try_(ActorSystem_init(page_gpa, total_actors));
    defer_(ActorSystem_fini(&sys));
    // 측정 시작
    var start = time_Instant_now();

    // 100만 개 Actor 생성
    io_stream_println(u8_l("Creating {:uz} actors..."), total_actors);

    // 상태 및 코루틴 컨텍스트 할당 (arena에서)
    let actor_gpa = heap_Arena_allocator(&sys.arena_actor);
    let field_types = typeInfos$(EchoState, Co_CtxFn$(echo_actor));
    let field_mem = try_(mem_Allocator_create(actor_gpa, u_typeInfoRecordN(total_actors, field_types)));
    defer_(mem_Allocator_destroy(actor_gpa, field_mem));
    let fields = u_fieldSlisMut(field_mem, total_actors, field_types, A_ref$((S$u_S$raw)((A$$(2, u_S$raw)){})));
    let states = u_castS$((S$(EchoState))(*S_at((fields)[0])));
    let ctxs = u_castS$((S$$(Co_CtxFn$(echo_actor)))(*S_at((fields)[1])));
    // Actor 생성
    for_(($rf(0), $s(states), $s(ctxs))(i, state, ctx) {
        state->id = i;
        state->ping_count = 0;
        *ctx = Co_CtxFn_init$(echo_actor);
        ctx->args.sys = &sys;
        ctx->args.self = try_(ActorSystem_spawn(&sys, ctx->anyraw, state));
        if (i % lit_n$(usize)(100, 000) == 0) {
            io_stream_println(u8_l("  Created {:uz} actors"), i);
        }
    });

    let create_time = time_Instant_elapsed(start);
    io_stream_println(u8_l("Created {:uz} actors in {:fl} seconds\n"), total_actors, time_Duration_asSecs$f64(create_time));
    // 메모리 사용량 출력
    let actor_mem = heap_Arena_queryCap(&sys.arena_actor);
    let msg_mem = heap_Arena_queryCap(&sys.arena_message);
    io_stream_println(u8_l("Memory usage:"));
    io_stream_println(u8_l("  Actors:   {:uz} MB"), actor_mem / (lit_n$(usize)(1024) * 1024));
    io_stream_println(u8_l("  Messages: {:uz} MB\n"), msg_mem / (lit_n$(usize)(1024) * 1024));

    // 정확한 메트릭 추적
    let messages_per_actor = 3; // increment*2 + stop
    let total_messages = total_actors * messages_per_actor;

    // 모든 Actor에게 메시지 보내기
    io_stream_println(u8_l("Sending {:uz} messages..."), total_messages);
    start = time_Instant_now();

    for_(($s(sys.actors))(actor) {
        if (actor->active) {
            try_(Actor_send(&sys, actor, ((Message){ .tag = MessageType_increment, .payload = { .i64_val = 1 } })));
            try_(Actor_send(&sys, actor, ((Message){ .tag = MessageType_increment, .payload = { .i64_val = 1 } })));
            try_(Actor_send(&sys, actor, ((Message){ .tag = MessageType_stop })));
        }
    });

    let send_time = time_Instant_elapsed(start);
    io_stream_println(
        u8_l("Sent {:uz} messages in {:fl} seconds\n"),
        total_messages, time_Duration_asSecs$f64(send_time)
    );

    // 메시지 전송 후 메모리 측정
    let msg_mem_peak = heap_Arena_queryCap(&sys.arena_message);
    io_stream_println(u8_l("Message memory peak: {:uz} MB"), msg_mem_peak / (lit_n$(usize)(1024) * 1024));

    // Actor 런타임 시작
    io_stream_println(u8_l("Processing..."));
    start = time_Instant_now();
    ActorSystem_runLoop(&sys, false);
    let process_time = time_Instant_elapsed(start);

    // 실제 처리된 메시지 카운트
    let actual_processed = blk({
        usize count = 0;
        for_(($s(states))(state) {
            count += state->ping_count;
        });
        blk_return_(count);
    });

    // 통계
    io_stream_println(u8_l("=== Summary ==="));
    io_stream_println(u8_l("Actors created: {:uz}"), total_actors);
    io_stream_println(u8_l("Messages sent: {:uz}"), total_messages);
    io_stream_println(u8_l("Messages processed: {:uz}"), actual_processed);
    io_stream_println(u8_l("Actor resumes: {:uz}"), total_actors); // 1 iteration
    io_stream_println(u8_l("Memory/actor: {:uz} bytes"), actor_mem / total_actors);
    io_stream_println(u8_l("Messages/sec: {:fl}"), as$(f64)(total_messages) / time_Duration_asSecs$f64(process_time));

    return_ok({});
} $unguarded_(fn);

/*
PS C:\dasae\dev\dasae-headers\dh\tests> dh-c run dev draft-async_actor.c
Using DH library from environment variable: C:\dasae\dev\dasae-headers\dh
Building draft-async_actor with config 'dev'...
Build successful!
Running draft-async_actor...
=== Million Actors Test ===

Creating 1000000 actors...
  Created 0 actors
  Created 100000 actors
  Created 200000 actors
  Created 300000 actors
  Created 400000 actors
  Created 500000 actors
  Created 600000 actors
  Created 700000 actors
  Created 800000 actors
  Created 900000 actors
Created 1000000 actors in 0.131389 seconds

Memory usage:
  Actors:   320 MB
  Messages: 0 MB

Sending 2000000 messages...
Sent 2000000 messages in 7.120902 seconds

Message memory peak: 157 MB
Processing...
[ActorSystem] Starting event loop
[ActorSystem] Iteration 0: processed 1000000 messages
[ActorSystem] Event loop finished
=== Summary ===
Actors created: 1000000
Messages sent: 2000000
Messages processed: 1000000
Actor resumes: 1000000
Memory/actor: 336 bytes
Messages/sec: 25366482.252341
*/
/*
PS C:\dasae\dev\dasae-headers\dh\tests> dh-c run performance draft-async_actor.c
Using DH library from environment variable: C:\dasae\dev\dasae-headers\dh
Building draft-async_actor with config 'performance'...
Build successful!
Running draft-async_actor...
=== Million Actors Test ===

Creating 1000000 actors...
  Created 0 actors
  Created 100000 actors
  Created 200000 actors
  Created 300000 actors
  Created 400000 actors
  Created 500000 actors
  Created 600000 actors
  Created 700000 actors
  Created 800000 actors
  Created 900000 actors
Created 1000000 actors in 0.071875 seconds

Memory usage:
  Actors:   320 MB
  Messages: 0 MB

Sending 2000000 messages...
Sent 2000000 messages in 0.102927 seconds

Message memory peak: 157 MB
Processing...
[ActorSystem] Starting event loop
[ActorSystem] Iteration 0: processed 1000000 messages
[ActorSystem] Event loop finished
=== Summary ===
Actors created: 1000000
Messages sent: 2000000
Messages processed: 1000000
Actor resumes: 1000000
Memory/actor: 336 bytes
Messages/sec: 37945839.902707
*/
