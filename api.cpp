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


int getsockfd()
{
    return sockfd;
}

Message * fetch_msg(bool tout)
{
    Mask * msk = new Mask();

    Interrupted = false;

    if (tout)
        alarm(TIMEOUT_SECONDS);
    
    do {
        if (recv(sockfd, msk, sizeof(Mask), 0) == -1) {
            if (Interrupted)
                return NULL;
            perror("recv: ");
            exit(-1);
        };
            
    } while (msk->mark != MARKER);

    alarm(0);

    Message * msg = new Message(msk);
    delete msk;
    return msg;
}


void send_ack(unsigned seq)
{
    Message * m = new Message(0, seq, ACK, NULL);
    send_msg(m);
    delete m;
}


void send_nack(unsigned seq)
{
    Message * m = new Message(0, seq, NACK, NULL);
    send_msg(m);
    delete m;
}


void send_ok(unsigned seq)
{
    Message * m = new Message(0, seq, OK, NULL);
    send_msg(m);
    delete m;
}


void send_msg(Message * msg)
{
    Mask * msk = new Mask(msg);
    if (send(sockfd, (const char *) msk, sizeof(Mask), 0) == -1) {
        perror("send_msg: ");
        exit(-1);
    };
    delete msk;
}


void assert_send(Message * msg)
{
    Message * answer = NULL;
    do {
        send_msg(msg);
        answer = fetch_msg(true);
    } while(!answer || !valid_msg(answer) || msg->type == NACK);
}


Message * assert_recv(unsigned seq)
{
    Message * msg = fetch_msg(false);
    while(!valid_msg(msg)) {
        delete msg;
        send_nack(seq);
        msg = fetch_msg(false);
    }
    return msg;
}


Message * send_window(queue<Message *>& window)
{
    Message * msg = NULL;
    Message * ans = NULL;
    do {
        for(int i = 0; i < window.size(); i++) {
            msg = window.front();
            window.pop();
            send_msg(msg);
        }
        ans = fetch_msg(true);
    } while(!ans);
    return ans;
}

void send_stream(FILE * stream)
{
    char buffer[63];
    Message * msg = NULL;
    unsigned seq = 0;
    int type = DADOS;
    unsigned bytes_read;

    long long sum = 0;
    string str;

    queue<Message *> to_send;
    queue<Message *> save;

    bool finish = false;
    while(!finish) {
        // le os bytes da stream
        while(to_send.size() < WINDOW_SIZE && type != FIM) {
            bytes_read = fread(buffer, 1, 63, stream);
            // se leu tudo, marca como fim
            if (feof(stream))
                type = FIM;

            msg = new Message(bytes_read, seq, type, buffer);
            to_send.push(msg);

            seq = (seq + 1) % 16;
        }
        
        save = to_send;
        Message * answer = send_window(to_send);

        while(answer->seq != save.front()->seq) {
            delete save.front();
            save.pop();
        }
        if (answer->type == ACK) {
            delete save.front();
            save.pop();
            if (type == FIM)
                finish = true;
        }
        delete answer;

        to_send = save;
    }
}


void recv_stream(string filename, bool standard_out)
{
    unsigned last_seq = 1987654321;
    unsigned seq = 0;
    int status = DADOS;

    FILE * f = NULL;
    if (!standard_out)
        f = fopen(&filename[0], "wb");

    struct cmp {bool operator()(Message * a, Message * b) {return a->seq == 15 && b->seq == 0 ? true : a->seq < b->seq;}};
    priority_queue<
        Message *,
        vector<Message *>,
        cmp
    > window;

    int last_valid = 0;
    bool finish = false;
    do {
        Message * msg = NULL;
        do {
            if (msg) window.push(msg);
            msg = fetch_msg(false);
        }
        while(window.size() < WINDOW_SIZE && msg->type == DADOS);

        do {
            msg = window.top();
            if (valid_msg(msg)) {
                if (standard_out)
                    cout << data_to_str(msg) << flush;
                else
                    fwrite(msg->data, 1, msg->size, f);
                last_valid = msg->seq;
                if (msg->type == FIM) {
                    finish = true;
                    break;
                }
            }
            else {
                send_nack(last_valid);
                break;
            }
            window.pop();
        } while(window.size() > 0);

        while(window.size() > 0) {
            delete window.top();
            window.pop();
        }
    } while(!finish);

    if (f)
        fclose(f);
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
        if (answer) delete answer;
        answer = fetch_msg(true);
    } while(!answer || !valid_msg(answer) || answer->type == NACK);
    
    delete msg;
    delete answer;
}


void read_garbage()
{
    Mask * msk = new Mask();
    recv(getsockfd(), msk, sizeof(Mask), 0);
    Message * msg = new Message(msk);
    if (msg->mark == MARKER) {
        if (!valid_msg(msg))
            send_nack(msg->seq);
        else
            send_ack(msg->seq);
    }
    delete msg;
    delete msk;
}
