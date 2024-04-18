#include "raylib.h"
#include "menu.h"
#include "button.h"

#include <stdio.h>

#define VERSION     "1.1.0"

static BUTTON buttonOne;
static BUTTON buttonTwo;
static BUTTON buttonThree;


static Texture2D Background;
static const Vector2 BackgroundVector = {0, 0};

void InitializeMainMenu(){

//    Continue button
    buttonOne = CreateButton (((float) GetScreenWidth()/2) - (500.0f / 2),
                              ((float) GetScreenHeight()/2) - 100,
                             500,
                             100,
                             DARKGRAY,
                             BLACK,
                             "P2P");

    buttonTwo = CreateButton (((float) GetScreenWidth()/2) - (500.0f / 2),
                              ((float) GetScreenHeight()/2) + 50,
                              500,
                              100,
                              DARKGRAY,
                              BLACK,
                              "Server");

    buttonThree = CreateButton (((float) GetScreenWidth()/2) - (500.0f / 2),
                              ((float) GetScreenHeight()/2) + 200,
                              500,
                              100,
                              DARKGRAY,
                              BLACK,
                              "Quit");

}

int UpdateMainMenu() {

    if(CheckButton(&buttonOne)){
        printf("P2P Pressed!\n");
        return 1;
    }

    if(CheckButton(&buttonTwo)) {
        printf("Server Pressed!\n");
        return 2;
    }

    if(CheckButton(&buttonThree)) {
        printf("exit pressed!\n");
        return 3;
    }

    return 0;
}


void DrawMainMenu(){
    BeginDrawing();

    ClearBackground(LIGHTGRAY);

//    DrawTexture(Background, screenWidth/2, screenHeight/2, WHITE);
    //DrawTextureEx(Background, BackgroundVector, 0.0f, 6, LIGHTGRAY);

//    Title
    //DrawRectangleRounded(TitleBorder, 1, 2, BLACK);
    DrawText("Link Chat",
             (50),
             (25),
             100,
             BLACK);

    DrawText(VERSION,
             GetScreenWidth() - MeasureText(VERSION, 25) - 20,
             GetScreenHeight() - 35,
             25,
             BLACK);


//    Continue button
    DrawButton(buttonOne);
    DrawButton(buttonTwo);
    DrawButton(buttonThree);

    EndDrawing();
}


void UnInitializeMainMenu(){
    //UnloadTexture(Background);
    return;
}
