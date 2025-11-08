#include "dh/main.h"
#include "dh/heap/Page.h"
#include "dh/opt.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>

// MPMC Channel node structure (internal)
#define sync_mpmc_Node$(T) pp_join($, sync_mpmc_Node, T)
#define sync_mpmc_Node$$(T) \
    struct sync_mpmc_Node$(T) { \
        T                                       data; \
        _Atomic(P$$(struct sync_mpmc_Node$(T))) next; \
    }
#define sync_mpmc_Node_useT$(T) typedef sync_mpmc_Node$$(T) sync_mpmc_Node$(T)

// Internal MPMC channel structure
#define sync_mpmc_Channel$(T) pp_join($, sync_mpmc_Channel, T)
#define sync_mpmc_Channel$$(T) \
    struct sync_mpmc_Channel$(T) { \
        _Atomic(P$$(sync_mpmc_Node$(T))) head; \
        _Atomic(P$$(sync_mpmc_Node$(T))) tail; \
        _Atomic(bool)                    disconnected; \
        pthread_mutex_t                  mutex; \
        pthread_cond_t                   recv_cond; \
        pthread_cond_t                   send_cond; \
    }
#define sync_mpmc_Channel_useT$(T) typedef sync_mpmc_Channel$$(T) sync_mpmc_Channel$(T)

// Reference counter structure
#define sync_mpmc_Counter$(T) pp_join($, sync_mpmc_Counter, T)
#define sync_mpmc_Counter$$(T) \
    struct sync_mpmc_Counter$(T) { \
        _Atomic(usize) senders; \
        _Atomic(usize) receivers; \
        _Atomic(bool)  destroy; \
        P$$(sync_mpmc_Channel$(T)) \
        chan; \
    }
#define sync_mpmc_Counter_useT$(T) typedef sync_mpmc_Counter$$(T) sync_mpmc_Counter$(T)

// Sender type (reference-counted wrapper)
#define sync_mpsc_Sender$(T) pp_join($, sync_mpsc_Sender, T)
#define sync_mpsc_Sender$$(T) \
    struct sync_mpsc_Sender$(T) { \
        mem_Allocator allocator; \
        P$$(sync_mpmc_Counter$(T)) \
        counter; \
    }
#define sync_mpsc_Sender_useT$(T) typedef sync_mpsc_Sender$$(T) sync_mpsc_Sender$(T)

// Receiver type (reference-counted wrapper)
#define sync_mpsc_Receiver$(T) pp_join($, sync_mpsc_Receiver, T)
#define sync_mpsc_Receiver$$(T) \
    struct sync_mpsc_Receiver$(T) { \
        mem_Allocator allocator; \
        P$$(sync_mpmc_Counter$(T)) \
        counter; \
    }
#define sync_mpsc_Receiver_useT$(T) typedef sync_mpsc_Receiver$$(T) sync_mpsc_Receiver$(T)

// Channel pair type
#define sync_mpsc_Channel$(T) pp_join($, sync_mpsc_Channel, T)
#define sync_mpsc_Channel$$(T) \
    struct sync_mpsc_Channel$(T) { \
        sync_mpsc_Sender$(T) sender; \
        sync_mpsc_Receiver$(T) receiver; \
    }
#define sync_mpsc_Channel_useT$(T) typedef sync_mpsc_Channel$$(T) sync_mpsc_Channel$(T)

// Send result enum
typedef enum {
    SYNC_MPSC_SEND_OK,
    SYNC_MPSC_SEND_DISCONNECTED
} sync_mpsc_SendResult;

// MPMC channel internal functions
#define sync_mpmc_Channel_send$(T)                           pp_join($, sync_mpmc_Channel_send, T)
#define sync_mpmc_Channel_send$$(T, _allocator, chan, value) ({ \
    sync_mpsc_SendResult result = SYNC_MPSC_SEND_DISCONNECTED; \
    if (!atomic_load(&(chan)->disconnected)) { \
        let new_node = meta_cast$( \
            P$$(sync_mpmc_Node$(T)), \
            catch_((mem_Allocator_create(_allocator, typeInfo$(sync_mpmc_Node$(T))))($ignore, claim_unreachable)) \
        ); \
        new_node->data = (value); \
        atomic_store(&new_node->next, null); \
\
        pthread_mutex_lock(&(chan)->mutex); \
        if (!atomic_load(&(chan)->disconnected)) { \
            sync_mpmc_Node$(T)* prev_head = atomic_exchange(&(chan)->head, new_node); \
            atomic_store(&prev_head->next, new_node); \
            pthread_cond_signal(&(chan)->recv_cond); \
            result = SYNC_MPSC_SEND_OK; \
        } else { \
            mem_Allocator_destroy(_allocator, anyPtr(new_node)); \
        } \
        pthread_mutex_unlock(&(chan)->mutex); \
    } \
    result; \
})

#define sync_mpmc_Channel_tryRecv$(T)                    pp_join($, sync_mpmc_Channel_tryRecv, T)
#define sync_mpmc_Channel_tryRecv$$(T, _allocator, chan) ({ \
    O$(T) \
    result = { .has_value = false }; \
\
    pthread_mutex_lock(&(chan)->mutex); \
    sync_mpmc_Node$(T)* tail = atomic_load(&(chan)->tail); \
    sync_mpmc_Node$(T)* next = atomic_load(&tail->next); \
\
    if (next != null) { \
        result.value     = next->data; \
        result.has_value = true; \
        atomic_store(&(chan)->tail, next); \
        mem_Allocator_destroy(_allocator, anyPtr(tail)); \
    } \
    pthread_mutex_unlock(&(chan)->mutex); \
\
    result; \
})

#define sync_mpmc_Channel_recv$(T)                    pp_join($, sync_mpmc_Channel_recv, T)
#define sync_mpmc_Channel_recv$$(T, _allocator, chan) ({ \
    O$(T) \
    result; \
\
    pthread_mutex_lock(&(chan)->mutex); \
    while (true) { \
        sync_mpmc_Node$(T)* tail = atomic_load(&(chan)->tail); \
        sync_mpmc_Node$(T)* next = atomic_load(&tail->next); \
\
        if (next != null) { \
            result.value     = next->data; \
            result.has_value = true; \
            atomic_store(&(chan)->tail, next); \
            mem_Allocator_destroy(_allocator, anyPtr(tail)); \
            break; \
        } \
\
        if (atomic_load(&(chan)->disconnected)) { \
            result.has_value = false; \
            break; \
        } \
\
        pthread_cond_wait(&(chan)->recv_cond, &(chan)->mutex); \
    } \
    pthread_mutex_unlock(&(chan)->mutex); \
\
    result; \
})

#define sync_mpmc_Channel_disconnect$(T)        pp_join($, sync_mpmc_Channel_disconnect, T)
#define sync_mpmc_Channel_disconnect$$(T, chan) ({ \
    pthread_mutex_lock(&(chan)->mutex); \
    atomic_store(&(chan)->disconnected, true); \
    pthread_cond_broadcast(&(chan)->recv_cond); \
    pthread_cond_broadcast(&(chan)->send_cond); \
    pthread_mutex_unlock(&(chan)->mutex); \
})

// Channel creation function
#define sync_mpsc_channel$(T)                 pp_join($, sync_mpsc_channel, T)
#define sync_mpsc_channel$$(T, _allocator...) ({ \
    /* Create dummy node for MPMC queue */ \
    let dummy = meta_cast$( \
        P$$(sync_mpmc_Node$(T)), \
        catch_((mem_Allocator_create(_allocator, typeInfo$(sync_mpmc_Node$(T))))($ignore, claim_unreachable)) \
    ); \
    atomic_store(&dummy->next, null); \
\
    /* Create internal MPMC channel */ \
    let mpmc_chan = meta_cast$( \
        P$$(sync_mpmc_Channel$(T)), \
        catch_((mem_Allocator_create(_allocator, typeInfo$(sync_mpmc_Channel$(T))))($ignore, claim_unreachable)) \
    ); \
    atomic_store(&mpmc_chan->head, dummy); \
    atomic_store(&mpmc_chan->tail, dummy); \
    atomic_store(&mpmc_chan->disconnected, false); \
    pthread_mutex_init(&mpmc_chan->mutex, null); \
    pthread_cond_init(&mpmc_chan->recv_cond, null); \
    pthread_cond_init(&mpmc_chan->send_cond, null); \
\
    /* Create reference counter */ \
    let counter = meta_cast$( \
        P$$(sync_mpmc_Counter$(T)), \
        catch_((mem_Allocator_create(_allocator, typeInfo$(sync_mpmc_Counter$(T))))($ignore, claim_unreachable)) \
    ); \
    atomic_store(&counter->senders, 1); \
    atomic_store(&counter->receivers, 1); \
    atomic_store(&counter->destroy, false); \
    counter->chan = mpmc_chan; \
\
    sync_mpsc_Channel$(T) ch = { \
        .sender   = { .allocator = _allocator, .counter = counter }, \
        .receiver = { .allocator = _allocator, .counter = counter } \
    }; \
    ch; \
})
#define sync_mpsc_channel_useT$(T) \
    sync_mpmc_Node_useT$(T); \
    sync_mpmc_Channel_useT$(T); \
    sync_mpmc_Counter_useT$(T); \
    sync_mpsc_Sender_useT$(T); \
    sync_mpsc_Receiver_useT$(T); \
    sync_mpsc_Channel_useT$(T); \
    fn_((sync_mpsc_channel$(T)(mem_Allocator allocator))(sync_mpsc_Channel$(T))) { \
        return sync_mpsc_channel$$(T, allocator); \
    }

// Sender functions
#define sync_mpsc_Sender_send$(T)                 pp_join($, sync_mpsc_Sender_send, T)
#define sync_mpsc_Sender_send$$(T, sender, value) ({ \
    sync_mpmc_Channel_send$$(T, (sender).allocator, (sender).counter->chan, value); \
})
#define sync_mpsc_Sender_send_useT$(T) \
    fn_((sync_mpsc_Sender_send$(T)(sync_mpsc_Sender$(T) sender, T value))(sync_mpsc_SendResult)) { \
        return sync_mpsc_Sender_send$$(T, sender, value); \
    }

#define sync_mpsc_Sender_trySend$(T)                 pp_join($, sync_mpsc_Sender_trySend, T)
#define sync_mpsc_Sender_trySend$$(T, sender, value) ({ \
    sync_mpsc_Sender_send$$(T, sender, value); \
})
#define sync_mpsc_Sender_trySend_useT$(T) \
    fn_((sync_mpsc_Sender_trySend$(T)(sync_mpsc_Sender$(T) sender, T value))(sync_mpsc_SendResult)) { \
        return sync_mpsc_Sender_trySend$$(T, sender, value); \
    }

// Sender clone function
#define sync_mpsc_Sender_clone$(T)          pp_join($, sync_mpsc_Sender_clone, T)
#define sync_mpsc_Sender_clone$$(T, sender) ({ \
    usize count = atomic_fetch_add(&(sender).counter->senders, 1); \
    /* Overflow protection like in Rust */ \
    if (count > (SIZE_MAX / 2)) { \
        abort(); \
    } \
    sync_mpsc_Sender$(T) cloned = { .counter = (sender).counter }; \
    cloned; \
})
#define sync_mpsc_Sender_clone_useT$(T) \
    fn_((sync_mpsc_Sender_clone$(T)(sync_mpsc_Sender$(T) sender))(sync_mpsc_Sender$(T))) { \
        return sync_mpsc_Sender_clone$$(T, sender); \
    }

// Sender drop function
#define sync_mpsc_Sender_drop$(T)          pp_join($, sync_mpsc_Sender_drop, T)
#define sync_mpsc_Sender_drop$$(T, sender) ({ \
    if (atomic_fetch_sub(&(sender).counter->senders, 1) == 1) { \
        /* Last sender - disconnect the channel */ \
        sync_mpmc_Channel_disconnect$$(T, (sender).counter->chan); \
\
        /* Check if we should destroy the counter */ \
        if (atomic_exchange(&(sender).counter->destroy, true)) { \
            /* Cleanup */ \
            sync_mpmc_Channel$(T)* chan = (sender).counter->chan; \
\
            /* Free remaining nodes */ \
            sync_mpmc_Node$(T)* node = atomic_load(&chan->tail); \
            while (node) { \
                sync_mpmc_Node$(T)* next = atomic_load(&node->next); \
                mem_Allocator_destroy((sender).allocator, anyPtr(node)); \
                node = next; \
            } \
\
            pthread_mutex_destroy(&chan->mutex); \
            pthread_cond_destroy(&chan->recv_cond); \
            pthread_cond_destroy(&chan->send_cond); \
            mem_Allocator_destroy((sender).allocator, anyPtr(chan)); \
            mem_Allocator_destroy((sender).allocator, anyPtr((sender).counter)); \
        } \
    } \
})
#define sync_mpsc_Sender_drop_useT$(T) \
    fn_((sync_mpsc_Sender_drop$(T)(sync_mpsc_Sender$(T) sender))(void)) { \
        sync_mpsc_Sender_drop$$(T, sender); \
    }

// Receiver functions
#define sync_mpsc_Receiver_recv$(T)            pp_join($, sync_mpsc_Receiver_recv, T)
#define sync_mpsc_Receiver_recv$$(T, receiver) ({ \
    sync_mpmc_Channel_recv$$(T, (receiver).allocator, (receiver).counter->chan); \
})
#define sync_mpsc_Receiver_recv_useT$(T) \
    fn_((sync_mpsc_Receiver_recv$(T)(sync_mpsc_Receiver$(T) receiver))(O$(T))) { \
        return sync_mpsc_Receiver_recv$$(T, receiver); \
    }

#define sync_mpsc_Receiver_tryRecv$(T)            pp_join($, sync_mpsc_Receiver_tryRecv, T)
#define sync_mpsc_Receiver_tryRecv$$(T, receiver) ({ \
    sync_mpmc_Channel_tryRecv$$(T, (receiver).allocator, (receiver).counter->chan); \
})
#define sync_mpsc_Receiver_tryRecv_useT$(T) \
    fn_((sync_mpsc_Receiver_tryRecv$(T)(sync_mpsc_Receiver$(T) receiver))(O$(T))) { \
        return sync_mpsc_Receiver_tryRecv$$(T, receiver); \
    }

// Receiver drop function
#define sync_mpsc_Receiver_drop$(T)            pp_join($, sync_mpsc_Receiver_drop, T)
#define sync_mpsc_Receiver_drop$$(T, receiver) ({ \
    if (atomic_fetch_sub(&(receiver).counter->receivers, 1) == 1) { \
        /* Last receiver - disconnect the channel */ \
        sync_mpmc_Channel_disconnect$$(T, (receiver).counter->chan); \
\
        /* Check if we should destroy the counter */ \
        if (atomic_exchange(&(receiver).counter->destroy, true)) { \
            /* Cleanup */ \
            sync_mpmc_Channel$(T)* chan = (receiver).counter->chan; \
\
            /* Free remaining nodes */ \
            sync_mpmc_Node$(T)* node = atomic_load(&chan->tail); \
            while (node) { \
                sync_mpmc_Node$(T)* next = atomic_load(&node->next); \
                mem_Allocator_destroy((receiver).allocator, anyPtr(node)); \
                node = next; \
            } \
\
            pthread_mutex_destroy(&chan->mutex); \
            pthread_cond_destroy(&chan->recv_cond); \
            pthread_cond_destroy(&chan->send_cond); \
            mem_Allocator_destroy((receiver).allocator, anyPtr(chan)); \
            mem_Allocator_destroy((receiver).allocator, anyPtr((receiver).counter)); \
        } \
    } \
})
#define sync_mpsc_Receiver_drop_useT$(T) \
    fn_((sync_mpsc_Receiver_drop$(T)(sync_mpsc_Receiver$(T) receiver))(void)) { \
        sync_mpsc_Receiver_drop$$(T, receiver); \
    }

// Generate all type definitions and functions for a specific type
#define sync_mpsc_useT$(T) \
    sync_mpsc_channel_useT$(T); \
    sync_mpsc_Sender_send_useT$(T); \
    sync_mpsc_Sender_trySend_useT$(T); \
    sync_mpsc_Sender_clone_useT$(T); \
    sync_mpsc_Sender_drop_useT$(T); \
    sync_mpsc_Receiver_recv_useT$(T); \
    sync_mpsc_Receiver_tryRecv_useT$(T); \
    sync_mpsc_Receiver_drop_useT$(T);

// Example usage
sync_mpsc_useT$(i32);

#include "dh/Thrd.h"
#include "dh/time/Duration.h"

Thrd_fn_(countThrd, ({ sync_mpsc_Sender$i32 sender; }, Void), ($ignore, args)$guard) {
    let sender = args->sender;
    defer_(sync_mpsc_Sender_drop$i32(sender));
    for (i32 i = 0; i < 10; ++i) {
        sync_mpsc_Sender_send$i32(sender, i);
        Thrd_sleep(time_Duration_fromSecs(1));
    }
    return_({});
} $unguarded_(Thrd_fn);

#include "dh/io/common.h"
#include "dh/fs/File.h"

fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    let channels = sync_mpsc_channel$i32(heap_Page_allocator(&(heap_Page){}));
    defer_(sync_mpsc_Receiver_drop$i32(channels.receiver));
    try_(Thrd_spawn(
        Thrd_SpawnConfig_default,
        Thrd_FnCtx_from(countThrd, { channels.sender }).base
    ));

    let std_out = fs_File_writer(io_getStdOut());
    while_some(sync_mpsc_Receiver_recv$i32(channels.receiver), (value)) {
        try_(io_Writer_print(std_out, u8_l("Received: {:d}\n"), value));
    }

    return_ok({});
} $unguarded_(fn);
