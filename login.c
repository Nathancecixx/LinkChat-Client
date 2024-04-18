
#include "p2p.h"

#include "textbox.h"
#include "textfield.h"
#include "button.h"
#include "networkmanager.h"
#include "login.h"

static BUTTON QuitButton;
static BUTTON LoginButton;
static TEXT_BOX Username_Box;
static TEXT_BOX Password_Box;

//int server;


void InitializeLogin(){

    Username_Box = CreateTextBox(((float) GetScreenWidth()/2) - (500.0f / 2),
                                 ((float) GetScreenHeight()/2) - 100,
                                 500,
                                 50,
                                 DARKGRAY,
                                 BLACK);

    Password_Box = CreateTextBox(((float) GetScreenWidth()/2) - (500.0f / 2),
                                 ((float) GetScreenHeight()/2) + 50,
                                 500,
                                 50,
                                 DARKGRAY,
                                 BLACK);

    LoginButton = CreateButton (((float) GetScreenWidth()/2) - (250.0f / 2),
                                ((float) GetScreenHeight()/2) + 150,
                                250,
                                50,
                                DARKGRAY,
                                BLACK,
                                "Login");

    //    Continue button
    QuitButton = CreateButton (50,
                               GetScreenHeight() - 100,
                               150,
                               50,
                               DARKGRAY,
                               BLACK,
                               "Quit");




}

int UpdateLogin(int server){

    CheckTextBox(&Username_Box);

    CheckTextBox(&Password_Box);

    if(Username_Box.IsSelected == true)
        UpdateTextBox(&Username_Box);

    if(Password_Box.IsSelected == true)
        UpdateTextBox(&Password_Box);

    if(CheckButton(&LoginButton) || IsKeyPressed(KEY_ENTER)) {
        //SendOverConnection(server, ClientTextBox.text);
        //AddMessageToTextField(&ClientTextField, "Nathan", ClientTextBox.text);
        //SendOverConnection(server, Username_Box.text);
        SetTextBoxText(&Username_Box, "\0");
        SetTextBoxText(&Password_Box, "\0");
        return 1;
    }

    if(CheckButton(&QuitButton))
        return 2;

    return 0;
}

void DrawLogin(int server){
    BeginDrawing();

    ClearBackground(LIGHTGRAY);

//    Title
    DrawText("Link Chat (LOGIN)", (25) , (25), 50, BLACK);



//    Continue button
    DrawButton(QuitButton);

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

    DrawText("Username:",
             GetScreenWidth()/2 - (MeasureText("Username",  25)/2),
             200,
             25,
             BLACK);

    DrawTextBox(Username_Box);

    DrawText("Password:",
             GetScreenWidth()/2 - (MeasureText("Password",  25)/2),
             350,
             25,
             BLACK);
    DrawTextBox(Password_Box);

    DrawButton(QuitButton);
    DrawButton(LoginButton);

    EndDrawing();
}

void UnInitializeLogin(){
    //Nothing to do for now
}
