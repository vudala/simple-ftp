
#include "api.h"
#include "message.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

char * Interface;


int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    // manda a mensagem e fica esperando uma resposta
    Message * msg = new Message(0, 0, LS, NULL);
    Message * answer = NULL;
    do {
        send_msg(msg);
        answer = fetch_msg(true);
    } while(!answer || !valid_msg(answer) || msg->type == NACK);

    string * str = recv_stream();

    cout << *str;
}