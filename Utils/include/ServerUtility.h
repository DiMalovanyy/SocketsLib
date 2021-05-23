#ifndef SERVER_UTILITY_H
#define SERVER_UTILITY_H


int SetupTCPServerSocket(const char *service);
int AcceptTCPConnection(int servSock);

#endif
