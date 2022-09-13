
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
    FILE * f = popen("tmp=$(df .); echo $tmp | cut -d' ' -f11", "r");
    long res;
    fscanf(f, "%ld", &res);
    fclose(f);
    return (unsigned long long) res;
}