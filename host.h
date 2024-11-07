#ifndef HOST_H
#define HOST_H

#include "raylib.h"
#include "networkmanager.h"
#include "message_queue.h"
#include "button.h"
#include "textbox.h"
#include "textfield.h"
#include "client_list_view.h"

// Include other necessary headers and definitions

typedef struct {
    // GUI components
    BUTTON BackButton;
    TEXT_BOX ClientTextBox;
    TEXT_FIELD ClientTextField;
    BUTTON SendButton;
    CLIENT_LIST ClientListView;

    int ServerSocket;
    char* CodeBuffer;
} HOST_PAGE;

void InitializeHost(HOST_PAGE* host);
int UpdateHost(HOST_PAGE* host);
void DrawHost(const HOST_PAGE* host);
void UnInitializeHost(HOST_PAGE* host);

#endif // HOST_H
