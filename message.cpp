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
    memset(this->data, 0x0, 63);
    if (data)
        memcpy(&this->data, data, size);
    this->crc = _crc(this);
}

Mask::Mask() {}

Mask::Mask (Message * msg)
{
    this->mark = MARKER;
    this->size = msg->size;
    this->seq = msg->seq;
    this->type = msg->type;
    memset(this->data, 0x0, 504);
    for(int i = 0; i < msg->size; i++)
        this->data[i] = (unsigned long long) msg->data[i];
    this->crc = msg->crc;
}


Message::Message (Mask * mask)
{
    this->mark = MARKER;
    this->size = mask->size;
    this->seq = mask->seq;
    this->type = mask->type;
    memset(this->data, 0x0, 63);
    for(int i = 0; i < mask->size; i++)
        this->data[i] = (unsigned char) mask->data[i];
    this->crc = mask->crc;
}


bool valid_msg(Message * m)
{
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


Message * build_descriptor(unsigned long long filesize)
{
    char buff[8];
    memcpy(buff, &filesize, 8);
    Message * msg = new Message(4, 0, DESCRITOR, buff);
    return msg;
}