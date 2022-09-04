
#include "api.h"
#include "message.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <bits/stdc++.h>

using namespace std;

char * Interface;

int main(int argc, char * argv[]) {
    Interface = argv[1];

    init_con();

    string a = "teste123";

    Message * m = new_msg(a.length(), 0, 0, &a[0]);
    
    send_msg(m);
}