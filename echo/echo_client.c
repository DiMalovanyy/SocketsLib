#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

//Sockets includes
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Some utils for both server and client
#include "echo_utils.h"

#define ErrorExit(x) {perror(x); exit(EXIT_FAILURE); }


int main (int argc, char ** argv) {

    // Usage
    if (argc < 3 || argc > 4) { //Test input correctness
        printf("Parameters: <Server address> <echo string> [<server port>]");
        exit(EXIT_FAILURE);
    }
    
    char *serverIP = argv[1]; //First arg server Ip
    char *echoString = argv[2]; //Second arg: echo string
    
    //Setup optional server port OR utils port
    in_port_t serverPort = (argc == 4) ? atoi(argv[3]) : SERVER_PORT;
    
    //Create reliable socket via (TCP)
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        ErrorExit("Socket creation");
    }
    
    //Create server address descriptor
    struct sockaddr_in servAddr; //Server addr
    memset(&servAddr, 0, sizeof(servAddr)); //Zero out structure
    servAddr.sin_family = AF_INET;          //IpV4 address family
    //Convert address
    int portRtv = inet_pton(AF_INET, serverIP, &servAddr.sin_addr.s_addr);
    if (portRtv == 0) {
        ErrorExit("Invalid address param");
    } else if (portRtv < 0) {
        ErrorExit("inet_pton() failed");
    }
    servAddr.sin_port = htons(serverPort); // Server port
    
    
    //Establish connection between client server
    if (connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0 ) {
        ErrorExit("connection error");
    }
    
    //Determine string len
    ssize_t echoStringLen = strlen(echoString);
    
    //Send the string tothe server
    ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
    if (numBytes < 0) {
        ErrorExit("sending error");
    } else {
        ErrorExit("send invalid amount of bytes");
    }
    
    //Recived the same string back from the server
    ssize_t totalBytesReceived = 0;
    fputs("Received: ", stdout);
    while (totalBytesReceived < echoStringLen) {
        char buffer[BUFSIZ]; // I/O buffer
        
        //Receive up to the buffer size (minus 1 to leave space for
        //   a null terminator) bytes from the sender
        
        numBytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (numBytes < 0) {
            ErrorExit("recv failed");
        } else if (numBytes == 0) {
            ErrorExit("connection closed permatently");
        }
        
        totalBytesReceived += numBytes;
        buffer[numBytes] = '\0';
        fputs(buffer, stdout);
    }
    
    fputc('\n', stdout);
    
    //Closing socket
    close(sock);
    exit(EXIT_SUCCESS);
}
