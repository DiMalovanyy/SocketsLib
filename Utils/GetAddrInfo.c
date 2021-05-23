#include "include/GetAddrInfo.h"

#include <stdlib.h>
#include <string.h>
#include <netdb.h> //For DNS service

#include <arpa/inet.h>

#include "Practical.h"

void PrintSocketAddress(const struct sockaddr *address, FILE* stream) {
   //Test for addr and stream
    if (address == NULL || stream == NULL) {
        return;
    }
    void *numericAddress; //Pointer to binary address
    //Buffer to hold result
    char addrBuffer[INET6_ADDRSTRLEN]; //(Ipv6 sufficient to hold IPv4)
    in_port_t port; //Port holder
    
    //Set pointer to address based on address family
    switch (address -> sa_family) {
        case AF_INET:
            numericAddress = &((struct sockaddr_in* )address) -> sin_addr;
            port = ntohs(((struct sockaddr_in*) address) -> sin_port);
            break;
        case AF_INET6:
            numericAddress = &((struct sockaddr_in6* )address) -> sin6_addr;
            port = ntohs(((struct sockaddr_in6*) address) -> sin6_port);
            break;
        default:
            fputs("[unknown type]", stream); // Unhandled type
            break;
    }
    
    //Convert binary to printable
    if (inet_ntop(address -> sa_family, numericAddress, addrBuffer, sizeof(addrBuffer)) == NULL) {
        fputs("[invalid address]", stream); //Unable to convert
    } else {
        fprintf(stream, "%s", addrBuffer);
        if (port != 0) {
            fprintf(stream, ":%u", port);
        }
    }
}

void GetAddrInfo(char *addrString, char *portString) {
    
    //Tell the system what king of adress we want
    struct addrinfo addrCriteria;
    memset(&addrCriteria, 0, sizeof(addrCriteria)); //Zero out structure
    addrCriteria.ai_family = AF_UNSPEC; //Any addr family
    addrCriteria.ai_socktype = SOCK_STREAM; //Only stream socket
    addrCriteria.ai_protocol = IPPROTO_TCP; //Only TCP protocol
    
    //get adress(es) associated with the specified name/service
    struct addrinfo* addrList; //Holder for list of addresses returned
    //Modify servAddr contents to reference linked list of addresses
    int rtnVal = getaddrinfo(addrString, portString, &addrCriteria, &addrList);
    if (rtnVal != 0) {
        DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));
        
        //Display returned addresses
        for ( struct addrinfo* addr = addrList; addr != NULL; addr = addr -> ai_next) {
            PrintSocketAddress( addr->ai_addr, stdout);
            fputc('\n', stdout);
        }

        freeaddrinfo(addrList); //Free addrinfo allocated in getaddrinfo()
    }
}
