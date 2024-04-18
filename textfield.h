#include <stdbool.h>

#include "raylib.h"

#define FONT_SIZE   50
#define MAX_MESSAGE_LENGTH      25
#define MAX_NAME_LENGTH     25

typedef struct listnode{
    char Name[MAX_NAME_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
    struct listnode* next;
} LISTNODE, * PLISTNODE;

typedef struct text_field {
    Rectangle rect;
    Color rectColor;
    Color textColor;
    int listCount;
    int scrollOffset;
    PLISTNODE head;
}TEXT_FIELD;

//C
TEXT_FIELD CreateTextField(float X, float Y, float Width, float Height, Color RectColor, Color TextColor);

//R
Rectangle GetTextFieldRectangle(TEXT_FIELD tf);
Color GetTextFieldRectColor (TEXT_FIELD tf);
Color GetTextFieldTextColor (TEXT_FIELD tf);
char* GetTextFieldText (TEXT_FIELD tf);

void DrawTextField(TEXT_FIELD tf);

//U
bool SetTextFieldRectangle(TEXT_FIELD* tf, float X, float Y, float Width, float Height);
bool SetTextFieldRectColor (TEXT_FIELD* tf, Color c);
bool SetTextFieldTextColor (TEXT_FIELD* tf, Color c);
bool SetTextFieldText (TEXT_FIELD* tf, char* Text);
bool ClearTextField (TEXT_FIELD* tf);

bool UpdateTextFieldScroll(TEXT_FIELD* tf);
bool AddMessageToTextField(TEXT_FIELD* tf, char* Name, char* Message);

//D
void DestroyTextField(TEXT_FIELD* tf);
