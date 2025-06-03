// dh-c build release sample-multi_rogue.c --args="-lws2_32"
//
// CLI Usage:
// ./sample-multi_rogue --server                     # Start server
// ./sample-multi_rogue --client                     # Connect to localhost:8080
// ./sample-multi_rogue --client 192.168.1.100       # Connect to specific IP
// ./sample-multi_rogue --client 192.168.1.100:9090  # Connect to specific IP:port
// ./sample-multi_rogue --dummy-ai                   # AI player on localhost
// ./sample-multi_rogue --dummy-ai 192.168.1.100     # AI player on remote server
//
// Performance optimizations available:
// - Add -DFAST_RENDER to enable optimized frame-buffer based rendering
//   1. Batching all terminal output into single buffer
//   2. Only redrawing cells that have changed (dirty rectangle detection)
//   3. Reducing ANSI escape sequence redundancy
//   4. Minimizing printf syscalls from ~2000 to 1 per frame
//
// Example: dh-c build release sample-multi_rogue.c --args="-lws2_32 -DFAST_RENDER"
//

#include "dh/main.h"
#include "dh/blk.h"
#include "dh/Err.h"
#include "dh/sli.h"
#include "dh/Arr.h"
#include "dh/Str.h"
#include "dh/Grid.h"
#include "dh/Pair.h"
#include "dh/Thrd.h"
#include "dh/heap/Page.h"
#include "dh/mem/common.h"
#include "dh/Random.h"
#include "dh/time.h"

/* ANSI escape codes ========================================================*/

#define ansi_attr_reset         "\033[0m"
#define ansi_attr_bold_on       "\033[1m"
#define ansi_attr_bold_off      "\033[21m"
#define ansi_attr_dim_on        "\033[2m"
#define ansi_attr_dim_off       "\033[22m"
#define ansi_attr_underline_on  "\033[4m"
#define ansi_attr_underline_off "\033[24m"
#define ansi_attr_blink_on      "\033[5m"
#define ansi_attr_blink_off     "\033[25m"
#define ansi_attr_reverse_on    "\033[7m"
#define ansi_attr_reverse_off   "\033[27m"
#define ansi_attr_hidden_on     "\033[8m"
#define ansi_attr_hidden_off    "\033[28m"

#define ansi_attr_fg_black   "\033[30m"
#define ansi_attr_fg_red     "\033[31m"
#define ansi_attr_fg_green   "\033[32m"
#define ansi_attr_fg_yellow  "\033[33m"
#define ansi_attr_fg_blue    "\033[34m"
#define ansi_attr_fg_magenta "\033[35m"
#define ansi_attr_fg_cyan    "\033[36m"
#define ansi_attr_fg_white   "\033[37m"
#define ansi_attr_fg_default "\033[39m"

#define ansi_attr_fg_bright_black   "\033[90m"
#define ansi_attr_fg_bright_red     "\033[91m"
#define ansi_attr_fg_bright_green   "\033[92m"
#define ansi_attr_fg_bright_yellow  "\033[93m"
#define ansi_attr_fg_bright_blue    "\033[94m"
#define ansi_attr_fg_bright_magenta "\033[95m"
#define ansi_attr_fg_bright_cyan    "\033[96m"
#define ansi_attr_fg_bright_white   "\033[97m"
#define ansi_attr_fg_bright_default "\033[99m"

#define ansi_attr_bg_black   "\033[40m"
#define ansi_attr_bg_red     "\033[41m"
#define ansi_attr_bg_green   "\033[42m"
#define ansi_attr_bg_yellow  "\033[43m"
#define ansi_attr_bg_blue    "\033[44m"
#define ansi_attr_bg_magenta "\033[45m"
#define ansi_attr_bg_cyan    "\033[46m"
#define ansi_attr_bg_white   "\033[47m"
#define ansi_attr_bg_default "\033[49m"

#define ansi_attr_bg_bright_black   "\033[100m"
#define ansi_attr_bg_bright_red     "\033[101m"
#define ansi_attr_bg_bright_green   "\033[102m"
#define ansi_attr_bg_bright_yellow  "\033[103m"
#define ansi_attr_bg_bright_blue    "\033[104m"
#define ansi_attr_bg_bright_magenta "\033[105m"
#define ansi_attr_bg_bright_cyan    "\033[106m"
#define ansi_attr_bg_bright_white   "\033[107m"
#define ansi_attr_bg_bright_default "\033[109m"

/* engine utils =============================================================*/

#if bti_plat_windows
#include "dh/os/windows.h"

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

/* game common ==============================================================*/

// common.h - Shared definitions and structures
#ifndef GAME_COMMON_INCLUDED
#define GAME_COMMON_INCLUDED (1)

#include <stdio.h>
#include <unistd.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#define game_port                (8080)
#define game_max_players         (10)
#define game_max_player_name_len (32)
#define game_max_player_msg_len  (248)
#define game_max_map_width       (64)
#define game_max_map_height      (20)
#define game_max_ip_addr_len     (64)

// Player class
typedef enum game_PlayerClass {
    game_PlayerClass_warrior = 0,
    game_PlayerClass_rogue   = 1,
    game_PlayerClass_archer  = 2,
    game_PlayerClass_mage    = 3,
    game_PlayerClass_count
} game_PlayerClass;
fn_(game_PlayerClass_strfy(game_PlayerClass self), Sli_const$u8) {
    static const Arr$$(game_PlayerClass_count, Sli_const$u8) s_class_names = { {
        [game_PlayerClass_warrior] = u8_l("warrior"),
        [game_PlayerClass_rogue]   = u8_l("rogue"),
        [game_PlayerClass_archer]  = u8_l("archer"),
        [game_PlayerClass_mage]    = u8_l("mage"),
    } };
    return Arr_getAt(s_class_names, self);
}
use_Opt$(game_PlayerClass);
fn_(game_PlayerClass_parse(Sli_const$u8 self, bool ignore_case), Opt$game_PlayerClass, $scope) {
    let eql   = ignore_case ? Str_eqlNoCase : Str_eql;
    let strfy = game_PlayerClass_strfy;
    if (eql(self, strfy(game_PlayerClass_warrior))) { return_some(game_PlayerClass_warrior); }
    if (eql(self, strfy(game_PlayerClass_rogue))) { return_some(game_PlayerClass_rogue); }
    if (eql(self, strfy(game_PlayerClass_archer))) { return_some(game_PlayerClass_archer); }
    if (eql(self, strfy(game_PlayerClass_mage))) { return_some(game_PlayerClass_mage); }
    return_none();
} $unscoped;

// Player structure
typedef struct game_Player {
    i32  id;
    char name[game_max_player_name_len];
    i32  x, y;
    i32  hp, max_hp;
    i32  mp, max_mp;
    i32  level;
    i32  exp;
    game_PlayerClass class;
    char symbol;
    i32  socket_fd;
    bool active;
} game_Player;
use_Ptr$(game_Player);
use_Sli$(game_Player);
use_Opt$(game_Player);

// Tile types
typedef enum game_TileType {
    game_TileType_empty     = '.',
    game_TileType_wall      = '#',
    game_TileType_door      = '+',
    game_TileType_stairs_up = '<',
    game_TileType_stairs_dn = '>',
    game_TileType_treasure  = '$',
    game_TileType_monster   = 'M'
} game_TileType;
fn_(game_TileType_getColor(game_TileType self), Sli_const$u8) {
    switch (self) {
    case game_TileType_empty:
        return u8_l(ansi_attr_fg_bright_black);
    case game_TileType_wall:
        $fallthrough;
    case game_TileType_door:
        $fallthrough;
    case game_TileType_stairs_up:
        $fallthrough;
    case game_TileType_stairs_dn:
        return u8_l(ansi_attr_fg_white);
    case game_TileType_treasure:
        return u8_l(ansi_attr_fg_bright_yellow);
    case game_TileType_monster:
        return u8_l(ansi_attr_fg_green);
    default:
        return u8_l(ansi_attr_fg_bright_white);
    }
}

// Game state structure
use_Grid$(u8);
typedef struct game_State {
    var_(mem, struct {
        var_(players, Arr$$(game_max_players, game_Player));
        var_(map, Arr$$(game_max_map_width * game_max_map_height, u8));
    });
    var_(players, Sli$game_Player);
    var_(player_count, i32);
    var_(map, Grid$u8);
    var_(mtx, Thrd_Mtx);
} game_State;

// Message types for network communication
typedef enum game_EventTag {
    game_EventTag_none = 0,
    game_EventTag_game_state,
    game_EventTag_clt_connect,
    game_EventTag_clt_disconnect,
    game_EventTag_clt_id_asg,
    game_EventTag_clt_join,
    game_EventTag_clt_leave,
    game_EventTag_move,
    game_EventTag_attack,
    game_EventTag_chat,
} game_EventTag;
// Game event structure
typedef struct game_Event {
    game_EventTag tag;
    union {
        struct game_Event_SvrConnect {
            i32 id;
        } clt_connect;
        struct game_Event_SvrDisconnect {
            i32 id;
        } clt_disconnect;
        struct game_Event_CltJoin {
            i32 id;
        } clt_join;
        struct game_Event_CltLeave {
            i32 id;
        } clt_leave;
        struct game_Event_CltIdAsg {
            i32 id;
        } clt_id_asg;
        struct game_Event_Move {
            i32 id;
            i32 x, y;
        } move;
        struct game_Event_Attack {
            i32 id;
            i32 x, y;
        } attack;
        struct game_Event_Chat {
            i32 id;
            Arr$$(game_max_player_msg_len, u8) msg;
            usize msg_len;
        } chat;
    } payload;
} game_Event;

#endif /* GAME_COMMON_INCLUDED */

/* game Server ==============================================================*/

typedef SOCKET Socket;
$static fn_(Socket_send(Socket self, Sli_const$u8 buf, i32 flag), void) {
    send(self, as$(const char*, buf.ptr), as$(int, buf.len), flag);
}
$must_check
$static fn_(Socket_recv(Socket self, Sli$u8 buf, i32 flag), Err$usize, $scope) {
    let len = recv(self, as$(char*, buf.ptr), as$(int, buf.len), flag);
    if (len <= 0) { return_err(Err_Unexpected()); }
    return_ok(len);
} $unscoped;
#if bti_plat_windows
$must_check
$static fn_(Socket_initWinsock(void), Err$void, $scope) {
    var wsaData = (WSADATA){};
    let result  = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return_err(Err_Unexpected());
    }
    return_ok({});
} $unscoped;
$static fn_(Socket_cleanupWinsock(void), void) {
    WSACleanup();
}
#else  /* !bti_plat_windows */
#endif /* bti_plat_windows */

typedef struct game_Svr {
    game_State state;
    Socket     socket;
} game_Svr;
static fn_(game_Svr_inst(void), game_Svr*) {
    static game_Svr inst = {};
    return &inst;
}
static fn_(game_Svr_init(void), game_Svr*) {
    let self = game_Svr_inst();
    Random_init();

    self->state.mtx          = Thrd_Mtx_init();
    self->state.players      = Sli_arr$(Sli$game_Player, self->state.mem.players);
    self->state.player_count = 0;
    self->state.map          = Grid_fromSli$(Grid$u8, Sli_arr$(Sli$u8, self->state.mem.map), game_max_map_width, game_max_map_height);

    // Initialize simple dungeon map
    for_grid(self->state.map, (tile, (px, py), (width, height)), {
        if (px == 0 || px == width - 1 || py == 0 || py == height - 1) { // NOLINT
            *tile = game_TileType_wall;
        } else if (Random_range_i64(0, 10) == 0) {
            *tile = game_TileType_wall;
        } else if (Random_range_i64(0, 50) == 0) {
            *tile = game_TileType_treasure;
        } else {
            *tile = game_TileType_empty;
        }
    });

    // Add some rooms and corridors
    for (i32 i = 0; i < 5; ++i) {
        let room_x = 10 + as$(i32, (Random_range_i64(0, 60)));
        let room_y = 5 + as$(i32, (Random_range_i64(0, 14)));
        let room_w = 5 + as$(i32, (Random_range_i64(0, 10)));
        let room_h = 3 + as$(i32, (Random_range_i64(0, 6)));

        for (i32 py = room_y; py < room_y + room_h && py < as$(i32, Grid_height(self->state.map)) - 1; ++py) {
            for (i32 px = room_x; px < room_x + room_w && px < as$(i32, Grid_width(self->state.map)) - 1; ++px) {
                Grid_setAt(self->state.map, px, py, game_TileType_empty);
            }
        }
    }
    return self;
}

use_Pair$(i32, i32);
use_Opt$(Pair$(i32, i32));
static fn_(game_Svr_findSpawnLocation(void), Opt$(Pair$(i32, i32)), $scope) {
    let self = game_Svr_inst();
    for (i32 attempts = 0; attempts < 100; ++attempts) {
        let x = 1 + Random_range_i64(0, as$(i32, Grid_width(self->state.map)) - 2);
        let y = 1 + Random_range_i64(0, as$(i32, Grid_height(self->state.map)) - 2);
        if (Grid_getAt(self->state.map, x, y) == game_TileType_empty) {
            return_some({ .first = x, .second = y });
        }
    }
    return_none();
} $unscoped;

static fn_(game_Svr_broadcastState(void), void) {
    let self = game_Svr_inst();
    var msg  = (game_Event){ .tag = game_EventTag_game_state };
    for_slice (self->state.players, (player)) {
        if (!player->active) { continue; }
        Socket_send(player->socket_fd, mem_asBytes_const(&msg), 0);
        Socket_send(player->socket_fd, mem_asBytes_const(&self->state), 0);
    }
}

static fn_(game_Svr_isValidMove(i32 x, i32 y), bool) {
    let self = game_Svr_inst();
    return 0 <= x && x < as$(i32, Grid_width(self->state.map))
        && 0 <= y && y < as$(i32, Grid_height(self->state.map))
        && Grid_getAt(self->state.map, x, y) != game_TileType_wall;
}
static fn_(game_Svr_handlePlayerMove(i32 player_id, i32 new_x, i32 new_y), void, $guard) {
    let self = game_Svr_inst();
    Thrd_Mtx_lock(&self->state.mtx);
    defer_({
        Thrd_Mtx_unlock(&self->state.mtx);
        game_Svr_broadcastState();
    });

    let player = Sli_at(self->state.players, player_id);
    // Validate move
    if (!game_Svr_isValidMove(new_x, new_y)) { return_void(); }
    // Check for collision with other players
    if (!blk_(collision, bool, {
            for_slice (self->state.players, (other)) {
                if (other->id == player_id) { continue; }
                if (!other->active) { continue; }
                if (!(other->x == new_x && other->y == new_y)) { continue; }
                blk_break_(collision, true);
            }
            blk_break_(collision, false);
        })) {
        player->x = new_x;
        player->y = new_y;
    }
    // Check for treasure pickup
    block_defer if (Grid_getAt(self->state.map, new_x, new_y) == game_TileType_treasure) {
        defer_(Grid_setAt(self->state.map, new_x, new_y, game_TileType_empty));
        player->exp += 10;
        if (player->exp >= player->level * 100) {
            player->level++;
            player->max_hp += 10;
            player->hp = player->max_hp;
        }
    } block_deferral;

    return_void();
} $unguarded;

static fn_(game_Svr_findAvailablePlayerId(void), Opt$i32, $scope) {
    let self = game_Svr_inst();
    for_slice_indexed (self->state.players, (player), (idx)) {
        if (player->active) { continue; }
        return_some(idx);
    }
    return_none();
} $unscoped;

static Thrd_fn_(game_Svr_handleClt, ({ Socket socket; }, Void), ($ignore_capture, args), $guard) {
    let self   = game_Svr_inst();
    let socket = args->socket;
    defer_(close(socket));

    // Find available player slot
    var id = 0;
    block_defer {
        Thrd_Mtx_lock(&self->state.mtx);
        defer_({
            Thrd_Mtx_unlock(&self->state.mtx);
            game_Svr_broadcastState();
        });

        id = orelse(game_Svr_findAvailablePlayerId(), eval({
            printf("No available player slot\n");
            return_void();
        }));
        /* Initialize player */ {
            let player        = Sli_at(self->state.players, id);
            player->id        = id;
            $ignore           = snprintf(player->name, game_max_player_name_len, "Player%d", id);
            player->socket_fd = as$(i32, socket);
            player->active    = true;
            player->hp = player->max_hp = 100;
            player->mp = player->max_mp = 50;
            player->level               = 1;
            player->exp                 = 0;
            player->class               = game_PlayerClass_warrior;
            player->symbol              = '@';

            if_some(game_Svr_findSpawnLocation(), pos) {
                player->x = pos.first;
                player->y = pos.second;
            } else {
                player->x = 5;
                player->y = 5;
            }
        }
        self->state.player_count++;
    } block_deferral;

    /* Send player ID to client */ {
        let event = (game_Event){ .tag = game_EventTag_clt_id_asg, .payload.clt_id_asg.id = id };
        Socket_send(socket, mem_asBytes_const(&event), 0);
        printf("Player %d connected\n", id);
    }

    var event = (game_Event){};
    while (!Socket_recv(socket, mem_asBytes(&event), 0).is_err) {
        switch (event.tag) {
        case game_EventTag_move: {
            let move = event.payload.move;
            printf("Move event received: %d, %d\n", move.x, move.y);
            game_Svr_handlePlayerMove(id, move.x, move.y);
        } break;
        case game_EventTag_chat: {
            let chat = event.payload.chat;
            let msg  = Sli_from$(Sli_const$u8, chat.msg.buf, chat.msg_len);
            printf("Chat message received: %*s\n", as$(i32, msg.len), msg.ptr);
            // Broadcast chat message
            for_slice (self->state.players, (other)) {
                if (!(other->active && other->id != id)) { continue; }
                Socket_send(other->socket_fd, mem_asBytes_const(&event), 0);
            }
        } break;
        case game_EventTag_clt_disconnect: {
            goto cleanup;
        } break;
        default:
            printf("Unknown message type: %d\n", event.tag);
            continue;
        }
    }

cleanup:
    block_defer {
        Thrd_Mtx_lock(&self->state.mtx);
        defer_({
            Thrd_Mtx_unlock(&self->state.mtx);
            game_Svr_broadcastState();
        });

        Sli_at(self->state.players, id)->active = false;
        self->state.player_count--;
    } block_deferral;
    printf("Player %d disconnected\n", id);

    return_void();
} $unguarded_Thrd_fn;

$must_check
$static fn_(game_Svr_run(void), Err$void, $guard) {
    let self = game_Svr_init();
    defer_(Thrd_Mtx_fini(&self->state.mtx));

#if bti_plat_windows
    try_(Socket_initWinsock());
    defer_(Socket_cleanupWinsock());
#endif /* bti_plat_windows */

    self->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (self->socket < 0) {
        perror("Socket creation failed");
        return_err(Err_Unexpected());
    }
    defer_(close(self->socket));

    i32 opt = 1;
    setsockopt(self->socket, SOL_SOCKET, SO_REUSEADDR, as$(const char*, &opt), sizeof(opt)); // TODO: use a better value

    let server_addr = (struct sockaddr_in){
        .sin_family      = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port        = htons(game_port)
    };
    if (bind(self->socket, as$(struct sockaddr*, &server_addr), sizeOf(server_addr)) < 0) { // TODO: use a better value
        perror("Bind failed");
        return_err(Err_Unexpected());
    }
    if (listen(self->socket, game_max_players) < 0) { // TODO: use a better value
        perror("Listen failed");
        return_err(Err_Unexpected());
    }
    printf("Roguelike server started on port %d\n", game_port);

    while (true) {
        let client_addr   = (struct sockaddr_in){};
        let client_len    = sizeof(client_addr);
        let client_socket = accept(self->socket, as$(struct sockaddr*, &client_addr), as$(socklen_t*, &client_len));
        if (client_socket <= 0) { continue; }

        printf("Client connected\n");
        let client_thread = try_(Thrd_spawn(Thrd_SpawnConfig_default, Thrd_FnCtx_from(game_Svr_handleClt, { client_socket }).base));
        Thrd_detach(client_thread);
    }

    return_ok({});
} $unguarded;

/* game Client ==============================================================*/

#if !bti_plat_windows
static fn_(game_Screen_enableRawMode(void), void) {
    let self = game_Clt_inst();
    tcgetattr(STDIN_FILENO, &self->orig_termios);
    atexit(game_Screen_disableRawMode);

    var raw = self->orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
static fn_(game_Screen_disableRawMode(void), void) {
    let self = game_Clt_inst();
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &self->orig_termios);
}
#endif /* !bti_plat_windows */
static fn_(game_Screen_clear(void), void) {
    $ignore = printf("\033[2J\033[H");
    $ignore = fflush(stdout);
}
static fn_(Game_Screen_moveCursor(u32 x, u32 y), void) {
    $ignore = printf("\033[%u;%uH", y + 1, x + 1);
}

// Optimized rendering system
typedef struct game_RenderBuffer {
    var_(data, Arr$$(game_max_map_width* game_max_map_height * 32, u8));   // 32 bytes per cell for ANSI codes + char
    var_(curr_frame, Arr$$(game_max_map_width* game_max_map_height, u32)); // Hash of each cell
    var_(prev_frame, Arr$$(game_max_map_width* game_max_map_height, u32)); // Previous frame for comparison
    var_(len, usize);
    var_(needs_full_redraw, bool);
} game_RenderBuffer;
static fn_(game_RenderBuffer_init(void), game_RenderBuffer*) {
    static game_RenderBuffer buffer = { .needs_full_redraw = true };
    return &buffer;
}
static fn_(game_RenderBuffer_clear(game_RenderBuffer* self), void) {
    self->len = 0;
}
static fn_(game_RenderBuffer_append(game_RenderBuffer* self, Sli_const$u8 data), void) {
    let remaining = Arr_len(self->data) - self->len;
    let copy_len  = data.len < remaining ? data.len : remaining;
    if (0 < copy_len) {
        mem_copy(self->data.buf + self->len, data.ptr, copy_len);
        self->len += copy_len;
    }
}
static fn_(game_RenderBuffer_appendChar(game_RenderBuffer* self, u8 c), void) {
    if (self->len < Arr_len(self->data)) {
        self->data.buf[self->len++] = c;
    }
}
static fn_(game_RenderBuffer_flush(game_RenderBuffer* self), void) {
    if (0 < self->len) {
        $ignore = fwrite(self->data.buf, 1, self->len, stdout);
        $ignore = fflush(stdout);
    }
}
// Simple hash function for cell content
static fn_(game_RenderBuffer_cellHash(u8 tile, u8 player_char, i32 player_id), u32) {
    return (as$(u32, tile) << 24) | (as$(u32, player_char) << 16) | (as$(u32, player_id) & 0xFFFF);
}

typedef struct game_Clt {
    Socket     socket;
    game_State state;
    i32        player_id;
    char       server_ip[game_max_ip_addr_len];
#if !bti_plat_windows
    struct termios orig_termios;
#endif /* !bti_plat_windows */
    // Performance tracking
    u64 last_render_time;
    u64 frame_count;
    f64 avg_frame_time_ms;
} game_Clt;
static fn_(game_Clt_inst(void), game_Clt*) {
    static game_Clt inst = {
        .player_id         = -1,
        .server_ip         = "127.0.0.1",
        .last_render_time  = 0,
        .frame_count       = 0,
        .avg_frame_time_ms = 0.0
    };
    return &inst;
}

// Player color highlight for own player
#define game_Clt_ansi_attr_self_highlight "\033[1;97;41m" // Bold white on red background
// Add color assignment function
static fn_(game_Clt_getPlayerColor(i32 player_id, bool is_self), Sli_const$u8) {
    if (is_self) { return u8_l(game_Clt_ansi_attr_self_highlight); }
    static let colors = (Arr$$(8, Sli_const$u8)){ {
        u8_l(ansi_attr_fg_bright_black),
        u8_l(ansi_attr_fg_bright_red),
        u8_l(ansi_attr_fg_bright_green),
        u8_l(ansi_attr_fg_bright_yellow),
        u8_l(ansi_attr_fg_bright_blue),
        u8_l(ansi_attr_fg_bright_magenta),
        u8_l(ansi_attr_fg_bright_cyan),
        u8_l(ansi_attr_fg_bright_white),
    } };
    return Arr_getAt(colors, player_id % Arr_len(colors));
}

// Optimized rendering function
$maybe_unused
$static fn_(game_Clt_drawGameOptimized(void), void) {
    let self       = game_Clt_inst();
    let start_time = time_Instant_now();
    let buffer     = game_RenderBuffer_init();

    // Pre-calculate player positions for fast lookup
    var player_map = (Arr$$(game_max_map_width * game_max_map_height, i32)){};
    for (i32 i = 0; i < game_max_map_width * game_max_map_height; ++i) {
        player_map.buf[i] = -1; // Initialize to "no player"
    }

    for_slice (self->state.players, (player)) {
        if (!player->active) { continue; }
        if (player->x < 0 || player->x >= game_max_map_width || player->y < 0 || player->y >= game_max_map_height) { continue; }
        let idx             = player->y * game_max_map_width + player->x;
        player_map.buf[idx] = player->id;
    }

    // Calculate current frame state (much faster now)
    for (i32 y = 0; y < game_max_map_height; ++y) {
        for (i32 x = 0; x < game_max_map_width; ++x) {
            let tile_idx                     = y * game_max_map_width + x;
            let tile                         = Grid_getAt(self->state.map, x, y);
            let player_id                    = player_map.buf[tile_idx];
            u8  player_char                  = (player_id >= 0) ? '@' : 0;
            buffer->curr_frame.buf[tile_idx] = game_RenderBuffer_cellHash(tile, player_char, player_id);
        }
    }

    // Check if we need any redraw
    var has_changes = buffer->needs_full_redraw;
    if (!has_changes) {
        for (i32 i = 0; i < game_max_map_width * game_max_map_height; ++i) {
            if (buffer->curr_frame.buf[i] != buffer->prev_frame.buf[i]) {
                has_changes = true;
                break;
            }
        }
    }

    if (has_changes) {
        if (buffer->needs_full_redraw) {
            game_Screen_clear();
            buffer->needs_full_redraw = false;
        }

        game_RenderBuffer_clear(buffer);

        // Use a MUCH more efficient approach: build entire screen as plain text first
        // Then add minimal ANSI codes only where needed

        // Position cursor at top-left
        game_RenderBuffer_append(buffer, u8_l("\033[1;1H"));

        // Build map content without ANSI codes for speed
        for (i32 y = 0; y < game_max_map_height; ++y) {
            for (i32 x = 0; x < game_max_map_width; ++x) {
                let player_id = player_map.buf[y * game_max_map_width + x];

                if (player_id >= 0) {
                    // Player at this position
                    let player  = Sli_at(self->state.players, player_id);
                    let is_self = (player->id == self->player_id);

                    if (is_self) {
                        // Highlight self player
                        game_RenderBuffer_append(buffer, u8_l(game_Clt_ansi_attr_self_highlight "@" ansi_attr_reset));
                    } else {
                        // Other player with simple color
                        static let player_colors = (Arr$$(4, Sli_const$u8)){
                            u8_l(ansi_attr_fg_bright_red "@" ansi_attr_reset),
                            u8_l(ansi_attr_fg_bright_green "@" ansi_attr_reset),
                            u8_l(ansi_attr_fg_bright_blue "@" ansi_attr_reset),
                            u8_l(ansi_attr_fg_bright_yellow "@" ansi_attr_reset)
                        };
                        let color_seq = Arr_getAt(player_colors, player_id % Arr_len(player_colors));
                        game_RenderBuffer_append(buffer, color_seq);
                    }
                } else {
                    // Just the tile
                    let tile = Grid_getAt(self->state.map, x, y);
                    game_RenderBuffer_appendChar(buffer, tile);
                }
            }

            // Add line break except for last row
            if (y < game_max_map_height - 1) {
                game_RenderBuffer_appendChar(buffer, '\n');
            }
        }

        // Update previous frame
        mem_copy(buffer->prev_frame.buf, buffer->curr_frame.buf, sizeof(buffer->curr_frame.buf[0]) * Arr_len(buffer->curr_frame));
    }

    // Draw UI (simplified)
    game_RenderBuffer_append(buffer, u8_l("\033[22;1H\n=== PLAYER INFO ===\n"));

    for_slice (self->state.players, (player)) {
        if (!player->active) { continue; }

        let is_self = (player->id == self->player_id);
        if (is_self) {
            game_RenderBuffer_append(buffer, u8_l(">>> " game_Clt_ansi_attr_self_highlight));
        } else {
            game_RenderBuffer_append(buffer, u8_l("    "));
        }

        // Simple player info without complex formatting
        var player_info = (Arr$$(128, u8)){};
        let info_len    = as$(usize, snprintf(as$(char*, player_info.buf), Arr_len(player_info), "%s Lv.%d HP:%d/%d Pos:(%d,%d)", player->name, player->level, player->hp, player->max_hp, player->x, player->y));
        game_RenderBuffer_append(buffer, Sli_from$(Sli_const$u8, player_info.buf, info_len));

        if (is_self) {
            game_RenderBuffer_append(buffer, u8_l(ansi_attr_reset));
        }
        game_RenderBuffer_appendChar(buffer, '\n');
    }

#ifdef FAST_RENDER
    // Simplified performance info
    var perf_info = (Arr$$(64, u8)){};
    let perf_len  = as$(usize, snprintf(as$(char*, perf_info.buf), Arr_len(perf_info), "\n[FAST] %.1fms | %llu frames\n", self->avg_frame_time_ms, as$(unsigned long long, self->frame_count)));
    game_RenderBuffer_append(buffer, Sli_from$(Sli_const$u8, perf_info.buf, perf_len));
#endif

    game_RenderBuffer_append(buffer, u8_l("WASD=move, Q=quit\n"));

    // Flush everything at once
    game_RenderBuffer_flush(buffer);

    // Update performance metrics
    let end_time       = time_Instant_now();
    let frame_duration = time_Instant_durationSince(end_time, start_time);
    let frame_time_ms  = time_Duration_asSecs_f64(frame_duration) * 1000.0;

    self->frame_count++;
    // Simple moving average
    self->avg_frame_time_ms = (self->avg_frame_time_ms * 0.9) + (frame_time_ms * 0.1);
}
// Legacy rendering function (kept for compatibility with NO_GRID)
static fn_(game_Clt_drawGame(void), void) {
    let self = game_Clt_inst();

#if defined(FAST_RENDER) && !defined(NO_GRID)
    game_Clt_drawGameOptimized();
    return;
#endif // FAST_RENDER && !NO_GRID
#ifndef FAST_RENDER
    let start_time = time_Instant_now();
    game_Screen_clear();
#endif /* FAST_RENDER */
#if NO_GRID
    // Draw map
    for (i32 y = 0; y < game_max_map_height; ++y) {
        for (i32 x = 0; x < game_max_map_width; ++x) {
            var c            = Grid_getAt(self->state.map, x, y);
            var player_found = false;

            // Check if there's a player at this position
            for_slice (self->state.players, (player)) {
                if (!player->active) { continue; }
                if (!(player->x == x && player->y == y)) { continue; }

                let is_self  = (player->id == self->player_id);
                let color    = game_getPlayerColor(player->id, is_self);
                $ignore      = printf("%*s%c" ansi_attr_reset, as$(i32, color.len), color.ptr, player->symbol);
                player_found = true;
                break;
            }

            if (!player_found) {
                $ignore = printf("%c", c);
            }
        }
        $ignore = printf("\n");
    }
#else
    // Draw map
    Game_Screen_moveCursor(0, 0);
    for_grid(self->state.map, (tile, (x, y)), {
        let color = game_TileType_getColor(*tile);
        Game_Screen_moveCursor(x, y);
        $ignore = printf("%*s%c" ansi_attr_reset, as$(i32, color.len), color.ptr, *tile);
    });

    // Draw players
    Game_Screen_moveCursor(0, 0);
    for_slice (self->state.players, (player)) {
        if (!player->active) { continue; }
        Game_Screen_moveCursor(player->x, player->y);
        let is_self = (player->id == self->player_id);
        let color   = game_Clt_getPlayerColor(player->id, is_self);
        $ignore     = printf("%*s%c" ansi_attr_reset, as$(i32, color.len), color.ptr, player->symbol);
    }
#endif // NO_GRID

    // Draw UI
    Game_Screen_moveCursor(0, self->state.map.height + 1);
    $ignore = printf("\n=== PLAYER INFO ===\n");
    for_slice (self->state.players, (player)) {
        if (!player->active) { continue; }

        let is_self     = (player->id == self->player_id);
        let color       = game_Clt_getPlayerColor(player->id, is_self);
        let name_prefix = is_self ? ">>> " : "    ";

        $ignore = printf("%s%*s%s [Lv.%d] HP:%d/%d MP:%d/%d Exp:%d" ansi_attr_reset "\n", name_prefix, as$(i32, color.len), color.ptr, player->name, player->level, player->hp, player->max_hp, player->mp, player->max_mp, player->exp);
    }

#ifndef FAST_RENDER
    // Add performance information for legacy renderer
    let end_time       = time_Instant_now();
    let frame_duration = time_Instant_durationSince(end_time, start_time);
    let frame_time_ms  = time_Duration_asSecs_f64(frame_duration) * 1000.0;

    self->frame_count++;
    self->avg_frame_time_ms = (self->avg_frame_time_ms * 0.9) + (frame_time_ms * 0.1);

    $ignore = printf("\n[LEGACY_RENDER] Avg frame time: %.2f ms | Frames: %llu\n", self->avg_frame_time_ms, as$(unsigned long long, self->frame_count));
#endif

    $ignore = printf("Controls: WASD to move, Q to quit\n");
    $ignore = printf("Your player: " game_Clt_ansi_attr_self_highlight "@" ansi_attr_reset "\n");
}

static Thrd_fn_(game_Clt_receiveEvents, ({}, Void), ($ignore_capture, $ignore_capture), $scope) {
    let self  = game_Clt_inst();
    var event = (game_Event){};
    while (!Socket_recv(self->socket, mem_asBytes(&event), 0).is_err) {
        switch (event.tag) {
        case game_EventTag_game_state: {
            catch_from(Socket_recv(self->socket, mem_asBytes(&self->state), 0), err, ({
                printf("Error receiving game state: %s\n", Err_codeToCStr(err));
                continue;
            }));
            self->state.players = Sli_arr$(Sli$game_Player, self->state.mem.players);
            self->state.map     = Grid_fromSli$(Grid$u8, Sli_arr$(Sli$u8, self->state.mem.map), game_max_map_width, game_max_map_height);
            game_Clt_drawGame();
        } break;
        case game_EventTag_clt_id_asg: {
            let clt_id_asg  = event.payload.clt_id_asg;
            self->player_id = clt_id_asg.id;
            printf("Assigned player ID: %d\n", self->player_id);
        } break;
        case game_EventTag_chat: {
            let chat = event.payload.chat;
            let msg  = Sli_from$(Sli_const$u8, chat.msg.buf, chat.msg_len);
            printf("\n[CHAT] %*s\n", as$(i32, msg.len), msg.ptr);
        } break;
        default:
            continue;
        }
    }
    return_void();
} $unscoped_Thrd_fn;
$must_check
$static fn_(game_Clt_findMyPlayer(void), Opt$Ptr$game_Player, $scope) {
    let self = game_Clt_inst();

    // If we don't have a player ID assigned yet, return none
    if (self->player_id < 0) { return_none(); }

    for_slice (self->state.players, (player)) {
        if (!player->active) { continue; }
        if (player->id != self->player_id) { continue; } // Use player ID instead of socket
        return_some(player);
    }
    return_none();
} $unscoped;

typedef struct game_CliOptions {
    Sli_const$u8 server_ip;
    i32          server_port;
    bool         is_dummy_ai;
} game_CliOptions;
$must_check
$static fn_(game_Clt_runWithOptions(game_CliOptions options), Err$void);
$must_check
$static fn_(game_Clt_runPlayer(Sli_const$u8 server_ip), Err$void, $scope) {
    return_ok(try_(game_Clt_runWithOptions((game_CliOptions){
        .server_ip = server_ip,
        .server_port = game_port,
        .is_dummy_ai = false
    })));
} $unscoped;
$must_check
$static fn_(game_Clt_runDummyAI(game_CliOptions options), Err$void, $scope) {
    printf("[DUMMY] Connecting to %*s:%d as AI player...\n", as$(i32, options.server_ip.len), options.server_ip.ptr, options.server_port);
    return_ok(catch_from(game_Clt_runWithOptions(options), err, eval({
        printf("[DUMMY] Failed to connect as AI player: %s\n", Err_codeToCStr(err));
        return_err(err);
    })));
} $unscoped;
fn_(game_Clt_runWithOptions(game_CliOptions options), Err$void, $guard) {
    let self = game_Clt_inst();

    // Copy IP address to client instance
    if (0 < options.server_ip.len && options.server_ip.len < game_max_ip_addr_len) {
        mem_copy(self->server_ip, options.server_ip.ptr, options.server_ip.len);
        self->server_ip[options.server_ip.len] = '\0';
    }

#if bti_plat_windows
    try_(Socket_initWinsock());
    defer_(Socket_cleanupWinsock());
#else  /* !bti_plat_windows */
    if (!options.is_dummy_player) {
        game_Screen_enableRawMode();
    }
#endif /* bti_plat_windows */

    self->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (self->socket < 0) {
        perror("Socket creation failed");
        return_err(Err_Unexpected());
    }
    defer_(close(self->socket));

    let server_addr = (struct sockaddr_in){
        .sin_family      = AF_INET,
        .sin_port        = htons(options.server_port),
        .sin_addr.s_addr = inet_addr(self->server_ip)
    };
    if (connect(self->socket, as$(struct sockaddr*, &server_addr), sizeOf(server_addr)) < 0) {
        perror("Connection failed");
        return_err(Err_Unexpected());
    }

    if (options.is_dummy_ai) {
        printf("[DUMMY] Connected to roguelike server at %s:%d!\n", self->server_ip, options.server_port);
    } else {
        printf("Connected to roguelike server at %s:%d!\n", self->server_ip, options.server_port);
    }

    let receive_thread = try_(Thrd_spawn(Thrd_SpawnConfig_default, Thrd_FnCtx_from(game_Clt_receiveEvents, {}).base));
    defer_(Thrd_detach(receive_thread));

    if (options.is_dummy_ai) {
        // AI dummy player loop
        Random_init();
        printf("[DUMMY] Starting AI behavior...\n");

        while (true) {
            if (engine_utils_kbhit()) {
                let input = as$(u8, engine_utils_getch());
                if (input == 'q' || input == 'Q') {
                    let event = (game_Event){
                        .tag                       = game_EventTag_clt_disconnect,
                        .payload.clt_disconnect.id = self->player_id,
                    };
                    Socket_send(self->socket, mem_asBytes_const(&event), 0);
                    break;
                }
            }
            // Wait for random interval (50-500ms)
            let wait_ms = 50 + Random_range_i64(0, 450);
            time_sleep(time_Duration_fromMillis(wait_ms));

            // Find my player
            let my_player = orelse(game_Clt_findMyPlayer(), eval({
                continue; // Keep waiting for player assignment
            }));
            // Perform random action
            let action = Random_range_i64(0, 5);
            var new_x  = my_player->x;
            var new_y  = my_player->y;

            // clang-format off
            switch (action) {
            case 0: { new_y--; } break; // Move up
            case 1: { new_y++; } break; // Move down
            case 2: { new_x--; } break; // Move left
            case 3: { new_x++; } break; // Move right
            case 4: $fallthrough;
            default: continue;
            }
            // clang-format on

            printf("[DUMMY] AI moving from (%d,%d) to (%d,%d)\n", my_player->x, my_player->y, new_x, new_y);

            let event = (game_Event){
                .tag             = game_EventTag_move,
                .payload.move.id = self->player_id,
                .payload.move.x  = new_x,
                .payload.move.y  = new_y,
            };
            Socket_send(self->socket, mem_asBytes_const(&event), 0);
        }
    } else {
        // Human player input loop
        while (true) {
            let input = as$(u8, engine_utils_getch());
            if (input == 'q' || input == 'Q') {
                let event = (game_Event){
                    .tag                       = game_EventTag_clt_disconnect,
                    .payload.clt_disconnect.id = self->player_id,
                };
                Socket_send(self->socket, mem_asBytes_const(&event), 0);
                break;
            }

            // Find my player
            let my_player = orelse(game_Clt_findMyPlayer(), eval({ continue; }));
            var new_x     = my_player->x;
            var new_y     = my_player->y;

            // clang-format off
            switch (input) {
            case 'w': case 'W': { new_y--; } break;
            case 's': case 'S': { new_y++; } break;
            case 'a': case 'A': { new_x--; } break;
            case 'd': case 'D': { new_x++; } break;
            default: continue;
            }
            // clang-format on

            let event = (game_Event){
                .tag             = game_EventTag_move,
                .payload.move.id = self->player_id,
                .payload.move.x  = new_x,
                .payload.move.y  = new_y,
            };
            Socket_send(self->socket, mem_asBytes_const(&event), 0);
        }
    }

    if (options.is_dummy_ai) {
        printf("[DUMMY] Disconnected from server\n");
    } else {
        printf("Disconnected from server\n");
    }
    return_ok({});
} $unguarded;

/* game Localization ========================================================*/

typedef enum game_Lang {
    game_Lang_en = 0,
    game_Lang_ko = 1,
    game_Lang_count
} game_Lang;
static var_(game_Lang_current, game_Lang) = game_Lang_ko;
$maybe_unused
$static fn_(game_Lang_displayList(void), void) {
    printf("Language:\n");
    printf("1. English\n");
    printf("2. Korean\n");
}

/* game Ip Menu =============================================================*/

static fn_(game_IpMenu_displayTitle(void), void);
static fn_(game_IpMenu_displayOptions(void), void);
typedef enum game_IpChoice {
    game_IpChoice_invalid  = -1,
    game_IpChoice_local    = 0,
    game_IpChoice_external = 1,
    game_IpChoice_back     = 2,
    game_IpChoice_count
} game_IpChoice;
static fn_(game_IpMenu_getUserChoice(void), game_IpChoice);
static fn_(game_IpMenu_getLocalIp(void), Sli_const$u8);
static fn_(game_IpMenu_displayPrompt(void), void);
use_Err$(Sli_const$u8);
$must_check
$static fn_(game_IpMenu_getExternalIp(void), Err$Sli_const$u8);
$must_check
$static fn_(game_IpMenu_run(void), Err$Opt$Sli_const$u8, $scope) {
    while (true) {
        game_IpMenu_displayTitle();
        game_IpMenu_displayOptions();
        switch (game_IpMenu_getUserChoice()) {
        case game_IpChoice_local: {
            return_ok(some(game_IpMenu_getLocalIp()));
        } break;
        case game_IpChoice_external: {
            game_IpMenu_displayPrompt();
            return_ok(some(catch_(game_IpMenu_getExternalIp(), eval({ continue; }))));
        } break;
        case game_IpChoice_back: {
            return_ok(none());
        } break;
        case game_IpChoice_invalid:
        default:
            continue;
        }
    }
} $unscoped;

static const Arr$$(game_Lang_count, Sli_const$u8) game_IpMenu_table_lang_title = Arr_init({
    [game_Lang_en] = u8_l("SELECT SERVER IP"),
    [game_Lang_ko] = u8_l("서버 IP 선택"),
});
fn_(game_IpMenu_displayTitle(void), void) {
    let title = Arr_getAt(game_IpMenu_table_lang_title, game_Lang_current);
    printf("=== %*s ===\n", as$(i32, title.len), title.ptr);
}
static const Arr$$(game_Lang_count, Arr$$(game_IpChoice_count, Sli_const$u8)) game_IpChoice_table_lang_opts = Arr_init({
    [game_Lang_en] = Arr_init({
        [game_IpChoice_local]    = u8_l("Local (127.0.0.1)"),
        [game_IpChoice_external] = u8_l("External IP"),
        [game_IpChoice_back]     = u8_l("Back"),
    }),
    [game_Lang_ko] = Arr_init({
        [game_IpChoice_local]    = u8_l("로컬 (127.0.0.1)"),
        [game_IpChoice_external] = u8_l("외부 IP"),
        [game_IpChoice_back]     = u8_l("뒤로"),
    }),
});
fn_(game_IpMenu_displayOptions(void), void) {
    let table_lang = game_IpChoice_table_lang_opts;
    let opts       = Arr_getAt(table_lang, game_Lang_current);
    for_array_indexed (opts, (opt), (idx)) {
        printf("%zu. %*s\n", idx + 1, as$(i32, opt->len), opt->ptr);
    }
}
fn_(game_IpMenu_getUserChoice(void), game_IpChoice) {
    let input = as$(u8, getchar());
    // Clear input buffer
    if (input != '\n') {
        while (getchar() != '\n') { /* consume rest of line */
        }
    }
    switch (input) {
        case_return('1') game_IpChoice_local;
        case_return('2') game_IpChoice_external;
        case_return('3') game_IpChoice_back;
        default_return game_IpChoice_invalid;
    }
}
static const Arr$$(game_Lang_count, Sli_const$u8) game_IpMenu_table_lang_prompt = Arr_init({
    [game_Lang_en] = u8_l("Enter server IP address: "),
    [game_Lang_ko] = u8_l("서버 IP 주소를 입력하세요: "),
});
fn_(game_IpMenu_displayPrompt(void), void) {
    let prompt_msg = Arr_getAt(game_IpMenu_table_lang_prompt, game_Lang_current);
    printf("%*s", as$(i32, prompt_msg.len), prompt_msg.ptr);
}
fn_(game_IpMenu_getLocalIp(void), Sli_const$u8) {
    static let local_ip = u8_l("127.0.0.1");
    return local_ip;
}
fn_(game_IpMenu_getExternalIp(void), Err$Sli_const$u8, $scope) {
    static Arr$$(game_max_ip_addr_len, u8) ip_buffer = {};

    $ignore = fflush(stdout);
    if (!fgets(as$(char*, ip_buffer.buf), game_max_ip_addr_len, stdin)) {
        return_err(Err_Unexpected());
    }

    var ip_view = Str_viewZ(as$(u8*, ip_buffer.buf));
    if (ip_view.len == 0) { return_err(Err_Unexpected()); }
    return_ok(ip_view);
} $unscoped;

/* game Main Menu ===========================================================*/

static fn_(game_MainMenu_displayTitle(void), void);
static fn_(game_MainMenu_displayOptions(void), void);
typedef enum game_MainMenuChoice {
    game_MainMenuChoice_invalid     = -1,
    game_MainMenuChoice_host_server = 0,
    game_MainMenuChoice_join_server = 1,
    game_MainMenuChoice_exit        = 2,
    game_MainMenuChoice_count
} game_MainMenuChoice;
static fn_(game_MainMenu_getUserChoice(void), game_MainMenuChoice);
$must_check
$static fn_(game_MainMenu_handleHostServer(void), Err$void);
$must_check
$static fn_(game_MainMenu_handleJoinServer(void), Err$void);
$must_check
$static fn_(game_MainMenu_run(void), Err$void, $scope) {
    while (true) {
        game_MainMenu_displayTitle();
        game_MainMenu_displayOptions();
        switch (game_MainMenu_getUserChoice()) {
        case game_MainMenuChoice_host_server: {
            try_(game_MainMenu_handleHostServer());
        } break;
        case game_MainMenuChoice_join_server: {
            try_(game_MainMenu_handleJoinServer());
        } break;
        case game_MainMenuChoice_exit: {
            printf("Goodbye!\n");
            return_ok({});
        } break;
        case game_MainMenuChoice_invalid:
            $fallthrough;
        default:
            continue;
        }
    }
    return_ok({});
} $unscoped;

static const Arr$$(game_Lang_count, Sli_const$u8) game_MainMenu_table_lang_title = Arr_init({
    [game_Lang_en] = u8_l("ROGUELIKE MULTIPLAYER"),
    [game_Lang_ko] = u8_l("로그라이크 멀티플레이"),
});
fn_(game_MainMenu_displayTitle(void), void) {
    let title = Arr_getAt(game_MainMenu_table_lang_title, game_Lang_current);
    printf("=== %*s ===\n", as$(i32, title.len), title.ptr);
}
static const Arr$$(game_Lang_count, Arr$$(game_MainMenuChoice_count, Sli_const$u8)) game_MainMenuChoice_table_lang_msgs = Arr_init({
    [game_Lang_en] = Arr_init({
        [game_MainMenuChoice_host_server] = u8_l("Host Server"),
        [game_MainMenuChoice_join_server] = u8_l("Join Server"),
        [game_MainMenuChoice_exit]        = u8_l("Exit"),
    }),
    [game_Lang_ko] = Arr_init({
        [game_MainMenuChoice_host_server] = u8_l("서버 호스트"),
        [game_MainMenuChoice_join_server] = u8_l("서버 참여"),
        [game_MainMenuChoice_exit]        = u8_l("종료"),
    }),
});
fn_(game_MainMenu_displayOptions(void), void) {
    let table     = game_MainMenuChoice_table_lang_msgs;
    let lang_msgs = Arr_getAt(table, game_Lang_current);
    for_array_indexed (lang_msgs, (msg), (idx)) {
        printf("%zu. %*s\n", idx + 1, as$(i32, msg->len), msg->ptr);
    }
}
fn_(game_MainMenu_getUserChoice(void), game_MainMenuChoice) {
    let input = as$(u8, getchar());
    // Clear input buffer
    if (input != '\n') {
        while (getchar() != '\n') { /* consume rest of line */
        }
    }
    switch (input) {
        case_return('1') game_MainMenuChoice_host_server;
        case_return('2') game_MainMenuChoice_join_server;
        case_return('3') game_MainMenuChoice_exit;
        default_return game_MainMenuChoice_invalid;
    }
}
static const Arr$$(game_Lang_count, Arr$$(2, Sli_const$u8)) game_MainMenu_host_server_table_lang_msgs = Arr_init({
    [game_Lang_en] = Arr_init({
        u8_l("Starting server on port %d...\n"),
        u8_l("Press Ctrl+C to stop the server.\n"),
    }),
    [game_Lang_ko] = Arr_init({
        u8_l("포트 %d에서 서버를 시작하는 중...\n"),
        u8_l("Ctrl+C를 눌러 서버를 종료할 수 있습니다.\n"),
    }),
});
fn_(game_MainMenu_handleHostServer(void), Err$void, $scope) {
    let table = game_MainMenu_host_server_table_lang_msgs;
    let msgs  = Arr_getAt(table, game_Lang_current);
    for_array (msgs, (msg)) {
        printf("%*s", as$(i32, msg->len), msg->ptr);
    }
    return_ok(try_(game_Svr_run()));
} $unscoped;
static const Arr$$(game_Lang_count, Arr$$(1, Sli_const$u8)) game_MainMenu_join_server_table_lang_msgs = Arr_init({
    [game_Lang_en] = Arr_init({
        u8_l("Connecting to server at %*s:%d...\n"),
    }),
    [game_Lang_ko] = Arr_init({
        u8_l("서버 %*s:%d에 연결하는 중...\n"),
    }),
});
fn_(game_MainMenu_handleJoinServer(void), Err$void, $scope) {
    while (true) {
        let selected_ip = try_(game_IpMenu_run());
        if_some(selected_ip, ip) {
            let table = game_MainMenu_join_server_table_lang_msgs;
            let msgs  = Arr_getAt(table, game_Lang_current);
            for_array (msgs, (msg)) {
                printf("%*s", as$(i32, msg->len), msg->ptr);
            }
            return_ok(catch_(game_Clt_runPlayer(ip), eval({ continue; })));
        }
        return_ok({});
    }
} $unscoped;

/* game exec ================================================================*/

static fn_(game_exec_printCliCmdHelp(Sli_const$u8 cmd), void);
static fn_(game_exec_parseSvrAddr(Sli_const$u8 addr_str), game_CliOptions);
static fn_(game_exec_printCliUnknownFlag(Sli_const$u8 flag), void);
fn_(dh_main(Sli$Str_const args), Err$void, $scope) {
    // Handle command line arguments first
    if (1 < args.len) {
        for_slice_indexed (args, (arg), (idx)) {
            if (idx == 0) { continue; } // Skip program name

            let flag_begin = orelse(Str_startsWith(*arg, u8_l("--")), eval({ continue; }));
            let flag       = Str_slice(*arg, flag_begin, arg->len);

            if (Str_eqlNoCase(flag, u8_l("server"))) { return_ok(try_(game_Svr_run())); }
            if (Str_eqlNoCase(flag, u8_l("client"))) {
                // Check if next argument is an IP address
                let next_idx = idx + 1;
                var options  = (game_CliOptions){
                     .server_ip   = u8_l("127.0.0.1"),
                     .server_port = game_port,
                     .is_dummy_ai = false
                };

                if (next_idx < args.len) {
                    let next_arg = Sli_getAt(args, next_idx);
                    // If next argument doesn't start with --, treat it as IP address
                    if_none(Str_startsWith(next_arg, u8_l("--"))) {
                        options = game_exec_parseSvrAddr(next_arg);
                    }
                }
                return_ok(try_(game_Clt_runWithOptions(options)));
            }
            if (Str_eqlNoCase(flag, u8_l("dummy-ai"))) {
                // Check if next argument is an IP address
                let next_idx = idx + 1;
                var options  = (game_CliOptions){
                     .server_ip   = u8_l("127.0.0.1"),
                     .server_port = game_port,
                     .is_dummy_ai = true
                };

                if (next_idx < args.len) {
                    let next_arg = Sli_getAt(args, next_idx);
                    // If next argument doesn't start with --, treat it as IP address
                    if_none(Str_startsWith(next_arg, u8_l("--"))) {
                        options             = game_exec_parseSvrAddr(next_arg);
                        options.is_dummy_ai = true; // Ensure dummy flag is set
                    }
                }
                return_ok(try_(game_Clt_runDummyAI(options)));
            }
            if (Str_eqlNoCase(flag, u8_l("help"))) {
                game_exec_printCliCmdHelp(Sli_getAt(args, 0));
                return_ok({});
            }
            game_exec_printCliUnknownFlag(flag);
            game_exec_printCliCmdHelp(Sli_getAt(args, 0));
            return_err(Err_Unexpected());
        }
    }

    // If no command line arguments, show interactive main menu
    return_ok(try_(game_MainMenu_run()));
} $unscoped;

fn_(game_exec_printCliCmdHelp(Sli_const$u8 cmd), void) {
    printf("Usage: %*s [OPTIONS]\n", as$(i32, cmd.len), cmd.ptr);
    printf("Options:\n");
    printf("  --help                      Show this help message\n");
    printf("  --server                    Start the game server\n");
    printf("  --client [IP[:PORT]]        Connect to server (default: 127.0.0.1:8080)\n");
    printf("  --dummy-ai [IP[:PORT]]  Connect as AI dummy player for debugging\n");
    printf("\nExamples:\n");
    printf("  %*s --server\n", as$(i32, cmd.len), cmd.ptr);
    printf("  %*s --client\n", as$(i32, cmd.len), cmd.ptr);
    printf("  %*s --client 192.168.1.100\n", as$(i32, cmd.len), cmd.ptr);
    printf("  %*s --client 192.168.1.100:9090\n", as$(i32, cmd.len), cmd.ptr);
    printf("  %*s --dummy-ai\n", as$(i32, cmd.len), cmd.ptr);
}
fn_(game_exec_parseSvrAddr(Sli_const$u8 addr_str), game_CliOptions) {
    var options = (game_CliOptions){
        .server_ip   = u8_l("127.0.0.1"),
        .server_port = game_port,
        .is_dummy_ai = false
    };

    if (addr_str.len == 0) {
        return options; // Use defaults
    }

    // Look for port separator ':'
    var colon_pos = -1;
    for_slice_indexed (addr_str, (ch), (idx)) {
        if (*ch == ':') {
            colon_pos = as$(i32, idx);
            break;
        }
    }

    if (colon_pos >= 0) {
        // Split IP and port
        options.server_ip = Str_slice(addr_str, 0, colon_pos);
        let port_str      = Str_slice(addr_str, colon_pos + 1, addr_str.len);

        // Parse port
        var port_val = 0;
        for_slice (port_str, (ch)) {
            if (*ch < '0' || *ch > '9') {
                // Invalid port, use default
                options.server_port = game_port;
                break;
            }
            port_val = port_val * 10 + (*ch - '0');
        }
        if (port_val > 0 && port_val <= 65535) {
            options.server_port = port_val;
        }
    } else {
        // Just IP, no port
        options.server_ip = addr_str;
    }

    return options;
}
fn_(game_exec_printCliUnknownFlag(Sli_const$u8 flag), void) {
    printf("Unknown flag: %*s\n", as$(i32, flag.len), flag.ptr);
}
