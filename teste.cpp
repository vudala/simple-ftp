

#include <bits/stdc++.h>

#include "api.h"
using namespace std;

char * Interface;
int main(){
    FILE * f1 = fopen("capa.jpg", "rb");
    FILE * f2 = fopen("copia.jpg", "ab");
    int res;
    char buff[64];
    while(!feof(f1)) {
        res = fread(buff, 1, 64, f1);
        fwrite(buff, 1, res, f2);
    }
    fclose(f1);
    fclose(f2);
}