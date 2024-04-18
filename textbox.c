#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "textbox.h"

//C
TEXT_BOX CreateTextBox(float X, float Y, float Width, float Height, Color RectColor, Color TextColor){
    TEXT_BOX tb;

    Rectangle tmpRect = {X, Y, Width, Height};
    tb.rect = tmpRect;

    tb.rectColor = RectColor;
    tb.textColor = TextColor;
    tb.text[0] = '\0';
    tb.charCount = 0;

    return tb;
}

//R
Rectangle GetTextBoxRectangle(TEXT_BOX tb){
    return tb.rect;
}
Color GetTextBoxRectColor (TEXT_BOX tb){
    return tb.rectColor;
}
Color GetTextBoxTextColor (TEXT_BOX tb){
    return tb.textColor;
}
char* GetTextBoxText (TEXT_BOX tb){
    char* returnText = (char*) malloc (sizeof(tb.text));
    strncpy(returnText, tb.text, MAX_MESSAGE_LENGTH);
    return returnText;
}

void DrawTextBox(TEXT_BOX tb){
    DrawRectangleRounded(tb.rect, 2, 2, LIGHTGRAY);
    DrawRectangleLinesEx(tb.rect, 2, tb.rectColor);

    int Font_Size = (tb.rect.height / 2);

    char tmp [6];
    snprintf(tmp, sizeof(char) * 6,"%d/%d", tb.charCount, MAX_MESSAGE_LENGTH );

   Color visualCountColor;
   if(tb.charCount <= (MAX_MESSAGE_LENGTH / 2))
       visualCountColor = BLACK;
   else
       visualCountColor = RED;

    DrawText(tmp,
             (int) (tb.rect.x + tb.rect.width) + 10,
             (int) (tb.rect.y + (tb.rect.height / 4)),
             20,
             visualCountColor);

    DrawText(tb.text,
             (int) ((tb.rect.x + (tb.rect.width/2)) - (MeasureText(tb.text, Font_Size)/ 2)),
             (int) (tb.rect.y + (tb.rect.height/4)),
             Font_Size,
             tb.textColor);
}

//U
bool SetTextBoxRectangle(TEXT_BOX* tb, float X, float Y, float Width, float Height){
    Rectangle tmpRect = {X, Y, Width, Height};
    tb->rect = tmpRect;
    return true;
}

bool SetTextBoxRectColor (TEXT_BOX* tb, Color c){
    tb->rectColor = c;
    return true;
}

bool SetTextBoxTextColor (TEXT_BOX* tb, Color c){
    tb->textColor = c;
    return true;
}

bool SetTextBoxText (TEXT_BOX* tb, char* Text){
    strncpy(tb->text, Text, MAX_MESSAGE_LENGTH);
    tb->charCount = strlen(Text);
    return true;
}

bool ClearTextBox (TEXT_BOX* tb){
    if(tb == NULL)
        return false;
    strcpy(tb->text, "\0");
    tb->charCount = strlen("\0");
    return true;
}

bool UpdateTextBox(TEXT_BOX* tb){

    // Get key press from user
    int key = GetKeyPressed();

    // Check if Shift key is down is also down
    bool shiftPressed = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    // If shift is not being pressed, edit numbers
    if (!shiftPressed) {
        // Shift is not pressed, if key is a letter change to lowercase
        if (key >= 'A' && key <= 'Z') {
            key += 32;
        }
    } else{
        //Shift is pressed, check if key is not a letter and change
        // If it is a letter leave it be as it is already uppercase
        if(key <= 'A' || key >= 'Z'){
            key -= 16;
        }
    }


    // NOTE: Adjust the allowed key range if needed
    if ((key >= 32) && (key <= 125) && (tb->charCount < MAX_MESSAGE_LENGTH)) {
        tb->text[tb->charCount] = (char)key;
        tb->text[tb->charCount+1] = '\0'; // Add null terminator at the end of the string.
        tb->charCount++;
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        tb->charCount--;
        if (tb->charCount < 0) tb->charCount = 0;
        tb->text[tb->charCount] = '\0';
    }

    return true;
}

bool CheckTextBox(TEXT_BOX* tb){
    //Get mosue position
    Vector2 mousePos = GetMousePosition();

    //Collision check
    if(CheckCollisionPointRec(mousePos, tb->rect) == true){
        tb->rectColor = RED;
        tb->IsSelected = true;
        //Check if button is pressed
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            tb->IsSelected = true;
        }
    } else{
        tb->rectColor = BLACK;
        tb->IsSelected = false;
    }
    return false;
}

//D
void DestroyTextBox(TEXT_BOX* tb);