#include "client_list_view.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Creates a new client list component
CLIENT_LIST CreateClientList(float X, float Y, float Width, float Height, Color RectColor, Color TextColor) {
    CLIENT_LIST cl;
    cl.rect = (Rectangle){X, Y, Width, Height};
    cl.rectColor = RectColor;
    cl.textColor = TextColor;
    cl.head = NULL;
    cl.clientCount = 0;
    cl.scrollOffset = 0;
    return cl;
}

// Adds a client to the list
bool AddClientToList(CLIENT_LIST* cl, const char* ip, const char* username) {
    if (cl == NULL || ip == NULL)
        return false;

    PCLIENT_NODE newNode = (PCLIENT_NODE)malloc(sizeof(CLIENT_NODE));
    if (newNode == NULL)
        return false;

    strncpy(newNode->ip, ip, MAX_IP_LENGTH - 1);
    newNode->ip[MAX_IP_LENGTH - 1] = '\0';  // Ensure null-termination

    if (username != NULL) {
        strncpy(newNode->username, username, MAX_USERNAME_LENGTH - 1);
        newNode->username[MAX_USERNAME_LENGTH - 1] = '\0';  // Ensure null-termination
    } else {
        newNode->username[0] = '\0';  // Empty string if username is NULL
    }

    newNode->next = cl->head;
    cl->head = newNode;
    cl->clientCount++;

    return true;
}

// Removes a client from the list by IP address
bool RemoveClientFromList(CLIENT_LIST* cl, const char* ip) {
    if (cl == NULL || ip == NULL)
        return false;

    PCLIENT_NODE current = cl->head;
    PCLIENT_NODE previous = NULL;

    while (current != NULL) {
        if (strcmp(current->ip, ip) == 0) {
            if (previous == NULL) {
                // Removing the head node
                cl->head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            cl->clientCount--;
            return true;
        }
        previous = current;
        current = current->next;
    }

    return false;  // Client not found
}

// Updates the scroll offset based on mouse wheel movement
bool UpdateClientListScroll(CLIENT_LIST* cl) {
    if (cl == NULL)
        return false;

    float mouseWheelMovement = GetMouseWheelMove();

    int maxOffset = cl->clientCount - 1;
    if (mouseWheelMovement > 0 && cl->scrollOffset > 0) {
        cl->scrollOffset--;
    } else if (mouseWheelMovement < 0 && cl->scrollOffset < maxOffset) {
        cl->scrollOffset++;
    }

    return true;
}

// Draws the client list component
void DrawClientList(CLIENT_LIST cl) {
    DrawRectangleRec(cl.rect, cl.rectColor);
    DrawRectangleLinesEx(cl.rect, 1, BLACK);

    int fontSize = 20;
    int lineHeight = fontSize + 5;
    int maxVisibleClients = (int)(cl.rect.height / lineHeight);

    PCLIENT_NODE current = cl.head;

    // Skip clients based on scroll offset
    for (int i = 0; i < cl.scrollOffset && current != NULL; i++) {
        current = current->next;
    }

    int i = 0;
    while (current != NULL && i < maxVisibleClients) {
        char displayText[MAX_IP_LENGTH + MAX_USERNAME_LENGTH + 10];
        if (current->username[0] != '\0') {
            snprintf(displayText, sizeof(displayText), "%s (%s)", current->username, current->ip);
        } else {
            snprintf(displayText, sizeof(displayText), "%s", current->ip);
        }

        DrawText(displayText,
                 (int)(cl.rect.x + 5),
                 (int)(cl.rect.y + 5 + i * lineHeight),
                 fontSize,
                 cl.textColor);

        current = current->next;
        i++;
    }
}

// Destroys the client list and frees memory
void DestroyClientList(CLIENT_LIST* cl) {
    if (cl == NULL)
        return;

    PCLIENT_NODE current = cl->head;
    while (current != NULL) {
        PCLIENT_NODE temp = current;
        current = current->next;
        free(temp);
    }
    cl->head = NULL;
    cl->clientCount = 0;
    cl->scrollOffset = 0;
}
