
#include "api.h"
#include "commands.h"

#include <bits/stdc++.h>


using namespace std;

char * Interface;

int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    while(1) {
        // recebe uma mensagem
        Message * msg = fetch_msg(false);
        while(!valid_msg(msg)) {
            free(msg);
            send_nack(msg->seq);
            msg = fetch_msg(false);
        }
        send_ok(msg->seq);
        
        // faz a operacao
        FILE * f;
        Message * result = NULL;
        switch (msg->type)
        {
        case LS:
            // lista os arquivos no server
            execute_ls();
            break;
        case CD:
            // muda de pasta no server
            break;
        case GET:
            // envia um arquivo para o client
            f = fopen(msg->data, "r");
            send_stream(f);
            fclose(f);
            break;
        case PUT:
            // recebe um arquivo do client
            recv_stream(ofstream(msg->data, ios_base::app), false);
            break;
        case MKDIR:
            // cria uma pasta no servidor
            result = execute_mkdir(msg->data);
            if (result)
                assert_send(result);
            break;
        case CLOSE:
            exit(0);
            break;
        default:
            // faz nada
            break;
        }
    }
}