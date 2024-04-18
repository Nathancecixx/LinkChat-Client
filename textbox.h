#include <stdbool.h>

#include "raylib.h"

#define MAX_MESSAGE_LENGTH     25
#define FONT_SIZE   50


typedef struct text_box {
    Rectangle rect;
    Color rectColor;
    Color textColor;
    int charCount;
    char text [MAX_MESSAGE_LENGTH];
    bool IsSelected;
}TEXT_BOX;


//C
TEXT_BOX CreateTextBox(float X, float Y, float Width, float Height, Color RectColor, Color TextColor);

//R
Rectangle GetTextBoxRectangle(TEXT_BOX tb);
Color GetTextBoxRectColor (TEXT_BOX tb);
Color GetTextBoxTextColor (TEXT_BOX tb);
char* GetTextBoxText (TEXT_BOX tb);

void DrawTextBox(TEXT_BOX tb);

//U
bool SetTextBoxRectangle(TEXT_BOX* tb, float X, float Y, float Width, float Height);
bool SetTextBoxRectColor (TEXT_BOX* tb, Color c);
bool SetTextBoxTextColor (TEXT_BOX* tb, Color c);
bool SetTextBoxText (TEXT_BOX* tb, char* Text);
bool ClearTextBox(TEXT_BOX* tb);

bool CheckTextBox(TEXT_BOX* tb);
bool UpdateTextBox(TEXT_BOX* tb);
bool CheckTextBox(TEXT_BOX* tb);

//D
void DestroyTextBox(TEXT_BOX* tb);
