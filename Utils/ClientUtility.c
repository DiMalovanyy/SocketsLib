#include "include/ClientUtility.h"

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "Practical.h"

int SetupTCPClientSocket( const char* host, const char *service) {
    //Tell the system what kind of address we want
    
    struct addrinfo addrCriteria;                   //Criteria for address to match
    memset(&addrCriteria, 0, sizeof(addrCriteria)); //Zero out structure
    addrCriteria.ai_family = AF_UNSPEC;             //Ipv4 or Ipv6
    addrCriteria.ai_socktype = SOCK_STREAM;         //Only streaming sockets
    addrCriteria.ai_protocol = IPPROTO_TCP;         //Only tcp protocol
    
    //Get address(es)
    struct addrinfo *servAddr; //Holder for returned list of server address(es)
    int rtnVal = getaddrinfo(host, service, &addrCriteria, &servAddr);
    if (rtnVal != 0) {
        DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));
    }
    
    int sock = -1;
    for (struct addrinfo* addr = servAddr; addr != NULL; addr = addr -> ai_next) {
        //Create reliabble, stream socket using TCP
        sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sock < 0) {
            continue; // Socket creation failed try another one address
        }
        
        //Establish connection to the server
        if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0) {
            break;
        }
        close(sock); // socket connection failed; try next addr
        sock = -1;
    }
    
    freeaddrinfo(servAddr);
    return sock;
}
