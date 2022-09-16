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
                return new Message(22, 0, ERROR, (char*) "Diretorio inexistente");
            case EACCES:
                return new Message(25, 0, ERROR, (char*) "Sem permissao de leitura");
            case ELOOP:
                return new Message(24, 0, ERROR, (char*) "Muitos links simbolicos");
            case ENAMETOOLONG:
                return new Message(20, 0, ERROR, (char*) "Caminho muito longo");
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
                return new Message(25, 0, ERROR, (char*) "Sem permissao de escrita");
            case EEXIST:
                return new Message(20, 0, ERROR, (char*) "Diretorio ja existe");
            case ENAMETOOLONG:
                return new Message(26, 0, ERROR, (char*) "Nome de pasta muito longo");
        }
    }
    return new Message(0, 0, OK, NULL);
}


void execute_get(string param)
{
    send_command(GET, param);

    Message * ans = assert_recv(0);
    // recebe descritor
    if (ans->type == DESCRITOR) {
        unsigned long long fsize;
        memcpy(&fsize, ans->data, 8);

        unsigned long long av = available_space();
        
        // se tem espaco pra receber o arquivo
        if (fsize <= av) {
            // informa que esta apto a receber e começa a requistar uma stream de dados
            ans = new Message(0, 0, OK, NULL);
            assert_send(ans);
            delete ans;

            cout << "[+] Recebendo arquivo\n" << flush;

            // comeca a receber a stream
            recv_stream(param, false);

            cout << "[+] Arquivo recebido\n" << flush;
        }
        // se nao tem, dispara um erro informando que o servidor nao deve enviar nada
        else {
            cout << "Sem espaço suficiente para receber o arquivo\n" << flush;
            ans = new Message(0, 0, ERROR, NULL);
            assert_send(ans);
            delete ans;
        }
    }
    // se recebeu um erro, o arquivo nao existe
    else {
        cout << data_to_str(ans) << flush;
    }
}
