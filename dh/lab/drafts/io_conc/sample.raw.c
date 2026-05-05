#include <stdio.h>

typedef enum TaskState {
    TASK_START,
    TASK_WAITING,
    TASK_DONE,
} TaskState;

typedef struct Task {
    const char* label;
    int n;
    int i;
    double interval;
    double started_at;
    double wake_at;
    double result;
    TaskState state;
} Task;

static void task_step(Task* task, double now) {
    switch (task->state) {
    case TASK_START:
        task->started_at = now;
        printf("[%s] before loop %.1f\n", task->label, task->interval);
        task->wake_at = now + task->interval;
        task->state = TASK_WAITING;
        return;

    case TASK_WAITING:
        if (now < task->wake_at) return;
        printf(
            "[%s] slept %.1f | i: %d < n: %d\n",
            task->label,
            task->interval,
            task->i,
            task->n
        );
        task->i += 1;
        if (task->i < task->n) {
            task->wake_at = now + task->interval;
            return;
        }
        task->result = now - task->started_at;
        printf("[%s] after loop %.1f\n", task->label, task->result);
        task->state = TASK_DONE;
        return;

    case TASK_DONE:
        return;
    }
}

static int task_is_done(const Task* task) {
    return task->state == TASK_DONE;
}

static double next_wake_at(const Task* a, const Task* b) {
    if (task_is_done(a)) return b->wake_at;
    if (task_is_done(b)) return a->wake_at;
    return a->wake_at < b->wake_at ? a->wake_at : b->wake_at;
}

int main(void) {
    Task task_a = {
        .label = "task a",
        .n = 2,
        .interval = 1.0,
        .state = TASK_START,
    };
    Task task_b = {
        .label = "task b",
        .n = 3,
        .interval = 0.6,
        .state = TASK_START,
    };

    double now = 0.0;

    puts("begin");
    task_step(&task_a, now);
    task_step(&task_b, now);

    while (!task_is_done(&task_a) || !task_is_done(&task_b)) {
        now = next_wake_at(&task_a, &task_b);
        task_step(&task_a, now);
        task_step(&task_b, now);
    }

    puts("end");
    printf("total: %.1f\n", task_a.result + task_b.result);
    return 0;
}
