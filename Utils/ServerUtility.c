#include "include/ServerUtility.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "Practical.h"
#include "GetAddrInfo.h"

//Maximum outstanding conections allowed
#define MAX_PENDING 5

int SetupTCPServerSocket(const char *service) {
    //Construct the server address structure
    struct addrinfo addrCriteria;   //Criteria for address match
    memset(&addrCriteria, 0, sizeof(addrCriteria)); //Zero out structure
    addrCriteria.ai_family = AF_UNSPEC; //Any address family
    addrCriteria.ai_flags = AI_PASSIVE; // Accept on any address/port
    addrCriteria.ai_socktype = SOCK_STREAM; //Only socket stream
    addrCriteria.ai_protocol = IPPROTO_TCP; //Only TCP protocol
    
    struct addrinfo* servAddr; //List of server addresses
    int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
    if (rtnVal != 0) {
        DieWithUserMessage("gettaddrinfo() failed", gai_strerror(rtnVal));
    }
    
    int servSock = -1;
    for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr -> ai_next) {
        //Create TCP socket
        servSock = socket(servAddr -> ai_family, servAddr -> ai_socktype, servAddr -> ai_protocol);
        if (servSock < 0) {
            continue; //Socket creation failed; try next address
        }
        
        //Bind to the local address and set socket to listening
        if ( (bind(servSock, servAddr->ai_addr, servAddr -> ai_addrlen) == 0) && (listen(servSock, MAX_PENDING) == 0) ) {
            //Print local address of the socket
            struct sockaddr_storage localAddress; //sockaddr_storage because we dont know ipv4 or ipv6
            socklen_t addrSize = sizeof(localAddress); //In-out parameter
            if (getsockname(servSock, (struct sockaddr *)&localAddress, &addrSize) < 0) {
                DieWithSystemMessage("getsockname() failed");
            }
            printf("Binding to ");
            PrintSocketAddress((struct sockaddr*)&localAddress, stdout);
            fputc('\n', stdout);
            break;
        }
        
        close(servSock); //Close and try again
        servSock = -1;
    }
    
    //free address list allocated by getaddrinfo()
    freeaddrinfo(servAddr);
    return servSock;
}

int AcceptTCPConnection(int servSock) {
    struct sockaddr_storage clntAddr; //Could by ipv4 or ipv6
    //Length client addr storage (in-out parameter)
    socklen_t clntAddrLen = sizeof(clntAddr);
    
    //Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &clntAddrLen);
    if (clntSock < 0) {
        DieWithSystemMessage("accept() failed");
    }
    
    //clntSock is connected to the client
    fputs("Handling client ", stdout);
    PrintSocketAddress((struct sockaddr*)&clntAddr, stdout);
    fputc('\n', stdout);
    
    return clntSock;
}
