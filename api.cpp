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
        recv(sockfd, m, sizeof(Message), 0);
        if (errno == TIMED_OUT)
            return NULL;
    } while (m->mark != MARKER);
    alarm(0);

    return m;
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


void send_msg(Message * msg)
{
    if (send(sockfd, (const char *) msg, sizeof(Message), 0) == -1) {
        perror("send_msg: ");
        exit(-1);
    };
}