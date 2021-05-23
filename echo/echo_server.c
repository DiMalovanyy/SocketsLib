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

#include <ServerUtility.h>
#include <Practical.h>

void HandleTCPClient( int clntSocket ) {
    char clientBuffer[BUFSIZ];
    
    //Receive msg from client
    ssize_t numBytesReceived = recv(clntSocket, clientBuffer, sizeof(clientBuffer), 0);
    if (numBytesReceived < 0) {
        DieWithSystemMessage("recv() failed");
    }
    
    //Send received string and receive again
    while (numBytesReceived > 0) {
        //Echo message back
        int numBytesSent = send(clntSocket, clientBuffer, numBytesReceived, 0);
        if (numBytesSent < 0) {
            DieWithSystemMessage("send() failed");
        } else if (numBytesSent != numBytesReceived) {
            DieWithUserMessage("send() failed", "sent unexpected amount of bytes");
        }
        
        //See if there more data to receive
        numBytesReceived = recv(clntSocket, clientBuffer, sizeof(clientBuffer), 0);
        if (numBytesReceived < 0) {
            DieWithSystemMessage("send() failed");
        }
        //Next client iteration
    }
}

int main( int argc, char ** argv) {
    
    //Validate input params
    if (argc > 2) {
        DieWithUserMessage("Parameter(s)", "[<server Port/Service>]" );
    }
    
    //Setup server port
    char *service = (argc == 2) ? argv[1] : "echo";
    
    //Create socket for incomming connections
    int servSock = SetupTCPServerSocket(service);
    if (servSock < 0) {
        DieWithUserMessage("SetupTCPServerSocket() failed", service);
    }
    
    //Infinity loop
    for(;;) {
        //New connection creates a connnected client socket
        int clntSock = AcceptTCPConnection(servSock);
        if (clntSock < 0) {
            DieWithUserMessage("AcceptTCPConnection() failed", "failed to create connection");
        }
        
        //Handling TCP socket
        HandleTCPClient(clntSock);
        close(clntSock);
    }
}

