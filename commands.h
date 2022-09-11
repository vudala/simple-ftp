#ifndef COMMNADS_INCLUDED
#define COMMANDS_INCLUDED

#include "message.h"

void execute_ls(bool local, string param);

Message * execute_mkdir(string name);

Message * execute_cd(string path);

void send_file(string filename);

#endif