#include "api.h"

#include "socket.h"
#include "message.h"

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

bool Interrupted = false;

void inthandler(int sig)
{
    int t = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(int));
    cout << "interrupted\n";
    exit(0);
}


void alrmhandler(int sig)
{
    Interrupted = true;
}


void init_con()
{
    sockfd = ConexaoRawSocket((char *) Interface);

    struct sigaction action;
    action.sa_handler = alrmhandler;
    sigemptyset (&action.sa_mask) ;
    action.sa_flags = 0 ;
    sigaction (SIGALRM, &action, 0);

    action.sa_handler = inthandler;
    sigemptyset (&action.sa_mask) ;
    action.sa_flags = 0 ;
    sigaction (SIGINT, &action, 0);
}


Message * fetch_msg(bool tout)
{
    Message * m = new Message();

    Interrupted = false;

    if (tout)
        alarm(TIMEOUT_SECONDS);
    
    do {
        if (recv(sockfd, m, sizeof(Message), 0) == -1) {
            if (Interrupted)
                return NULL;
            perror("recv: ");
            exit(-1);
        };
            
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


void send_ok(unsigned seq)
{
    Message * m = new Message(0, seq, OK, NULL);
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
    } while(!answer || !valid_msg(answer) || msg->type == NACK || answer->seq != msg->seq);
}


Message * assert_recv(unsigned seq)
{
    Message * msg = fetch_msg(false);
    while(!valid_msg(msg)) {
        free(msg);
        send_nack(seq);
        msg = fetch_msg(false);
    }
    return msg;
}


void send_stream(FILE * stream)
{
    char buffer[64];
    Message * msg = NULL;
    unsigned seq = 0;
    int type = DADOS;

    char c;
    unsigned bytes_read;
    string str;
    while(type != FIM) {
        // le os bytes da stream
        bytes_read = 0;
        c = fgetc(stream);
        while(c != EOF && bytes_read < 64) {
            buffer[bytes_read] = c;
            bytes_read++;
            if (bytes_read < 64)
                c = fgetc(stream);
        }
        
        // se leu tudo, marca como fim
        if (feof(stream))
            type = FIM;

        // envia os dados
        msg = new Message(bytes_read - 1, seq, type, buffer);
        assert_send(msg);
        free(msg);

        seq = (seq + 1) % 16;
    }
}


void recv_stream(ofstream output, bool standard_out)
{
    Message * msg = NULL;
    unsigned last_seq = 1987654321;
    unsigned seq = 0;
    int status = DADOS;

    do {
        msg = assert_recv(seq);
        status = msg->type;

        if (msg->seq == seq) {
            for(int i = 0; i <= msg->size; i++) {
                if (standard_out)
                    cout << msg->data[i];
                else 
                    output << msg->data[i];
            }
            send_ack(seq);

            last_seq = seq;
            seq = (seq + 1) % 16;
        }
        // se recebeu a anterior, significa que o ack nao chegou no destino
        else if (msg->seq == last_seq)
            send_ack(last_seq);

        free(msg);
    } while(status != FIM);

    output.close();
}



void print_stream(FILE * stream)
{
    char c = fgetc(stream);
    while(c != EOF) {
        cout << c;
        c = fgetc(stream);
    }
}


void send_command(int opt, string param)
{
    // manda a mensagem e fica esperando uma resposta
    Message * msg;
    if (param.length() > 0)
        msg = new Message(param.length(), 0, opt, &param[0]);
    else
        msg = new Message(0, 0, opt, NULL);
        
    Message * answer = NULL;
    do {
        send_msg(msg);
        answer = fetch_msg(true);
    } while(!answer || !valid_msg(answer) || msg->type == NACK);
    free(msg);
    free(answer);
}
