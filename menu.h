#ifndef LINKCHAT_MENU_H
#define LINKCHAT_MENU_H

#include "raylib.h"
#include "button.h"

#include <stdio.h>

#define VERSION     "1.1.0"

#define BTN_COUNT 3

typedef struct {
    BUTTON buttons[BTN_COUNT];

}MAIN_MENU;

void InitializeMainMenu(MAIN_MENU* menu);

int UpdateMainMenu(MAIN_MENU* menu);

void DrawMainMenu(const MAIN_MENU* menu);

void UnInitializeMainMenu(MAIN_MENU* menu);

#endif