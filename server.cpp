
#include "api.h"
#include "commands.h"

#include <bits/stdc++.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

char * Interface;

int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    struct pollfd descriptors[2] = {
        {.fd = 0, .events = POLLIN, .revents = 0},
        {.fd = getsockfd(), .events = POLLIN, .revents = 0}
    };

    while(1) {
        poll(descriptors, 2, -1);
        if(descriptors[1].revents & POLLIN){
            read_garbage();
        }
        else if(descriptors[0].revents & POLLIN){
        // recebe uma mensagem
        Message * msg = fetch_msg(false);
        while(!valid_msg(msg)) {
            free(msg);
            send_nack(msg->seq);
            msg = fetch_msg(false);
        }
        send_ack(msg->seq);
        
        // faz a operacao
        FILE * f;
        string param;
        ofstream ofs;
        switch (msg->type)
        {
        case LS:
            // lista os arquivos no server
            param = "ls ";
            param.append(data_to_str(msg));
            f = popen(&param[0], "r");
            send_stream(f);
            fclose(f);
            break;
        case CD:
            // muda de pasta no server
            param = data_to_str(msg);
            execute_cd(param);
            break;
        case GET:
            // envia um arquivo para o client
            param = data_to_str(msg);
            if (filesystem::exists(param)) {
                f = fopen(&param[0], "rb");
                send_stream(f);
                fclose(f);
            }
            break;
        case PUT:
            // recebe um arquivo do client
            param = data_to_str(msg);
            if (!filesystem::exists(param))
                recv_stream(param, false);
            break;
        case MKDIR:
            // cria uma pasta no servidor
            param = data_to_str(msg);
            execute_mkdir(param);
            break;
        case PWD:
            // envia o diretorio atual
            f = popen("echo -n $PWD" , "r");
            send_stream(f);
            fclose(f);
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
}