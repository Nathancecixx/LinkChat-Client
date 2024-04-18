/*********************************************************************************************************************
 * Nathan Ceci ~ 2024 ~ LinkChat *
 *********************************
 *
 * ~ This file contains functions that deal with the client to server connection.
 *
 * ~ Uses uniform packets to communicate.
 *
 *
 * Command Identifiers:
 *                      '@' Message
 *                      '$' Room Change
 *                      '%' Login Details
 *                      '!' Edit account info
 *                      '+' Create new account
 *
 * Packet Template: "(Identifier)(data)"
 *
 * Example: "@Hello World"
 *           ^
 *           Command identifying a message followed by the message
 *
 *********************************************************************************************************************/
#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "Ws2_32.lib")
    #define CLOSESOCKET closesocket
    #define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)

#else
    #include <unistd.h>
    #include <netinet/in.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #define CLOSESOCKET close
    #define ISVALIDSOCKET(s) ((s) >= 0)
#endif


#include <string.h>
#include "networkmanager.h"

int OpenConnection(int port, char* ip){

    //Needed for sockets to work on windows
    #ifdef _WIN32
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) {
             fprintf(stderr, "Failed to initialize.\n");
             return 1;
        }
    #endif

    //Create Socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    //server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_addr.s_addr = inet_addr(ip);

    //Connect to server address
    int connection_status = connect(network_socket,
                                    (struct sockaddr*) &server_address,
                                    sizeof(server_address));

    //Check connection
    if(connection_status == -1) {
        printf("Error making connection to server...\n");
        return -1;
    }

    //Return the socket number that is now connected to the server
    return network_socket;
}

bool SendOverConnection(int Socket, char* Buffer){
    if(send(Socket, Buffer, 80, 0) != 80){
        printf("Failed to send packet to server.\n");
        return false;
    }

    return true;
}

bool CheckSocketActivity(int Socket){
    // Non-blocking check for incoming message
    fd_set readfds;
    struct timeval tv;

    FD_ZERO(&readfds);
    FD_SET(Socket, &readfds);

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int activity = select(Socket + 1, &readfds, NULL, NULL, &tv);

    if(activity < 0){
        fprintf(stderr, "Select error\n");
        return false;
    }

    if(FD_ISSET(Socket, &readfds)){
        return true;
    } else{
        return false;
    }
}

bool ReceiveOverConnection(int Socket, char* DestBuffer){

    char recv_buffer[25];

    // Recieve the packet over socket
    int bytes_received = recv(Socket, recv_buffer, sizeof(recv_buffer) - 1, 0);

    //Checks if bytes where recieved and if so copy into buffer pointer
    if (bytes_received > 0) {
        recv_buffer[bytes_received] = '\0'; // Null-terminate the message
        printf("INCOMING MESSAGE: %s\n", recv_buffer);
        strncpy(DestBuffer, recv_buffer, 25);
        return true;
    } else if (bytes_received == 0) {
        printf("Server closed the connection.\n");
        return false;
    } else {
        perror("ERROR: recv error\n");
        return false;
    }
}

bool CloseConnection(int Socket){

#ifdef _WIN32
    shutdown(Socket, SD_SEND);
    if(closesocket(Socket) != 0)
        return false;
    else
        return true;
#else
    shutdown(Socket, SHUT_WR);
    if(close(Socket) != 0)
        return false;
    else
        return true;
#endif
}
