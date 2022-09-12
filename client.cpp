
#include "api.h"
#include "message.h"
#include "commands.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

char * Interface;

bool Local = true;


vector<string> ls_opts = {
    "-l", "-a", "-la"
};


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

    while(1) {
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
            recv_stream(ofstream(), true);
            cout << ": ";
        }

        cin.clear();

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
                recv_stream(ofstream(), true);
            }
        }
        else if (str_op == "cd") {
            if (Local)
                execute_cd(param);
            else
                send_command(CD, param);
        }
        else if (str_op == "get") {
            if (param.length() > 0 && Local) {
                send_command(GET, param);
                recv_stream(ofstream(param, ios_base::app), false);
            }
        }
        else if (str_op == "put" && Local) {
            if (filesystem::exists(param)) {
                FILE * f = fopen(&param[0], "r");
                send_stream(f);
                fclose(f);
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