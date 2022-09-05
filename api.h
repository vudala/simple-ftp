#ifndef API_INCLUDED
#define API_INCLUDED

#include "message.h"

#define TIMED_OUT 99
#define TIMEOUT_SECONDS 5

// inicializa estruturas
void init_con();

// recebe uma mensagem com opcao de timeout
Message * fetch_msg(bool tout);

// envia um ack para a interface
void send_ack(unsigned seq);

// envia um nack para a interface
void send_nack(unsigned seq);

// envia um mensagem para a interface
void send_msg(Message * msg);

#endif