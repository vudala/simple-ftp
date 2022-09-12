#include "message.h"

#include <stdlib.h>
#include <string.h>
#include <bits/stdc++.h>

using namespace std;

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
        memcpy(&this->data, data, size + 1);
    this->crc = _crc(this);
}

bool valid_msg(Message * m)
{
    return _crc(m) == m->crc;
}


string data_to_str(Message * m)
{
    string str;
    for(int i = 0; i < m->size; i++)
        str.push_back(m->data[i]);
    str.push_back('\0');
    return str;
}