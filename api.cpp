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

using namespace std;

extern char * Interface;

int sockfd;

void sighandler(int sign)
{
    int t = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(int));
    cout << "interrupted\n";
    exit(0);
}

void init_con()
{
    sockfd = ConexaoRawSocket((char *) Interface);
    signal(SIGINT, sighandler);
}


Message * fetch_msg()
{
    Message * m = new Message;

    do {
        recv(sockfd, m, sizeof(Message), 0);
    } while (m->mark != MARKER || !m);

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
    if (send(
        sockfd,
        (const char *) msg,
        sizeof(Message),  
        0
    ) == -1) {
        perror("sendto: ");
    };
}