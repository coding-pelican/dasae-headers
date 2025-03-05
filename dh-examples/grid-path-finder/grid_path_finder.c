#include <pthread.h>

#include "dh/main.h"
#include "dh/claim/unreachable.h"
#include "dh/core.h"

#include "dh/debug/common.h"
#include "dh/heap/Page.h"

#include "dh/time.h"
#include "dh/Random.h"

#include "dh/time/Duration.h"
#include "dh/time/Instant.h"
#include "engine.h"
#include "cfg_values.h"

#if debug_comp_enabled
#define main_window_res_width  (window_res_width__80x50)
#define main_window_res_height (window_res_height__80x50)
#else /* !debug_comp_enabled */
#define main_window_res_width  (window_res_width__320x160 - 20)
#define main_window_res_height (window_res_height__320x160 + 40)
#endif /* debug_comp_enabled */
#define main_window_res_size (as$(usize, main_window_res_width) * main_window_res_height)

#define main_target_fps (target_fps__31_250)
#define main_target_spf (1.0 / main_target_fps)


#include "dh/ArrList.h"

// First declare and implement ArrList for Vec2i
use_Sli$(Vec2i);
use_ArrList$(Vec2i);

// Stack implementation using ArrList
typedef struct Stack_Vec2i {
    ArrList$Vec2i list;
} Stack_Vec2i;
Stack_Vec2i Stack_Vec2i_create(mem_Allocator allocator) {
    return (Stack_Vec2i){
        .list = {
            .base = { [0] = ArrList_init(typeInfo$(Vec2i), allocator) } }
    };
}
void Stack_Vec2i_destroy(Stack_Vec2i* self) {
    ArrList_fini(self->list.base);
}
bool Stack_Vec2i_empty(const Stack_Vec2i* self) {
    return self->list.items.len == 0;
}
mem_AllocErr$void Stack_Vec2i_push(Stack_Vec2i* self, Vec2i item) {
    return ArrList_append(self->list.base, meta_refPtr(&item));
}
Vec2i Stack_Vec2i_top(const Stack_Vec2i* self) {
    return *Sli_at(self->list.items, self->list.items.len - 1);
}
void Stack_Vec2i_pop(Stack_Vec2i* self) {
    ArrList_pop(self->list.base);
}

// Queue implementation using ArrList
typedef struct Queue_Vec2i {
    ArrList$Vec2i list;
} Queue_Vec2i;
Queue_Vec2i Queue_Vec2i_create(mem_Allocator allocator) {
    return (Queue_Vec2i){
        .list = {
            .base = { [0] = ArrList_init(typeInfo$(Vec2i), allocator) } }
    };
}
void Queue_Vec2i_destroy(Queue_Vec2i* self) {
    ArrList_fini(self->list.base);
}
bool Queue_Vec2i_empty(const Queue_Vec2i* self) {
    return self->list.items.len == 0;
}
mem_AllocErr$void Queue_Vec2i_push(Queue_Vec2i* self, Vec2i item) {
    return ArrList_append(self->list.base, meta_refPtr(&item));
}
Vec2i Queue_Vec2i_front(const Queue_Vec2i* self) {
    return *Sli_at(self->list.items, 0);
}
void Queue_Vec2i_pop(Queue_Vec2i* self) {
    ArrList_shift(self->list.base);
}


// Maze cell states
typedef enum Cell {
    Cell_wall,
    Cell_open,
    Cell_visited
} Cell;

// Global variables
#if debug_comp_enabled
#define maze_size       (20)
#define maze_cell_scale (2)
#else
#define maze_size       (32)
#define maze_cell_scale (5)
#endif /* debug_comp_enabled */
static Cell maze[maze_size][maze_size];
static bool visited[maze_size][maze_size];

// Directions for movement (left, right, up, down)
static Vec2i dirs[] = {
    math_Vec2i_left,
    math_Vec2i_right,
    math_Vec2i_up,
    math_Vec2i_down
};
static const usize dirs_size = countOf(dirs);

// Thread synchronization primitives
static pthread_mutex_t maze_mutex        = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  algo_done_cond    = PTHREAD_COND_INITIALIZER;
static bool            algorithm_running = false;
static int             current_algorithm = -1; // 0: DFS, 1: BFS, 2: Quantum

// Initialize the maze with random walls and open spaces
void InitMaze(void) {
    pthread_mutex_lock(&maze_mutex);
    for (i32 i = 0; i < maze_size; ++i) {
        for (i32 j = 0; j < maze_size; ++j) {
            maze[i][j]    = (Random_i32() % 5 == 0) ? Cell_wall : Cell_open;
            visited[i][j] = false;
        }
    }
    maze[0][0] = maze[maze_size - 1][maze_size - 1] = Cell_open;
    pthread_mutex_unlock(&maze_mutex);
}

// Thread structures
typedef struct {
    i32           start_x;
    i32           start_y;
    time_Duration delta_time;
} ThreadArgs;

// Modified thread functions using our new implementations
void* DFS_thread(void* args) {
    ThreadArgs* tArgs      = (ThreadArgs*)args;
    Stack_Vec2i stack      = Stack_Vec2i_create(heap_Page_allocator(&(heap_Page){}));
    let         start_time = time_Instant_now();
    Vec2i       start      = { .x = tArgs->start_x, .y = tArgs->start_y };

    pthread_mutex_lock(&maze_mutex);
    catch (Stack_Vec2i_push(&stack, start), err, {
        log_error("Error pushing start point onto stack: %s", Err_codeToCStr(err));
        Stack_Vec2i_destroy(&stack);
        pthread_mutex_unlock(&maze_mutex);
        return null;
    });
    visited[tArgs->start_x][tArgs->start_y] = true;
    pthread_mutex_unlock(&maze_mutex);

    while (!Stack_Vec2i_empty(&stack) && time_Duration_lt(time_Instant_elapsed(start_time), tArgs->delta_time)) {
        pthread_mutex_lock(&maze_mutex);
        Vec2i current = Stack_Vec2i_top(&stack);
        Stack_Vec2i_pop(&stack);
        maze[current.x][current.y] = Cell_visited;

        for (i32 i = 0; i < 4; ++i) {
            i32 nx = current.x + dirs[i].x;
            i32 ny = current.y + dirs[i].y;
            if (nx >= 0 && ny >= 0 && nx < maze_size && ny < maze_size && !visited[nx][ny] && maze[nx][ny] != Cell_wall) {
                visited[nx][ny] = true;
                Vec2i next      = { .x = nx, .y = ny };
                catch (Stack_Vec2i_push(&stack, next), err, {
                    log_error("Error pushing next point onto stack: %s", Err_codeToCStr(err));
                    Stack_Vec2i_destroy(&stack);
                    pthread_mutex_unlock(&maze_mutex);
                    return null;
                });
            }
        }
        pthread_mutex_unlock(&maze_mutex);
        time_sleepMillis(10);
    }

    pthread_mutex_lock(&maze_mutex);
    algorithm_running = false;
    pthread_cond_signal(&algo_done_cond);
    pthread_mutex_unlock(&maze_mutex);

    Stack_Vec2i_destroy(&stack);
    free(args);
    return null;
}

void* BFS_thread(void* args) {
    ThreadArgs* tArgs      = (ThreadArgs*)args;
    Queue_Vec2i queue      = Queue_Vec2i_create(heap_Page_allocator(&(heap_Page){}));
    let         start_time = time_Instant_now();
    Vec2i       start      = { .x = tArgs->start_x, .y = tArgs->start_y };

    pthread_mutex_lock(&maze_mutex);
    catch (Queue_Vec2i_push(&queue, start), err, {
        log_error("Error pushing start point onto queue: %s", Err_codeToCStr(err));
        Queue_Vec2i_destroy(&queue);
        pthread_mutex_unlock(&maze_mutex);
        return null;
    });
    visited[tArgs->start_x][tArgs->start_y] = true;
    pthread_mutex_unlock(&maze_mutex);

    while (!Queue_Vec2i_empty(&queue) && time_Duration_lt(time_Instant_elapsed(start_time), tArgs->delta_time)) {
        Vec2i current = Queue_Vec2i_front(&queue);
        Queue_Vec2i_pop(&queue);
        maze[current.x][current.y] = Cell_visited;

        for (i32 i = 0; i < 4; ++i) {
            i32 nx = current.x + dirs[i].x;
            i32 ny = current.y + dirs[i].y;
            if (nx >= 0 && ny >= 0 && nx < maze_size && ny < maze_size && !visited[nx][ny] && maze[nx][ny] != Cell_wall) {
                visited[nx][ny] = true;
                Vec2i next      = { .x = nx, .y = ny };
                catch (Queue_Vec2i_push(&queue, next), err, {
                    log_error("Error pushing next point onto queue: %s", Err_codeToCStr(err));
                    Queue_Vec2i_destroy(&queue);
                    pthread_mutex_unlock(&maze_mutex);
                    return null;
                });
            }
        }
        pthread_mutex_unlock(&maze_mutex);
        time_sleepMillis(10);
    }

    pthread_mutex_lock(&maze_mutex);
    algorithm_running = false;
    pthread_cond_signal(&algo_done_cond);
    pthread_mutex_unlock(&maze_mutex);

    Queue_Vec2i_destroy(&queue);
    free(args);
    return null;
}

void* QuantumSearch_thread(void* args) {
    ThreadArgs* tArgs      = (ThreadArgs*)args;
    Queue_Vec2i queue      = Queue_Vec2i_create(heap_Page_allocator(&(heap_Page){}));
    let         start_time = time_Instant_now();
    Vec2i       start      = { .x = tArgs->start_x, .y = tArgs->start_y };

    pthread_mutex_lock(&maze_mutex);
    catch (Queue_Vec2i_push(&queue, start), err, {
        log_error("Failed to push start point into queue", Err_codeToCStr(err));
        Queue_Vec2i_destroy(&queue);
        pthread_mutex_unlock(&maze_mutex);
        return null;
    });
    visited[tArgs->start_x][tArgs->start_y] = true;
    pthread_mutex_unlock(&maze_mutex);

    while (!Queue_Vec2i_empty(&queue) && time_Duration_lt(time_Instant_elapsed(start_time), tArgs->delta_time)) {
        Vec2i current = Queue_Vec2i_front(&queue);
        Queue_Vec2i_pop(&queue);
        maze[current.x][current.y] = Cell_visited;

        for (i32 i = 0; i < 4; ++i) {
            i32 nx = current.x + dirs[i].x;
            i32 ny = current.y + dirs[i].y;
            if (nx >= 0 && ny >= 0 && nx < maze_size && ny < maze_size && !visited[nx][ny] && maze[nx][ny] != Cell_wall) {
                visited[nx][ny] = true;
                Vec2i next      = { .x = nx, .y = ny };
                catch (Queue_Vec2i_push(&queue, next), err, {
                    log_error("Failed to push next point into queue", Err_codeToCStr(err));
                    Queue_Vec2i_destroy(&queue);
                    pthread_mutex_unlock(&maze_mutex);
                    return null;
                });
            }
        }
        pthread_mutex_unlock(&maze_mutex);
        time_sleepMillis(10);
    }

    pthread_mutex_lock(&maze_mutex);
    algorithm_running = false;
    pthread_cond_signal(&algo_done_cond);
    pthread_mutex_unlock(&maze_mutex);

    Queue_Vec2i_destroy(&queue);
    free(args);
    return null;
}

// Draw the maze
void DrawMaze(engine_Canvas* canvas) {
    pthread_mutex_lock(&maze_mutex);
    static const Color dark_gray  = literal_Color_fromOpaque(100, 100, 100);
    static const Color light_gray = literal_Color_fromOpaque(200, 200, 200);
    for (i32 i = 0; i < maze_size; ++i) {
        for (i32 j = 0; j < maze_size; ++j) {
            let color = eval({
                Color c = cleared();
                switch (maze[i][j]) {
                case Cell_wall:
                    c = dark_gray;
                    break;
                case Cell_open:
                    c = light_gray;
                    break;
                case Cell_visited:
                    c = Color_green;
                    break;
                }
                eval_return c;
            });

            let unit_cell = math_Vec2i_fill(maze_cell_scale);
            let pos       = math_Vec2i_scale(math_Vec2i_from(i, j), maze_cell_scale);
            let max       = math_Vec2i_add(pos, unit_cell);
            engine_Canvas_fillRect(canvas, pos.x, pos.y, max.x, max.y, color);
        }
    }
    pthread_mutex_unlock(&maze_mutex);
}

Err$void dh_main(Sli$Str_const args) {
    unused(args);
    scope_reserveReturn(Err$void) {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("debug.log", "w"), debug_file) {
            log_initWithFile(debug_file);
            // Configure logging behavior
            log_setLevel(log_Level_info);
            log_showTimestamp(true);
            log_showLevel(true);
            log_showLocation(false);
            log_showFunction(true);
        }
        defer(log_fini());

        log_info("initializing...\n");
        // Create window
        var window = try(engine_Window_init(&(engine_PlatformParams){
            .backend_type = engine_RenderBackendType_vt100,
            .window_title = "Grid Path Finder Simulation",
            .width        = main_window_res_width,
            .height       = main_window_res_height,
        }));
        defer(engine_Window_fini(window));
        log_info("window created\n");

        var canvas = try(engine_Canvas_createWithDefault(main_window_res_width, main_window_res_height, engine_CanvasType_rgba, Color_transparent));
        defer(engine_Canvas_destroy(canvas));
        log_info("canvas created\n");

        engine_Window_addCanvasView(window, canvas, 0, 0, main_window_res_width, main_window_res_height);
        log_info("canvas views added\n");

        Random_init();
        log_info("random initialized\n");

        // init for DFS, BFS, and Quantum Search
        InitMaze();
        log_info("maze initialized\n");

        ignore getchar();

        // Run each algorithm for 5 seconds
        pthread_t navigation_thread = 0;
        defer(if (navigation_thread) { pthread_join(navigation_thread, null); });
        let execute_interval = time_Duration_fromSecs(30);
        defer(
            pthread_mutex_destroy(&maze_mutex);
            pthread_cond_destroy(&algo_done_cond);
        );

        // Initialize timing variables
        let time_frame_target = time_Duration_fromSecs_f64(main_target_spf);
        var time_frame_prev   = time_Instant_now();
        log_info("main loop started\n");

        bool is_running = true;
        while (is_running) {
            let time_frame_curr = time_Instant_now();
            let time_elapsed    = time_Instant_durationSince(time_frame_curr, time_frame_prev);
            let time_dt         = time_Duration_asSecs_f64(time_elapsed);

            // 1) Process input
            try(engine_Window_processEvents(window));

            // 2) Update and thread management
            if (engine_Key_pressed(engine_KeyCode_esc)) { is_running = false; }

            if (!algorithm_running) {
                current_algorithm = (current_algorithm + 1) % 3;
                algorithm_running = true;
                InitMaze();

                let args         = (ThreadArgs*)malloc(sizeof(ThreadArgs));
                args->start_x    = 0;
                args->start_y    = 0;
                args->delta_time = execute_interval;

                log_info("algorithm running\n");
                switch (current_algorithm) {
                case 0:
                    pthread_create(&navigation_thread, null, DFS_thread, args);
                    break;
                case 1:
                    pthread_create(&navigation_thread, null, BFS_thread, args);
                    break;
                case 2:
                    pthread_create(&navigation_thread, null, QuantumSearch_thread, args);
                    break;
                default:
                    claim_unreachable;
                }
            }

            // 3) Render and timing code remains the same...
            {
                // Draw the current state
                DrawMaze(canvas);
                engine_Window_present(window);

                // Display screen info
                {
                    printf("\033[H\033[40;37m"); // Move cursor to top left
                    const f64 time_fps = (0.0 < time_dt) ? (1.0 / time_dt) : 9999.0;
                    {
                        printf(
                            "\rFPS:%5.2f | RES:%dx%d\n",
                            time_fps,
                            main_window_res_width,
                            main_window_res_height
                        );
                    }
                    printf("\033[0m"); // Reset color
                    debug_only(        // log frame every 1s
                        static f64 total_game_time_for_timestamp = 0.0;
                        static f64 logging_after_duration        = 0.0;
                        total_game_time_for_timestamp += time_dt;
                        logging_after_duration += time_dt;
                        if (1.0 < logging_after_duration) {
                            logging_after_duration = 0.0;
                            log_debug("[t=%6.2f] dt: %6.2f, fps %6.2f\n", total_game_time_for_timestamp, time_dt, 1.0 / time_dt);
                        }
                    );
                }

                // Display current algorithm
                {
                    printf("\033[H\n");
                    switch (current_algorithm) {
                    case 0:
                        printf("\033[40;31mDFS Exploration\033[0m\n");
                        break;
                    case 1:
                        printf("\033[40;34mBFS Exploration\033[0m\n");
                        break;
                    case 2:
                        printf("\033[40;32mQuantum Search Exploration\033[0m\n");
                        break;
                    default:
                        claim_unreachable;
                    }
                }
            }

            // 5) sleep
            let time_now        = time_Instant_now();
            let time_frame_used = time_Instant_durationSince(time_now, time_frame_curr);
            if_some(time_Duration_chkdSub(time_frame_target, time_frame_used), time_leftover) {
                time_sleep(time_leftover);
            }
            time_frame_prev = time_frame_curr;
        }
        return_void();
    }
    scope_returnReserved;
}
