#include "api.h"

#include "socket.h"
#include "message.h"
#include "utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <bits/stdc++.h>
#include <errno.h>
#include <unistd.h>

using namespace std;

extern char * Interface;

int sockfd;

void inthandler(int sig)
{
    int t = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(int));
    cout << "interrupted\n";
    exit(0);
}


void alrmhandler(int sig)
{
    errno = TIMED_OUT;
}


void init_con()
{
    sockfd = ConexaoRawSocket((char *) Interface);
    signal(SIGINT, inthandler);
    signal(SIGALRM, alrmhandler);
}


Message * fetch_msg(bool tout)
{
    Message * m = new Message();

    if (tout) {
        errno = 0;
        alarm(TIMEOUT_SECONDS);
    }
    do {
        if (recv(sockfd, m, sizeof(Message), 0) == -1) {
            perror("recv: ");
            exit(-1);
        };
        if (errno == TIMED_OUT)
            return NULL;
    } while (m->mark != MARKER);
    alarm(0);

    return m;
}


void send_ack(unsigned seq)
{
    Message * m = new Message(0, seq, ACK, NULL);
    send_msg(m);
    free(m);
}


void send_nack(unsigned seq)
{
    Message * m = new Message(0, seq, NACK, NULL);
    send_msg(m);
    free(m);
}


void send_ok()
{
    Message * m = new Message(0, 0, OK, NULL);
    send_msg(m);
    free(m);
}


void send_msg(Message * msg)
{
    if (send(sockfd, (const char *) msg, sizeof(Message), 0) == -1) {
        perror("send_msg: ");
        exit(-1);
    };
}


void assert_send(Message * msg)
{
    Message * answer = NULL;
    do {
        send_msg(msg);
        answer = fetch_msg(true);
    } while(!answer || !valid_msg(answer) || msg->type != ACK || answer->seq != msg->seq);
}


Message * assert_recv(unsigned int seq)
{
    Message * msg = fetch_msg(false);
    while(!valid_msg(msg) || msg->seq != seq) {
        free(msg);
        send_nack(seq);
        msg = fetch_msg(false);
    }
    send_ack(seq);

    return msg;
}
