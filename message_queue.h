#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <stdbool.h>

// Forward declaration of the MessageQueue structure
typedef struct MessageQueue MessageQueue;

// Function declarations
MessageQueue* CreateMessageQueue();
void DestroyMessageQueue(MessageQueue* queue);
bool EnqueueMessage(MessageQueue* queue, const char* message);
char* DequeueMessage(MessageQueue* queue);

#endif // MESSAGE_QUEUE_H
