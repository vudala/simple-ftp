#include "commands.h"

#include "message.h"
#include "api.h"
#include "utils.h"

#include <stdio.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

Message * execute_cd(string path)
{
    errno = 0;
    int ret = chdir(&path[0]);
    if (ret == -1) {
        switch (errno) {
            case ENOENT:
                return new Message(23, 0, ERROR, (char*) "Diretorio inexistente\n");
            case EACCES:
                return new Message(26, 0, ERROR, (char*) "Sem permissao de leitura\n");
            case ELOOP:
                return new Message(20, 0, ERROR, (char*) "Muitos links simbolicos\n");
            case ENAMETOOLONG:
                return new Message(27, 0, ERROR, (char*) "Caminho muito longo\n");
        }
    }
    return new Message(0, 0, OK, NULL);
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
    return new Message(0, 0, OK, NULL);
}


void execute_get(string param)
{
    send_command(GET, param);
    Message * ans = assert_recv(0);
    // RECEBE DESCRIPTOR
    if (ans->type == DESCRITOR) {
        unsigned long long fsize;
        memcpy(&fsize, ans->data, 8);

        // se tem espaco pra receber o arquivo
        if (fsize <= available_space()) {
            ans = new Message(0, 0, OK, NULL);
            assert_send(ans);
            recv_stream(param, false);
        }
        else {
            cout << "Sem espaço suficiente para receber o arquivo HAHAHAHAHAHA\n" << flush;
            Message * ans = new Message(0, 0, ERROR, NULL);
            assert_send(ans);
            delete ans;
        }
    }
    else {
        cout << "Arquivo inexistente\n" << flush;
    }
}
