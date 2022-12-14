
#include "api.h"
#include "commands.h"
#include "utils.h"

#include <bits/stdc++.h>

using namespace std;

char * Interface;

int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    while(1) {
        // recebe um comando
        Message * msg = fetch_msg(false);
        while(!valid_msg(msg)) {
            delete msg;
            send_nack(0);
            msg = fetch_msg(false);
        }
        send_ack(0);
        
        // faz a operacao
        FILE *f, *p;
        string param;
        ofstream ofs;
        Message * result = NULL;
        Message * ans = NULL;
        switch (msg->type)
        {
        case LS:
            // lista os arquivos no server
            param = "ls ";
            param.append(data_to_str(msg));

            p = popen(&param[0], "r");
            send_stream(p);
            pclose(p);

            break;
        case CD:
            // muda de pasta no server
            param = data_to_str(msg);
            result = execute_cd(param);
            assert_send(result);
            delete result;
            break;
        case GET:
            cout << "get recebido\n" << flush;
            // envia um arquivo para o client
            param = data_to_str(msg);
            // verifica se o arquivo existe
            if (filesystem::exists(param)) {
                ans = build_descriptor(filesize(param));
                assert_send(ans);
                delete ans;

                ans = assert_recv(0);
                // se recebeu uma resposta positiva pra enviar
                if (ans->type == OK) {
                    f = open_file(param);
                    send_stream(f);
                    fclose(f);
                }
            }
            else {
                ans = new Message(19, 0, ERROR, (char *) "Arquivo nao existe");
                assert_send(ans);
                delete ans;
            }
            break;
        case PUT:
            result = assert_recv(0);
            if (result->type == DESCRITOR) {
                unsigned long long fsize;
                memcpy(&fsize, result->data, 8);
                
                if (fsize < available_space()) {
                    ans = new Message(0, 0, OK, NULL);
                    assert_send(ans);
                    delete ans;

                    param = data_to_str(msg);
                    recv_stream(param, false);
                }
                else {
                    ans = new Message(25, 0, ERROR, (char *) "Sem espa??o o suficiente");
                    assert_send(ans);
                    delete ans;
                }
            }
            break;
        case MKDIR:
            // cria uma pasta no servidor
            param = data_to_str(msg);
            result = execute_mkdir(param);
            assert_send(result);
            delete result;
            break;
        case PWD:
            // envia o diretorio atual
            p = popen("echo -n $PWD" , "r");
            send_stream(p);
            pclose(p);
            break;
        default:
            // faz nada
            break;
        }
    }
}