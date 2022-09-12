#include "commands.h"

#include "message.h"
#include "api.h"

#include <stdio.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

void execute_ls(bool local, string param)
{
    string command = "ls ";
    command.append(param);
    FILE * f = popen(&command[0], "r");
    if (local)
        print_stream(f);
    else {
        send_command(LS, param);
        send_stream(f);
    }
        
    fclose(f);
}


void send_file(string filename)
{
    FILE * f = fopen(&filename[0], "r");
    send_stream(f);
    fclose(f);
}


Message * execute_cd(string path)
{
    errno = 0;
    int ret = chdir(&path[0]);
    if (ret == -1) {
        switch (errno) {
            case EACCES:
                return new Message(26, 0, ERROR, (char*) "Sem permissao de leitura\n");
            case ELOOP:
                return new Message(20, 0, ERROR, (char*) "Muitos links simbolicos\n");
            case ENAMETOOLONG:
                return new Message(27, 0, ERROR, (char*) "Caminho muito longo\n");
        }
    }
    return NULL;
}


Message * execute_mkdir(string name)
{
    errno = 0;
    int ret = mkdir(&name[0], S_IRWXU);
    if (ret == -1) {
        switch (errno) {
            case EACCES :
                return new Message(26, 0, ERROR, (char*) "Sem permissao de escrita\n");
            case EEXIST:
                return new Message(20, 0, ERROR, (char*) "Pasta ja existente\n");
            case ENAMETOOLONG:
                return new Message(27, 0, ERROR, (char*) "Nome de pasta muito longo\n");
        }
    }
    return NULL;
}
