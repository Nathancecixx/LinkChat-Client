
#include "join.h"


#include <stdbool.h>

#include "p2p.h"

#include "textbox.h"
#include "textfield.h"
#include "button.h"
#include "networkmanager.h"

// Extern declaration of the message queue
extern MessageQueue* g_messageQueue;

void InitializeJoin(JOIN_PAGE* joinPage){
    // Initialize the message queue
    g_messageQueue = CreateMessageQueue();

//    Continue button
    joinPage->BackButton = CreateButton (50,
                               GetScreenHeight() - 100,
                               150,
                               50,
                               DARKGRAY,
                               BLACK,
                               "Back");

    joinPage->ClientTextBox = CreateTextBox(250,
                                  GetScreenHeight() - 100,
                                  500,
                                  50,
                                  DARKGRAY,
                                  BLACK);

    joinPage->ClientTextField = CreateTextField(100,
                                      100,
                                      800,
                                      400,
                                      DARKGRAY,
                                      BLACK);

    joinPage->SendButton = CreateButton (850,
                               GetScreenHeight() - 100,
                               150,
                               50,
                               DARKGRAY,
                               BLACK,
                               "Send");

     joinPage->ServerSocket = OpenConnection(SERVER_PORT, "127.0.0.1", g_messageQueue);

}

int UpdateJoin(JOIN_PAGE* joinPage){


    UpdateTextBox(&joinPage->ClientTextBox);

    if(CheckButton(&joinPage->BackButton))
        return 1;

    if(CheckButton(&joinPage->SendButton) || IsKeyPressed(KEY_ENTER)) {
        SendMsgOverConnection(joinPage->ServerSocket, joinPage->ClientTextBox.text);
        AddMessageToTextField(&joinPage->ClientTextField, "Nathan", joinPage->ClientTextBox.text);
        SetTextBoxText(&joinPage->ClientTextBox, "\0");
    }

    if(joinPage->ServerSocket == -1)
        return 0;

    UpdateTextFieldScroll(&joinPage->ClientTextField);

    char* result;
    while((result = DequeueMessage(g_messageQueue)) != NULL){
        printf("Dequeuing Message\n");
        AddMessageToTextField(&joinPage->ClientTextField, "Server", result);
    }

    /*
    char test [25] = {0};
    if(CheckSocketActivity(server)){
        ReceiveOverConnection(server, &test[0]);
        AddMessageToTextField(&ClientTextField, "SERVER: ", test);
        printf("IF PASSED - %s\n", test);
    }
*/

    return 0;
}

void DrawJoin(const JOIN_PAGE* joinPage){
    BeginDrawing();

    ClearBackground(LIGHTGRAY);

//    Title
    DrawText("Link Chat (Join)", (25) , (25), 50, BLACK);



//    Continue button
    DrawButton(joinPage->BackButton);

    if(joinPage->ServerSocket == -1){
        char ErrorMessage[] ="Connection Failure...";
        DrawText(ErrorMessage,
                 GetScreenWidth()/2 - (MeasureText(ErrorMessage, 75) / 2) ,
                 GetScreenHeight()/2 - 10,
                 75,
                 DARKGRAY);
        EndDrawing();
        return;
    }

    DrawTextBox(joinPage->ClientTextBox);
    DrawTextField(joinPage->ClientTextField);
    DrawButton(joinPage->SendButton);

    EndDrawing();
}

void UnInitializeJoin(JOIN_PAGE* joinPage){
    DestroyTextField(&joinPage->ClientTextField);
    //CloseConnection(server);
}
