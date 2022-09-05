
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

    string data = "1987654321";

    // manda a mensagem e fica esperando uma resposta
    Message * msg = new Message(data.length(), 0, 0, &data[0]);
    Message * answer = NULL;
    do {
        send_msg(msg);
        answer = fetch_msg(true);
    } while(!answer);

    // resposta chega
    if (valid_msg(answer)) {
        // se recebeu um nack, envia a mensagem até ser aceita
        while(answer->type == NACK) {
            do {
                send_msg(msg);
                answer = fetch_msg(true);
            } while(!answer);
        }
    }
    
}