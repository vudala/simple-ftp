#ifndef API_INCLUDED
#define API_INCLUDED

#include "message.h"

#include <bits/stdc++.h>

#define TIMEOUT_SECONDS 1

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

// recebe uma sequencia de conteudos de uma interface
std::string * recv_stream();

// envia o conteudo de stream para a interface
void send_stream(FILE * stream);

#endif