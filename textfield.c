#include <stdlib.h>
#include <string.h>

#include "textfield.h"


#define MESSAGE_OFFSET_Y      30

//C
TEXT_FIELD CreateTextField(float X, float Y, float Width, float Height, Color RectColor, Color TextColor) {
    TEXT_FIELD tf;

    Rectangle tmpRect = {X, Y, Width, Height};
    tf.rect = tmpRect;

    tf.rectColor = RectColor;
    tf.textColor = TextColor;
    tf.head = NULL;
    tf.listCount = 0;
    tf.scrollOffset = 0;

    return tf;
}

//R
Rectangle GetTextFieldRectangle(TEXT_FIELD tf){
    return tf.rect;
}

Color GetTextFieldRectColor (TEXT_FIELD tf){
    return tf.rectColor;
}

Color GetTextFieldTextColor (TEXT_FIELD tf){
    return tf.textColor;
}

//TODO
char* GetTextFieldText (TEXT_FIELD tf){
    return NULL;
}

void DrawTextField(TEXT_FIELD tf){
    DrawRectangleRounded(tf.rect, 2, 2, LIGHTGRAY);
    DrawRectangleLinesEx(tf.rect, 2, tf.rectColor);

    int Font_Size = 25;

    PLISTNODE current = tf.head;

    //Skip messages based on scroll offset
    for(int i = 0; i < tf.scrollOffset; i++){
        if(current != NULL) current = current->next;
    }

   for(int i = 0; current != NULL && i < tf.listCount; i++){

       if(i > 11)
           return;
        DrawText(current->Name,
                      (int) (tf.rect.x + 5),
                      (int) (tf.rect.y + (tf.rect.height -  40) - (MESSAGE_OFFSET_Y * i)),
                      Font_Size,
                      tf.textColor);

        DrawText(current->message,
                      (int) (tf.rect.x + MeasureText(current->Name, Font_Size) + 20),
                      (int) (tf.rect.y + (tf.rect.height - 40) - (MESSAGE_OFFSET_Y * i)),
                      Font_Size,
                      tf.textColor);

        current = current->next;
    }


    /*
    DrawText(tf.text,
             (int) ((tf.rect.x + (tf.rect.width/2)) - (MeasureText(tf.text, Font_Size)/ 2)),
             (int) (tf.rect.y + (tf.rect.height/4)),
             Font_Size,
             tf.textColor);
             */

}

//U
//TODO
bool SetTextFieldRectangle(TEXT_FIELD* tf, float X, float Y, float Width, float Height);
bool SetTextFieldRectColor (TEXT_FIELD* tf, Color c);
bool SetTextFieldTextColor (TEXT_FIELD* tf, Color c);
bool SetTextFieldText (TEXT_FIELD* tf, char* Text);

bool UpdateTextFieldScroll(TEXT_FIELD* tf){
    if(tf == NULL)
        return false;
    //Return 1 if scrolled up and -1 scrolled down
    float mouseWheelMovement = GetMouseWheelMove();

    if(mouseWheelMovement == 1 && tf->scrollOffset < tf->listCount -1 ){
        tf->scrollOffset++;
    } else if(mouseWheelMovement == -1 && tf->scrollOffset > 0){
        tf->scrollOffset--;
    }

    return true;
}

bool AddMessageToTextField(TEXT_FIELD* tf, char* Name, char* Message){

    PLISTNODE NewNode = (PLISTNODE) malloc(sizeof(LISTNODE));
    if(NewNode == NULL) return false;

    strncpy(NewNode->Name, Name, MAX_NAME_LENGTH);
    strncpy(NewNode->message, Message, MAX_MESSAGE_LENGTH);

    NewNode->next = tf->head;
    tf->head = NewNode;

    tf->listCount++;

    return true;
}

//D
void DestroyTextField(TEXT_FIELD* tf){
    //Destroy message list
    PLISTNODE current = tf->head;

    while(current != NULL){
        PLISTNODE prev = current;
        current = current->next;
        free(prev);
    }
    tf->head = NULL;
}
