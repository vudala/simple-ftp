#include "message.h"
#include "utils.h"

#include <stdlib.h>

int parity(Message * m)
{
    return 1;
}

Message * new_msg(unsigned size, unsigned seq, unsigned type, char * data)
{
    Message * m = (Message*) malloc(sizeof(Message));
    must_alloc(m);

    m->mark = MARKER;
    m->size = size;
    m->seq = seq;
    m->type = type;
    m->data = data;
    m->count = parity(m);

    return m;
}