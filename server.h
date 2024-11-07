#ifndef LINKCHAT_SERVER_H
#define LINKCHAT_SERVER_H

#include <stdbool.h>

void InitializeServer();

int UpdateServer(int server);

int DrawServer();

void UnInitializeServer();



void InitializeServerMenu();

int UpdateServerMenu(int server);

void DrawServerMenu();

void UnInitializeServerMenu();

#endif