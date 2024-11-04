#include "menu.h"


void InitializeMainMenu(MAIN_MENU* menu){

//    Continue button
    float buttonCenterX = ((float) GetScreenWidth()/2) - (500.0f / 2);
    float buttonCenterY = (float) GetScreenHeight()/2;

    menu->buttons[0] = CreateButton (buttonCenterX,
                                     buttonCenterY - 100,
                                     500,
                                     100,
                                 DARKGRAY,
                                 BLACK,
                                 "Host");

    menu->buttons[1] = CreateButton (buttonCenterX,
                                     buttonCenterY + 50,
                                     500,
                                     100,
                                     DARKGRAY,
                                     BLACK,
                                     "Join");

    menu->buttons[2] = CreateButton (buttonCenterX,
                                  buttonCenterY + 200,
                                  500,
                                  100,
                                  DARKGRAY,
                                  BLACK,
                                  "Quit");

}

int UpdateMainMenu(MAIN_MENU* menu) {

    if(CheckButton(&menu->buttons[0])){
        printf("Host Pressed!\n");
        return 1;
    }

    if(CheckButton(&menu->buttons[1])) {
        printf("Join Pressed!\n");
        return 2;
    }

    if(CheckButton(&menu->buttons[2])) {
        printf("exit pressed!\n");
        return 3;
    }

    return 0;
}


void DrawMainMenu(const MAIN_MENU* menu){
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
    for(int i = 0; i < BTN_COUNT; i++){
        DrawButton(menu->buttons[i]);
    };

    EndDrawing();
}


void UnInitializeMainMenu(MAIN_MENU* menu){
    return;
}
