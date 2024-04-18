
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//10.192.57.66

#define SERVER_IP       "127.0.0.1"
#define SERVER_PORT     9002


int OpenConnection(int port, char* ip);

bool SendOverConnection(int Socket, char* Buffer);

bool ReceiveOverConnection(int Socket, char* DestBuffer);

bool CheckSocketActivity(int Socket);

bool CloseConnection(int socket);
