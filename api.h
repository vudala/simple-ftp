#ifndef API_INCLUDED
#define API_INCLUDED

#include "message.h"

#include <bits/stdc++.h>

#define WINDOW_SIZE 4
#define TIMEOUT_SECONDS 1

// inicializa estruturas
void init_con();

// retorna o sockfd
int getsockfd();

// recebe uma mensagem com opcao de timeout
Message * fetch_msg(bool tout);

// envia um ack para a interface
void send_ack(unsigned seq);

// envia um nack para a interface
void send_nack(unsigned seq);

// envia um ok para a interface
void send_ok(unsigned seq);

// envia um mensagem para a interface
void send_msg(Message * msg);

// assegura que a mensagem foi enviada
void assert_send(Message * msg);

// assegura o recebimento de uma mensagem
Message * assert_recv(unsigned seq);

// recebe uma sequencia de conteudos de uma interface
void recv_stream(std::string filename, bool standard_out);

// envia o conteudo de stream para a interface
void send_stream(FILE * stream);

// imprime o conteudo de um arquivo
void print_stream(FILE * stream);

// envia comando para o servidor
void send_command(int opt, std::string param);

// 
void read_garbage();

#endif