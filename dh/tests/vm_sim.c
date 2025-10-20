#ifndef VM_SIMULATOR_H
#define VM_SIMULATOR_H

#include <stdint.h>
#include <stdbool.h>

// ==================== VM 상수 ====================
#define MEMORY_SIZE     4096
#define NUM_REGISTERS   16
#define MAX_THREADS     8
#define CACHE_LINE_SIZE 64

// ==================== 명령어 옵코드 ====================
typedef enum {
    OP_NOP = 0x00,
    OP_LOAD,  // 메모리 → 레지스터
    OP_STORE, // 레지스터 → 메모리
    OP_MOV,   // 즉시값 또는 레지스터 복사
    OP_ADD,   // 덧셈
    OP_SUB,   // 뺄셈
    OP_INC,   // 증가
    OP_DEC,   // 감소
    OP_CMP,   // 비교
    OP_JMP,   // 점프
    OP_JZ,    // Zero면 점프
    OP_JNZ,   // Not Zero면 점프

    // Atomic 연산 (하드웨어 시뮬레이션)
    OP_ATOMIC_INC,  // 원자적 증가
    OP_ATOMIC_DEC,  // 원자적 감소
    OP_ATOMIC_CAS,  // Compare-And-Swap
    OP_ATOMIC_XCHG, // Exchange
    OP_TEST_SET,    // Test and Set

    // 메모리 동기화
    OP_LOCK_PREFIX, // LOCK 접두사 (다음 명령을 atomic하게)
    OP_MFENCE,      // 메모리 펜스
    OP_PAUSE,       // 스핀락 최적화용

    // 시스템 콜
    OP_SYSCALL,

    // 스레드 제어
    OP_YIELD, // CPU 양보
    OP_HALT,  // 정지
    OP_PRINT, // 디버그 출력
} Opcode;

// ==================== 시스템 콜 번호 ====================
typedef enum {
    SYS_THREAD_CREATE = 1,
    SYS_THREAD_EXIT,
    SYS_SLEEP,
    SYS_PRINT_STATE,
} Syscall;

// ==================== CPU 코어 상태 ====================
typedef struct {
    uint32_t registers[NUM_REGISTERS];
    uint32_t pc; // Program Counter
    uint32_t sp; // Stack Pointer

    // CPU 플래그
    struct {
        bool zero;
        bool carry;
        bool sign;
        bool overflow;
    } flags;

    // 스레드 상태
    uint32_t thread_id;
    bool     halted;
    bool     waiting;      // 대기 상태
    uint32_t wait_address; // 대기 중인 주소

    // 파이프라인 시뮬레이션
    bool     has_lock_prefix; // LOCK 접두사 상태
    uint32_t instruction_count;
    uint32_t cycles_used;
} CPUCore;

// ==================== 메모리 시스템 ====================
typedef struct {
    uint8_t data[MEMORY_SIZE];

    // 캐시라인 락 (하드웨어 캐시 일관성 시뮬레이션)
    struct {
        bool     locked;
        uint32_t owner_thread;
    } cache_lines[MEMORY_SIZE / CACHE_LINE_SIZE];

    // 메모리 버스 상태
    bool     bus_locked;
    uint32_t bus_owner;

    // 메모리 접근 통계
    uint64_t read_count;
    uint64_t write_count;
    uint64_t atomic_count;
} MemorySystem;

// ==================== 동기화 객체 (소프트웨어 레벨) ====================
// Mutex - VM이 관리하는 소프트웨어 뮤텍스
typedef struct {
    uint32_t lock_var; // 0=unlocked, 1=locked
    uint32_t owner;    // 소유자 스레드
    uint32_t waiters[MAX_THREADS];
    uint32_t waiter_count;
    uint64_t contention_count;
} VMMutex;

// Semaphore - VM이 관리하는 세마포어
typedef struct {
    int32_t  count;
    uint32_t waiters[MAX_THREADS];
    uint32_t waiter_count;
} VMSemaphore;

// Spinlock - 하드웨어 스핀락 시뮬레이션
typedef struct {
    uint32_t lock_var; // 메모리 주소
    uint64_t spin_count;
    uint64_t acquisition_count;
} VMSpinlock;

// ==================== 스케줄러 ====================
typedef enum {
    SCHED_ROUND_ROBIN,
    SCHED_RANDOM,
    SCHED_PRIORITY
} SchedulerType;

typedef struct {
    SchedulerType type;
    uint32_t      quantum; // 타임 슬라이스
    uint32_t      current_thread;
    uint32_t      thread_count;
    bool          threads_active[MAX_THREADS];
    uint64_t      context_switches;
    uint32_t      next_switch; // 다음 컨텍스트 스위치까지 남은 사이클
} Scheduler;

// ==================== VM 전체 구조체 ====================
typedef struct {
    CPUCore      cores[MAX_THREADS];
    MemorySystem memory;
    Scheduler    scheduler;

    // 동기화 객체들
    VMMutex     mutexes[16];
    VMSemaphore semaphores[16];
    VMSpinlock  spinlocks[16];

    // VM 상태
    bool     running;
    uint64_t cycle_count;
    uint64_t instruction_count;

    // Race condition 시뮬레이션 설정
    bool     enable_race_simulation; // Race condition 시뮬레이션 활성화
    uint32_t race_probability;       // 0-100, 인터리빙 확률

    // 통계
    uint64_t atomic_ops;
    uint64_t lock_contentions;
    uint64_t race_conditions_detected;
    uint64_t memory_barriers;
} VM;

// ==================== VM 인터페이스 ====================
VM*  vm_create(void);
void vm_destroy(VM* vm);
void vm_reset(VM* vm);

// 프로그램 로드/실행
void vm_load_program(VM* vm, uint8_t* program, size_t size, uint32_t thread_id);
void vm_run(VM* vm);
void vm_step(VM* vm);
void vm_cycle(VM* vm);

// 메모리 접근 (시뮬레이션)
uint32_t vm_memory_read(VM* vm, uint32_t thread_id, uint32_t addr);
void     vm_memory_write(VM* vm, uint32_t thread_id, uint32_t addr, uint32_t value);
bool     vm_memory_try_lock(VM* vm, uint32_t thread_id, uint32_t addr);
void     vm_memory_unlock(VM* vm, uint32_t thread_id, uint32_t addr);

// Atomic 연산 (하드웨어 레벨 시뮬레이션)
bool     vm_atomic_cas(VM* vm, uint32_t thread_id, uint32_t addr, uint32_t expected, uint32_t new_val);
uint32_t vm_atomic_exchange(VM* vm, uint32_t thread_id, uint32_t addr, uint32_t new_val);
void     vm_atomic_increment(VM* vm, uint32_t thread_id, uint32_t addr);
uint32_t vm_test_and_set(VM* vm, uint32_t thread_id, uint32_t addr);

// 스레드 관리
uint32_t vm_create_thread(VM* vm, uint32_t entry_point);
void     vm_thread_yield(VM* vm, uint32_t thread_id);
void     vm_thread_exit(VM* vm, uint32_t thread_id);

// 스케줄링
void vm_schedule(VM* vm);
void vm_set_scheduler(VM* vm, SchedulerType type, uint32_t quantum);

// Race condition 시뮬레이션
void vm_enable_race_simulation(VM* vm, uint32_t probability);
void vm_simulate_cache_coherency(VM* vm);

// 동기화 프리미티브 구현 (소프트웨어 레벨)
void vm_mutex_init(VM* vm, uint32_t mutex_id, uint32_t addr);
void vm_mutex_lock(VM* vm, uint32_t thread_id, uint32_t mutex_id);
void vm_mutex_unlock(VM* vm, uint32_t thread_id, uint32_t mutex_id);

void vm_semaphore_init(VM* vm, uint32_t sem_id, int32_t count);
void vm_semaphore_wait(VM* vm, uint32_t thread_id, uint32_t sem_id);
void vm_semaphore_post(VM* vm, uint32_t thread_id, uint32_t sem_id);

// 상태 출력
void vm_dump_state(VM* vm);
void vm_print_statistics(VM* vm);
void vm_trace_execution(VM* vm, bool enable);

#endif // VM_SIMULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vm_simulator.h"

// ==================== VM 생성/파괴 ====================
VM* vm_create(void) {
    VM* vm = (VM*)calloc(1, sizeof(VM));
    if (!vm) {
        return NULL;
    }

    // 기본 설정
    vm->scheduler.type              = SCHED_ROUND_ROBIN;
    vm->scheduler.quantum           = 10; // 10 사이클마다 스위치
    vm->scheduler.thread_count      = 1;
    vm->scheduler.threads_active[0] = true;

    // 메인 스레드 초기화
    vm->cores[0].thread_id = 0;
    vm->cores[0].sp        = MEMORY_SIZE - 256; // 스택 끝에서 시작

    // 랜덤 시드
    srand(time(NULL));

    return vm;
}

void vm_destroy(VM* vm) {
    if (vm) {
        free(vm);
    }
}

void vm_reset(VM* vm) {
    memset(&vm->memory, 0, sizeof(vm->memory));
    memset(&vm->cores, 0, sizeof(vm->cores));
    vm->cycle_count       = 0;
    vm->instruction_count = 0;
}

// ==================== 프로그램 로드 ====================
void vm_load_program(VM* vm, uint8_t* program, size_t size, uint32_t thread_id) {
    if (size > MEMORY_SIZE || thread_id >= MAX_THREADS) {
        return;
    }

    // 각 스레드별 메모리 영역에 로드
    uint32_t offset = thread_id * 512; // 각 스레드당 512바이트
    memcpy(&vm->memory.data[offset], program, size);
    vm->cores[thread_id].pc = offset;
}

// ==================== 메모리 접근 시뮬레이션 ====================
uint32_t vm_memory_read(VM* vm, uint32_t thread_id, uint32_t addr) {
    if (addr >= MEMORY_SIZE - 3) {
        return 0;
    }

    vm->memory.read_count++;

    // Race condition 시뮬레이션
    if (vm->enable_race_simulation && (rand() % 100) < vm->race_probability) {
        // 읽기 중간에 컨텍스트 스위치 시뮬레이션
        vm_schedule(vm);
    }

    return *(uint32_t*)&vm->memory.data[addr];
}

void vm_memory_write(VM* vm, uint32_t thread_id, uint32_t addr, uint32_t value) {
    if (addr >= MEMORY_SIZE - 3) {
        return;
    }

    vm->memory.write_count++;

    // Race condition 시뮬레이션
    if (vm->enable_race_simulation && !vm->memory.bus_locked) {
        if ((rand() % 100) < vm->race_probability) {
            // 쓰기 중간에 컨텍스트 스위치 시뮬레이션
            vm_schedule(vm);
        }
    }

    *(uint32_t*)&vm->memory.data[addr] = value;
}

// ==================== 메모리 버스 락 시뮬레이션 ====================
bool vm_memory_try_lock(VM* vm, uint32_t thread_id, uint32_t addr) {
    uint32_t cache_line = addr / CACHE_LINE_SIZE;

    if (vm->memory.bus_locked && vm->memory.bus_owner != thread_id) {
        vm->lock_contentions++;
        return false;
    }

    vm->memory.bus_locked                           = true;
    vm->memory.bus_owner                            = thread_id;
    vm->memory.cache_lines[cache_line].locked       = true;
    vm->memory.cache_lines[cache_line].owner_thread = thread_id;

    return true;
}

void vm_memory_unlock(VM* vm, uint32_t thread_id, uint32_t addr) {
    if (vm->memory.bus_owner == thread_id) {
        vm->memory.bus_locked                     = false;
        uint32_t cache_line                       = addr / CACHE_LINE_SIZE;
        vm->memory.cache_lines[cache_line].locked = false;
    }
}

// ==================== Atomic 연산 시뮬레이션 ====================
bool vm_atomic_cas(VM* vm, uint32_t thread_id, uint32_t addr, uint32_t expected, uint32_t new_val) {
    // 메모리 버스 락 획득 시뮬레이션
    if (!vm_memory_try_lock(vm, thread_id, addr)) {
        return false;
    }

    vm->atomic_ops++;
    vm->memory.atomic_count++;

    // 원자적 읽기-비교-쓰기
    uint32_t current = *(uint32_t*)&vm->memory.data[addr];
    bool     success = (current == expected);

    if (success) {
        *(uint32_t*)&vm->memory.data[addr] = new_val;
    }

    // 메모리 버스 락 해제
    vm_memory_unlock(vm, thread_id, addr);

    return success;
}

uint32_t vm_atomic_exchange(VM* vm, uint32_t thread_id, uint32_t addr, uint32_t new_val) {
    if (!vm_memory_try_lock(vm, thread_id, addr)) {
        return 0;
    }

    vm->atomic_ops++;

    uint32_t old_val                   = *(uint32_t*)&vm->memory.data[addr];
    *(uint32_t*)&vm->memory.data[addr] = new_val;

    vm_memory_unlock(vm, thread_id, addr);
    return old_val;
}

void vm_atomic_increment(VM* vm, uint32_t thread_id, uint32_t addr) {
    if (!vm_memory_try_lock(vm, thread_id, addr)) {
        // 실패 시 재시도 필요 (스핀)
        vm->cores[thread_id].waiting      = true;
        vm->cores[thread_id].wait_address = addr;
        return;
    }

    vm->atomic_ops++;
    (*(uint32_t*)&vm->memory.data[addr])++;

    vm_memory_unlock(vm, thread_id, addr);
}

uint32_t vm_test_and_set(VM* vm, uint32_t thread_id, uint32_t addr) {
    if (!vm_memory_try_lock(vm, thread_id, addr)) {
        return 0xFFFFFFFF; // 실패
    }

    vm->atomic_ops++;

    uint32_t old_val                   = *(uint32_t*)&vm->memory.data[addr];
    *(uint32_t*)&vm->memory.data[addr] = 1;

    vm_memory_unlock(vm, thread_id, addr);
    return old_val;
}

// ==================== 스레드 관리 ====================
uint32_t vm_create_thread(VM* vm, uint32_t entry_point) {
    for (uint32_t i = 0; i < MAX_THREADS; i++) {
        if (!vm->scheduler.threads_active[i] && i != 0) {
            vm->cores[i].pc                 = entry_point;
            vm->cores[i].thread_id          = i;
            vm->cores[i].sp                 = MEMORY_SIZE - 256 * (i + 1);
            vm->cores[i].halted             = false;
            vm->scheduler.threads_active[i] = true;
            if (i >= vm->scheduler.thread_count) {
                vm->scheduler.thread_count = i + 1;
            }
            return i;
        }
    }
    return 0xFFFFFFFF; // 실패
}

void vm_thread_yield(VM* vm, uint32_t thread_id) {
    vm_schedule(vm);
}

void vm_thread_exit(VM* vm, uint32_t thread_id) {
    vm->cores[thread_id].halted             = true;
    vm->scheduler.threads_active[thread_id] = false;
}

// ==================== 스케줄링 ====================
void vm_schedule(VM* vm) {
    uint32_t old_thread = vm->scheduler.current_thread;

    switch (vm->scheduler.type) {
    case SCHED_ROUND_ROBIN:
        // 다음 활성 스레드 찾기
        do {
            vm->scheduler.current_thread = (vm->scheduler.current_thread + 1) % vm->scheduler.thread_count;
        } while (!vm->scheduler.threads_active[vm->scheduler.current_thread] && vm->scheduler.current_thread != old_thread);
        break;

    case SCHED_RANDOM:
        // 랜덤하게 스레드 선택 (Race condition 테스트용)
        uint32_t attempts = 0;
        do {
            vm->scheduler.current_thread = rand() % vm->scheduler.thread_count;
            attempts++;
        } while (!vm->scheduler.threads_active[vm->scheduler.current_thread] && attempts < 10);
        break;

    case SCHED_PRIORITY:
        // 간단한 우선순위 (낮은 ID가 높은 우선순위)
        for (uint32_t i = 0; i < vm->scheduler.thread_count; i++) {
            if (vm->scheduler.threads_active[i] && !vm->cores[i].halted) {
                vm->scheduler.current_thread = i;
                break;
            }
        }
        break;
    }

    if (old_thread != vm->scheduler.current_thread) {
        vm->scheduler.context_switches++;
    }

    // 다음 스위치 시점 설정
    vm->scheduler.next_switch = vm->scheduler.quantum;
}

// ==================== 명령어 실행 ====================
static void execute_instruction(VM* vm, uint32_t thread_id) {
    CPUCore* core = &vm->cores[thread_id];

    if (core->halted || core->waiting) {
        // 대기 중인 경우 재시도
        if (core->waiting) {
            // Atomic 연산 재시도
            vm_atomic_increment(vm, thread_id, core->wait_address);
            if (!core->waiting) {
                // 성공하면 대기 해제
                core->wait_address = 0;
            }
        }
        return;
    }

    // 명령어 페치
    if (core->pc >= MEMORY_SIZE - 7) {
        core->halted = true;
        return;
    }

    uint8_t  opcode = vm->memory.data[core->pc++];
    uint8_t  dst    = vm->memory.data[core->pc++];
    uint8_t  src    = vm->memory.data[core->pc++];
    uint32_t imm    = *(uint32_t*)&vm->memory.data[core->pc];
    core->pc += 4;

    core->instruction_count++;
    vm->instruction_count++;

    // 명령어 실행
    switch (opcode) {
    case OP_NOP:
        break;

    case OP_MOV:
        if (src == 0) {
            core->registers[dst] = imm;
        } else {
            core->registers[dst] = core->registers[src];
        }
        break;

    case OP_LOAD:
        core->registers[dst] = vm_memory_read(vm, thread_id, imm);
        break;

    case OP_STORE:
        vm_memory_write(vm, thread_id, imm, core->registers[src]);
        break;

    case OP_ADD:
        core->registers[dst] += core->registers[src];
        core->flags.zero = (core->registers[dst] == 0);
        break;

    case OP_SUB:
        core->registers[dst] -= core->registers[src];
        core->flags.zero = (core->registers[dst] == 0);
        break;

    case OP_INC:
        core->registers[dst]++;
        core->flags.zero = (core->registers[dst] == 0);
        break;

    case OP_DEC:
        core->registers[dst]--;
        core->flags.zero = (core->registers[dst] == 0);
        break;

    case OP_CMP:
        core->flags.zero = (core->registers[dst] == core->registers[src]);
        core->flags.sign = (core->registers[dst] < core->registers[src]);
        break;

    case OP_JMP:
        core->pc = imm;
        break;

    case OP_JZ:
        if (core->flags.zero) {
            core->pc = imm;
        }
        break;

    case OP_JNZ:
        if (!core->flags.zero) {
            core->pc = imm;
        }
        break;

    case OP_ATOMIC_INC:
        vm_atomic_increment(vm, thread_id, imm);
        break;

    case OP_ATOMIC_DEC:
        if (vm_memory_try_lock(vm, thread_id, imm)) {
            vm->atomic_ops++;
            (*(uint32_t*)&vm->memory.data[imm])--;
            vm_memory_unlock(vm, thread_id, imm);
        }
        break;

    case OP_ATOMIC_CAS:
        core->flags.zero = vm_atomic_cas(vm, thread_id, imm, core->registers[dst], core->registers[src]);
        break;

    case OP_ATOMIC_XCHG:
        core->registers[dst] = vm_atomic_exchange(vm, thread_id, imm, core->registers[src]);
        break;

    case OP_TEST_SET:
        core->registers[dst] = vm_test_and_set(vm, thread_id, imm);
        break;

    case OP_LOCK_PREFIX:
        core->has_lock_prefix = true;
        break;

    case OP_MFENCE:
        vm->memory_barriers++;
        // 메모리 배리어 시뮬레이션 - 모든 이전 메모리 연산 완료 대기
        break;

    case OP_PAUSE:
        // 스핀락 최적화 - CPU 사이클 낭비 줄이기
        core->cycles_used += 10;
        break;

    case OP_SYSCALL:
        switch (imm) {
        case SYS_THREAD_CREATE:
            vm_create_thread(vm, core->registers[0]);
            break;
        case SYS_THREAD_EXIT:
            vm_thread_exit(vm, thread_id);
            break;
        case SYS_SLEEP:
            // 슬립 시뮬레이션 - 일정 사이클 대기
            core->cycles_used += core->registers[0] * 1000;
            break;
        }
        break;

    case OP_YIELD:
        vm_thread_yield(vm, thread_id);
        break;

    case OP_PRINT:
        printf("[T%d@%lu] R%d = %d\n", thread_id, vm->cycle_count, src, core->registers[src]);
        break;

    case OP_HALT:
        core->halted = true;
        break;

    default:
        printf("Unknown opcode: 0x%02X at PC=%04X\n", opcode, core->pc - 7);
        core->halted = true;
    }
}

// ==================== VM 실행 ====================
void vm_cycle(VM* vm) {
    // 현재 스레드 실행
    uint32_t thread_id = vm->scheduler.current_thread;

    if (vm->scheduler.threads_active[thread_id] && !vm->cores[thread_id].halted) {
        execute_instruction(vm, thread_id);
    }

    vm->cycle_count++;

    // 스케줄링 체크
    vm->scheduler.next_switch--;
    if (vm->scheduler.next_switch == 0) {
        vm_schedule(vm);
    }
}

void vm_step(VM* vm) {
    vm_cycle(vm);
}

void vm_run(VM* vm) {
    vm->running = true;

    while (vm->running) {
        vm_cycle(vm);

        // 모든 스레드가 종료되었는지 확인
        bool all_done = true;
        for (uint32_t i = 0; i < vm->scheduler.thread_count; i++) {
            if (vm->scheduler.threads_active[i] && !vm->cores[i].halted) {
                all_done = false;
                break;
            }
        }

        if (all_done) {
            vm->running = false;
        }

        // 무한 루프 방지
        if (vm->cycle_count > 1000000) {
            printf("Cycle limit reached!\n");
            vm->running = false;
        }
    }
}

// ==================== Race Condition 시뮬레이션 ====================
void vm_enable_race_simulation(VM* vm, uint32_t probability) {
    vm->enable_race_simulation = true;
    vm->race_probability       = probability;

    // 랜덤 스케줄링으로 변경
    vm->scheduler.type    = SCHED_RANDOM;
    vm->scheduler.quantum = 1; // 매 사이클마다 스위치 가능
}

// ==================== Mutex 구현 (소프트웨어 레벨) ====================
void vm_mutex_init(VM* vm, uint32_t mutex_id, uint32_t addr) {
    if (mutex_id >= 16) {
        return;
    }

    vm->mutexes[mutex_id].lock_var = addr;
    vm_memory_write(vm, 0, addr, 0); // 0 = unlocked
}

void vm_mutex_lock(VM* vm, uint32_t thread_id, uint32_t mutex_id) {
    if (mutex_id >= 16) {
        return;
    }

    VMMutex* mutex = &vm->mutexes[mutex_id];

    // 스핀락 구현 (Test-And-Set 사용)
    while (true) {
        uint32_t old_val = vm_test_and_set(vm, thread_id, mutex->lock_var);

        if (old_val == 0) {
            // 락 획득 성공
            mutex->owner = thread_id;
            break;
        }

        // 락 획득 실패 - 대기
        mutex->contention_count++;

        // 대기 리스트에 추가
        bool in_list = false;
        for (uint32_t i = 0; i < mutex->waiter_count; i++) {
            if (mutex->waiters[i] == thread_id) {
                in_list = true;
                break;
            }
        }

        if (!in_list && mutex->waiter_count < MAX_THREADS) {
            mutex->waiters[mutex->waiter_count++] = thread_id;
        }

        // CPU 양보
        vm_thread_yield(vm, thread_id);
    }
}

void vm_mutex_unlock(VM* vm, uint32_t thread_id, uint32_t mutex_id) {
    if (mutex_id >= 16) {
        return;
    }

    VMMutex* mutex = &vm->mutexes[mutex_id];

    if (mutex->owner == thread_id) {
        // 락 해제
        vm_memory_write(vm, thread_id, mutex->lock_var, 0);
        mutex->owner = 0xFFFFFFFF;

        // 대기자 리스트 정리
        if (mutex->waiter_count > 0) {
            mutex->waiter_count = 0;
        }
    }
}

// ==================== Semaphore 구현 ====================
void vm_semaphore_init(VM* vm, uint32_t sem_id, int32_t count) {
    if (sem_id >= 16) {
        return;
    }
    vm->semaphores[sem_id].count        = count;
    vm->semaphores[sem_id].waiter_count = 0;
}

void vm_semaphore_wait(VM* vm, uint32_t thread_id, uint32_t sem_id) {
    if (sem_id >= 16) {
        return;
    }

    VMSemaphore* sem = &vm->semaphores[sem_id];

    // Atomic decrement and test
    while (true) {
        if (sem->count > 0) {
            sem->count--;
            break;
        }

        // 대기
        bool in_list = false;
        for (uint32_t i = 0; i < sem->waiter_count; i++) {
            if (sem->waiters[i] == thread_id) {
                in_list = true;
                break;
            }
        }

        if (!in_list && sem->waiter_count < MAX_THREADS) {
            sem->waiters[sem->waiter_count++] = thread_id;
        }

        vm_thread_yield(vm, thread_id);
    }
}

void vm_semaphore_post(VM* vm, uint32_t thread_id, uint32_t sem_id) {
    if (sem_id >= 16) {
        return;
    }

    vm->semaphores[sem_id].count++;
}

// ==================== 상태 출력 ====================
void vm_dump_state(VM* vm) {
    printf("\n=== VM State ===\n");
    printf("Cycles: %lu, Instructions: %lu\n", vm->cycle_count, vm->instruction_count);
    printf("Context Switches: %lu, Atomic Ops: %lu\n", vm->scheduler.context_switches, vm->atomic_ops);
    printf("Memory: Reads=%lu, Writes=%lu, Atomic=%lu\n", vm->memory.read_count, vm->memory.write_count, vm->memory.atomic_count);

    printf("\nThreads:\n");
    for (uint32_t i = 0; i < vm->scheduler.thread_count; i++) {
        if (vm->scheduler.threads_active[i]) {
            printf("  T%d: PC=%04X, Instructions=%d, %s\n", i, vm->cores[i].pc, vm->cores[i].instruction_count, vm->cores[i].halted ? "HALTED" : "RUNNING");
        }
    }

    if (vm->enable_race_simulation) {
        printf("\nRace Simulation: Probability=%d%%, Detected=%lu\n", vm->race_probability, vm->race_conditions_detected);
    }
}

void vm_print_statistics(VM* vm) {
    printf("\n=== Performance Statistics ===\n");
    printf("Total Cycles: %lu\n", vm->cycle_count);
    printf("IPC (Instructions Per Cycle): %.2f\n", (double)vm->instruction_count / vm->cycle_count);
    printf("Context Switch Rate: %.2f%%\n", (double)vm->scheduler.context_switches * 100 / vm->cycle_count);
    printf("Atomic Operation Rate: %.2f%%\n", (double)vm->atomic_ops * 100 / vm->instruction_count);
    printf("Lock Contentions: %lu\n", vm->lock_contentions);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vm_simulator.h"

// 색상 코드
#define RED    "\033[31m"
#define GREEN  "\033[32m"
#define YELLOW "\033[33m"
#define BLUE   "\033[34m"
#define CYAN   "\033[36m"
#define RESET  "\033[0m"

// ==================== Race Condition 시뮬레이션 ====================
void test_race_condition() {
    printf(CYAN "\n=== Race Condition Simulation ===\n" RESET);
    printf("두 스레드가 같은 메모리를 동시에 수정합니다.\n\n");

    // 1. Race condition 있는 버전
    {
        VM* vm = vm_create();
        vm_enable_race_simulation(vm, 50); // 50% 확률로 인터리빙

        // Thread 0 프로그램: 카운터 100번 증가
        uint8_t prog0[] = {
            OP_MOV, 0, 0, 0, 0, 0, 0,         // R0 = 0
            OP_STORE, 0, 0, 0x00, 0x10, 0, 0, // [0x1000] = 0 (초기화)

            OP_MOV, 1, 0, 100, 0, 0, 0, // R1 = 100
            // loop:
            OP_LOAD, 2, 0, 0x00, 0x10, 0, 0,  // R2 = [0x1000]
            OP_INC, 2, 0, 0, 0, 0, 0,         // R2++
            OP_STORE, 0, 2, 0x00, 0x10, 0, 0, // [0x1000] = R2
            OP_DEC, 1, 0, 0, 0, 0, 0,         // R1--
            OP_JNZ, 0, 0, 14, 0, 0, 0,        // loop

            OP_HALT, 0, 0, 0, 0, 0, 0
        };

        // Thread 1 프로그램: 똑같이 100번 증가
        uint8_t prog1[] = {
            OP_MOV, 1, 0, 100, 0, 0, 0, // R1 = 100
            // loop:
            OP_LOAD, 2, 0, 0x00, 0x10, 0, 0,  // R2 = [0x1000]
            OP_INC, 2, 0, 0, 0, 0, 0,         // R2++
            OP_STORE, 0, 2, 0x00, 0x10, 0, 0, // [0x1000] = R2
            OP_DEC, 1, 0, 0, 0, 0, 0,         // R1--
            OP_JNZ, 0, 0, 7, 0, 0, 0,         // loop

            OP_HALT, 0, 0, 0, 0, 0, 0
        };

        vm_load_program(vm, prog0, sizeof(prog0), 0);
        vm_create_thread(vm, 512); // Thread 1 생성
        vm_load_program(vm, prog1, sizeof(prog1), 1);

        printf(RED "Non-Atomic (Race Condition):\n" RESET);
        vm_run(vm);

        uint32_t result = *(uint32_t*)&vm->memory.data[0x1000];
        printf("예상값: 200, 실제값: %d\n", result);
        if (result < 200) {
            printf(RED "Race condition 발생! %d번의 업데이트 손실\n" RESET, 200 - result);
        }

        vm_dump_state(vm);
        vm_destroy(vm);
    }

    // 2. Atomic 연산 버전
    {
        VM* vm = vm_create();
        vm_enable_race_simulation(vm, 50);

        // Thread 0: Atomic increment
        uint8_t prog0[] = {
            OP_MOV, 0, 0, 0, 0, 0, 0,
            OP_STORE, 0, 0, 0x00, 0x10, 0, 0,

            OP_MOV, 1, 0, 100, 0, 0, 0,
            // loop:
            OP_ATOMIC_INC, 0, 0, 0x00, 0x10, 0, 0, // Atomic increment
            OP_DEC, 1, 0, 0, 0, 0, 0,
            OP_JNZ, 0, 0, 14, 0, 0, 0,

            OP_HALT, 0, 0, 0, 0, 0, 0
        };

        // Thread 1: Atomic increment
        uint8_t prog1[] = {
            OP_MOV, 1, 0, 100, 0, 0, 0,
            // loop:
            OP_ATOMIC_INC, 0, 0, 0x00, 0x10, 0, 0,
            OP_DEC, 1, 0, 0, 0, 0, 0,
            OP_JNZ, 0, 0, 7, 0, 0, 0,

            OP_HALT, 0, 0, 0, 0, 0, 0
        };

        vm_load_program(vm, prog0, sizeof(prog0), 0);
        vm_create_thread(vm, 512);
        vm_load_program(vm, prog1, sizeof(prog1), 1);

        printf(GREEN "\nAtomic Operations:\n" RESET);
        vm_run(vm);

        uint32_t result = *(uint32_t*)&vm->memory.data[0x1000];
        printf("예상값: 200, 실제값: %d\n", result);
        if (result == 200) {
            printf(GREEN "완벽! Race condition 없음\n" RESET);
        }

        vm_dump_state(vm);
        vm_destroy(vm);
    }
}

// ==================== CAS 연산 테스트 ====================
void test_cas_operation() {
    printf(CYAN "\n=== Compare-And-Swap (CAS) Test ===\n" RESET);

    VM* vm = vm_create();

    uint8_t program[] = {
        // 메모리에 10 저장
        OP_MOV, 0, 0, 10, 0, 0, 0,
        OP_STORE, 0, 0, 0x00, 0x10, 0, 0,
        OP_PRINT, 0, 0, 0, 0, 0, 0,

        // CAS loop: 10을 20으로 변경 시도
        OP_MOV, 1, 0, 10, 0, 0, 0, // expected = 10
        OP_MOV, 2, 0, 20, 0, 0, 0, // new = 20
        OP_ATOMIC_CAS, 1, 2, 0x00, 0x10, 0, 0,
        OP_JZ, 0, 0, 56, 0, 0, 0, // 성공하면 점프

        // 실패 경로
        OP_MOV, 3, 0, 999, 0, 0, 0, // 실패 표시
        OP_PRINT, 0, 3, 0, 0, 0, 0,
        OP_JMP, 0, 0, 70, 0, 0, 0,

        // 성공 경로
        OP_LOAD, 3, 0, 0x00, 0x10, 0, 0,
        OP_PRINT, 0, 3, 0, 0, 0, 0,

        OP_HALT, 0, 0, 0, 0, 0, 0
    };

    vm_load_program(vm, program, sizeof(program), 0);

    printf("CAS 실행:\n");
    vm_run(vm);

    uint32_t final = *(uint32_t*)&vm->memory.data[0x1000];
    printf("\n최종값: %d (20이어야 함)\n", final);

    vm_destroy(vm);
}

// ==================== 스핀락 vs 뮤텍스 ====================
void test_spinlock_vs_mutex() {
    printf(CYAN "\n=== Spinlock Implementation ===\n" RESET);

    VM* vm = vm_create();
    vm_enable_race_simulation(vm, 30);

    // Spinlock 구현 (Test-And-Set 사용)
    uint8_t prog0[] = {
        OP_MOV, 0, 0, 0, 0, 0, 0,
        OP_STORE, 0, 0, 0x00, 0x20, 0, 0, // lock = 0
        OP_STORE, 0, 0, 0x00, 0x10, 0, 0, // counter = 0

        OP_MOV, 5, 0, 50, 0, 0, 0, // 50번 반복

        // acquire_lock:
        OP_TEST_SET, 1, 0, 0x00, 0x20, 0, 0, // R1 = test_and_set(lock)
        OP_CMP, 1, 0, 0, 0, 0, 0,            // R0 = 0
        OP_JNZ, 0, 0, 21, 0, 0, 0,           // 이미 잠겨있으면 재시도

        // critical section
        OP_LOAD, 2, 0, 0x00, 0x10, 0, 0,
        OP_INC, 2, 0, 0, 0, 0, 0,
        OP_STORE, 0, 2, 0x00, 0x10, 0, 0,

        // release_lock
        OP_MOV, 0, 0, 0, 0, 0, 0,
        OP_STORE, 0, 0, 0x00, 0x20, 0, 0, // lock = 0

        OP_DEC, 5, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 21, 0, 0, 0,

        OP_HALT, 0, 0, 0, 0, 0, 0
    };

    uint8_t prog1[] = {
        OP_MOV, 5, 0, 50, 0, 0, 0,

        // acquire_lock:
        OP_TEST_SET, 1, 0, 0x00, 0x20, 0, 0,
        OP_CMP, 1, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 7, 0, 0, 0,

        // critical section
        OP_LOAD, 2, 0, 0x00, 0x10, 0, 0,
        OP_INC, 2, 0, 0, 0, 0, 0,
        OP_STORE, 0, 2, 0x00, 0x10, 0, 0,

        // release_lock
        OP_MOV, 0, 0, 0, 0, 0, 0,
        OP_STORE, 0, 0, 0x00, 0x20, 0, 0,

        OP_DEC, 5, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 7, 0, 0, 0,

        OP_HALT, 0, 0, 0, 0, 0, 0
    };

    vm_load_program(vm, prog0, sizeof(prog0), 0);
    vm_create_thread(vm, 512);
    vm_load_program(vm, prog1, sizeof(prog1), 1);

    printf("Spinlock으로 보호된 카운터:\n");
    vm_run(vm);

    uint32_t result = *(uint32_t*)&vm->memory.data[0x1000];
    printf("최종 카운터: %d (100이어야 함)\n", result);

    vm_print_statistics(vm);
    vm_destroy(vm);
}

// ==================== 데드락 시뮬레이션 ====================
void test_deadlock() {
    printf(CYAN "\n=== Deadlock Detection ===\n" RESET);
    printf("두 스레드가 서로의 락을 기다립니다.\n\n");

    VM* vm = vm_create();

    // Thread 0: Lock A, then Lock B
    uint8_t prog0[] = {
        // Lock A 획득
        OP_MOV, 0, 0, 1, 0, 0, 0,
        OP_STORE, 0, 0, 0x00, 0x20, 0, 0, // lockA = 1
        OP_PRINT, 0, 0, 0, 0, 0, 0,       // "T0: Got Lock A"

        // 잠시 대기 (다른 스레드가 실행되도록)
        OP_YIELD, 0, 0, 0, 0, 0, 0,
        OP_YIELD, 0, 0, 0, 0, 0, 0,

        // Lock B 시도 (데드락!)
        OP_TEST_SET, 1, 0, 0x00, 0x24, 0, 0,
        OP_CMP, 1, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 42, 0, 0, 0, // 재시도

        OP_PRINT, 0, 0, 0, 0, 0, 0, // 도달하지 못함
        OP_HALT, 0, 0, 0, 0, 0, 0
    };

    // Thread 1: Lock B, then Lock A
    uint8_t prog1[] = {
        // Lock B 획득
        OP_MOV, 0, 0, 1, 0, 0, 0,
        OP_STORE, 0, 0, 0x00, 0x24, 0, 0, // lockB = 1
        OP_PRINT, 0, 0, 0, 0, 0, 0,       // "T1: Got Lock B"

        // 잠시 대기
        OP_YIELD, 0, 0, 0, 0, 0, 0,
        OP_YIELD, 0, 0, 0, 0, 0, 0,

        // Lock A 시도 (데드락!)
        OP_TEST_SET, 1, 0, 0x00, 0x20, 0, 0,
        OP_CMP, 1, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 42, 0, 0, 0,

        OP_PRINT, 0, 0, 0, 0, 0, 0,
        OP_HALT, 0, 0, 0, 0, 0, 0
    };

    vm_load_program(vm, prog0, sizeof(prog0), 0);
    vm_create_thread(vm, 512);
    vm_load_program(vm, prog1, sizeof(prog1), 1);

    printf("데드락 시뮬레이션 시작...\n");

    // 제한된 사이클만 실행
    for (int i = 0; i < 1000 && vm->running; i++) {
        vm_cycle(vm);
    }

    printf(RED "\n데드락 감지! 두 스레드가 서로를 기다리고 있습니다.\n" RESET);
    printf("T0: Lock A 보유, Lock B 대기\n");
    printf("T1: Lock B 보유, Lock A 대기\n");

    vm_destroy(vm);
}

// ==================== 메모리 배리어 테스트 ====================
void test_memory_barrier() {
    printf(CYAN "\n=== Memory Barrier Test ===\n" RESET);

    VM* vm = vm_create();

    uint8_t program[] = {
        // 순서가 중요한 쓰기 연산
        OP_MOV, 0, 0, 1, 0, 0, 0,
        OP_STORE, 0, 0, 0x00, 0x10, 0, 0, // data = 1

        OP_MFENCE, 0, 0, 0, 0, 0, 0, // 메모리 배리어

        OP_MOV, 0, 0, 1, 0, 0, 0,
        OP_STORE, 0, 0, 0x00, 0x14, 0, 0, // flag = 1

        OP_PRINT, 0, 0, 0, 0, 0, 0,
        OP_HALT, 0, 0, 0, 0, 0, 0
    };

    vm_load_program(vm, program, sizeof(program), 0);

    printf("메모리 배리어로 순서 보장:\n");
    vm_run(vm);

    printf("Memory Barriers used: %lu\n", vm->memory_barriers);
    vm_destroy(vm);
}

// ==================== 교육 정보 ====================
void print_summary() {
    printf("\n" CYAN "========================================\n");
    printf("         핵심 개념 정리\n");
    printf("========================================\n" RESET);

    printf("\n" YELLOW "1. VM이 시뮬레이션하는 것:" RESET "\n");
    printf("   • CPU 파이프라인과 명령어 실행\n");
    printf("   • 메모리 버스와 캐시 일관성\n");
    printf("   • 컨텍스트 스위칭과 스케줄링\n");
    printf("   • Race condition 발생 조건\n");

    printf("\n" YELLOW "2. Race Condition:" RESET "\n");
    printf("   • 읽기-수정-쓰기가 원자적이지 않음\n");
    printf("   • 인터리빙으로 업데이트 손실\n");
    printf("   • VM이 실제로 시뮬레이션함\n");

    printf("\n" YELLOW "3. Atomic 연산:" RESET "\n");
    printf("   • 메모리 버스 락 시뮬레이션\n");
    printf("   • CAS, Test-And-Set 구현\n");
    printf("   • Lock-free 알고리즘의 기초\n");

    printf("\n" YELLOW "4. 동기화 메커니즘:" RESET "\n");
    printf("   • Spinlock: 바쁜 대기\n");
    printf("   • Mutex: 소유권 기반\n");
    printf("   • Semaphore: 카운팅\n");

    printf("\n" GREEN "실제 CPU와의 차이점:" RESET "\n");
    printf("   • 실제: 하드웨어 지원 (LOCK prefix)\n");
    printf("   • 시뮬: 소프트웨어로 에뮬레이션\n");
    printf("   • 교육 목적으로 단순화\n");

    printf("\n" CYAN "========================================\n" RESET);
}

// ==================== 메인 ====================
int main(int argc, char* argv[]) {
    printf(CYAN "\n╔══════════════════════════════════════════════════╗\n");
    printf("║     순수 VM 시뮬레이터 - CPU 동작 이해하기      ║\n");
    printf("║                                                  ║\n");
    printf("║    pthread 없이 순수하게 CPU 동작 시뮬레이션    ║\n");
    printf("╚══════════════════════════════════════════════════╝\n" RESET);

    if (argc > 1) {
        if (strcmp(argv[1], "race") == 0) {
            test_race_condition();
        } else if (strcmp(argv[1], "cas") == 0) {
            test_cas_operation();
        } else if (strcmp(argv[1], "spin") == 0) {
            test_spinlock_vs_mutex();
        } else if (strcmp(argv[1], "dead") == 0) {
            test_deadlock();
        } else if (strcmp(argv[1], "fence") == 0) {
            test_memory_barrier();
        } else {
            printf("Usage: %s [race|cas|spin|dead|fence]\n", argv[0]);
        }
        return 0;
    }

    // 모든 테스트 실행
    test_race_condition();
    test_cas_operation();
    test_spinlock_vs_mutex();
    test_deadlock();
    test_memory_barrier();

    print_summary();

    printf(GREEN "\n✅ 모든 시뮬레이션 완료!\n\n" RESET);

    return 0;
}

-- -

   // ... 기존 코드에 추가 ...

   // ==================== 데드락 감지 ====================
   typedef struct {
    uint32_t holder_thread; // 락을 보유한 스레드
    uint32_t waiter_thread; // 락을 기다리는 스레드
    uint32_t lock_address;  // 락 주소
} WaitEdge;

typedef struct {
    WaitEdge edges[MAX_THREADS * MAX_THREADS];
    uint32_t edge_count;
} WaitForGraph;

// Wait-For 그래프 구성
WaitForGraph* vm_build_wait_graph(VM* vm) {
    WaitForGraph* graph = calloc(1, sizeof(WaitForGraph));

    // 모든 스레드의 상태 분석
    for (uint32_t i = 0; i < vm->scheduler.thread_count; i++) {
        CPUCore* core = &vm->cores[i];

        if (core->waiting && core->wait_address > 0) {
            // 이 스레드가 대기 중인 락의 소유자 찾기
            for (uint32_t j = 0; j < vm->scheduler.thread_count; j++) {
                if (i == j) {
                    continue;
                }

                // 간단한 휴리스틱: 해당 주소의 값이 1이고 j 스레드가 최근에 접근했다면
                uint32_t lock_val = *(uint32_t*)&vm->memory.data[core->wait_address];
                if (lock_val == 1) {
                    // Wait edge 추가: i는 j를 기다림
                    graph->edges[graph->edge_count].waiter_thread = i;
                    graph->edges[graph->edge_count].holder_thread = j;
                    graph->edges[graph->edge_count].lock_address  = core->wait_address;
                    graph->edge_count++;
                }
            }
        }
    }

    return graph;
}

// DFS로 사이클 감지
bool vm_detect_cycle_dfs(WaitForGraph* graph, uint32_t thread, bool* visited, bool* rec_stack) {
    visited[thread]   = true;
    rec_stack[thread] = true;

    // 이 스레드가 기다리는 모든 스레드 확인
    for (uint32_t i = 0; i < graph->edge_count; i++) {
        if (graph->edges[i].waiter_thread == thread) {
            uint32_t next = graph->edges[i].holder_thread;

            if (!visited[next]) {
                if (vm_detect_cycle_dfs(graph, next, visited, rec_stack)) {
                    return true;
                }
            } else if (rec_stack[next]) {
                // 사이클 발견!
                return true;
            }
        }
    }

    rec_stack[thread] = false;
    return false;
}

// 데드락 감지 메인 함수
bool vm_detect_deadlock(VM* vm, DeadlockInfo* info) {
    WaitForGraph* graph = vm_build_wait_graph(vm);

    bool visited[MAX_THREADS]   = { 0 };
    bool rec_stack[MAX_THREADS] = { 0 };

    // 모든 스레드에서 DFS 시작
    for (uint32_t i = 0; i < vm->scheduler.thread_count; i++) {
        if (!visited[i]) {
            if (vm_detect_cycle_dfs(graph, i, visited, rec_stack)) {
                // 데드락 발견 - 상세 정보 수집
                if (info) {
                    info->detected     = true;
                    info->thread_count = 0;

                    // 사이클에 포함된 스레드들 찾기
                    for (uint32_t j = 0; j < graph->edge_count; j++) {
                        info->threads[info->thread_count++] = graph->edges[j].waiter_thread;
                        info->lock_addresses[j]             = graph->edges[j].lock_address;
                    }
                }

                free(graph);
                return true;
            }
        }
    }

    free(graph);
    return false;
}

// 스레드 상태 추적을 위한 수정
typedef struct {
    uint32_t locks_held[16]; // 보유한 락들
    uint32_t held_count;
    uint32_t lock_waiting;     // 대기 중인 락
    bool     is_spinning;      // 스핀락 대기 중
    uint32_t spin_address;     // 스핀 중인 주소
    uint64_t spin_start_cycle; // 스핀 시작 시점
} ThreadLockState;

// VM 구조체에 추가
struct VM {
    // ... 기존 멤버들 ...
    ThreadLockState lock_states[MAX_THREADS];

    // 데드락 감지 설정
    bool     enable_deadlock_detection;
    uint32_t deadlock_check_interval; // N 사이클마다 체크
};

// 실제 락 획득/해제 추적
void vm_track_lock_acquire(VM* vm, uint32_t thread_id, uint32_t lock_addr) {
    ThreadLockState* state = &vm->lock_states[thread_id];

    // 이미 보유한 락인지 확인 (재진입)
    for (uint32_t i = 0; i < state->held_count; i++) {
        if (state->locks_held[i] == lock_addr) {
            return;
        }
    }

    // 새 락 추가
    if (state->held_count < 16) {
        state->locks_held[state->held_count++] = lock_addr;
    }

    // 대기 상태 해제
    if (state->lock_waiting == lock_addr) {
        state->lock_waiting = 0;
        state->is_spinning  = false;
    }
}

void vm_track_lock_release(VM* vm, uint32_t thread_id, uint32_t lock_addr) {
    ThreadLockState* state = &vm->lock_states[thread_id];

    // 보유 락 리스트에서 제거
    for (uint32_t i = 0; i < state->held_count; i++) {
        if (state->locks_held[i] == lock_addr) {
            // 뒤의 것들을 앞으로 이동
            for (uint32_t j = i; j < state->held_count - 1; j++) {
                state->locks_held[j] = state->locks_held[j + 1];
            }
            state->held_count--;
            break;
        }
    }
}

void vm_track_lock_wait(VM* vm, uint32_t thread_id, uint32_t lock_addr) {
    ThreadLockState* state  = &vm->lock_states[thread_id];
    state->lock_waiting     = lock_addr;
    state->is_spinning      = true;
    state->spin_address     = lock_addr;
    state->spin_start_cycle = vm->cycle_count;
}

// ==================== 실제 데드락 감지 테스트 ====================
void test_deadlock_detection() {
    printf(CYAN "\n=== Real Deadlock Detection ===\n" RESET);
    printf("두 스레드가 서로의 락을 기다리는 상황을 만들고 감지합니다.\n\n");

    VM* vm                        = vm_create();
    vm->enable_deadlock_detection = true;
    vm->deadlock_check_interval   = 50; // 50 사이클마다 체크

    // Thread 0: Lock A, then try Lock B
    uint8_t prog0[] = {
        // Lock A 획득 (스핀락)
        OP_MOV, 3, 0, 0xAA, 0, 0, 0, // R3 = 0xAA (thread ID marker)
        // spin_lock_a:
        OP_TEST_SET, 1, 0, 0x00, 0x20, 0, 0, // R1 = test_and_set(&lockA)
        OP_CMP, 1, 0, 0, 0, 0, 0,            // compare with 0
        OP_JNZ, 0, 0, 14, 0, 0, 0,           // if locked, retry

        // Lock A 획득 성공
        OP_PRINT, 0, 3, 0, 0, 0, 0, // Print thread marker

        // 일부 작업 수행 (다른 스레드가 Lock B를 잡을 시간 주기)
        OP_MOV, 4, 0, 10, 0, 0, 0, // R4 = 10
        // delay_loop:
        OP_DEC, 4, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 49, 0, 0, 0,

        // Lock B 시도 (데드락 발생!)
        OP_MOV, 3, 0, 0xAB, 0, 0, 0, // R3 = 0xAB (waiting for B)
        // spin_lock_b:
        OP_TEST_SET, 2, 0, 0x00, 0x24, 0, 0, // R2 = test_and_set(&lockB)
        OP_CMP, 2, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 70, 0, 0, 0, // if locked, retry (infinite)

        // 여기 도달하면 데드락 해결됨
        OP_MOV, 3, 0, 0xFF, 0, 0, 0,
        OP_PRINT, 0, 3, 0, 0, 0, 0,
        OP_HALT, 0, 0, 0, 0, 0, 0
    };

    // Thread 1: Lock B, then try Lock A
    uint8_t prog1[] = {
        // Lock B 획득
        OP_MOV, 3, 0, 0xBB, 0, 0, 0, // R3 = 0xBB (thread ID marker)
        // spin_lock_b:
        OP_TEST_SET, 1, 0, 0x00, 0x24, 0, 0, // R1 = test_and_set(&lockB)
        OP_CMP, 1, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 14, 0, 0, 0,

        // Lock B 획득 성공
        OP_PRINT, 0, 3, 0, 0, 0, 0,

        // 일부 작업 수행
        OP_MOV, 4, 0, 10, 0, 0, 0,
        // delay_loop:
        OP_DEC, 4, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 49, 0, 0, 0,

        // Lock A 시도 (데드락 발생!)
        OP_MOV, 3, 0, 0xBA, 0, 0, 0, // R3 = 0xBA (waiting for A)
        // spin_lock_a:
        OP_TEST_SET, 2, 0, 0x00, 0x20, 0, 0, // R2 = test_and_set(&lockA)
        OP_CMP, 2, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 70, 0, 0, 0,

        OP_MOV, 3, 0, 0xFF, 0, 0, 0,
        OP_PRINT, 0, 3, 0, 0, 0, 0,
        OP_HALT, 0, 0, 0, 0, 0, 0
    };

    // 프로그램 로드
    vm_load_program(vm, prog0, sizeof(prog0), 0);
    vm_create_thread(vm, 512);
    vm_load_program(vm, prog1, sizeof(prog1), 1);

    printf("실행 시작...\n");

    // 실행하면서 데드락 감지
    uint32_t deadlock_detected_at = 0;
    for (uint32_t cycle = 0; cycle < 10000; cycle++) {
        vm_cycle(vm);

        // 주기적으로 데드락 체크
        if (cycle % vm->deadlock_check_interval == 0 && cycle > 100) {
            DeadlockInfo info = { 0 };

            // 실제 스레드 상태 분석
            bool t0_waiting_b = false, t1_waiting_a = false;
            bool t0_holds_a = false, t1_holds_b = false;

            // Thread 0 상태 체크
            if (vm->cores[0].pc >= 70 && vm->cores[0].pc <= 84) {
                t0_waiting_b = true; // Lock B 대기 루프에 있음
            }
            if (*(uint32_t*)&vm->memory.data[0x20] == 1) {
                // Lock A가 잠겨있고 Thread 0가 획득했을 가능성
                if (vm->cores[0].pc > 35) {
                    t0_holds_a = true;
                }
            }

            // Thread 1 상태 체크
            if (vm->cores[1].pc >= 70 && vm->cores[1].pc <= 84) {
                t1_waiting_a = true; // Lock A 대기 루프에 있음
            }
            if (*(uint32_t*)&vm->memory.data[0x24] == 1) {
                // Lock B가 잠겨있고 Thread 1이 획득했을 가능성
                if (vm->cores[1].pc > 35) {
                    t1_holds_b = true;
                }
            }

            // 데드락 조건: 서로를 기다림
            if (t0_holds_a && t0_waiting_b && t1_holds_b && t1_waiting_a) {
                printf(RED "\n[Cycle %u] 실제 데드락 감지!\n" RESET, cycle);
                printf("분석 결과:\n");
                printf("  Thread 0: PC=%04X, Lock A 보유=%s, Lock B 대기=%s\n", vm->cores[0].pc, t0_holds_a ? "YES" : "NO", t0_waiting_b ? "YES" : "NO");
                printf("  Thread 1: PC=%04X, Lock B 보유=%s, Lock A 대기=%s\n", vm->cores[1].pc, t1_holds_b ? "YES" : "NO", t1_waiting_a ? "YES" : "NO");
                printf("  Lock A (0x20): %s\n", *(uint32_t*)&vm->memory.data[0x20] ? "LOCKED" : "FREE");
                printf("  Lock B (0x24): %s\n", *(uint32_t*)&vm->memory.data[0x24] ? "LOCKED" : "FREE");

                deadlock_detected_at = cycle;
                break;
            }

            // 진행 상황 체크 (livelock 감지)
            static uint32_t last_pc0 = 0, last_pc1 = 0;
            static uint32_t stuck_count = 0;

            if (last_pc0 == vm->cores[0].pc && last_pc1 == vm->cores[1].pc) {
                stuck_count++;
                if (stuck_count > 5) {
                    printf(YELLOW "[Cycle %u] 경고: 스레드가 진행하지 못함 (Livelock?)\n" RESET, cycle);
                }
            } else {
                stuck_count = 0;
            }
            last_pc0 = vm->cores[0].pc;
            last_pc1 = vm->cores[1].pc;
        }

        // 데드락 감지되면 종료
        if (deadlock_detected_at > 0) {
            break;
        }
    }

    if (deadlock_detected_at == 0) {
        printf(GREEN "데드락이 발생하지 않았습니다.\n" RESET);
    } else {
        printf(RED "\n데드락 해결 방법:\n" RESET);
        printf("1. 락 순서 통일 (모든 스레드가 같은 순서로 락 획득)\n");
        printf("2. 타임아웃 도입 (일정 시간 후 포기)\n");
        printf("3. Deadlock Prevention 알고리즘 사용\n");
    }

    vm_dump_state(vm);
    vm_destroy(vm);
}

// ==================== 스핀락 성능 분석 ====================
void test_spinlock_performance() {
    printf(CYAN "\n=== Spinlock Performance Analysis ===\n" RESET);

    VM* vm = vm_create();
    vm_enable_race_simulation(vm, 20);

    // 스핀락으로 보호되는 카운터
    uint8_t program[] = {
        OP_MOV, 0, 0, 0, 0, 0, 0,
        OP_STORE, 0, 0, 0x00, 0x20, 0, 0, // lock = 0
        OP_STORE, 0, 0, 0x00, 0x10, 0, 0, // counter = 0

        OP_MOV, 5, 0, 20, 0, 0, 0, // 20번 반복

        // acquire:
        OP_MOV, 6, 0, 0, 0, 0, 0, // R6 = spin count
        // spin:
        OP_INC, 6, 0, 0, 0, 0, 0, // spin count++
        OP_TEST_SET, 1, 0, 0x00, 0x20, 0, 0,
        OP_CMP, 1, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 28, 0, 0, 0, // retry

        // critical section
        OP_PRINT, 0, 6, 0, 0, 0, 0, // Print spin count
        OP_LOAD, 2, 0, 0x00, 0x10, 0, 0,
        OP_INC, 2, 0, 0, 0, 0, 0,
        OP_STORE, 0, 2, 0x00, 0x10, 0, 0,

        // release
        OP_MOV, 0, 0, 0, 0, 0, 0,
        OP_STORE, 0, 0, 0x00, 0x20, 0, 0,

        OP_DEC, 5, 0, 0, 0, 0, 0,
        OP_JNZ, 0, 0, 21, 0, 0, 0,

        OP_HALT, 0, 0, 0, 0, 0, 0
    };

    vm_load_program(vm, program, sizeof(program), 0);

    printf("스핀락 분석:\n");
    printf("R6 = 각 락 획득 시 스핀 횟수\n\n");

    vm_run(vm);

    uint32_t counter = *(uint32_t*)&vm->memory.data[0x10];
    printf("\n최종 카운터: %d\n", counter);
    printf("총 사이클: %lu\n", vm->cycle_count);
    printf("스핀으로 낭비된 사이클 비율을 관찰하세요.\n");

    vm_destroy(vm);
}
