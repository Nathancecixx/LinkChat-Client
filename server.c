
#include <string.h>

#include "server.h"
#include "textbox.h"
#include "textfield.h"
#include "button.h"
#include "networkmanager.h"

static BUTTON BackButton;
static BUTTON SendButton;
static TEXT_BOX ClientTextBox;
static TEXT_FIELD ClientTextField;


static BUTTON RoomOne;
static BUTTON RoomTwo;
static BUTTON RoomThree;
static BUTTON RoomFour;
static BUTTON RoomFive;
static BUTTON RoomBack;

static bool IsInRoom;
static int RoomNum;

//static int server;

void InitializeServer(){
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

    InitializeServerMenu();

    IsInRoom = false;
    //server = OpenConnection(9002, 10);
}

int UpdateServer(int server){

    //If room is not chosen, do different update
    if(!IsInRoom){
        int result = UpdateServerMenu(server);
        if (result == 0)
            return 0;
        else if(result == -1)
            return 1;
        else{
            IsInRoom = true;
            RoomNum = result;
        }
    }

    //Room is chosen, Continue to normal update

    UpdateTextBox(&ClientTextBox);

    if(CheckButton(&BackButton)){
        IsInRoom = false;
        DestroyTextField(&ClientTextField);
        
    }

    if(CheckButton(&SendButton) || IsKeyPressed(KEY_ENTER)) {
        //Add '@' to front of message to signify message instead of command
        char message [MAX_MESSAGE_LENGTH + 1];
        strcpy(message, "@");
        strcat(message, ClientTextBox.text);

        SendOverConnection(server, message);
        AddMessageToTextField(&ClientTextField, "Nathan", ClientTextBox.text);
        SetTextBoxText(&ClientTextBox, "\0");
    }

    if(server == -1)
        return 0;

    UpdateTextFieldScroll(&ClientTextField);

    char test [25] = {0};
    if(CheckSocketActivity(server)){
        ReceiveOverConnection(server, &test[0]);
        AddMessageToTextField(&ClientTextField, "SERVER: ", test);
        printf("IF PASSED - %s\n", test);
    }

    return 0;
}

void DrawServer(){
    BeginDrawing();

    if(!IsInRoom){
        DrawServerMenu();
        EndDrawing();
        return 0;
    }

    ClearBackground(LIGHTGRAY);

//    Title
    DrawText("Link Chat (Server)", (25) , (25), 50, BLACK);

    switch (RoomNum) {
        case 1:{
            DrawText("Room One", (750) , (25), 50, BLACK);
        }

    }




//    Continue button
    DrawButton(BackButton);

    DrawTextBox(ClientTextBox);
    DrawTextField(ClientTextField);
    DrawButton(SendButton);

    EndDrawing();
}

void UnInitializeServer(){
    DestroyTextField(&ClientTextField);
    //CloseConnection(server);
}


void InitializeServerMenu(){
    RoomOne = CreateButton (GetScreenWidth()/2 - (250/2),
                               200,
                               250,
                               50,
                               DARKGRAY,
                               BLACK,
                               "Room One");

    RoomTwo = CreateButton (GetScreenWidth()/2 - (250/2),
                            275,
                            250,
                            50,
                            DARKGRAY,
                            BLACK,
                            "Room Two");

    RoomThree = CreateButton (GetScreenWidth()/2 - (250/2),
                            350,
                            250,
                            50,
                            DARKGRAY,
                            BLACK,
                            "Room Three");

    RoomFour = CreateButton (GetScreenWidth()/2 - (250/2),
                            425,
                            250,
                            50,
                            DARKGRAY,
                            BLACK,
                            "Room Four");

    RoomFive = CreateButton (GetScreenWidth()/2 - (250/2),
                            500,
                            250,
                            50,
                            DARKGRAY,
                            BLACK,
                            "Room Five");

    RoomBack = CreateButton (GetScreenWidth()/2 - (250/2),
                            575,
                            250,
                            50,
                            DARKGRAY,
                            BLACK,
                            "Back");

}

int UpdateServerMenu(int server){
    if(CheckButton(&RoomOne)){
        char message [MAX_MESSAGE_LENGTH]= "$0";
        SendOverConnection(server, message);
        return 1;
    }
    if(CheckButton(&RoomTwo)){
        char message[MAX_MESSAGE_LENGTH] = "$1";
        SendOverConnection(server, message);
        return 2;
    }
    if(CheckButton(&RoomThree)){
        char message[MAX_MESSAGE_LENGTH] = "$2";
        SendOverConnection(server, message);
        return 3;
    }
    if(CheckButton(&RoomFour)){
        char message[MAX_MESSAGE_LENGTH] = "$3";
        SendOverConnection(server, message);
        return 4;
    }
    if(CheckButton(&RoomFive)){
        char message[MAX_MESSAGE_LENGTH] = "$4";
        SendOverConnection(server, message);
        return 5;
    }

    if(CheckButton(&RoomBack))
        return -1;

    return 0;
}

void DrawServerMenu(){
    ClearBackground(LIGHTGRAY);
    DrawText("Rooms:", (25) , (25), 50, BLACK);

    DrawButton(RoomOne);
    DrawButton(RoomTwo);
    DrawButton(RoomThree);
    DrawButton(RoomFour);
    DrawButton(RoomFive);
    DrawButton(RoomBack);
}

void UnInitializeServerMenu();