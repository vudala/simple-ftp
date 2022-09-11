#include "message.h"

#include <stdlib.h>
#include <string.h>

int _crc(Message * m)
{
    return 1;
}

Message::Message() {}

Message::Message (unsigned size, unsigned seq, unsigned type, char * data)
{
    this->mark = MARKER;
    this->size = size;
    this->seq = seq;
    this->type = type;
    if (data)
        memcpy(&this->data, data, size);
    this->crc = _crc(this);
}

bool valid_msg(Message * m)
{
    return _crc(m) == m->crc;
}