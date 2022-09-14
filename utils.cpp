
#include <bits/stdc++.h>

using namespace std;

unsigned long long filesize(string filename)
{
    ifstream in(filename, ifstream::ate | ifstream::binary);
    unsigned long long ret = in.tellg();
    in.close();
    return ret;
}


unsigned long long available_space()
{
    FILE * p = popen("tmp=$(df .); echo $tmp | cut -d' ' -f11", "r");
    long res;
    fscanf(p, "%ld", &res);
    pclose(p);
    return (unsigned long long) res;
}


FILE * open_file(string filepath)
{
    FILE * f = fopen(&filepath[0], "rb");
    return f;
}