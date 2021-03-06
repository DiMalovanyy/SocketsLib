#ifndef VOTE_ENCODING_H
#define VOTE_ENCODING_H

#include <stdint.h>
#include <stdlib.h>

#include "VoteProtocol.h"

size_t Encode(const VoteInfo *v, uint8_t *outBuf, const size_t bufSize);
bool Decode(uint8_t *inBuf, const size_t mSize, VoteInfo *v);

size_t EncodeB(VoteInfo *v, uint8_t *outBuf, size_t bufSize);
bool DecodeB(uint8_t *inBuf, size_t mSize, VoteInfo *v);

#endif
