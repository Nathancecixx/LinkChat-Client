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
#include "networkmanager.h"

#define PAGE_COUNT  3

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


    MAIN_MENU menu;
    HOST_PAGE host;

    //Set the screen to login page
    InitializeMainMenu(&menu);
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
                result = UpdateMainMenu(&menu);
                if(result == 1) {
                    UnInitializeMainMenu(&menu);
                    InitializeHost(&host);
                    CurrentScreen = HOST;
                }
                else if(result == 2){
                    UnInitializeMainMenu(&menu);

                    CurrentScreen = JOIN;
                }
                else if(result == 3){
                    UnInitializeMainMenu(&menu);
                    UserQuit = true;
                }
                break;
            }

            case HOST:{
                result = UpdateHost(&host);
                if(result == 1){
                    UnInitializeHost(&host);
                    InitializeMainMenu(&menu);
                    CurrentScreen = MENU;
                }
                break;
            }

            case JOIN:{

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
                DrawMainMenu(&menu);
                break;
            }
            case HOST:{
                DrawHost(&host);
                break;
            }
            case JOIN:{

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
    UnInitializeMainMenu(&menu);
    UnInitializeServer();
    //CloseConnection(server);
    //--------------------------------------------------------------------------------------

	return 0;
}