/*********************************************************************************************************************
 * Nathan Ceci ~ 2024 ~ LinkChat *
 *********************************
 *
 * This file contains functions for both hosting a message server and connecting as a client.
 * It handles asynchronous communication, adding incoming messages to the provided message queue.
 *
 *********************************************************************************************************************/

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#ifndef SHUT_RDWR
#define SHUT_RDWR SD_BOTH
#endif
#pragma comment(lib, "Ws2_32.lib")
#define CLOSESOCKET closesocket
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#else
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#define CLOSESOCKET close
#define ISVALIDSOCKET(s) ((s) >= 0)
#endif

#include "networkmanager.h"
#include "message_queue.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <pthread.h>
#endif

// Maximum number of clients
#define MAX_CLIENTS 10

static CLIENT_CONNECTION Clients[MAX_CLIENTS];
static int ClientCount;

// Mutex Locks
#ifdef _WIN32
static CRITICAL_SECTION client_list_lock;
#else
static pthread_mutex_t client_list_lock = PTHREAD_MUTEX_INITIALIZER;
#endif

// Thread function prototypes
#ifdef _WIN32
DWORD WINAPI ServerAcceptThread(LPVOID arg);
DWORD WINAPI ClientRecvThread(LPVOID arg);
DWORD WINAPI ClientHandlerThread(LPVOID arg);
#else
#include <pthread.h>
void* ServerAcceptThread(void* arg);
void* ClientRecvThread(void* arg);
void* ClientHandlerThread(void* arg);
#endif

// Structure to pass arguments to server threads
typedef struct {
    int server_socket;
    MessageQueue* message_queue;
} ServerThreadArgs;

// Structure to pass arguments to client handler threads
typedef struct {
    int client_socket;
    MessageQueue* message_queue;
} ClientHandlerArgs;

// Structure to pass arguments to client receive thread
typedef struct {
    int socket;
    MessageQueue* message_queue;
} ClientRecvArgs;

static bool initialize_sockets() {
#ifdef _WIN32
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize WinSock.\n");
        return false;
    }
    InitializeCriticalSection(&client_list_lock);
#else
    // POSIX systems mutex is statically initialized
#endif
    return true;
}

static void CleanSockets() {
#ifdef _WIN32
    WSACleanup();
    DeleteCriticalSection(&client_list_lock);
#else
    // On POSIX systems, nothing is needed here
#endif
}

#ifdef _WIN32
void PrintSocketError(const char* function_name) {
    int err = WSAGetLastError();
    char* msg_buf = NULL;
    FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, err, 0,
            (LPSTR)&msg_buf, 0, NULL);
    fprintf(stderr, "%s failed with error %d: %s\n", function_name, err, msg_buf);
    LocalFree(msg_buf);
}
#else
void PrintSocketError(const char* function_name) {
    perror(function_name);
}
#endif

int GetActiveIp(char *ip_buffer, size_t buffer_size) {
    if (!initialize_sockets()) {
        fprintf(stderr, "Socket initialization failed. Exiting...\n");
        return -1;
    }

    int sock;
    struct sockaddr_in serv;
    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        CloseConnection(sock);
        return -1;
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("8.8.8.8");
    serv.sin_port = htons(80);

    if (connect(sock, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("connect");
        CloseConnection(sock);
        return -1;
    }

    if (getsockname(sock, (struct sockaddr *)&name, &namelen) < 0) {
        perror("getsockname");
        CloseConnection(sock);
        return -1;
    }

    if (inet_ntop(AF_INET, &name.sin_addr, ip_buffer, buffer_size) == NULL) {
        perror("inet_ntop");
        CloseConnection(sock);
        return -1;
    }

    CloseConnection(sock);
    return 0;
}

// Function to send a message to all connected clients
void SendMessageToClients(const char* message, int origin_socket) {
    if (!message) return;

    // Lock the client list
#ifdef _WIN32
    EnterCriticalSection(&client_list_lock);
#else
    pthread_mutex_lock(&client_list_lock);
#endif

    for (int i = 0; i < ClientCount; ++i) {
        int ClientSocket = Clients[i].SocketId;
        if(ClientSocket == origin_socket) break;
        if (!SendMsgOverConnection(ClientSocket, message)) {
            fprintf(stderr, "Failed to send message to client %d. Closing connection.\n", ClientSocket);
            CloseConnection(ClientSocket);
            // Remove client from the list
            for (int j = i; j < ClientCount - 1; ++j) {
                Clients[j].SocketId = Clients[j + 1].SocketId;
            }
            --ClientCount;
            --i;
        }
    }

#ifdef _WIN32
    LeaveCriticalSection(&client_list_lock);
#else
    pthread_mutex_unlock(&client_list_lock);
#endif
}

// Server function to host a connection
int HostConnection(int port, MessageQueue* message_queue) {
    if (!initialize_sockets()) {
        fprintf(stderr, "Socket initialization failed. Exiting...\n");
        return -1;
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!ISVALIDSOCKET(server_socket)) {
        PrintSocketError("socket");
        CloseConnection(server_socket);
        return -1;
    }

    // Set socket options to allow address reuse
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0) {
        PrintSocketError("setsockopt");
        CLOSESOCKET(server_socket);
        CloseConnection(server_socket);
        return -1;
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    // Bind to all interfaces
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        PrintSocketError("bind");
        CLOSESOCKET(server_socket);
        CloseConnection(server_socket);
        return -1;
    }

    if (listen(server_socket, SOMAXCONN) < 0) {
        PrintSocketError("listen");
        CLOSESOCKET(server_socket);
        CloseConnection(server_socket);
        return -1;
    }

    // Create a thread to accept incoming connections
    ServerThreadArgs* args = malloc(sizeof(ServerThreadArgs));
    if (!args) {
        fprintf(stderr, "Failed to allocate memory for server thread args.\n");
        CLOSESOCKET(server_socket);
        CloseConnection(server_socket);
        return -1;
    }
    args->server_socket = server_socket;
    args->message_queue = message_queue;

#ifdef _WIN32
    HANDLE accept_thread = CreateThread(NULL, 0, ServerAcceptThread, args, 0, NULL);
    if (accept_thread == NULL) {
        fprintf(stderr, "Failed to create server accept thread.\n");
        CLOSESOCKET(server_socket);
        free(args);
        CloseConnection(server_socket);
        return -1;
    }
    CloseHandle(accept_thread); // Optionally close the handle if not needed
#else
    pthread_t accept_thread;
    if (pthread_create(&accept_thread, NULL, ServerAcceptThread, args) != 0) {
        fprintf(stderr, "Failed to create server accept thread.\n");
        CLOSESOCKET(server_socket);
        free(args);
        CleanSockets();
        return -1;
    }
    pthread_detach(accept_thread); // Detach the thread
#endif

    return server_socket;
}

// Thread function to accept incoming client connections
#ifdef _WIN32
DWORD WINAPI ServerAcceptThread(LPVOID arg) {
#else
    void* ServerAcceptThread(void* arg) {
#endif
    ServerThreadArgs* args = (ServerThreadArgs*)arg;
    int server_socket = args->server_socket;
    MessageQueue* message_queue = args->message_queue;
    free(args); // Free the arguments structure

    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);
        if (!ISVALIDSOCKET(client_socket)) {
            PrintSocketError("accept");
            continue;
        }

        // Add client socket to the list
#ifdef _WIN32
        EnterCriticalSection(&client_list_lock);
#else
        pthread_mutex_lock(&client_list_lock);
#endif

        if (ClientCount < MAX_CLIENTS) {
            Clients[ClientCount++].SocketId = client_socket;
        } else {
            fprintf(stderr, "Maximum clients reached. Connection refused.\n");
            CloseConnection(client_socket);
#ifdef _WIN32
            LeaveCriticalSection(&client_list_lock);
#else
            pthread_mutex_unlock(&client_list_lock);
#endif
            continue;
        }

#ifdef _WIN32
        LeaveCriticalSection(&client_list_lock);
#else
        pthread_mutex_unlock(&client_list_lock);
#endif

        // Create a thread to handle the client
        ClientHandlerArgs* client_args = malloc(sizeof(ClientHandlerArgs));
        if (!client_args) {
            fprintf(stderr, "Failed to allocate memory for client handler args.\n");
            CLOSESOCKET(client_socket);
            continue;
        }
        client_args->client_socket = client_socket;
        client_args->message_queue = message_queue;

#ifdef _WIN32
        HANDLE client_thread = CreateThread(NULL, 0, ClientHandlerThread, client_args, 0, NULL);
        if (client_thread == NULL) {
            fprintf(stderr, "Failed to create client handler thread.\n");
            CLOSESOCKET(client_socket);
            free(client_args);
            continue;
        }
        CloseHandle(client_thread);
#else
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, ClientHandlerThread, client_args) != 0) {
            fprintf(stderr, "Failed to create client handler thread.\n");
            CLOSESOCKET(client_socket);
            free(client_args);
            continue;
        }
        pthread_detach(client_thread);
#endif
    }

#ifdef _WIN32
    return 0;
#else
    return NULL;
#endif
}

// Thread function to handle communication with a client
#ifdef _WIN32
DWORD WINAPI ClientHandlerThread(LPVOID arg) {
#else
    void* ClientHandlerThread(void* arg) {
#endif
    ClientHandlerArgs* client_args = (ClientHandlerArgs*)arg;
    int client_socket = client_args->client_socket;
    MessageQueue* message_queue = client_args->message_queue;
    free(client_args); // Free the arguments structure

    char* message = NULL;
    size_t message_size = 0;

    while (1) {
        if (!RecMsgOverConnection(client_socket, &message, &message_size)) {
            // Connection was closed or error occurred
            printf("Client disconnected\n");
            break;
        }

        // Enqueue the received message
        if (!EnqueueMessage(message_queue, message)) {
            fprintf(stderr, "Failed to enqueue message from client.\n");
        } else {
            // Broadcast the message to other clients
            SendMessageToClients(message, client_socket);
        }

        free(message);
        message = NULL;
        message_size = 0;
    }

    // Remove client socket from the list
#ifdef _WIN32
    EnterCriticalSection(&client_list_lock);
#else
    pthread_mutex_lock(&client_list_lock);
#endif

    for (int i = 0; i < ClientCount; ++i) {
        if (Clients[i].SocketId == client_socket) {
            for (int j = i; j < ClientCount - 1; ++j) {
                Clients[j].SocketId = Clients[j + 1].SocketId;
            }
            --ClientCount;
            break;
        }
    }

#ifdef _WIN32
    LeaveCriticalSection(&client_list_lock);
#else
    pthread_mutex_unlock(&client_list_lock);
#endif

    CloseConnection(client_socket);

#ifdef _WIN32
    return 0;
#else
    return NULL;
#endif
}

// Client function to open a connection to a server
int OpenConnection(int port, const char* ip, MessageQueue* message_queue) {
    if (!initialize_sockets()) {
        fprintf(stderr, "Socket initialization failed. Exiting...\n");
        return -1;
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (!ISVALIDSOCKET(socket_fd)) {
        PrintSocketError("socket");
        CloseConnection(socket_fd);
        return -1;
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &server_address.sin_addr) <= 0) {
        PrintSocketError("inet_pton");
        CloseConnection(socket_fd);
        return -1;
    }

    if (connect(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        PrintSocketError("connect");
        CloseConnection(socket_fd);
        return -1;
    }

    // Create a thread to receive messages from the server
    ClientRecvArgs* args = malloc(sizeof(ClientRecvArgs));
    if (!args) {
        fprintf(stderr, "Failed to allocate memory for client recv args.\n");
        CloseConnection(socket_fd);
        return -1;
    }
    args->socket = socket_fd;
    args->message_queue = message_queue;

#ifdef _WIN32
    HANDLE recv_thread = CreateThread(NULL, 0, ClientRecvThread, args, 0, NULL);
    if (recv_thread == NULL) {
        fprintf(stderr, "Failed to create client receive thread.\n");
        CLOSESOCKET(socket_fd);
        free(args);
        CloseConnection(socket_fd);
        return -1;
    }
    CloseHandle(recv_thread); // Optionally close the handle if not needed
#else
    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, ClientRecvThread, args) != 0) {
        fprintf(stderr, "Failed to create client receive thread.\n");
        CLOSESOCKET(socket_fd);
        free(args);
        CleanSockets();
        return -1;
    }
    pthread_detach(recv_thread); // Detach the thread
#endif

    return socket_fd;
}

// Thread function to receive messages from the server
#ifdef _WIN32
DWORD WINAPI ClientRecvThread(LPVOID arg) {
#else
    void* ClientRecvThread(void* arg) {
#endif
    ClientRecvArgs* args = (ClientRecvArgs*)arg;
    int socket_fd = args->socket;
    MessageQueue* message_queue = args->message_queue;
    free(args); // Free the arguments structure

    char* message = NULL;
    size_t message_size = 0;

    while (1) {
        // Receive message over connection
        if (!RecMsgOverConnection(socket_fd, &message, &message_size)) {
            // Error or connection closed
            break;
        }

        // Enqueue message into the message queue
        if (!EnqueueMessage(message_queue, message)) {
            fprintf(stderr, "Failed to enqueue message from server.\n");
        }

        free(message); // Free the message after enqueuing
        message = NULL;
        message_size = 0;
    }

    // Close the socket
    CloseConnection(socket_fd);

#ifdef _WIN32
    return 0;
#else
    return NULL;
#endif
}

// Communication functions

bool SendMsgOverConnection(int socket_fd, const char* message) {
    if (!message) return false;

    uint32_t message_length = (uint32_t)strlen(message);
    uint32_t network_order_length = htonl(message_length);

    ssize_t bytes_sent = send(socket_fd, (const char*)&network_order_length, sizeof(network_order_length), 0);
    if (bytes_sent != sizeof(network_order_length)) {
        PrintSocketError("send");
        return false;
    }

    bytes_sent = send(socket_fd, message, message_length, 0);
    if (bytes_sent != message_length) {
        PrintSocketError("send");
        return false;
    }

    return true;
}

bool RecMsgOverConnection(int socket_fd, char** dest_buffer, size_t* dest_size) {
    uint32_t network_order_length;
    ssize_t bytes_received = recv(socket_fd, (char*)&network_order_length, sizeof(network_order_length), 0);
    if (bytes_received != sizeof(network_order_length)) {
        PrintSocketError("recv");
        return false;
    }

    uint32_t message_length = ntohl(network_order_length);
    *dest_buffer = (char*)malloc(message_length + 1);
    if (!*dest_buffer) {
        fprintf(stderr, "Memory allocation failed.\n");
        return false;
    }

    size_t total_received = 0;
    while (total_received < message_length) {
        bytes_received = recv(socket_fd, *dest_buffer + total_received, message_length - total_received, 0);
        if (bytes_received <= 0) {
            PrintSocketError("recv");
            free(*dest_buffer);
            *dest_buffer = NULL;
            return false;
        }
        total_received += bytes_received;
    }

    (*dest_buffer)[message_length] = '\0';
    *dest_size = message_length;

    return true;
}

bool CloseConnection(int socket_fd) {
    shutdown(socket_fd, SHUT_RDWR);
    if (CLOSESOCKET(socket_fd) != 0) {
        PrintSocketError("close");
        return false;
    }
    return true;
}

int GetClientCount(void){
        return ClientCount;
}

CLIENT_CONNECTION* GetCLientList(){
        return Clients;
}
