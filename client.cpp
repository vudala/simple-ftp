
#include "api.h"
#include "message.h"
#include "commands.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>

using namespace std;

char * Interface;

bool Local = true;


vector<string> ls_opts = {
    "-l", "-a", "-la"
};


void remove_garbage()
{
    Message * msg = new Message();

    recv(get_sockfd(), msg, sizeof(Message), 0);

    if(msg->mark != MARKER) {
        delete msg;
        return;
    }

    send_ack(msg->seq);
}

void print_path()
{
    if (Local) {
        cout << "\n[LOCAL] ";
        FILE * f = popen("echo -n $PWD", "r");
        print_stream(f);
        cout << ": ";
        fclose(f);
    }
    else {
        cout << "\n[REMOTO] ";
        send_command(PWD, string());
        recv_stream(string(), true);
        cout << ": ";
    }
}


int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    cout << "Bem vindo ao SimpleFTP do vudala\n\n";
    cout << "Comando disponiveis:\n";
    cout << "ls     { ,-l,-a,-la}\n";
    cout << "cd     {path}\n";
    cout << "get    {filename}\n";
    cout << "put    {filename}\n";
    cout << "mkdir  {dirname}\n";
    cout << "env    # troca de ambiente\n";
    cout << "close  # encerra o cliente\n";

    struct pollfd descriptors[2] = {
        {.fd = 0, .events = POLLIN, .revents = 0},
        {.fd = get_sockfd(), .events = POLLIN, .revents = 0}
    };


    while(1) {
        poll(descriptors, 2, -1);
        if (descriptors[1].revents & POLLIN) {
            remove_garbage();
        }
        else if (descriptors[0].revents & POLLIN) {
            print_path();

            cin.clear();

            // le entrada
            string buff;
            getline(cin, buff);

            stringstream ss(buff);
            string str_op, param;
            ss >> str_op >> param;

            Message * result;

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
                    result = execute_cd(param);
                    if (result) // caso tenha dado erro
                        cout << data_to_str(result);
                }
                else {
                    send_command(CD, param);
                    result = assert_recv(0);
                    send_ack(0);
                }
            }
            else if (str_op == "get") {
                if (param.length() > 0 && Local) {
                    send_command(GET, param);
                    recv_stream(param, false);
                }
            }
            else if (str_op == "put" && Local) {
                // se o arquivo nao existe nao envia
                if (filesystem::exists(param)) {
                    send_command(PUT, param);
                    FILE * f = fopen(&param[0], "rb");
                    send_stream(f);
                    fclose(f);
                }
                else {
                    cout << "Arquivo inexistente\n";
                }
            }
            else if (str_op == "mkdir") {
                if (param.length() > 0) {
                    if (Local)
                        execute_mkdir(&param[0]);
                    else
                        send_command(MKDIR, param);
                }
            }
            else if (str_op == "env") {
                Local = !Local;
            }
            else if (str_op == "close")
                exit(0);
        }
    }
}