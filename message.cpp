#include "message.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

int crc(Message * m)
{
    return 1;
}

Message * new_msg(unsigned size, unsigned seq, unsigned type, char * data)
{
    if (size > 64) return NULL;

    Message * m = new Message;

    m->mark = MARKER;
    m->size = size;
    m->seq = seq;
    m->type = type;
    if (data)
        memcpy(&m->data, data, size);
    m->crc = crc(m);

    return m;
}

bool valid_msg(Message * m)
{
    return crc(m) == m->crc;
}