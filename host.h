
#ifndef LINKCHAT_HOST_H
#define LINKCHAT_HOST_H

#include <stdbool.h>

#include "textbox.h"
#include "textfield.h"
#include "button.h"
#include "networkmanager.h"

typedef struct {
    BUTTON BackButton;
    BUTTON SendButton;
    TEXT_BOX ClientTextBox;
    TEXT_FIELD ClientTextField;
}HOST_PAGE;

void InitializeHost(HOST_PAGE* host);

int UpdateHost(HOST_PAGE* host);

void DrawHost(const HOST_PAGE* host);

void UnInitializeHost(HOST_PAGE* host);

#endif //LINKCHAT_HOST_H
