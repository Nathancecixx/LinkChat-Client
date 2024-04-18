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
#include "networkmanager.h"

//-------------------------------------------------------------------------------------------
// Global Variables


//Screen Variables
typedef enum {MAIN_MENU, LOGIN, P2P, SERVER}SCREEN;
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

    //Set the screen to login page
    InitializeLogin();
    CurrentScreen = LOGIN;

    //Open connection to server
    server = OpenConnection(SERVER_PORT, SERVER_IP);

    int result;
    bool UserQuit = false;


    //--------------------------------------------------------------------------------------

    // Main loop
    while (!WindowShouldClose() && !UserQuit)    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch (CurrentScreen) {

            case LOGIN: {
                result = UpdateLogin(server);
                if(result == 1){
                    UnInitializeLogin();
                    InitializeMainMenu();
                    CurrentScreen = MAIN_MENU;
                }
                if(result == 2){
                    UnInitializeLogin();
                    UserQuit = true;
                }
                break;
            }

            case MAIN_MENU:{
                result = UpdateMainMenu();
                if(result == 1) {
                    UnInitializeMainMenu();
                    InitializeP2P();
                    CurrentScreen = P2P;
                }
                else if(result == 2){
                    UnInitializeMainMenu();
                    InitializeServer();
                    CurrentScreen = SERVER;
                }
                else if(result == 3){
                    UnInitializeMainMenu();
                    UserQuit = true;
                }
                break;
            }

            case P2P:{
                result = UpdateP2P();
                if(result == 1){
                    UnInitializeP2P();
                    InitializeMainMenu();
                    CurrentScreen = MAIN_MENU;
                }
                break;
            }

            case SERVER:{
                result = UpdateServer(server);
                if(result == 1){
                    UnInitializeServer();
                    InitializeMainMenu();
                    CurrentScreen = MAIN_MENU;
                }
                break;
            }

        }
        //----------------------------------------------------------------------------------



        // Draw
        //----------------------------------------------------------------------------------
        switch (CurrentScreen) {

            case LOGIN: {
                DrawLogin(server);
                break;
            }
            case MAIN_MENU:{
                DrawMainMenu();
                break;
            }
            case P2P:{
                DrawP2P();
                break;
            }
            case SERVER:{
                DrawServer();
                break;
            }

        }
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    UnInitializeMainMenu();
    UnInitializeServer();
    CloseConnection(server);
    //--------------------------------------------------------------------------------------

	return 0;
}