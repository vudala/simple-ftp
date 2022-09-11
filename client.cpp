
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


int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    cout << filesystem::current_path() << ": ";

    cin.clear();
    cin.ignore(INT_MAX);

    string str_op, param;
    cin >> str_op >> param;

    if (str_op == "ls") {
        if (Local) {
            string command = "ls ";
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
    if (str_op == "cd") {
        if (Local)
            execute_cd(param);
        else {
            send_command(CD, param);
        }
    }
    if (str_op == "get") {
        send_command(GET, param);
        recv_stream(ofstream(param, ios_base::app), false);
    }
    if (str_op == "put") {
        send_command(PUT, param);
        send_file(&param[0]);
    }
    if (str_op == "mkdir") {
        if (Local) {
            Message * m = execute_mkdir(&param[0]);
            if (m)
                cout << string(m->data);
        }
        else {
            send_command(MKDIR, param);
            // recvok
        }
    }
    if (str_op == "env") {
        Local = !Local;
    }
    if (str_op == "close")
        exit(0);
}