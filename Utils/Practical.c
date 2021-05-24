#include "Practical.h"


#include <stdio.h>
#include <stdlib.h>

static const long serverPort = 8080;

void DieWithUserMessage(const char* msg, const char *details) {
    printf("%s: %s\n", msg, details);
    exit(EXIT_FAILURE);
}

void DieWithSystemMessage(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
