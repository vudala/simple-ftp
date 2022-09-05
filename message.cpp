#include "message.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

int _crc(Message * m)
{
    return 1;
}

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
    if (!m) return false;
    return _crc(m) == m->crc;
}