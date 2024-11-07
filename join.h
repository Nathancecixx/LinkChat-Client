
#ifndef LINKCHAT_JOIN_H
#define LINKCHAT_JOIN_H

#include <stdbool.h>

#include "raylib.h"
#include "networkmanager.h"
#include "message_queue.h"
#include "button.h"
#include "textbox.h"
#include "textfield.h"

typedef struct {
    // GUI components
    BUTTON BackButton;
    TEXT_BOX ClientTextBox;
    TEXT_FIELD ClientTextField;
    BUTTON SendButton;

    int ServerSocket;
} JOIN_PAGE;

void InitializeJoin(JOIN_PAGE* joinPage);

int UpdateJoin(JOIN_PAGE* joinPage);

void DrawJoin(const JOIN_PAGE* joinPage);

void UnInitializeJoin(JOIN_PAGE* joinPage);


#endif //LINKCHAT_JOIN_H
