
#include "api.h"
#include "message.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <bits/stdc++.h>

using namespace std;

char * Target_Addr;

int main(int argc, char * argv[]) {
    Target_Addr = argv[1];

    init_con();

    string a = "teste123";

    Message * m = new_msg(a.length(), 0, 0, &a[0]);
    
    send_msg(m);
}