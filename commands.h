#ifndef COMMNADS_INCLUDED
#define COMMANDS_INCLUDED

#include "message.h"

#include <bits/stdc++.h>

Message * execute_mkdir(std::string name);

Message * execute_cd(std::string path);

void execute_get(std::string param);

#endif