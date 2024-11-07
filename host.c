#include "host.h"
#include "message_queue.h"
#include "encrypter.h"

// Extern declaration of the message queue
extern MessageQueue* g_messageQueue;

void InitializeHost(HOST_PAGE* host) {
    // Initialize the message queue
    g_messageQueue = CreateMessageQueue();

    unsigned char key[16] = "your-16-byte-key";
    unsigned char iv[16] = "your-16-byte-ivv";
    Encrypter_Init(key, sizeof(key), iv, sizeof(iv));

    char ipAddr[IP_LENGTH];
    GetActiveIp(ipAddr, sizeof(ipAddr));
    printf("Plain ip: %s\n", ipAddr);
    host->CodeBuffer = EncryptIP(ipAddr);
    printf("Encrypted: %s\n", host->CodeBuffer);

    // Open a connection to the server
    int socket = HostConnection(SERVER_PORT, g_messageQueue);
    if (socket < 0) {
        fprintf(stderr, "Failed to host connection.\n");
        // Handle error
    }

    // Store socket if needed
    host->ServerSocket = socket;

    // Initialize GUI components
    host->BackButton = CreateButton(50, GetScreenHeight() - 100, 150, 50, DARKGRAY, BLACK, "Back");

    host->ClientTextBox = CreateTextBox(250, GetScreenHeight() - 100, 500, 50, DARKGRAY, BLACK);

    host->ClientTextField = CreateTextField(100, 100, 800, 400, DARKGRAY, BLACK);

    host->SendButton = CreateButton(850, GetScreenHeight() - 100, 150, 50, DARKGRAY, BLACK, "Send");

    host->ClientListView = CreateClientList(950, 100, 300, 400, LIGHTGRAY, BLACK);

    AddClientToList(&host->ClientListView, "Ip address", "Big Red");

}

int UpdateHost(HOST_PAGE* host) {
    UpdateTextBox(&host->ClientTextBox);

    if(CheckButton(&host->BackButton))
        return 1;

    if(CheckButton(&host->SendButton) || IsKeyPressed(KEY_ENTER)) {
        // Send message to clients (if implemented)
        // For now, we can add the message to our own text field
        SendMessageToClients(host->ClientTextBox.text, -1);
        AddMessageToTextField(&host->ClientTextField, "Server", host->ClientTextBox.text);
        SetTextBoxText(&host->ClientTextBox, "\0");
    }


    UpdateTextFieldScroll(&host->ClientTextField);
    UpdateClientListScroll(&host->ClientListView);

    // Dequeue messages from clients and add them to the text field
    char* message;
    while ((message = DequeueMessage(g_messageQueue)) != NULL) {
        AddMessageToTextField(&host->ClientTextField, "Client", message);
        free(message);
    }

    CLIENT_CONNECTION* Clients = GetCLientList();
    int ClientCount = GetClientCount();
    for(int i = 0; i < ClientCount; i++){
        if(Clients[i].SocketId != 0){
            AddClientToList(&host->ClientListView, "Ip", "Username");
        }
    }

    return 0;
}

void DrawHost(const HOST_PAGE* host) {
    BeginDrawing();

    ClearBackground(LIGHTGRAY);

    char Title[50];
    int clientCount = GetClientCount();
    snprintf(Title, sizeof(Title), "Hosting %s", host->CodeBuffer,clientCount);


    // Title
    DrawText(Title, (25) , (25), 50, BLACK);

    // Draw GUI components
    DrawButton(host->BackButton);
    DrawTextBox(host->ClientTextBox);
    DrawTextField(host->ClientTextField);
    DrawButton(host->SendButton);
    DrawClientList(host->ClientListView);

    EndDrawing();
}

void UnInitializeHost(HOST_PAGE* host) {
    DestroyTextField(&host->ClientTextField);

    // Clean up the message queue
    DestroyMessageQueue(g_messageQueue);
    g_messageQueue = NULL;  // Optional, but good practice


    // TODO: Signal the server thread to stop if necessary
    CloseConnection(host->ServerSocket); // If you have access to the server socket
}
