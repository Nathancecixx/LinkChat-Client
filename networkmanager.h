#ifndef LINKCHAT_NETWORKMAN_H
#define LINKCHAT_NETWORKMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "message_queue.h"

#define MAX_CLIENTS 10

#define SERVER_IP       "127.0.0.1"
#define SERVER_PORT     9003

#define IP_LENGTH   16
#define MAX_USERNAME    10

typedef struct {
    int SocketId;
    char UserName[MAX_USERNAME];
    char IpAddr[IP_LENGTH];
}CLIENT_CONNECTION;

typedef struct {
    int port;
} SERVER_THREAD_ARGS;

// Hosting functions
int GetActiveIp(char* ip_buffer, size_t buffer_size);
int GetClientCount(void);

int HostConnection(int port, MessageQueue* message_queue);

// Joining functions
int OpenConnection(int port, const char* ip, MessageQueue* message_queue);

bool SendMsgOverConnection(int socket, const char* message);

bool RecMsgOverConnection(int socket, char** dest_buffer, size_t* dest_size);

void SendMessageToClients(const char* message, int origin_socket);

bool CloseConnection(int socket);

static void CleanSockets(void);

CLIENT_CONNECTION* GetCLientList();

#endif // LINKCHAT_NETWORKMAN_H
