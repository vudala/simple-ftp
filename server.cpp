
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
        send_ack(msg->seq);
        
        // faz a operacao
        FILE * f;
        Message * result = NULL;
        string command;
        ofstream ofs;
        switch (msg->type)
        {
        case LS:
            // lista os arquivos no server
            command = "ls ";
            command.append(data_to_str(msg));
            f = popen(&command[0], "r");
            send_stream(f);
            fclose(f);
            break;
        case CD:
            // muda de pasta no server
            command = data_to_str(msg);
            execute_cd(command);
            break;
        case GET:
            // envia um arquivo para o client
            command = data_to_str(msg);
            if (filesystem::exists(command)) {
                f = fopen(&command[0], "r");
                send_stream(f);
                fclose(f);
            }
            break;
        case PUT:
            // recebe um arquivo do client
            command = data_to_str(msg);
            if (!filesystem::exists(command))
                recv_stream(ofstream(command, ios::app | ios::out | ios::ate), true);
            break;
        case MKDIR:
            // cria uma pasta no servidor
            command = data_to_str(msg);
            execute_mkdir(command);
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