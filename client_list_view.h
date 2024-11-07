#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#include "raylib.h"

#define MAX_IP_LENGTH 46        // For IPv6 addresses
#define MAX_USERNAME_LENGTH 64  // Maximum length for username

typedef struct CLIENT_NODE {
    char ip[MAX_IP_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    struct CLIENT_NODE* next;
} CLIENT_NODE, *PCLIENT_NODE;

typedef struct CLIENT_LIST {
    Rectangle rect;
    Color rectColor;
    Color textColor;
    PCLIENT_NODE head;
    int clientCount;
    int scrollOffset;
} CLIENT_LIST;

// Creates a new client list component
CLIENT_LIST CreateClientList(float X, float Y, float Width, float Height, Color RectColor, Color TextColor);

// Adds a client to the list
bool AddClientToList(CLIENT_LIST* cl, const char* ip, const char* username);

// Removes a client from the list by IP address
bool RemoveClientFromList(CLIENT_LIST* cl, const char* ip);

// Updates the scroll offset based on mouse wheel movement
bool UpdateClientListScroll(CLIENT_LIST* cl);

// Draws the client list component
void DrawClientList(CLIENT_LIST cl);

// Destroys the client list and frees memory
void DestroyClientList(CLIENT_LIST* cl);

#endif // CLIENT_LIST_H
