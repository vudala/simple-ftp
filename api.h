#ifndef API_INCLUDED
#define API_INCLUDED


#include "message.h"

Message * fetch_msg();

void init_con();

void send_ack(unsigned seq);

void send_nack(unsigned seq);

void send_msg(Message * msg);

#endif