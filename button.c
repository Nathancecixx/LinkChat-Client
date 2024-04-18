#include <stdio.h>
#include <string.h>
#include "button.h"

//C
BUTTON CreateButton(float X, float Y, float Width, float Height, Color RectColor, Color TextColor, char* Text){
    BUTTON b;

    Rectangle tmpRect = {X, Y, Width, Height};
    b.rect = tmpRect;

    b.rectColor = RectColor;
    b.textColor = TextColor;
    strncpy(b.text, Text, MAX_BUTTON_TEXT);

    return b;
}

//R
Rectangle GetButtonRectangle(BUTTON b){
    return b.rect;
}

Color GetButtonRectColor (BUTTON b){
    return b.rectColor;
}

Color GetButtonTextColor (BUTTON b){
    return b.textColor;
}

char* GetButtonText (BUTTON b){
    return "tmp";
}

void DrawButton(BUTTON b){
    DrawRectangleRounded(b.rect, 3, 2, b.rectColor);

    int Font_Size = (b.rect.height / 2);

    DrawText(b.text,
             (int) ((b.rect.x + (b.rect.width/2)) - (MeasureText(b.text, Font_Size)/ 2)),
             (int) (b.rect.y + (b.rect.height/4)),
             Font_Size,
             b.textColor);
}

//U
bool SetButtonRectangle(BUTTON* b, float X, float Y, float Width, float Height){
    Rectangle tmpRect = {X, Y, Width, Height};
    b->rect = tmpRect;
    return true;
}

bool SetButtonRectColor (BUTTON* b, Color c){
    b->rectColor = c;
    return true;
}

bool SetButtonTextColor (BUTTON* b, Color c){
    b->textColor = c;
    return true;
}

bool SetButtonText (BUTTON b, char* Text){
    strncpy(b.text, Text, MAX_BUTTON_TEXT);
    return true;
}

bool CheckButton(BUTTON* button){
    //Get mosue position
    Vector2 mousePos = GetMousePosition();

    //Collision check
    if(CheckCollisionPointRec(mousePos, button->rect) == true){
        button->rectColor = RED;
        button->textColor = LIGHTGRAY;
        //Check if button is pressed
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            return true;
        }
    } else{
        button->rectColor = DARKGRAY;
        button->textColor = BLACK;
    }
    return false;
}

//D
void DestroyButton(BUTTON b){
    //Nothing to do here yet...
}