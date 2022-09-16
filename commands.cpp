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
