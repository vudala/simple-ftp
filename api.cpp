#include "api.h"

#include "socket.h"
#include "message.h"
#include "utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <bits/stdc++.h>

using namespace std;

int sockfd;

void init_con()
{
    sockfd = ConexaoRawSocket((char *) "lo");
}


Message * fetch_msg()
{
    Message * m = new Message;

    do {
        recv(sockfd, &m, sizeof(Message), 0);
    } while (m->mark != MARKER);

    return m;
}


void send_msg(Message * m)
{
    send(sockfd, m, sizeof(Message), 0);
}


void send_ack(unsigned seq)
{
    Message * m = new_msg(0, seq, ACK, NULL);
    send_msg(m);
    free(m);
}


void send_nack(unsigned seq)
{
    Message * m = new_msg(0, seq, NACK, NULL);
    send_msg(m);
    free(m);
}
