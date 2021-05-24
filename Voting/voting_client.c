#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>

//Our lib params
#include <Practical.h>
#include <ClientUtility.h>

//utils
#include "utils/VoteProtocol.h"
#include "utils/Framer.h"
#include "utils/VoteEncoding.h"

enum {
    MAX_CANDIDATE = 1000
};

int main(int argc, char *argv[]) {
    
    if (argc < 4 || argc > 5) {
        DieWithUserMessage("Parameter(s)", "<Server> <Port/Service> <Candidate> [I]");
    }
    
    char *server = argv[1]; //First arg: server address/name
    char *service = argv[2]; //Second arg: port/service
    
    int candidate = atoi(argv[3]); //Third arg: candidate
    if (candidate < 0 || candidate > MAX_CANDIDATE) {
        DieWithUserMessage("Cadnidate # not valid", argv[3]);
    }
    
    bool inq = argc > 4 && strcmp(argv[4], "I") == 0;
    
    //Create a connecting TCP socket using our lib
    int sock = SetupTCPClientSocket(server, service);
    if (sock < 0) {
        DieWithUserMessage("SetupTCPClientSocket() error", "unable to connect");
    }
    
    //Create socket file descriptor
    FILE *str = fdopen(sock, "r+"); //Wrap for stream I/O
    if (str == NULL) {
        DieWithSystemMessage("fdopen() failed");
    }
    
    //Set up for the request
    VoteInfo vi;
    memset(&vi, 0, sizeof(vi));
    
    vi.isInquiry = inq;
    vi.candidate = candidate;
    
    //Encode for transmission
    uint8_t outbuf[MAX_WIRE_SIZE];
    size_t reqSize = Encode(&vi, outbuf, MAX_WIRE_SIZE);
    
    //Print some info
    print("Sending %d-byte %s for candidate %d...\n", reqSize, (inq ? "inquiry" : "vote"), candidate);
    
    //Frame and send
    if (PutMsg(outbuf, reqSize, str) < 0) {
        DieWithSystemMessage("PutMsg() failed");
    }
    
    //Receive and print response
    uint8_t inbuf[MAX_WIRE_SIZE];
    size_t respSize = GetNextMsg(str, inbuf, MAX_WIRE_SIZE); //Get the message
    if (Decode(inbuf, respSize, %vi)) { //Parse it
        printf("Received:\n");
        if (vi.isResponse) {
            printf("Response to ");
        }
        if (vi.isInquiry) {
            printf("inquiry ");
        } else {
            printf("vote ");
        }
        printf("for candidate %d\n", vi.candidate);
        if (vi.isResponse) {
            printf(" count = %llu\n", vi.count);
        }
    }
    
    //Close up
    fclose(str);
    exit(EXIT_SUCCESS);
    
}
