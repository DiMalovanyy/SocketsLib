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


//libutils.a
#include <Practical.h>
#include <ClientUtility.h>


int main (int argc, char ** argv) {
//    SetupTCPClientSocket("", "");
    // Usage
    if (argc < 3 || argc > 4) { //Test input correctness
        DieWithUserMessage("Parameter(s)", "<Server address/Name> <Echo Word> [<Server Port/Service>]");
    }
    
    char *serverIP = argv[1]; //First arg server Ip
    char *echoString = argv[2]; //Second arg: echo string
    //Third arg (optional): server port/service;
    char *service = (argc == 4) ? argv[3] : "echo";
    
    //Create connection TCP Socket
    int sock = SetupTCPClientSocket(serverIP, service);
    if (sock < 0) {
        DieWithUserMessage("SetupTCPClientSocketFailed", "unable to connect");
    }
    
    ssize_t echoStringLen = strlen(echoString); //Determine input length
    
    //Send the string tothe server
    ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
    if (numBytes < 0) {
        DieWithSystemMessage("send() failed");
    } else if (numBytes != echoStringLen ){
        DieWithUserMessage("send()", "send unexpected number of bytes");
    }
    
    
    //Recived the same string back from the server
    ssize_t totalBytesReceived = 0; //Count of total bytes received
    fputs("Received: ", stdout);
    while (totalBytesReceived < echoStringLen) {
        char buffer[BUFSIZ]; // I/O buffer
        
        //Receive up to the buffer size (minus 1 to leave space for
        //   a null terminator) bytes from the sender
        numBytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (numBytes < 0) {
            DieWithSystemMessage("recv() failed");
        } else if (numBytes == 0) {
            DieWithUserMessage("recv()", "conection closed permaturely");
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
