#include "api.h"

#include "socket.h"
#include "message.h"
#include "utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <bits/stdc++.h>

using namespace std;

extern char * Target_Addr;
struct sockaddr_in Target;

int sockfd;

void init_con()
{
    sockfd = ConexaoRawSocket((char *) "lo");

    memset(&Target, 0, sizeof(struct sockaddr_in));
    Target.sin_family = AF_INET; // IPv4 
    inet_aton(Target_Addr, &Target.sin_addr);
}


Message * fetch_msg()
{
    Message * m = new Message;

    do {
        recv(sockfd, &m, sizeof(Message), 0);
    } while (m->mark != MARKER);

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
    sendto(
        sockfd,
        (const char *) msg,
        sizeof(Message),  
        MSG_CONFIRM,
        (const struct sockaddr *) &Target, 
        sizeof(Target)
    );
}