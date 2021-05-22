#ifndef ECHO_UTILS_H
#define ECHO_UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define SERVER_PORT 8080

void DieWithUserMessage(const char* msg, const char* details) {
    dprintf(STDOUT_FILENO, "%s: %s\n", msg, details);
    exit(EXIT_FAILURE);
}

void DieWithSystemMessage(const char *msg ) {
    perror(msg);
    exit(EXIT_FAILURE);
}

#endif
