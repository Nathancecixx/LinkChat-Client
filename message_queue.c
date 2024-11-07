#include "message_queue.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
    #include <string.h>
#endif

#include <stdlib.h>

// Define the MessageQueue structure with platform-specific synchronization primitives
struct MessageQueue {
#ifdef _WIN32
    CRITICAL_SECTION lock;
#else
    pthread_mutex_t lock;
#endif
    // Message storage (e.g., a linked list or array)
    char** messages;
    int head;
    int tail;
    int capacity;
};

MessageQueue* CreateMessageQueue() {
    MessageQueue* queue = malloc(sizeof(MessageQueue));
    if (!queue) return NULL;

#ifdef _WIN32
    InitializeCriticalSection(&queue->lock);
#else
    pthread_mutex_init(&queue->lock, NULL);
#endif

    // Initialize message storage
    queue->capacity = 100;
    queue->messages = malloc(sizeof(char*) * queue->capacity);
    queue->head = queue->tail = 0;

    return queue;
}

void DestroyMessageQueue(MessageQueue* queue) {
    if (!queue) return;

#ifdef _WIN32
    DeleteCriticalSection(&queue->lock);
#else
    pthread_mutex_destroy(&queue->lock);
#endif

    // Free stored messages
    for (int i = queue->head; i != queue->tail; i = (i + 1) % queue->capacity) {
        free(queue->messages[i]);
    }
    free(queue->messages);
    free(queue);
}

bool EnqueueMessage(MessageQueue* queue, const char* message) {
    if (!queue || !message) return false;

#ifdef _WIN32
    EnterCriticalSection(&queue->lock);
#else
    pthread_mutex_lock(&queue->lock);
#endif

    // Check for capacity
    int next_tail = (queue->tail + 1) % queue->capacity;
    if (next_tail == queue->head) {
        // Queue is full
#ifdef _WIN32
        LeaveCriticalSection(&queue->lock);
#else
        pthread_mutex_unlock(&queue->lock);
#endif
        return false;
    }

    // Copy the message
    queue->messages[queue->tail] = strdup(message);
    queue->tail = next_tail;

#ifdef _WIN32
    LeaveCriticalSection(&queue->lock);
#else
    pthread_mutex_unlock(&queue->lock);
#endif

    return true;
}

char* DequeueMessage(MessageQueue* queue) {
    if (!queue) return NULL;

#ifdef _WIN32
    EnterCriticalSection(&queue->lock);
#else
    pthread_mutex_lock(&queue->lock);
#endif

    if (queue->head == queue->tail) {
        // Queue is empty
#ifdef _WIN32
        LeaveCriticalSection(&queue->lock);
#else
        pthread_mutex_unlock(&queue->lock);
#endif
        return NULL;
    }

    char* message = queue->messages[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;

#ifdef _WIN32
    LeaveCriticalSection(&queue->lock);
#else
    pthread_mutex_unlock(&queue->lock);
#endif

    return message;
}
