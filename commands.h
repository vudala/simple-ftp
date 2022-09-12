#ifndef COMMNADS_INCLUDED
#define COMMANDS_INCLUDED

#include "message.h"

#include <bits/stdc++.h>

void execute_ls(bool local, std::string param);

Message * execute_mkdir(std::string name);

Message * execute_cd(std::string path);

#endif