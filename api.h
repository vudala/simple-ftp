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

// envia um ok para a interface
void send_ok();

// envia um mensagem para a interface
void send_msg(Message * msg);

// assegura que a mensagem foi enviada
void assert_send(Message * msg);

// assegura o recebimento de uma mensagem
Message * assert_recv(unsigned seq);

#endif