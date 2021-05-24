#ifndef VOTE_ENCODING_H
#define VOTE_ENCODING_H

#include "VoteProtocol.h"

size_t Encode(const VoteInfo *v, uint8_t *outBuf, const size_t bufSize);
bool Decode(uint8_t *inBuf, const size_t mSize, VoteInfo *v);

#endif
