#include "host.h"


static int PeerSocket;


void InitializeHost(HOST_PAGE* host){
//    Continue button
    host->BackButton = CreateButton (50,
                               GetScreenHeight() - 100,
                               150,
                               50,
                               DARKGRAY,
                               BLACK,
                               "Back");

    host->ClientTextBox = CreateTextBox(250,
                                  GetScreenHeight() - 100,
                                  500,
                                  50,
                                  DARKGRAY,
                                  BLACK);

    host->ClientTextField = CreateTextField(100,
                                      100,
                                      800,
                                      400,
                                      DARKGRAY,
                                      BLACK);

    host->SendButton = CreateButton (850,
                               GetScreenHeight() - 100,
                               150,
                               50,
                               DARKGRAY,
                               BLACK,
                               "Send");

}

int UpdateHost(HOST_PAGE* host){


    UpdateTextBox(&host->ClientTextBox);

    if(CheckButton(&host->BackButton))
        return 1;

    if(CheckButton(&host->SendButton) || IsKeyPressed(KEY_ENTER)) {
        //SendOverConnection(server, ClientTextBox.text);
        AddMessageToTextField(&host->ClientTextField, "Nathan", host->ClientTextBox.text);
        SetTextBoxText(&host->ClientTextBox, "\0");
    }

//    if(server == -1)
//        return 0;

    UpdateTextFieldScroll(&host->ClientTextField);

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

void DrawHost(const HOST_PAGE* host){
    BeginDrawing();

    ClearBackground(LIGHTGRAY);

//    Title
    DrawText("Link Chat (Server)", (25) , (25), 50, BLACK);



//    Continue button
    DrawButton(host->BackButton);

//    if(server == -1){
//        char ErrorMessage[] ="Connection Failure...";
//        DrawText(ErrorMessage,
//                 GetScreenWidth()/2 - (MeasureText(ErrorMessage, 75) / 2) ,
//                 GetScreenHeight()/2 - 10,
//                 75,
//                 DARKGRAY);
//        EndDrawing();
//        return;
//    }

    DrawTextBox(host->ClientTextBox);
    DrawTextField(host->ClientTextField);
    DrawButton(host->SendButton);

    EndDrawing();
}

void UnInitializeHost(HOST_PAGE* host){
    DestroyTextField(&host->ClientTextField);
    //CloseConnection(server);
}
