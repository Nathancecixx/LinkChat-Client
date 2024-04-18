#include "raylib.h"

#define MAX_BUTTON_TEXT     15
#define FONT_SIZE   50


typedef struct button {
    Rectangle rect;
    Color rectColor;
    Color textColor;
    char text [MAX_BUTTON_TEXT];
}BUTTON;


//C
BUTTON CreateButton(float X, float Y, float Width, float Height, Color RectColor, Color TextColor, char* Text);

//R
Rectangle GetButtonRectangle(BUTTON b);
Color GetButtonRectColor (BUTTON b);
Color GetButtonTextColor (BUTTON b);
char* GetButtonText (BUTTON b);

void DrawButton(BUTTON b);

//U
bool SetButtonRectangle(BUTTON* b, float X, float Y, float Width, float Height);
bool SetButtonRectColor (BUTTON* b, Color c);
bool SetButtonTextColor (BUTTON* b, Color c);
bool SetButtonText (BUTTON b, char* Text);

bool CheckButton(BUTTON* button);

//D
void DestroyButton(BUTTON b);