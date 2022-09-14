
#include "api.h"
#include "message.h"
#include "commands.h"
#include "utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>

using namespace std;

char * Interface;

bool Local = true;


vector<string> ls_opts = {
    "-l", "-a", "-la"
};


void print_path()
{
    if (Local) {
        cout << "\n[LOCAL] ";
        FILE * f = popen("echo -n $PWD", "r");
        print_stream(f);
        cout << ": " << flush;
        fclose(f);
    }
    else {
        cout << "\n[REMOTO] ";
        send_command(PWD, string());
        recv_stream(string(), true);
        cout << ": " << flush;
    }
}

void print_commands()
{
    cout << "\nComando disponiveis:\n";
    cout << "ls     { ,-l,-a,-la}\n";
    cout << "cd     {path}\n";
    cout << "get    {filename}\n";
    cout << "put    {filename}\n";
    cout << "mkdir  {dirname}\n";
    cout << "env    # troca de ambiente\n";
    cout << "close  # encerra o cliente\n" << flush;
}


int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    struct pollfd descriptors[2] = {
        {.fd = 0, .events = POLLIN, .revents = 0},
        {.fd = getsockfd(), .events = POLLIN, .revents = 0}
    };

    cout << "Bem vindo ao SimpleFTP do vudala HAHAHAHAHAHAHAHAHAH\n\n";
    print_commands();

    print_path();

    while(1) {
        poll(descriptors, 2, -1);
        if(descriptors[1].revents & POLLIN) {
            read_garbage();
        }
        else if(descriptors[0].revents & POLLIN) {

        string buff;
        getline(cin, buff);

        stringstream ss(buff);
        string str_op, param;
        ss >> str_op >> param;

        if (str_op == "ls") {
            if (Local) {
                string command = "ls ";
                if (find(ls_opts.begin(), ls_opts.end(), param) != ls_opts.end())
                    command.append(param);
                FILE * f = popen(&command[0], "r");
                print_stream(f);
                fclose(f);
            }
            else {
                send_command(LS, param);
                recv_stream(string(), true);
            }
        }
        else if (str_op == "cd") {
            if (Local) {
                Message * res = execute_cd(param);
                if (res->type == ERROR) {
                    cout << data_to_str(res) << flush;
                }
            }   
            else {
                send_command(CD, param);
                cout << "comando enviado\n" << flush;
                Message * ans = assert_recv(0);
                if (ans->type == ERROR)
                    cout << data_to_str(ans) << flush;
                cout << "comando executado\n" << flush;
            }
        }
        else if (str_op == "get") {
            execute_get(param);
        }
        else if (str_op == "put" && Local) {
            // se o arquiv existe
            if (filesystem::exists(param)) {
                // envia o tamanho dele pro servidor
                send_command(PUT, param);
                Message * tos = build_descriptor(filesize(param));

                assert_send(tos);
                delete tos;

                Message * ans = assert_recv(0);
                // se o servidor tem capacidade de receber o arquivo
                if (ans->type == OK) {
                    FILE * f = fopen(&param[0], "rb");
                    send_stream(f);
                    fclose(f);
                }
                // se nao envia dispara erro
                else {
                    cout << "Servidor nao tem espaço para receber o arquivo\n";
                }
            }
            else {
                cout << "Arquivo não existe\n" << flush;
            }
        }
        else if (str_op == "mkdir") {
            if (param.length() > 0) {
                if (Local) {
                    Message * res = execute_mkdir(param);
                    if (res->type == ERROR) {
                        cout << data_to_str(res) << flush;
                    }
                }   
                else {
                    send_command(MKDIR, param);
                    Message * ans = assert_recv(0);
                    if (ans->type == ERROR)
                        cout << data_to_str(ans) << flush;      
                }
            }
        }
        else if (str_op == "env") {
            Local = !Local;
        }
        else if (str_op == "close")
            exit(0);

        print_path();
        }
    }
}