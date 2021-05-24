#include "BruteForceCoding.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <Practical.h>


//Test values
const uint8_t val8 = 101; // One hundred and one
const uint16_t val16 = 10001; // Ten thousand and one
const uint32_t val32 = 100000001; // One hundred million and one
const uint64_t val64 = 1000000000001L; // One trillion and one
const int MESSAGELENGTH = sizeof(uint8_t) + sizeof(uint16_t) + sizeof(uint32_t)
    + sizeof(uint64_t);


static char stringBuf[BUFSIZ];
char *BytesToDecString(uint8_t *byteArray, int arrayLength) {
    char *cp = stringBuf;
    ssize_t bufSpaceLeft = BUFSIZ;
    for (int i = 0; i < arrayLength && bufSpaceLeft > 0; i++) {
        int strl = snprintf(cp, bufSpaceLeft, "%u ", byteArray[i]);
        bufSpaceLeft -= strl;
        cp += strl;
    }
    return stringBuf;
}

int EncodeIntBigEndian( uint8_t dst[], uint64_t val, int offset, int size) {
    for (int i = 0; i < size; i++) {
        dst[offset++] = (uint8_t) (val >> ((size -1) - i) * CHAR_BIT);
    }
    return offset;
}

uint64_t DecodeIntBigEndian( uint8_t val[], int offset, int size) {
    uint64_t rtn = 0;
    for (int i = 0; i < size; i++) {
        rtn = (rtn << CHAR_BIT) | val[offset + i];
    }
    return rtn;
}

