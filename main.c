/*********************************************************************************************************************
 * Nathan Ceci ~ 2024 ~ LinkChat *
 *********************************
 * LinkChat Messenger Client
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/



#include <stdbool.h>

#include "raylib.h"
#include "menu.h"
#include "p2p.h"
#include "server.h"
#include "login.h"
#include "host.h"
#include "join.h"
#include "networkmanager.h"

#include "message_queue.h"

MessageQueue* g_messageQueue;

//-------------------------------------------------------------------------------------------
// Global Variables


//Screen Variables
typedef enum {MENU, HOST, JOIN, CHAT}SCREEN;
SCREEN CurrentScreen;
const int screenWidth = 1200;
const int screenHeight = 650;

//Server Socket
int server;

//-------------------------------------------------------------------------------------------


int main(void){

// Initialization
    //--------------------------------------------------------------------------------------

    //Create the window
    InitWindow(screenWidth, screenHeight, "Link Chat");
    SetTargetFPS(60);

    char ip[16]; // For IPv4

    if (GetActiveIp(ip, sizeof(ip)) == 0) {
        printf("Local IP address used for outgoing connections: %s\n", ip);
    } else {
        fprintf(stderr, "Failed to retrieve local IP address.\n");
    }


    MAIN_MENU menuPage;
    HOST_PAGE hostPage;
    JOIN_PAGE joinPage;

    //Set the screen to login page
    InitializeMainMenu(&menuPage);
    CurrentScreen = MENU;

    //Open connection to server
    //server = OpenConnection(SERVER_PORT, SERVER_IP);

    int result;
    bool UserQuit = false;

    //--------------------------------------------------------------------------------------

    // Main loop
    while (!WindowShouldClose() && !UserQuit)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch (CurrentScreen) {
            case MENU:{
                result = UpdateMainMenu(&menuPage);
                if(result == 1) {
                    UnInitializeMainMenu(&menuPage);
                    InitializeHost(&hostPage);
                    CurrentScreen = HOST;
                }
                else if(result == 2){
                    UnInitializeMainMenu(&menuPage);
                    InitializeJoin(&joinPage);
                    CurrentScreen = JOIN;
                }
                else if(result == 3){
                    UnInitializeMainMenu(&menuPage);
                    UserQuit = true;
                }
                break;
            }

            case HOST:{
                result = UpdateHost(&hostPage);
                if(result == 1){
                    UnInitializeHost(&hostPage);
                    InitializeMainMenu(&menuPage);
                    CurrentScreen = MENU;
                }
                break;
            }

            case JOIN:{
                result = UpdateJoin(&joinPage);
                if(result == 1){
                    UnInitializeHost(&hostPage);
                    InitializeMainMenu(&menuPage);
                    CurrentScreen = MENU;
                }
                break;
            }

            case CHAT:{

                break;
            }

        }
        //----------------------------------------------------------------------------------



        // Draw
        //----------------------------------------------------------------------------------
        switch (CurrentScreen) {
            case MENU:{
                DrawMainMenu(&menuPage);
                break;
            }
            case HOST:{
                DrawHost(&hostPage);
                break;
            }
            case JOIN:{
                DrawJoin(&joinPage);
                break;
            }
            case CHAT:{

                break;
            }

        }
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    UnInitializeMainMenu(&menuPage);
    UnInitializeServer();
    //CloseConnection(server);
    //--------------------------------------------------------------------------------------

	return 0;
}