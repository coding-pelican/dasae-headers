#include "dh/atomic.h"
#include "dh/builtin/comp.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// -pthread

// Shared counter between threads
typedef atomic_Value$(int) AtomicCounter;
use_atomic_Value$(int);
// Shared buffer state
struct SharedBuffer {
    atomic_Value$(int) write_index;
    atomic_Value$(int) read_index;
    atomic_Value$(bool) is_active;
    int data[1024];
};

// Initialize shared buffer
void init_buffer(struct SharedBuffer* buffer) {
    buffer->write_index = atomic_init$(TypeOf(buffer->write_index), 0);
    buffer->read_index  = atomic_init$(TypeOf(buffer->read_index), 0);
    buffer->is_active   = atomic_init$(TypeOf(buffer->is_active), true);
}

// Producer thread function
void* producer(void* arg) {
    struct SharedBuffer* buffer = (struct SharedBuffer*)arg;
    int                  value  = 0;

    while (atomic_load$(bool, &buffer->is_active.raw, atomic_MemOrder_acquire)) {
        int write_idx = atomic_load$(int, &buffer->write_index.raw, atomic_MemOrder_acquire);
        int read_idx  = atomic_load$(int, &buffer->read_index.raw, atomic_MemOrder_acquire);

        // Check if buffer is not full
        if (write_idx - read_idx < 1024) {
            // Store value in buffer
            buffer->data[write_idx % 1024] = value;

            // Increment write index atomically
            atomic_store$(int, &buffer->write_index.raw, write_idx + 1, atomic_MemOrder_release);

            printf("Produced: %d\n", value++);
        } else {
            // Buffer full, yield to consumer
            atomic_spinLoopHint();
        }
    }

    return null;
}

// Consumer thread function
void* consumer(void* arg) {
    struct SharedBuffer* buffer         = (struct SharedBuffer*)arg;
    int                  consumed_count = 0;

    while (atomic_load$(bool, &buffer->is_active.raw, atomic_MemOrder_acquire)) {
        int write_idx = atomic_load$(int, &buffer->write_index.raw, atomic_MemOrder_acquire);
        int read_idx  = atomic_load$(int, &buffer->read_index.raw, atomic_MemOrder_acquire);

        // Check if buffer is not empty
        if (read_idx < write_idx) {
            // Read value from buffer
            int value = buffer->data[read_idx % 1024];

            // Increment read index atomically
            atomic_store$(int, &buffer->read_index.raw, read_idx + 1, atomic_MemOrder_release);

            printf("Consumed: %d\n", value);
            consumed_count++;

            // Stop after consuming 100 items
            if (consumed_count >= 100) {
                atomic_store$(bool, &buffer->is_active.raw, false, atomic_MemOrder_release);
                break;
            }
        } else {
            // Buffer empty, yield to producer
            atomic_spinLoopHint();
        }
    }

    return null;
}

int main() {
    // Initialize shared buffer
    struct SharedBuffer buffer = cleared();
    init_buffer(&buffer);

    // Create producer and consumer threads
    pthread_t producer_thread = cleared();
    pthread_t consumer_thread = cleared();
    pthread_create(&producer_thread, null, producer, &buffer);
    pthread_create(&consumer_thread, null, consumer, &buffer);

    // Wait for threads to complete
    pthread_join(producer_thread, null);
    pthread_join(consumer_thread, null);

    printf("Processing complete\n");
    return 0;
}
