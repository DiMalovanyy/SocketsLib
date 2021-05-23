#ifndef GET_ADDR_INFO_H
#define GET_ADDR_INFO_H

#include <stdio.h>
#include <sys/socket.h>

void PrintSocketAddress(const struct sockaddr *address, FILE* stream);
void GetAddrInfo(char *addrString, char *portString);

#endif
