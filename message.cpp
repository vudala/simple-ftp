#include "message.h"

#include <stdlib.h>
#include <string.h>
#include <bits/stdc++.h>

using namespace std;

unsigned char _crc(Message * m)
{
    unsigned char ret = 0x0;
    for(int i = 0; i < m->size; i++)
        ret ^= m->data[i];
    return ret;
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
    if ((rand() % 10) == 3) {
        cout << "erro "  << m->seq << "\n" <<  flush;
        return false;
    }
    else {
        cout << "aceito " <<  m->seq << "\n" << flush;
        return true;
    }
    return _crc(m) == m->crc;
}


string data_to_str(Message * m)
{
    string str;
    for(int i = 0; i < m->size; i++)
        str.push_back((char) m->data[i]);
    str.push_back('\0');
    return str;
}