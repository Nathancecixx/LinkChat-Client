#include <stdbool.h>

#include "p2p.h"

#include "textbox.h"
#include "textfield.h"
#include "button.h"
#include "networkmanager.h"


static BUTTON BackButton;
static BUTTON SendButton;
static TEXT_BOX ClientTextBox;
static TEXT_FIELD ClientTextField;

static int server;

void InitializeP2P(){
//    Continue button
    BackButton = CreateButton (50,
                              GetScreenHeight() - 100,
                              150,
                              50,
                              DARKGRAY,
                              BLACK,
                              "Back");

    ClientTextBox = CreateTextBox(250,
                                  GetScreenHeight() - 100,
                                  500,
                                  50,
                                  DARKGRAY,
                                  BLACK);

    ClientTextField = CreateTextField(100,
                                      100,
                                      800,
                                      400,
                                      DARKGRAY,
                                      BLACK);

    SendButton = CreateButton (850,
                               GetScreenHeight() - 100,
                               150,
                               50,
                               DARKGRAY,
                               BLACK,
                               "Send");

    //server = OpenConnection(9002, 10);

}

int UpdateP2P(){


    UpdateTextBox(&ClientTextBox);

    if(CheckButton(&BackButton))
        return 1;

    if(CheckButton(&SendButton) || IsKeyPressed(KEY_ENTER)) {
        //SendOverConnection(server, ClientTextBox.text);
        AddMessageToTextField(&ClientTextField, "Nathan", ClientTextBox.text);
        SetTextBoxText(&ClientTextBox, "\0");
    }

    if(server == -1)
        return 0;

    UpdateTextFieldScroll(&ClientTextField);

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

void DrawP2P(){
    BeginDrawing();

    ClearBackground(LIGHTGRAY);

//    Title
    DrawText("Link Chat (Server)", (25) , (25), 50, BLACK);



//    Continue button
    DrawButton(BackButton);

    if(server == -1){
        char ErrorMessage[] ="Connection Failure...";
        DrawText(ErrorMessage,
                 GetScreenWidth()/2 - (MeasureText(ErrorMessage, 75) / 2) ,
                 GetScreenHeight()/2 - 10,
                 75,
                 DARKGRAY);
        EndDrawing();
        return;
    }

    DrawTextBox(ClientTextBox);
    DrawTextField(ClientTextField);
    DrawButton(SendButton);

    EndDrawing();
}

void UnInitializeP2P(){
    DestroyTextField(&ClientTextField);
    //CloseConnection(server);
}
