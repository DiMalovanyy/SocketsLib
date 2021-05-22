#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>


//Sockets
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Some utils for both server and client
#include "echo_utils.h"

#define ErrorExit(x) {perror(x); exit(EXIT_FAILURE); }
#define MAX_PEDDING 5


void HandleTCPClient( int clntSocket ) {
    char clientBuffer[BUFSIZ];
    
    //Receive msg from client
    ssize_t numBytesReceived = recv(clntSocket, clientBuffer, sizeof(clientBuffer), 0);
    if (numBytesReceived < 0) {
        ErrorExit("receive error");
    }
    
    //Send received string and receive again
    while (numBytesReceived > 0) {
        //Echo message back
        int numBytesSent = send(clntSocket, clientBuffer, sizeof(clientBuffer), 0);
        if (numBytesSent < 0) {
            ErrorExit("sending error");
        } else if (numBytesSent != numBytesReceived) {
            ErrorExit("sending unexpected bytes amount");
        }
        
        //See if there more data to receive
        numBytesReceived = recv(clntSocket, clientBuffer, sizeof(clientBuffer), 0);
        if (numBytesReceived < 0) {
            ErrorExit("received");
        }
        //Next client iteration
    }
    
    //Close client socket
    close(clntSocket);
}

int main( int argc, char ** argv) {
    
    //Validate input params
    if (argc > 2) {
        dprintf(STDOUT_FILENO, "Parameters: [<port>]");
        exit(EXIT_FAILURE);
    }
    
    //Setup server port
    in_port_t serverPort = (argc == 2) ? atoi(argv[1]) : SERVER_PORT;
    
    //Create socket for incomming connections
    int servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (servSock < 0) {
        ErrorExit("socket creation");
    }
    
    //Construct local address descriptor
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(serverPort);
    
    //Bind socket to the local address
    if (bind(servSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0 ) {
        ErrorExit("binding");
    }
    
    //Mark servSocket as listeing socket
    if (listen(servSock, MAX_PEDDING) < 0) {
        ErrorExit("listening");
    }
    
    //Infinity loop
    for(;;) {
        struct sockaddr_in clientAddr; //Client address descriptor
        
        //Wait client connection
        int clientSock = accept(servSock, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
        if (clientSock < 0) {
            ErrorExit("accept failed");
        }
        
        char clientName[INET_ADDRSTRLEN]; //String to contain client addr
        if (inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, clientName,
                      sizeof(clientName)) != NULL) {
           printf("Handling client %s/%d\n", clientName, ntohs(clientAddr.sin_port));
        } else {
           puts("Unable to get client address");
        }
        //Handling TCP socket
        HandleTCPClient(clientSock);
    }
}

