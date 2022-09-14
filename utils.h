#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <bits/stdc++.h>

// retorna o tamanho do arquivo em bytes
unsigned long long filesize(std::string filename);

// retorna o espaço em disco disponivel em bytes
unsigned long long available_space();

// abre um arquivo corretamente
FILE * open_file(std::string filepath);

#endif