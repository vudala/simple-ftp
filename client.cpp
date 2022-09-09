
#include "api.h"
#include "message.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

char * Interface;

tuple<int, string> get_op()
{
    string str_op, param;
    cin >> str_op;

    if (str_op == "ls")
        return {LS, NULL};
    
    if (str_op == "cd") {
        cin >> param;
        return {CD, param};
    }
    if (str_op == "get") {
        cin >> param;
        return {GET, param};
    }
    if (str_op == "put") {
        cin >> param;
        return {PUT, param};
    }
    if (str_op == "mkdir") {
        cin >> param;
        return {MKDIR, param};
    }
    if (str_op == "close")
        return {CLOSE, NULL};

    return {0, NULL};
}


int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    while(1) {
        auto [opt, param] = get_op();
        if (opt == 0) continue;

        // manda a mensagem e fica esperando uma resposta
        Message * msg = new Message(param.length(), 0, opt, &param[0]);
        Message * answer = NULL;
        do {
            send_msg(msg);
            answer = fetch_msg(true);
        } while(!answer || !valid_msg(answer) || msg->type == NACK);
        free(msg);

        FILE * f;
        switch (opt)
        {
        case LS:
            // lista os arquivos no server
            recv_stream(ofstream(), true);
            break;
        case CD:
            // muda de pasta no server, nao necesita acao no cliente
            break;
        case GET:
            // recebe um arquivo
            recv_stream(ofstream(param, ios_base::app), false);
            break;
        case PUT:
            // envia um arquivo para server
            f = fopen(&param[0], "r");
            send_stream(f);
            fclose(f);
            break;
        case MKDIR:
            // cria uma pasta no servidor, nao necesita acao no cliente
            break;
        case CLOSE:
            // encerra a conexao
            exit(0);
            break;
        default:
            // faz nada
            break;
        }
    }
}