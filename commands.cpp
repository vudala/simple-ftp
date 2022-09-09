#include "commands.h"

#include "message.h"
#include "api.h"

#include <stdio.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <errno.h>

using namespace std;

void execute_ls()
{
    FILE * f = popen("ls", "r");
    send_stream(f);
    fclose(f);
}


void execute_get(char * filename)
{
    FILE * f = fopen(filename, "r");
    send_stream(f);
    fclose(f);
}


Message * execute_mkdir(char * name)
{
    errno = 0;
    int ret = mkdir(name, S_IRWXU);
    if (ret == -1) {
        switch (errno) {
            case EACCES :
                return new Message(26, 0, ERROR, (char*) "Sem permissao de escrita\n");
            case EEXIST:
                return new Message(20, 0, ERROR, (char*) "Pasta ja existente\n");
            case ENAMETOOLONG:
                return new Message(27, 0, ERROR, (char*) "Nome de pasta muito longo\n");
            default:
                return NULL;
        }
    }
    return NULL;
}
